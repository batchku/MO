#include <Bounce2.h>

#define SHUDOWN_SIG 1     //Pin 6
#define PI_POWER 2        //Pin 7
#define SWITCH 0          //Pin 5
#define POWER_LED 4       //Pin 9

// Instantiate a Bounce object
Bounce debouncer = Bounce();

boolean ShutDown = true;

void setup() {
  // Setup the button with an internal pull-up :
  pinMode(SWITCH, INPUT_PULLUP);

  // After setting up the button, setup the Bounce instance :
  debouncer.attach(SWITCH);
  debouncer.interval(5); // interval in ms

  pinMode(PI_POWER, OUTPUT);
  pinMode(SHUDOWN_SIG, OUTPUT);
  pinMode(POWER_LED, OUTPUT);
}

void loop() {
  // Update the Bounce instance :
  debouncer.update();
  // Get the updated value :
  int value = debouncer.read();
  // Turn on or off the Relay as determined by the state of the SWITCH :
  if ( value == HIGH ) {
    if (ShutDown == false) ShutDown = true; // reset ShutDown bool
    digitalWrite(PI_POWER, HIGH );  // Keep the relay open
    digitalWrite(POWER_LED, HIGH);
    digitalWrite(SHUDOWN_SIG, HIGH);   //  Don't send the trigger signal to Pi
  }
  else {
    if (ShutDown) {
      digitalWrite(SHUDOWN_SIG, LOW);  //Trigger Pi's Python script to shutdown
      analogWrite(POWER_LED, 100);
      delay(8000);                    // Wait x amount of time so Pi properly shuts down
      analogWrite(POWER_LED, LOW);
      digitalWrite(PI_POWER, LOW );   //Close relay
      ShutDown = false;
    }
  }
}
