#!/usr/bin/env python
"""
@file    psVis.py.in

@brief   Documentation goes here.

@version $Id: psVis.py.in 2147 2020-08-04 16:00:35Z smillie $

Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
All rights reserved.
"""

#!/usr/bin/env python

#########################################################
# Load modules

import os
import sys
import string

def Usage( code ):
    print " "
    print "Usage: Polyswift++ remote VisIt script: "
    print " "
    print "   visit -cli -s psVis.py"
    print " "
    print " Note: if unstable add -noconfig"
    print " "
    sys.exit( code )
#########################################################

def setDataNames():

    global sisoDirName
    global sisoDirList

    sisoDirName = [" " ]
    sisoDirList = [ [" "] ]

    # srcDir = os.getenv("POLYSWIFT_LOC")
    # bilderDir = os.getenv("BILDER_TOPDIR")
    # 0

    # 1
    sisoDirName.append("ps1-pi1-7030")
    sisoDirList.append([
            "ps1-pi1-3d128-2.7030",
            "ps1-pi1-3d128-3.7030",
            "ps1-pi1-3d128-4.7030",
            "ps1-pi1-3d128-5.7030"
            ])
    # 2
    sisoDirName.append("ps1-pi2-7426")
    sisoDirList.append([
            "ps1-pi2-3d128-2.7426",
            "ps1-pi2-3d128-3.7426",
            "ps1-pi2-3d128-4.7426",
            "ps1-pi2-3d128-5.7426",
            ])
    # 3
    sisoDirName.append("ps2-pi1-7030")
    sisoDirList.append([
            "ps2-pi1-3d128-2.7030",
            "ps2-pi1-3d128-3.7030",
            "ps2-pi1-3d128-4.7030",
            "ps2-pi1-3d128-5.7030"
            ])
    # 4
    sisoDirName.append("ps2-pi2-7030")
    sisoDirList.append([
            "ps2-pi2-3d128-2.7030",
            "ps2-pi2-3d128-3.7030",
            "ps2-pi2-3d128-4.7030",
            "ps2-pi2-3d128-5.7030"
            ])
    # Asymmetric SISO 1 [5]
    sisoDirName.append("sisoAsym-A1")
    sisoDirList.append([
            "sisoAsym-A1-c00",
            "sisoAsym-A1-c02",
            "sisoAsym-A1-c04",
            "sisoAsym-A1-c06"
            ])
    # Asymmetric SISO 2 [6]
    sisoDirName.append("sisoAsymT120-A2")
    sisoDirList.append([
            "sisoAsymT120-A2-c00",
            "sisoAsymT120-A2-c02",
            "sisoAsymT120-A2-c04",
            "sisoAsymT120-A2-c06"
            ])
    # Asymmetric SISO 3 [7]
    sisoDirName.append("sisoAsym-A3")
    sisoDirList.append([
            "sisoAsym-A3-c00",
            "sisoAsym-A3-c02",
            "sisoAsym-A3-c04",
            "sisoAsym-A3-c06"
            ])
    # Asymmetric SISO 4 [8]
    sisoDirName.append("sisoAsym2-A4")
    sisoDirList.append([
            "sisoAsym2-A4-c00",
            "sisoAsym2-A4-c02",
            "sisoAsym2-A4-c04",
            "sisoAsym2-A4-c06"
            ])

    # Asymmetric SISO 5 [9]
    sisoDirName.append("sisoAsym-A5")
    sisoDirList.append([
            "sisoAsym-A5-c00",
            "sisoAsym-A5-c02",
            "sisoAsym-A5-c04",
            "sisoAsym-A5-c06"
            ])

def help():

    global sisoDirList
    global sisoDirName

    setDataNames()

    print "\n Script for automating VisIt"
    print "run with ----> 'visit -cli -s myscript.py'"
    print " "
    print "\n Available modules"
    print " "
    print "def setDataNames()"
    print "def visPGoxy(dataDirStr)"
    print "def visFeOxy(dataDirList)"
    print "def visPGoxyAll(dataDirList, dataDirName, plotFlagStr)"
    print "def save3DWindow(iName)"
    print "def save2DWindow(iName)"
    print "def setPseudoColorAttrs()"
    print "def setContourAttrs()"
    print "def rotateImageXY()"
    print "def rotateImageXZ()"
    print "def set3DView1()"
    print "def set3DView2()"
    print " "
    print "\n Data lists"
    print " "
    for n in range(len(sisoDirList)):
        print "index = ", n, " ", sisoDirList[n]

    print "Data directory names"
    print sisoDirName

##############################################################################

