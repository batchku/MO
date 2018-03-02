#include <Bounce2.h>

#define POWER_SIG 0     //Pin 5
#define PI_POWER 1      //Pin 6
#define SWITCH 2        //Pin 7

// Instantiate a Bounce object
Bounce debouncer = Bounce(); 

void setup() {

  // Setup the button with an internal pull-up :
  pinMode(SWITCH,INPUT_PULLUP);

  // After setting up the button, setup the Bounce instance :
  debouncer.attach(SWITCH);
  debouncer.interval(5); // interval in ms

  //Setup the LED :
  pinMode(PI_POWER,OUTPUT);
  pinMode(POWER_SIG,OUTPUT);

}

void loop() {
  // Update the Bounce instance :
  debouncer.update();

  // Get the updated value :
  int value = debouncer.read();

  // Turn on or off the Relay as determined by the state of the SWITCH :
  if ( value == HIGH ) {
    digitalWrite(PI_POWER, HIGH );  // Keep the relay open
    digitalWrite(POWER_SIG, LOW);   //  Don't send the trigger signal to Pi
  } 
  else {
    digitalWrite(POWER_SIG, HIGH);  //Trigger Pi's Python script to shutdown
    delay(8000);                    // Wait x amount of time so Pi properly shuts down
    digitalWrite(PI_POWER, LOW );   //Close relay 
  }

}


