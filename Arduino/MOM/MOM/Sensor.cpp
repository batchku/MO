#include "Sensor.h"

Sensor::Sensor()
{

}

Sensor::Sensor(int _input, int _midiAddress)
{
  input = _input;
  midiAddress = _midiAddress;
}


void Sensor::set(int _input, int _midiAddress) {
  input = _input;
  midiAddress = _midiAddress;
}

void Sensor::setInput(int _input) {
  input = _input;
}

void Sensor::setMidi(int _midiAddress) {
  midiAddress = _midiAddress;
}

void Sensor::update()
{
  int f = analogRead(input) / 8;

  if (((f + potPrevs[0] + potPrevs[1] + potPrevs[2]) / 4) != potSend)  {
    usbMIDI.sendControlChange(midiAddress, 127 - ((f + potPrevs[0] + potPrevs[1] + potPrevs[2]) / 4), 1);
    potSend = (f + potPrevs[0] + potPrevs[1] + potPrevs[2]) / 4;
    potPrevs[2] = potPrevs[1];
    potPrevs[1] = potPrevs[0];
    potPrevs[0] = f;
  }
}

void Sensor::read()
{
  int f = analogRead(input) / 8;
  
  if (((f + potPrevs[0] + potPrevs[1] + potPrevs[2]) / 4) != potSend)  {
    change = true;
    potSend = (f + potPrevs[0] + potPrevs[1] + potPrevs[2]) / 4;
    potPrevs[2] = potPrevs[1];
    potPrevs[1] = potPrevs[0];
    potPrevs[0] = f;
    change = false;
  }
}


int Sensor::getVal(){

  int f = analogRead(input) / 8;
  
  return (f + potPrevs[0] + potPrevs[1] + potPrevs[2]) / 4;
  }
 
int Sensor::getLastVal(){
  return potSend;
  }
bool Sensor::getChange(){
  return change;
  }
