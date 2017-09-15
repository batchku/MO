//Change these values for debugging
#define NUM_POTS 0     //12
#define NUM_USERPOTS 4  //4
#define NUM_USERINS 0   //4
#define NUM_BUTTONS 0     //12
#define NUM_USERBUTTONS 4  //4
#define NUM_LEDS 0     //16 
#define NUM_USERLEDS 4   //10
#define NUM_JOYPOTS 0   //4
#define NUM_JOY_BUTTONS 0  //2
#define NUM_SERVOS 0    //2
#define NUM_MOTORS 0    //2

#include "MOD.h"
#include "Arrays.h"
#include "MIDI.h"
#include "Filters.h"
#include <Servo.h>

Servo servo[NUM_SERVOS];

elapsedMillis msec = 0;

void setup() {

  for (int b = 0; b < NUM_POTS; b++) {
    potPrevs[0][b] = -1;
  } // set all previous pot values to -1 at startup

  for (int b = 0; b < NUM_JOYPOTS; b++) {
    joyPotPrevs[0][b] = -1;
  } // set all previous joystick potentiometer values to -1 at startup

  for (int b = 0; b < NUM_JOYPOTS; b++) {
    int read1 = analogRead(JOYPOTS[b]) / 8;
    delay(5);
    int read2 = analogRead(JOYPOTS[b]) / 8;
    delay(5);
    int read3 = analogRead(JOYPOTS[b]) / 8;
    delay(5);
    int read4 = analogRead(JOYPOTS[b]) / 8;
    delay(5);
    joyPotCenters[b] = (read1 + read2 + read3 + read4) / 4;
  } // read all current (center) values from the joy pots

  for (int b = 0; b < NUM_JOY_BUTTONS; b++) {
    pinMode(JOYBUTTONS[b], INPUT_PULLDOWN);
    joyButtDebouncer[b].attach(JOYBUTTONS[b]);
    joyButtDebouncer[b].interval(5);
  } //set up button pins and attach them to Bounce objects and set time interals


  for (int b = 0; b < NUM_BUTTONS; b++) {
    pinMode(BUTTONS[b], INPUT_PULLUP);
    buttDebouncer[b].attach(BUTTONS[b]);
    buttDebouncer[b].interval(5);
  } //set up button pins and attach them to Bounce objects and set time interals

  for (int b = 0; b < NUM_USERBUTTONS; b++) {
    pinMode(USERBUTTONS[b], INPUT_PULLUP);
    userButtDebouncer[b].attach(USERBUTTONS[b]);
    userButtDebouncer[b].interval(5);
  }

  for (int b = 0; b < NUM_SERVOS; b++) {
    servo[b].attach(SERVOS[b]);
  }

  for (int b = 0; b < NUM_MOTORS; b++) {
    pinMode(MOTORINS[b], OUTPUT);
  }

  for (int b = 0; b < NUM_LEDS; b++) {
    pinMode(LEDS[b], OUTPUT);
  }

  usbMIDI.setHandleNoteOff(OnNoteOff); //OnNoteOff function below
  usbMIDI.setHandleNoteOn(OnNoteOn);  //OnNoteOn function below
  usbMIDI.setHandleControlChange(OnControlChange);

}

void buttonInput() {

  for (int b = 0; b < NUM_JOY_BUTTONS; b++) {
    joyButtDebouncer[b].update();
    if (joyButtDebouncer[b].fallingEdge()) {
      usbMIDI.sendNoteOff(JOYBUTTONS_MIDI[b], 0, channel);
    }
    if (joyButtDebouncer[b].risingEdge()) {
      usbMIDI.sendNoteOn(JOYBUTTONS_MIDI[b], 127, channel);
    }
  }


  for (int b = 0; b < NUM_BUTTONS; b++) {
    buttDebouncer[b].update();
    if (buttDebouncer[b].fallingEdge()) {
      usbMIDI.sendNoteOn(BUTTONS_MIDI[b], 127, channel);
    }
    if (buttDebouncer[b].risingEdge()) {
      usbMIDI.sendNoteOff(BUTTONS_MIDI[b], 0, channel);
    }
  }

  for (int b = 0; b < NUM_USERBUTTONS; b++) {
    userButtDebouncer[b].update();
    if (userButtDebouncer[b].fallingEdge()) {
      usbMIDI.sendNoteOn(USERBUTTONS_MIDI[b], 127, channel);
    }
    if (userButtDebouncer[b].risingEdge()) {
      usbMIDI.sendNoteOff(USERBUTTONS_MIDI[b], 0, channel);
    }
  }

}

