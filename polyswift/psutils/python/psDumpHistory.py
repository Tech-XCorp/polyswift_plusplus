#!/usr/bin/env python
"""
@file    psDumpHistory.py

@brief   Dumping PolySwift++ history info 

@version $Id$

Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
All rights reserved.
"""

import sys
#sys.path = ['.','/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/lib-dynload','/contrib/lib/python2.7/site-packages/numexpr-1.4.2-py2.7-macosx-10.7-intel.egg','/contrib/lib/python2.7/site-packages','/System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python','/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7']

import string
import tables
import glob
import numpy
from numpy import *
from optparse import OptionParser

#################### MAIN: ###########################

# Command line option parse
usage = """usage: ps_DumpHistory.py [options]

   Looks for all x_History.h5 files and takes each dataset in the
   root group and dumps scalar history data to a .dat ascii file
"""

parser = OptionParser(usage=usage)

parser.add_option("-d", action="store_true",
                  dest="prtDebug",
                  default=False,
                  help="Print debug info from script")
"""
parser.add_option("--indir",
                  dest="inDir",
                  default=".",
                  help="Input directory path")

parser.add_option("--outdir",
                  dest="outDir",
                  default=".",
                  help="Output directory path")
"""

#
# Parse History file for all datasets and store in dictionary
#
def getHistFileDictionary(histFile):

   dsetDict = {}
   if prtDebug: print "Trying to open ", histFile      # debug info
   h5file = tables.openFile(histFile,"r")              # open hist file with tables
   pyNodes = h5file.listNodes("/", classname='Array')  # nodes in history file

   for pyTableObj in pyNodes:                        # For each dataset
     sizeTuple = pyTableObj.shape                    # Shape of history
     dsetSize = sizeTuple[0]                         # Length of history
     dsetName = str(pyTableObj.name)                 # string of dataset name
     if prtDebug: print " found history ", dsetName  #
     if len(sizeTuple) != 1:                         # Tensor data not supported yet
       print "..not converting non-scalar history"   #  so skip and go to next dataset
       break                                         #
     dsetDict[dsetName] = pyTableObj[0:dsetSize]     # put hist dat in dictionary

   h5file.close()
   return dsetDict

#
# Dump history data to separate text file
#
def dumpHistoryData(histName, dsetDict):

   for dsetName in dsetDict.keys():                     # For all dataset in dictionary

     dsetData = dsetDict[dsetName]                      # Get data from dict
     histDatFile = histName+'_'+dsetName+'.dat'         # Construct history dat file name
     if prtDebug:                                       # status
        print " Writing ascii file ", histDatFile       # status
     f0=open(histDatFile,'w')                           # open .dat file

     for idat in range(len(dsetData)):                  #
       f0.write('%i  %le  \n' %(idat, dsetData[idat]))  # print out (step, histdat)

     f0.close()                                         # close .dat file

#
# main()
#

# ---------------------------------------
# Acessing options
(options,args) = parser.parse_args()
prtDebug       = options.prtDebug
# inDir       = str(options.inDir)
# outDir      = str(options.outDir)
# ---------------------------------------

histFiles = glob.glob("*History*.h5")
for histFile in histFiles:

   print "\n Analyzing file ", histFile      # status
   histName = string.split(histFile,".")[0]  # strip .h5 from file name
   histDct = getHistFileDictionary(histFile) # parse all datasest in file to dictionary
   dumpHistoryData(histName, histDct)        # dump ASCII for each dataset in dictionary
   print ""
