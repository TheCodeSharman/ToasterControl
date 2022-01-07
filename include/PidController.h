#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include <PID_v1.h>

#include "Sensor.h"
#include "ControlledDevice.h"

class PidController {
    private:
        Sensor& input;
        ControlledDevice& output;

        double inputValue;
        double outputValue;
        double setPoint;
        double Kp, Ki, Kd;

        PID pid;
    
    public:
        double getInput() { return inputValue; }
        double getSetPoint() { return setPoint; }
        double getOutputValue() { return outputValue; }

        void setSetPoint( double setPoint ) { this->setPoint = setPoint; }

        PidController( Sensor& input, ControlledDevice& output, double Kp, double Ki, double Kd );
        void process();
        void start( double setPoint );
        void stop();
};

#endif