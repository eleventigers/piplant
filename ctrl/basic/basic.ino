/*
 
 */

const int moistSensPin = A0;  
const int tempSensPin = A1; 

const int pumpCtrlPin = 13;
const int lightCtrlPin = 12;
 
const int tempSmoothSteps = 10;
const int tempSensRate = 1000;
float tempSensRaw, tempSensCurr, tempSensHist[tempSmoothSteps];

const int moistSmoothSteps = 10;
const int moistSensRate = 2000;
const int moistLow = 450;
const int moistHigh = 850;
float moistSensRaw, moistSensCurr, moistSensHist[moistSmoothSteps];

const int pumpCycle = 10000;

unsigned long currentTime, tempSensLast, moistSensLast, pumpOnStart;

int pumpOn = 0;

void setup() {
  
  delay (1000);
  
  pinMode(moistSensPin, INPUT);
  pinMode(tempSensPin, INPUT);
    
  pinMode(pumpCtrlPin, OUTPUT);
  pinMode(lightCtrlPin, OUTPUT);    

  Serial.begin(9600);   
  
}

void loop() {
  
  currentTime = millis();
  
  if (abs(currentTime - tempSensLast) >= tempSensRate) {
    tempRead();  
  } 
  
  if (abs(currentTime - moistSensLast) >= moistSensRate && pumpOn != 1) {  
    moistRead();  
  } 
  
  if (moistSensCurr <= moistLow && pumpOn == 0){
    pumpOnStart = currentTime;
    digitalWrite(pumpCtrlPin, HIGH);
    pumpOn = 1;
    Serial.print("\t pump on");
  }
  
  if (pumpOn == 1) {
    if (abs(currentTime - pumpOnStart) >= pumpCycle){
      if (abs(currentTime - moistSensLast) >= moistSensRate){  
        moistRead();  
      } 
      if (moistSensCurr >= moistHigh){
        digitalWrite(pumpCtrlPin, LOW);
        pumpOn = 0;
        Serial.print("\t pump off");   
      }
    } 
  }
  
}

void moistRead(){
  
    moistSensLast = currentTime;
    
    moistSensRaw = analogRead(moistSensPin);
    moistSensCurr = moistSensRaw;
    for(int i=moistSmoothSteps-1; i>0; i--){
      moistSensHist[i] = moistSensHist[i-1];
      moistSensCurr += moistSensHist[i];
    }
    moistSensHist[0] = moistSensRaw;
    moistSensCurr = moistSensCurr / moistSmoothSteps;  
   
    Serial.print("\t moisture = ");      
    Serial.println(moistSensCurr); 
  
}

void tempRead(){
  
    tempSensLast = currentTime;
    
    tempSensRaw = getTemp(tempSensPin);
    tempSensCurr = tempSensRaw;
    for(int i=tempSmoothSteps-1; i>0; i--){
      tempSensHist[i] = tempSensHist[i-1];
      tempSensCurr += tempSensHist[i];
    }
    tempSensHist[0] = tempSensRaw;
    tempSensCurr = tempSensCurr / tempSmoothSteps;  
   
    Serial.print("\t temperature = ");      
    Serial.println(tempSensCurr);   
     
}

// >> temperature functions
// ---------------------------


float getTemp(int tempPin){
  
  float temperature = getVoltage(tempPin);
  temperature = (temperature - .5) * 100;
  
  return temperature;
}

float getVoltage(int pin){
    return (analogRead(pin) * .004882814);
  }

// ---------------------------
// << temperature functions
// ---------------------------

  
