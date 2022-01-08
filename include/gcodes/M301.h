#ifndef M301_H
#define M301_H

#include "GCode.h"
#include "PidController.h"

/*
    Enters the system bootloader ready for firmware update.
    M997 is the code Marlin uses for the same.
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