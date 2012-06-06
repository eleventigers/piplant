/*
 
 */

const int moistSensPin = A0;  
const int tempSensPin = A1; 

const int pumpCtrlPin = 13;
const int lightCtrlPin = 12;
 
const int tempSmoothSteps = 10;
const int tempSensRate = 3000;
float tempSensRaw, tempSensCurr, tempSensHist[tempSmoothSteps];

const int moistSmoothSteps = 10;
const int moistSensRate = 7000;
const int moistLow = 550;
const int moistHigh = 620;
float moistSensRaw, moistSensCurr, moistSensHist[moistSmoothSteps];

const int pumpCycle = 5000;
const int pumpInitDelay = 20000;

const int updateRate = 1000;

unsigned long currentTime, tempSensLast, moistSensLast, updateLast, pumpOnStart;

int pumpOn = 0;
int sensRead = 0;

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
  
  if (abs(currentTime - tempSensLast) >= tempSensRate && sensRead != 1) {
    tempRead();  
  } 
  
  if (abs(currentTime - moistSensLast) >= moistSensRate && pumpOn != 1 && sensRead != 1) {  
    moistRead();  
  } 
  
  if (abs(currentTime - updateLast) >= updateRate) {  
    updateServer();  
  } 
  
  if (moistSensCurr <= moistLow && pumpOn == 0 && currentTime > pumpInitDelay){
    pumpOnStart = currentTime;
    digitalWrite(pumpCtrlPin, HIGH);
    pumpOn = 1;
   // Serial.println("pump on");
  }
  
  if (pumpOn == 1) {
    if (abs(currentTime - pumpOnStart) >= pumpCycle){
      if (abs(currentTime - moistSensLast) >= moistSensRate){  
        moistRead();  
      } 
      if (moistSensCurr >= moistHigh){
        digitalWrite(pumpCtrlPin, LOW);
        pumpOn = 0;
       // Serial.print("\t pump off");   
      }
    } 
  }
  
}

void moistRead(){
  
    sensRead = 1;
  
    moistSensLast = currentTime;
    
    moistSensRaw = analogRead(moistSensPin);
    moistSensCurr = moistSensRaw;
    for(int i=moistSmoothSteps-1; i>0; i--){
      moistSensHist[i] = moistSensHist[i-1];
      moistSensCurr += moistSensHist[i];
    }
    moistSensHist[0] = moistSensRaw;
    moistSensCurr = moistSensCurr / moistSmoothSteps;  
   
    sensRead = 0;
  
}

void tempRead(){
  
    sensRead = 1;
  
    tempSensLast = currentTime;
    
    tempSensRaw = getTemp(tempSensPin);
    tempSensCurr = tempSensRaw;
    for(int i=tempSmoothSteps-1; i>0; i--){
      tempSensHist[i] = tempSensHist[i-1];
      tempSensCurr += tempSensHist[i];
    }
    tempSensHist[0] = tempSensRaw;
    tempSensCurr = tempSensCurr / tempSmoothSteps;  
   
    sensRead = 0;
}

void updateServer(){
  
  updateLast = currentTime;
  
  Serial.print("\t t");
  Serial.println(tempSensCurr);
  Serial.print("\t m");
  Serial.println(moistSensCurr);  
  
}

// >> temperature functions
// ---------------------------


float getTemp(int tempPin){
  
  float temperature = getVoltage(tempPin);
  temperature = (temperature - .5) * 100;
  
  return temperature;
}

float getVoltage(int pin){
    return (analogRead(pin) * .005);//4882814);
  }

// ---------------------------
// << temperature functions
// ---------------------------

  

