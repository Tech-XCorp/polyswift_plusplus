######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building polyswift.
#
# @version $Id: CMakeLists.txt 2173 2022-08-01 22:47:53Z alexanda $
#
# Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

######################################################################
#
# CMakeLists.txt for PolySwift++
#
# $Id: CMakeLists.txt 2173 2022-08-01 22:47:53Z alexanda $
#
######################################################################

# Set policies
 include(${CMAKE_SOURCE_DIR}/scimake/SciPolicies.cmake NO_POLICY_SCOPE)
 cmake_policy(SET CMP0105 OLD)
 cmake_policy(SET CMP0057 NEW)
# In CMake 3.17 and below, link options are not used by the device link step.
# The OLD behavior for this policy is to ignore the link options.
# The NEW behavior of this policy is to use the link options during the device link step.

# Required version: 3.15 needed for CMAKE_MSVC_RUNTIME_LIBRARY
cmake_minimum_required(VERSION 3.15)

# Set policies
include(${CMAKE_SOURCE_DIR}/scimake/SciPolicies.cmake NO_POLICY_SCOPE)

# Project information
PROJECT(polyswift)

######################################################################
#
# Usual initialization stuff
#
######################################################################

message(STATUS "---------- Setting NOFORTRAN TRUE by hand ----------")
set(NOFORTRAN TRUE)

# if (${CMAKE_C_COMPILER} MATCHES cl.exe)
#   set(NOFORTRAN TRUE)
# endif ()
include(${CMAKE_SOURCE_DIR}/scimake/SciInit.cmake)
message(STATUS "!!!!!!!")
message(STATUS "CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}")
include(${CMAKE_SOURCE_DIR}/txcmake/TxInit.cmake)
set(POLYSWIFT_VERSION "${PROJECT_VERSION}")
set(POLYSWIFT_REV "${PROJECT_REV}")

######################################################################
#
# Set for inclusion in config.h
#
######################################################################

set(CXX ${CMAKE_CXX_COMPILER})
set(BUILD_FLAGS_VAR  CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE})
set(BUILD_FLAGS_VAL "${${BUILD_FLAGS_VAR}}")
set(CXXFLAGS "${BUILD_FLAGS_VAL} ${CMAKE_CXX_FLAGS}")
set(HAVE_CMAKE 1 CACHE STRING "Whether built with CMake")

######################################################################
#
# Always use rpath to greatest extent.
# See: http://www.itk.org/Wiki/CMake_RPATH_handling
# Add -DCMAKE_SKIP_RPATH:BOOL=TRUE to prevent any rpath handling
#
######################################################################

# Add the automatically determined parts of the RPATH that
# point to directories outside the build tree to the install RPATH

# SET(CMAKE_SKIP_BUILD_RPATH FALSE)
# Use, i.e. don't skip the full RPATH for the build tree
# Not needed since we build static libs

# SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
# When building, don't link with the install RPATH at build,
# but add later on when installing.
# Not needed since we build static libs

# SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
# The RPATH to be used when installing
# Not needed since we build static libs

if (NOT USE_STATIC_SYSLIBS)
  if (NOT DEFINED CMAKE_INSTALL_RPATH_USE_LINK_PATH)
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
  endif ()
endif ()
# Add the automatically determined parts of the RPATH that
# point to directories outside the build tree to the install RPATH
######################################################################

##################################################################
#
# Find packages
#
##################################################################

# This is matching the vorpal setup for setting the PSDEPS_STATE variable
set(PSDEPS_STATE REQUIRED)
if (BASIC_DEPS_REQUIRED STREQUAL "False") # If set to false, change to empty
  message(STATUS "Unsetting REQUIRED in find_package calls for dependencies.")
#  set(PSDEPS_STATE)
endif ()

find_package(SciTxBase  REQUIRED)
find_package(SciZ REQUIRED)

# Sz is not a dependency on Windows
IF(NOT WIN32)
  find_package(SciSz)
ENDIF(NOT WIN32)

#
# HDF5
if (WIN32 AND (CMAKE_BUILD_TYPE STREQUAL Debug))
  set(Hdf5_NEEDED_LIBS hdf5_D)
else ()
  set(Hdf5_NEEDED_LIBS hdf5 hdf5dll hdf5_debug OPTIONAL)
endif ()
find_package(SciHdf5 ${PSDEPS_STATE})
add_definitions(${Hdf5_DEFINITIONS} -DH5_USE_18_API)

# Libraries needed by users of hdf5 alone
if (USE_STATIC_SYSLIBS)
  set(HDF5_ALL_LIBS ${Hdf5_STLIBS})
else ()
  set(HDF5_ALL_LIBS ${Hdf5_LIBRARIES})
endif ()
set(HDF5_ALL_LIBS ${HDF5_ALL_LIBS} ${Sz_LIBRARIES} ${Z_LIBRARIES})
if (LINUX AND (NOT (CMAKE_CXX_COMPILER_ID STREQUAL Intel)) AND
    (NOT USE_STATIC_SYSLIBS))
  set(HDF5_ALL_LIBS ${HDF5_ALL_LIBS} dl)
endif ()
set(Hdf5_LIBRARIES ${HDF5_ALL_LIBS})
SciPrintVar(Hdf5_LIBRARIES)

