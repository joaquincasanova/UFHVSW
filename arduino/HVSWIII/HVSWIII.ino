
/*
Basic use of Arduino library for MicroChip MCP4728 I2C D/A converter
For discussion and feedback, please go to http://arduino.cc/forum/index.php/topic,51842.0.html
*/

#include <Wire.h>
#include "mcp4728.h"

mcp4728 dac = mcp4728(0); // instantiate mcp4728 object, Device ID = 0

//enable gate driver positive
int ENSW1Pin = 9;
//enable gate driver negative
int ENSW2Pin = 6;

double LVMax = 1000;
double HVMax = 1000;
double DMax = 100;
double DMin = 0;
int i=0;

void setup()
{
  Serial.begin(9600);  // initialize serial interface for print()
  Serial.print("Howdy");
  dac.begin();  // initialize i2c interface
  dac.vdd(5000); // set VDD(mV) of MCP4728 for correct conversion between LSB and Vout
  dac.setVref(0, 0, 0, 0); // set to use external voltage reference (=VDD, 2.7 - 5.5V)
  int vref = dac.getVref(1); // get current voltage reference setting of channel 1
  Serial.print("Voltage reference setting of channel 1 = "); // serial print of value
  Serial.println(vref, DEC); // serial print of value
  pinMode(ENSW1Pin, OUTPUT);
  pinMode(ENSW2Pin, OUTPUT);
}

void reset()  {
  //set everything to zero
  Serial.println("RESET");
  dac.voutWrite(0, 0, 0, 0);
  digitalWrite(ENSW1Pin, LOW);
  digitalWrite(ENSW2Pin, LOW);
}

void experiment(double LV, double HV, double D) {

  int PWMValue = 0;
  PWMValue = ((int) (D) / (DMax) * 5000); 
  int HVValue = 0;
  HVValue = abs((int) HV / HVMax * 5000);
  int LVValue = 0;
  LVValue = abs((int) LV / LVMax * 5000);

  Serial.println("Voltage out of DAC (mv) ");
  Serial.println(PWMValue);
  Serial.println(HVValue);
  Serial.println(LVValue);
  delay(100);
  //positive or negative? enable correct gate driver

  if (HV >= 0) {
    digitalWrite(ENSW1Pin, HIGH);
    digitalWrite(ENSW2Pin, LOW);
  } else {
    digitalWrite(ENSW1Pin, LOW);
    digitalWrite(ENSW2Pin, HIGH);
  };

  delay(100);
  Serial.println("Voltage assignation");
  if (HV >= 0) {
    Serial.println("Positive");
    dac.voutWrite(PWMValue,  0 ,LVValue, HVValue);
    for (i = 0; i < 4; i++) {
      int vout = dac.getVout(i); // get current voltage out of channel 1
      Serial.print("Voltage out of channel ");
      Serial.println(i, DEC);
      Serial.print(" = ");
      Serial.println(vout, DEC); // serial print of value
    }
  } else {
    Serial.println("Negative");
    dac.voutWrite(PWMValue, HVValue,  LVValue, 0);   
    for (i = 0; i < 4; i++) {
      int vout = dac.getVout(i); // get current voltage out of channel 1
      Serial.print("Voltage out of channel ");
      Serial.println(i, DEC);
      Serial.print(" = ");
      Serial.println(vout, DEC); // serial print of value
    }
  }

  ///
  //actual run
  Serial.println("EXPERIMENT");
  delay(6000);
  ///
  reset();
  delay(100);
}

void loop() {

  dac.setVref(0, 0, 0, 0); // set to use external voltage reference (=VDD, 2.7 - 5.5V)
  int vref = dac.getVref(1); // get current voltage reference setting of channel 1
  Serial.print("Voltage reference setting of channel 1 = "); // serial print of value
  Serial.println(vref, DEC); // serial print of value

  int id = dac.getId(); // return devideID of object
  Serial.print("Device ID  = "); // serial print of value
  Serial.println(id, DEC); // serial print of value

  double DV = 200;
  double CV = 0;
  double D = 50;
  double HV = 0;//DV + CV;
  Serial.println("HV:");
  Serial.println(HV);
  double LV = -200;//DV * D / (100 - D) + CV ;
  Serial.println("LV:");
  Serial.println(LV);
  delay(100);
  if (abs(HV - LV) > 1000) {
    Serial.println("Voltage out of range");
    delay(10000);
    reset();
  } else {
    reset();
    Serial.println("Run :");
    experiment(LV, HV, D);
    reset();
  }
//  DV = -100;
//  HV = DV + CV;
//  Serial.println("HV:");
//  Serial.println(HV);
//  LV = -HV * D / (100 - D) + CV ;
//  Serial.println("LV:");
//  Serial.println(LV);
//  delay(10000);
//  if (abs(HV - LV) > 1000) {
//    Serial.println("Voltage out of range");
//    delay(10000);
//    reset();
//  } else {
//    reset();
//    Serial.println("Run:");
//    experiment(LV, HV, D);
//    reset();
//  }
}
