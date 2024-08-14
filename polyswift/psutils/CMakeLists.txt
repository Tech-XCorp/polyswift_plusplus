######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building polyswift//psutils.
#
# @version $Id: CMakeLists.txt 2147 2020-08-04 16:00:35Z smillie $
#
# Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

######################################################################
#
# CMakeLists.txt for psutils
#
# $Id: CMakeLists.txt 2147 2020-08-04 16:00:35Z smillie $
#
######################################################################

# Variable holding shared data
set(DATA
)

# Variable holding executable scripts
set(SCRIPTS
)

# Installing data
if (DATA)
  install(FILES
    ${DATA}
    DESTINATION share
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ ${SCI_GROUP_WRITE}
  )
endif ()

# Installing executable scripts
if (DATA)
  install(PROGRAMS
    ${SCRIPTS}
    DESTINATION bin
    PERMISSIONS OWNER_READ OWNER_EXECUTE OWNER_WRITE
                GROUP_READ GROUP_EXECUTE ${SCI_GROUP_WRITE}
  )
endif ()

# Link executable scripts to build dir
# Does not work on windows, natch.
if (NOT WIN32)
add_custom_target(psutils-scripts-stamp ALL
  COMMAND ${CMAKE_SOURCE_DIR}/scimake/mklinks.sh psutils-scripts-stamp
    ${CMAKE_CURRENT_SOURCE_DIR} ${SCRIPTS} ${DATA}
)
endif ()

add_subdirectory(macros)
add_subdirectory(python)
add_subdirectory(scripts)
