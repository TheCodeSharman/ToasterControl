#include "Settings.h"

void Settings::saveSettings() {
    eeprom_buffer_fill();
    writeStruct(SETTINGS_VERSION_ADDRESS,SETTINGS_VERSION);
    writeStruct(SETTINGS_PID_CONTOLLER_ADDRESS,pid.getPidCalibration());
    writeStruct(SETTINGS_K_TYPE_PROBE_ADDRESS,probe.getProbeCalibration());
    eeprom_buffer_flush();
}

void Settings::loadSettings() {
    eeprom_buffer_fill();
    
    // check the settings version and reset them if they are out of date
    int version;
    readStruct(SETTINGS_VERSION_ADDRESS,version);
    if ( version != SETTINGS_VERSION ) {
        output.printf("; Settings::loadSettings(): warning: VERSION incorrect, resetting to factory defaults\r\n");
        resetSettings();
        return;
    }
    
    readStruct(SETTINGS_PID_CONTOLLER_ADDRESS,pid.getPidCalibration());
    readStruct(SETTINGS_K_TYPE_PROBE_ADDRESS,probe.getProbeCalibration());
}

void Settings::resetSettings() {
    probe.setProbeCalibration( SETTINGS_K_TYPE_PROBE_DEFAULT_CALIBRATION );
    pid.setPidCalibration( SETTINGS_PID_DEFAULT_CALIBRATION );
    saveSettings();
}