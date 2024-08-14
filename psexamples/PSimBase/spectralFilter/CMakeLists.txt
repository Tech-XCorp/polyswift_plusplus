######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building psexamples/PSimBase/spectralFilter.
#
# @version $Id$
#
# Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

######################################################################
#
# CMakeLists.txt for spectralFilter
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
  spectralFilter.pre       # Pre file
  spectralFilter.png       # Main image
  spectralFilterTn.png     # Thumbnail
)

set(EXAMPLE_DOC_FILES
   spectralFilter.rst        # ReST description of example
# Following are images used by the ReST file
  spectralFilterSetupWin.png
 spectralFilterRunWin.png
  spectralFilterVizWin.png
  spectralFilterVizWin-1.png
)

### End developer modifications

TxCheckSphinxVars()
TxCreateSphinxOutput(RST_FILE_BASE "${RST_FILE_BASE}"
  DOCFILES "${EXAMPLE_DOC_FILES}"
  INSTALL_SUPERDIR "${EXAMPLE_GROUP_INSTALL_SUBDIR}"
  CREATE_PDF
)
