#ifndef Motor_H
#define Motor_H

#include <Arduino.h>

#define MotorInL1V 32 //Motor 1 L Voor
#define MotorInL1A 33 //Motor 1 L Achteruit
#define MotorInL2V 27//Motor 2 L Voor
#define MotorInL2A 14 //Motor 2 L Achteruit

#define MotorInR1V 25 //Motor 1 R Voor
#define MotorInR1A 26 //Motor 1 R Achteruit
#define MotorInR2V 12 //Motor 2 R Voor
#define MotorInR2A 13 //Motor 2 R Achteruit

// #define EnableL1  //Enable Motor 1 L
// #define EnableL2  //Enable Motor 2 L
// #define EnableR1  //Enable Motor 1 R
// #define EnableR2  //Enable Motor 2 R


void motorSetup(){
    pinMode(MotorInL1V, OUTPUT);
    pinMode(MotorInL1A, OUTPUT);
    pinMode(MotorInL2V, OUTPUT);
    pinMode(MotorInL2A, OUTPUT);
    pinMode(MotorInR1V, OUTPUT);
    pinMode(MotorInR1A, OUTPUT);
    pinMode(MotorInR2V, OUTPUT);
    pinMode(MotorInR2A, OUTPUT);

    // pinMode(EnableL1, OUTPUT);
    // pinMode(EnableL2, OUTPUT);
    // pinMode(EnableR1, OUTPUT);
    // pinMode(EnableR2, OUTPUT);
}

void gaVooruit() {
    //
    // //Enable pins
    // analogWrite(EnableL1, 180);
    // analogWrite (EnableL2, 180);
    // analogWrite (EnableR1, 180);
    // analogWrite (EnableR2, 180);
    
    //Motor pins vooruit
    analogWrite(MotorInL1V, 180);
    analogWrite(MotorInL2V, 180);
    analogWrite(MotorInR1V, 180);
    analogWrite(MotorInR2V, 180);
    
    //Motor pins achteruit
    analogWrite(MotorInL1A, LOW);
    analogWrite(MotorInR1A, LOW);
    analogWrite(MotorInL2A, LOW);
    analogWrite(MotorInR2A, LOW);
}

void gaAchteruit(){

    // // //Enable pins
    // analogWrite(EnableL1, 180);
    // analogWrite (EnableL2, 180);
    // analogWrite (EnableR1, 180);
    // analogWrite (EnableR2, 180);

    //Motor pins vooruit
    analogWrite(MotorInL1V, LOW);
    analogWrite(MotorInL2V, LOW);
    analogWrite(MotorInR1V, LOW);
    analogWrite(MotorInR2V, LOW);

    //Motor pins achteruit
    analogWrite(MotorInL1A, 180);
    analogWrite(MotorInR1A, 180);
    analogWrite(MotorInL2A, 180);
    analogWrite(MotorInR2A, 180);
}

void gaLinks(){
    // //Enable pins
    
    // analogWrite(EnableL1, 180);
    // analogWrite (EnableL2, 180);
    // analogWrite (EnableR1, 180);
    // analogWrite (EnableR2, 180);

    //Motor pins vooruit
    analogWrite(MotorInL1V, LOW);
    analogWrite(MotorInL2V, LOW);
    analogWrite(MotorInR1V, 180);
    analogWrite(MotorInR2V, 180);
    
    //Motor pins achteruit
    analogWrite(MotorInL1A, 180);
    analogWrite(MotorInR1A, 180);
    analogWrite(MotorInL2A, LOW);
    analogWrite(MotorInR2A, LOW);
}

void gaRechts(){

    // // //Enable pins
    // analogWrite(EnableL1, 180);
    // analogWrite (EnableL2, 180);
    // analogWrite (EnableR1, 180);
    // analogWrite (EnableR2, 180);

    //Motor pins vooruit
    analogWrite(MotorInL1V, 180);
    analogWrite(MotorInL2V, 180);
    analogWrite(MotorInR1V, LOW);
    analogWrite(MotorInR2V, LOW);
    
    //Motor pins achteruit
    analogWrite(MotorInL1A, LOW);
    analogWrite(MotorInR1A, LOW);
    analogWrite(MotorInL2A, 180);
    analogWrite(MotorInR2A, 180);
}

void stopMotoren(){

  // //Enable pins
    // analogWrite(EnableL1, 180);
    // analogWrite (EnableL2, 180);
    // analogWrite (EnableR1, 180);
    // analogWrite (EnableR2, 180);
    
    //Motor pins vooruit
    analogWrite(MotorInL1V, LOW);
    analogWrite(MotorInL2V, LOW);
    analogWrite(MotorInR1V, LOW);
    analogWrite(MotorInR2V, LOW);

    //Motor pins achteruit
    analogWrite(MotorInL1A, LOW);
    analogWrite(MotorInR1A, LOW);
    analogWrite(MotorInL2A, LOW);
    analogWrite(MotorInR2A, LOW);
}


#endif

/*
Wat moet er nog gebeuren:

- Pins voor de EnablePins moeten bepaald worden;
- PWM signaal moet bepaald worden op de EnablePins;
- Toepassen op de main code samen met het algoritme.

*/
