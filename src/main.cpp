#include <Arduino.h>
#include "stm32yyxx_ll_adc.h"

#if defined(STM32F4xx)
  #define VREFINT   1210
  #define ADC_RANGE 1024
#else
  #error Unsupported board: board needs to be STM32F4xx !
#endif

#define LED PC13

static int32_t readVref()
{
  return (VREFINT * ADC_RANGE / analogRead(AVREF)); // ADC sample to mV
} 

static int32_t readTempSensor(int32_t VRef)
{
  return (__LL_ADC_CALC_TEMPERATURE(VRef, analogRead(ATEMP), LL_ADC_RESOLUTION_10B));
}

void setup() {
  pinMode(LED, OUTPUT);  
  digitalWrite(LED, LOW);
  analogReadResolution(10);
}

void loop() {
  int32_t vref = readVref();
  Serial.printf("Vref = %i", vref);
  int32_t temperature = readTempSensor(vref);
  Serial.printf("\tTemp(°C) = %i\n",temperature);
  delay(1000);              
}