def visPGoxy(dataDirStr):

    global topDir
    global dataBaseName
    global hostName
    global densTag

    # Name of machine must exactly match profile name
    # ie "oxygen:" not "oxygen.txcorp.com:"
    testsDir = hostName + topDir + "/" + dataDirStr
    dBasePath = testsDir + "/" + dataBaseName

    OpenDatabase(dBasePath, 0)

    AddPlot("Pseudocolor", "MonomerDensity", 1, 1)
    SetActivePlots(0)
    SetActiveWindow(1)
    setPseudoColorAttrs()

    set3DView2()
    DrawPlots()

def visFeOxy(dataDirList):

    global topDir
    global dataBaseName
    global hostName
    global densTag

    print "\n", dataDirList
    testsDir = hostName + topDir
    feFileName = "polyswift_History.h5"
    dBaseList = []

    SetActiveWindow(2)

    for n in range(len(dataDirList)):

        dBasePath = testsDir + "/" + dataDirList[n] + "/" + feFileName
        dBaseList.append(dBasePath)
        OpenDatabase(dBasePath, 0)
        AddPlot("Curve", "freeE1", 1, 1)

    DrawPlots()
    print "dBaseList = ", dBaseList
    return dBaseList

def visPGoxyAll(dataDirList, dataDirName, plotFlagStr):

    global topDir
    global dataBaseName
    global hostName
    global densTag

    SetActiveWindow(1)

    numDirs = len(dataDirList)
    print "\n Number of data directories = ", numDirs

    for idir in range(numDirs):

        dataDirStr = dataDirList[idir]

        print "\n Analyzing data in ", dataDirStr
        testsDir = hostName + topDir + "/" + dataDirStr
        dBasePath = testsDir + "/" + dataBaseName
        OpenDatabase(dBasePath, 0)
        lastState = GetDatabaseNStates()
        ReplaceDatabase(dBasePath, lastState)

        if (plotFlagStr == "contour"):
            AddPlot("Contour", "MonomerDensity", 1, 1)
        elif (plotFlagStr == "pseudo"):
            AddPlot("Pseudocolor", "MonomerDensity", 1, 1)
        else:
            print "Plot type string in visPGoxyAll not recognized"
            sys.exit()

        SetActivePlots(0)
        set3DView1()

        if (plotFlagStr == "contour"):
            setContourAttrs()
        else:
            setPseudoColorAttrs()

        DrawPlots()

        # Make graphics file
        save3DWindow(dataDirStr + densTag)

        # Order matters
        DeleteActivePlots()
        CloseDatabase(dBasePath)
        ClearWindow()

    # Plot history data
    print "\n Plotting all fE data"
    dataBases = visFeOxy(dataDirList)
    save2DWindow(dataDirName + "-fE")

    # Clear all plots (must be before data cleared)
    DeleteAllPlots()
    ClearAllWindows()

    # Clear all history data
    for id in range(len(dataBases)):
        print "\n Closing database ", dataBases[id]
        CloseDatabase(dataBases[id])

def save3DWindow(iName):

    imageName = iName + ".png"

    # Set the save window options
    s = SaveWindowAttributes()
    s.fileName = imageName
    s.format = s.PNG
    s.family = 0
    s.progressive = 1
    s.quality = 90
    SetSaveWindowAttributes(s)
    SaveWindow()     # Save active vis window.

def save2DWindow(iName):

    imageName = iName + ".png"

    # Set the save window options
    s = SaveWindowAttributes()
    s.fileName = imageName
    s.format = s.PNG
    s.family = 0
    s.progressive = 1
    s.width = 800
    s.height = 400
    s.quality = 90
    SetSaveWindowAttributes(s)
    SaveWindow()     # Save active vis window.

def setPseudoColorAttrs():

    PseudocolorAtts = PseudocolorAttributes()
#    PseudocolorAtts.min = 0
#    PseudocolorAtts.max = 1
#    PseudocolorAtts.minFlag = 1
#    PseudocolorAtts.maxFlag = 1
    PseudocolorAtts.legendFlag = 1
    PseudocolorAtts.lightingFlag = 1
    PseudocolorAtts.centering = PseudocolorAtts.Natural  # Natural, Nodal, Zonal
    PseudocolorAtts.scaling = PseudocolorAtts.Linear  # Linear, Log, Skew
    PseudocolorAtts.limitsMode = PseudocolorAtts.OriginalData  # OriginalData, CurrentPlot
    PseudocolorAtts.pointSize = 0.05
    PseudocolorAtts.pointType = PseudocolorAtts.Point  # Box, Axis, Icosahedron, Point, Sphere
    PseudocolorAtts.skewFactor = 1
    PseudocolorAtts.opacity = 1
    PseudocolorAtts.colorTableName = "hot"
    PseudocolorAtts.smoothingLevel = 0
    PseudocolorAtts.pointSizeVarEnabled = 0
    PseudocolorAtts.pointSizeVar = "default"
    PseudocolorAtts.pointSizePixels = 2
    SetPlotOptions(PseudocolorAtts)

