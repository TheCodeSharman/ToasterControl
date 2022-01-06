

#include <Arduino.h>

#include "MultiTask.h" 
#include "KTypeProbe.h"
#include "CommandProcessor.h"


#define ADC_VREF 3300

const uint8_t LED = PC13; 

KProbeCalibration K_PROBE_CAL = { -3, 104, 2022, 2716 };
KTypeProbe<A2,K_PROBE_CAL> probe;
MultiTask tasks;
CommandProcessor command(Serial);

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
  pinMode(PA15,OUTPUT);
  digitalWrite(PA15,HIGH);
  digitalWrite(LED, LOW);
  analogReadResolution(12);
  tasks.every(1000,displayProbeStatus);
  tasks.every(500,blinkLed);
  delay(500);
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
  probe.update();      
}