#include "PidController.h"

PidController::PidController( Sensor& input, ControlledDevice& output,
    double Kp, double Ki, double Kd )
        : input( input ), output( output), Kp(Kp), Ki(Ki), Kd(Kd),
          pid(&inputValue,&outputValue,&setPoint, Kp, Ki, Kd, DIRECT) {
    
}

void PidController::process() {
    // still read input value even if we are disabled for monitoring purposes
    inputValue = input.readSensor();
    if ( pid.Compute() ) {
        output.setValue(outputValue);
    }
}

void PidController::start( double setPoint ) {
    this->setPoint = setPoint;
    pid.SetMode(AUTOMATIC);
}

void PidController::stop() {
    pid.SetMode(MANUAL);
    output.setValue(0);
}