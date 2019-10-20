// AUTHOR: CCA iGEM 
// BUILD ON ARDUINO GENUINO UNO

#include "dht.h"
#include "programmable_air.h"
#include <Stepper.h>
int stepCount = 0;  // number of steps the motor has taken

// temperature instantiation  
dht DHT;     // variable for the DHT temperature sensor
const int buttonPin = 2;     // the number of the pushbutton pin
#define DHT11_PIN 7

// heater instantiation
#define PIN_SWITCH 5

// button instantiation
int buttonState = 0;         // variable for reading the pushbutton status
String mode = "O2";         // mode of bioreactor; default is O2

// pressure instantiation
float setPressure; 
float tolerence;
int state;

// stepper motor instantiation for stirrer
const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

// potentiometer setup

void setup() {
  pinMode(buttonPin, INPUT); // initialize the pushbutton pin as an input
  initializePins(); // initilize pins for pressure maintenance using pumps
  pinMode(PIN_SWITCH, OUTPUT); // initialize the heater's pin
}

void loop() {
  // put your main code here, to run repeatedly:
  int chk = DHT.read11(DHT11_PIN);     // checks DHT sensor
  buttonState = digitalRead(buttonPin);
  float pressure = readPressure();
  if (buttonState == LOW) {
    if (mode == "O2") {
      mode = "N2"; // if mode is O2, set to N2 if button pressed
    }
    if (mode == "N2") {
      mode == "O2"; // if mode is N2, set to O2 if button pressed
    }
  }
  
  if (mode == "N2") {
    maintainPressure(pressure,2,.25,1); // maintain a pressure of 2 atm on pump 2 with a tolerance of 0.25 atm
    maintainTemperature(DHT.temperature, 22, 1); // maintain a temperature of 

    int sensorReading = analogRead(A0); // read the potentiometer's value
    int motorSpeed = map(sensorReading, 0, 1023, 0, 100); // map it to a range from 0 to 100:

    myStepper.setSpeed(motorSpeed); // set the stepper motor's speed
    myStepper.step(stepsPerRevolution / 100); // step 1/100 of a revolution
  }

  if (mode == "O2") {
    maintainPressure(pressure,2,.25,2); // maintain a pressure of 2 atm on pump 2 with a tolerance of 0.25 atm
    maintainTemperature(DHT.temperature, 22, 1); // maintain a temperature of 

    int sensorReading = analogRead(A0); // read the potentiometer's value
    int motorSpeed = map(sensorReading, 0, 1023, 0, 100); // map it to a range from 0 to 100:

    myStepper.setSpeed(motorSpeed); // set the stepper motor's speed
    myStepper.step(stepsPerRevolution / 100); // step 1/100 of a revolution
  }

  delay(100);

}

void maintainTemperature(int temperature, int setTemperature, int tolerance) {
  if (temperature > setTemperature + tolerence) {
    digitalWrite(PIN_SWITCH, LOW); //  heater off by setting power output to low
  }
  if (temperature < (setTemperature - tolerence)) {
    digitalWrite(PIN_SWITCH, HIGH); // heater on by setting power output to high
  }
}
 
void maintainPressure (int pressure, int setTemperature, int tolerance, int pump) {
  if (pressure > setPressure + tolerence) {
    vent(pump); // vent the pump
    state = DECREASING; // set the state to decreasing
    //    suck(pump);
  }
  else {
    if (pressure < (setPressure - tolerence)) {
      blow(pump); // turn on the pump
      state = INCREASING; // set the state to incresing
    }
    else {
      if (state == UN_KNOWN)
      {
        closeAll(pump); // close the pump due to malfunction
      }
    }
  }
  
  if (pressure < setPressure && state == DECREASING) {
  state = UN_KNOWN;
  }
  
  if (pressure > setPressure && state == INCREASING) {
  state = UN_KNOWN;
  }
}
