#include "Arduino.h"
#include "Shield.h"
#include "MO.h"
#include "Arrays.h"
#include "MIDI.h"

Shield::Shield()
{
  
  for (int b = 0; b < NUM_USERCONTROLS; b++) {
    pinMode(USERBUTTONS[b], INPUT_PULLUP);
    button[b].attach(USERBUTTONS[b]);
    button[b].interval(5);
  }

    for (int b = 0; b < NUM_USERCONTROLS; b++) {
    pinMode(LEDS[b], OUTPUT);
  }
  
}

void Shield::update()
{
  potInput();
  buttonInput();
}

void Shield::potInput()
{
      for (int b = 0; b < NUM_USERCONTROLS; b++) {
      int f = analogRead(USERPOTS[b]) / 8;
      if (((f + potPrevs[0][b] + potPrevs[1][b] + potPrevs[2][b]) / 4) != potSends[b])  {
        usbMIDI.sendControlChange(USERPOTS_MIDI[b], 127 - ((f + potPrevs[0][b] + potPrevs[1][b] + potPrevs[2][b]) / 4), 1);
        potSends[b] = (f + potPrevs[0][b] + potPrevs[1][b] + potPrevs[2][b]) / 4;
        potPrevs[2][b] = potPrevs[1][b];
        potPrevs[1][b] = potPrevs[0][b];
        potPrevs[0][b] = f;
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
