#include <Arduino.h>

#include "Every.h"
#include "KTypeProbe.h"

const uint8_t LED = PC13;

KTypeProbe probe(A2,3300,-3,104,2022,2716);
Every executor;

void updateAndDisplayProbeStats() {
  Serial.printf("Probe Temp(°C) = %i\n", probe.getTemperature()/1000);
}

void blinkLed() {
  static bool isLedOn = false;
  digitalWrite(LED, (isLedOn?HIGH:LOW));
  isLedOn = !isLedOn;
}

void setup() {
  pinMode(LED, OUTPUT);  
  digitalWrite(LED, LOW);
  analogReadResolution(12);
  executor.every(1000,updateAndDisplayProbeStats);
  executor.every(500,blinkLed);
}

void loop() {
  executor.process();   
  probe.update();      
}