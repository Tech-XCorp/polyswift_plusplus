######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building polyswift//pscomm.
#
# @version $Id: CMakeLists.txt 2147 2020-08-04 16:00:35Z smillie $
#
# Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

######################################################################
#
# CMakeLists.txt for pscomm
#
# $Id: CMakeLists.txt 2147 2020-08-04 16:00:35Z smillie $
#
######################################################################

set (PSCOMM_SOURCES
  PsCommMakerMap.cpp
  PsMpiComm.cpp PsCommHldr.cpp
)

set (PSCOMM_HEADERS
  PsCommMakerMap.h
  PsMpiComm.h PsCommHldr.h
)

include_directories (
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/psbase
  ${CMAKE_SOURCE_DIR}/psstd
  ${CMAKE_BINARY_DIR}
  ${TxBase_INCLUDE_DIRS}
  ${TxUtils_INCLUDE_DIRS}
)

if (HAVE_SECURITY)
  include_directories( ${TxLicMgr_INCLUDE_DIRS} ${OpenSsl_INCLUDE_DIRS})
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

set(pscomm_projlibs psbase psstd)

set(pscomm_extlibs
  ${TxUtils_txutils_LIBRARY}
)
TxAddSysCudaLicLibs(pscomm CUDA_LIBS curand cudart_static
  ADD_SECURITY_LIBS ADD_PYTHON_LIBS
)
list(APPEND pscomm_extlibs
  ${TxBase_LIBRARIES}
  ${Hdf5_LIBRARIES}
)

# Create the library
TxCreateProjLibrary(PsComm Polyswift)

