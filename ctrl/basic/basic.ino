/*
 
 */

const int moistSensPin = A0;  
const int tempSensPin = A1; 

const int pumpCtrlPin = 13;
const int lightCtrlPin = 12;

int moistCurrRead = 0;        
int tempCurrRead = 0;   

void setup() {
  
  delay (1000);
  
  pinMode(moistSensPin, INPUT);
  pinMode(tempSensPin, INPUT);
    
  pinMode(pumpCtrlPin, OUTPUT);
  pinMode(lightCtrlPin, OUTPUT);    

  Serial.begin(9600);   
  
}

void loop() {
  // read the analog in value:
  moistCurrRead = analogRead(moistSensPin);
  tempCurrRead = analogRead(tempSensPin);    

//  if (sensorValue <= 450) {
//    digitalWrite(13, HIGH); 
//  }
//  if (sensorValue > 850) {
//    digitalWrite(13, LOW); 
//  }
  
  // print the results to the serial monitor:
  Serial.print("moisture = " );                       
  Serial.print(moistCurrRead);      
  Serial.print("\t temperature = ");      
  Serial.println(LM335ATempConvert(tempCurrRead, 'C'));   

  // wait 100 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(1000);        
}

// >> temperature functions
// ---------------------------

// math learned from GreenRobotics in a comment on SparkFun's website
int LM335ATempConvert(int tempIn, char unitSystem){
  int KelvinC=273;
  int KelvinTemp = (long(tempIn) * 5 * 100) / 1023; // convert 
  int CelsiusTemp = KelvinTemp-KelvinC;
  int FahrenheitTemp = (CelsiusTemp)*(9/5)+32;
  int tempOut;

  switch(unitSystem){
  case 'K':
    tempOut = KelvinTemp;
    break;
  case 'C':
    tempOut = CelsiusTemp;
    break;
  case 'F':
    tempOut = FahrenheitTemp;
    break;
  }
  return tempOut;
}

// ---------------------------
// << temperature functions
// ---------------------------

  
