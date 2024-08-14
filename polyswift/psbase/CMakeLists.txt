######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building polyswift//psbase.
#
# @version $Id: CMakeLists.txt 2147 2020-08-04 16:00:35Z smillie $
#
# Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

######################################################################
#
# CMakeLists.txt for psbase
#
# $Id: CMakeLists.txt 2147 2020-08-04 16:00:35Z smillie $
#
######################################################################

set (PSBASE_SOURCES
  PsFFTBase.cpp
  PsFieldMakerMap.cpp
  PsBndryBase.cpp
  PsBndryDataBase.cpp
  PsCommBase.cpp
  PsDecompBase.cpp
  PsDomainSingletons.cpp
  PsDynObj.cpp
  PsDynObjBase.cpp
  PsEffHamilBase.cpp
  PsGridField.cpp
  PsBlockBase.cpp
  PsFieldBase.cpp
  PsFunctionalQ.cpp
  PsHistoryBase.cpp
  PsInteraction.cpp
  PsPhysField.cpp
  PsPolymer.cpp
  PsSolvent.cpp
  PsTensor.cpp
  PsUpdater.cpp
)

set (PSBASE_HEADERS
  PsFieldMakerMap.h
  PsBlockTypes.h
  PsBlockBase.h
  PsBndryBase.h
  PsBndryDataBase.h
  PsCommBase.h
  PsDecompBase.h
  PsDomainSingletons.h
  PsDynObj.h
  PsDynObjBase.h
  PsEffHamilBase.h
  PsFFTBase.h
  PsGridField.h
  PsFieldBase.h
  PsFunctionalQ.h
  PsGridBase.h
  PsGridBaseItr.h
  PsHistoryBase.h
  PsInteraction.h
  PsPhysField.h
  PsPolymer.h
  PsSolvent.h
  PsTensor.h
  PsUpdater.h
)

include_directories (
  ${CMAKE_CURRENT_SOURCE_DIR}
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

set(psbase_projlibs psstd)

set(psbase_extlibs
  ${TxUtils_txutils_LIBRARY}
)
TxAddSysCudaLicLibs(psbase CUDA_LIBS curand cudart_static
  ADD_SECURITY_LIBS ADD_PYTHON_LIBS
)
list(APPEND psbase_extlibs
  ${TxBase_LIBRARIES}
  ${Hdf5_LIBRARIES}
)

# Create the library
TxCreateProjLibrary(PsBase Polyswift)

