#include "Arduino.h"
#include "MO.h"
#include "Arrays.h"
#include "MIDI.h"
#include "Joystick.h"

Joystick::Joystick(int _joystickNum) {
  joystickNum = _joystickNum * 2;

  pinMode(JOYBUTTONS[joystickNum / 2], INPUT_PULLUP);
  button -> attach(JOYBUTTONS[joystickNum / 2]);
  button -> interval(5);

  //  for (int b = 0; b < 2; b++) {
  //    pots[b].set(JOYPOTS[b + joystickNum], JOYPOTS_MIDI[b + joystickNum]);
  //  }
  pots[0].set(JOYPOTS[joystickNum / 2], JOYPOTS_MIDI[joystickNum / 2]);
  pots[1].set(JOYPOTS[joystickNum / 2 + 2], JOYPOTS_MIDI[joystickNum / 2 + 2]);
}

void Joystick::calibrate() {
  // read all current[b] (center) values from the joy pots
  int sample[4];
  for (int i = 0; i < 4; i++) {
    sample[i] = analogRead(JOYPOTS[joystickNum / 2]) >> 3;
    delay(5);
    center[0] += sample[i];
  }
  center[0] *=  .25;

  for (int i = 0; i < 4; i++) {
    sample[i] = analogRead(JOYPOTS[joystickNum / 2 + 2]) >> 3;
    delay(5);
    center[1] += sample[i];
  }
  center[1] *=  .25;
}

void Joystick::update() {
  potInput();
  buttonInput();
}

void Joystick::potInput() {
  //Only look at the pots every 30 millis
  if (timer >= 5) {
    //Iterate through the pots in a Joystick object
    for (int b = 0; b < 2; b++) {
      //Read the pot
      pots[b].read();
      //set current to be the pot value
      current[b] = pots[b].getCurrent();

      if (current[b] == center[b]) {
        if (lastSentVal[b] > center[b]) {
          lastSentVal[b]--;
          sendMIDI(b, map(lastSentVal[b],center[b], 127, 63, 127));
        }
        else if (lastSentVal[b] < center[b]) {
          lastSentVal[b]++;
          sendMIDI(b, map(lastSentVal[b], 0, center[b], 0, 63));
        }

      }
      else {
        //If the current value is less than the center by two...
        if (current[b] < center[b] - 2) {
          //send that value mapped to a MIDI equivalent
          sendMIDI(b, map(current[b], 0, center[b], 0, 64));
          //set lastSentVal to the value of current to track
          lastSentVal[b] = current[b];
        }
        //If the current value is greater than the center by two...
        else if (current[b] > center[b] + 2) {
          //send that value mapped to a MIDI equivalent
          sendMIDI(b, map(current[b], center[b], 127, 65, 127));
          //set lastSentVal to the value of current to track
          lastSentVal[b] = current[b];
        }
      }
    }
    //Reset Timer
    timer = 0;
  }
}

void Joystick::sendMIDI(int which, int val) {
  if ( (which % 2) == 1) usbMIDI.sendControlChange(pots[which].getMidiAddress(), val, 1);
  // Flip the MIDI output if its the vertical pot
  if ( (which % 2) == 0) usbMIDI.sendControlChange(pots[which].getMidiAddress(), 127 - val, 1);
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
