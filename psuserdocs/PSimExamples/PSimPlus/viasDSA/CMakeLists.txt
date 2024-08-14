######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building psexamples/PSimPlus/viasDSA.
#
# @version $Id$
#
# Copyright &copy; 2017-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

######################################################################
#
# CMakeLists.txt for viasDSA
#
# $Id: CMakeLists.txt 1208 2012-10-28 15:50:58Z swsides $
#
######################################################################

TxResetExampleVars()

### Start developer modifications

# Check for *.pre *.dat *.mac *.png *.py *.rst, but include only what it needed.
set(EXAMPLE_RUN_FILES
  viasDSA.pre              # Pre file
  viasDSA.png              # Main image
  viasDSATn.png            # Thumbnail
  viasDSA.py               # imported Python file
  viasDSA_filteredMask.dat # Processed mask file (running smoothingForDSA.py on VIAS file)
  editGeometry.py          # Script for pre-processing
  smoothingForDSA.py       # Script for pre-processing
)

set(EXAMPLE_DOC_FILES
  viasDSA.rst           # ReST description of example
  viasDSASetupWin.png   # Following are images used by the ReST file
  viasDSARunWin.png
  viasDSAVizWin.png
  viasDSA_ProbDescription1.png
)

### End developer modifications

TxCheckSphinxVars()

TxCreateSphinxOutput(RST_FILE_BASE "${RST_FILE_BASE}"
  DOCFILES "${EXAMPLE_DOC_FILES}"
  INSTALL_SUPERDIR "${EXAMPLE_GROUP_INSTALL_SUBDIR}"
  CREATE_PDF
)