void potInput() {

  if (msec >= 20) {
    msec = 0;


    for (int b = 0; b < NUM_POTS; b++) {
      int f = analogRead(POTS[b]) / 8;
      //      if ( (f != potPrevs[b]) ) {
      if (((f + potPrevs[0][b] + potPrevs[1][b] + potPrevs[2][b]) / 4) != potSends[b])  {
        usbMIDI.sendControlChange(POTS_MIDI[b], 127 - ((f + potPrevs[0][b] + potPrevs[1][b] + potPrevs[2][b]) / 4), channel);
        potSends[b] = (f + potPrevs[0][b] + potPrevs[1][b] + potPrevs[2][b]) / 4;
        potPrevs[2][b] = potPrevs[1][b];
        potPrevs[1][b] = potPrevs[0][b];
        potPrevs[0][b] = f;
      }
    } // read the pots

    for (int b = 0; b < NUM_USERPOTS; b++) {
      int f = analogRead(USERPOTS[b]) >> 3;
      usbMIDI.sendControlChange(USERPOTS_MIDI[b], f, channel);
      //      if ( (f != potPrevs[b]) ) {
//      if (((f + userPotPrevs[0][b] + userPotPrevs[1][b] + userPotPrevs[2][b]) / 4) != userPotSends[b])  {
//        usbMIDI.sendControlChange(USERPOTS_MIDI[b], ((f + userPotPrevs[0][b] + userPotPrevs[1][b] + userPotPrevs[2][b]) / 4), channel);
//        
//        userPotSends[b] = (f + userPotPrevs[0][b] + userPotPrevs[1][b] + userPotPrevs[2][b]) / 4;
//        userPotPrevs[2][b] = userPotPrevs[1][b];
//        userPotPrevs[1][b] = userPotPrevs[0][b];
//        userPotPrevs[0][b] = f;
//      }
    } // read the pots


    for (int b = 0; b < NUM_USERINS; b++) {
      int f = analogRead(USERINS[b]) / 8;
      //      if ( (f != potPrevs[b]) ) {
      if (((f + potPrevs[0][b] + potPrevs[1][b] + potPrevs[2][b]) / 4) != potSends[b])  {
        usbMIDI.sendControlChange(USERINS_MIDI[b], 127 - ((f + potPrevs[0][b] + potPrevs[1][b] + potPrevs[2][b]) / 4), channel);
        potSends[b] = (f + potPrevs[0][b] + potPrevs[1][b] + potPrevs[2][b]) / 4;
        potPrevs[2][b] = potPrevs[1][b];
        potPrevs[1][b] = potPrevs[0][b];
        potPrevs[0][b] = f;
      }
    } // read the pots

    for (int b = 0; b < NUM_JOYPOTS; b++) {
      int f = analogRead(JOYPOTS[b]) / 8;
      int testVal = (f + joyPotPrevs[0][b] + joyPotPrevs[1][b] + joyPotPrevs[2][b]) / 4;
      if ( (testVal <= joyPotSends[b] - 2) | (testVal >= joyPotSends[b] + 2)) {
        if (f <= joyPotCenters[b]) {

          f = map(f, 0, joyPotCenters[b], 0, 64);
          if ( (b % 2) == 1 ) {
            usbMIDI.sendControlChange(JOYPOTS_MIDI[b], (f + joyPotPrevs[0][b] + joyPotPrevs[1][b] + joyPotPrevs[2][b]) / 4, channel);
          }
          if ( (b % 2) == 0 ) {
            usbMIDI.sendControlChange(JOYPOTS_MIDI[b], 127 - ((f + joyPotPrevs[0][b] + joyPotPrevs[1][b] + joyPotPrevs[2][b]) / 4), channel);
          }
          joyPotSends[b] = ((f + joyPotPrevs[0][b] + joyPotPrevs[1][b] + joyPotPrevs[2][b]) / 4);
          joyPotPrevs[2][b] = joyPotPrevs[1][b];
          joyPotPrevs[1][b] = joyPotPrevs[0][b];
          joyPotPrevs[0][b] = f;
        }

        if (f > joyPotCenters[b]) {

          f = map(f, joyPotCenters[b], 127, 65, 127);
          if ( (b % 2) == 1 ) {
            usbMIDI.sendControlChange(JOYPOTS_MIDI[b], (f + joyPotPrevs[0][b] + joyPotPrevs[1][b] + joyPotPrevs[2][b]) / 4, channel);
          }
          if ( (b % 2) == 0 ) {
            usbMIDI.sendControlChange(JOYPOTS_MIDI[b], 127 - ((f + joyPotPrevs[0][b] + joyPotPrevs[1][b] + joyPotPrevs[2][b]) / 4), channel);
          }
          joyPotSends[b] = ((f + joyPotPrevs[0][b] + joyPotPrevs[1][b] + joyPotPrevs[2][b]) / 4);
          joyPotPrevs[2][b] = joyPotPrevs[1][b];
          joyPotPrevs[1][b] = joyPotPrevs[0][b];
          joyPotPrevs[0][b] = f;
        }
      }
    }
  }
}

