#include "Arduino.h"
#include "MO.h"
#include "Arrays.h"
#include "MIDI.h"
#include "Joystick.h"

Joystick::Joystick(int _joystickNum) {
  joystickNum = _joystickNum * 2;

  pinMode(JOYBUTTONS[joystickNum/2], INPUT_PULLUP);
  button -> attach(JOYBUTTONS[joystickNum/2]);
  button -> interval(5);

//  for (int b = 0; b < 2; b++) {
//    pots[b].set(JOYPOTS[b + joystickNum], JOYPOTS_MIDI[b + joystickNum]);
//  }
  pots[0].set(JOYPOTS[joystickNum/2], JOYPOTS_MIDI[joystickNum/2]);
  pots[1].set(JOYPOTS[joystickNum/2 + 2], JOYPOTS_MIDI[joystickNum/2 + 2]);
}

void Joystick::calibrate() {
  // read all current (center) values from the joy pots
  for (int b = 0; b < 2; b++) {
    for (int i = 0; i < 4; i++) {
      sample[i] = analogRead(JOYPOTS[b + joystickNum]) >> 3;
      delay(5);
      center[b] += sample[i];
    }
    center[b] *=  .25;
  }
}

void Joystick::update() {
  potInput();
  buttonInput();
}

void Joystick::potInput() {
  if (timer >= 20) {
    for (int b = 0; b < 2; b++) {
      pots[b].read();
      int current = pots[b].getCurrent();
      if (current <= center[b]) {
        current = map(current, 0, center[b], 0, 64);
        if( (b % 2) == 1) usbMIDI.sendControlChange(pots[b].getMidiAddress(), current, 1);
        if( (b % 2) == 0) usbMIDI.sendControlChange(pots[b].getMidiAddress(), 127 - current, 1);
        
      }
      if (current >= center[b]) {
        current = map(current, center[b], 127, 65, 127);
        if( (b % 2) == 1) usbMIDI.sendControlChange(pots[b].getMidiAddress(), current, 1);
        if( (b % 2) == 0) usbMIDI.sendControlChange(pots[b].getMidiAddress(), 127 - current, 1);
        
      }
    }
    timer = 0;
  }
}

void Joystick::buttonInput() {
  button -> update();
  if (button -> fallingEdge()) {
    usbMIDI.sendNoteOn(JOYBUTTONS_MIDI[joystickNum / 2], 127, 1);
  }
  if (button -> risingEdge()) {
    usbMIDI.sendNoteOff(JOYBUTTONS_MIDI[joystickNum / 2], 0, 1);
  }
}
