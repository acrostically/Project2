#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <SPIFFS.h>

// Connection Data for ESP
const char* AP_SSID = "GROEP_6_PROJECT_2";
const char* AP_PASSWORD = "KGADW&^54AWDKJ&R^";

#define US1EchoPin 19
#define US1TriggerPin 18

WiFiServer server(80);
String header;

String state;

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

int USRead() {
    // Clears the trigPin
    digitalWrite(US1TriggerPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(US1TriggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(US1TriggerPin, LOW);

    // Reads the echoPin, returns the sound wave travel time in microseconds
    float duration = pulseIn(US1EchoPin, HIGH);

    // Calculate the distance
    float distanceCm = duration * 0.0343/2;


    // Prints the distance in the Serial Monitor
    Serial.print("Distance (cm): ");
    Serial.println(distanceCm);
    delay(500);
    return distanceCm;
}

void setup() {
    Serial.begin(9600);
    // Serial.println("Mac Address:");
    // readMacAddress();

    pinMode(US1TriggerPin, OUTPUT);
    pinMode(US1EchoPin , INPUT);

    if (!SPIFFS.begin()) {
        Serial.println("An error occured while mounting SPIFFS, aborting!");
        return;
    }

    WiFi.softAP(AP_SSID, AP_PASSWORD);
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());

    server.begin();
}

void loop() {
    USRead();
    WiFiClient client = server.available();
    if (client) {
        Serial.println("HTTP Client Connected");
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                Serial.print(c);
                header += c;

                String path = "";

                if (header.indexOf("GET /test") >= 0) {
                    path = "/TEST";
                    Serial.println("TEST HIT");
                }
                path += ".html";

                if (c == '\n') {
                    File file = SPIFFS.open(path);

                    if (!file) {
                        client.println("HTTP/1.1 404 Not Found");
                        client.println("Content-Type: application/json");
                        client.println("Connection: close");
                        client.println();
                        break;
                    }

                    client.println(file.readString());
                    client.println();
                    break;
                }
            }
        }

        header = "";
        client.stop();
        Serial.println("HTTP Client Disconnected\n");
    }
}