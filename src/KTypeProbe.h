#ifndef KTYPE_PROBE_H
#define KTYPE_PROBE_H

#include <Arduino.h>


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
      close to linear in the region -10°C to 400°C so this is reasonable for a
      rough reading.
*/
typedef int32_t milliCelcius_t;

class KTypeProbe
{

private:
  milliCelcius_t coldJunction;
  int32_t probeAdc;
  milliCelcius_t probeOffset;
  milliCelcius_t probe;

protected:
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

public:
  KTypeProbe(const uint8_t K_TYPE_PROBE, const int32_t ADC_VREF,
             const int32_t CAL_TEMP_OFFSET_A, const int32_t CAL_TEMP_OFFSET_B,
             const int32_t CAL_ADC_A, const int32_t CAL_ADC_B)
      : K_TYPE_PROBE(K_TYPE_PROBE),
        ADC_VREF(ADC_VREF),
        CAL_TEMP_OFFSET_A(CAL_TEMP_OFFSET_A), CAL_TEMP_OFFSET_B(CAL_TEMP_OFFSET_B),
        CAL_ADC_A(CAL_ADC_A), CAL_ADC_B(CAL_ADC_B)
  {
  }

  milliCelcius_t getColdJunction() const { return coldJunction; }
  milliCelcius_t getProbeOffset() const { return probeOffset; }
  milliCelcius_t getTemperature() const { return probe; }

  milliCelcius_t update();

protected:
  // over ride the following to customise how the probe reads it's temperature
  virtual milliCelcius_t readColdJunction();
  virtual int32_t readProbe();

};

#endif