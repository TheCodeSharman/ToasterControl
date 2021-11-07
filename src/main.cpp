#include <Arduino.h>
#include "stm32yyxx_ll_adc.h"

#define LED PC13
#define K_TYPE_PROBE A2
#define K_TYPE_PROBE_REF A1

// The Vref+ voltage supplied to the STM32 chip.
const int32_t ADC_VREF = 3300; 

// measured temperature difference from ambient
const int32_t CAL_TEMP_OFFSET_A = 26-29;
const int32_t CAL_TEMP_OFFSET_B = 134-30;

// corresponding measured ADC values
const int32_t CAL_ADC_A = 2022;
const int32_t CAL_ADC_B = 2716;

static int32_t readAverage(uint32_t pin) {
  int32_t value = 0;
  int32_t i;
  for(i=0; i<100; i++ ) {
    value += analogRead(pin);
  }
  return value/i;
}

/*
  In order to make the circuit simpler, we read the STM32 chip temperature sensor
  to get an estimate of the cold junction temperature. 
  
  For this to work we are assuming that there is a constant offset from the
  internal chip sensor and the cold junction.

  Certainly not a precision measurement, but saves us measuring the cold junction with 
  a thermister. The hope is that when the board is enclosed this offset will be
  sufficiently close to be practical.
*/
static int32_t readInternalTempSensor()
{
  return __LL_ADC_CALC_TEMPERATURE(ADC_VREF, readAverage(ATEMP), LL_ADC_RESOLUTION_12B);
}

static int32_t readKTypeProbeSensor() {

  int32_t die_temp = readInternalTempSensor();
  int32_t probe_val = readAverage(K_TYPE_PROBE);

  // Interpolate value based on calibration constants
  int32_t k_probe_temp_delta 
    = CAL_TEMP_OFFSET_A + 
      (CAL_TEMP_OFFSET_B - CAL_TEMP_OFFSET_A) *
      (probe_val-CAL_ADC_A)/(CAL_ADC_B-CAL_ADC_A);
  
  Serial.printf("die_temp = %i probe_val = %i k_probe_temp_delta = %i\n", die_temp, probe_val, k_probe_temp_delta);
  return k_probe_temp_delta + die_temp;
}

void setup() {
  pinMode(LED, OUTPUT);  
  digitalWrite(LED, LOW);
  analogReadResolution(12);
}

void loop() {
  digitalWrite(LED, LOW);
  
  Serial.printf("Probe Temp(°C) = %i\n", readKTypeProbeSensor());

  delay(500);   
  digitalWrite(LED, HIGH);  
  delay(500);         
}