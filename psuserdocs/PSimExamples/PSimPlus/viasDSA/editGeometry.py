#!/usr/bin/env python
"""
@file    editGeometry.py

@brief   Script that edits out edges in data file for 2D parallel test
         for the TSMC lithography project 

@version $Id$

Copyright &copy; 2017-2020, Tech-X Corporation, Boulder, CO.
All rights reserved.
"""

#!/usr/bin/env python

################################################################################################
# Check for dependent modules and load

import sys, os

try:
    from optparse import OptionParser
except:
    print "Error: optparse not found. Install package or check your PYTHONPATH"
    sys.exit(0)

try:
    import numpy as np
except:
    print "Error: numpy not found. Install package or check your PYTHONPATH"
    sys.exit(0)
################################################################################################

def tsplit(string, delimiters):
    """
    Behaves like str.split but supports multiple delimiters.

    Args:
        string (str):       Input string to be split
        delimiters (tuple): Of strings to base split on

    Returns:
        List of string split result
    """
    delimiters = tuple(delimiters)
    stack = [string,]

    for delimiter in delimiters:
        for i, substring in enumerate(stack):
            substack = substring.split(delimiter)
            stack.pop(i)
            for j, _substring in enumerate(substack):
                stack.insert(i+j, _substring)

    return stack

def modifyHeaderAfterEdit(header):
    """
    Take header and set x-y directions after editing out edges

    Args:
        header (list) -- List of string of header file for geometry data

    Return:
        header modified so "I=100" and "J=100" after editing
    """

    print "Reducing header values from I,J=101 --> I,J=100 after editing out edges"

    for i,hline in enumerate(header):
        if "I=" in hline:
            newline = hline.replace("I="+str(101), "I="+str(100))
            header[i] = newline
        if "J=" in hline:
            newline = newline.replace("J="+str(101), "J="+str(100))
            header[i] = newline

    # print "New header ", header
    return header

def editRasterDataFormat1(header, inName, outName='filter.dat'):
    """
    Read in confinemment raster data. Edits out edges of specific data
    files so dimensions can be appropriately domain decomposed for parallel runs

    Args:
        inName  (str): Filename containing raster data
        outName (str): Filename for output file

    Returns numpy array of raster data in 'grid-coordinates'
    """
    coordList=list()
    valList=list()
    numHeaderLines = 4
    numColumns = 4

    xEdgeVal = 375.0
    yEdgeVal = 375.0

    # Get header from TSMC raster file (Format 1)
    inFobj  = open(inName, "r+")
    outFobj = open(outName, "w")

    #
    # Output filtered data
    #
    outFobj = open(outName, "w")
    for line in header:                   # Output header
        outFobj.write(line)               #

    # Read in data
    inFobj.seek(0)                                    # Rewind file
    for line in inFobj.readlines()[numHeaderLines:]:  # Loop over data values in file

        floatsLine = map(float, line.split())     # Convert list of strings to floats
        fval = floatsLine[numColumns-1]           # Pick out wall value
        xval = floatsLine[0]
        yval = floatsLine[1]
        if (xval == xEdgeVal) or (yval == yEdgeVal):
            continue
        else:
            outFobj.write(line)

    inFobj.close()   # Clean file object
    outFobj.close()

def inputRasterDataFormat1(inName, outName='filter.dat'):
    """
    Read in confinemment raster data. Reads in header (Format1)
    for the Cartesian grid sizes. Converts x,y,z to 'grid-coordinates'
    running from [0...Nx],[0...Ny],[0...Nz]

    Args:
        inName  (str): Filename containing raster data
        outName (str): Filename for output file

    Returns numpy array of raster data in 'grid-coordinates'
    """
    header=list()
    coordList=list()
    valList=list()
    numHeaderLines = 4
    numColumns = 4

    # Get header from TSMC raster file (Format 1)
    inFobj  = open(inName, "r+")
    for line in inFobj.readlines()[0:numHeaderLines]:
        header.append(line)

    print "---------- Header in geometry file -----------"
    for hline in header:
        print hline.strip("\n")
    print "----------------------------------------------"

    # Parse header info for dims
    dimLine = header[2]                  # 3rd line from header
    parseList=tsplit(dimLine, ('=',',')) # Parse out dims from 3rd line
    nx = int(parseList[1])
    ny = int(parseList[3])
    nz = int(parseList[5])
    print "Dimensions from header --> nx, ny, nz = [ ", nx, " ", ny, " ", nz, " ] \n"

    # Allocate numpy array for output of data
    numVals = nx*ny*nz
    npRasterDat = np.zeros(numVals).reshape(nx,ny,nz)

    # Read in data
    inFobj.seek(0)                                    # Rewind file
    for line in inFobj.readlines()[numHeaderLines:]:  # Loop over data values in file

        floatsLine = map(float, line.split())     # Convert list of strings to floats
        fval = floatsLine[numColumns-1]           # Pick out wall value
        coordList.append(floatsLine[:-1])         # only [x,y,z] real data list
        valList.append(fval)                      # val data list only

    inFobj.close()                                # Clean file object

    # Convert data to np.array with 'grid-coordinates'
    n = 0
    for z in range(nz):
        for y in range(ny):
            for x in range(nx):
                npRasterDat[x,y,z] = valList[n]
                n+=1

    return header, npRasterDat, nz

###################################################################################################
# Command line option parse

parser = OptionParser()
usage = """%prog [options]

Script that edits geometry file for setting up 2D example simulating DSA processes
for TSMC lithography project
"""

parser.set_usage(usage)

parser.add_option("--geometryFile",
                  dest="geometryFile",
                  type="string",
                  help="File name with geometry data. Script expects specfic format for file \n")

parser.add_option("--outputFile",
                  dest="outputFile",
                  type="string",
                  help="Output file name w/edited geometry data, suitable for 2D parallel test \n")

# Acessing options
options, args = parser.parse_args()

# Get arugment values
geometryFile = options.geometryFile
outputFile   = options.outputFile

# Check for no arguments and print help message
numArgs = len(sys.argv[1:])
if (numArgs==0):
    parser.print_help()
    sys.exit(0)

# Check for needed options
if geometryFile == None:
    print "geometryFile argument not set. Use --help option for instructions"
    sys.exit(0)
if outputFile == None:
    print "outputFile argument not set. Use --help option for instructions"
    sys.exit(0)
###################################################################################################

if __name__ == '__main__':

    print ""

    # Input raster data and format into np.array
    header, npFileDat, nzOrig = inputRasterDataFormat1(geometryFile)

    # Modify header info for edited example
    header = modifyHeaderAfterEdit(header)

    # Edit raster data and output with edited header
    editRasterDataFormat1(header, geometryFile, outputFile)

    print "Edited output mask data in ", outputFile
    print ""
