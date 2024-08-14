#!/usr/bin/env python
"""
@file    polyswift-vis.py

@brief   Script for doing visualization on POLYSWIFT++ HDF5
         data files 

@version $Id$

Copyright &copy; 2009-2020, Tech-X Corporation, Boulder, CO.
All rights reserved.
"""

##################################################
# Load modules
import os
import tables
import string
from glob import glob

def raw_default(prompt, dflt=None):
    if dflt:
        prompt = "%s [%s]: " % (prompt, dflt)
        res = raw_input(prompt)
        if not res and dflt:
            return dflt
        return res
##################################################

##################################################################
def parseDataFiles():

    filePrefix = raw_default("Run name prefix: ",'./psData')
    print "Default file prefix --> ",filePrefix
    dataFileName = raw_default("Data name: ",'totStyrDens')

    fileNames = filePrefix + "_" + dataFileName + "*" + ".h5"
    fileNameList = glob(fileNames)
    fileNameList.sort()

    print "Number of files", len(fileNameList)
    print fileNameList
    return fileNameList

##################################################################

##################################################################
def loadDensityData(fileNameList):

    rawList = []

    numFiles = len(fileNameList)
    print "Number of files found", numFiles
    print fileNameList

    for n in range(numFiles):

        inFile = fileNameList[n]

        print inFile
        f = tables.openFile(inFile)
        rawMonoDens = f.root.MonomerDensity
        b=rawMonoDens[:,:,0,0]
        rawList.append(b)

    print " "
    print "Returned list of density 2D array data"
    print " "
    return rawList

##################################################################

##################################################################
def printDensityData(densData, densNames):

    n = 0
    numDataArrays = len(densData)
    print numDataArrays, " datasets available to plot"

    while n < numDataArrays:

        imshow(densData[n])

        tmpStr = string.strip(densNames[n],".h5")
        tmpStr = string.strip(tmpStr,"./")
        title(tmpStr)
        graphicsFileName = tmpStr + ".png"
        print "Saving to file: ", graphicsFileName
        savefig(graphicsFileName)

        n = n + 1

    print "All done"
##################################################################

##################################################################
def showDensityData(densData, densNames):

    n = 0
    numDataArrays = len(densData)
    print numDataArrays, " datasets available to plot"

    while n < numDataArrays:

        close()
        imshow(densData[n])
        colorbar()

        tmpStr = string.strip(densNames[n],".h5")
        tmpStr = string.strip(tmpStr,"./")
        title(tmpStr)
        next = n + 1
        n = int(raw_default("Index of dataset: next = ",next))

    print "All done"
##################################################################

##################################################################
def showDensity(densData, densNames, n):

    imshow(densData[n])
    colorbar()

    tmpStr = string.strip(densNames[n],".h5")
    tmpStr = string.strip(tmpStr,"./")
    title(tmpStr)

    print "All done"
##################################################################

#####################################################
def refreshAll():

    global densDataNames
    global densDataList

    densDataNames = parseDataFiles()
    densDataList = loadDensityData(densDataNames)
    print "refreshed files/name returned"

#####################################################

###################################################################
# Run data modules

densDataNames = parseDataFiles()
densDataList = loadDensityData(densDataNames)

print " "
print "Methods available"
print " "
print "parseDataFiles()"
print "loadDensityData(fileNameList)"
print "printDensityData(densData, densNames)"
print "showDensityData(densData, densNames)"
print "showDensity(densData, densNames, n)"
print "refreshAll()"
print " "
print " "
print "*************************************************** "
print "SUMMARY: polyswift-vis.py "
print " "
print "Data file names in list: densDataNames"
print "Datasets in list       : densDataList"
print " "
print "Ran: "
print " densDataNames = parseDataFiles() "
print " densDataList = loadDensityData(densDataNames)"
print " "
print "Use: "
print "   showDensityData([data], [data names]): "
print "*************************************************** "
print " "
print " "
###################################################################
