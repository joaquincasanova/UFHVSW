
/*
Basic use of Arduino library for MicroChip MCP4728 I2C D/A converter
For discussion and feedback, please go to http://arduino.cc/forum/index.php/topic,51842.0.html
*/

#include <Wire.h>
#include "mcp4728.h"

mcp4728 dac = mcp4728(0); // instantiate mcp4728 object, Device ID = 0

//enable gate driver
int ENSW1Pin = 9;

double LVMax = 1000;
double HVMax = 1000;
double DMax = 100;
double DMin = 0;
int i=0;

void setup()
{
  Serial.begin(9600);  // initialize serial interface for print()
  Serial.println("Initializing...");
  dac.begin();  // initialize i2c interface
  dac.vdd(5000); // set VDD(mV) of MCP4728 for correct conversion between LSB and Vout
  dac.setVref(0, 0, 0, 0); // set to use external voltage reference (=VDD, 2.7 - 5.5V)
  int vref = dac.getVref(1); // get current voltage reference setting of channel 1
  pinMode(ENSW1Pin, OUTPUT);
}

void reset()  {
  //set everything to zero
  Serial.println("RESET");
  dac.voutWrite(0, 0, 0, 0);
  digitalWrite(ENSW1Pin, LOW); 
}

void experiment(double LV, double HV, double D) {

  int PWMValue = 0;
  PWMValue = ((int) (D) / (DMax) * 5000); 
  int HVValue = 0;
  HVValue = abs((int) HV / HVMax * 5000);
  int LVValue = 0;
  LVValue = abs((int) LV / LVMax * 5000);

  delay(100);
  dac.voutWrite(PWMValue, LVValue, 0 , HVValue);

  digitalWrite(ENSW1Pin, HIGH);

  ///
  //actual run
  Serial.println("EXPERIMENT");
  delay(30000);
  ///
  reset();
  delay(10000);
}

void loop() {

  dac.setVref(0, 0, 0, 0); // set to use external voltage reference (=VDD, 2.7 - 5.5V)
  int vref = dac.getVref(1); // get current voltage reference setting of channel 1
  int id = dac.getId(); // return devideID of object
  Serial.println("Duty?"); //Prompt User for Input
  while(Serial.available()==0) { // Wait for User to Input Data  
  }
  double D=Serial.parseFloat();  //Read the data the user has input
  Serial.println("DV?"); //Prompt User for Input
  while(Serial.available()==0) { // Wait for User to Input Data  
  }
  double DV=Serial.parseFloat();  //Read the data the user has input
  Serial.println("CV?"); //Prompt User for Input
  while(Serial.available()==0) { // Wait for User to Input Data  
  }
  double CV=Serial.parseFloat();  //Read the data the user has input

  double HV = DV + CV;
  Serial.println("HV:");
  Serial.println(HV);
  double LV = -DV * D/(100 - D) + CV;
  Serial.println("LV:");
  Serial.println(LV);
  delay(100);
  if (abs(HV - LV) > 1000)  {
    Serial.println("Voltage out of range");
    delay(10000);
    reset();
  } else {
    reset();
    Serial.println("Run :");
    experiment(LV, HV, D);
    reset();
  }

}
