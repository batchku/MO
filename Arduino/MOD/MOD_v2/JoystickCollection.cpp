#include "Arduino.h"
#include "MO.h"
#include "Arrays.h"
#include "MIDI.h"
#include "JoystickCollection.h"
#include "Joystick.h"

JoystickCollection::JoystickCollection()
{
  joystickList = LinkedList<Joystick*>();
}

void JoystickCollection::update()
{
  Joystick * joystick;

  for (int i = 0; i < joystickList.size(); i++) {
    joystick = joystickList.get(i);
    joystick->update();
  }
}

void JoystickCollection::calibrate()
{
  Joystick * joystick;

  for (int i = 0; i < joystickList.size(); i++) {
    joystick = joystickList.get(i);
    joystick->calibrate();
  }
}


void JoystickCollection::add(Joystick *joystick) {
  joystickList.add(joystick);
}
