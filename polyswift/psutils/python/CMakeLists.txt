######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building polyswift//psutils/python.
#
# @version $Id$
#
# Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

####################################################################
#
# CMakeLists.txt for python
#
# $Id: CMakeLists.txt 20516 2012-11-15 20:38:19Z alexanda $
#
######################################################################

# Variable holding shared data
set(DATA
)

# Variable holding executable scripts
set(EXSCRIPTS
        geometry.py  ps-run.py  geometry.py  polyswift.py  pspp-gen.py  psVis.py
)

# Variable holding python scripts
set(PYSCRIPTS
        psDumpHistory.py  psPlotHistory.py psPlotHistoryRPA.py
)

#
# Install only python files
# This gets txutils .py files installed in same place as
# as psutils python files. Needed so txppBase.py gets installed
#
set(TxUtils_PYSCRIPTS)
foreach (fl ${TxUtils_FILES})
  if (${fl} MATCHES "\\.py$")
    set(TxUtils_PYSCRIPTS ${TxUtils_PYSCRIPTS} ${fl})
  endif ()
endforeach ()
set(PYSCRIPTS ${PYSCRIPTS} ${TxUtils_PYSCRIPTS} )

# Substitute paths in conf.py.in file (this installs in build directory)
CONFIGURE_FILE(ps-run.py.in    ${CMAKE_CURRENT_SOURCE_DIR}/ps-run.py   @ONLY)
CONFIGURE_FILE(psVis.py.in       ${CMAKE_CURRENT_SOURCE_DIR}/psVis.py     @ONLY)

# Installing data
if (DATA)
  install(FILES
    ${DATA}
    DESTINATION share
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ ${SCI_GROUP_WRITE}
  )
endif ()

# Installing executable scripts
if (EXSCRIPTS)
  install(PROGRAMS
    ${EXSCRIPTS}
    DESTINATION bin
    PERMISSIONS OWNER_READ OWNER_EXECUTE OWNER_WRITE
                GROUP_READ GROUP_EXECUTE ${SCI_GROUP_WRITE}
  )
endif ()

# Installing python scripts
if (PYSCRIPTS)
  install(FILES
    ${PYSCRIPTS}
    DESTINATION share/scripts
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ ${SCI_GROUP_WRITE}
  )
endif ()

# Link executable scripts to build dir
# Does not work on windows, natch.
if (NOT WIN32)
add_custom_target(psutils-python-scripts-stamp ALL
  COMMAND ${CMAKE_SOURCE_DIR}/scimake/mklinks.sh psutils-python-scripts-stamp
    ${CMAKE_CURRENT_SOURCE_DIR} ${SCRIPTS} ${DATA}
)
endif ()
