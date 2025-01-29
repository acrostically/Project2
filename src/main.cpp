#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <SPIFFS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "Motor.h"
#include "Sensoren.h"

// Connection Data for ESP
const char* AP_SSID = "GROEP_6_PROJECT_2";
const char* AP_PASSWORD = "KGADW&^54AWDKJ&R^";

constexpr int detectThreshold = 5; // in cm

bool carEnabled = false;

AsyncWebServer server(80);
AsyncEventSource events("/events");

enum class Direction {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    STOP
};
Direction carDirection = Direction::STOP;

// event update frequency in MS for UI
unsigned long lastPulseMS = 0;
constexpr int pulseDelay = 300;

int IRData;
int USData;


unsigned long lastMS = 0;

String getMotorDirectionString();
void eventPulse();

TaskHandle_t eventPulseTask;
SemaphoreHandle_t dataMutex;

void setup() {
    Serial.begin(115200);

    dataMutex = xSemaphoreCreateMutex();
    if (dataMutex == nullptr) {
        Serial.println("Failed to create mutex!");
        return;
    }
    Serial.println("Mutex created successfully.");

    setupSensors();

    if (!SPIFFS.begin()) {
        Serial.println("An error occured while mounting SPIFFS, aborting!");
        return;
    }
    Serial.println("SPIFFS initialized successfully.");

    WiFi.softAP(AP_SSID, AP_PASSWORD);
    Serial.println("AccessPoint initialized succesfully!");
    Serial.println("AP SSID: " + String(AP_SSID));
    Serial.println("AP PASSWORD: " + String(AP_PASSWORD));

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", String(), false);
    });

    server.on("/toggle", HTTP_GET, [](AsyncWebServerRequest *request) {
        carEnabled = !carEnabled; // Toggle the value of carEnabled
        String response = carEnabled ? "true" : "false"; // Convert to string for the response
        request->send(200, "text/plain", response); // Send the response
        Serial.print("TOGGLED CAR TO ");
        Serial.println(carEnabled ? "ON" : "OFF");
    });

    // event handling
    events.onConnect([](const AsyncEventSourceClient *client) {
        if (client->lastId()) {
            Serial.println("Client reconnected! Last message ID: " + String(client->lastId()));
        } else {
            Serial.println("New client connected!");
            events.send("connected", "open", millis());
        }
    });
    server.addHandler(&events);
    server.begin();
    Serial.print("HTTP server started! Load dashboard on ");
    Serial.println(WiFi.softAPIP());

    // setup event source handler on separate core
    xTaskCreatePinnedToCore(
        reinterpret_cast<TaskFunction_t>(eventPulse),
        "EventPulse",
        4096,
        nullptr,
        1,
        &eventPulseTask,
        0);
    Serial.println("Server Sent Events loaded on core 0!");

    Serial.println("Starting main loop!\n");
    motorSetup();
}

unsigned long correctionStartTime = 0;
constexpr unsigned long reverseDuration = 750;  // Move backward for 1500ms
bool forwardDetectCache = false;
bool isCorrecting = false;
Direction lastCorrection = Direction::RIGHT; // don't know why we load this with right, it just felt... right

void loop() {
    if (!carEnabled) {
        if (carDirection != Direction::STOP) {
            carDirection = Direction::STOP;
            stopMotoren();
        }
    };

    if (digitalRead(35)) {
        carDirection = Direction::STOP;
        stopMotoren();
        return;
    }

    xSemaphoreTake(dataMutex, portMAX_DELAY);
    USData = USRead(7);
    IRData = IRRead();
    xSemaphoreGive(dataMutex);

    // Serial.println("US" + String(USData));

    // ALWAYS stop ASAP if we detect something in front of us, DO NOT DELAY this check
    bool usforward = (USData & 0b10) >> 1;
    bool usdownward = (USData & 0b01);

    if (((!usdownward && IRData == 3) || usforward) && !forwardDetectCache) {
        carDirection = Direction::BACKWARD;
        gaAchteruit();
        forwardDetectCache = true;
        isCorrecting = true;
        correctionStartTime = millis();

        return;
    }

    if (isCorrecting) {
        if (millis() - correctionStartTime >= reverseDuration) {
            isCorrecting = false;  // Done reversing, move to correction
        } else {
            return;  // Keep reversing until time is up
        }
    }

    // if we detect something in front of us, we should correct
    if (forwardDetectCache) {
        switch (lastCorrection) { // we should always correct in the opposite direction we last corrected to prevent getting stuck
            case Direction::LEFT:
                carDirection = Direction::RIGHT;
                correctionStartTime = millis() - 500;
                gaRechts();
                lastCorrection = Direction::RIGHT;
                break;
            case Direction::RIGHT:
                carDirection = Direction::LEFT;
                correctionStartTime = millis();
                gaLinks();
                lastCorrection = Direction::LEFT;
                break;
            default: break;
        }
        isCorrecting = true;
        forwardDetectCache = false;
    }



    // if we detect something on the left, we should go right
    else if ((IRData & 0b10)) {
        carDirection = Direction::RIGHT;
        gaRechts();
        isCorrecting = true;
        correctionStartTime = millis() - 500;
    }

    // if we detect something on the right, we should go left
    else if ((IRData & 0b01)) {
        carDirection = Direction::LEFT;
        gaLinks();
        isCorrecting = true;
        correctionStartTime = millis();
    }

    // if we're not going forward and not detecting anything, we should go forward
    else if (carDirection != Direction::FORWARD) {
        carDirection = Direction::FORWARD;
        gaVooruit();
        isCorrecting = true;
        correctionStartTime = millis();
    };
}

void eventPulse() {
    unsigned long x = millis();
    for (;;) {
        if (millis() - lastPulseMS >= pulseDelay) {
            xSemaphoreTake(dataMutex, portMAX_DELAY);
            const bool localCarEnabled = carEnabled;
            const int localIRData = IRData;
            const int localUSData = USData;
            xSemaphoreGive(dataMutex);

            String JSON = "{";
            JSON += "\"ON\": " + String(localCarEnabled) + ",";
            JSON += "\"IRData\": " + String(localIRData) + ",";
            JSON += "\"USData\": " + String(localUSData) + ",";
            JSON += "\"Direction\": \"" + getMotorDirectionString() + "\"";
            JSON += "}";

            events.send(JSON.c_str(), "pulse", millis(), pulseDelay);
            lastPulseMS = millis();
        }
        vTaskDelay(100);
    }
}

String getMotorDirectionString() {
    switch (carDirection) {
        case Direction::FORWARD:
            return "FORWARD";
        case Direction::BACKWARD:
            return "BACKWARD";
        case Direction::LEFT:
            return "LEFT";
        case Direction::RIGHT:
            return "RIGHT";
        case Direction::STOP:
            return "STOP";
        default:
            return "UNKNOWN";
    }
}