#include <Arduino.h>
#include "stm32yyxx_ll_adc.h"

#if defined(STM32F4xx)
  #define VREFINT   1210
  #define ADC_RANGE 4096
#else
  #error Unsupported board: board needs to be STM32F4xx !
#endif

#define LED PC13
#define K_TYPE_PROBE A2
#define K_TYPE_PROBE_REF A1

// Ambient temperate offset from internal chip sensor.
// How accurate is this ??
#define SELF_HEATING_OFFSET (20-25)

static int32_t readVref()
{
  int32_t a_vref = analogRead(AVREF);
  return (VREFINT * ADC_RANGE / a_vref); // ADC sample to mV
} 

static int32_t readTempSensor(int32_t VRef)
{
  int32_t a_temp = analogRead(ATEMP);
  return (__LL_ADC_CALC_TEMPERATURE(VRef, a_temp, LL_ADC_RESOLUTION_12B));
}

void setup() {
  pinMode(LED, OUTPUT);  
  digitalWrite(LED, LOW);
  analogReadResolution(12);
}

void loop() {
  digitalWrite(LED, LOW);
  int32_t adc_v_ref = 3300; //readVref();
  int32_t temperature = readTempSensor(adc_v_ref);
  
  
  int32_t k_probe_v_abs = analogRead(K_TYPE_PROBE);
  int32_t k_probe_v_ref = 4096/2; //analogRead(K_TYPE_PROBE_REF);
  int32_t k_probe_v = k_probe_v_abs - k_probe_v_ref;

  
  const int32_t gain = 5 + 80000/645;
  // 24.487*Vprobe + 0.312
  // adc_vref= volts on Vref+ pin = 3300
  // => adc_vref/4096 is volts per unit on ADC 
  float k_probe_mv = ((float)k_probe_v)*(adc_v_ref/1000)/4096;
  k_probe_mv = k_probe_mv*1000/gain;
  float k_probe_temp = k_probe_mv*24.487 + 0.312; 
  float k_probe_temp_abs = k_probe_temp + temperature + SELF_HEATING_OFFSET;
  
  Serial.printf("\nAmbient Temp(°C) = %i\n",temperature + SELF_HEATING_OFFSET);

  Serial.printf("K probe delta Temp(°C) =");
  Serial.print(k_probe_temp);
  Serial.printf(" V = %i, (ref = %i, abs = %i, adc_v_ref = %i)\n",k_probe_v, k_probe_v_ref, k_probe_v_abs, adc_v_ref);
  Serial.print("Probe absolute Temp(°C) = ");
  Serial.print( k_probe_temp_abs);
  delay(500);   
  digitalWrite(LED, HIGH);  
  delay(500);         
}