#ifndef OVEN_CHAMBER_H
#define OVEN_CHAMBER_H

#include "ControlledDevice.h"

#include <Arduino.h>

class OvenChamber : public ControlledDevice {
    private:
        const uint8_t chamberPin;
        const int windowSize = 500;
        double outputValue;
        unsigned long windowStartTime;

    public:
        OvenChamber(const uint8_t chamberPin);
        int getWindowSize() { return windowSize; }
        void setup();
        void process();
        virtual void setValue( double value );
};


#endif