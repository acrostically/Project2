//
// Created by lukas on 1/9/2025.
//

#ifndef SENSOREN_H
#define SENSOREN_H

#include <Arduino.h>

int IRLeftPin = 2;
int IRRightPin = 0;

int USForwardEchoPin = 17;
int USForwardTriggerPin = 5;
int USDownEchoPin = 19;
int USDownTriggerPin = 18;

int GetUSDistance(const int TrigPin, const int EchoPin) {
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

int USRead(int treshhold) {
  int res = 0b00;

  if (GetUSDistance(USForwardTriggerPin, USForwardEchoPin) < treshhold) {
    res |= 0b10;
  }
  if (GetUSDistance(USDownTriggerPin, USDownEchoPin) < treshhold) {
    res |= 0b01;
  }

  return res;
}

int IRRead() {
  int res = 0b00;

  res |= !digitalRead(IRLeftPin) << 1;
  res |= !digitalRead(IRRightPin);

  return res;
};

#endif //SENSOREN_H
