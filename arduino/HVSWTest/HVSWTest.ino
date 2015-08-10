
int PWMPin = 3;   
//positive HV pin
int VREG1Pin = 5;
//bipolar LV pin
int VREG2Pin = 6;
//negative HV pin
int VREG3Pin = 9; 

int RLSTARTPin = 2; 
int RLPOLPin = 8; 
//enable gate driver positive
int ENSW1Pin = 10;
//enable gate driver positive
int ENSW2Pin = 11;

double LVMax = 100;
double HVMax = 1000;

void setup()  {
  //pin mapping
  pinMode(PWMPin,OUTPUT);    
  pinMode(VREG1Pin,OUTPUT);   
  pinMode(VREG2Pin,OUTPUT); 
  pinMode(VREG3Pin,OUTPUT);    
  pinMode(RLSTARTPin,OUTPUT); 
  pinMode(RLPOLPin,OUTPUT);   
  pinMode(ENSW1Pin,OUTPUT);  
  pinMode(ENSW2Pin,OUTPUT);   
  
} 

void zero(){
  //set everything to zero
  analogWrite(PWMPin, fadeValue);
  analogWrite(VREG1Pin, fadeValue);
  analogWrite(VREG2Pin, fadeValue);
  analogWrite(VREG3Pin, fadeValue);
  digitalWrite(RLSTARTPin,LOW); 
  digitalWrite(ENSW1Pin,LOW);  
  digitalWrite(ENSW2Pin,LOW);   
}

void experiment(double LV,double HV, double D){
  //start up - set relays
  digitalWrite(RLSTARTPin,HIGH); 
  //positive or negative? enable correct gate driver
  if(HV>=0){
    digitalWrite(ENSW1Pin,HIGH);
    digitalWrite(ENSW2Pin,LOW);
  }else{  
    digitalWrite(ENSW1Pin,LOW);
    digitalWrite(ENSW2Pin,HIGH);
  };
  //polarity of low voltage rail
  if(LV<0){digitalWrite(RLPOLPin,HIGH)};
  
  delay(100);      
  // fade in from min to max in increments of 5 points:
  int PWMValue = (int D)/100*255;
  println(PWMValue);
  for(int fadeValue = 0 ; fadeValue <= PWMValue; fadeValue +=5) {
    analogWrite(PWMPin, fadeValue);   
    delay(1000);   
  }                    
  int HVValue = abs(int HV)/HVMax*255
  println(HVValue);
  for(int fadeValue = 0 ; fadeValue <= HVValue; fadeValue +=5) {
    if(HV>=0){
      analogWrite(VREG1Pin, fadeValue);
    }else{
      analogWrite(VREG3Pin, fadeValue);
    };   
    delay(1000);   
  }                 
  int LVValue = abs(int LV)/LVMax*255
  println(LVValue);
  for(int fadeValue = 0 ; fadeValue <= HVValue; fadeValue +=5) {
    analogWrite(VREG2Pin, fadeValue);   
    delay(1000);   
  }
  ///
  //actual run
  delay(10000);
  ///
  //fade down
  for(int fadeValue = PWMValue ; fadeValue >=0 ; fadeValue -=5) {
    analogWrite(PWMPin, fadeValue);   
    delay(1000);   
  }  
  for(int fadeValue = HVValue ; fadeValue >=0 ; fadeValue -=5) {
    analogWrite(VREG1Pin, fadeValue);   
    delay(1000);   
  }                 
  for(int fadeValue = LVValue ; fadeValue >=0 ; fadeValue -=5) {
    analogWrite(VREG2Pin, fadeValue);   
    delay(1000);   
  }  
  //turn off
  digitalWrite(RLSTARTPin,LOW);  
  digitalWrite(ENSW1Pin,LOW);
  digitalWrite(ENSW2Pin,LOW);
  digitalWrite(RLPOLPin,LOW);
  delay(100);                        
}

void loop()  {                 
  double DV = 500;
  double CV = 10;
  double D = 25;
  double HV = DV + CV;
  println(HV);
  double LV = HV*D/(1-D)+CV ;
  println(LV);
  
  
}


