#!/usr/bin/env python
"""
@file    ps-move.py

@brief   Script for creating archive for POLYSWIFT++
         and packing up data files. 

@version $Id$

Copyright &copy; 2009-2020, Tech-X Corporation, Boulder, CO.
All rights reserved.
"""

#!/usr/bin/env python
#  S. Sides  (Tech-X May 28 2009)
#  Script for creating archive for POLYSWIFT++
#  and packing up data files.
##################################################

# Load modules
import os
import sys
import string
import math
from glob import glob
from time import time
from optparse import OptionParser

def raw_default(prompt, dflt=None):
    if dflt:
        prompt = "%s [%s]: " % (prompt, dflt)
        res = raw_input(prompt)
        if not res and dflt:
            return dflt
        return res

def Usage( code ):
    print " "
    print "Usage for Polyswift++ move script:"
    print "   ps-move.py -d=<run directory> -i=<input file> "
    print "              -n=<machine hostname> \n"
    print "NOTE: Creates .tgz archive file with fraction of datafiles"
    print "      PhysFields parsed from input file *.pre"
    print "      Can select to exclude previously archived files "
    print "      in order to save time/bandwidth \n"
    sys.exit( code )

#
# Returns list of lines from file that contain
# search string
#
# @param search_string string type search
# @param fileName      string name of file in current directory
#
def grepToList(search_string, fileName):
    resList = [];
    for line in open(fileName):
        if search_string in line:
            resList.append(line)
    return resList

#
# Strips out the timestamp from a PolySwift++
# datafile name.
#
def getPsTimeStamp(fileName):
    tmpStr = string.rstrip(fileName, ".h5")
    splitStr = string.split(tmpStr,"_")
    numString = splitStr[2]
    tStep = long(numString) - 1000000
    return tStep

#
# Returns list of strings of PhysFields names in .pre file
#  Needs: grepToList()
#
def getPSFieldNames(inName, inStr):
    # inStr = "<PhysField"
    searchStr = grepToList(inStr, inName)
    psNames = []
    for str1 in searchStr:
        str2 = string.strip(str1, inStr)
        str3 = string.strip(str2, ">\n")
        str4 = string.strip(str3, " ")
        psNames.append(str4)
    return psNames

#
#
#
def getTarFieldNames(baseName, psFields, t0, dtRound, tf):
    tNames = []

    print "baseName = ", baseName
    print "psFields = ", psFields
    print "t0 = ", t0
    print "dtRound = ", dtRound
    print "tf = " , tf

    for dFieldName in psFields:

        print "Found <PhysField> = ", dFieldName
        # Append first file by default
        fileGlob = baseName + "*" + dFieldName + "*" + str(t0) + ".h5"
        tNames.append(glob(fileGlob)[0])
        # Append subset of intermediate files
        tNext = t0 + dtRound
        while (tNext < tf):
            fileGlob = baseName + "*" + dFieldName + "*" + str(tNext) + ".h5"
            print glob(fileGlob)[0]
            tNames.append(glob(fileGlob)[0])
            tNext = tNext + dtRound
        # Append last file by default
        fileGlob = baseName + "*" + dFieldName + "*" + str(tf) + ".h5"
        tNames.append(glob(fileGlob)[0])

    return tNames
##################################################

##########################################################################
# Command line option parse

parser = OptionParser()
parser.add_option("-d","--directory",
                  dest="workDir",
                  help="directory containing data",
                  default="./")

parser.add_option("-i","--preName",
                  dest="preName",
                  help="name of pre file used in run",
                  default="polyswift.pre")

parser.add_option("-n","--namehost",
                  dest="hostName",
                  help="name of host machine destination",
                  default="none")

(options,args) = parser.parse_args()
workDir  = str(options.workDir)
hostName = str(options.hostName)
preName  = str(options.preName)
baseName = string.rstrip(preName, ".pre")

# Check options
numArgs = len(sys.argv) - 1
if (numArgs == 0):
    Usage( 0 )
else:
    print " "

#
# Set interactive options
#
useExcludeFlag = raw_default("Use exclude [no/yes]? ", 'no')
tmp            = raw_default("Frac. of data to archive [1.0] ", '1.0')
tarFraction = float(tmp)
##########################################################################

