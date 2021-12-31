#include "KTypeProbe.h"
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

milliCelcius_t defaultReadColdJunction(){
  return __LL_ADC_CALC_TEMPERATURE(ADC_VREF, overSampleRead(10, ATEMP), LL_ADC_RESOLUTION_12B) * 1000;
}