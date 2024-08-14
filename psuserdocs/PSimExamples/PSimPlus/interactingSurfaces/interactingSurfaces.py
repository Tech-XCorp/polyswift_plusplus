#!/usr/bin/env python
"""
@file    interactingSurfaces.py

@brief   File used to define geometry in PolySwift++ runs 

@version $Id$

Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
All rights reserved.
"""


# Import modules
import sys
import os
import math
import shutil
from shutil import *

# Since mpirun does not transfer the entire environment, need to do
sys.path.append(".")
from interactingSurfacesVars import *

###################################################################

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
# Sine modulated flat wall on "low" side of system
#
def lowSineWall(x, y, z, t, params={'widthParam':1.0,
                                     'walledge':10,
                                     'amplitude':3.0,
                                     'freq':4.0}):
  global NX
  global NY
  global NZ

  # Convert dictionary to variable assigments
  widthParam = params['widthParam']
  walledge   = params['walledge']
  amplitude  = params['amplitude']
  freq       = params['freq']

  if (NZ == 1):
    r = [x,y,1]
    negEdge = [x,0,1]
  else:
    r = [x,y,z]
    negEdge = [x,0,z]

  negRdis = rdistance(r,negEdge)
  negRdis = negRdis - (amplitude*math.sin(freq*2.0*math.pi*x/NX))

  tanhval = math.tanh((walledge-negRdis)/widthParam)
  wallValue = (tanhval+1)/2.0
  return wallValue

#
# Sine modulated flat wall on "high" side of system
#
def highSineWall(x, y, z, t, params={'widthParam':1.0,
                                      'walledge':10,
                                      'amplitude':3.0,
                                      'freq':4.0}):

  global NX
  global NY
  global NZ

  # Convert dictionary to variable assigments
  widthParam = params['widthParam']
  walledge   = params['walledge']
  amplitude  = params['amplitude']
  freq       = params['freq']

  if (NZ == 1):
    r = [x,y,1]
    negEdge = [x,0,1]
  else:
    r = [x,y,z]
    negEdge = [x,0,z]

  negRdis = rdistance(r,negEdge)
  negRdis = negRdis - (amplitude*math.sin(freq*2.0*math.pi*x/NX))

  tanhval = math.tanh((negRdis-walledge)/widthParam)
  wallValue = (tanhval+1)/2.0
  return wallValue

#
# Simple flat wall on "low" side of system
#
def lowFlatWall(x, y, z, t, params={'widthParam':1.0,
                                     'walledge':10 }):

  global NX
  global NY
  global NZ

  # Convert dictionary to variable assigments
  widthParam = params['widthParam']
  walledge   = params['walledge']

  if (NZ == 1):
    r = [x,y,1]
    negEdge = [x,0,1]
  else:
    r = [x,y,z]
    negEdge = [x,0,z]

  negRdis = rdistance(r,negEdge)
  tanhval = math.tanh((walledge-negRdis)/widthParam)
  wallValue = (tanhval+1)/2.0
  return wallValue


#
# Simple flat wall on "high" side of system
#
def highFlatWall(x, y, z, t, params={'widthParam':1.0,
                                      'walledge':10 }):

  global NX
  global NY
  global NZ

  # Convert dictionary to variable assigments
  widthParam = params['widthParam']
  walledge   = params['walledge']

  if (NZ == 1):
    r = [x,y,1]
    negEdge = [x,0,1]
  else:
    r = [x,y,z]
    negEdge = [x,0,z]

  negRdis = rdistance(r,negEdge)
  tanhval = math.tanh((negRdis-walledge)/widthParam)
  wallValue = (tanhval+1)/2.0
  return wallValue
