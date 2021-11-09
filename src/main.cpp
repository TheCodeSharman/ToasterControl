#include <Arduino.h>

#include "MultiTask.h" 
#include "KTypeProbe.h"

const uint8_t LED = PC13;

KProbeCalibration K_PROBE_CAL = { -3, 104, 2022, 2716 };
KTypeProbe<A2,3300,K_PROBE_CAL> probe;
MultiTask tasks;

void displayProbeStatus() {
  Serial.printf("Probe Temp(°C) = %i (coldJunction = %i, probeAdc = %u)\n", 
    probe.getTemperature()/1000, probe.getColdJunction()/1000, probe.getProbeAdc() );
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
  
  tasks.every(1000,displayProbeStatus);
  tasks.every(500,blinkLed);
}

void loop() {
  tasks.process();   
  probe.update();      
}