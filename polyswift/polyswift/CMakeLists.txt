######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building polyswift//polyswift.
#
# @version $Id: CMakeLists.txt 2173 2022-08-01 22:47:53Z alexanda $
#
# Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

######################################################################
#
# CMakeLists.txt for polyswift
#
# $Id: CMakeLists.txt 2173 2022-08-01 22:47:53Z alexanda $
#
######################################################################

include_directories(
  ${CMAKE_SOURCE_DIR}/pstrol
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
  ${Python_INCLUDE_DIRS}
  ${TxBase_INCLUDE_DIRS}
  ${TxUtils_INCLUDE_DIRS}
  ${Hdf5_INCLUDE_DIRS}
  ${Fftw_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
)

if (HAVE_SECURITY)
  include_directories(${TxLicMgr_INCLUDE_DIRS} ${OpenSsl_INCLUDE_DIRS})
endif ()

if (PYTHONLIBS_FOUND)
  include_directories(${Python_INCLUDE_DIRS})
endif ()

if (ENABLE_PARALLEL AND NOT SCI_HAVE_MPICXX_COMPILER_WRAPPER)
  include_directories("${MPI_INCLUDE_DIRS}")
endif ()

######################################################################
#
# Collect the internal libraries
#
######################################################################

set(polyswift_projlibs
  pstrol
  pscomm
  psbndry
  psdecomp pseffhamil psfft
  psgrid pshist psphysf pspolymer pssolvent
  psbase psstd
)

######################################################################
#
# Collect external, non-Tech-X, non-license libraries
#
######################################################################

set(polyswift_extlibs
  ${TxUtils_LIBRARIES}
  ${TxBase_LIBRARIES}
  ${Hdf5_LIBRARIES}
  ${Fftw_LIBRARIES}
)

TxAddSysCudaLicLibs(polyswift CUDA_LIBS cusparse curand cudart_static cudadevrt
  ADD_PYTHON_LIBS ADD_SECURITY_LIBS)

# Boost must be after TxLicMgr libs on Linux
set(polyswift_extlibs ${polyswift_extlibs}
  ${Boost_STLIBS}
)

if (NOT WIN32)
# Cray wrappers include Z.  Make sure not present in implicit libraries
  list(FIND CMAKE_Fortran_IMPLICIT_LINK_LIBRARIES "z" zindx)
  message(STATUS "z library found at ${zindx}.")
  if (zindx EQUAL -1)
    list(APPEND polyswfit_extlibs ${Z_LIBRARIES})
  endif ()
endif ()

message(STATUS "polyswift_extlibs = ${polyswift_extlibs}.")
#SciGetStaticLibs("${polyswift_extlibs}" polyswift_extlibs)
#message(STATUS "After SciGetStaticLibs, polyswift_extlibs = ${polyswift_extlibs}.")

######################################################################
#
# Collect system libraries
#
######################################################################

# System libraries that we can assume always present
# Python needs these libraries on Hopper, but they have to be at the end.
# Add linear algebra and fortran libraries here, as they need to
# be share on ubuntu
set(polyswift_syslibs)
if (WIN32)
  list(APPEND polyswift_syslibs ws2_32 winmm)
else ()
# Remove hugetables library in parallel, since included by default on Cray; but
# need to add in serial.
  if (ENABLE_PARALLEL)
    list(FILTER polyswift_syslibs EXCLUDE REGEX hugetlbfs)
  else ()
    if (EXISTS /usr/lib64/libhugetlbfs.a)
      list(APPEND polyswift_syslibs hugetlbfs)
    endif ()
  endif ()
endif ()
message(STATUS "polyswift_syslibs = ${polyswift_syslibs}.")
list(REVERSE polyswift_syslibs)
list(REMOVE_DUPLICATES polyswift_syslibs)
list(REVERSE polyswift_syslibs)
SciGetStaticLibs("${polyswift_syslibs}" polyswift_stsyslibs)
message(STATUS "polyswift_stsyslibs = ${polyswift_stsyslibs}.")
if (NOT WIN32)
  list(APPEND polyswift_syslibs pthread util dl)
# Needed for cori with PrgEnv-gnu
  list(APPEND polyswift_stsyslibs -Wl,-Bdynamic pthread util dl)
  if (LINUX)
    list(APPEND polyswift_syslibs rt)
    list(APPEND polyswift_stsyslibs rt)
  endif ()
endif ()

######################################################################
#
# Create target, link internal and external libraries
#
######################################################################

if (ENABLE_PARALLEL)
  set(POLYSWIFT_EXE polyswift)
else ()
  set(POLYSWIFT_EXE polyswiftser)
endif ()
add_executable(${POLYSWIFT_EXE} polyswift.cxx)
target_link_libraries(${POLYSWIFT_EXE} ${polyswift_projlibs})
target_link_libraries(${POLYSWIFT_EXE} ${polyswift_extlibs})
set_target_properties(${POLYSWIFT_EXE} PROPERTIES
  CUDA_RESOLVE_DEVICE_SYMBOLS OFF)

######################################################################
#
# Link system libs, statically or dynamically as requested, with
# correct ending for linke line
#
######################################################################

if (USE_STATIC_SYSLIBS)
  message(STATUS "Using static system libraries: ${polyswift_stsyslibs}")
  target_link_libraries(${POLYSWIFT_EXE} ${polyswift_stsyslibs})
# This gets the link line to end with -Wl,-Bstatic, so that subsequent
# libraries linked by the Cray compiler wrappers are linked static.
# http://www.cmake.org/cmake/help/cmake2.6docs.html#prop_tgt:LINK_SEARCH_END_STATIC
  set_target_properties(${POLYSWIFT_EXE} PROPERTIES LINK_SEARCH_END_STATIC TRUE)
else ()
  message(STATUS "Using system libraries: ${polyswift_syslibs}")
  target_link_libraries(${POLYSWIFT_EXE} ${polyswift_syslibs})
  if (LINUX)
    set_target_properties(${POLYSWIFT_EXE} PROPERTIES INSTALL_RPATH "\$ORIGIN:\$ORIGIN/../lib")
  endif ()
# make sure all names added as static
# See http://mail.kde.org/pipermail/kde-buildsystem/2008-February/004478.html
#  set(LINK_SEARCH_END_STATIC TRUE)
#  set(CMAKE_LINK_OLD_PATHS TRUE)
endif ()

######################################################################
#
# Install polyswift
#
######################################################################

install(TARGETS ${POLYSWIFT_EXE}
  DESTINATION bin
  PERMISSIONS OWNER_READ OWNER_EXECUTE OWNER_WRITE
              GROUP_READ GROUP_EXECUTE ${SCI_GROUP_WRITE}
              ${SCI_WORLD_PROGRAM_PERMS}
)

######################################################################
#
# Generate license
#
######################################################################

# If we are building with the license manager, we generate a 128-core license
# in the build area so that we can run tests and developers can run the latest
# build. Later when the composer is installed, we generate a 2-core license to
# replace this one in the distribution.
include(${TXCMAKE_DIR}/TxGenerateLicense.cmake)
if (HAVE_SECURITY)
  TxGenerateEngineLicense(PSim Polyswift ${PSIM_VERSION_MAJOR} 128
        "All" ${CMAKE_BINARY_DIR}/polyswift)
  install(FILES ${CMAKE_BINARY_DIR}/polyswift/license.txt
      DESTINATION bin
      PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
      OPTIONAL # License manager may not be present
  )
endif ()

######################################################################
#
# Unit tests
#
######################################################################

if (HAVE_SECURITY)
  message(STATUS "Adding polyswift/licenseTests subdirectory.")
  add_subdirectory(licenseTests)
endif ()

