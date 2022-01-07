#ifndef OVEN_CHAMBER_H
#define OVEN_CHAMBER_H

#include "ControlledDevice.h"

#include <Arduino.h>

class OvenChamber : public ControlledDevice {
    private:
        const uint8_t chamberPin;

    public:
        OvenChamber(const uint8_t chamberPin);

        void setup();
        virtual void setValue( double value );
};


#endif