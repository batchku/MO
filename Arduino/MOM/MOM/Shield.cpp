#include "Arduino.h"
#include "MO.h"
#include "Arrays.h"
#include "MIDI.h"
#include "Shield.h"

Shield::Shield()
{
  msec = 0;
  for (int b = 0; b < NUM_USERCONTROLS; b++) {
    pinMode(USERBUTTONS[b], INPUT_PULLUP);
    button[b].attach(USERBUTTONS[b]);
    button[b].interval(5);
  }

  for (int b = 0; b < NUM_USERCONTROLS; b++) {
    pinMode(USERLEDS[b], OUTPUT);
  }

  for (int b = 0; b < NUM_USERCONTROLS; b++) {
    pots[b]->set(USERPOTS[b], USERPOTS_MIDI[b]);
    sensors.add(pots[b]);
  }

}

void Shield::update()
{
  potInput();
  buttonInput();
}

void Shield::add(Sensor *sensor) {
  sensors.add(sensor);
}

void Shield::potInput() {
  if (msec >= 20) {
    msec = 0;
    for (int b = 0; b < sensors.size(); b++) {
      sensors.get(b) -> update();
    }
  }
}

void Shield::buttonInput()
{
  for (int b = 0; b < NUM_USERCONTROLS; b++) {
    button[b].update();
    if (button[b].fallingEdge()) {
      usbMIDI.sendNoteOn(USERBUTTONS_MIDI[b], 127, 1);
    }
    if (button[b].risingEdge()) {
      usbMIDI.sendNoteOff(USERBUTTONS_MIDI[b], 0, 1);
    }
  }
}

void Shield::light(int note, int velocity) {

  for (int b = 0; b < NUM_USERCONTROLS; b++) {
    if (note == USERLEDS_MIDI[b]) {
      analogWrite(USERLEDS[b], velocity * 2);
    }
  }
}

