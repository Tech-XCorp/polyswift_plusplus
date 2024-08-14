#!/usr/bin/env python
"""
@file    viasDsa.py

@brief   File used to import geometry file for PolySwift++ runs 

@version $Id$

Copyright &copy; 2015-2020, Tech-X Corporation, Boulder, CO.
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
from viasDsaVars import *

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

def inputRasterDataFormat1(inName):
    """
    Read in confinemment raster data. Reads in header (Format1)
    for the Cartesian grid sizes.

    Filtering/processing data then outputs to file

    Args:
        inName  (str): Filename containing raster data

    Returns:
        list of 4-comp raster data, filtered with a lower/upper threshold value
    """
    header=list()
    rawDat=list()
    newDat=list()
    numHeaderLines = 4

    # Get header from TSMC raster file (Format 1)
    inFobj  = open(inName, "r+")
    for line in inFobj.readlines()[0:numHeaderLines]:
        header.append(line)
    print header

    # Parse header info for dims
    dimLine = header[2]                  # 3rd line from header
    parseList=tsplit(dimLine, ('=',',')) # Parse out dims from 3rd line
    nx = int(parseList[1])
    ny = int(parseList[3])
    nz = int(parseList[5])
    numVals = nx*ny*nz

    # Read in data
    inFobj.seek(0)                                      # Rewind file
    for line in inFobj.readlines()[numHeaderLines+1:]:  # Loop over data values in file
        floatsLine = map(float, line.split())           # Convert list of strings to floats
        rawDat.append(floatsLine)                       # Build list of float/line
    inFobj.close()                                      # Clean file object

    # Filter data and assign to newDat
    lolim = 0.1
    uplim = 0.4
    for fline in rawDat:                             # Loop over data values in file
        fval = lithoFilter(fline[3], lolim, uplim)   # Use filter function
        fline[3] = fval                              # Assign to new
        newDat.append(fline)                         # data list

#
# Simple hollow cylinder: Alignment is fixed
# and depends on 2D/3D
#
def rasterdata(x, y, z, t, params={'filename':'raster-default.dat'} ):

  # Convert dictionary to variable assigments
  filename = params['filename']

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
