#include <RCSwitch.h> // https://code.google.com/p/rc-switch/
RCSwitch mySwitch = RCSwitch(); 

int calibrationTime = 20;        //the time we give the sensor to calibrate (10-60 secs according to the datasheet)
long unsigned int lowIn;         //the time when the sensor outputs a low impulse
long unsigned int pause = 5000;  //the amount of milliseconds the sensor has to be low before we assume all motion has stopped

boolean lockLow = true;
boolean takeLowTime;  
boolean pirUp;  

// Pins
int PIR = 2; 
int LED = 13;
int BEEP = 6;    // optional beeper
int RFTX = 10;   // Transmitter is connected to Arduino Pin #10  

int rfAlertCode = 666; // code send when motion detected

void setup(){
  Serial.begin(9600);
  pinMode(PIR, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(BEEP, OUTPUT);
  mySwitch.enableTransmit(RFTX);

  digitalWrite(LED, LOW);
  CalibratePirSensor();  //give the sensor some time to calibrate
}

void loop(){
    bool movementDetected = (digitalRead(PIR) == HIGH);
    
    if(movementDetected)
    {
        digitalWrite(LED, HIGH);
        if(lockLow)
        {  
           //makes sure we wait for a transition to LOW before any further output is made:
           lockLow = false;            
           Msg("Motion detected");
           delay(50);
        }
        takeLowTime = true;
    } else 
    {
        digitalWrite(LED, LOW); 

        if(takeLowTime){
            lowIn = millis();          //save the time of the transition from high to LOW
            takeLowTime = false;       //make sure this is only done at the start of a LOW phase
       }
       if(!lockLow && millis() - lowIn > pause){  
           //makes sure this block of code is only executed again after 
           //a new motion sequence has been detected
           lockLow = true;
           Msg("Motion ended");
           delay(50);
       }
    }

    BeepIt();
}
  
void Msg(String msg)
{
    Serial.print("[");
    Serial.print((millis()/1000));
    Serial.print("] ");
    Serial.println(msg);
}

int beepCnt = 0;
boolean beepLow = false;

void BeepIt()
{
  if(!lockLow)
  {
    /*
    beepCnt++;
    if(beepCnt > 5000)
    {
      beepLow =!beepLow;
      beepCnt = 0;
    }
    //analogWrite(beepPin, (beepCnt/100));
    
    if(beepLow)
      analogWrite(beepPin, 100);
    else 
      analogWrite(beepPin, 250);
    */
      mySwitch.send(rfAlertCode, 24);
  } else
  {
      beepCnt = 0;
      analogWrite(BEEP, 0);
  }
}

void CalibratePirSensor()
{
  Msg("Starting ");
  for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
   }
  Serial.println("");
  Msg("SENSOR ACTIVE");
  delay(50);
}

