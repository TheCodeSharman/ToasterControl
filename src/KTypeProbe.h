#ifndef KTYPE_PROBE_H
#define KTYPE_PROBE_H

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
      close to linear in the region -10°C to 400°C so this is reasonable for a
      rough reading.
      
  To use, instantiate the template with the required parameters to specify the 
  calibration constants. It is also possible to override the functions called to read
  the cold junction and the hot junction - this is implemented using default template
  parameters in order to avoid bloating the data type with.


*/
typedef int32_t milliCelcius_t;


uint32_t overSampleRead(int N, uint32_t pin);

template<uint32_t ADC_VREF>
milliCelcius_t defaultReadColdJunction(){
  return __LL_ADC_CALC_TEMPERATURE(ADC_VREF, overSampleRead(10, ATEMP), LL_ADC_RESOLUTION_12B) * 1000;
}

template<uint8_t K_TYPE_PROBE>
uint32_t defaultReadProbe(){
  return overSampleRead(10, K_TYPE_PROBE);
}

typedef struct {
  const int32_t tempOffsetA;
  const int32_t tempOffsetB;
  const uint32_t adcA;
  const uint32_t adcB;
} KProbeCalibration;

template<uint8_t K_TYPE_PROBE,
         uint32_t ADC_VREF,
         const KProbeCalibration& calibration,
         milliCelcius_t (*readColdJunction)() = defaultReadColdJunction<ADC_VREF>,
         uint32_t (*readProbe)() = defaultReadProbe<K_TYPE_PROBE>>

class KTypeProbe
{

private:
  milliCelcius_t coldJunction;
  uint32_t probeAdc;
  milliCelcius_t probeOffset;

public:
  milliCelcius_t  getColdJunction() const { return coldJunction; }
  uint32_t        getProbeAdc() const { return probeAdc; }
  milliCelcius_t  getProbeOffset() const { return probeOffset; }
  milliCelcius_t  getTemperature() const { return probeOffset + coldJunction; }

  void update() {
    coldJunction = readColdJunction();
    probeAdc = readProbe();
    probeOffset = calibration.tempOffsetA * 1000 +
                  (calibration.tempOffsetB - calibration.tempOffsetA) * 1000 *
                      (((int32_t)probeAdc) - (int32_t)calibration.adcA) / ((int32_t)calibration.adcB - (int32_t)calibration.adcA);
  };
};

#endif