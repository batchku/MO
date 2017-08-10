#2017 Ali Momeni, Daniel McNamara
#Made for Python 3.6
#   usage:
#   1.  export a pinlist from eagle SCHEMATIC, here called inputfile
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
    skiptText = 'unconnected|3V3|AREF|AGND|GND|\*\*\*'     #skip lines with these strings

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
            thisline = re.split(" +", theTextLines[i])
            thisline = [thisline[1], thisline[-1]]
            
            pinName = thisline[1]                        #index 0 is the signal name
            pinName = re.sub('\n','',pinName)            #remove line breaks from pinnames
            
            pinNum =re.sub('DAC0','A21', thisline[0])   #special substitutions
            pinNum =re.sub('DAC1','A22', pinNum)        #special substitutions
    
            test = re.search(skiptText,thisline[1]) #skip lines that aren't connected
            if test is None :
                pinDict[pinName] = changeToInt(pinNum)

    output = open("MO.h", "w")

    output.write("//------------------------------------------------------\n")
    output.write("//Thiis file is automatically generated with pinlister.py\n")
    output.write("//------------------------------------------------------\n")
    output.write("\n")
    for key, value in pinDict.items():
        #outputLine = "int " + key + "\t" + "= " + str(value) + ";\n"
        outputLine = '{}{:10}{}{}{}'.format('const int ',key,'= ',value,';\n')
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
