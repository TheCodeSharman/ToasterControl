#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include <Arduino.h>
#include <vector>

#include "GCode.h"
#include "PidController.h"
/*
    Command processor is responsible for recieve bytes from the serial
    port and executing G-Code commands.
*/

class CommandProcessor {
    GCodeParser GCode = GCodeParser();
    Stream& output;

    M105 m105;
    M104 m104;
    M997 m997;
    M301 m301;
    std::vector<AbstractGCodeCommand*> commands;

    public:
        CommandProcessor(Stream& output, PidController& oven);

    private:
        void processCommand();

    public:
        void init();
        void addByte(char inChar);
};

#endif