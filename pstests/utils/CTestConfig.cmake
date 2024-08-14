######################################################################
#
# CTestConfig: Set variables for testing.
#
# $Id: CTestConfig.cmake 417 2020-08-04 16:54:16Z smillie $
#
# Copyright &copy; 2016-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

#
# enable_testing() and include(CTest) must be called after this is included.
#
# set(CTEST_DROP_SITE "cdash.txcorp.com")
# JRC: the above caused my submissions at home to fail
include(${PROJECT_SOURCE_DIR}/scimake/SciTestConfig.cmake)

