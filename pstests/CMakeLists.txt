#######################################################################
#
# CMakeLists.txt for pstests
#
# $Id: CMakeLists.txt 424 2021-12-06 20:48:28Z alexanda $
#
# Copyright &copy; 2016-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
#######################################################################

# Required version:  3.18 needed for CMAKE_CUDA_ARCHITECTURES
cmake_minimum_required(VERSION 3.18)

# Set policies
include(${CMAKE_SOURCE_DIR}/scimake/SciPolicies.cmake NO_POLICY_SCOPE)

# Project information
project(pstests) # Need compilers to find mpi

######################################################################
#
# Usual initialization stuff
#
######################################################################

# Must set OpenMP before SciInit to ensure correct compiler flags
include(${CMAKE_SOURCE_DIR}/scimake/SciFuncsMacros.cmake)
# option(USE_OPENMP "Whether to use OpenMP if found" FALSE)

set(CHECK_OMP_SSE_AVX FALSE)
#set(ENABLE_PARALLEL TRUE)
set(RESULTS_DIR_BASE psresults)
include(${CMAKE_SOURCE_DIR}/scimake/SciInit.cmake)
set(PSTESTS_HOSTNAME ${HOSTNAME})
set(PSTESTS_HOSTTYPE ${HOSTTYPE})
set(PSTESTS_REV "${PROJECT_REV}")
set(PS_SCIMAKE_REV "${SCIMAKE_REV}")
message(STATUS "PSTESTS_SCIMAKE_REV = ${PSTESTS_SCIMAKE_REV}.")
if (RESULTS_DIR)
  set(PS_RESULTS_DIR ${RESULTS_DIR})
  message(STATUS "PS_RESULTS_DIR = ${PS_RESULTS_DIR}.")
else ()
  message(FATAL_ERROR "psresults not found.")
endif ()

# Handling slower tests
set(SCI_TEST_TIMEOUT 900)

# option cannot be used as not a bool
message("")
include(${PROJECT_SOURCE_DIR}/txcmake/TxInit.cmake)

# For running PsTests
include(${PROJECT_SOURCE_DIR}/txcmake/TxEngTestMacros.cmake)

######################################################################
#
# Find packages
#
######################################################################

# Find python libraries if enabled, fix up for various compilers
find_package(SciPython)
find_package(SciMpi)
set(MAX_MPI_PROCS 8)

# For GPU tests
find_package(SciCuda)

# Z is not a dependency on Windows
if (NOT WIN32)
  find_package(SciZ)
endif ()

# Find packages
find_package(SciHdf5 ${PSDEPS_STATE})
set(H5_USE_16_API 1)

# Use source dir for txutils if possible
if (FALSE)
if (TxUtils_SOURCE_DIR)
  get_filename_component(TxUtils_SOURCE_DIR "${SOURCE_DIR}" REALPATH)
else ()
  get_filename_component(TxUtils_SOURCE_DIR "${CMAKE_SOURCE_DIR}/../txutils" REALPATH)
endif ()
if (EXISTS ${TxUtils_SOURCE_DIR})
  set(TxUtils_ROOT_DIR ${TxUtils_SOURCE_DIR})
  # message(FATAL_ERROR "${TxUtils_SOURCE_DIR} does not exist.")
endif ()
endif ()
#set(TxUtils_FINDLIBS FALSE)
find_package(TxUtils REQUIRED)

######################################################################
#
# Find polyswift
#
######################################################################

message("")
message(STATUS "--------- Seeking polyswift -----------")

get_filename_component(PolyswiftAll_BUILD_DIR "${CMAKE_BINARY_DIR}/../.." REALPATH)
SciPrintVar(PolyswiftAll_BUILD_DIR)
if (SOURCE_DIR)
  get_filename_component(Polyswift_SOURCE_DIR "${SOURCE_DIR}" REALPATH)
else ()
  get_filename_component(Polyswift_SOURCE_DIR "${CMAKE_SOURCE_DIR}/../polyswift" REALPATH)
endif ()
get_filename_component(PolyswiftAll_PROJECT_DIR "${Polyswift_SOURCE_DIR}/.." REALPATH)

if (WIN32)
  string(REGEX REPLACE "^C:/cygdrive/c" "C:" Polyswift_SOURCE_DIR "${Polyswift_SOURCE_DIR}")
  string(REGEX REPLACE "^D:/cygdrive/d" "D:" Polyswift_SOURCE_DIR "${Polyswift_SOURCE_DIR}")
