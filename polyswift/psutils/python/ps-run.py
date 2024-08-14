#!/usr/bin/env python
"""
@file    ps-run.py.in

@brief   Script for creating and setting up POLYSWIFT++ run directories 

@version $Id: ps-run.py.in 2147 2020-08-04 16:00:35Z smillie $

Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
All rights reserved.
"""


######################################################################
# Load modules

import os
import sys
import string
from glob import glob
from optparse import OptionParser

# This is to obtain copyfile
import shutil
from shutil import *

def raw_default(prompt, dflt=None):
    if dflt:
        prompt = "%s [%s]: " % (prompt, dflt)
        res = raw_input(prompt)
        if not res and dflt:
            return dflt
        return res

def getLastPStime():

    fileNames = glob("*Dens*.h5")

    if (len(fileNames) == 0):
        print "\n Error: (restart) no '*Dens*.h5' files found..."
        print "  was this directory created by this script?"
        print "  Did initial run fail to create h5 files? "
        print "  Run directory may have to be checked manually. \n"
        sys.exit(0)

    fileNames.sort()
    lastFileName = fileNames[-1]
    tmpStr = string.strip(lastFileName, ".h5")
    splitStr = string.split(tmpStr,"_")
    numString = splitStr[-1]
    restartStep = long(numString) - 1000000

    return restartStep

def selectHostName(rawName):

    # Might have to add other search strings
    bfindVal1 = string.find(rawName,"b0")
    bfindVal2 = string.find(rawName,"b1")
    bfindVal3 = string.find(rawName,"b2")

    if ((bfindVal1 != -1) or
        (bfindVal2 != -1) or
        (bfindVal3 != -1) ):
        newNameStr = "oic"
    else:
        newNameStr = rawName

    return newNameStr

######################################################################

#########################################################
# Command line option parse
usage = """%prog [options]

  Examples:
     initial run: ps-run.py -d <new run directory> -i <.pre input file>
     restart run: ps-run.py -d <existing directory>
"""
parser = OptionParser()

parser.add_option("-d","--directory",
                  dest="runDir",
                  help="Polyswift++ run directory")

parser.add_option("-i","--inputfile",
                  dest="inFile",
                  default="polyswift.pre",
                  help="Polyswift++ input file")

parser.add_option("-b","--printBinDir",
                  dest="prtBinDir",
                  action='store_true',
                  default=False,
                  help="Show the Polyswift++ bin directory used by script")

(options,args) = parser.parse_args()

# Acessing options
runDir = str(options.runDir)
inFile = str(options.inFile)
prtBinDir = options.prtBinDir
#########################################################

######################################################################################
# Setting script paths

hostName = os.getenv("HOSTNAME")
if hostName == None:
    import commands
    # print "HOSTNAME not found, using hostname command"
    status, hostName = commands.getstatusoutput("hostname -s")

hostNameStr = selectHostName(hostName)
binDir = "/Users/swsides/opt/software-llvm_appleclang/polyswift-r2170/bin"
# binDir = "/scr1_oxygen/swsides/internal-trunk/polyswift-r1696/bin"

# If install binDir not found default to composer install
if not os.path.exists(binDir):
    binDir = "/Applications/Polyswift-1.0.0/PolyswiftComposer.app/Contents/engine/bin"
    print "Setting bin direcotry to: \n", binDir
######################################################################################

################################################################################
# Error checking

runExists = os.path.exists(runDir)
preExists = os.path.exists(inFile)
binExists = os.path.exists(binDir)

if (binExists == False):
    print " "
    print "*****************************************************************"
    print "ERROR: path to 'polyswift/bin' not found \n"
    print "   1. Check to see that this bin directory exists \n"
    print "   2. Check the permissions on the bin directory \n"
    print "   3. 'polyswift' soft link in the install directory may be set"
    print "       to old revision. This could be corrected manually \n"
    print "   4. If these do not work, contact swsides@txcorp.com \n"
    print "*****************************************************************"
    print " "
    sys.exit(0)

if (prtBinDir == True):
    print " "
    print "PolySwift++ engine will use bin directory ", binDir
    print " "
    sys.exit(0)

if ( (preExists == False) and (runExists == False) ):
    print " "
    print "*****************************************************************"
    print "ERROR: Input '*.pre' file not found \n"
    print "       Script assumes 'pre' file at same directory level from "
    print "       which this script is executed. \n"
    print "Note:  '-i ___*pre' file may be omitted if this is a restart."
    print "       and run directory must be specified"
    print "*****************************************************************"
    print " "
    sys.exit(0)

copyFileList = [
    binDir + '/pspp-gen.py',
    binDir + '/polyswift.py',
    binDir + '/geometry.py',
    binDir + '/cleanps.sh',
    './' + inFile
]
################################################################################

