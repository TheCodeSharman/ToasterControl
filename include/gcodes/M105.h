#ifndef M105_H
#define M105_H

#include "GCode.h"
#include <Arduino.h>

class M105 : public AbstractGCodeCommand {
    private:
        Stream& output;
    public:
        M105(Stream& output, GCodeParser& GCode) 
            : output(output), AbstractGCodeCommand(GCode) {}
        virtual bool match();
        virtual void execute();
};

#endif