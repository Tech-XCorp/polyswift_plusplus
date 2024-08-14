######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building psexamples/PSimBase.
#
# @version $Id$
#
# Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

######################################################################
#
# CMakeLists.txt for PSimBase
#
# $Id: CMakeLists.txt 1460 2012-11-13 22:56:42Z jdelamere $
#
######################################################################

# Set example group variables
setExampleGroupVars()

### Begin developer changes

#
# Add subdirs (change this file only here)
#
add_subdirectory(linearDiblock)      # Scott Sides
# add_subdirectory(estimateRPA)       # Scott Sides
add_subdirectory(linearTriblock)     # Scott Sides
add_subdirectory(mixtureABwA)     # Scott Sides
add_subdirectory(starABC)              # Scott Sides
add_subdirectory(zoneAnneal)        # Scott Sides
add_subdirectory(spectralFilter)      # Scott Sides

### End developer changes

# Save the variables up one level
saveExampleGroupVars(PARENTIZE_SUBDIRS TRUE)
