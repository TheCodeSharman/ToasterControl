#include "CommandProcessor.h"
#include "KTypeProbe.h"

CommandProcessor::CommandProcessor(Stream& output, PidController& oven)
    : output( output ),
      m104( output, GCode, oven ),
      m105( output, GCode, oven ),
      m997( GCode ) {
    commands = std::vector<AbstractGCodeCommand*> {
         &m104, &m105, &m997 };
}

void CommandProcessor::processCommand() {
    for( auto command : commands) {
        if ( command->match() ) {
            command->execute();
            return;
        }
    }
    output.printf("ERROR: Unable to execute command '%s'\r\n", GCode.line);
}

void CommandProcessor::init() {
    delay(1000);
    output.printf("ToasterControl> ");
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
        if ( strlen(GCode.line) > 0 ) {
            processCommand();
        }
        output.printf("ToasterControl> ");
    }
    
}