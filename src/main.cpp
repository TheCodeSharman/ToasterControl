#include <Arduino.h>

#include "MultiTask.h" 
#include "KTypeProbe.h"
#include "OvenChamber.h"
#include "CommandProcessor.h"

const uint8_t LED_PIN = PC13; 
const uint8_t K_PROBE_PIN = A2;
const uint8_t HEATER_PIN = PA15;

MultiTask tasks;

KTypeProbe probe(K_PROBE_PIN);
OvenChamber ovenChamber(HEATER_PIN);
PidController oven(probe,ovenChamber,tasks);
Settings settings(Serial,probe,oven);


CommandProcessor command(Serial,oven,probe,settings,tasks);

void blinkLed() {
  static bool isLedOn = false;
  digitalWrite(LED_PIN, (isLedOn?HIGH:LOW));
  isLedOn = !isLedOn;
}

void setup() {
  // Firstly load settings from EEPROM (emulated in flash here)
  // If the settings don't exist then factory settings will be stored.
  settings.loadSettings();

  probe.setup();
  ovenChamber.setup();
  oven.setOutputLimits(0,ovenChamber.getWindowSize());

  pinMode(LED_PIN, OUTPUT);  
  digitalWrite(LED_PIN, LOW);
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
  ovenChamber.process(); // operates at microseconds not milliseconds
  tasks.process();       
}