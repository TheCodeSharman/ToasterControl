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
    CommandProcessor(Stream& output): output( output ) {
        GCode = GCodeParser();
    }

    private:
        void setTemperature( double temp ) {
            output.printf("Setting temperature to %d\n", temp);
        }

        void processCommand() {
            if ( GCode.HasWord('M') ) {
                switch( (int)GCode.GetWordValue('M') ) {
                    case 105:
                        if ( GCode.HasWord('T') ) {
                            setTemperature( GCode.GetWordValue('T') );
                        }
                }

            }
        }
    public:
        void init() {
            output.printf("ToasterControl ready.\n")
        } 
        void addByte(char inChar) {
            if ( GCode.AddCharToLine(inChar)  ) {
                GCode.ParseLine();
                processCommand();
            }
        }
};

#endif