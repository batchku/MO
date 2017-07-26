// the MIDI channel number to send messages
const int channel = 1; 

//Pot MIDI CC #s
//Three layers, 1+2 = loop layers, 3 = live layer 
//Broken const into: OPACITY, THRESH, SOFT, SCALE
const int POTCCs[]        = {0,1,2,3,
                             4,5,6,7,
                             8,9,10,11};

//Joystick MIDI CC #s                   
const int JOYCCs[]        = {30,31,
                             32,33};
 
//Button MIDI CC #s    
const int BTNCCs[]        = {64,65,66,67,
                             68,69,70,71,
                             72,73,74,75};
 
//Joystick Button MIDI CC #s                      
const int JBTNCCs[]       = {110,
                             111};
                   
//LED MIDI CC #s                 
const int LEDCCs[]        = {64,
                             65,
                             66,
                             67,
                             68,
                             69,
                             70,
                             71,
                             72,
                             73,
                             74,
                             75,
                             76,
                             77,
                             78,
                             79};
//User LEDs MIDI CC #s (duplicate, also inside LEDCCs[])                     
const int USRCCs[]        = {76,
                             77,
                             78,
                             79};
                             
const int SERVOCCs[]      = {12,
                             13};
                             
const int MOTORPWMCCs[]   = {14,
                             15};
                             
const int MOTORINCCs[]    = {16,
                             17};
                             
