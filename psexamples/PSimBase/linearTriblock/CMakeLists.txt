######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building psexamples/PSimBase/linearTriblock.
#
# @version $Id$
#
# Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

######################################################################
#
# CMakeLists.txt for linearTriblock
#
# $Id: CMakeLists.txt 1208 2012-10-28 15:50:58Z cary $
#
######################################################################

TxResetExampleVars()

### Start developer modifications

# Check for *.pre *.dat *.mac *.png *.py *.rst, but include only what it needed.
set(EXAMPLE_RUN_FILES
  linearTriblock.pre       # Pre file
  linearTriblock.png       # Main image
  linearTriblockTn.png     # Thumbnail
)

set(EXAMPLE_DOC_FILES
   linearTriblock.rst        # ReST description of example
   # Following are images used by the ReST file
    linearTriblockSetupWin.png
    linearTriblockRunWin.png
    linearTriblockVizWin.png
)

### End developer modifications

TxCheckSphinxVars()
TxCreateSphinxOutput(RST_FILE_BASE "${RST_FILE_BASE}"
  DOCFILES "${EXAMPLE_DOC_FILES}"
  INSTALL_SUPERDIR "${EXAMPLE_GROUP_INSTALL_SUBDIR}"
  CREATE_PDF
)
