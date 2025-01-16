#ifndef Motor_H
#define Motor_H

#include <Arduino.h>

// voorkant motor pins
#define MOTOR_VOORKANT_LINKS_VOORUIT 32 //Motor L Vooruit
#define MOTOR_VOORKANT_LINKS_ACHTERUIT 33 //Motor L Achteruit
#define MOTOR_VOORKANT_RECHTS_VOORUIT 25 //Motor R Vooruit
#define MOTOR_VOORKANT_RECHTS_ACHTERUIT 26 //Motor R Achteruit

// achterkant motor pins
#define MOTOR_ACHTERKANT_LINKS_VOORUIT 12 //Motor L Vooruit
#define MOTOR_ACHTERKANT_LINKS_ACHTERUIT 13 //Motor L Achteruit
#define MOTOR_ACHTERKANT_RECHTS_VOORUIT 27 //Motor R Vooruit
#define MOTOR_ACHTERKANT_RECHTS_ACHTERUIT 14 //Motor R Achteruit

#define MOTOR_ENABLE_LINKS_VOOR 12 //Enable Motor 1 L
#define MOTOR_ENABLE_LINKS_ACHTER 13 //Enable Motor 2 L
#define MOTOR_ENABLE_RECHTS_VOOR 23 //Enable Motor 1 R
#define MOTOR_ENABLE_RECHTS_ACHTER 22 //Enable Motor 2 R


void motorSetup(){
    pinMode(MOTOR_VOORKANT_LINKS_VOORUIT, OUTPUT);
    pinMode(MOTOR_VOORKANT_LINKS_ACHTERUIT, OUTPUT);
    pinMode(MOTOR_ACHTERKANT_LINKS_VOORUIT, OUTPUT);
    pinMode(MOTOR_ACHTERKANT_LINKS_ACHTERUIT, OUTPUT);
    pinMode(MOTOR_VOORKANT_RECHTS_VOORUIT, OUTPUT);
    pinMode(MOTOR_VOORKANT_RECHTS_ACHTERUIT, OUTPUT);
    pinMode(MOTOR_ACHTERKANT_RECHTS_VOORUIT, OUTPUT);
    pinMode(MOTOR_ACHTERKANT_RECHTS_ACHTERUIT, OUTPUT);

    pinMode(MOTOR_ENABLE_LINKS_VOOR, OUTPUT);
    pinMode(MOTOR_ENABLE_LINKS_ACHTER, OUTPUT);
    pinMode(MOTOR_ENABLE_RECHTS_VOOR, OUTPUT);
    pinMode(MOTOR_ENABLE_RECHTS_ACHTER, OUTPUT);
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
    digitalWrite(MOTOR_VOORKANT_LINKS_VOORUIT, HIGH);
    digitalWrite(MOTOR_ACHTERKANT_LINKS_VOORUIT, HIGH);
    digitalWrite(MOTOR_VOORKANT_RECHTS_VOORUIT, HIGH);
    digitalWrite(MOTOR_ACHTERKANT_RECHTS_VOORUIT, HIGH);
    //
    // //Motor pins achteruit
    // digitalWrite(MotorInL1A, LOW);
    // digitalWrite(MotorInR1A, LOW);
    // digitalWrite(MotorInL2A, LOW);
    // digitalWrite(MotorInR2A, LOW);
}

void gaAchteruit(){

    digitalWrite (MOTOR_ENABLE_LINKS_VOOR, HIGH);
    digitalWrite (MOTOR_ENABLE_LINKS_ACHTER, HIGH);
    digitalWrite (MOTOR_ENABLE_RECHTS_VOOR, HIGH);
    digitalWrite (MOTOR_ENABLE_RECHTS_ACHTER, HIGH);

    //Motor pins vooruit
    digitalWrite(MOTOR_VOORKANT_LINKS_VOORUIT, LOW);
    digitalWrite(MOTOR_ACHTERKANT_LINKS_VOORUIT, LOW);
    digitalWrite(MOTOR_VOORKANT_RECHTS_VOORUIT, LOW);
    digitalWrite(MOTOR_ACHTERKANT_RECHTS_VOORUIT, LOW);

    //Motor pins achteruit
    digitalWrite(MOTOR_VOORKANT_LINKS_ACHTERUIT, HIGH);
    digitalWrite(MOTOR_VOORKANT_RECHTS_ACHTERUIT, HIGH);
    digitalWrite(MOTOR_ACHTERKANT_LINKS_ACHTERUIT, HIGH);
    digitalWrite(MOTOR_ACHTERKANT_RECHTS_ACHTERUIT, HIGH);
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

void stopMotor(){
    // digitalWrite (EnableL1, LOW);
    // digitalWrite (EnableL2, LOW);
    // digitalWrite (EnableR1, LOW);
    // digitalWrite (EnableR2, LOW);
    //
    // //Motor pins vooruit
    // digitalWrite(MotorInL1V, LOW);
    // digitalWrite(MotorInL2V, LOW);
    // digitalWrite(MotorInR1V, LOW);
    // digitalWrite(MotorInR2V, LOW);
    //
    // //Motor pins achteruit
    // digitalWrite(MotorInL1A, LOW);
    // digitalWrite(MotorInR1A, LOW);
    // digitalWrite(MotorInL2A, LOW);
    // digitalWrite(MotorInR2A, LOW);
})

#endif
