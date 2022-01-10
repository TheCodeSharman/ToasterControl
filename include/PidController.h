#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include <PID_v1.h>

#include "Sensor.h"
#include "ControlledDevice.h"

typedef struct {
  float Kp = 2;
  float Ki = 5;
  float Kd = 1;
  int P_MODE = P_ON_E;
} PidCalibration;

/*  PID Controller:
    "An empirical procedure that works pretty well is the following:
        (a) with I and D terms turned off, increase the P gain until 
            the system begins to oscillate, then back off a bit; the
            sytem should now exhibit overshoot and ringing, but not
            sustained oscillation, in response to a step change in 
            setpoint;
        (b) now add D gain until the response to a step is critically 
            damped;
        (c) finally, while watching the error signal itself, add I gain
            to achieve minimum settling time."
    (Art of Electronics 3rd edition page 1074 para 15.6.2)
*/

class PidController {
    private:
        PidCalibration calibration;
        Sensor& input;
        ControlledDevice& output;

        double inputValue;
        double outputValue;
        double setPoint;

        PID pid;
    
    public:
        double getInput() { return inputValue; }
        double getSetPoint() { return setPoint; }
        double getOutput() { return outputValue; }
        double getError() { return setPoint - inputValue; }
        PidCalibration& getPidCalibration() { return calibration; }

        void setOutputLimits( double min, double max) {
            pid.SetOutputLimits(min,max);
        }
        void setSetPoint( double setPoint ) { this->setPoint = setPoint; }
        void setPidCalibration( const PidCalibration& calibration );


        PidController( Sensor& input, ControlledDevice& output );

        void process();
        void start( double setPoint );
        void stop();
};

#endif