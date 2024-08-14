######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building psexamples/PSimPlus/chompstAMR.
#
# @version $Id$
#
# Copyright &copy; 2013-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

######################################################################
#
# CMakeLists.txt for chompstAMR
#
# $Id: CMakeLists.txt 1208 2012-10-28 15:50:58Z cary $
#
######################################################################

TxResetExampleVars()

### Start developer modifications

# Check for *.pre *.dat *.mac *.png *.py *.rst, but include only what it needed.
set(EXAMPLE_RUN_FILES
  chompstAMR.pre       # Pre file
  chompstAMR.png       # Main image
  chompstAMRTn.png     # Thumbnail
)

set(EXAMPLE_DOC_FILES
 chompstAMR.rst        # ReST description of example
 # Following are images used by the ReST file
 chompstAMRSetupWin.png
 chompstAMRRunWin.png
 chompstAMRVizWin.png
)

### End developer modifications

TxCheckSphinxVars()
TxCreateSphinxOutput(RST_FILE_BASE "${RST_FILE_BASE}"
  DOCFILES "${EXAMPLE_DOC_FILES}"
  INSTALL_SUPERDIR "${EXAMPLE_GROUP_INSTALL_SUBDIR}"
  CREATE_PDF
)
