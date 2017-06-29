//Edited by Shania Stewart, June 2017
//The following code is based off Congduc Pham's LowCostLoRaGw Arduino sketches.
//This interfaces the Photoresistor sensor module from Sunfounder into the
//Arduino_LoRa_Generic_Sensor sketch.

#include "photoresistor.h"

photoresistor::photoresistor(char* nomenclature, bool is_analog, bool is_connected, bool is_low_power, uint8_t pin_read, uint8_t pin_power):Sensor(nomenclature, is_analog, is_connected, is_low_power, pin_read, pin_power){
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

void photoresistor::update_data()
{	if (get_is_connected()){
		
		float res = 0;
    float for_avg = 0;
    float value = 0;
    float volt_af_div = 0;
	
		//powering with a digital pin
		if(get_is_low_power())
			digitalWrite(get_pin_power(), HIGH);
		
		//delay
		delay(get_wait_time());
		if (get_is_analog()){
			//from the sunsounder documentation for the sensor
			for (int i=0; i<10; i++) {
         
          value = analogRead(get_pin_read());
          for_avg += value;
          
          delay(100);
      }
    
    for_avg = for_avg/10;
    volt_af_div = for_avg * (3.3/pow(2,10));
    res = (3.3 - volt_af_div)/(volt_af_div/1000);
   
		}
		
		if (get_is_low_power())
			digitalWrite(get_pin_power(),LOW);
		
			set_data(res);
	}
	else {
		if (has_fake_data())
			set_data((double)random(-10, 30));
	}
	
}

double photoresistor::get_value()
{
	update_data();
	return get_data();
}

