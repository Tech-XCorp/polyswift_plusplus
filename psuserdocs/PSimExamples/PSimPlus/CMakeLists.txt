######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building psexamples/PSimPlus.
#
# @version $Id$
#
# Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

######################################################################
#
# CMakeLists.txt for PSimPlus
#
# $Id: CMakeLists.txt 1460 2012-11-13 22:56:42Z swsides $
#
######################################################################

# Set example group variables
setExampleGroupVars()

message(STATUS "---------------------------------- INCLUDE_TSMC_EXAMPLES = ${INCLUDE_TSMC_EXAMPLES}")

if (INCLUDE_TSMC_EXAMPLES)
  set(PSIM_PLUS_EXAMPLES "interactingSurfaces,neutralPoreConfine,nanoPtclWall,viasDSA")
else ()
  set(PSIM_PLUS_EXAMPLES "interactingSurfaces,neutralPoreConfine,nanoPtclWall")
endif ()

message(STATUS "PSIM_PLUS_EXAMPLES = ${PSIM_PLUS_EXAMPLES}")

# Set the psim_plus example names
configure_file(description.ini.in ${CMAKE_CURRENT_SOURCE_DIR}/description.ini)

### ------------------ Begin developer changes --------------------------

#
# Add subdirs (change this file only here)
#
add_subdirectory(neutralPoreConfine)      # Scott Sides
add_subdirectory(interactingSurfaces)     # Scott Sides
add_subdirectory(nanoPtclWall)            # Scott Sides

# add_subdirectory(polyDisperse)          # disabling this as polydisperse example not developed
# add_subdirectory(chompstAMR)            # disabling this for now as chompst not supported

if (INCLUDE_TSMC_EXAMPLES)
   message(STATUS "----------------------------------")
   message(STATUS "     Will include TSMC examples   ")
   message(STATUS "----------------------------------")
   add_subdirectory(viasDSA)
endif ()

### ----------------- End developer changes ----------------------------

# Save the variables up one level
saveExampleGroupVars(PARENTIZE_SUBDIRS TRUE)
