#!/usr/bin/env python
"""
@file    geometry.py

@brief   Documentation goes here.

@version $Id: geometry.py 417 2020-08-04 16:54:16Z smillie $

Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
All rights reserved.
"""

# Import modules
import os
import math
import sys
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
# Sine modulated flat wall on "left" side of system
#
def leftSineWall(x, y, z, t, params={'widthParam':1.0,'walledge':10,
                                      'amplitude':3.0,'freq':4.0}):

  global NX
  global NY
  global NZ

  # Convert dictionary to variable assigments
  globals().update(params)

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
  globals().update(params)

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
  globals().update(params)

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
  globals().update(params)

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
  globals().update(params)

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
def cylinder(x, y, z, t, params={'radius':2.0,
                                 'widthParam':1.0,
                                 'cylcenterX':1.0,
                                 'cylcenterY':1.0,
                                 'cylcenterZ':1.0 } ):

  global NZ

  # Convert dictionary to variable assigments
  globals().update(params)

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
# Stripe regions running along x direction
#
def stripeRegionsX(x, y, z, t, params={'leftedge':0.0,'rightedge':0.0 }):

  global NX
  global NY
  global NZ

  # Convert dictionary to variable assigments
  globals().update(params)

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
  globals().update(params)

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

def writeGeometry(nx,ny,nz):

  global NX
  global NY
  global NZ

  NX = nx
  NY = ny
  NZ = nz

  # Open output files / write headers
  f1=open("./" + 'geometry.dat','w')
  f1.write('x   y    z   f \n')

  for ix in range(nx):
    for iy in range(ny):
      for iz in range(nz):
        fval = 0.0
#        fval = fval + bottomFlatWall(ix, iy, iz, 0,{'widthParam':1.0,'walledge':10})
#        fval = fval + topFlatWall(ix, iy, iz, 0,{'widthParam':1.0,'walledge':54})
#        fval = fval + leftFlatWall(ix, iy, iz, 0,{'widthParam':1.0,'walledge':10})
#        fval = fval + rightFlatWall(ix, iy, iz, 0,{'widthParam':1.0,'walledge':54})
#        fval = fval + stripeRegionsX(ix, iy, iz, 0,{'leftedge':20,'rightedge':40,'leftedge2':50,'rightedge2':65})
#        fval = fval + zoneAnnealX(ix, iy, iz, 0,{'zoneSize':10.0,'widthParam':2.0,'chiNmax':14.0,'chiNmin':2.0,'initialXpos':20.0,'zoneRate':200.0})
        fval = fval + leftSineZWall(ix, iy, iz, 0, params={'widthParam':1.0,'walledge':10,'amplitude':3.0,'freq':4.0})
        f1.write('%i %i %i %le  \n' % (ix, iy, iz, fval ))

  f1.close()

  print "Running mathematica batch file"
  os.system("/Applications/Mathematica.app/Contents/MacOS/MathKernel -script geovis.m")
  os.system("open geometry.png")
  print "Finished writeGeometry and opening png file"
  os.system("rm -rf geometry.dat")
