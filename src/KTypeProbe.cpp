#include "KTypeProbe.h"
#include "stm32yyxx_ll_adc.h"

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

milliCelcius_t KTypeProbe::readColdJunction()
{
    return __LL_ADC_CALC_TEMPERATURE(ADC_VREF, overSampleRead(10, ATEMP), LL_ADC_RESOLUTION_12B) * 1000;
}

/* Read the value of the attached K Type probe in ADC units */
int32_t KTypeProbe::readProbe()
{
    return overSampleRead(10, K_TYPE_PROBE);
}

milliCelcius_t KTypeProbe::update()
{
    coldJunction = readColdJunction();
    probeAdc = readProbe();
    probeOffset = CAL_TEMP_OFFSET_A * 1000 +
                  (CAL_TEMP_OFFSET_B - CAL_TEMP_OFFSET_A) * 1000 *
                      (probeAdc - CAL_ADC_A) / (CAL_ADC_B - CAL_ADC_A);
    probe = probeOffset + coldJunction;
    return probe;
};