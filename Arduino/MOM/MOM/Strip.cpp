#include "Arduino.h"
#include "MO.h"
#include "Arrays.h"
#include "MIDI.h"
#include "Strip.h"

Strip::Strip(int _stripNum) {
  msec = 0;
  stripNum = _stripNum * 2;

  for (int b = 0; b < NUM_CONTROLS; b++) {
    pinMode(BUTTONS[stripNum + b], INPUT_PULLUP);
    button[b].attach(BUTTONS[stripNum + b]);
    button[b].interval(5);
  }

  for (int b = 0; b < NUM_CONTROLS; b++) {
    pinMode(LEDS[stripNum + b], OUTPUT);
  }

  for (int b = 0; b < NUM_CONTROLS; b++) {
    pots[b] -> set(POTS[stripNum + b], POTS_MIDI[stripNum + b]);
    sensors.add(pots[b]);
  }

}

void Strip::update()
{
  potInput();
  buttonInput();
}

void Strip::add(Sensor *sensor) {
  sensors.add(sensor);
}

void Strip::potInput() {
  if (msec >= 20) {
    msec = 0;
    for (int b = 0; b < sensors.size(); b++) {
      sensors.get(b) -> update();
    }
  }
}

void Strip::buttonInput()
{

  for (int b = 0; b < NUM_CONTROLS; b++) {
    button[b].update();
    if (button[b].fallingEdge()) {
      usbMIDI.sendNoteOn(BUTTONS_MIDI[stripNum + b], 127, 1);
    }
    if (button[b].risingEdge()) {
      usbMIDI.sendNoteOff(BUTTONS_MIDI[stripNum + b], 0, 1);
    }
  }

}

void Strip::light(int note, int velocity) {

  for (int b = 0; b < NUM_CONTROLS; b++) {
    if (note == LEDS_MIDI[stripNum + b]) {
      analogWrite(LEDS[stripNum + b], velocity * 2);
    }
  }

  if (note == STRIPLEDS_MIDI[stripNum / 2]) {
    analogWrite(STRIPLEDS[stripNum / 2], velocity * 2);
  }
}

