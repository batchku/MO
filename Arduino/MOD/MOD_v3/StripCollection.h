#ifndef StripCollection_h
#define StripCollection_h
#define NUM_CONTROLS  2

#include "Arduino.h"
#include "Strip.h"
#include <LinkedList.h>

class StripCollection
{
  public:
    StripCollection();
    void update();
    void add(Strip *strip);
    void light(int note, int velocity);
  private:
    LinkedList<Strip*> stripList;
};

#endif
