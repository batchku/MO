# livedraw-mini
openFrameworks based live video looper made for the Raspberry Pi 3, with a Pi Camera v2

# code
`ConsoleListener.h` - used for debugging in console on the Pi
`Consts.h` - definition of application parameters
`main.cpp` - application; set up openGL context, link to main class
`ofApp.cpp` - main functions for init, update and draw
`ofApp.h` - goes with ofApp.cpp
`Utils.cpp` - helper functions for math and such
`Utils.h` - goes with Utils.cpp
`vidLayer.cpp` - main class to handle a video layer; includes memory allocaiton, rendering, updating, and real-time control
`vidLayer.h` - goes with vidLayer.cpp
