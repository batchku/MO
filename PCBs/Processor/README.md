# Pin Mapping Generator
This python script generates the following header files for the arduino program (MOD.ino)[https://github.com/batchku/MO/blob/master/Arduino/MOD/MOD.ino]:
1. `MO.H`: defines a variable for each used pin
2. `ARRAYS.H`: defines arrays of all the related variables (pots together, buttons together) for easier coding
3. `MIDI.h`: defines midi not and control numbers for each pin

It also generates a text file for the Pd abstractions in the `MO Li

# Coming...
- XML file for openFrameworks for MIDI mapping
- 

To run:
1.  export a pinlist from eagle schematic, here called inputfile
2.  execute:
        ```python3 createFiles.py intputfile```
