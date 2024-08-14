######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building psexamples/PSimBase/starABC.
#
# @version $Id$
#
# Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

TxResetExampleVars()

### Start developer modifications

# Check for *.pre *.dat *.mac *.png *.py *.rst, but include only what it needed.
set(EXAMPLE_RUN_FILES
  starABC.pre        # Pre file
  starABC.png       # Main image
  starABCTn.png   # Thumbnail
)

set(EXAMPLE_DOC_FILES
   starABC.rst        # ReST description of example
   # Following are images used by the ReST file
    starABCSetupWin.png
    starABCRunWin.png
    starABCVizWin.png
)

### End developer modifications

TxCheckSphinxVars()
TxCreateSphinxOutput(RST_FILE_BASE "${RST_FILE_BASE}"
  DOCFILES "${EXAMPLE_DOC_FILES}"
  INSTALL_SUPERDIR "${EXAMPLE_GROUP_INSTALL_SUBDIR}"
  CREATE_PDF
)
