//Change these values for debugging
#define NUM_STRIPS 2
#define NUM_SERVOS 2    //2
#define NUM_MOTORS 2    //2

const int channel = 1;

#include "MO.h"
#include "Arrays.h"
#include "MIDI.h"

#include "Shield.h"
#include "StripCollection.h"
#include "Strip.h"
#include "Sensor.h"
//#include "Joystick.h"

#include <Servo.h>

Servo servo[NUM_SERVOS];

Shield shield;

Strip *strip1 = new Strip(0);
Strip *strip2 = new Strip(1);

//Joystick joy1(0);

//Add Sensors
Sensor *flex = new Sensor(POTS[4], POTS_MIDI[4]);

StripCollection strips;

void setup() {

  usbMIDI.setHandleNoteOff(OnNoteOff); //OnNoteOff function below
  usbMIDI.setHandleNoteOn(OnNoteOn);  //OnNoteOn function below
  usbMIDI.setHandleControlChange(OnControlChange);

  //Add Each strip to strips collection
  strips.add(strip1);
  strips.add(strip2);

  //Add additional sensors to shield
  shield.add(flex);
}


void OnNoteOn(byte channel, byte note, byte velocity) {
  shield.light(note, velocity);
  strips.light(note, velocity);

}

void OnNoteOff(byte channel, byte note, byte velocity) {
  shield.light(note, 0);
  strips.light(note, 0);
}

void OnControlChange(byte channel, byte number, byte value) {


}

void loop() {

  shield.update();
  strips.update();

  //joy1.update();

  usbMIDI.read();
}
