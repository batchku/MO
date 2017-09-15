//Change these values for debugging
#define NUM_SERVOS 0    //2
#define NUM_MOTORS 0    //2

const int channel = 1;

#include "MO.h"
#include "Arrays.h"
#include "MIDI.h"

#include "Shield.h"
#include "StripCollection.h"
#include "JoystickCollection.h"
#include "Strip.h"
#include "Sensor.h"
#include "Joystick.h"

#include <Servo.h>

Servo servo[NUM_SERVOS];

Shield shield;

Strip * strip1 = new Strip(0);
Strip * strip2 = new Strip(1);

Joystick joy1(0);
Joystick joy2(1);

//Add Sensors
Sensor * ir = new Sensor(POTS[4], POTS_MIDI[4]);
Sensor * ir2 = new Sensor(POTS[5], POTS_MIDI[5]);


StripCollection strips;
JoystickCollection joysticks;

void setup() {

  usbMIDI.setHandleNoteOff(OnNoteOff); //OnNoteOff function below
  usbMIDI.setHandleNoteOn(OnNoteOn);  //OnNoteOn function below
  usbMIDI.setHandleControlChange(OnControlChange);

  joysticks.add( &joy1);
  joysticks.add( &joy2);

  joysticks.calibrate();

  //Add Each strip to strips collection
  strips.add(strip1);
  strips.add(strip2);

  //Add additional sensors to shield
  shield.add(ir);
  shield.add(ir2);
}



void loop() {

  shield.update();
  strips.update();
  joysticks.update();

  usbMIDI.read();
}



void OnNoteOn(byte channel, byte note, byte velocity) {
  shield.light(note, velocity);
  strips.light(note, velocity);

  //INVERSE MOTOR DIRECTION
  for (int b = 0; b < NUM_MOTORS; b++) {
    if (note == MOTORINS_MIDI[b]) {
      analogWrite(MOTORINS[b], velocity * 2); //Send 0|1 based on 0-127 range
    }
  }

}

void OnNoteOff(byte channel, byte note, byte velocity) {
  shield.light(note, 0);
  strips.light(note, 0);

  //INVERSE MOTOR DIRECTION
  for (int b = 0; b < NUM_MOTORS; b++) {
    if (note == MOTORINS_MIDI[b]) {
      analogWrite(MOTORINS[b], 0); //Send 0|1 based on 0-127 range
    }
  }
}

void OnControlChange(byte channel, byte number, byte value) {
  //SERVO CONTROL
  for (int b = 0; b < NUM_SERVOS; b++) {
    if (number == SERVOS_MIDI[b]) {
      int val = map(value, 0, 127, 0, 180);
      servo[b].write(val);
    }
  }
  //MOTOR SPEED
  for (int b = 0; b < NUM_MOTORS; b++) {
    if (number == MOTORPWMS_MIDI[b]) {
      analogWrite(MOTORPWMS[b], value * 2);
    }
  }

}
