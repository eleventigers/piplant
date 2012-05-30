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
  Serial.println(tempCurrRead);   

  // wait 100 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(1000);        
}

  
