######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building psexamples/PSimPlus/polyDisperse.
#
# @version $Id$
#
# Copyright &copy; 2013-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

######################################################################
#
# CMakeLists.txt for polyDisperse
#
# $Id: CMakeLists.txt 1208 2012-10-28 15:50:58Z cary $
#
######################################################################

TxResetExampleVars()

### Start developer modifications

# Check for *.pre *.dat *.mac *.png *.py *.rst, but include only what it needed.
set(EXAMPLE_RUN_FILES
  polyDisperse.pre       # Pre file
  polyDisperse.png       # Main image
  polyDisperseTn.png     # Thumbnail
  polyDisperse.py        # imported Python file
)

set(EXAMPLE_DOC_FILES
 polyDisperse.rst        # ReST description of example
 # Following are images used by the ReST file
 polyDisperseSetupWin.png
 polyDisperseRunWin.png
 polyDisperseVizWin.png
)

### End developer modifications

TxCheckSphinxVars()
TxCreateSphinxOutput(RST_FILE_BASE "${RST_FILE_BASE}"
  DOCFILES "${EXAMPLE_DOC_FILES}"
  INSTALL_SUPERDIR "${EXAMPLE_GROUP_INSTALL_SUBDIR}"
  CREATE_PDF
)
