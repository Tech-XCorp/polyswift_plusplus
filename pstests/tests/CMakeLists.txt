#######################################################################
#
# CMakeLists.txt for pstests/tests
#
# $Id: CMakeLists.txt 417 2020-08-04 16:54:16Z smillie $
#
# Copyright &copy; 2020-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
#######################################################################

add_subdirectory(boundary)
add_subdirectory(diblock)
# Tests not triggered in autotools testing
#add_subdirectory(amrchompst)
#add_subdirectory(tsmc-litho)
