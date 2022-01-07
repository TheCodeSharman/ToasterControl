#include <Arduino.h>

#include "MultiTask.h" 
#include "KTypeProbe.h"
#include "OvenChamber.h"
#include "CommandProcessor.h"

const uint8_t LED_PIN = PC13; 
const uint8_t K_PROBE_PIN = A2;
const uint8_t HEATER_PIN = PA15;

KProbeCalibration K_PROBE_CAL = { -3, 104, 2022, 2716 };
KTypeProbe probe(K_PROBE_PIN,K_PROBE_CAL);
OvenChamber ovenChamber(HEATER_PIN);
PidController oven( probe, ovenChamber, 2, 5, 1 );

CommandProcessor command(Serial,oven);

MultiTask tasks;

void blinkLed() {
  static bool isLedOn = false;
  digitalWrite(LED_PIN, (isLedOn?HIGH:LOW));
  isLedOn = !isLedOn;
}

void processPidControllers() {
  oven.process();
}

void setup() {
  probe.setup();
  ovenChamber.setup();

  pinMode(LED_PIN, OUTPUT);  
  digitalWrite(LED_PIN, LOW);
  tasks.every(500,blinkLed);
  tasks.every(100,processPidControllers);
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
}