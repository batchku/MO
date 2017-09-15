#include "Sensor.h"

Sensor::Sensor()
{

}

Sensor::Sensor(const int _input, const int _midiAddress)
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

void Sensor::update() {
  read();
  send();
}

void Sensor::read()
{
  if (timer >= 20) {
    current = analogRead(input) >> 3;
    
    current = (current + potPrev[0] + potPrev[1] + potPrev[2]) / 4;
    if (current != potAvg) {
      dataChange = true;
      potAvg = (current + potPrev[0] + potPrev[1] + potPrev[2]) / 4;
      potPrev[2] = potPrev[1];
      potPrev[1] = potPrev[0];
      potPrev[0] = analogRead(input) >> 3;
    }
    timer = 0;
  }
}

void Sensor::send(){
  if(dataChange){
    usbMIDI.sendControlChange(midiAddress, 127 - current, 1);
    dataChange = false;
    }
  }


int Sensor::getCurrent(){
  return current;
  }

void Sensor::setName(String _name) {
  name = _name;
}

void Sensor::debug() {
  Serial.print(name);
  Serial.print(" , ");
  Serial.println(midiAddress);
}

