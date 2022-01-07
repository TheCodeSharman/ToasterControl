#ifndef SENSOR_H
#define SENSOR_H

/*
    A sensor that can read a double value
*/
class Sensor {
  public:
    virtual double readSensor() = 0;
};

#endif