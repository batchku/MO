# Pin Mapping Generator
This python script generates the following header files for the arduino program [MOD.ino](https://github.com/batchku/MO/blob/master/Arduino/MOD/MOD.ino):
- [MO.H](https://github.com/batchku/MO/blob/master/PCBs/Processor/MO.h): defines a variable for each used pin
- [ARRAYS.H](https://github.com/batchku/MO/blob/master/PCBs/Processor/ARRAYS.h): defines arrays of all the related variables for easier coding
- [MIDI.h](https://github.com/batchku/MO/blob/master/PCBs/Processor/MIDI.h): defines midi note and control numbers for each pin

It also generates a text file for the [MO Pd abstractions](https://github.com/batchku/MO/tree/master/Pd/MO)
- `inputs.txt`: defines MIDI values for UI elements in abstractions ([example](https://github.com/batchku/MO/blob/master/PCBs/Processor/inputs.txt))

# Coming...
- XML file for openFrameworks for MIDI mapping
- 

To run:
1.  export a pinlist from eagle schematic, here called inputfile
2.  execute:
        ```python3 createFiles.py intputfile```
