#ifndef IRSensor_H
#define IRSensor_H

#include <Arduino.h>

#define IRSensorFront 1
#define IRSensorLeft 2
#define IRSensorRight 3

void IRSetup() {
pinMode(IRSensorFront, INPUT);
pinMode(IRSensorLeft,  INPUT);
pinMode(IRSensorRight, INPUT);
}

 void readSensors(bool &frontState, bool &leftState, bool &rightState){
    frontState  = digitalRead(IRSensorFront);
    leftState   = digitalRead(IRSensorLeft);
    rightState  = digitalRead(IRSensorRight);
 }

#endif