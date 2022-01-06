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
        void jumpToBootloader() {
            HAL_RCC_DeInit();
            HAL_DeInit();
            __HAL_REMAPMEMORY_SYSTEMFLASH();
            // arm-none-eabi-gcc 4.9.0 does not correctly inline this
            // MSP function, so we write it out explicitly here.
            //__set_MSP(*((uint32_t*) 0x00000000));
            #pragma GCC diagnostic ignored "-Wdeprecated"
            __ASM volatile ("movs r3, #0\nldr r3, [r3, #0]\nMSR msp, r3\n" : : : "r3", "sp");
            ((void (*)(void)) *((uint32_t*) 0x00000004))();
            #pragma GCC diagnostic pop
        }

        void setTemperature( double temp ) {
            output.printf("Setting temperature to %d\n", temp);
        }

        void processCommand() {
            if ( GCode.HasWord('M') ) {
                switch( (int)GCode.GetWordValue('M') ) {
                    case 997:
                        jumpToBootloader();
                        break;

                    case 105:
                        if ( GCode.HasWord('T') ) {
                            setTemperature( GCode.GetWordValue('T') );
                        }
                        break;
                }

            }
        }
    public:
        void init() {
            output.printf("ToasterControl ready.\n");
        } 
        void addByte(char inChar) {
            if ( GCode.AddCharToLine(inChar)  ) {
                GCode.ParseLine();
                processCommand();
            }
        }
};

#endif