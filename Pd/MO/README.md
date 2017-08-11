# MO Abstraction Set
The mo. abstraction set is designed to interface with the MO hardware with no additional setup.
# Usage
### mo.arrayReader
All mo. objects require the presence of ```mo.arrayReader``` to be present in a patch. They refer to it for MIDI information.
### input.txt
If you have changed MIDI information in EAGLE, be sure to use the [generator script](https://github.com/batchku/MO/blob/master/PCBs/Processor/createFiles.py) to create a new ```input.txt``` file and place it in this folder.
### Abstraction i/o
All abstractions recieve and output messages from one input and one output. Each control's name is represented under it with ```[]```.
