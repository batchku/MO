#include <Bounce2.h>


const int channel = 1; // the MIDI channel number to send messages

const int ledPins[] = {9, 10, 2, 3, 4, 5, 6, 7, 8, 11}; //led pin numbers
const int ledNotes[] = {60, 61, 65, 63, 64, 65, 62, 67, 68, 69}; //note numbers for leds
const int leds = sizeof(ledPins)/sizeof(leds); //used for loops

const int faderPins[] = {28, 19, 22, A10, 21, 23, A12, A11, A13, A6}; //fader pins
const int faderCtls[] = {10, 11, 14, 15, 17, 16, 18, 12, 19, 13}; //fader MIDI CC #s
const int faders = sizeof(faderPins)/sizeof(faders); //used for loops
int faderPrevs[faders]; //used to store previous values
int faderPrevs2[faders]; //more previous values
int faderPrevs3[faders]; //more more previous values
int faderSends[faders];

const int joyPotPins[] = {31, 14, A16, A15}; //pins
const int joyPotCtls[] = {21, 20, 23, 22}; //CCs
const int joyPots = sizeof(joyPotPins)/sizeof(joyPots); //for loops
int joyPotPrevs[joyPots]; //previous values
int joyPotPrevs2[joyPots]; //more previous values
int joyPotPrevs3[joyPots]; //more more previous values
int joyPotCenters[joyPots]; //center value of joystick pots
int joyPotSends[joyPots];

const int joyButtonPins[] = {24, 12}; //pins
const int joyButtonNotes [] = {65, 66}; //note numbers
const int joyButtons = sizeof(joyButtonPins)/sizeof(joyButtons); // for loops
Bounce * joyButtDebouncer = new Bounce[joyButtons]; // Create Bounce objects for each button.

const int buttonPins[] = {0, 1}; //pins
const int buttonNotes[] = {61, 62}; //note nums
const int buttons = sizeof(buttonPins)/sizeof(buttons); //for loops
Bounce * buttDebouncer = new Bounce[buttons]; // Create Bounce objects for each button.

elapsedMillis msec = 0;


void setup() {

 // analogWrite(ledPins[0], 127);
 // analogWrite(ledPins[1], 127);
  
  for (int b = 0; b < faders; b++) {
    faderPrevs[b] = -1;
  } // set all previous fader values to -1 at startup

  for (int b = 0; b < joyPots; b++) {
    joyPotPrevs[b] = -1;
  } // set all previous joystick potentiometer values to -1 at startup

  for (int b = 0; b < joyPots; b++) {
    int read1 = analogRead(joyPotPins[b]) / 8;
    delay(5);
    int read2 = analogRead(joyPotPins[b]) / 8;
    delay(5);
    int read3 = analogRead(joyPotPins[b]) / 8;
    delay(5);
    int read4 = analogRead(joyPotPins[b]) / 8;
    delay(5);
    joyPotCenters[b] = (read1+read2+read3+read4)/4;  
  } // read all current (center) values from the joy pots

  for (int b = 0; b < joyButtons; b++) {
    pinMode(joyButtonPins[b], INPUT_PULLUP);
    joyButtDebouncer[b].attach(joyButtonPins[b]);
    joyButtDebouncer[b].interval(5);
  } //set up button pins and attach them to Bounce objects and set time interals


  for (int b = 0; b < buttons; b++) {
    pinMode(buttonPins[b], INPUT_PULLUP);
    buttDebouncer[b].attach(buttonPins[b]);
    buttDebouncer[b].interval(5);
  } //set up button pins and attach them to Bounce objects and set time interals

  usbMIDI.setHandleNoteOff(OnNoteOff); //OnNoteOff function below
  usbMIDI.setHandleNoteOn(OnNoteOn);  //OnNoteOn function below
  
}

void buttonInput(){

  for (int b = 0; b < joyButtons; b++) {
    joyButtDebouncer[b].update();
  if (joyButtDebouncer[b].fallingEdge()) {
    usbMIDI.sendNoteOn(joyButtonNotes[b], 127, channel);  
  }
  if (joyButtDebouncer[b].risingEdge()) {
    usbMIDI.sendNoteOff(joyButtonNotes[b], 0, channel); 
  }
  }


  for (int b = 0; b < buttons; b++) {
    buttDebouncer[b].update();
  if (buttDebouncer[b].fallingEdge()) {
    usbMIDI.sendNoteOn(buttonNotes[b], 127, channel);  
  }
  if (buttDebouncer[b].risingEdge()) {
    usbMIDI.sendNoteOff(buttonNotes[b], 0, channel); 
  }
  }
  
}

