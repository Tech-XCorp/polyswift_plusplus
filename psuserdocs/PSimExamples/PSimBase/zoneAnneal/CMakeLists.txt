######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building psexamples/PSimBase/zoneAnneal.
#
# @version $Id$
#
# Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

######################################################################
#
# CMakeLists.txt for zoneAnneal
#
# $Id: CMakeLists.txt 1208 2012-10-28 15:50:58Z cary $
#
######################################################################

TxResetExampleVars()

### Start developer modifications

# Check for *.pre *.dat *.mac *.png *.py *.rst, but include only what it needed.
set(EXAMPLE_RUN_FILES
  zoneAnneal.pre       # Pre file
  zoneAnneal.png       # Main image
  zoneAnnealTn.png     # Thumbnail
)

set(EXAMPLE_DOC_FILES
   zoneAnneal.rst        # ReST description of example
   # Following are images used by the ReST file
   zoneAnnealSetupWin.png
   zoneAnnealRunWin.png
   zoneAnnealVizWin.png
)

### End developer modifications

TxCheckSphinxVars()
TxCreateSphinxOutput(RST_FILE_BASE "${RST_FILE_BASE}"
  DOCFILES "${EXAMPLE_DOC_FILES}"
  INSTALL_SUPERDIR "${EXAMPLE_GROUP_INSTALL_SUBDIR}"
  CREATE_PDF
)
