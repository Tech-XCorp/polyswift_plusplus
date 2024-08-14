######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building polyswift//psdecomp.
#
# @version $Id: CMakeLists.txt 2147 2020-08-04 16:00:35Z smillie $
#
# Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

######################################################################
#
# CMakeLists.txt for psdecomp
#
# $Id: CMakeLists.txt 2147 2020-08-04 16:00:35Z smillie $
#
######################################################################

set (PSDECOMP_SOURCES
  PsDecompMakerMap.cpp
  PsDecomp.cpp
  PsDecompRegular.cpp
  PsDecompFFTW.cpp
  PsDecompHldr.cpp
)

set (PSDECOMP_HEADERS
  PsDecompMakerMap.h
  PsDecomp.h PsDecompRegular.h
  PsDecompFFTW.h
  PsDecompHldr.h
)

include_directories (
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/psbase
  ${CMAKE_SOURCE_DIR}/psstd
  ${CMAKE_BINARY_DIR}
  ${TxBase_INCLUDE_DIRS}
  ${TxUtils_INCLUDE_DIRS}
  ${Fftw_INCLUDE_DIRS}
)

if (HAVE_SECURITY)
  include_directories( ${TxLicMgr_INCLUDE_DIRS} ${OpenSsl_INCLUDE_DIRS})
endif ()

if (ENABLE_PARALLEL AND NOT SCI_HAVE_MPICXX_COMPILER_WRAPPER)
  include_directories("${MPI_INCLUDE_DIRS}")
endif()

if (BOOST_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif ()

######################################################################
#
# Library
#
######################################################################

# Libs needed if built shared and for tests

set(psdecomp_projlibs psbase psstd)

set(psdecomp_extlibs
  ${TxUtils_txutils_LIBRARY}
)
TxAddSysCudaLicLibs(psdecomp CUDA_LIBS curand cudart_static
  ADD_SECURITY_LIBS ADD_PYTHON_LIBS
)
list(APPEND psdecompo_extlibs
  ${TxBase_LIBRARIES}
  ${Hdf5_LIBRARIES}
)

# Create the library
TxCreateProjLibrary(PsDecomp Polyswift)

