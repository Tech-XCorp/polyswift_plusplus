#!/usr/bin/env python
"""
@file    nanoPtclWall.py

@brief   File used to import geometry file for PolySwift++ runs 

@version $Id$

Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
All rights reserved.
"""


# Import modules
import sys
import os
import math
from nanoPtclWallVars import *

#
# Generalised Heaviside step function.
#
def H(x, x0=0):
  if x < x0:
    return 0
  else:
    return 1

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
# Simple flat wall on "bottom" side of system
#
def bottomFlatWall(x, y, z, t, params={'widthParam':1.0,
                                       'walledge':10 }):

  global NX
  global NY
  global NZ

  # Convert dictionary to variable assigments
  widthParam = params['widthParam']
  walledge   = params['walledge']

  if (NZ == 1):
    r = [x,y,1]
    negEdge = [0,y,1]
  else:
    r = [x,y,z]
    negEdge = [0,y,z]

  negRdis = rdistance(r,negEdge)
  tanhval = math.tanh((walledge-negRdis)/widthParam)
  wallValue = (tanhval+1)/2.0
  return wallValue


#
# Simple flat wall on "top" side of system
#
def topFlatWall(x, y, z, t, params={'widthParam':1.0,
                                    'walledge':10 }):

  global NX
  global NY
  global NZ

  # Convert dictionary to variable assigments
  widthParam = params['widthParam']
  walledge = params['walledge']

  if (NZ == 1):
    r = [x,y,1]
    negEdge = [0,y,1]
  else:
    r = [x,y,z]
    negEdge = [0,y,z]

  negRdis = rdistance(r,negEdge)
  tanhval = math.tanh((negRdis-walledge)/widthParam)
  wallValue = (tanhval+1)/2.0
  return wallValue

#
# Stripe regions running along x direction
#
def stripeRegionsX(x, y, z, t, params={'leftedge':0.0,
                                       'rightedge':0.0 }):

  global NX
  global NY
  global NZ

  # Convert dictionary to variable assigments
  leftedge   = params['leftedge']
  rightedge  = params['rightedge']

  # Set 1st stripe values
  val1 = 0.0
  val1 = H(x,leftedge) - H(x,rightedge)

  # Set 2nd stripe values
  try:
    leftedge2
    rightedge2
    val2 = H(x,leftedge2) - H(x,rightedge2)
  except:
    val2 = 0.0

  return float( val1 + val2 )
