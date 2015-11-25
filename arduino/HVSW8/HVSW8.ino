
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

void scan(double DV, double D, double CVStart, double CVStop, double ScanTime ) {

  int PWMValue = 0;
  PWMValue = ((int) (D) / (DMax) * 5000); 
  int HVValue = 0;
  int LVValue = 0;
  double CVDelta=(CVStop-CVStart)/ScanTime*50;
  //250 ms response time of EMCO
  double CV=CVStart;
  //Get to start voltage
  double HV = DV + CV;
  double LV = -DV * D/(100 - D) + CV;
  if (abs(HV - LV) > 1000)  {
    Serial.println("Voltage out of range");
    delay(1000);
    reset();
    return;
  } else { 
    HVValue = abs((int) HV / HVMax * 5000);
    LVValue = abs((int) LV / LVMax * 5000);
    if (HV<0){
      dac.voutWrite(PWMValue, HVValue, 0, LVValue);    
    }else{
      dac.voutWrite(PWMValue, LVValue, 0, HVValue);
    }
    Serial.println("Stabilize");
    delay(250);
  }
  Serial.println("Starting in 5 seconds...");  
  delay(1000);
  Serial.println("Starting in 4 seconds...");  
  delay(1000);
  Serial.println("Starting in 3 seconds...");  
  delay(1000);
  Serial.println("Starting in 2 seconds...");  
  delay(1000);
  Serial.println("Starting in 1 seconds...");  
  delay(1000);
  Serial.println("GO!!!");  
  digitalWrite(ENSW1Pin, HIGH);
  int CVIndex = 0;
  while(CV<=CVStop){
    if (Serial.available()!=0){
      reset();
      break;
    }    
    double HV = DV + CV;
    if (CVIndex % 10 == 0){
      Serial.println("CV:");
      Serial.println(CV);
    }
    double LV = -DV * D/(100 - D) + CV;
    if (abs(HV - LV) > 1000)  {
      Serial.println("Voltage out of range");
      delay(1000);
      reset();
      return;
    } else { 
      HVValue = abs((int) HV / HVMax * 5000);
      LVValue = abs((int) LV / LVMax * 5000);
      if (HV<0){
        dac.voutWrite(PWMValue, HVValue, 0, LVValue);    
      }else{
        dac.voutWrite(PWMValue, LVValue, 0, HVValue);
      }   
    delay(50);
    }
    CV=CV+CVDelta;
    CVIndex++;
    }
  reset();
  //cool down
  delay(60000);
}
void loop() {

  dac.setVref(0, 0, 0, 0); // set to use external voltage reference (=VDD, 2.7 - 5.5V)
  int vref = dac.getVref(1); // get current voltage reference setting of channel 1
  int id = dac.getId(); // return devideID of object
  Serial.println("Duty?"); //Prompt User for Input
  while(Serial.available()==0) { // Wait for User to Input Data  
  }
  double D=Serial.parseFloat();  //Read the data the user has input
  Serial.println(D); 
  Serial.println("DV?"); //Prompt User for Input
  while(Serial.available()==0) { // Wait for User to Input Data  
  }
  double DV=Serial.parseFloat();  //Read the data the user has input
  Serial.println(DV); 
  Serial.println("CV start?"); //Prompt User for Input
  while(Serial.available()==0) { // Wait for User to Input Data  
  }
  double CVStart=Serial.parseFloat();  //Read the data the user has input
  Serial.println(CVStart); 
  Serial.println("CV stop?"); //Prompt User for Input
  while(Serial.available()==0) { // Wait for User to Input Data  
  }
  double CVStop=Serial.parseFloat();  //Read the data the user has input
  Serial.println(CVStop); 
  Serial.println("Scan Duration?"); //Prompt User for Input
  while(Serial.available()==0) { // Wait for User to Input Data  
  }
  double ScanTime=1000*Serial.parseFloat();  //Read the data the user has input
  Serial.println(ScanTime/1000); 

  Serial.println("Confirm? (y/n)"); //Prompt User for Input
  while(Serial.available()==0) { // Wait for User to Input Data  
  }   
  if (Serial.read() == 'y' ||Serial.read() == 'Y'){
    scan(DV, D, CVStart, CVStop, ScanTime);
  } else{
    reset();
  }
  
}
