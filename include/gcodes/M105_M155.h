#ifndef M105_M155_H
#define M105_M155_H

#include <Arduino.h>

#include "GCode.h"
#include "KTypeProbe.h"
#include "PidController.h"
#include "MultiTask.h"

/*
    Reports the temperature of the oven
*/
class M105_M155 : public AbstractGCodeCommand {
    private:
        PidController& oven;
        KTypeProbe& probe;
        Stream& output;
        MultiTask& multitask;
        MultiTask::CallbackFunction *reportingCallback;
    public:
        M105_M155(Stream& output, GCodeParser& GCode, PidController& oven, KTypeProbe& probe, MultiTask& multitask) 
            : output(output), AbstractGCodeCommand(GCode), oven(oven), probe(probe), multitask(multitask), reportingCallback(NULL) {}
        virtual bool match();
        virtual void execute();
        void reportTemperatures();
};

#endif