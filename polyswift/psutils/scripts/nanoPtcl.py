#!/usr/bin/env python
"""
@file    nanoPtcl.py

@brief   File used to import geometry file for PolySwift++ runs 

@version $Id$

Copyright &copy; 2013-2020, Tech-X Corporation, Boulder, CO.
All rights reserved.
"""


# Import modules
import sys
import os
import math
import shutil
from shutil import *

# Since mpirun does not transfer
#   the entire environment, need to do
sys.path.append(".")

# Parsing input file name from arguments
#   that are set by readPython in main()
idstr = sys.argv[0]

# String out base name from idstr
namestr = idstr[0:-3]
varsName = namestr + "Vars.py"

# Print on rank 0 only
if MPI_RANK == 0:
  print "Importing file = ", varsName
  print " copying 'jobname'Vars.py to geometryVars.py"
  copyfile(varsName, "geometryVars.py")
else:
  fileExists = os.path.exists("geometryVars.py")
  while (not(fileExists)):
    # print "MPI_RANK ", MPI_RANK, " waiting for Vars.py file"
    fileExists = os.path.exists("geometryVars.py")

#######################################################
#      Geometry file containing common functions      #
#            for defining spatial regions             #
#######################################################
import geometry
from geometry import *
#######################################################

#######################################################
#     Additional imports for data/methods can be      #
#                    inserted below                   #
#import 'userModule'                                  #
#######################################################
