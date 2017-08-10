# Pin Mapping Generator
This python script generates:
1. `MO.H`, `ARRAYS.H`, and `MIDI.h` header files for Arduino
2. XML file for openFrameworks for MIDI mapping
3. MIDI I/O list for Pd abstractions

To run:
1.  export a pinlist from eagle schematic, here called inputfile
2.  execute:
        ```python3 createFiles.py intputfile```
