#include <Arduino.h>
#include "stm32yyxx_ll_adc.h"

#define LED PC13
#define K_TYPE_PROBE A2
#define K_TYPE_PROBE_REF A1

// The Vref+ voltage supplied to the STM32 chip.
const int32_t ADC_VREF = 3300; 

// DC offset of ref pin on the instrument amplifier (units=mV).
const int32_t GND_REF = 1650;
const int32_t CAL1 = 19; // calibrate for voltage offset
const int32_t CAL2 = 3200; // offset from reported ambient temperature

// IN126 gain resister value (units=ohms).
const int32_t GAIN_RESISTER = 645; 

// Formula for gain from the IN126 datasheet.
const int32_t GAIN = 125; //5 + 80000/GAIN_RESISTER;

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
  
  For this to work we are assuming that there is a constant thermal resistance between 
  the microproccesor and the cold end of the K type probe. 

  Certainly not a precision measurement, but saves us measuring the cold junction with 
  a thermister. The hope is that when the board is enclosed this offset will be
  sufficiently stable.
*/
static int32_t readInternalTempSensor()
{
  return __LL_ADC_CALC_TEMPERATURE(ADC_VREF, readAverage(ATEMP), LL_ADC_RESOLUTION_12B);
}

static int32_t readKTypeProbeSensor() {
  /* 1. Calculate the measured voltage on the ADC and subtract the ref voltage
        from this value to get the amplified voltage from the probe (units=mV). */
  int32_t k_probe_amplified = __LL_ADC_CALC_DATA_TO_VOLTAGE(ADC_VREF,
    readAverage(K_TYPE_PROBE),LL_ADC_RESOLUTION_12B) - GND_REF + CAL1;

  Serial.printf("\nk_probe_amplified = %i\n", k_probe_amplified);

  /* 2. Convert this value into µV value measured at the K type probe (before 
        amplification). */
  int32_t k_probe_uv = k_probe_amplified*1000/GAIN; 

  Serial.printf("k_probe_uv = %i\n", k_probe_uv);

  /* 3. Using the linear polygon fit to the NIST K type probe data calculate
        the temperature delta from the cold junction to the hot junction. */
  int32_t k_probe_temp_delta = (k_probe_uv*24487)/1000 + 312 - CAL2;

  /* 4. Using the internal temperature sensor, convert to an
        absolute temperature. */
  int32_t die_temp = readInternalTempSensor();
  Serial.printf("die_temp = %i\n", die_temp);
  return k_probe_temp_delta/1000 + die_temp;
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