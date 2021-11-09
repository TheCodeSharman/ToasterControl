#ifndef EVERY_H
#define EVERY_H

#include <Arduino.h>
#include <vector>

/*
    Keeps track of multiple functions to be called at specified intervals.

    To use declare an instance of Every:

            MultiTask tasks;

        In some initialisation function add some callbacks:

            tasks.every(1000,callbackFunction1);
            tasks.every(500,callbackFunction2);

        Then in the main loop call process():

            tasks.process();

    It's up to the impementor to ensure there is no blocking code and return 
    promptly - otherwise nothing will be called whilst a function is being run.
*/
class MultiTask {
  uint32_t current;
  typedef struct  {
    void (*callback)();
    uint32_t nextCall;
    uint32_t period;
  } CallbackFunction;

  std::vector<CallbackFunction> callbacks;

  public:
    /* Add a call back function to be called every time the clock ticks over the 
       specified delay in milliseconds */
    void every(uint32_t delay, void (*callback)()) {
      CallbackFunction cb = { callback, 0, delay };
      callbacks.push_back(cb);
    }

    /* Needs to be called as often as possible */
    void process() {
      current = millis();
      for(auto &cb : callbacks ) {
        if ( cb.nextCall < current ) {
          cb.nextCall = current+cb.period;
          cb.callback();
        }
      }
    }
};


#endif