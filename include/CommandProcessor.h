#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include <Arduino.h>
#include <vector>

#include "GCode.h"

/*
    Command processor is responsible for recieve bytes from the serial
    port and executing G-Code commands.
*/

class CommandProcessor {
    GCodeParser GCode = GCodeParser();
    Stream& output;

    M104 m104;
    M997 m997;
    std::vector<AbstractGCodeCommand*> commands;

    public:
        CommandProcessor(Stream& output);

    private:
        void processCommand();

    public:
        void init();
        void addByte(char inChar);
};

#endif