def setContourAttrs():

    ContourAtts = ContourAttributes()

    ContourAtts.contourMethod = ContourAtts.Value  # Level, Value, Percent
    ContourAtts.contourNLevels = 2
    ContourAtts.contourValue = (0.75, 0.50)
#    ContourAtts.contourValue = (0.40, 0.20)
    ContourAtts.contourPercent = ()

    ContourAtts.defaultPalette.GetControlPoints(0).colors = (255, 0, 0, 255)
    ContourAtts.defaultPalette.GetControlPoints(0).position = 0
    ContourAtts.defaultPalette.GetControlPoints(1).colors = (0, 255, 0, 205)
    ContourAtts.defaultPalette.GetControlPoints(1).position = 0.03
    ContourAtts.defaultPalette.GetControlPoints(2).colors = (0, 0, 255, 255)
    ContourAtts.defaultPalette.GetControlPoints(2).position = 0.07
    ContourAtts.defaultPalette.GetControlPoints(3).colors = (0, 255, 255, 255)
    ContourAtts.defaultPalette.GetControlPoints(3).position = 0.1
    ContourAtts.defaultPalette.GetControlPoints(4).colors = (255, 0, 255, 255)
    ContourAtts.defaultPalette.GetControlPoints(4).position = 0.14
    ContourAtts.defaultPalette.GetControlPoints(5).colors = (255, 255, 0, 255)
    ContourAtts.defaultPalette.GetControlPoints(5).position = 0.17
    ContourAtts.defaultPalette.GetControlPoints(6).colors = (255, 135, 0, 255)
    ContourAtts.defaultPalette.GetControlPoints(6).position = 0.21
    ContourAtts.defaultPalette.GetControlPoints(7).colors = (255, 0, 135, 255)
    ContourAtts.defaultPalette.GetControlPoints(7).position = 0.24
    ContourAtts.defaultPalette.GetControlPoints(8).colors = (168, 168, 168, 255)
    ContourAtts.defaultPalette.GetControlPoints(8).position = 0.28
    ContourAtts.defaultPalette.GetControlPoints(9).colors = (255, 68, 68, 255)
    ContourAtts.defaultPalette.GetControlPoints(9).position = 0.31
    ContourAtts.defaultPalette.GetControlPoints(10).colors = (99, 255, 99, 255)
    ContourAtts.defaultPalette.GetControlPoints(10).position = 0.34
    ContourAtts.defaultPalette.GetControlPoints(11).colors = (99, 99, 255, 255)
    ContourAtts.defaultPalette.GetControlPoints(11).position = 0.38
    ContourAtts.defaultPalette.GetControlPoints(12).colors = (40, 165, 165, 255)
    ContourAtts.defaultPalette.GetControlPoints(12).position = 0.41
    ContourAtts.defaultPalette.GetControlPoints(13).colors = (255, 99, 255, 255)
    ContourAtts.defaultPalette.GetControlPoints(13).position = 0.45
    ContourAtts.defaultPalette.GetControlPoints(14).colors = (255, 255, 99, 255)
    ContourAtts.defaultPalette.GetControlPoints(14).position = 0.48
    ContourAtts.defaultPalette.GetControlPoints(15).colors = (255, 170, 99, 255)
    ContourAtts.defaultPalette.GetControlPoints(15).position = 0.52
    ContourAtts.defaultPalette.GetControlPoints(16).colors = (170, 79, 255, 255)
    ContourAtts.defaultPalette.GetControlPoints(16).position = 0.55
    ContourAtts.defaultPalette.GetControlPoints(17).colors = (150, 0, 0, 255)
    ContourAtts.defaultPalette.GetControlPoints(17).position = 0.59
    ContourAtts.defaultPalette.GetControlPoints(18).colors = (0, 150, 0, 255)
    ContourAtts.defaultPalette.GetControlPoints(18).position = 0.62
    ContourAtts.defaultPalette.GetControlPoints(19).colors = (0, 0, 150, 255)
    ContourAtts.defaultPalette.GetControlPoints(19).position = 0.66
    ContourAtts.defaultPalette.GetControlPoints(20).colors = (0, 109, 109, 255)
    ContourAtts.defaultPalette.GetControlPoints(20).position = 0.69
    ContourAtts.defaultPalette.GetControlPoints(21).colors = (150, 0, 150, 255)
    ContourAtts.defaultPalette.GetControlPoints(21).position = 0.72
    ContourAtts.defaultPalette.GetControlPoints(22).colors = (150, 150, 0, 255)
    ContourAtts.defaultPalette.GetControlPoints(22).position = 0.76
    ContourAtts.defaultPalette.GetControlPoints(23).colors = (150, 84, 0, 255)
    ContourAtts.defaultPalette.GetControlPoints(23).position = 0.79
    ContourAtts.defaultPalette.GetControlPoints(24).colors = (160, 0, 79, 255)
    ContourAtts.defaultPalette.GetControlPoints(24).position = 0.83
    ContourAtts.defaultPalette.GetControlPoints(25).colors = (255, 104, 28, 255)
    ContourAtts.defaultPalette.GetControlPoints(25).position = 0.86
    ContourAtts.defaultPalette.GetControlPoints(26).colors = (0, 170, 81, 255)
    ContourAtts.defaultPalette.GetControlPoints(26).position = 0.9
    ContourAtts.defaultPalette.GetControlPoints(27).colors = (68, 255, 124, 255)
    ContourAtts.defaultPalette.GetControlPoints(27).position = 0.93
    ContourAtts.defaultPalette.GetControlPoints(28).colors = (0, 130, 255, 255)
    ContourAtts.defaultPalette.GetControlPoints(28).position = 0.97
    ContourAtts.defaultPalette.GetControlPoints(29).colors = (130, 0, 255, 255)
    ContourAtts.defaultPalette.GetControlPoints(29).position = 1
    ContourAtts.defaultPalette.smoothingFlag = 0
    ContourAtts.defaultPalette.equalSpacingFlag = 1
    ContourAtts.defaultPalette.discreteFlag = 1
    ContourAtts.defaultPalette.externalFlag = 0
    ContourAtts.changedColors = (0, 1)
    ContourAtts.colorType = ContourAtts.ColorByMultipleColors  # ColorBySingleColor, ColorByMultipleColors, ColorByColorTable
    ContourAtts.colorTableName = "Default"
    ContourAtts.invertColorTable = 0
    ContourAtts.legendFlag = 1
    ContourAtts.lineStyle = ContourAtts.SOLID  # SOLID, DASH, DOT, DOTDASH
    ContourAtts.lineWidth = 0
    ContourAtts.singleColor = (255, 0, 0, 255)
    ContourAtts.SetMultiColor(0, (0, 0, 255, 255))
    ContourAtts.SetMultiColor(1, (0, 255, 0, 129))
    ContourAtts.SetMultiColor(2, (0, 0, 255, 255))
    ContourAtts.SetMultiColor(3, (0, 255, 255, 255))
    ContourAtts.SetMultiColor(4, (255, 0, 255, 255))
    ContourAtts.SetMultiColor(5, (255, 255, 0, 255))
    ContourAtts.SetMultiColor(6, (255, 135, 0, 255))
    ContourAtts.SetMultiColor(7, (255, 0, 135, 255))
    ContourAtts.SetMultiColor(8, (168, 168, 168, 255))
    ContourAtts.SetMultiColor(9, (255, 68, 68, 255))
    ContourAtts.minFlag = 0
    ContourAtts.maxFlag = 0
    ContourAtts.min = 0
    ContourAtts.max = 1
    ContourAtts.scaling = ContourAtts.Linear  # Linear, Log
    ContourAtts.wireframe = 0

    SetPlotOptions(ContourAtts)