################################################################################
# Error checking

preExists = os.path.exists(workDir + "/" + preName)
if (preExists == False):
    print " "
    print "*****************************************************************"
    print "ERROR: Input '*.pre' file not found \n"
    print "       Script assumes 'pre' file at same directory level from "
    print "       which this script is executed. \n"
    print "*****************************************************************"
    print " "
    sys.exit(0)
################################################################################

###############################################################################
# Input file number fraction, used to select subset of files
# Calculates time-stamp info
#   Note: changes to work directory so name search logic is cleaner

os.chdir(workDir)

##################### USER SETTINGS ####################
physFields = getPSFieldNames(preName, "<PhysField")
bndrFields = getPSFieldNames(preName, "<Boundary")
psFields = physFields + bndrFields
psFields.remove("defaultPressure")

defaultNameList = []
defaultNameList.append(preName);
defaultNameList.append(baseName + '*History*');
##################### USER SETTINGS ####################

#
# Determine integer increment w/tarFraction
# Rounds down to nearest increment for tarFraction
# Generate file names and exit data directory
#
searchStr = baseName + "*" + psFields[0] + "*" + ".h5" # Search string for all datatype files
print "searchStr = ", searchStr
tmpNames = glob(searchStr)                             # glob search results
tmpNames.sort()
print "tmpNames = ", tmpNames
t0 = getPsTimeStamp(tmpNames[0])                       # time stamp of 1st data files
dt = getPsTimeStamp(tmpNames[1]) - t0                  # dt of time stamps (t1-t0)
tf = getPsTimeStamp(tmpNames[-1])                      # time stamp of last data file
mSlope = dt - (tf - t0)                                # line Eqn -- slope
dy = dt + mSlope*(tarFraction - 1.0)                   # line Eqn -- frac vs tstamp incr.
dtRound = int(dt*math.floor(dy/dt))                    # round to integer tstamp incr.

tarFieldNames = getTarFieldNames(baseName, psFields, t0, dtRound, tf)
fileNameList = tarFieldNames + defaultNameList
os.chdir("..")
###############################################################################

############################################################################

#
# Setting filenames with path included
#
tarFile = workDir + "-" + str(time()) + ".tgz "
excludeFile = workDir + "/excludeFile.txt"
excludeExists = os.path.exists(excludeFile)

#
# Set tar command string and execute
#
if not(excludeExists):
    cmdString = "tar -zcvf " + tarFile
else:
    if (useExcludeFlag == 'yes'):
        cmdString = "tar -X " + excludeFile + " -zcvf " + tarFile
    else:
        cmdString = "tar -zcvf " + tarFile

#
# Put selected file names in cmdString
# appending workDir to each file name
#
for n in range(len(fileNameList) ):
    fileName = fileNameList[n]
    cmdString = cmdString + workDir + "/" + fileName + " "

#
# Execute tar and setting exclude file archive file
#
print "\n Constructing tar command \n", cmdString
executeTar = raw_default("\n Execute tar cmd? [no/yes]? ", 'yes')

if not(executeTar):
    print "\n Ok. Try again w/another fraction value? \n"
    sys.exit(0)
else:
    print " "
    os.system(cmdString)

if not(excludeExists):
    print "\n Creating file for h5 files to exclude from  archives \n"
    os.system("tar ft " + tarFile + " |grep h5 > " + excludeFile)
else:
    print " "
    print "\n Appending file for h5 files to exclude from archives \n"
    os.system("tar ft " + tarFile + " |grep h5 >> " + excludeFile)
############################################################################

###################################################################
# Move to hostname

if (hostName == "dipole"):
    print "scp ", tarFile, " to dipole:"
    os.system("scp " + tarFile + "swsides@dipole.txcorp.com:")
elif (hostName == "polymer"):
    print "scp ", tarFile, " to polymer:"
    os.system("scp " + tarFile + "swsides@polymer.txcorp.com:")
elif (hostName == "oic"):
    print "scp ", tarFile, " to oic login node:"
    os.system("scp " + tarFile + "s2z@login1.ornl.gov:")
else:
    print "\n hostname not recognized"
###################################################################
