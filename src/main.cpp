#include <Arduino.h>

#include "MultiTask.h" 
#include "KTypeProbe.h"
#include "OvenChamber.h"
#include "CommandProcessor.h"

const uint8_t LED_PIN = PC13; 
const uint8_t K_PROBE_PIN = A2;
const uint8_t HEATER_PIN = PA15;

KTypeProbe probe(K_PROBE_PIN);
OvenChamber ovenChamber(HEATER_PIN);
PidController oven( probe,ovenChamber);
Settings settings(Serial,probe,oven);

CommandProcessor command(Serial,oven,probe,settings);

MultiTask tasks;

void blinkLed() {
  static bool isLedOn = false;
  digitalWrite(LED_PIN, (isLedOn?HIGH:LOW));
  isLedOn = !isLedOn;
}

void processPidControllers() {
  oven.process();
  ovenChamber.process();
}
void reportDebug() {
  Serial.printf("input = %f output = %f\r\n", oven.getInput(), oven.getOutput() );
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
  tasks.every(10,processPidControllers);
  //tasks.every(1000,reportDebug);
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