def rotateImageXY():
    View3DAtts = GetView3D()
    vNorm = View3DAtts.viewNormal
    x = vNorm[0]
    y = vNorm[1]
    z = vNorm[2]

    # Find current view angle in XY plane
    Rxy = math.sqrt(1 - (z*z))
    thetaXY = math.atan(y/x)
    thetaXY = thetaXY + 0.1
    xp = Rxy*math.cos(thetaXY)
    yp = Rxy*math.sin(thetaXY)
    zp = z

    vNorm = (xp,yp,zp)
    View3DAtts.viewNormal = vNorm
    SetView3D(View3DAtts)
    DrawPlots()

def rotateImageXZ():
    View3DAtts = GetView3D()
    vNorm = View3DAtts.viewNormal
    x = vNorm[0]
    y = vNorm[1]
    z = vNorm[2]

    # Find current view angle in XY plane
    Rxz = math.sqrt(1 - (y*y))
    thetaXZ = math.atan(z/x)
    thetaXZ = thetaXZ + 0.1
    xp = Rxz*math.cos(thetaXZ)
    yp = y
    zp = Rxz*math.sin(thetaXZ)

    vNorm = (xp,yp,zp)
    View3DAtts.viewNormal = vNorm
    SetView3D(View3DAtts)
    DrawPlots()

