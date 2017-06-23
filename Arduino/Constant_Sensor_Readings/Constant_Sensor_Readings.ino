//Constant sensor reading module
// Adapted from Congduc Pham's Arduino_LoRa_temp
// and Arduino_LoRa_Ping_Pong sketches to transmit constantly

#include <SPI.h>  
// Include the SX1272
#include "SX1272.h"

// IMPORTANT
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// please uncomment only 1 choice
//
//#define ETSI_EUROPE_REGULATION
#define FCC_US_REGULATION
//#define SENEGAL_REGULATION
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 

// IMPORTANT
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// uncomment if your radio is an HopeRF RFM92W, HopeRF RFM95W, Modtronix inAir9B, NiceRF1276
// or you known from the circuit diagram that output use the PABOOST line instead of the RFO line
//#define PABOOST
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 

// IMPORTANT
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// please uncomment only 1 choice
//#define BAND868
#define BAND900
//#define BAND433
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef ETSI_EUROPE_REGULATION
#define MAX_DBM 14
#elif defined SENEGAL_REGULATION
#define MAX_DBM 10
#elif defined FCC_US_REGULATION
#define MAX_DBM 14
#endif

#ifdef BAND868
#ifdef SENEGAL_REGULATION
const uint32_t DEFAULT_CHANNEL=CH_04_868;
#else
const uint32_t DEFAULT_CHANNEL=CH_10_868;
#endif
#elif defined BAND900
const uint32_t DEFAULT_CHANNEL=CH_12_900;
#elif defined BAND433
const uint32_t DEFAULT_CHANNEL=CH_00_433;
#endif

///////////////////////////////////////////////////////////////////
// CHANGE HERE THE LORA MODE, NODE ADDRESS 
#define LORAMODE  9
#define node_addr 8
//////////////////////////////////////////////////////////////////

// we wrapped Serial.println to support the Arduino Zero or M0
#if defined __SAMD21G18A__ && not defined ARDUINO_SAMD_FEATHER_M0
#define PRINTLN                   SerialUSB.println("")              
#define PRINT_CSTSTR(fmt,param)   SerialUSB.print(F(param))
#define PRINT_STR(fmt,param)      SerialUSB.print(param)
#define PRINT_VALUE(fmt,param)    SerialUSB.print(param)
#define FLUSHOUTPUT               SerialUSB.flush();
#else
#define PRINTLN                   Serial.println("")
#define PRINT_CSTSTR(fmt,param)   Serial.print(F(param))
#define PRINT_STR(fmt,param)      Serial.print(param)
#define PRINT_VALUE(fmt,param)    Serial.print(param)
#define FLUSHOUTPUT               Serial.flush();
#endif

#define TEMP_PIN_READ A1

#define DEFAULT_DEST_ADDR 1

uint8_t message[100];

int loraMode=LORAMODE;

void setup()
{
  int e;
  
  // Open serial communications and wait for port to open:
#if defined __SAMD21G18A__ && not defined ARDUINO_SAMD_FEATHER_M0 
  SerialUSB.begin(38400);
#else
  Serial.begin(38400);  
#endif 

  // Print a start message
  PRINT_CSTSTR("%s","Simple LoRa ping-pong with the gateway\n");  

#ifdef ARDUINO_AVR_PRO
  PRINT_CSTSTR("%s","Arduino Pro Mini detected\n");
#endif

#ifdef ARDUINO_AVR_NANO
  PRINT_CSTSTR("%s","Arduino Nano detected\n");
#endif

#ifdef ARDUINO_AVR_MINI
  PRINT_CSTSTR("%s","Arduino MINI/Nexus detected\n");
#endif

#ifdef __MK20DX256__
  PRINT_CSTSTR("%s","Teensy31/32 detected\n");
#endif

#ifdef __SAMD21G18A__ 
  PRINT_CSTSTR("%s","Arduino M0/Zero detected\n");
#endif

  // Power ON the module
  sx1272.ON();
  
  // Set transmission mode and print the result
  e = sx1272.setMode(loraMode);
  PRINT_CSTSTR("%s","Setting Mode: state ");
  PRINT_VALUE("%d", e);
  PRINTLN;

  // enable carrier sense
  sx1272._enableCarrierSense=true;
    
  // Select frequency channel
  e = sx1272.setChannel(DEFAULT_CHANNEL);
  PRINT_CSTSTR("%s","Setting Channel: state ");
  PRINT_VALUE("%d", e);
  PRINTLN;
  
  // Select amplifier line; PABOOST or RFO
#ifdef PABOOST
  sx1272._needPABOOST=true;
  // previous way for setting output power
  // powerLevel='x';
#else
  // previous way for setting output power
  // powerLevel='M';  
#endif

  // previous way for setting output power
  // e = sx1272.setPower(powerLevel); 

  e = sx1272.setPowerDBM((uint8_t)MAX_DBM); 
  PRINT_CSTSTR("%s","Setting Power: state ");
  PRINT_VALUE("%d", e);
  PRINTLN;
  
  // Set the node address and print the result
  e = sx1272.setNodeAddress(node_addr);
  PRINT_CSTSTR("%s","Setting node addr: state ");
  PRINT_VALUE("%d", e);
  PRINTLN;
  
  // Print a success message
  PRINT_CSTSTR("%s","SX1272 successfully configured\n");

  delay(10);
}


void loop(void)
{
  uint8_t r_size;
  int e;

  sx1272.CarrierSense();

  sx1272.setPacketType(PKT_TYPE_DATA);

  r_size=sprintf((char*)message, "Working");

   int value = analogRead(TEMP_PIN_READ);
   
  // change here how the temperature should be computed depending on your sensor type
  //  
  int beta = 3950;
  long a = 0;
  float temp = 0;

  a = 1023 - value;

  PRINT_CSTSTR("%s","Reading ");
  PRINT_VALUE("%d", value);
  PRINTLN;

  temp = beta/(log((1025.*10/a-10)/10)+beta/298.0)-273.0;

  PRINT_CSTSTR("%s","Temp is ");
  PRINT_VALUE("%f", temp);
  PRINTLN;
  
  e = sx1272.sendPacketTimeout(DEFAULT_DEST_ADDR, message, r_size);
		
  PRINT_CSTSTR("%s","Packet sent, state ");
  PRINT_VALUE("%d", e);
  PRINTLN;
  
  delay(20);    
}        






