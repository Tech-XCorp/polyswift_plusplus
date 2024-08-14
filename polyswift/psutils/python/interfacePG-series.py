#!/usr/bin/env python
"""
@file    interfacePG-series.py

@brief   Script for creating and setting up POLYSWIFT++ run directories 

@version $Id: interfacePG-series.py 2147 2020-08-04 16:00:35Z smillie $

Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
All rights reserved.
"""

# Load modules
import os
import sys
import string
from glob import glob
######################################################################

###########################################################################
# Job series parameters

inputfileTemplatePrefix = "interfacePG"
# Generated/temp file names
preFile  = inputfileTemplatePrefix + "-gen.pre"
tmpFile1 = inputfileTemplatePrefix + "Vars.py"
inFile   = inputfileTemplatePrefix + ".in"

runParamListXXX = [
["pg2d-2comp-B1.0-chiN100", "BRATIO=1.0 NUMSTEPS=4000   UPPER_CHI=1.0" ],
["pg2d-2comp-B1.0-chiN150", "BRATIO=1.0 NUMSTEPS=8000   UPPER_CHI=1.5" ],
["pg2d-2comp-B1.0-chiN200", "BRATIO=1.0 NUMSTEPS=10000  UPPER_CHI=2.0" ],
["pg2d-2comp-B1.0-chiN250", "BRATIO=1.0 NUMSTEPS=12000  UPPER_CHI=2.5" ],

["pg2d-2comp-B3.0-chiN100", "BRATIO=3.0 NUMSTEPS=4000   UPPER_CHI=1.0" ],
["pg2d-2comp-B3.0-chiN150", "BRATIO=3.0 NUMSTEPS=8000   UPPER_CHI=1.5" ],
["pg2d-2comp-B3.0-chiN200", "BRATIO=3.0 NUMSTEPS=10000  UPPER_CHI=2.0" ],
["pg2d-2comp-B3.0-chiN250", "BRATIO=3.0 NUMSTEPS=12000  UPPER_CHI=2.5" ]
]

#["pg2d-3compDI-vf-0.010", "SURFRAC=0.010" ],
#["pg2d-3compDI-vf-0.005", "SURFRAC=0.005" ],
#["pg2d-3compDI-vf-0.002", "SURFRAC=0.002" ],
#["pg2d-3compDI-vf-0.001", "SURFRAC=0.001" ]

runParamList = [
["pg2d-3compDI-vf-0.050", "SURFRAC=0.050" ],
["pg2d-3compDI-vf-0.020", "SURFRAC=0.020" ]
]
###########################################################################

########################################
# Review parameter lists
########################################

print "Run parameters"
for n in range(len(runParamList) ):
    print runParamList[n]

################################################################################
#
# 1. Parse parameter lists
# 2. Generate txpp.py cmds and execute
# 3. Generate ps-run.py cmds and execute
#
################################################################################
for n in range(len(runParamList) ):

    # Parse parameter list and run txpp.py
    params = runParamList[n]
    runDir    = params[0]
    txppInStr = params[1]
    print " "
    print "Setting up ", runDir, " with txpp.py replace string = ", txppInStr
    print " "
    exectxppStr = "txpp.py " + inputfileTemplatePrefix + ".pre " + txppInStr
    os.system(exectxppStr)

    # Renaming generated .in file to -gen.pre file to keep filenames unique
    execMv = "mv " + inFile + " " + preFile
    os.system(execMv)

    # Execute ps-run.py command
    execJobStr = "ps-run.py -d " + runDir + " -i " + preFile
    print "execute command --> ", execJobStr
    print " "
    os.system(execJobStr)

#######################################
#
# Cleaning temporary/generated files
#
#######################################
os.system("rm -rf " + preFile)
os.system("rm -rf " + inFile)
os.system("rm -rf " + tmpFile1)
