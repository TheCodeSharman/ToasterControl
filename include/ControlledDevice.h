#ifndef CONTROLLED_DEVICE_H
#define CONTROLLED_DEVICE_H

/*
    A device that can be controlled by setting a single value.
*/
class ControlledDevice {
  public:
    virtual void setValue( double value ) = 0;
};

#endif