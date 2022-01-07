#ifndef M105_H
#define M105_H

#include <Arduino.h>

#include "GCode.h"
#include "KTypeProbe.h"
#include "PidController.h"

/*
    Reports the temperature of the oven
*/
class M105 : public AbstractGCodeCommand {
    private:
        PidController& oven;
        Stream& output;
    public:
        M105(Stream& output, GCodeParser& GCode, PidController& oven) 
            : output(output), AbstractGCodeCommand(GCode), oven(oven) {}
        virtual bool match();
        virtual void execute();
};

#endif