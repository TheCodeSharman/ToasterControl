#ifndef SETTINGS_H
#define SETTINGS_H

#include <EEPROM.h>
#include "PidController.h"
#include "KTypeProbe.h"

static const int SETTINGS_VERSION = 0x1000004;
static const uint16_t SETTINGS_VERSION_ADDRESS = 0;

static const uint16_t SETTINGS_PID_CONTOLLER_ADDRESS = 4;
static const PidCalibration SETTINGS_PID_DEFAULT_CALIBRATION = { 500, 0, 0 };

static const uint16_t SETTINGS_K_TYPE_PROBE_ADDRESS = 20;
static const KProbeCalibration SETTINGS_K_TYPE_PROBE_DEFAULT_CALIBRATION = { -3, 2022, 104, 2716 };

/*
    Manages the settings for the machine in EEPROM
    (uses EEPROM emulator currently)
*/
class Settings {
    private:
        Stream& output;
        PidController& pid;
        KTypeProbe& probe;

        template< typename T > void writeStruct(uint16_t pos, const T &t)
        {
            const uint8_t *ptr = (const uint8_t *) &t;
            for (int count = sizeof(T) ; count ; --count ) {
                eeprom_buffered_write_byte(pos++,*ptr++);
            }
        }

        template< typename T > void readStruct(uint16_t pos, T &t)
        {
            uint8_t *ptr = (uint8_t *) &t;
            for (int count = sizeof(T) ; count ; --count ) {
                *ptr++ = eeprom_buffered_read_byte(pos++);
            }
        }

    public:

    
        Settings(Stream& output, KTypeProbe& probe,PidController& pid) : output(output), probe(probe), pid(pid)
        {}

        PidCalibration& getPidCalibration() { return pid.getPidCalibration(); }
        KProbeCalibration& getProbeCalibration() { return probe.getProbeCalibration(); }

        void saveSettings();
        void loadSettings();
        void resetSettings();
};

#endif