######################## NEW RUN ################################
if (runExists == False):

    print "*******************************************"
    print "***************** New run *****************"
    print "*******************************************"

    # Create run directory
    print "Creating run directory", runDir
    print " "
    os.mkdir(runDir)

    # Copy files into run directory
    for n in range(len(copyFileList) ):
        fileName = copyFileList[n]
        destName = runDir
        copy2(fileName, destName)
        print "Copied file ", fileName, " to ", destName

    # Changing into run directory
    print "Switching to run directory ", runDir
    os.chdir(runDir)
    print " "

####################### EXISTING RUN ############################
else:

    print "Run directory ", runDir, " exists... assuming restart"
    print " "

    # Changing into run directory
    print "Switching to run directory ", runDir
    os.chdir(runDir)
    print " "
    lastPSstep = getLastPStime( )

    preFileList = glob("*.pre")
    if len(preFileList) != 1:
        print "More than one .pre found... edit directory"
        sys.exit(0)
    else:
        inFile = preFileList[0]

    print "lastPSstep      = ", lastPSstep
    print "Found .pre file = ", inFile
    print " "

#################################################################

####################### COMMON RUN TASKS ########################

# Show hostname to be used in script
print "Using hostname = ", hostNameStr

# Copy polyswift.py template to runname.py
if (runExists == False):
    runName = inFile[0:-4]
    pyName = runName + ".py"
    print "moving polyswift.py template to name = ", pyName
    move("polyswift.py", pyName)

# Store run notes
print " "
notesStr = raw_default("Notes (one line):  ", ' ')
print "Notes: ", notesStr

if (runExists == False):
    print "...writing notes to file."
    g=open("run-notes.txt", 'w')
    g.write("Log for Run Notes \n")
    g.write("\n")
    g.write(notesStr)
    g.write("\n")
    g.write("\n")
    g.close()
else:
    g=open("run-notes.txt", 'a')
    print "...appending notes to file."
    g.write(notesStr)
    g.write("\n")
    g.write("\n")
    g.close()

#################################################################

######################################################################################
# Run PBS script generator

if (hostNameStr == 'frost2'   or
    hostNameStr == 'frost3'   or
    hostNameStr == 'frost4'   or
    hostNameStr == 'frost5'   or
    hostNameStr == 'oxygen02' or
    hostNameStr == 'oxygen03' or
    hostNameStr == 'iter'     or
    hostNameStr == 'carbon'   or
    hostNameStr == 'oic'):

    print "Running script generator for ", hostNameStr

    # Setting up -i xxx.pre file name
    preFileExec = " -i " + inFile[0:-4]

    if (runExists == False):
        ppExecStr = "pspp-gen.py -d " + runDir + " -b " + binDir + preFileExec
        print "Template generator  exec string: ", ppExecStr
        os.system(ppExecStr)
        print " "
    else:
        ppExecStr = "pspp-gen.py -d " + runDir + " -r " + str(lastPSstep) + " -b " + binDir + preFileExec
        print "Template generator  exec string: ", ppExecStr
        os.system(ppExecStr)
        print " "

else:

    print "No script generator for ", hostNameStr, "... assuming local run"
    print " "

######################################################################################

################################################################################
#  Run command execution

if (hostNameStr == 'oxygen02' or
    hostNameStr == 'oxygen03' or
    hostNameStr == 'carbon'   or
    hostNameStr == 'frost2'   or
    hostNameStr == 'frost3'   or
    hostNameStr == 'frost4'   or
    hostNameStr == 'frost5'   or
    hostNameStr == 'iter'     or
    hostNameStr == 'oic'):

    print "Submitting PBS script"
    os.system("qsub pspp.pbs")

else:

    # Selects restart options if run directory exists
    if (runExists):
        restartStep = raw_default("Found restart step:  ", lastPSstep)
        restartExecStr = " -r " + str(restartStep)
    else:
        restartExecStr = " "

    print "Running polyswift locally"
    runFlag = raw_default("Run type:[serial/parallel]: ", 'serial')
    preFileExec = " -i " + inFile
    runNameExec = " -o " + inFile[0:-4]

    if (runFlag == 'serial'):
        runExecStr = binDir + "/polyswiftser " + preFileExec + restartExecStr + runNameExec
        print "Execute string --> ", runExecStr
    else:
        nprocs = raw_default("number of processors", 2)
        runExecStr = "mpiexec -np " + str(nprocs) + " " + binDir + "/polyswift "  \
                                    + preFileExec + restartExecStr + runNameExec
        print "Execute string --> ", runExecStr

    g=open("run-cmd.sh", 'w')
    g.write("# Local run command \n")
    g.write("\n")
    g.write(runExecStr)
    g.close()
    os.system("chmod +x run-cmd.sh")

    os.system(runExecStr)

print " "
################################################################################
