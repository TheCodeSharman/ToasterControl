#include "OvenChamber.h"

#include <Arduino.h>

OvenChamber::OvenChamber(const uint8_t chamberPin) : chamberPin(chamberPin) 
{}

void OvenChamber::setup() {
        pinMode(chamberPin,OUTPUT);
        analogWrite(chamberPin,0);
    }

void OvenChamber::setValue( double value ) {
    //Serial.printf("OvenChamber::setValue(%f)\r\n", value);
    analogWrite(chamberPin, value);
}