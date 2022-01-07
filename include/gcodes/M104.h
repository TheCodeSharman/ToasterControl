#ifndef M104_H
#define M104_H

#include "GCode.h"
#include <Arduino.h>

class M104 : public AbstractGCodeCommand {
    private:
        Stream& output;
    public:
        M104(Stream& output, GCodeParser& GCode) 
            : output(output), AbstractGCodeCommand(GCode) {}
        virtual bool match();
        virtual void execute();
};

#endif