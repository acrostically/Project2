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
    // digitalWrite(EnableL1, HIGH);
    // digitalWrite (EnableL2, HIGH);
    // digitalWrite (EnableR1, HIGH);
    // digitalWrite (EnableR2, HIGH);
    //
    //Motor pins vooruit
    digitalWrite(MotorInL1V, HIGH);
    digitalWrite(MotorInL2V, HIGH);
    digitalWrite(MotorInR1V, HIGH);
    digitalWrite(MotorInR2V, HIGH);
    
    //Motor pins achteruit
    digitalWrite(MotorInL1A, LOW);
    digitalWrite(MotorInR1A, LOW);
    digitalWrite(MotorInL2A, LOW);
    digitalWrite(MotorInR2A, LOW);
}

void gaAchteruit(){

    // digitalWrite (EnableL1, HIGH);
    // digitalWrite (EnableL2, HIGH);
    // digitalWrite (EnableR1, HIGH);
    // digitalWrite (EnableR2, HIGH);

    //Motor pins vooruit
    digitalWrite(MotorInL1V, LOW);
    digitalWrite(MotorInL2V, LOW);
    digitalWrite(MotorInR1V, LOW);
    digitalWrite(MotorInR2V, LOW);

    //Motor pins achteruit
    digitalWrite(MotorInL1A, HIGH);
    digitalWrite(MotorInR1A, HIGH);
    digitalWrite(MotorInL2A, HIGH);
    digitalWrite(MotorInR2A, HIGH);
}

void gaLinks(){
    //
    // digitalWrite (EnableL1, HIGH);
    // digitalWrite (EnableL2, HIGH);
    // digitalWrite (EnableR1, HIGH);
    // digitalWrite (EnableR2, HIGH);
    //
    // //Motor pins vooruit
    // digitalWrite(MotorInL1V, LOW);
    // digitalWrite(MotorInL2V, LOW);
    // digitalWrite(MotorInR1V, HIGH);
    // digitalWrite(MotorInR2V, HIGH);
    //
    // //Motor pins achteruit
    // digitalWrite(MotorInL1A, HIGH);
    // digitalWrite(MotorInR1A, HIGH);
    // digitalWrite(MotorInL2A, LOW);
    // digitalWrite(MotorInR2A, LOW);
}

void gaRechts(){

    // digitalWrite (EnableL1, HIGH);
    // digitalWrite (EnableL2, HIGH);
    // digitalWrite (EnableR1, HIGH);
    // digitalWrite (EnableR2, HIGH);
    //
    // //Motor pins vooruit
    // digitalWrite(MotorInL1V, HIGH);
    // digitalWrite(MotorInL2V, HIGH);
    // digitalWrite(MotorInR1V, LOW);
    // digitalWrite(MotorInR2V, LOW);
    //
    // //Motor pins achteruit
    // digitalWrite(MotorInL1A, LOW);
    // digitalWrite(MotorInR1A, LOW);
    // digitalWrite(MotorInL2A, HIGH);
    // digitalWrite(MotorInR2A, HIGH);
}

#endif
