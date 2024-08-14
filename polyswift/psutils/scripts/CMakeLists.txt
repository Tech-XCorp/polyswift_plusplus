######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building polyswift//psutils/scripts.
#
# @version $Id$
#
# Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

####################################################################
#
# CMakeLists.txt for scripts
#
# $Id: CMakeLists.txt 20516 2012-11-15 20:38:19Z alexanda $
#
######################################################################

# Variable holding shared data
set(DATA
)

# Variable holding executable scripts
set(EXSCRIPTS
        rmExtras.sh cleanps.sh
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
if (EXSCRIPTS)
  install(PROGRAMS
    ${EXSCRIPTS}
    DESTINATION bin
    PERMISSIONS OWNER_READ OWNER_EXECUTE OWNER_WRITE
                GROUP_READ GROUP_EXECUTE ${SCI_GROUP_WRITE}
  )
endif ()
