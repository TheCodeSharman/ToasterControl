#include "OvenChamber.h"

#include <Arduino.h>

OvenChamber::OvenChamber(const uint8_t chamberPin) : chamberPin(chamberPin), outputValue(0) 
{}

void OvenChamber::setup() {
        pinMode(chamberPin,OUTPUT);
        digitalWrite(chamberPin,LOW);
        windowStartTime = micros();
}

void OvenChamber::process() {
    int elapsed = micros() - windowStartTime;
    
    // move to next window if outside current window
    if ( elapsed > (windowSize*100) ) {
        windowStartTime = micros();
        elapsed = windowSize;
    }

    // if we are in the ON part of the cycle turn on the heater
    if ( outputValue*100 >= elapsed ) {
        digitalWrite(chamberPin, HIGH);
    } else {
        digitalWrite(chamberPin, LOW);
    }
    
}

void OvenChamber::setValue( double value ) {
    outputValue = value;
}