endif ()
set(Polyswift_MACROS_DIR ${Polyswift_SOURCE_DIR}/psutils/macros)
set(Polyswift_PSPYTHON_DIR ${Polyswift_SOURCE_DIR}/psutils/python)
set(Polyswift_PSDATA_DIR ${Polyswift_SOURCE_DIR}/psutils/data)
SciPrintVar(PolyswiftAll_PROJECT_DIR)
SciPrintVar(Polyswift_SOURCE_DIR)
SciPrintVar(Polyswift_MACROS_DIR)
SciPrintVar(Polyswift_PSPYTHON_DIR)
SciPrintVar(Polyswift_PSDATA_DIR)

if (SERIAL_DIR)
  get_filename_component(PS_SERIAL_DIR "${SERIAL_DIR}" REALPATH)
else ()
# Need a glob here for builds-v, builds-i.
  get_filename_component(PS_SERIAL_DIR "${CMAKE_BINARY_DIR}/../../polyswift/ser" REALPATH)
endif ()
set(Polyswift_SEREXE polyswiftser)
if (WIN32)
  string(REGEX REPLACE "^C:/cygdrive/c" "C:" PS_SERIAL_DIR "${PS_SERIAL_DIR}")
  string(REGEX REPLACE "^D:/cygdrive/d" "D:" PS_SERIAL_DIR "${PS_SERIAL_DIR}")
  set(Polyswift_SEREXE ${Polyswift_SEREXE}.exe)
endif ()
# message(STATUS "Seeking ${PS_SERIAL_DIR}/polyswift/polyswiftser.")
if (EXISTS ${PS_SERIAL_DIR}/polyswift/${Polyswift_SEREXE})
  set(Polyswift_POLYSWIFTSER ${PS_SERIAL_DIR}/polyswift/${Polyswift_SEREXE})
  # message(STATUS "Polyswift_POLYSWIFTSER = ${Polyswift_POLYSWIFTSER}.")
else ()
  message(FATAL_ERROR "Cannot find serial executable ${Polyswift_SEREXE}.")
endif ()
SciPrintVar(Polyswift_POLYSWIFTSER)

if (PARALLEL_DIR)
  get_filename_component(PS_PARALLEL_DIR "${PARALLEL_DIR}" REALPATH)
else ()
  get_filename_component(PS_PARALLEL_DIR "${CMAKE_BINARY_DIR}/../../polyswift/ser" REALPATH)
endif ()
set(Polyswift_PAREXE polyswift)
if (WIN32)
  string(REGEX REPLACE "^C:/cygdrive/c" "C:" PS_PARALLEL_DIR "${PS_PARALLEL_DIR}")
  string(REGEX REPLACE "^D:/cygdrive/d" "D:" PS_PARALLEL_DIR "${PS_PARALLEL_DIR}")
  set(Polyswift_PAREXE ${Polyswift_PAREXE}.exe)
endif ()
# message(STATUS "Seeking ${PS_PARALLEL_DIR}/polyswift/polyswift.")
if (EXISTS ${PS_PARALLEL_DIR}/polyswift/${Polyswift_PAREXE})
  set(Polyswift_POLYSWIFT ${PS_PARALLEL_DIR}/polyswift/${Polyswift_PAREXE})
  # message(STATUS "Polyswift_POLYSWIFT = ${Polyswift_POLYSWIFT}.")
else ()
  message(FATAL_ERROR "Cannot find parallel executable ${Polyswift_PAREXE}.")
endif ()
SciPrintVar(Polyswift_POLYSWIFT)

######################################################################
#
# Paths
#
######################################################################

message("")
message(STATUS "--------- Setting paths -----------")
# PYTHONPATH and TXPP_PATH use system path separator.
SciPrintVar(TXPP_PATH)
if (WIN32)
  set(TXPP_PATH "${Polyswift_MACROS_DIR};${TxUtils_MACROS_DIR}")
  set(PsTests_PYTHONPATH "$ENV{PYTHONPATH};${TxUtils_SHARE_DIR};${Polyswift_PSPYTHON_DIR}")
  STRING(REPLACE "/cygdrive/c/" "/" TXPP_PATH "${TXPP_PATH}")
  STRING(REPLACE "/cygdrive/d/" "/" TXPP_PATH "${TXPP_PATH}")
  STRING(REPLACE "/" "\\" TXPP_PATH "${TXPP_PATH}")
  STRING(REPLACE "\\" "\\\\\\\\" TXPP_PATH "${TXPP_PATH}")
  STRING(REPLACE ";" "\\;" TXPP_PATH "${TXPP_PATH}")
  STRING(REPLACE "/" "\\" PsTests_PYTHONPATH "${PsTests_PYTHONPATH}")
  STRING(REPLACE "\\" "\\\\\\\\" PsTests_PYTHONPATH "${PsTests_PYTHONPATH}")
  STRING(REPLACE ";" "\\;" PsTests_PYTHONPATH "${PsTests_PYTHONPATH}")
