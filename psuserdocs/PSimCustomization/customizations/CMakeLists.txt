######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building psuserdocs/PSimCustomization/customizations.
#
# @version $Id$
#
# Copyright &copy; 2018-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

######################################################################
#
# CMakeLists.txt for sphinx docs
#
# $Id: CMakeLists.txt 1767 2017-07-07 16:06:06Z cary $
#
######################################################################

# add_subdirectory(templates)

file(GLOB thesefiles *.jpg *.ico *.png)
message(STATUS "Copying files in customizations" ${thesefiles})
file(COPY ${thesefiles} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")
message(STATUS "Copying " ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/templates DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "Done copying files in customizations")

