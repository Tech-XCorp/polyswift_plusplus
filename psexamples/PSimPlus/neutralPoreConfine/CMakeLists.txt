######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building psexamples/PSimPlus/neutralPoreConfine.
#
# @version $Id$
#
# Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

######################################################################
#
# CMakeLists.txt for neutralPoreConfine
#
# $Id: CMakeLists.txt 1208 2012-10-28 15:50:58Z cary $
#
######################################################################

TxResetExampleVars()

### Start developer modifications

# Check for *.pre *.dat *.mac *.png *.py *.rst, but include only what it needed.
set(EXAMPLE_RUN_FILES
  neutralPoreConfine.pre       # Pre file
  neutralPoreConfine.png       # Main image
  neutralPoreConfineTn.png     # Thumbnail
  neutralPoreConfine.py       # imported Python file
)

set(EXAMPLE_DOC_FILES
   neutralPoreConfine.rst        # ReST description of example
# Following are images used by the ReST file
 neutralPoreConfineSetupWin.png
neutralPoreConfineRunWin.png
 neutralPoreConfineVizWin.png
)

### End developer modifications

TxCheckSphinxVars()
TxCreateSphinxOutput(RST_FILE_BASE "${RST_FILE_BASE}"
  DOCFILES "${EXAMPLE_DOC_FILES}"
  INSTALL_SUPERDIR "${EXAMPLE_GROUP_INSTALL_SUBDIR}"
  CREATE_PDF
)