void faderInput(){

  if (msec >= 20) {
    msec = 0;

    
    for (int b = 0; b < faders; b++) {
      int f = analogRead(faderPins[b]) / 8;
//      if ( (f != faderPrevs[b]) ) {
      if (((f + faderPrevs[b] + faderPrevs2[b] + faderPrevs3[b]) / 4) != faderSends[b])  {        
        usbMIDI.sendControlChange(faderCtls[b], 127 - ((f + faderPrevs[b] + faderPrevs2[b] + faderPrevs3[b]) / 4), channel);
        faderSends[b] = (f + faderPrevs[b] + faderPrevs2[b] + faderPrevs3[b]) / 4;
        faderPrevs3[b] = faderPrevs2[b];
        faderPrevs2[b] = faderPrevs[b];
        faderPrevs[b] = f;
      }
    } // read the pots


    
    for (int b = 0; b < joyPots; b++) {
      int f = analogRead(joyPotPins[b]) / 8;
//      if (f != joyPotPrevs[b]) {
      if (((f + joyPotPrevs[b] + joyPotPrevs2[b] + joyPotPrevs3[b]) / 4) != joyPotSends[b]) {        
        if (f <= joyPotCenters[b]){
           
          f = map(f, 0, joyPotCenters[b], 0, 64);
          if ( (b % 2) == 1 ){
          usbMIDI.sendControlChange(joyPotCtls[b], (f + joyPotPrevs[b] + joyPotPrevs2[b] + joyPotPrevs3[b]) / 4, channel);
          }
          if ( (b % 2) == 0 ){
          usbMIDI.sendControlChange(joyPotCtls[b], 127 - ((f + joyPotPrevs[b] + joyPotPrevs2[b] + joyPotPrevs3[b]) / 4), channel);
          }          
         joyPotSends[b] = ((f + joyPotPrevs[b] + joyPotPrevs2[b] + joyPotPrevs3[b]) / 4);
         joyPotPrevs3[b] = joyPotPrevs2[b];
         joyPotPrevs2[b] = joyPotPrevs[b];
         joyPotPrevs[b] = f; 
        }


        
        if (f > joyPotCenters[b]){

          f = map(f, joyPotCenters[b], 127, 65, 127);
          if ( (b % 2) == 1 ){
          usbMIDI.sendControlChange(joyPotCtls[b], (f + joyPotPrevs[b] + joyPotPrevs2[b] + joyPotPrevs3[b]) / 4, channel);
          }
          if ( (b % 2) == 0 ){
          usbMIDI.sendControlChange(joyPotCtls[b], 127 - ((f + joyPotPrevs[b] + joyPotPrevs2[b] + joyPotPrevs3[b]) / 4), channel);
          }  
         joyPotSends[b] = ((f + joyPotPrevs[b] + joyPotPrevs2[b] + joyPotPrevs3[b]) / 4);          
         joyPotPrevs3[b] = joyPotPrevs2[b];
         joyPotPrevs2[b] = joyPotPrevs[b];
         joyPotPrevs[b] = f; 
        }
      }
    }    
  }
}

void OnNoteOn(byte channel, byte note, byte velocity)
{

  for (int b = 0; b < leds; b++) {
  if (note==ledNotes[b]){
    analogWrite(ledPins[b], velocity*2);
  }
  }
    
}

void OnNoteOff(byte channel, byte note, byte velocity)
{

  for (int b = 0; b < leds; b++) {
  if (note==ledNotes[b]){
    analogWrite(ledPins[b], 0);
  }
  }
    
}

void loop() {
  int type, note, velocity, channel, d1, d2;

  //---------for sending midi---------
  //process buttons
  buttonInput();

  //process faders
  faderInput();

  //--------for receiving midi---------

  
    // MIDI Controllers should discard incoming MIDI messages.
  // http://forum.pjrc.com/threads/24179-Teensy-3-Ableton-Analog-CC-causes-midi-crash
  usbMIDI.read();
}
