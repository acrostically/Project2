#include <Arduino.h>

void setup(){
    pinMode (12, INPUT);
    Serial.begin(9600);


}

bool startStatusIR;
bool AfgelezenStatusIR;

void loop(){
    
    startStatusIR = digitalRead(12);

    if (startStatusIR != AfgelezenStatusIR)
    {
        Serial.println(startStatusIR);
        AfgelezenStatusIR == startStatusIR;
    }
    else{
        Serial.println("0");
    }
    
}