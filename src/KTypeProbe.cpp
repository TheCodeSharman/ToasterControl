#include "KTypeProbe.h"


KTypeProbe::KTypeProbe( const uint8_t probePin, KProbeCalibration& calibration )
    : probePin(probePin), calibration(calibration)
{

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
  return __LL_ADC_CALC_TEMPERATURE(ADC_VREF, overSampleRead(10, ATEMP), LL_ADC_RESOLUTION_12B) * 1000;
}

uint32_t KTypeProbe::readProbeAdc(){
  return overSampleRead(10, probePin);
}

milliCelcius_t KTypeProbe::readTemperature() {
    coldJunction = readColdJunction();
    probeAdc = readProbeAdc();
    probeOffset = calibration.tempOffsetA * 1000 +
                  (calibration.tempOffsetB - calibration.tempOffsetA) * 1000 *
                      (((int32_t)probeAdc) - (int32_t)calibration.adcA) / ((int32_t)calibration.adcB - (int32_t)calibration.adcA);
    return getTemperature();
}

double KTypeProbe::readSensor() {
    return ((double)readTemperature())/1000.0;
}