#!/usr/bin/env python
"""
@file    di2abHomoACyl.py

@brief   File used to import geometry file for PolySwift++ runs 

@version $Id$

Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
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
from di2abHomoACylVars import *

#
# Distance between point r and r0 (r is a python vector [x,y,z])
#
def rdistance(r,r0):
  x = r[0]
  y = r[1]
  z = r[2]
  x0 = r0[0]
  y0 = r0[1]
  z0 = r0[2]
  r2 = math.pow(x-x0,2)+math.pow(y-y0,2)+math.pow(z-z0,2)
  return math.sqrt(r2)

#
# Simple hollow cylinder: Alignment is fixed
# and depends on 2D/3D
#
def cylinder(x, y, z, t, params={'radius':2.0,'widthParam':1.0,
                                 'cylcenterX':1.0, 'cylcenterY':1.0, 'cylcenterZ':1.0 } ):

  global NZ

  # Convert dictionary to variable assigments
  radius     = params['radius']
  widthParam = params['widthParam']
  cylcenterX = params['cylcenterX']
  cylcenterY = params['cylcenterY']
  cylcenterZ = params['cylcenterZ']

  if (NZ == 1):
    r = [x,y,1]
    cylcenterZ = 1
    # This assumes z direction is the "long" axis of the cylinder
    cylcenter = [cylcenterX, cylcenterY, 1]
  else:
    r = [x,y,z]
    # This assumes x direction is the long axis of the cylinder
    cylcenter = [x, cylcenterY, cylcenterZ]

  r = rdistance(r,cylcenter)
  tanhval = math.tanh((r-radius)/widthParam)
  wallValue = (tanhval+1)/2.0

  return wallValue