def set3DView1():
    # Begin spontaneous state
    View3DAtts = View3DAttributes()
    View3DAtts.viewNormal = (-0.521132, -0.414823, 0.745884)
    View3DAtts.focus = (6.8736, 5.952, 6.4)
    View3DAtts.viewUp = (0.277648, -0.908799, -0.311442)
    View3DAtts.viewAngle = 30
    View3DAtts.parallelScale = 11.119
    View3DAtts.nearPlane = -22.238
    View3DAtts.farPlane = 22.238
    View3DAtts.imagePan = (0, 0)
    View3DAtts.imageZoom = 1
    View3DAtts.perspective = 1
    View3DAtts.eyeAngle = 2
    View3DAtts.centerOfRotationSet = 0
    View3DAtts.centerOfRotation = (6.8736, 5.952, 6.4)
    SetView3D(View3DAtts)
    # End spontaneous state

def set3DView2():

    # Begin spontaneous state
    View3DAtts = View3DAttributes()
    View3DAtts.viewNormal = (0.452223, 0.362173, 0.815061)
    View3DAtts.focus = (6.8736, 5.952, 6.4)
    View3DAtts.viewUp = (-0.171681, 0.932101, -0.318926)
    View3DAtts.viewAngle = 30
    View3DAtts.parallelScale = 11.119
    View3DAtts.nearPlane = -22.238
    View3DAtts.farPlane = 22.238
    View3DAtts.imagePan = (0, 0)
    View3DAtts.imageZoom = 1
    View3DAtts.perspective = 1
    View3DAtts.eyeAngle = 2
    View3DAtts.centerOfRotationSet = 0
    View3DAtts.centerOfRotation = (6.8736, 5.952, 6.4)
    SetView3D(View3DAtts)
    # End spontaneous state

# visPGoxy("siso-3d128-2g-2")
# for n in range(100):
#     rotateImageXZ()
#     print "view = ", n

######################################
# Auto commands
######################################

###################################################
# Command line option parse
numArgs = len(sys.argv) - 1
# print "args = ", sys.argv
# print "numArgs = ",numArgs

if (numArgs == 0):
    Usage( 0 )
elif (sys.argv[1] == '-h'):
    help()
    sys.exit(0)
else:
    print " "
    print "Running visit with -s option"
    print " "
###################################################

# topDir       = "/Users/swsides/data-SISO"
# dataBaseName = "diblock2s_totOxyDens_*.h5 database"
# hostName     = "localhost:"
# densTag      = "-OxyDens"

# topDir       = "/scr_oxygen/swsides"
# dataBaseName = "diblock2s_totIsoDens_*.h5 database"
# hostName     = "oxygen:"
# densTag      = "-IsoDens"

# topDir       = "/lustre/widow2/scratch/swsides"
# dataBaseName = "polyswift_totIsoDens_*.h5 database"
# hostName     = "lens0.ccs.ornl.gov:"
# densTag      = "-IsoDens"

# topDir       = "/lustre/widow2/scratch/swsides"
# dataBaseName = "polyswift_totOxyDens_*.h5 database"
# hostName     = "lens0.ccs.ornl.gov:"
# densTag      = "-OxyDens"

setDataNames()
# visPGoxyAll( sisoDirList[5], sisoDirName[5], "pseudo" )  # A1
# visPGoxyAll( sisoDirList[6], sisoDirName[6], "contour" ) # A2 T120
# visPGoxyAll( sisoDirList[7], sisoDirName[7], "contour" ) # A3
# visPGoxyAll( sisoDirList[8], sisoDirName[8], "contour" ) # A4 Asym2
# visPGoxyAll( sisoDirList[9], sisoDirName[9], "contour" ) # A5

topDir       = "/lustre/widow2/scratch/swsides"
dataBaseName = "polyswift_totIsoDens_*.h5 database"
hostName     = "lens0.ccs.ornl.gov:"
densTag      = "-IsoDens"

# PS-PI runs
# typeStr = "contour"
typeStr = "pseudo"
visPGoxyAll( sisoDirList[1],  sisoDirName[1],  typeStr )
visPGoxyAll( sisoDirList[2],  sisoDirName[2],  typeStr )
visPGoxyAll( sisoDirList[3],  sisoDirName[3],  typeStr )
visPGoxyAll( sisoDirList[4],  sisoDirName[4],  typeStr )