find_package(SciFftw  REQUIRED)

# Boost required for use of Trilinos
if (WIN32)
# This prevents the Boost autolink feature, which looks for
# libraries with tagged names.
# https://www.npcglib.org/~stathis/blog/2012/05/25/coding-tip-disable-autolinking-in-boost/
  add_definitions( -DBOOST_ALL_NO_LIB )
  if (NOT BUILD_SHARED_LIBS)
# Must be explicit about this for Windows or declspecs will be wrong
    add_definitions(-DBOOST_THREAD_USE_LIB)
  endif ()
  find_package(SciBoost
    COMPONENTS
      unit_test_framework
      program_options filesystem chrono system REQUIRED
  )
else ()
  find_package(SciBoost
    COMPONENTS
      unit_test_framework
      program_options filesystem thread chrono system exception REQUIRED
  )
endif ()

# Find Python with new Tx find
find_package(SciPython)
MESSAGE("PYTHON_FOUND = ${PYTHON_FOUND}")
MESSAGE("PYTHON_INCLUDE_DIRS = ${PYTHON_INCLUDE_DIRS}")
MESSAGE("PYTHON_LIBRARIES = ${PYTHON_LIBRARIES}")
add_definitions(${Python_DEFINITIONS})

# Find txutils
find_package(TxUtils REQUIRED)

# add redoptcomp stuff because it is needed for the build
list(GET TxUtils_EXECUTABLES 0 REDOPTCOMP)
get_filename_component(REDOPTCOMP ${REDOPTCOMP} PATH)
set(REDOPTCOMP "${REDOPTCOMP}/redoptcomp.sh")

# Link data and scripts to build dir
if (NOT WIN32)
  execute_process(COMMAND mkdir txutils)
  execute_process(COMMAND ln -sf ${REDOPTCOMP} txutils/CC)
  execute_process(COMMAND ln -sf ${REDOPTCOMP} txutils/cxx)
  execute_process(COMMAND ln -sf ${REDOPTCOMP} txutils/cc)
  execute_process(COMMAND ln -sf ${REDOPTCOMP} txutils/f90)
  execute_process(COMMAND ln -sf ${REDOPTCOMP} txutils/f77)
endif ()

if (WIN32)
# Define so that including the Windows.h doesn't override min/max
  add_definitions(-DNOMINMAX)
  add_definitions(-DWIN32_LEAN_AND_MEAN)
endif ()

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
# Testing
#
######################################################################

# Unit tests
include(CTestConfig.cmake)
include(CTest)  # Must be added at top level, apparently
enable_testing()

#
# Subdirectories
#
add_subdirectory(psbase)
add_subdirectory(psbndry)
add_subdirectory(pscomm)
add_subdirectory(psdecomp)
add_subdirectory(pseffhamil)
add_subdirectory(psfft)
add_subdirectory(psgrid)
add_subdirectory(pshist)
add_subdirectory(psphysf)
add_subdirectory(pspolymer)
add_subdirectory(pssolvent)
add_subdirectory(psstd)
add_subdirectory(pstrol)
add_subdirectory(psutils)
add_subdirectory(polyswift)

######################################################################
#
# Documentation
#   Could all this be moved into the directory?
#
######################################################################

# Documentation sudirectories
include(scimake/SciDocLogic.cmake)
if (ENABLE_DEVELDOCS)
   add_subdirectory(psdevdocs)
endif ()
######################################################

######################################################################
#
# Install txutils items
#
######################################################################

# Installing txutils scripts
install(PROGRAMS
  ${TxUtils_PROGRAMS}
  DESTINATION bin
  PERMISSIONS OWNER_READ OWNER_EXECUTE OWNER_WRITE
              GROUP_READ GROUP_EXECUTE ${TX_GROUP_WRITE}
              ${TX_WORLD_PROGRAM_PERMS}
)

# Install preprocessing macro files from txutils
set(TxUtils_MACROS)
foreach (fl ${TxUtils_FILES})
  if (${fl} MATCHES "\\.mac$")
    set(TxUtils_MACROS ${TxUtils_MACROS} ${fl})
  endif ()
endforeach ()

# Installing txutils macros
install(FILES
  ${TxUtils_MACROS}
  DESTINATION share/macros
  PERMISSIONS OWNER_READ OWNER_WRITE
              GROUP_READ ${TX_GROUP_WRITE}
              ${TX_WORLD_PROGRAM_PERMS}
)

######################################################################
#
# Finalize (installation, packaging)
#
######################################################################

# Installing txutils scripts
install(PROGRAMS
  ${TxUtils_EXECUTABLES}
  DESTINATION bin
  PERMISSIONS OWNER_READ OWNER_EXECUTE OWNER_WRITE
              GROUP_READ GROUP_EXECUTE ${TX_GROUP_WRITE}
              ${TX_WORLD_PROGRAM_PERMS}
)

# Set before final installation and packaging
set(COMMON_INSTALL TRUE)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "PolySwift++ Computational Framework")
set(CONFIG_FILES)  # Clear out
include(${CMAKE_SOURCE_DIR}/scimake/SciFinalize.cmake)

