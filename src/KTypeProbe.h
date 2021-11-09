#pragma once

#include <Arduino.h>
#include "stm32yyxx_ll_adc.h"

/*
  Very imprecise K-Type probe temperature sensing.

  Two major reasons this is imprecise:

  1.  In order to make the circuit simpler, we read the STM32 chip temperature sensor
      to get an estimate of the cold junction temperature. 
  
      For this to work we are assuming that there is a constant offset from the internal 
      chip sensor and the cold junction.

      Certainly not a precision measurement, but saves us measuring the cold junction with 
      a thermister. The hope is that when the board is enclosed this offset will be
      sufficiently close to be practical.

  2.  We assume, for simplicity, that the probe value is linear with respect to the
      temperature. This reduces cold junction compensation to a simply addition of
      temperature values. Of course K-Type probes are not linear, however, they are
      so close to linear in the region -10°C to 400°C so this is reasonable for a
      rough reading.
*/
typedef int32_t milliCelcius_t;

class KTypeProbe {

  private:
    milliCelcius_t coldJunction;
    int32_t probeAdc;
    milliCelcius_t probeOffset;
    milliCelcius_t probe;

  public:
    milliCelcius_t getColdJunction() const { return coldJunction; }
    milliCelcius_t getProbeOffset() const { return probeOffset; }
    milliCelcius_t getTemperature() const { return probe; }

  public:
    KTypeProbe(const uint8_t K_TYPE_PROBE, const int32_t ADC_VREF,
        const int32_t CAL_TEMP_OFFSET_A, const int32_t CAL_TEMP_OFFSET_B,
        const int32_t CAL_ADC_A, const int32_t CAL_ADC_B)
      : K_TYPE_PROBE(K_TYPE_PROBE),
        ADC_VREF(ADC_VREF),
        CAL_TEMP_OFFSET_A(CAL_TEMP_OFFSET_A), CAL_TEMP_OFFSET_B(CAL_TEMP_OFFSET_B),
        CAL_ADC_A(CAL_ADC_A), CAL_ADC_B(CAL_ADC_B)
    {}

  public:

    // Pin that is attached to the K Type probe
    const uint8_t K_TYPE_PROBE;

    // The Vref+ voltage supplied to the STM32 chip.
    const int32_t ADC_VREF; 

    // Measured temperature difference from ambient
    const int32_t CAL_TEMP_OFFSET_A;
    const int32_t CAL_TEMP_OFFSET_B;

    // Corresponding measured ADC values
    const int32_t CAL_ADC_A;
    const int32_t CAL_ADC_B;

  private:

    /*
      Simple over sampling, returns the mean of N samples of a pin.
    */
    uint32_t overSampleRead(int N, uint32_t pin) {
      uint32_t value = 0;
      int i;
      for(i=0; i<N; i++ ) {
        value += analogRead(pin);
      }
      return value/i;
    }

  protected:
    /* Read the cold junction temperature in milli °C */
    virtual milliCelcius_t readColdJunction() {
      return __LL_ADC_CALC_TEMPERATURE(ADC_VREF, overSampleRead(10,ATEMP), LL_ADC_RESOLUTION_12B) * 1000;
    }

    /* Read the value of the attached K Type probe in ADC units */
    virtual int32_t readProbe() {
      return overSampleRead(10,K_TYPE_PROBE);
    }

  public:
    milliCelcius_t update() {
        coldJunction = readColdJunction();
        probeAdc = readProbe();
        probeOffset 
          = CAL_TEMP_OFFSET_A * 1000 + 
            (CAL_TEMP_OFFSET_B - CAL_TEMP_OFFSET_A) * 1000 *
            (probeAdc-CAL_ADC_A)/(CAL_ADC_B-CAL_ADC_A);
        probe = probeOffset + coldJunction;
        return probe;
    };
};