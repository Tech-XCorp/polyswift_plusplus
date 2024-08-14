######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building psexamples/PSimPlus/nanoPtclWall.
#
# @version $Id$
#
# Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

######################################################################
#
# CMakeLists.txt for nanoPtclWall
#
# $Id: CMakeLists.txt 1208 2012-10-28 15:50:58Z cary $
#
######################################################################

TxResetExampleVars()

### Start developer modifications

# If conventions followed, none of the variables below need setting.
# set(RST_FILE_BASE dipoleLikeCharge)
# set(RST_AUTHOR "")
# set(RST_TITLE "Like-charge Dipole")

# Check for *.pre *.dat *.mac *.png *.py *.rst, but include only what it needed.
set(EXAMPLE_RUN_FILES
  nanoPtclWall.pre       # Pre file
  nanoPtclWall.png       # Main image
  nanoPtclWallTn.png     # Thumbnail
  nanoPtclWall.py
)

set(EXAMPLE_DOC_FILES
  nanoPtclWall.rst        # ReST description of example
  # Following are images used by the ReST file
  nanoPtclWallSetupWin.png
  nanoPtclWallRunWin.png
  nanoPtclWallVizWin-1.png
  nanoPtclWallVizWin-2.png
  nanoPtclWallVizWin-3.png
  nanoPtclWallVizWin-4.png
)

### End developer modifications

TxCheckSphinxVars()
TxCreateSphinxOutput(RST_FILE_BASE "${RST_FILE_BASE}"
  DOCFILES "${EXAMPLE_DOC_FILES}"
  INSTALL_SUPERDIR "${EXAMPLE_GROUP_INSTALL_SUBDIR}"
  CREATE_PDF
)
