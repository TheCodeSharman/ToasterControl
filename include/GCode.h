#ifndef GCODE_H
#define GCODE_H

#include <GCodeParser.h>

// Each GCode implements this class to define execution
class AbstractGCodeCommand {
    protected:
        GCodeParser& GCode;
    public:
        AbstractGCodeCommand( GCodeParser & GCode ) : GCode(GCode) {}

        // return true if the GCode parser state matches this command
        virtual bool match() = 0;

        // Implement this function to execute the matched gcode command
        virtual void execute() = 0;
};

#include "gcodes/M104.h"
#include "gcodes/M105_M155.h"
#include "gcodes/M997.h"
#include "gcodes/M301.h"
#include "gcodes/M305.h"
#include "gcodes/M500_503.h"

#endif