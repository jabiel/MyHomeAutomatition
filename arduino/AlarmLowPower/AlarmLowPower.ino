#include <JeeLib.h>
#include <RCSwitch.h> // https://code.google.com/p/rc-switch/
#include <PinChangeInt.h>

RCSwitch mySwitch = RCSwitch(); 
ISR(WDT_vect) { Sleepy::watchdogEvent(); } // Setup for low power waiting


int calibrationTime = 20;        //the time we give the sensor to calibrate (10-60 secs according to the datasheet)
long unsigned int lowIn;         //the time when the sensor outputs a low impulse
long unsigned int pause = 5000;  //the amount of milliseconds the sensor has to be low before we assume all motion has stopped

boolean lockLow = true;
boolean takeLowTime;  
boolean pirUp;  

// Pins
int PIR = 2; 
//int LED = 13;
int RFTX = 10;   // Transmitter is connected to Arduino Pin #10  

int rfAlertCode = 666; // code send when motion detected

void setup(){
  Serial.begin(9600);
  pinMode(PIR, INPUT);
  //pinMode(LED, OUTPUT);
  //pinMode(BEEP, OUTPUT);
  mySwitch.enableTransmit(RFTX);

  //digitalWrite(LED, LOW);
  CalibratePirSensor();  //give the sensor some time to calibrate
  
  PRR = bit(PRTIM1);                           // only keep timer 0 going
  ADCSRA &= ~ bit(ADEN); bitSet(PRR, PRADC);   // Disable the ADC to save power
  PCintPort::attachInterrupt(PIR, wakeUp, CHANGE);
}

void wakeUp(){
}

void loop(){
  if (digitalRead(PIR) == HIGH) {
    Msg("Motion detected");
    mySwitch.send(rfAlertCode, 24);
    //digitalWrite(LED, HIGH);
  }
  
  Sleepy::powerDown();
}
  
void Msg(String msg)
{
    Serial.print("[");
    Serial.print((millis()/1000));
    Serial.print("] ");
    Serial.println(msg);
}

void CalibratePirSensor()
{
  Msg("Calibrating PIR sensor ");
  for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
   }
  Serial.println("");
  Msg("Sensor active");
  delay(50);
}

