#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include <PID_v1.h>

#include "Sensor.h"
#include "ControlledDevice.h"


typedef struct {
  double Kp;
  double Ki;
  double Kd;
  int P_MODE;
} PidCalibration;

class PidController {
    private:
        PidCalibration& calibration;
        Sensor& input;
        ControlledDevice& output;

        double inputValue;
        double outputValue;
        double setPoint;

        PID pid;
    
    public:
        double getInput() { return inputValue; }
        double getSetPoint() { return setPoint; }
        double getOutputValue() { return outputValue; }
        PidCalibration& getPidCalibration() { return calibration; }

        void setSetPoint( double setPoint ) { this->setPoint = setPoint; }
        void setPidCalibration( PidCalibration& calibration );


        PidController( Sensor& input, ControlledDevice& output, PidCalibration& calibration );
        void process();
        void start( double setPoint );
        void stop();
};

#endif