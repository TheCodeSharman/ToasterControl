#include <Arduino.h>
#include "KTypeProbe.h"

#define LED PC13

KTypeProbe probe(A2,3300,-3,104,2022,2716);

void setup() {
  pinMode(LED, OUTPUT);  
  digitalWrite(LED, LOW);
  analogReadResolution(12);
}

void loop() {
  digitalWrite(LED, LOW);
  probe.update();
  Serial.printf("Probe Temp(°C) = %i\n", probe.getTemperature()/1000);

  delay(500);   
  digitalWrite(LED, HIGH);  
  delay(500);         
}