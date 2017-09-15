//Change these values for debugging
#define NUM_STRIPS 2
#define NUM_SERVOS 2    //2
#define NUM_MOTORS 2    //2

const int channel = 1;

#include "MO.h"
#include "Arrays.h"
#include "MIDI.h"

#include "Shield.h"


#include <Servo.h>

Servo servo[NUM_SERVOS];

Shield shield;

void setup() {

  usbMIDI.setHandleNoteOff(OnNoteOff); //OnNoteOff function below
  usbMIDI.setHandleNoteOn(OnNoteOn);  //OnNoteOn function below
  usbMIDI.setHandleControlChange(OnControlChange);
}


void OnNoteOn(byte channel, byte note, byte velocity) {
  shield.light(note, velocity);

}

void OnNoteOff(byte channel, byte note, byte velocity) {
  shield.light(note, 0);
}

void OnControlChange(byte channel, byte number, byte value) {


}

void loop() {

  shield.update();

  usbMIDI.read();
}
