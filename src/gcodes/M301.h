#ifndef M301_H
#define M301_H

#include "GCode.h"
#include "PidController.h"

/*
    Sets the PID parameters of the heater
*/
class M301 : public AbstractGCodeCommand {
    private:
        Stream& output;
        PidController& pid;
    public:
        M301(Stream& output, GCodeParser& GCode, PidController& pid) : output(output), pid(pid), AbstractGCodeCommand(GCode) {}
        virtual bool match();
        virtual void execute();
};

#endif