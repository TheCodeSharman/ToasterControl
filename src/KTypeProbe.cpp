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