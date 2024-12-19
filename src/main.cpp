#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <SPIFFS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

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

constexpr int detectTreshold = 5; // in cm

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
const int pulseDelay = 500;

int IRLeftData;
int IRRightData;
int USDownData;
int USForwardData;

void readMacAddress() {
    uint8_t baseMac[6];
    esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
    if (ret == ESP_OK) {
        Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X\n",
                      baseMac[0], baseMac[1], baseMac[2],
                      baseMac[3], baseMac[4], baseMac[5]);
    } else {
        Serial.println("Failed to read MAC address");
    }
}

int USRead(int TrigPin, int EchoPin) {
    // Clears the trigPin
    digitalWrite(TrigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(TrigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin, LOW);

    // Reads the echoPin, returns the sound wave travel time in microseconds
    float duration = pulseIn(EchoPin, HIGH, 30000);

    if (duration == 0) {
        return -1;
    }

    // Calculate the distance
    float distanceCm = duration * 0.0343/2;


    return distanceCm;
}

String getMotorDirectionString();
void eventPulse();

TaskHandle_t eventPulseTask;
SemaphoreHandle_t dataMutex;


void setup() {
    Serial.begin(9600);
    // Serial.println("Mac Address:");
    // readMacAddress();

    dataMutex = xSemaphoreCreateMutex();
    if (dataMutex == NULL) {
        Serial.println("Failed to create mutex!");
    } else {
        Serial.println("Mutex created successfully.");
    }

    pinMode(USForwardTriggerPin, OUTPUT);
    pinMode(USForwardEchoPin , INPUT);
    pinMode(USDownTriggerPin, OUTPUT);
    pinMode(USDownEchoPin , INPUT);

    if (!SPIFFS.begin()) {
        Serial.println("An error occured while mounting SPIFFS, aborting!");
        return;
    }

    WiFi.softAP(AP_SSID, AP_PASSWORD);
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", String(), false);
    });

    // event handling
    events.onConnect([](AsyncEventSourceClient *client) {
        if (client->lastId()) {
            Serial.println("Client reconnected! Last message ID: " + String(client->lastId()));
        } else {
            Serial.println("New client connected!");
            events.send("connected", "open", millis());
        }
    });
    server.addHandler(&events);
    server.begin();

    // setup event source handler on seperate core
    xTaskCreatePinnedToCore(
        reinterpret_cast<TaskFunction_t>(eventPulse),
        "EventPulse",
        4096,
        nullptr,
        1,
        &eventPulseTask,
        0);
}

void loop() {
    Serial.println("Loop attempting to take mutex...");
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    Serial.println("Loop acquired mutex!");
    USDownData = USRead(USDownTriggerPin, USDownEchoPin);
    USForwardData = USRead(USForwardTriggerPin, USForwardEchoPin);
    IRLeftData = digitalRead(IRLeftPin);
    IRRightData = digitalRead(IRRightPin);
    xSemaphoreGive(dataMutex);
    Serial.println("Loop released mutex!");

    bool shouldCorrect = USForwardData < detectTreshold;
    if (shouldCorrect) {
        // if (IRLeftData)
    }


}

void eventPulse() {
    unsigned long x = millis();
    for (;;) {
        Serial.println("Event pulse is alive");
        if (millis() - lastPulseMS >= pulseDelay) {
            Serial.println("Event pulse attempting to take mutex...");
            xSemaphoreTake(dataMutex, portMAX_DELAY);
            Serial.println("Event pulse acquired mutex!");
            const int localIRLeftData = IRLeftData;
            const int localIRRightData = IRRightData;
            const int localUSDownData = USDownData;
            const int localUSForwardData = USForwardData;
            xSemaphoreGive(dataMutex);
            Serial.println("Event pulse released mutex!");

            Serial.println("Event pulse attempting to send data");
            events.send(String(localIRLeftData).c_str(), "IRLeft", millis(), pulseDelay);
            if (millis() - x > pulseDelay) {
                events.send(String(localIRRightData).c_str(), "IRRight", millis(), pulseDelay);
            }
            if (localUSDownData != -1) events.send(String(localUSDownData).c_str(), "USDown", millis(), pulseDelay);
            else events.send("N/A", "USDown", millis(), pulseDelay);
            if (localUSForwardData != -1) events.send(String(localUSForwardData).c_str(), "USForward", millis(), pulseDelay);
            else events.send("N/A", "USDown", millis(), pulseDelay);
            events.send(getMotorDirectionString().c_str(), "DIRECTION", millis(), pulseDelay);
            lastPulseMS = millis();
        }
        vTaskDelay(pdMS_TO_TICKS(100));
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