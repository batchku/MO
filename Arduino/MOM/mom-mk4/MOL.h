//...INPUTS...
// M = MOTOR
// S = SERVO
//JOYSTICK BUTTONS
int M_JOY_BTN   = 0;
int S_JOYB_BTN  = 1;
//LED DRIVER PINS 1-7
int LED1        = 2;
int LED2        = 3;
int LED3        = 4;
int LED4        = 5;
int LED5        = 6;
int LED6        = 7;
int LED7        = 8;
//SERVO 1, 2 PWM PIN
int S1          = 9;
int S2          = 10;
//BUTTONS 1-2
int BTN1        = 11;
int BTN2        = 12;
// X = 13;
//TRIMPOT 1-3
int POT1        = 14;
int POT2        = 15;
int POT3        = 16;
//MOTOR: JOYSTICK VERTICAL, HORIZONTAL
int M_JOY_V     = 17;
int M_JOY_H     = 18;
//SERVO: JOYSTICK VERTICAL, HORIZONTAL
int S_JOY_V     = 19;
int S_JOY_H     = 20;
//MOTOR: A, B PWM PIN
int M_PWM_A     = 21;
int M_PWM_B     = 22;
//LED DRIVER PIN 8
int LED8        = 23;
//BUTTONS 3-7
int BTN3        = 24;
int BTN4        = 25;
int BTN5        = 26;
int BTN6        = 27;
int BTN7        = 28;
//LED DRIVER PIN 9-12
int LED9        = 29;
int LED10       = 30;
int LED11       = 31;
int LED12       = 32;
//TRIMPOT 4-7
int POT4        = 33;
int POT5        = 34;
int POT6        = 35;
int POT7        = 36;
//LED DRIVER PIN 10-11
//X Mistake
//int L10 = 37;
//int L11 = 38;
// TRIMPOT 8
int POT8        = 39;
//MOTOR A, B DIRECTION
int M_IN_A      = 40;
int M_IN_B      = 41;
//BUTTONS 8-12
int BTN8        = 42;
int BTN9        = 43;
int BTN10       = 44;
int BTN11       = 45;
int BTN12       = 46;
//I2C SCL,SDA
int I2C_SCL     = 47;
int I2C_SDA     = 48;
//LED DRIVER PIN 12
//X Mistake?
//int L12 = 49;
//USER FEEDBACK 1-4
int U1          = 51;
int U2          = 52;
int U3          = 53;
int U4          = 54;
//TRIMPOT 9-12
int POT9        = A21;
int POT10       = A22;
int POT11       = A10;
int POT12       = A11;

//...ARRAYS...
int POTS[] = {POT1,
              POT2,
              POT3,
              POT4,
              POT5,
              POT6,
              POT7,
              POT8,
              POT9,
              POT10,
              POT11,
              POT12};
int BTNS[] = {BTN1,
              BTN2,
              BTN3,
              BTN4,
              BTN5,
              BTN6,
              BTN7,
              BTN8,
              BTN9,
              BTN10,
              BTN11,
              BTN12};
int LEDS[] = {LED1,
              LED2,
              LED3,
              LED4,
              LED5,
              LED6,
              LED7,
              LED8,
              LED9,
              LED10,
              LED11,
              LED12};
int USR[] = {U1,
             U2,
             U3,
             U4};
