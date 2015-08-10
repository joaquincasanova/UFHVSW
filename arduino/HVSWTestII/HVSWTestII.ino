
int PWMPin = 3;   
//positive HV pin
int VREG1Pin = 5;
//bipolar LV pin
int VREG2Pin = 6;
//negative HV pin
int VREG3Pin = 9; 

//enable gate driver positive
int ENSW1Pin = 8;
//enable gate driver negative
int ENSW2Pin = 10;

double LVMax = 500;
double HVMax = 1000;
double DMax = 100;
double DMin = 0;

void setup()  {  
  // start //Serial port at 9600 bps and wait for port to open:
  Serial.begin(9600);
  //while (!Serial) {;} // wait for //Serial port to connect. Needed for Leonardo only
  //pin mapping
  pinMode(PWMPin,OUTPUT);    
  pinMode(VREG1Pin,OUTPUT);   
  pinMode(VREG2Pin,OUTPUT); 
  pinMode(VREG3Pin,OUTPUT);   
  pinMode(ENSW1Pin,OUTPUT);  
  pinMode(ENSW2Pin,OUTPUT);   
  
}

void reset()  {
  //set everything to zero
  analogWrite(PWMPin, 0);
  analogWrite(VREG1Pin, 0);
  analogWrite(VREG2Pin, 0);
  analogWrite(VREG3Pin, 0);
  digitalWrite(ENSW1Pin,LOW);  
  digitalWrite(ENSW2Pin,LOW);   
}

void experiment(double LV,double HV, double D) {

  int PWMValue = 0;
  PWMValue=((int) (DMax-D))/(DMax)*255;//it's opposite
  int HVValue = 0;
  HVValue=abs((int) HV)/HVMax*255;         
  int LVValue = 0;
  LVValue=abs((int) LV)/LVMax*255;
  Serial.println(PWMValue);
  Serial.println(HVValue);
  Serial.println(LVValue);
  delay(100);
  //positive or negative? enable correct gate driver

  if(HV>=0){
    digitalWrite(ENSW1Pin,HIGH);
    digitalWrite(ENSW2Pin,LOW);
  }else{  
    digitalWrite(ENSW1Pin,LOW);
    digitalWrite(ENSW2Pin,HIGH);
  };
  
  delay(100);      
  // fade in from min to max in increments of 5 points:
  
 
  Serial.println("PWM RAMP UP");
  for(int fadeValue = 0 ; fadeValue <= PWMValue; fadeValue +=1) {
    analogWrite(PWMPin, fadeValue);  
    delay(100);   
  }                    
  
  Serial.println("HV VREG RAMP UP");
  for(int fadeValue = 0 ; fadeValue <= HVValue; fadeValue +=1) {
    if(HV>=0){
      analogWrite(VREG1Pin, fadeValue);
    }else{
      analogWrite(VREG3Pin, fadeValue);
    }    
    delay(100);   
  }        
  Serial.println("LV VREG RAMP UP");
  for(int fadeValue = 0 ; fadeValue <= LVValue; fadeValue +=1) {
    analogWrite(VREG2Pin, fadeValue);
    delay(100);   
  }
  ///
  //actual run
  
  Serial.println("EXPERIMENT");
  delay(6000);
  ///
  //fade down
  
  Serial.println("HV VREG RAMP DOWN");
  for(int fadeValue = HVValue ; fadeValue >=0 ; fadeValue -=1) {
    if(HV>=0){
      analogWrite(VREG1Pin, fadeValue);
    }else{
      analogWrite(VREG3Pin, fadeValue);
    } 
    delay(100);   
  }        
  Serial.println("LV VREG RAMP DOWN");         
  for(int fadeValue = LVValue ; fadeValue >=0 ; fadeValue -=1) {
    analogWrite(VREG2Pin, fadeValue); 
    delay(100);   
  }  
  Serial.println("PWM RAMP DOWM");
  for(int fadeValue = PWMValue ; fadeValue >=0 ; fadeValue -=1) {
    analogWrite(PWMPin, fadeValue);
    delay(100);   
  } 
  //turn off
  digitalWrite(ENSW1Pin,LOW);
  digitalWrite(ENSW2Pin,LOW);
  delay(100);                        
}

void loop()  {                 
  double DV = 100;
  double CV = 0;
  double D = 50;
  double HV = DV + CV;
  Serial.println("HV:");
  Serial.println(HV);
  double LV = -HV*D/(100-D)+CV ;
  Serial.println("LV:");
  Serial.println(LV);
  delay(100);
  if(abs(HV-LV)>1000){
    Serial.println("Voltage out of range");    
    delay(10000);
    reset();
  }else{
    reset();
    Serial.println("Run :");
    experiment(LV, HV, D);
    reset();
  }                
  DV = -100;
  HV = DV + CV;
  Serial.println("HV:");
  Serial.println(HV);
  LV = -HV*D/(100-D)+CV ;
  Serial.println("LV:");
  Serial.println(LV);
  delay(10000);
  if(abs(HV-LV)>1000){
    Serial.println("Voltage out of range");    
    delay(10000);
    reset();
  }else{
    reset();
    Serial.println("Run:");
    experiment(LV, HV, D);
    reset();
  }
}

