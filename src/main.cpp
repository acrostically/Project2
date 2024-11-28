#include <Arduino.h>
#include <WiFi.h>
#include <credentials.h> // Edit Wi-Fi credentials in `include/credentials.h` for your current network

void setup() {
    Serial.begin(9600);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // these are loaded from `credentials.h`
    Serial.print("\nConnecting to Wi-Fi...");
    while (WiFiClass::status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nSuccessfully connected to Wi-Fi!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop() {
// write your code here
}