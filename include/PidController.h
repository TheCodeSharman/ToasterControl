#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include <PID_v1.h>
#include <PID_AutoTune_v0.h>

#include "Sensor.h"
#include "ControlledDevice.h"


typedef struct {
  float Kp = 2;
  float Ki = 5;
  float Kd = 1;
  int P_MODE = P_ON_E;
} PidCalibration;

class PidController {
    private:
        PidCalibration calibration;
        Sensor& input;
        ControlledDevice& output;

        double inputValue;
        double outputValue;
        double setPoint;

        bool isTuning = false;

        PID pid;
        PID_ATune pidAutoTune;
    
    public:
        double getInput() { return inputValue; }
        double getSetPoint() { return setPoint; }
        double getOutput() { return outputValue; }
        PidCalibration& getPidCalibration() { return calibration; }

        void setOutputLimits( double min, double max) {
            pid.SetOutputLimits(min,max);
        }
        void setSetPoint( double setPoint ) { this->setPoint = setPoint; }
        void setPidCalibration( const PidCalibration& calibration );


        PidController( Sensor& input, ControlledDevice& output );

        void startAutoTune( double setPoint );
        void cancelAutoTune();
        bool getIsTuning() { return isTuning; }

        void process();
        void start( double setPoint );
        void stop();
};

#endif