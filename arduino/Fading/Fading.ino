
int PWMPin = 3;    
int VREGAMPPin = 5;   
int VREGBIASPin = 6;   
int RLPWMPin = 4;   
int RLHVSWPin = 7;   


void setup()  {
  pinMode(PWMPin,OUTPUT);    
  pinMode(VREGAMPPin,OUTPUT);   
  pinMode(VREGBIASPin,OUTPUT);  
  pinMode(RLPWMPin,OUTPUT);   
  pinMode(RLHVSWPin,OUTPUT);  

  pinMode(FLTCLRPin,OUTPUT);    
  pinMode(NFLTSDPin,INPUT);  
  pinMode(NSYFLTPin,INPUT);  
} 

void loop()  { 
  int PWMmax = 50;
  // fade in from min to max in increments of 5 points:
  for(int fadeValue = 0 ; fadeValue <= PWMmax; fadeValue +=5) {
    analogWrite(PWMPin, fadeValue);   
    delay(1000);   
    
  }       
  digitalWrite(RLPWMPin,LOW); 
  delay(100);   
  digitalWrite(RLPWMPin,HIGH); 
  delay(100);                      

  // fade out from max to min in increments of 5 points:
  for(int fadeValue = PWMmax ; fadeValue >= 0; fadeValue -=5) { 
    analogWrite(PWMPin, fadeValue);
    delay(1000
   );                            
  }       
  digitalWrite(RLPWMPin,LOW); 
  delay(100);     
  digitalWrite(RLPWMPin,HIGH); 
  delay(100);                    
}


