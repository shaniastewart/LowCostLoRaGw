//Edited by Shania Stewart, June 2017
//The following code is based off Congduc Pham's LowCostLoRaGw Arduino sketches.
//This interfaces the Photoresistor sensor module from Sunfounder into the
//Arduino_LoRa_Generic_Sensor sketch. 

#ifndef photoresistor_H
#define photoresistor_H

#include "Sensor.h"

class photoresistor : public Sensor {
  public:    
    photoresistor(char* nomenclature, bool is_analog, bool is_connected, bool is_low_power, uint8_t pin_read, uint8_t pin_power);
    void update_data();
    double get_value();
};

#endif