# Pin Mapping Generator
This python script generates the following header files for the arduino program [MOD.ino](https://github.com/batchku/MO/blob/master/Arduino/MOD/MOD.ino):
- `MO.H`: defines a variable for each used pin ([example](https://github.com/batchku/MO/blob/master/PCBs/Processor/MO.h))
- `ARRAYS.H`: defines arrays of all the related variables (pots together, buttons together) for easier coding ([example](https://github.com/batchku/MO/blob/master/PCBs/Processor/ARRAYS.h))
- `MIDI.h`: defines midi not and control numbers for each pin ([example](https://github.com/batchku/MO/blob/master/PCBs/Processor/MIDI.h))

It also generates a text file for the [MO Pd abstractions](https://github.com/batchku/MO/tree/master/Pd/MO) i
- `inputs.txt`: defines MIDI values for UI elements in abstractions (see [inputs.txt](https://github.com/batchku/MO/blob/master/PCBs/Processor/inputs.txt))

# Coming...
- XML file for openFrameworks for MIDI mapping
- 

To run:
1.  export a pinlist from eagle schematic, here called inputfile
2.  execute:
        ```python3 createFiles.py intputfile```
