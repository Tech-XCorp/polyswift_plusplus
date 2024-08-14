#!/usr/bin/env python
"""
@file    polyDisperse.py

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
#import geometry
#from geometry import *
#######################################################

#######################################################
#     Additional imports for data/methods can be      #
#                    inserted below                   #
#import 'userModule'                                  #
#######################################################

# Import modules
#import os
#import math
#import sys
import geometryVars
from geometryVars import *

# Since mpirun does not transfer
# the entire environment, need to do
sys.path.append(".")

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
# Sine modulated wall on "negative Z" side of system
#
def negZCrenaWall(x, y, z, t, params={'widthParam':1.0,
                                      'lowEdge':10,
                                      'highEdge':20,
                                      'edgeSharpness':0.2,
                                      'freq':4.0}):

  global NX
  global NY
  global NZ

  # Convert dictionary to variable assigments
  widthParam    = params['widthParam']
  lowEdge       = params['lowEdge']
  highEdge      = params['highEdge']
  edgeSharpness = params['edgeSharpness']
  freq          = params['freq']

  r = [x,y,z]
  negEdge = [x,y,0]
  negRdis = rdistance(r,negEdge)

  sinpart = math.sin(freq*2.0*math.pi*y/NY)
  tpart = math.tanh(sinpart/edgeSharpness)
  walledge =  (0.5*(highEdge-lowEdge)*tpart) + (0.5*(highEdge+lowEdge))

  tanhval = math.tanh((walledge-negRdis)/widthParam)
  wallValue = (tanhval+1)/2.0
  return wallValue

#
# Sine modulated wall on "negative Z" side of system
#
def negZFlatWall(x, y, z, t, params={'widthParam':1.0,'walledge':10}):

  global NX
  global NY
  global NZ

  # Convert dictionary to variable assigments
  widthParam = params['widthParam']
  walledge   = params['walledge']

  r = [x,y,z]
  negEdge = [x,y,0]

  negRdis = rdistance(r,negEdge)
  tanhval = math.tanh((walledge-negRdis)/widthParam)
  wallValue = (tanhval+1)/2.0
  return wallValue

#
# Sine modulated wall on "negative Z" side of system
#
def posZFlatWall(x, y, z, t, params={'widthParam':1.0,'walledge':10}):

  global NX
  global NY
  global NZ

  # Convert dictionary to variable assigments
  widthParam = params['widthParam']
  walledge   = params['walledge']

  r = [x,y,z]
  negEdge = [x,y,0]

  negRdis = rdistance(r,negEdge)
  tanhval = math.tanh((negRdis-walledge)/widthParam)
  wallValue = (tanhval+1)/2.0
  return wallValue

#
# Sine modulated wall on "negative Z" side of system
#
def negZSineWall(x, y, z, t, params={'widthParam':1.0,'walledge':10,
                                     'amplitude':3.0,'freq':4.0}):

  global NX
  global NY
  global NZ

  # Convert dictionary to variable assigments
  widthParam = params['widthParam']
  walledge   = params['walledge']
  amplitude  = params['amplitude']
  freq       = params['freq']

  r = [x,y,z]
  negEdge = [x,y,0]

  negRdis = rdistance(r,negEdge)
  negRdis = negRdis - (amplitude*math.sin(freq*2.0*math.pi*y/NY))

  tanhval = math.tanh((walledge-negRdis)/widthParam)
  wallValue = (tanhval+1)/2.0
  return wallValue

#
# Sine modulated wall on "negative Z" side of system
#
def posZSineWall(x, y, z, t, params={'widthParam':1.0,'walledge':10,
                                     'amplitude':3.0,'freq':4.0}):

  global NX
  global NY
  global NZ

  # Convert dictionary to variable assigments
  widthParam = params['widthParam']
  walledge   = params['walledge']
  amplitude  = params['amplitude']
  freq       = params['freq']

  r = [x,y,z]
  negEdge = [x,y,0]

  negRdis = rdistance(r,negEdge)
  negRdis = negRdis - (amplitude*math.sin(freq*2.0*math.pi*y/NY))

  tanhval = math.tanh((negRdis-walledge)/widthParam)
  wallValue = (tanhval+1)/2.0
  return wallValue

#
# Sine modulated flat wall on "left" side of system
#
def leftSineWall(x, y, z, t, params={'widthParam':1.0,'walledge':10,
                                      'amplitude':3.0,'freq':4.0}):

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
# Sine modulated flat wall on "right" side of system
#
def rightSineWall(x, y, z, t, params={'widthParam':1.0,'walledge':10,
                                      'amplitude':3.0,'freq':4.0}):

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
# Sine modulated flat wall on "left" side of system
#
def leftSineZWall(x, y, z, t, params={'widthParam':1.0,'walledge':10,
                                      'amplitude':3.0,'freq':4.0}):

  global NX
  global NY
  global NZ

  # Convert dictionary to variable assigments
  widthParam = params['widthParam']
  walledge   = params['walledge']
  amplitude  = params['amplitude']
  freq       = params['freq']

  globals().update(params)

  if (NZ == 1):
    r = [x,y,1]
    negEdge = [x,0,1]
  else:
    r = [x,y,z]
    negEdge = [x,0,z]

  negRdis = rdistance(r,negEdge)
  negRdis = negRdis - (amplitude*math.sin(freq*2.0*math.pi*z/NX))

  tanhval = math.tanh((walledge-negRdis)/widthParam)
  wallValue = (tanhval+1)/2.0
  return wallValue

#
# Simple flat wall on "left" side of system
#
def leftFlatWall(x, y, z, t, params={'widthParam':1.0,'walledge':10 }):

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
# Simple flat wall on "right" side of system
#
def rightFlatWall(x, y, z, t, params={'widthParam':1.0,'walledge':10 }):

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

#
# Simple flat wall on "bottom" side of system
#
def bottomFlatWall(x, y, z, t, params={'widthParam':1.0,'walledge':10 }):

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
def topFlatWall(x, y, z, t, params={'widthParam':1.0,'walledge':10 }):

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

#
# Simple filled cylinder: Alignment is fixed
# and depends on 2D/3D
#
def cylinderPtcl(x, y, z, t, params={'radius':2.0,
                                     'widthParam':1.0,
                                     'cylcenterX':1.0,
                                     'cylcenterY':1.0,
                                     'cylcenterZ':1.0 } ):

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

  # Flipping inside/out
  wallValue = 1.0 - wallValue

  return wallValue

#
# Simple filled cylinder: Alignment is fixed
# and depends on 2D/3D
#
def chiPatch(x, y, z, t, params={'radius':2.0,
                                 'chiNBase':1.0,
                                 'chiNDel':1.0,
                                 'cylcenterX':1.0,
                                 'cylcenterY':1.0,
                                 'cylcenterZ':1.0 } ):

  global NZ

  # Default
  widthParam = 1.0

  # Convert dictionary to variable assigments
  radius     = params['radius']
  chiNBase   = params['chiNBase']
  chiNDel    = params['chiNDel']
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

  # Flipping inside/out
  chiNPatch = chiNDel*(1.0 - wallValue)
  chiN = chiNBase+chiNPatch
  return chiN

#
# Simple hollow cylinder: Alignment is fixed
# and depends on 2D/3D
#
def tricorne(x, y, z, t, params={'radius':10.0,
                                 'outRadius':14.0,
                                 'widthParam':1.0,
                                 'cylcenterX':1.0,
                                 'cylcenterY':1.0,
                                 'cylcenterZ':1.0 } ):

  global NZ

  # Convert dictionary to variable assigments
  radius     = params['radius']
  outRadius  = params['outRadius']
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

  dely = y-cylcenterY
  delx = x-cylcenterX

  if (abs(delx) < 0.00001):
    delx = math.copysign(0.00001,delx)

  theta = math.atan2(dely,delx)
  cospart = math.pow(math.cos(1.5*theta),2)
  rval = ((outRadius-radius)*cospart) + radius

  r = rdistance(r,cylcenter)
  tanhval = math.tanh((r-rval)/widthParam)
  wallValue = (tanhval+1)/2.0

  return wallValue

#
# Stripe regions running along x direction
#
def stripeRegionsX(x, y, z, t, params={'leftedge':0.0,'rightedge':0.0 }):

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

#
# Stripe regions running along x direction
#
def stripeRegionsY(x, y, z, t, params={'leftedge':0.0,'rightedge':0.0 }):

  global NX
  global NY
  global NZ

  # Convert dictionary to variable assigments
  leftedge   = params['leftedge']
  rightedge  = params['rightedge']

  # Set 1st stripe values
  val1 = 0.0
  val1 = H(y,leftedge) - H(y,rightedge)

  # Set 2nd stripe values
  try:
    leftedge2
    rightedge2
    val2 = H(y,leftedge2) - H(y,rightedge2)
  except:
    val2 = 0.0

  return float( val1 + val2 )

#
# Stripe regions running along x direction
#
def zoneAnnealX(x, y, z, t, params={'zoneSize':10.0,
                                    'widthParam':2.0,
                                    'chiNmax':14.0,
                                    'chiNmin':2.0,
                                    'initialXpos':20.0,
                                    'zoneRate':200.0}):
  global NX
  global NY
  global NZ

  # Convert dictionary to variable assigments
  # globals().update(params)
  zoneSize    = params['zoneSize']
  widthParam  = params['widthParam']
  chiNmax     = params['chiNmax']
  chiNmin     = params['chiNmin']
  initialXpos = params['initialXpos']
  zoneRate    = params['zoneRate']

  # Use time to calc center/edges of zone
  cpos = initialXpos + (t/zoneRate)
  ll = x - (cpos - zoneSize)
  rr = (cpos + zoneSize) - x

  # Tanh interface on left and right sides of zone
  tmpLeft  = ll/widthParam
  tmpRight = rr/widthParam
  tanhLeft  = math.tanh(tmpLeft)
  tanhRight = math.tanh(tmpRight)

  # Zone value and scale for max/min chiN
  tmp = 0.5*(tanhLeft + tanhRight)
  val  = (tmp*(chiNmax-chiNmin))+chiNmin

  return float(val)
