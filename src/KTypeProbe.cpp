#include "KTypeProbe.h"


KTypeProbe::KTypeProbe( const uint8_t probePin )
    : probePin(probePin)
{
    KProbeCalibration defaultCal;
    setProbeCalibration(defaultCal);
}

void KTypeProbe::setup() {
    analogReadResolution(12);
    pinMode(probePin, INPUT); 
}

/*
    Simple over sampling, returns the mean of N samples of a pin.
*/
uint32_t KTypeProbe::overSampleRead(int N, uint32_t pin) {
    uint32_t value = 0;
    int i;
    for(i=0; i<N; i++ ) {
        value += analogRead(pin);
    }
    return value/i;
}

milliCelcius_t KTypeProbe::readColdJunction(){
    // Calculate internal temperature in millidegrees, this is unrealistic precision
    // but we want to avoid rounding errors in subsequent calculations.
    //
    // We don't use __LL_ADC_CALC_TEMPERATURE because it is integer precions degrees.
    //return __LL_ADC_CALC_TEMPERATURE(ADC_VREF, overSampleRead(100, ATEMP), LL_ADC_RESOLUTION_12B) * 1000;
    int32_t T = (int32_t)overSampleRead(100, ATEMP);
    T = T - (int32_t)*TEMPSENSOR_CAL1_ADDR;
    T = T * (TEMPSENSOR_CAL2_TEMP - TEMPSENSOR_CAL1_TEMP);

    // multiply by 1000 to add three extra digits of fixed point precison - the result is now milidegrees
    T = (T*1000) / ((int32_t)( (int32_t)*TEMPSENSOR_CAL2_ADDR - (int32_t)*TEMPSENSOR_CAL1_ADDR));
    T = T + TEMPSENSOR_CAL1_TEMP*1000;
    
    return  T;
}

uint32_t KTypeProbe::readProbeAdc(){
  return overSampleRead(100, probePin);
}

milliCelcius_t KTypeProbe::readTemperature() {
    coldJunction = readColdJunction();
    probeAdc = readProbeAdc();
    probeOffset = calibration.tempOffsetA * 1000.0 +
                  (calibration.tempOffsetB - calibration.tempOffsetA) * 1000.0 *
                      (((int32_t)probeAdc) - (int32_t)calibration.adcA) / ((int32_t)calibration.adcB - (int32_t)calibration.adcA);
    return getTemperature();
}

double KTypeProbe::readSensor() {
    return ((double)readTemperature())/1000.0;
}