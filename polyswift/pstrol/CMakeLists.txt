######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building polyswift//pstrol.
#
# @version $Id: CMakeLists.txt 2147 2020-08-04 16:00:35Z smillie $
#
# Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

######################################################################
#
# CMakeLists.txt for pstrol
#
# $Id: CMakeLists.txt 2147 2020-08-04 16:00:35Z smillie $
#
######################################################################

set (PSTROL_SOURCES
  PsDomainMakerMap.cpp
  PolyswiftCmdLineArgs.cpp
  PsDomMakerMaps.cpp
  PsDomain.cpp
)

set (PSTROL_HEADERS
  PsDomainMakerMap.h
  PolyswiftCmdLineArgs.h
  PsDomMakerMaps.h
  PsDomain.h
  PsDomainBase.h
)

include_directories (
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/psbndry
  ${CMAKE_SOURCE_DIR}/pscomm
  ${CMAKE_SOURCE_DIR}/psdecomp
  ${CMAKE_SOURCE_DIR}/pseffhamil
  ${CMAKE_SOURCE_DIR}/psfft
  ${CMAKE_SOURCE_DIR}/psgrid
  ${CMAKE_SOURCE_DIR}/pshist
  ${CMAKE_SOURCE_DIR}/psphysf
  ${CMAKE_SOURCE_DIR}/pspolymer
  ${CMAKE_SOURCE_DIR}/pssolvent
  ${CMAKE_SOURCE_DIR}/psbase
  ${CMAKE_SOURCE_DIR}/psstd
  ${CMAKE_BINARY_DIR}
  ${TxBase_INCLUDE_DIRS}
  ${TxUtils_INCLUDE_DIRS}
  ${Hdf5_INCLUDE_DIRS}
  ${Fftw_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
)

if (HAVE_SECURITY)
   include_directories( ${TxLicMgr_INCLUDE_DIRS}  ${OpenSsl_INCLUDE_DIRS} )
endif ()

if (ENABLE_PARALLEL AND NOT SCI_HAVE_MPICXX_COMPILER_WRAPPER)
  include_directories("${MPI_INCLUDE_DIRS}")
endif ()

######################################################################
#
# Library
#
######################################################################

# Libs needed if built shared and for tests

set(pstrol_projlibs
  psbndry pscomm psdecomp pseffhamil
  psfft psgrid psphysf pspolymer pssolvent
  psbase psstd
)

set(pstrol_extlibs
  ${TxUtils_txutils_LIBRARY}
)
TxAddSysCudaLicLibs(pstrol CUDA_LIBS curand cudart_static
  ADD_SECURITY_LIBS ADD_PYTHON_LIBS
)
list(APPEND pstrol_extlibs
  ${TxBase_LIBRARIES}
  ${Hdf5_LIBRARIES}
)

# Create the library
TxCreateProjLibrary(PsTrol Polyswift)

