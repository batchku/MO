#2017 Ali Momeni, Daniel McNamara
#Made for Python 3.6
#   usage:
#   1.  export a pinlist from eagle SCHEMATIC, here called inputfile
#   2.  execute:
#       python3 pinlister.py intputfile

import sys
import midilister
import pinlister

def main():
    theFile = sys.argv[1]
    #for eachFile in theFiles:
    #print(theFile)
    midilister.process(theFile)
    pinlister.process(theFile)
    
if __name__ == '__main__':
    main()