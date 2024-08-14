#!/usr/bin/env python
"""
@file    geometry.py

@brief   Documentation goes here.

@version $Id$

Copyright &copy; 2009-2020, Tech-X Corporation, Boulder, CO.
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

  params={'radius':26.0,'widthParam':1.0,'cylcenterX':32.0, 'cylcenterY':32.0, 'cylcenterZ':32.0 }

  for ix in range(nx):
    for iy in range(ny):
      for iz in range(nz):
        fval = 0.0
        fval = fval + cylinder(ix, iy, iz, 0, params)
#        fval = fval + bottomFlatWall(ix, iy, iz, 0,{'widthParam':1.0,'walledge':10})
#        fval = fval + topFlatWall(ix, iy, iz, 0,{'widthParam':1.0,'walledge':54})
#        fval = fval + leftFlatWall(ix, iy, iz, 0,{'widthParam':1.0,'walledge':10})
#        fval = fval + rightFlatWall(ix, iy, iz, 0,{'widthParam':1.0,'walledge':54})
#        fval = fval + stripeRegionsX(ix, iy, iz, 0,{'leftedge':20,'rightedge':40,'leftedge2':50,'rightedge2':65})
#        fval = fval + leftSineZWall(ix, iy, iz, 0, params={'widthParam':1.0,'walledge':10,'amplitude':3.0,'freq':4.0})
#        fval = fval + negZSineWall(ix, iy, iz, 0, params={'widthParam':1.0,'walledge':10,'amplitude':3.0,'freq':4.0})
#        fval = fval + negZCrenaWall(ix, iy, iz, 0, params={'widthParam':1.0,'lowEdge':10,'highEdge':20,'edgeSharpness':0.2,'freq':4.0})
#        fval = fval + tricorne(ix, iy, iz, 0, params={'radius':20.0,'outRadius':28.0,'widthParam':1.0,'cylcenterX':32.0,'cylcenterY':32.0,'cylcenterZ':1.0})
#        fval = fval + cylinderPtcl(ix, iy, iz, 0, params={'radius':20.0,'outRadius':28.0,'widthParam':1.0,'cylcenterX':32.0,'cylcenterY':32.0,'cylcenterZ':1.0})
#        fval = fval + chiPatch(ix, iy, iz, 0, params={'radius':5.44,'chiNBase':0.0,'chiNDel':10.0,'cylcenterX':64.0,'cylcenterY':55.0,'cylcenterZ':1.0})
        f1.write('%i %i %i %le  \n' % (ix, iy, iz, fval ))

  f1.close()

#print "Running mathematica batch file"
#os.system("/Applications/Mathematica.app/Contents/MacOS/MathKernel -script geovis.m")
#os.system("open geometry.png")
#print "Finished writeGeometry and opening png file"
#os.system("rm -rf geometry.dat")
