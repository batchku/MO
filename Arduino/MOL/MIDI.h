// the MIDI channel number to send messages
const int channel = 1; 

//Pot MIDI CC #s
//Three layers, 1+2 = loop layers, 3 = live layer 
//Broken const into: OPACITY, THRESH, SOFT, SCALE
const int POTCCs[]        = {11,10,13,12,
                             15,14,16,17,
                             18,19,20,21};
                             
const int USERPOTCCs[]    = {0,1,2,3};

const int USERINCCs[]     = {4,5,6,7};

//Joystick MIDI CC #s                   
const int JOYCCs[]        = {6,7,
                             8,9};
 
//Button MIDI I/O #s    
const int BTNCCs[]        = {11,10,13,12,
                             15,14,16,17,
                             18,19,20,21};

//Button MIDI I/O #s    
const int USERBTNCCs[]    = {0,
                             1,
                             2,
                             3};
                             
//Joystick Button MIDI I/O #s                      
const int JBTNCCs[]       = {6,
                             8};
                   
//LED MIDI CC #s                 
const int LEDCCs[]        = {11,
                             10,
                             13,
                             12,
                             15,
                             14,
                             16,
                             17,
                             18,
                             19,
                             20,
                             21};
//LED MIDI CC #s                 
const int USERLEDCCs[]    = {0,
                             1,
                             2,
                             3,
                             20,
                             21,
                             22,
                             23,
                             24,
                             25};
                             
//User LEDs MIDI CC #s (duplicate, also inside LEDCCs[])                     
const int USRCCs[]        = {76,
                             77,
                             78,
                             79};
                             
const int SERVOCCs[]      = {10,
                             11};
                             
const int MOTORPWMCCs[]   = {14,
                             15};
                             
const int MOTORINCCs[]    = {16,
                             17};
                             
