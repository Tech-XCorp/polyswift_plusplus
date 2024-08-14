#!/usr/bin/env python
"""
@file    smoothingForDSA.py

@brief   Script that pre-processes geometry file for simulating DSA processes 

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

try:
    import scipy.ndimage
except:
    print "Error scipy.ndimage not found. Install package or check your PYTHONPATH"
    sys.exit(0)

try:
    import matplotlib.pyplot as plt
    mpltFlag = True
except:
    print "Error: matplotlib.pyplot not found. Install package or check your PYTHONPATH"
    print "Script will skip 2D image generation"
    mpltFlag = False
    #    sys.exit(0)

try:
    import scipy.signal as sig
except:
    print "Error: scipy.signal not found. Install package or check your PYTHONPATH"
    sys.exit(0)
################################################################################################

def gaussian_kernel_2D(sigma, size, size_y=None):
    """
    Calculate gaussian kernel explicitly

    Args:
        sigma (float) -- Width parameter for Gaussian
        size    (int) -- Length of array in x-dir
        size_y  (int) -- Length of array in y-dir (if empty then square)

    Returns:
         numpy array with gaussian matrix kernel
    """

    size = int(size)
    if not size_y:
        size_y = size
    else:
        size_y = int(size_y)

    x, y = np.mgrid[-size:size+1, -size_y:size_y+1]
    g = np.exp(-(x**2/(2*sigma**2*float(size))+y**2/(2*sigma**2*float(size_y))) )
    return g / g.sum()

def gaussian_kernel_3D(sigma, sizes):
    """
    Calculatse gaussian kernel explicitly

    Args:
        sigma (float) -- Width parameter for Gaussian
        sizes  (list) -- Length in x,y,z dirs

    Returns:
         numpy array with gaussian matrix kernel
    """

    nx = sizes[0]
    ny = sizes[1]
    nz = sizes[2]

    x, y, z = np.mgrid[-nx:nx+1, -ny:ny+1, -nz:nz+1 ]
    g = np.exp(-(x**2/(2*sigma**2*float(nx)) +
                 y**2/(2*sigma**2*float(ny)) +
                 z**2/(2*sigma**2*float(nz)) ) )
    return g / g.sum()

def lithoFilter(rawVal, lowerT, upperT):
    """
    Apply a filter for the mask values based on a threshold
    that reflects how the lithography will expose to a mask

    e.g. 0 -- 0 -- 0 -- lowerT -- x -- x -- x -- upperT -- 1 -- 1 -- 1 ...

    Args:
        rawVal  (list) -- List of (x,y,z, val) entries
        lowerT (float) -- lower threshold
        upperT (float) -- upper threshold

    Returns:
        Filtered list
    """
    if ( rawVal <= lowerT ):
        # return 0.0
        return 1.0
    if ( rawVal >= upperT ):
        # return 1.0
        return 0.0

    if ( (lowerT < rawVal) and (rawVal < upperT) ):
        return rawVal

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

def modifyHeaderFor2D(header, rasterDat, nzOrig):
    """
    Take header and set z-direction parameter for 2D example

    Args:
        header (list) -- List of string of header file for geometry data
        rasterDat     -- Data variable (to get dim info)
        nzOrig        -- Original 3D nz value

    Return:
        header modified so "K=1" for 2d examples
    """

    # Check dimensions and set nz value
    dims = rasterDat.shape
    numDims = len(dims)

    if numDims == 2:
        print "Adjust header info for 2D example \n"
        newNz = 1
    elif numDims == 3:
        newNz = nzOrig
    else:
        print "Error: dimension of filtered output data not = 2 or 3"
        sys.exit(0)

    for i,hline in enumerate(header):
        if "K=" in hline:
            newline = hline.replace("K="+str(nzOrig), "K="+str(newNz))
            header[i] = newline

    # print "New header ", header
    return header

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

    # Get header from DSA raster file (Format 1)
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

def outputRasterDataFormat1(header, rasterDat, outName):
    """
    Output pre-processed raster file data to a file. File format same as the
    input file format, but is now shifted to 'grid-coordinates'

    Args:
        header     (list)     -- List of strings for the header file
        reasterDat (np.array) -- [x,y,z,val] data
        outName    (str)      -- Name of output file
    """

    # Set x,y array sizes and #-dims
    dims = rasterDat.shape
    numDims = len(dims)
    nx = dims[0]
    ny = dims[1]

    # Check dimensions and set z-dim
    if numDims == 2:
        print "Printing 2D slice of data to output file suitable for PSim"
        nz = 1
    elif numDims == 3:
        print "Printing full 3D data to output file suitable for PSim"
        print "The 3D setup/workflow for PSim not yet in place"
        nz = dims[2]
    else:
        "Error: dimension of filtered output data not = 2 or 3"
        sys.exit(0)

    print "Actual output data dimensions --> nx, ny, nz = ", nx, " " , ny, " ", nz, "\n"

    #
    # Output filtered data
    #
    outFobj = open(outName, "w")
    for line in header:                   # Output header
        outFobj.write(line)               #

    # SWS: take care last line ? Seems to be working with 2D
    # SWS: x,y,z order here depends on DSA file format
    if numDims == 3:

        n = 0
        for z in range(nz):
            for y in range(ny):
                for x in range(nx):
                    val = rasterDat[x,y,z]
                    outFobj.write('%6d %6d %6d %10f\n' % (x, y, z, val))
                    n+=1

    elif numDims == 2:

        n = 0
        z2dVal = 0
        for y in range(ny):
            for x in range(nx):
                val = rasterDat[x,y]
                outFobj.write('%6d %6d %6d %10f\n' % (x, y, z2dVal, val))
                n+=1

    else:
        "Error: attempting to output data of dimension not = 2 or 3"
        sys.exit(0)

    # Given logic in PSim... last newline needs to be removed
    outFobj.seek(-1, os.SEEK_END)
    outFobj.truncate()

    # Clean file object
    outFobj.close()

    # Exit message
    print "Filtered/smoothed geometry data in ---> ", outName

def outputPNG2D(npDat, titleStr="", outName="out.png"):
    """
    Use matplotlib to make 2D image. If the input is a 3D array
    a 2D slice will be used (eg dat[:,:,0])

    Args:
        npDat (np.array) -- Numpy array of values

    Returns:
        Outputs 2D png image
    """

    # Take 2D slice if needed
    numDims = len(npDat.shape)
    if (numDims == 3):
        npXYslice = npDat[:,:,0]
    else:
        npXYslice = npDat

    print "Making image of: ", titleStr, " to file --> ", outName

    # Make 2D plot
    plt.title(titleStr)
    plt.imshow(npXYslice, cmap=plt.get_cmap('jet'))
    plt.colorbar()
    plt.savefig(outName)
    plt.clf()

def gaussianSmoothing(npDat, sigma=0.2, lowerThreshold=0.3, upperThreshold=0.3):
    """
    Generate gaussian kernel (with correct length scales) and use
    scipy signal modules to perform convolution in order to realize smoothing algorithm.
    Also, performs pre-smoothing step by applying a step threshold given lower and upper bounds
    Default threshold is set at 0.3 for both

    Note: inverts values around 0--1 first in order to deal with edge effects

    Args:
        npDat (np array)       -- Raw data to be smoothed
        sigma (float)          -- Gaussian width parameter
        lowerThreshold (float) -- Pre-smoothing step lower threshold value
        upperThreshold (float) -- Pre-smoothing step upper threshold value

    Return:
        nparray of smoothed data
    """

    # Apply threshold filtering (inverted)
    for x in np.nditer(npDat, op_flags=['readwrite']):
        x[...] = lithoFilter(x, lowerThreshold, upperThreshold)

    # Set x,y array sizes and #-dims
    dims = npDat.shape
    numDims = len(dims)
    nx = dims[0]
    ny = dims[1]

    # Make the Gaussian by calling the function
    # SWS: must get length scaling right on kernel
    gkern = gaussian_kernel_2D(sigma, nx)
    # gkern = gaussian_kernel_2D(sigma, 101)

    # Eventual 3D example
    # gkern = gaussian_kernel_3D(sigma, [101, 101, 11])
    # gkern = gaussian_kernel_3D(sigma, [10, 10, 4])
    print "Normalization for gaussian filter = ", gkern.sum()

    print "Convolving gaussian filter with DSA raster data"
    print "Taking 2D slice of data as an example test \n"
    npXYslice = npFileDat[:,:,0]
    gaussian2 = sig.convolve(npXYslice, gkern, mode='same')
    # gaussian2 = sig.convolve(npDat, gkern, mode='same')

    # Invert values after filtering
    for x in np.nditer(gaussian2, op_flags=['readwrite']):
        x[...] = 1.0 - x

    return gaussian2

###################################################################################################
# Command line option parse

parser = OptionParser()
usage = """%prog [options]

