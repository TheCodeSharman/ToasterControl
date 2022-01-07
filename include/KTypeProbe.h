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
  parameters in order to avoid requiring inheritance, since no polymorhism is needed.


*/

#ifndef ADC_VREF
#define ADC_VREF 3300
#endif

typedef int32_t milliCelcius_t;

/*
  FIXME: 
  
  This assumes resolution of ADC is set to 12 - which may not be the case.
  Nothing else assumes this, we could add a new parameter to the template that 
  defines the ADC resolution. 
  
  But that seems unnecessary. 
  
  For that matter ADC_VREF is only used below, since it is irrelevant given the 
  calibration data.


*/

typedef struct {
  const int32_t tempOffsetA;
  const int32_t tempOffsetB;
  const uint32_t adcA;
  const uint32_t adcB;
} KProbeCalibration;

class Sensor {
  public:
    virtual double readSensor() = 0;
};

class KTypeProbe : public Sensor
{
public:
  KTypeProbe( const uint8_t probePin, KProbeCalibration& calibration )
    : probePin(probePin), calibration(calibration)
  {}

private:
  uint32_t overSampleRead(int N, uint32_t pin);
  const KProbeCalibration& calibration;
  milliCelcius_t coldJunction;
  uint32_t probeAdc;
  milliCelcius_t probeOffset;
  const uint8_t probePin;

public:
  milliCelcius_t  getColdJunction() { return coldJunction; }
  uint32_t        getProbeAdc() { return probeAdc; }
  milliCelcius_t  getProbeOffset() { return probeOffset; }
  milliCelcius_t  getTemperature() { return probeOffset + coldJunction; }

  virtual milliCelcius_t readColdJunction();
  virtual uint32_t readProbeAdc();
  virtual milliCelcius_t readTemperature();
  virtual double readSensor();
};

#endif