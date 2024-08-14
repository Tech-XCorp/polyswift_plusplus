######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building polyswift//psbndry.
#
# @version $Id: CMakeLists.txt 2147 2020-08-04 16:00:35Z smillie $
#
# Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

######################################################################
#
# CMakeLists.txt for psbndry
#
# $Id: CMakeLists.txt 2147 2020-08-04 16:00:35Z smillie $
#
######################################################################

set (PSBNDRY_SOURCES
  PsBndryMakerMap.cpp
  PsBndryDataBaseMakerMap.cpp
  PsBndryData.cpp
  PsSphereData.cpp PsWallData.cpp PsNanoPtcl.cpp
  PsSphere.cpp
  PsProbeSphere.cpp
  PsInteractingSphere.cpp PsBndryHldr.cpp
  PsFixedWall.cpp
)

set (PSBNDRY_HEADERS
  PsBndryMakerMap.h
  PsBndryDataBaseMakerMap.h
  PsBndryData.h PsSphereData.h PsWallData.h PsNanoPtcl.h
  PsProbeSphere.h
  PsSphere.h
  PsInteractingSphere.h
  PsBndryHldr.h PsFixedWall.h
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
  include_directories( ${TxLicMgr_INCLUDE_DIRS} ${OpenSsl_INCLUDE_DIRS})
endif ()

######################################################################
#
# Library
#
######################################################################

# Libs needed if built shared and for tests

set(psbndry_projlibs psbase psstd)

set(psbndry_extlibs
  ${TxUtils_txutils_LIBRARY}
)
TxAddSysCudaLicLibs(psbndry CUDA_LIBS curand cudart_static
  ADD_SECURITY_LIBS ADD_PYTHON_LIBS
)
list(APPEND psbndry_extlibs
  ${TxBase_LIBRARIES}
  ${Hdf5_LIBRARIES}
)

# Create the library
TxCreateProjLibrary(PsBndry Polyswift)

