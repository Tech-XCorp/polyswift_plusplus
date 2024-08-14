######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building polyswift//pssolvent.
#
# @version $Id: CMakeLists.txt 2147 2020-08-04 16:00:35Z smillie $
#
# Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

######################################################################
#
# CMakeLists.txt for pssolvent
#
# $Id: CMakeLists.txt 2147 2020-08-04 16:00:35Z smillie $
#
######################################################################

set (PSSOLVENT_SOURCES
  PsSolventMakerMap.cpp
  PsSolventHldr.cpp
  PsSimpleSolvent.cpp
  PsSimpleIons.cpp
)

set (PSSOLVENT_HEADERS
  PsSolventMakerMap.h
  PsSolventHldr.h
  PsSimpleSolvent.h
  PsSimpleIons.h
)

include_directories (
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/psbase
  ${CMAKE_SOURCE_DIR}/psstd
  ${CMAKE_BINARY_DIR}
  ${TxBase_INCLUDE_DIRS}
  ${TxUtils_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
)

if (HAVE_SECURITY)
  include_directories( ${TxLicMgr_INCLUDE_DIRS} ${OpenSsl_INCLUDE_DIRS} )
endif ()

######################################################################
#
# Library
#
######################################################################

# Libs needed if built shared and for tests

set(pssolvent_projlibs psbase psstd)

set(pssolvent_extlibs
  ${TxUtils_txutils_LIBRARY}
)
TxAddSysCudaLicLibs(pssolvent CUDA_LIBS curand cudart_static
  ADD_SECURITY_LIBS ADD_PYTHON_LIBS
)
list(APPEND pssolvent_extlibs
  ${TxBase_LIBRARIES}
  ${Hdf5_LIBRARIES}
)

# Create the library
TxCreateProjLibrary(PsSolvent Polyswift)

