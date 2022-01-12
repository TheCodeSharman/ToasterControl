#include "CommandProcessor.h"


CommandProcessor::CommandProcessor(Stream& output, 
    PidController& oven, KTypeProbe& probe, Settings& settings, MultiTask& multitask)
    : output(output),
      multitask(multitask),
      m104(output,GCode,oven),
      m105_155(output,GCode,oven,probe,multitask),
      m301(output,GCode,oven),
      m305(output,GCode,probe),
      m500_503(output,GCode,settings),
      m997(GCode) {
    commands = std::vector<AbstractGCodeCommand*> {
         &m104, &m105_155, &m997, &m301, &m305, &m500_503 };
}

void CommandProcessor::processCommand() {
    for( auto command : commands) {
        if ( command->match() ) {
            command->execute();
            return;
        }
    }
    output.printf("error: Unknown command: \"%s\"\r\n", GCode.line);
}

void CommandProcessor::init() {
} 

void CommandProcessor::addByte(char inChar) {
    if ( GCode.AddCharToLine(inChar)  ) {
        GCode.ParseLine();
        if ( strlen(GCode.line) > 0 ) {
            processCommand();
        }
    }
    
}