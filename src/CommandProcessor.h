#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include <GCodeParser.h>
#include <Arduino.h>
/*
    Command processor is responsible for recieve bytes from the serial
    port and executing G-Code commands.
*/

class CommandProcessor {
    GCodeParser GCode;
    Stream& output;

    public:
        CommandProcessor(Stream& output);

    private:
        void jumpToBootloader();
        void setTemperature(double temp);
        void processCommand();

    public:
        void init();
        void addByte(char inChar);
};

#endif