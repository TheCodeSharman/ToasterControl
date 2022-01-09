#ifndef M303_H
#define M303_H

#include <Arduino.h>

#include "GCode.h"
#include "PidController.h"

/*
    PID autotune
*/
class M303 : public AbstractGCodeCommand {
    private:
        Stream& output;
        PidController& oven;
    public:
        M303(Stream& output, GCodeParser& GCode, PidController& oven) 
            : output(output), AbstractGCodeCommand(GCode), oven(oven) {}
        virtual bool match();
        virtual void execute();
};

#endif