else ()
  set(TXPP_PATH "${TxUtils_MACROS_DIR}:${Polyswift_MACROS_DIR}")
  set(PsTests_PYTHONPATH "${Polyswift_PSPYTHON_DIR}:${TxUtils_SHARE_DIR}:$ENV{PYTHONPATH}")
endif ()

SciPrintVar(TXPP_PATH)
SciPrintVar(PsTests_PYTHONPATH)
SciPrintVar(Polyswift_PSDATA_DIR)

######################################################################
#
# VpExamples libraries
#
######################################################################

message("")
message(STATUS "--------- Seeking psexamples -----------")
if (NOT PSEXAMPLES_DIR)
  if (EXISTS ${CMAKE_SOURCE_DIR}/../psexamples)
    get_filename_component(PSEXAMPLES_DIR ${CMAKE_SOURCE_DIR}/../psexamples ABSOLUTE)
  else ()
    message(FATAL_ERROR "Cannot find PSEXAMPLES_DIR.")
  endif ()
else ()
  get_filename_component(PSEXAMPLES_DIR ${PSEXAMPLES_DIR} ABSOLUTE)
endif ()

set(pathSeparator ":")
if (WIN32)
  set(pathSeparator ";")
  string(REPLACE "/cygdrive/c/" "/" PSEXAMPLES_DIR ${PSEXAMPLES_DIR})
  string(REPLACE "/cygdrive/d/" "/" PSEXAMPLES_DIR ${PSEXAMPLES_DIR})
#  string(REPLACE "/" "\\" PSEXAMPLES_DIR ${PSEXAMPLES_DIR})
endif ()

message(STATUS "PSEXAMPLES_DIR = ${PSEXAMPLES_DIR}.")

######################################################################
#
# Set permissions before adding subdirectories
#
######################################################################

set(SCI_GROUP_WRITE)
set(SCI_WORLD_FILE_PERMS)
set(SCI_WORLD_PROGRAM_PERMS)

######################################################################
#
# Enable ctest
#
######################################################################

# CTest tests
set(NUM_MPI_PROCS 8)
set(CTEST_PARALLEL_LEVEL ${NUM_MPI_PROCS})
include(utils/CTestConfig.cmake)
include(CTest)  # Must be added at top level, apparently
enable_testing()

######################################################################
#
# Setting Common PsTest Values
#
######################################################################

set(Polyswift_INSFX pre)
set(Polyswift_INARG -i)
set(Polyswift_OUTARG -o)
set(Polyswift_DIFFER "${Python_EXE}")
set(Polyswift_DIFFER_ARGS "${TxUtils_txh5diff_py}" -m ${CMAKE_SOURCE_DIR}/utils/ignorenames.txt -d 1e-12)
set(Polyswift_RESULT_REGEX
     "Failed: Datasets or attributes missing"
     "Failed: Different lengths"
     "Failed: Differing strings"
     "Failed: Different number of dimenions"
     "Failed: NaN in datasets"
     "Failed: Differing dimension extents"
     "are not acceptable"
     "Recommend accepting new results"
) # Listed as in DIFFER output order
set(Polyswift_IGNORE_FILES
  ".cmake$"
  "^Makefile$"
  ".txt$"
  ".h5.backup$"
  ".stl$"
  ".mm$"
  ".mtx$"
  ".tmp$" #In case builds crash or cancel while logging
)
set(Polyswift_RESULT_FILE_NAME "TxAnnotatedFailures.log")
set(Polyswift_SER_CMD "${Polyswift_POLYSWIFTSER}")
set(Polyswift_PAR_CMD "${Polyswift_POLYSWIFT}")
set(Polyswift_PYTHONPATH "${PsTests_PYTHONPATH}")
set(Polyswift_TXPP_PATH "${TXPP_PATH}")
set(Polyswift_SET_PATHS
  TO_SET TXPP_PATH PYTHONPATH SIM_DATA_PATH
  TO_USE Polyswift_TXPP_PATH PsTests_PYTHONPATH Polyswift_PSDATA_DIR
)

######################################################################
#
# Add subdirectories
#
######################################################################

add_subdirectory(tests)

######################################################################
#
# Configure CTest Custom Settings
#
######################################################################

configure_file(utils/CTestCustom.ctest.in CTestCustom.ctest @ONLY)

######################################################################
#
# Configure files
#
######################################################################

configure_file(utils/ReplaceAllRecommended.sh.in ReplaceAllRecommended.sh @ONLY)

######################################################################
#
# Finalize (installation, packaging)
#
######################################################################

# Set before final installation and packaging
set(COMMON_INSTALL TRUE)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "PsTests: Polyswift Test Framework")
set(CONFIG_FILES)  # Clear out

# Finish up
set(INSTALL_CONFIG_HEADERS FALSE)
set(GENERATE_AUTOTOOLS FALSE)
include(${CMAKE_SOURCE_DIR}/scimake/SciFinalize.cmake)