void OnNoteOn(byte channel, byte note, byte velocity)
{

  for (int b = 0; b < NUM_LEDS; b++) {
    if (note == LEDS_MIDI[b]) {
      analogWrite(LEDS[b], velocity * 2);
    }
  }

  for (int b = 0; b < NUM_USERLEDS; b++) {
    if (note == USERLEDS_MIDI[b]) {
      analogWrite(USERLEDS[b], velocity * 2);
    }
  }

  //INVERSE MOTOR DIRECTION
  for (int b = 0; b < NUM_MOTORS; b++) {
    if (note == MOTORINS_MIDI[b]) {
      //int val = map(value, 0, 127, LOW, HIGH);
      //digitalWrite(MOTORINS[b], value + 1 >> 7); //Send 0|1 based on 0-127 range
      analogWrite(MOTORINS[b], velocity * 2); //Send 0|1 based on 0-127 range
    }
  }

}

void OnNoteOff(byte channel, byte note, byte velocity)
{

  for (int b = 0; b < NUM_LEDS; b++) {
    if (note == LEDS_MIDI[b]) {
      analogWrite(LEDS[b], 0);
    }
  }

  for (int b = 0; b < NUM_USERLEDS; b++) {
    if (note == USERLEDS_MIDI[b]) {
      analogWrite(USERLEDS[b], 0);
    }
  }

  //INVERSE MOTOR DIRECTION
  for (int b = 0; b < NUM_MOTORS; b++) {
    if (note == MOTORINS_MIDI[b]) {
      //int val = map(value, 0, 127, LOW, HIGH);
      //digitalWrite(MOTORINS[b], value + 1 >> 7); //Send 0|1 based on 0-127 range
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

void loop() {

  //---------for sending midi---------
  //process buttons
  buttonInput();

  //process pots
  potInput();

  //--------for receiving midi---------


  // MIDI Controllers should discard incoming MIDI messages.
  // http://forum.pjrc.com/threads/24179-Teensy-3-Ableton-Analog-CC-causes-midi-crash
  usbMIDI.read();
}
