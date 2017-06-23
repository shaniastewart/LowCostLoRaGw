//Edited by Shania Stewart, June 2017
//The following code is based off Congduc Pham's LowCostLoRaGw Arduino sketches.
//This interfaces the Thermistor sensor module from Sunfounder into the
//Arduino_LoRa_Generic_Sensor sketch.

#include "thermo.h"

thermo::thermo(char* nomenclature, bool is_analog, bool is_connected, bool is_low_power, uint8_t pin_read, uint8_t pin_power):Sensor(nomenclature, is_analog, is_connected, is_low_power, pin_read, pin_power){
  if (get_is_connected()){

    pinMode(get_pin_read(), INPUT);
    pinMode(get_pin_power(),OUTPUT);
    
	if(get_is_low_power())
       digitalWrite(get_pin_power(),LOW);
    else
		digitalWrite(get_pin_power(),HIGH);
	
    set_wait_time(500);
  }
}

void thermo::update_data()
{	if (get_is_connected()){
	
		int beta = 3950; //beta value for thermosistor
		long a = 0;
		float tempC = 0;
	
		//powering with a digital pin
		if(get_is_low_power())
			digitalWrite(get_pin_power(), HIGH);
		
		//delay
		delay(get_wait_time());
		if (get_is_analog()){
			//from the sunsounder documentation for the sensor
			a = 1023 - analogRead(get_pin_read());
			tempC = beta/(log((1025.0*10/a-10)/10)+beta/298.0) - 273.0;
		}
		
		if (get_is_low_power())
			digitalWrite(get_pin_power(),LOW);
		
			set_data(tempC);
	}
	else {
		if (has_fake_data())
			set_data((double)random(-10, 30));
	}
	
}

double thermo::get_value()
{
	update_data();
	return get_data();
}

