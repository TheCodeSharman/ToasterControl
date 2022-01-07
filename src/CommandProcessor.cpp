#include "CommandProcessor.h"

CommandProcessor::CommandProcessor(Stream& output): output( output ) {
    GCode = GCodeParser();
}

void CommandProcessor::jumpToBootloader() {
    HAL_RCC_DeInit();
    HAL_DeInit();
    SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL = 0;
    __HAL_SYSCFG_REMAPMEMORY_SYSTEMFLASH();
    // arm-none-eabi-gcc 4.9.0 does not correctly inline this
    // MSP function, so we write it out explicitly here.
    //__set_MSP(*((uint32_t*) 0x00000000));
    #pragma GCC diagnostic ignored "-Wdeprecated"
    __ASM volatile ("movs r3, #0\nldr r3, [r3, #0]\nMSR msp, r3\n" : : : "r3", "sp");
    ((void (*)(void)) *((uint32_t*) 0x00000004))();
    #pragma GCC diagnostic pop
}

void CommandProcessor::setTemperature( int temp ) {
    output.printf("Target Temperature = %i °C\r\n", temp);
}

void CommandProcessor::processCommand() {
    if ( GCode.HasWord('M') ) {
        switch( (int)GCode.GetWordValue('M') ) {
            case 997:
                jumpToBootloader();
                break;

            case 105:
                if ( GCode.HasWord('T') ) {
                    
                    setTemperature( (int)GCode.GetWordValue('T') );
                }
                break;
        }

    }
}

void CommandProcessor::init() {
    
} 

void CommandProcessor::addByte(char inChar) {
    // Output input as feedback to serial monitor users
    // but suppress carriage return and line feed characters
    // so the tmerinal prompt is displaed as expected.
    if ( inChar != '\n' && inChar != '\r')
        output.print(inChar);
        
    if ( GCode.AddCharToLine(inChar)  ) {
        GCode.ParseLine();
        output.printf("\r\n");
        processCommand();
        output.printf("ToasterControl> ");
    }
    
}