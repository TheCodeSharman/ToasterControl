#ifndef M500_503_H
#define M500_503_H

#include "GCode.h"

#include "Settings.h"

/*
    Saves and load configurable settings to EEPROM
*/
class M500_503 : public AbstractGCodeCommand {
    private:
        Stream& output;
        Settings& settings;
    public:
        M500_503(Stream& output, GCodeParser& GCode, Settings& settings) 
            : output(output), settings(settings), AbstractGCodeCommand(GCode) {}
        virtual bool match();
        virtual void execute();
};

#endif