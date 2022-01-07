#include <Arduino.h>

#include "MultiTask.h" 
#include "KTypeProbe.h"
#include "CommandProcessor.h"

const uint8_t LED_PIN = PC13; 
const uint8_t K_PROBE_PIN = A2;
const uint8_t HEATER_PIN = PA15;

KProbeCalibration K_PROBE_CAL = { -3, 104, 2022, 2716 };
KTypeProbe probe(K_PROBE_PIN,K_PROBE_CAL);
MultiTask tasks;
CommandProcessor command(Serial);

void displayProbeStatus() {
  Serial.printf("Probe Temp(°C) = %i (coldJunction = %i, probeAdc = %u)\n", 
    probe.getTemperature()/1000, probe.getColdJunction()/1000, probe.getProbeAdc() );
}

void blinkLed() {
  static bool isLedOn = false;
  digitalWrite(LED_PIN, (isLedOn?HIGH:LOW));
  isLedOn = !isLedOn;
}

void setup() {
  analogReadResolution(12);

  pinMode(LED_PIN, OUTPUT);  
  digitalWrite(LED_PIN, LOW);

  pinMode(HEATER_PIN,OUTPUT);
  digitalWrite(HEATER_PIN,HIGH);
  
  
  //tasks.every(1000,displayProbeStatus);
  tasks.every(500,blinkLed);
  command.init();
}

void processSerial() {
  while ( Serial.available() > 0 ) {
      command.addByte( Serial.read() );
  }
}

void loop() {
  processSerial();
  tasks.process();   
  probe.readTemperature();      
}