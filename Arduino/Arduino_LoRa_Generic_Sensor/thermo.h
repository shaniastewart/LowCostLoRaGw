//Edited by Shania Stewart, June 2017
//The following code is based off Congduc Pham's LowCostLoRaGw Arduino sketches.
//This interfaces the Thermistor sensor module from Sunfounder into the
//Arduino_LoRa_Generic_Sensor sketch. 

#ifndef thermo_H
#define thermo_H

#include "Sensor.h"

class thermo : public Sensor {
  public:    
    thermo(char* nomenclature, bool is_analog, bool is_connected, bool is_low_power, uint8_t pin_read, uint8_t pin_power);
    void update_data();
    double get_value();
};

#endif

