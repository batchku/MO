#include "Joystick.h"
#include "MO.h"
#include "Arrays.h"
#include "MIDI.h"
#include "Sensor.h"


Joystick::Joystick(int _joystickNum) {
  joystickNum = _joystickNum;

  pinMode(JOYBUTTONS[joystickNum], INPUT_PULLUP);
  button -> attach(JOYBUTTONS[joystickNum]);
  button -> interval(5);

  for (int b = 0; b < 2; b++) {
    pots[b] -> set(JOYPOTS[(joystickNum * 2) + b], JOYPOTS_MIDI[(joystickNum * 2) + b]);
    sensors.add(pots[b]);
  }

  // read all current (center) values from the joy pots
  for (int b = 0; b < 2; b++) {
    for (int i = 0; b < 4; i++) {
      sample[i] = analogRead(JOYPOTS[b]) / 8;
      delay(5);
      center[b] += sample[i];
    }
    center[b] *= .25;
  }
}

void Joystick::update() {
  potInput();
  buttonInput();
}

void Joystick::potInput() {
  for (int b = 0; b < 2; b++) {

    pots[b] -> read();
    int current = pots[b] -> getVal();

    if ( (current <= pots[b] -> getLastVal() - 2) | (current >= pots[b] -> getLastVal() + 2)) {
      if (current <= center[b]) {

        current = map(current, 0, center[b], 0, 64);
        if ( (b % 2) == 1 ) {
          usbMIDI.sendControlChange(JOYPOTS_MIDI[b], current, 1);
        }
        if ( (b % 2) == 0 ) {
          usbMIDI.sendControlChange(JOYPOTS_MIDI[b], 127 - current, 1);
        }
        pots[b] -> read();
      }

    }

    if (current > center[b]) {

      current = map(current, center[b], 127, 65, 127);
      if ( (b % 2) == 1 ) {
        usbMIDI.sendControlChange(JOYPOTS_MIDI[b], current, 1);
      }
      if ( (b % 2) == 0 ) {
        usbMIDI.sendControlChange(JOYPOTS_MIDI[b], 127 - current, 1);
      }
      pots[b] -> read();
    }
  }
}

void Joystick::buttonInput() {
  button -> update();
  if (button -> fallingEdge()) {
    usbMIDI.sendNoteOn(JOYBUTTONS_MIDI[joystickNum], 127, 1);
  }
  if (button -> risingEdge()) {
    usbMIDI.sendNoteOff(JOYBUTTONS_MIDI[joystickNum], 0, 1);
  }
}

