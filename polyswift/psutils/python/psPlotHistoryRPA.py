#!/usr/bin/env python
"""
@file    psPlotHistoryRPA.py

@brief   Plot PolySwift++ history datasets as X,Y pair for RPA example 

@version $Id$

Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
All rights reserved.
"""

import sys
import string
import tables
import glob
import numpy
import os
from numpy import *
from optparse import OptionParser

#import matplotlib
#from matplotlib import pylab

#################### MAIN: ###########################

# Command line option parse
usage = """usage: psPlotHistoryRPA.py [options]

   Looks for a runName_History.h5 file and looks for multiple datasets
   in the root group, one for each run. These dataset sets are plotting
   for free-energy vs. Flory paramter in order to pick out the ODT.
"""

parser = OptionParser(usage=usage)

parser.add_option("-d", action="store_true",
                  dest="prtDebug",
                  default=False,
                  help="Print debug info from script")

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

#def main():

# ---------------------------------------
# Acessing options
(options,args) = parser.parse_args()
prtDebug = options.prtDebug
# ---------------------------------------

def main():
  # Check paths
  if prtDebug:
     print "Set internally sys.path = ", set.path

  # Check for one history file
  histFiles = glob.glob("*History*.h5")
  histFiles
  if len(histFiles) > 1:
     print "Only valid for one history file, check run directory"
     exit(1)

  histFile = histFiles[0]                   # Only one history file at a time
  print "\n Analyzing file ", histFile      # status
  histName = string.split(histFile,".")[0]  # strip .h5 from file name
  histDct = getHistFileDictionary(histFile) # parse all datasest in file to dictionary

  print "histDct.keys = ", histDct.keys()

  keyList = histDct.keys()
  numRuns = len(keyList)/2
  print "numRuns = ", numRuns

  baseName = os.path.basename(glob.glob(os.getcwd()+'/*.h5')[0]).split('_', 1)[0]
  new_h5file = baseName + "_0.h5"
  h5file = tables.openFile(new_h5file, mode = "w")
  group_name = 'params'
  g1 = h5file.createGroup(h5file.root, group_name)

  # Create new histories for each run
  for n in range(1,numRuns+1):

    dsXname = "chiN_run" + str(n)
    dsYname = "fE_run" + str(n)

    print "datasets ", dsXname, " ", dsYname

    # Check if datasets exist / and length consistency
    try:
      lenX = len(histDct[dsXname])
      lenY = len(histDct[dsYname])
      if lenX != lenY:
        print "x-dataset and y-dataset must be same length"
        exit(1)
    except:
      print "Dataset(s) ", dsXname, " ", dsYname, "do not exist, skipping."
      print "Check history file or set options for dsXname/dsYname"
      exit(1)

    npy_X = histDct[dsXname]
    npy_Y = histDct[dsYname]

    # Create file with chi values
    dataSet = h5file.createArray(g1, dsXname, npy_X)
    dataSet.attrs.vsKind = 'structured'
    dataSet.attrs.vsPoints = 'vertices'
    dataSet.attrs.vsType = 'mesh'

    # Create file with fE values
    dataSet = h5file.createArray(g1, dsYname, npy_Y)
    dataSet.attrs.vsMesh = dsXname
    dataSet.attrs.vsType = 'variable'
    dataSet.attrs.vsCentering = 'zonal'

  # Close H5 file
  h5file.close()

if __name__=="__main__":
  main()
