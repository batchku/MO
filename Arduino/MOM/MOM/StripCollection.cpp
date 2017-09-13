#include "Arduino.h"
#include "MO.h"
#include "Arrays.h"
#include "MIDI.h"
#include "StripCollection.h"
#include "Strip.h"

StripCollection::StripCollection()
{
  stripList = LinkedList<Strip*>();
}

void StripCollection::update()
{
  Strip *strip;

  for (int i = 0; i < stripList.size(); i++) {
    strip = stripList.get(i);
    strip->update();
  }
}

void StripCollection::add(Strip *strip){
    stripList.add(strip);
  }

void StripCollection::light(int note, int velocity) {
  Strip *strip;

  for (int i = 0; i < stripList.size(); i++) {
    strip = stripList.get(i);
    strip->light(note, velocity);
  }
}

