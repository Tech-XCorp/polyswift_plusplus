######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building polyswift//pseffhamil.
#
# @version $Id: CMakeLists.txt 2147 2020-08-04 16:00:35Z smillie $
#
# Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

######################################################################
#
# CMakeLists.txt for pseffhamil
#
# $Id: CMakeLists.txt 2147 2020-08-04 16:00:35Z smillie $
#
######################################################################

set (PSEFFHAMIL_SOURCES
  PsInteractionMakerMap.cpp
  PsEffHamilMakerMap.cpp
  PsUpdaterMakerMap.cpp
  PsConstraintUpdater.cpp
  PsEffHamilHldr.cpp PsSteepDUpdater.cpp PsPoissonUpdater.cpp
  PsPolymerUpdater.cpp PsSpecFilterUpdater.cpp PsMultiSpecFilter.cpp
  PsSimpleSpecFilter.cpp PsFloryInteraction.cpp PsFloryWallInteraction.cpp
  PsEffHamil.cpp PsCanonicalMF.cpp
)

set (PSEFFHAMIL_HEADERS
  PsEffHamilMakerMap.h
  PsConstraintUpdater.h
  PsUpdaterMakerMap.h
  PsEffHamilHldr.h PsEffHamil.h PsFloryInteraction.h
  PsFloryWallInteraction.h
  PsSteepDUpdater.h PsPoissonUpdater.h
  PsPolymerUpdater.h
  PsSpecFilterUpdater.h
  PsMultiSpecFilter.h PsSimpleSpecFilter.h PsCanonicalMF.h
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

set(pseffhamil_projlibs psbase psstd)

set(pseffhamil_extlibs
  ${TxUtils_txutils_LIBRARY}
)
TxAddSysCudaLicLibs(pseffhamil CUDA_LIBS curand cudart_static
  ADD_SECURITY_LIBS ADD_PYTHON_LIBS
)
list(APPEND pseffhamil_extlibs
  ${TxBase_LIBRARIES}
  ${Hdf5_LIBRARIES}
)

# Create the library
TxCreateProjLibrary(PsEffhamil Polyswift)

