#include "OvenChamber.h"

#include <Arduino.h>

OvenChamber::OvenChamber(const uint8_t chamberPin) : chamberPin(chamberPin), outputValue(0) 
{}

void OvenChamber::setup() {
        pinMode(chamberPin,OUTPUT);
        digitalWrite(chamberPin,LOW);
        windowStartTime = millis();
}

void OvenChamber::process() {
    int elapsed = millis() - windowStartTime;
    
    // move to next window if outside current window
    if ( elapsed > windowSize ) {
        windowStartTime = millis();
        elapsed = windowSize;
    }

    // if we are in the ON part of the cycle turn on the heater
    if ( outputValue >= elapsed ) {
        digitalWrite(chamberPin, HIGH);
    } else {
        digitalWrite(chamberPin, LOW);
    }
    
}

void OvenChamber::setValue( double value ) {
    outputValue = value;
}