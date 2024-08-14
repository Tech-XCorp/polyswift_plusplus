#!/usr/bin/env python
"""
@file    psPlotHistory.py

@brief   Plot PolySwift++ history datasets as X,Y pair 

@version $Id$

Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
All rights reserved.
"""

import sys
# sys.path = ['.','/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7/lib-dynload','/contrib/lib/python2.7/site-packages/numexpr-1.4.2-py2.7-macosx-10.7-intel.egg','/contrib/lib/python2.7/site-packages','/System/Library/Frameworks/Python.framework/Versions/2.7/Extras/lib/python','/System/Library/Frameworks/Python.framework/Versions/2.7/lib/python2.7']

import string
import tables
import glob
import numpy
from numpy import *
from optparse import OptionParser
import pylab
from pylab import *

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

parser.add_option("--dsXname",
                  dest="dsXname",
                  help="Dataset name for x-coord")

parser.add_option("--dsYname",
                  dest="dsYname",
                  help="Dataset name for y-coord")

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
# main()
#

# ---------------------------------------
# Acessing options
(options,args) = parser.parse_args()
prtDebug = options.prtDebug
dsXname  = str(options.dsXname)
dsYname  = str(options.dsYname)
# ---------------------------------------

# Check paths
if prtDebug:
   print "Set internally sys.path = ", set.path

# Check for one history file
histFiles = glob.glob("*History*.h5")
if len(histFiles) > 1:
   print "Only valid for one history file, check run directory"
   exit(1)

histFile = histFiles[0]                   # Only one history file at a time
print "\n Analyzing file ", histFile      # status
histName = string.split(histFile,".")[0]  # strip .h5 from file name
histDct = getHistFileDictionary(histFile) # parse all datasest in file to dictionary

# Check if datasets exist / and length consistency
try:
   lenX = len(histDct[dsXname])
   lenY = len(histDct[dsYname])
   if lenX != lenY:
      print "x-dataset and y-dataset must be same length"
      exit(1)
except:
   print "Dataset(s) ", dsXname, " ", dsYname, "dont exist."
   print "Check history file or set options for dsXname/dsYname"
   exit(1)

npy_X = histDct[dsXname]
npy_Y = histDct[dsYname]
plot(npy_X, npy_Y, linewidth=2.0)

axvline(x=10.498, linewidth=3.0, color='g')
text(10.25, -0.01, r'$\chi N_{ODT}$')
xlabel('chiN')
ylabel('fE')
title('Estimate of ODT from numerical SCFT simulation')
grid(True)
show()

print ""
