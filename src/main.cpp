#include <Arduino.h>
#include "stm32yyxx_ll_adc.h"

#if defined(STM32F4xx)
  #define VREFINT   1210
  #define ADC_RANGE 1024
#else
  #error Unsupported board: board needs to be STM32F4xx !
#endif

#define LED PC13
#define K_TYPE_PROBE A1
#define K_TYPE_PROBE_REF A2

// Ambient temperate offset from internal chip sensor.
// How accurate is this ??
#define SELF_HEATING_OFFSET (20-25)

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
  analogReadResolution(12);
}

void loop() {
  digitalWrite(LED, LOW);
  int32_t adc_v_ref = readVref();
  int32_t temperature = readTempSensor(adc_v_ref);
  int32_t k_probe_v_abs = analogRead(K_TYPE_PROBE);
  int32_t k_probe_v_ref = analogRead(K_TYPE_PROBE_REF);
  int32_t k_probe_v = k_probe_v_abs - k_probe_v_ref;
  Serial.printf("Ambient Temp(°C) = %i\n",temperature + SELF_HEATING_OFFSET);
  Serial.printf("K probe V = %i, (ref = %i, abs = %i, adc_v_ref = %i)\n",k_probe_v, k_probe_v_ref, k_probe_v_abs, adc_v_ref);
  delay(500);   
  digitalWrite(LED, HIGH);  
  delay(500);         
}