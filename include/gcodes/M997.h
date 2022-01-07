#ifndef M997_H
#define M997_H

#include "GCode.h"

/*
    Enters the system bootloader ready for firmware update.
    M997 is the code Marlin uses for the same.
*/
class M997 : public AbstractGCodeCommand {
    public:
        M997(GCodeParser& GCode) : AbstractGCodeCommand(GCode) {}
        virtual bool match();
        virtual void execute();
};

#endif