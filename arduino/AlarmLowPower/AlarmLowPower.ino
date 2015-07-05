#include <JeeLib.h>
#include <RCSwitch.h> // https://code.google.com/p/rc-switch/
#include <PinChangeInt.h>

RCSwitch rf = RCSwitch();
ISR(WDT_vect) {Sleepy::watchdogEvent();}  // Setup for low power waiting

int calibrationTime = 10;        //the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int rfPacketsToSend = 10; 

// Pins
int PIR = 2;
int LED = 13;
int RFTX = 10;   // Transmitter is connected to Arduino Pin #10

int rfAlertCode = 666; // code send when motion detected

void setup() {
  Serial.begin(9600);
  pinMode(PIR, INPUT);
  pinMode(LED, OUTPUT);
  rf.enableTransmit(RFTX);

  PRR = bit(PRTIM1);                           // only keep timer 0 going
  ADCSRA &= ~ bit(ADEN); bitSet(PRR, PRADC);   // Disable the ADC to save power
  PCintPort::attachInterrupt(PIR, wakeUp, CHANGE);
  
  CalibratePirSensor();  //give the sensor some time to calibrate
}

void loop() {
  if (digitalRead(PIR) == HIGH) {
    Serial.println("Motion detected");
    for (int i = 0; i < rfPacketsToSend; i++)
    {
      Serial.print(".");
      rf.send(rfAlertCode, 24);
    }
    Serial.println("");
    delay(100);
  }
  Sleepy::powerDown();
}

void wakeUp() {}

void CalibratePirSensor()
{
  int ll = HIGH;
  Serial.println("Calibrating PIR sensor ");
  for (int i = 0; i < calibrationTime; i++) {
    Serial.print(".");
    delay(1000);
    digitalWrite(LED, ll);
    ll = ll == HIGH ? LOW : HIGH;
  }
  Serial.println("");
  Serial.println("Sensor active");
  digitalWrite(LED, LOW);
  delay(50);
}