Script that pre-processes geometry file for simulating DSA processes
for DSA lithography project

Workflow:
  1. Take raw geometry file and process with this script
  2. Move processed geometry file to directory with PSim input files
  3. Set name of processed geometry file in PSim .pre input file
  4. Run simulation
"""

parser.set_usage(usage)

parser.add_option("--geometryFile",
                  dest="geometryFile",
                  type="string",
                  help="File name with geometry data. Script expects specfic format for file \n")

parser.add_option("--outputFile",
                  dest="outputFile",
                  type="string",
                  help="Output file name w/filtered geometry data, suitable for input to PSim \n")

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

    # Make 2D image (if 3D array then a top slice is taken)
    if (mpltFlag):
        outputPNG2D(npFileDat, "Raw DSA geometry data (2D image from slice of data)", 'dsa_filter-2d.png')

    # Run smoothing algorithm (2D or 3D option must be edited in this method manually)
    # Once 2D/3D set manually, the output in 2D or 3D currently
    gsmooth = gaussianSmoothing(npFileDat, 0.2)

    # Modify header info for 2D example
    header = modifyHeaderFor2D(header, gsmooth, nzOrig)

    # Make 2D image
    if (mpltFlag):
        outputPNG2D(gsmooth, "Filtering & Gaussian smoothing of DSA geometry data", "dsa_gFiltered_result.png")

    # Output, code selects for 2D test or full 3D example
    outputRasterDataFormat1(header, gsmooth, outputFile)

    print ""
