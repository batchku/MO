#2017 Ali Momeni, Daniel McNamara
#Made for Python 3.6
#   usage:
#   1.  export a pinlist from eagle SCHEMATIC, here called inputfile
#   2.  execute:
#       python3 midilister.py intputfile

import string
import sys
import re
import os

def process(filename):
    #define dictionary for storing pins and signals
    pinDict = dict()

    #open the input file
    theText = open(filename, "r").read()

    #make array of all lines of input filename
    theTextLines = open(filename, "r").readlines()

    #define important strings
    startText = 'MIDI_IN'                           #starting text
    endText = '         3V3(4)'                     #ending text
    skiptText = ' N\$'                              #skip lines with these strings

    #find starting and ending lines
    with open(filename) as myFile:
            for num, line in enumerate(myFile,1):
                    if startText in line:
                        startLine = num - 1 #first line is off by 1 for some reason
                    if endText in line:
                        endLine = num
            lineRange = range(startLine,endLine)

    #populate dictionary
    for i in lineRange:
        #print('------')
        #print(theTextLines[i])
        
        thisline = re.split(" +", theTextLines[i])
        thisline = [e for e in thisline if e not in ('*', 'none', 'io','','MIDI_IN','MIDI_OUT', '\n')]
        if thisline: #skips lists that are empty
            #print(thisline)
            pinName = thisline[0] #index 0 is the signal name
            midiNum = re.sub('\n','',thisline[1]) #remove line breaks from midi numbers
            test = re.search(skiptText,theTextLines[i]) #skip lines that aren't connected
            #print(midiNum)
            #print(test)
            if test is None :
                pinDict[midiNum] = changeToInt(pinName)
    
    def createList(searchTerm, listToAppendTo):
        listToAppendTo.clear()
        for key in pinDict.keys():
            if key.startswith(searchTerm):
                listToAppendTo.append(key)
                listToAppendTo.sort(key=natural_keys)
        for index, item in enumerate(listToAppendTo):   
            #print(pinDict.get(item))
            a = [item,pinDict.get(item)]
            listToAppendTo[index] = a             
    
    
    def createList2(searchTerm):
        listToAppendTo = []
        listToAppendTo.clear()
        for key in pinDict.keys():
            if key.startswith(searchTerm):
                listToAppendTo.append(key)
                listToAppendTo.sort(key=natural_keys)
        for index, item in enumerate(listToAppendTo):    
            #print(pinDict.get(item))
            a = [item,pinDict.get(item)]
            listToAppendTo[index] = a
        return listToAppendTo
        
    # create userIns array by copying an aspect of pots but changing Midi values
    def subArray(copyFrom, copyTo, shiftMidi):
        for index, item in enumerate(copyFrom):    
            copyTo.append(item[:]) #copy by value, not reference 
            copyTo[index][1] = shiftMidi + index #add an increment to each value
                
    #define lists to be populated for arrays
    pots, joyPots, userPots, userIns, btns, joyBtns, userBtns, leds, userLeds, stripLeds, servos, motorPWMs, motorINs = ([] for i in range(13))

    arrayNames = {'POTS' : pots,
                  'BUTTONS' : btns,
                  'JOYPOTS' : joyPots,
                  'JOYBUTTONS' : joyBtns,
                  'LEDS' : leds,
                  'SERVOS' : servos,
                  'MOTORPWMS' : motorPWMs,
                  'MOTORINS' : motorINs,
                  'USERPOTS' : userPots,
                  'USERBUTTONS' : userBtns,
                  'USERLEDS' : userLeds,
                  'USERINS' : userIns,
                  'STRIPLEDS': stripLeds}

    #create lists using search terms and arrays provided 
    #if there are errors this is most likely where it is happening...
    #look at the search terms and make sure they still are present in the Eagle file
     
    #Create a list by searching for any dict items conatining 'AN' and place those items in pots[]
    createList('AN', pots) 
    joyPots += (createList2('JOYV'))
    joyPots += (createList2('JOYH'))
    subArray(pots[8:], userPots, 0)     #sets the userPots midi at 0
    subArray(pots[4:8], userIns, 20)    #sets the userIns midi at 20


    createList('JOYB', joyBtns)
    createList('B', btns)
    subArray(btns[12:], userBtns, 0)

    createList('PWM', leds)
    createList('LED', stripLeds)
    subArray(leds[12:], userLeds, 0)

    createList('S', servos)
    createList('M_IN', motorINs)
    createList('M_PWM', motorPWMs)
    
    #Create a new header file for MIDI
    output = open("MIDI.h", "w")
    
    output.write("//------------------------------------------------------\n")
    output.write("//Thiis file is automatically generated with midilister.py\n")
    output.write("//------------------------------------------------------\n")
    
    #Create MIDI arrays
    output.write('\n')
    output.write('//------------')
    output.write('---MIDI VALUES---')
    output.write('------------\n')
    output.write('\n')
    for key, value in arrayNames.items():
    
        outputLine = '{}{:20}{:}'.format('const int ',key + '_MIDI[] ','= {')
        outputLine += '{}{}{}'.format(value[0][1],',','\n')
        for val in value[1 :-1]:
            outputLine += '{:>33}{}{}{}'.format(' ',val[1],',','\n')
        outputLine += '{:>33}{}{}{}'.format(' ',value[-1][1],'}',';\n')
        outputLine += '\n'
        output.write(outputLine)
  
    output.close()
    
    output = open("ARRAYS.h", "w")
    
    output.write("//------------------------------------------------------\n")
    output.write("//Thiis file is automatically generated with midilister.py\n")
    output.write("//------------------------------------------------------\n")
    

    #Create a new header file for ARRAYS used for variables for looping 
    output.write('\n')
    output.write('//------------')
    output.write('---ARRAYS---')
    output.write('------------\n')
    output.write('\n')
    for key, value in arrayNames.items():
    
        outputLine = '{}{:18}{:}'.format('const int ',key + '[] ','= {')
        outputLine += '{}{}{}'.format(value[0][0],',','\n')
        for val in value[1 :-1]:
            outputLine += '{:>31}{}{}{}'.format(' ',val[0],',','\n')
        outputLine += '{:>31}{}{}{}'.format(' ',value[-1][0],'}',';\n')
        outputLine += '\n'
        output.write(outputLine)
  
    output.close()
    
    #Create inputs.txt file for Pd abstractions
    output = open("inputs.txt", "w")

    #Create arrays for variables for looping 
    for key, value in arrayNames.items():
    
        outputLine = key
        for val in value:
            outputLine += '{}{}'.format(' ',val[1])
        outputLine += ';'
        outputLine += '\n'
        output.write(outputLine)
  
    output.close()

def changeToInt(s):
    try:
        int(s)
        return int(s)
    except ValueError:
        return s
        
def atoi(text):
    return int(text) if text.isdigit() else text

def natural_keys(text):
    return [ atoi(c) for c in re.split('(\d+)', text) ]

def main():
    theFile = sys.argv[1]
    #for eachFile in theFiles:
    #print(theFile)
    process(theFile)
    #pinlister_.process(theFile)
    
if __name__ == '__main__':
    main()
