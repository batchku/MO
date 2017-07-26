#2017 Ali Momeni
#Made for Python 3.6
#   usage:
#   1.  export a pinlist from eagle board, here called inputfile
#   2.  execute:
#       python3 pinlister.py intputfile

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
    startText = 'TEENSY   0'                        #starting text
    endText = '         3V3(4)'                     #ending text
    skiptText = 'unconnected|3V3|AREF|AGND|GND'     #skip lines with these strings

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
        thisline = re.split(" +", theTextLines[i])  #split text by spaces
        #print(thisline)

        pinNum = re.sub('DAC0','A21',thisline[1])   #special substitutions
        pinNum = re.sub('DAC1','A22',pinNum)        #special substitutions
        pinSig = re.sub('\n','',thisline[2])        #remove newline chars
        test = re.search(skiptText,theTextLines[i])

        #print(test)
        if test is None :
            pinDict[pinSig] = changeToInt(pinNum)
            #pinDict[pinSig] = pinNum
    
    #define lists to be populated for arrays
    pots, joyPots, btns, joyBtns, leds, usr = ([] for i in range(6))
    #define dicts that associate arrays with their name in header file
    arrayNames = {'POTS'    : pots,
                  'JPOTS'   : joyPots, 
                  'BTNS'    : btns,
                  'JBTNS'   : joyBtns,
                  'LEDS'    : leds, 
                  'USR'     : usr}
    def atoi(text):
        return int(text) if text.isdigit() else text

    def natural_keys(text):
        return [ atoi(c) for c in re.split('(\d+)', text) ]
    
    def createList(searchTerm, listToAppendTo):
        listToAppendTo.clear()
        for key in pinDict.keys():
            if key.startswith(searchTerm):
                listToAppendTo.append(key)
                listToAppendTo.sort(key=natural_keys)
    
    #create lists using search terms and arrays provided  
    createList('BTN', btns)
    createList('AN', pots)
    createList('JOY_AN', joyPots)
    createList('JOY_BTN', joyBtns)
    createList('L',leds)
    createList('U',usr)
    #combine leds and usr
    leds += usr 
    
    output = open("MOL.h", "w")

    output.write("//------------------------------------------------------\n")
    output.write("//Thiis file is automatically generated with pinlister.py\n")
    output.write("//------------------------------------------------------\n")
    output.write("\n")
    for key, value in pinDict.items():
        #outputLine = "int " + key + "\t" + "= " + str(value) + ";\n"
        outputLine = '{}{:10}{}{}{}'.format('const int ',key,'= ',value,';\n')
        output.write(outputLine)

    #Create arrays for variables for looping 
    output.write('\n')
    output.write('//------------\n')
    output.write('//---ARRAYS---\n')
    output.write('//------------\n')
    output.write('\n')
    for key, value in arrayNames.items():
    
        outputLine = '{}{:10}{}'.format('const int ',key +'[]','= {')
        outputLine += '{}{}{}'.format(value[0],',','\n')
        for val in value[1 :-1]:
            outputLine += '{:>23}{}{}{}'.format(' ',val,',','\n')
        outputLine += '{:>23}{}{}{}'.format(' ',value[-1],'}',';\n')
        outputLine += '\n'
        output.write(outputLine)

    output.close()

def changeToInt(s):
    try:
        int(s)
        return int(s)
    except ValueError:
        return s


def main():
    theFile = sys.argv[1]
    #for eachFile in theFiles:
    #print(theFile)
    process(theFile)
    
if __name__ == '__main__':
    main()
