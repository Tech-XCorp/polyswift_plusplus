######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building polyswift//pspolymer.
#
# @version $Id: CMakeLists.txt 2162 2021-03-13 19:17:51Z swsides $
#
# Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

######################################################################
#
# CMakeLists.txt for pspolymer
#
# $Id: CMakeLists.txt 2162 2021-03-13 19:17:51Z swsides $
#
######################################################################

set (PSPOLYMER_SOURCES
  PsGaussLagQuad.cpp
  PsBlockMakerMap.cpp
  PsPolymerMakerMap.cpp
  PsBlock.cpp
  PsBlockCopolymer.cpp
  PsPolyDisperseBCP.cpp
  PsChargeFlexPseudoSpec.cpp
  PsFlexPseudoSpec.cpp
  PsPolymerHldr.cpp
  PsSemiFlexibleBlock.cpp
)

set (PSPOLYMER_HEADERS
  PsGaussLagQuad.h
  PsBlockMakerMap.h
  PsPolymerMakerMap.h
  PsBlock.h
  PsBlockCopolymer.h
  PsPolyDisperseBCP.h
  PsChargeFlexPseudoSpec.h
  PsFlexPseudoSpec.h
  PsPolymerHldr.h
  PsSemiFlexibleBlock.h
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

set(pspolymer_projlibs psbase psstd)

set(pspolymer_extlibs
  ${TxUtils_txutils_LIBRARY}
)
TxAddSysCudaLicLibs(pspolymer CUDA_LIBS curand cudart_static
  ADD_SECURITY_LIBS ADD_PYTHON_LIBS
)
list(APPEND pspolymer_extlibs
  ${TxBase_LIBRARIES}
  ${Hdf5_LIBRARIES}
)

# Create the library
TxCreateProjLibrary(PsPolymer Polyswift)
