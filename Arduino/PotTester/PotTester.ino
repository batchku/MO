#include "MO.h"

int sensorPin = AN5;    // select the input pin for the potentiometer
int sensorPin2 = AN3; 
int buttonPin = JOYBTN2;
int ledPin = PWM13;      // select the pin for the LED
int ledPin2 = PWM14;
// Pin 13: Arduino has an LED connected on pin 13
// Pin 11: Teensy 2.0 has the LED on pin 11
// Pin  6: Teensy++ 2.0 has the LED on pin 6
// Pin 13: Teensy 3.0 has the LED on pin 13
int sensorValue = 0;  // variable to store the value coming from the sensor
int buttonValue = 0;

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // read the value from the sensor:
  readSensors();
  //blinkLed(ledPin, ledPin2);

}

void readSensors() {
  sensorValue = analogRead(sensorPin);
  buttonValue = digitalRead(buttonPin);
  Serial.print(sensorValue);
  Serial.print(" ,");
  Serial.println(buttonValue);

//  usbMIDI.sendControlChange(3, int(sensorValue/8), 1);
//  usbMIDI.sendControlChange(4, int(buttonValue/8), 1);
}

void blinkLed(int ledPin) {

  digitalWrite(ledPin, HIGH);
  delay(3000);
  digitalWrite(ledPin, LOW);
  delay(3000);
}

void blinkLed(int ledPin, int ledPinB) {

  digitalWrite(ledPin, HIGH);
  digitalWrite(ledPinB, HIGH);
  delay(3000);
  digitalWrite(ledPin, LOW);
  digitalWrite(ledPinB, LOW);
  delay(3000);
}
