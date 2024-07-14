#ifndef M306_H
#define M306_H

#include "GCode.h"
#include "KTypeProbe.h"

/*
    Sets the calibration parameters of temperature sensor:
        M305 A-3 B2022 C104 D2200
    
    A: the first test temperature 
    B: the ADC value measured at this temperature
    C: the second test temperature
    D: the ADC value measured at this temperature
*/
class M305 : public AbstractGCodeCommand {
    private:
        Stream& output;
        KTypeProbe& probe;
    public:
        M305(Stream& output, GCodeParser& GCode, KTypeProbe& probe) : output(output), probe(probe), AbstractGCodeCommand(GCode) {}
        virtual bool match();
        virtual void execute();
};

#endif