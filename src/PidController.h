#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include <deque>

#include "Sensor.h"
#include "ControlledDevice.h"
#include "MultiTask.h"

typedef struct {
  float Kp = 2;
  float Ki = 0.01;
  float Kd = 0.2;
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
        MultiTask& tasks;

        double outputMin;
        double outputMax;

        double inputValue;
        double outputValue;

        double setPoint;

        double I;
        double P;
        double D;

        int samplePeriod = 200;   // ms
        int averageN = 10;
        std::deque<double> errors;
        double slope;

        double lastError;
        double error;

        MultiTask::CallbackFunction *processLoop;

        void resetParameters();
    
    public:
        PidController( Sensor& input, ControlledDevice& output, MultiTask& tasks );

        double getInput() { return inputValue; }
        double getSetPoint() { return setPoint; }
        double getOutput() { return outputValue; }
    
        double getError() { return error; }
        double getSlope() { return slope; }

        double getP() { return P; }
        double getI() { return I; }
        double getD() { return D; }

        int getSamplePeriod() { return samplePeriod; }
        int getAverages() { return averageN; }

        PidCalibration& getPidCalibration() { return calibration; }

        void setSamplePeriod( int ms ) { samplePeriod = ms; }
        void setAverages( int n ) { averageN = n; }

        void setOutputLimits( double min, double max) {
            outputMin = min;
            outputMax = max;
        }

        void setSetPoint( double setPoint ) { this->setPoint = setPoint; }
        void setPidCalibration( const PidCalibration& calibration );


        

        void process();
        void start( double setPoint );
        void stop();
};

#endif