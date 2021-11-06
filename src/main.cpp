#include <Arduino.h>
#include "stm32yyxx_ll_adc.h"

#define LED PC13
#define K_TYPE_PROBE A2
#define K_TYPE_PROBE_REF A1

// The Vref+ voltage supplied to the STM32 chip.
const int32_t ADC_VREF = 3300; 

// Ambient temperature offset from internal chip sensor.
// This is assumed to be constant - needs to be found experimentally.
const int32_t SELF_HEATING_OFFSET = 5;

// DC offset of ref pin on the instrument amplifier (units=mV).
const int32_t GND_REF = 1650;

// IN126 gain resister value (units=ohms).
const int32_t GAIN_RESISTER = 645; 

// Formula for gain from the IN126 datasheet.
const int32_t GAIN = 5 + 80000/GAIN_RESISTER;

/*
  In order to make the circuit simpler, we read the STM32 chip temperature sensor
  to get an estimate of the cold junction temperature. 
  
  For this to work we are assuming that there is a constant thermal resistance between 
  the microproccesor and the cold end of the K type probe. 

  Certainly not a precision measurement, but it seems accurate to a couple of degrees,
  so saves us measuring the cold junction with a thermister.
*/
static int32_t readInternalTempSensor()
{
  return __LL_ADC_CALC_TEMPERATURE(ADC_VREF, analogRead(ATEMP), LL_ADC_RESOLUTION_12B);
}

static int32_t readKTypeProbeSensor() {
  /* 1. Calculate the measured voltage on the ADC and subtract the ref voltage
        from this value to get the amplified voltage from the probe (units=mV). */
  int32_t k_probe_amplified = __LL_ADC_CALC_DATA_TO_VOLTAGE(ADC_VREF,
    analogRead(K_TYPE_PROBE),LL_ADC_RESOLUTION_12B) - GND_REF;

  /* 2. Convert this value into µV value measured at the K type probe (before 
        amplification). */
  int32_t k_probe_uv = k_probe_amplified*1000/GAIN; 

  /* 3. Using the linear polygon fit to the NIST K type probe data calculate
        the temperature delta from the cold junction to the hot junction. */
  int32_t k_probe_temp_delta = (k_probe_uv*24487)/1000 + 312;

  /* 4. Using the internal temperature sensor, convert to an
        absolute temperature. */
  int32_t die_temp = readInternalTempSensor();
  return k_probe_temp_delta/1000 + die_temp - SELF_HEATING_OFFSET;
}

void setup() {
  pinMode(LED, OUTPUT);  
  digitalWrite(LED, LOW);
  analogReadResolution(12);
}

void loop() {
  digitalWrite(LED, LOW);
  
  Serial.printf("Ambient Temp(°C) = %i ",readInternalTempSensor() - SELF_HEATING_OFFSET);
  Serial.printf("Probe Temp(°C) = %i\n", readKTypeProbeSensor());

  delay(500);   
  digitalWrite(LED, HIGH);  
  delay(500);         
}