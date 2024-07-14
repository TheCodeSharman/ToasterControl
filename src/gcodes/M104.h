#ifndef M104_H
#define M104_H

#include <Arduino.h>

#include "GCode.h"
#include "PidController.h"

/*
    Sets the temperature of the oven
*/
class M104 : public AbstractGCodeCommand {
    private:
        Stream& output;
        PidController& oven;
    public:
        M104(Stream& output, GCodeParser& GCode, PidController& oven) 
            : output(output), AbstractGCodeCommand(GCode), oven(oven) {}
        virtual bool match();
        virtual void execute();
};

#endif