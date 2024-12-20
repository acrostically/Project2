#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <SPIFFS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "Motor.h"

// Connection Data for ESP
const char* AP_SSID = "GROEP_6_PROJECT_2";
const char* AP_PASSWORD = "KGADW&^54AWDKJ&R^";

constexpr int USForwardEchoPin = 17;
constexpr int USForwardTriggerPin = 5;
constexpr int USDownEchoPin = 19;
constexpr int USDownTriggerPin = 18;

constexpr int IRLeftPin = 16;
constexpr int IRBackPin = 4;
constexpr int IRRightPin = 0;

constexpr int detectThreshold = 5; // in cm

AsyncWebServer server(80);
AsyncEventSource events("/events");

enum class Direction {
    FORWARD,
    BACKWARD,
    STOP
};
Direction carDirection = Direction::STOP;

// event update frequency in MS for UI
unsigned long lastPulseMS = 0;
constexpr int pulseDelay = 500;

int IRLeftData;
int IRRightData;
int USDownData;
int USForwardData;

int USRead(const int TrigPin, const int EchoPin) {
    // Clears the trigPin
    digitalWrite(TrigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(TrigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin, LOW);

    // Reads the echoPin, returns the sound wave travel time in microseconds
    const unsigned long duration = pulseIn(EchoPin, HIGH, 30000);

    if (duration == 0) {
        return -1;
    }

    // return distance in CM
    return (duration * 0.0343 / 2);
}

String getMotorDirectionString();
void eventPulse();

TaskHandle_t eventPulseTask;
SemaphoreHandle_t dataMutex;

bool enabled = false;

void setup() {
    Serial.begin(9600);

    dataMutex = xSemaphoreCreateMutex();
    if (dataMutex == nullptr) {
        Serial.println("Failed to create mutex!");
        return;
    }
    Serial.println("Mutex created successfully.");

    pinMode(USForwardTriggerPin, OUTPUT);
    pinMode(USForwardEchoPin , INPUT);
    pinMode(USDownTriggerPin, OUTPUT);
    pinMode(USDownEchoPin , INPUT);

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

    server.on("/toggle", HTTP_POST, [](AsyncWebServerRequest *request) {
        enabled = !enabled;
        Serial.print("ACM is now ");
        Serial.println(enabled ? "ENABLED" : "DISABLED");

        request->send(200, "text/plain", enabled ? "true" : "false");
    });

    // event handling
    events.onConnect([](const AsyncEventSourceClient *client) {
        if (client->lastId()) {
            Serial.println("Client reconnected! Last message ID: " + String(client->lastId()));
        } else {
            Serial.println("New client connected!");
            events.send(String(enabled).c_str(), "open", millis());
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
}

void loop() {
    if (!enabled) return;

    xSemaphoreTake(dataMutex, portMAX_DELAY);
    USDownData = USRead(USDownTriggerPin, USDownEchoPin);
    USForwardData = USRead(USForwardTriggerPin, USForwardEchoPin);
    IRLeftData = digitalRead(IRLeftPin);
    IRRightData = digitalRead(IRRightPin);
    xSemaphoreGive(dataMutex);

    bool shouldCorrect = USForwardData < detectThreshold;
    if (shouldCorrect) {
        // if (IRLeftData)
    }


}

void eventPulse() {
    for (;;) {
        if (millis() - lastPulseMS >= pulseDelay) {
            xSemaphoreTake(dataMutex, portMAX_DELAY);
            const int localIRLeftData = IRLeftData;
            const int localIRRightData = IRRightData;
            const int localUSDownData = USDownData;
            const int localUSForwardData = USForwardData;
            xSemaphoreGive(dataMutex);

            String JSON = "{";
            JSON += "\"IRLeftData\":" + String(localIRLeftData) + ",";
            JSON += "\"IRRightData\":" + String(localIRRightData) + ",";
            JSON += "\"USDownData\":" + String(localUSDownData) + ",";
            JSON += "\"USForwardData\":" + String(localUSForwardData) + ",";
            JSON += "\"direction\": \"" + getMotorDirectionString() + "\"";
            JSON += "}";

            events.send(JSON.c_str(), "pulse", millis());

            lastPulseMS = millis();
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

String getMotorDirectionString() {
    switch (carDirection) {
        case Direction::FORWARD:
            return "FORWARD";
        case Direction::BACKWARD:
            return "BACKWARD";
        case Direction::STOP:
            return "STOP";
        default:
            return "UNKNOWN";
    }
}