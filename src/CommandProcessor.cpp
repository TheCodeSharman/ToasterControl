#include "CommandProcessor.h"


CommandProcessor::CommandProcessor(Stream& output, 
    PidController& oven, KTypeProbe& probe, Settings& settings)
    : output(output),
      m104(output,GCode,oven),
      m105(output,GCode,oven,probe),
      m301(output,GCode,oven),
      m303(output,GCode,oven),
      m306(output,GCode,probe),
      m500_503(output,GCode,settings),
      m997(GCode) {
    commands = std::vector<AbstractGCodeCommand*> {
         &m104, &m105, &m997, &m301, &m303, &m306, &m500_503 };
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
    output.printf("ToasterControl> ");
} 

void CommandProcessor::addByte(char inChar) {
    // Output input as feedback to serial monitor users
    // but suppress carriage return and line feed characters
    // so the terminal prompt is displayed as expected.
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