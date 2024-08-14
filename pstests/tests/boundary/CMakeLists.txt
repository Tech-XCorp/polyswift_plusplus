######################################################################
#
# CMakeLists.txt for pstests/tests/boundary
#
# $Id: CMakeLists.txt 419 2020-10-07 15:48:51Z alexanda $
#
# Copyright &copy; 2020-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

message(STATUS "Adding tests of type boundary.")

######################################################################
# Create a list of all tests
######################################################################

# List of tests - each is defined below, but will only be tested if
# in this list (comment out tests that aren't ready yet)
set(PSBOUNDARY_TESTS
  di2abCylinder2s
  di2abCylinder2p
  di2abHomoACyl2s
  di2abHomoACyl2p
  di2abSolCylinder2s
  di2abSolCylinder2p
# See http://redmine.txcorp.com/issues/22647
#  nanoPtclWall2s
  nanoPtclWall2p
  cza2s
  cza2p
)

#####################################################################
#
# The Tests
#
#####################################################################

# Each test follows the following format:
# set([name of test]
#     [ARG] [value for arg]
#     [ARG] [value for arg]
#     ...)
#
# The following arguments are common:
# -- INFILE_NAME:  The name of the input file, without file suffix
#                  (assumed to be .pre by default)
# -- RUN_ARGS:     Any args you would pass to Polyswift
# -- RESTART_ARGS: Any args you would pass to Polyswift for a second run.
# -- NP:           The number of processors to run with, if parallel.
#                  (NP=1 is valid.  Do not set NP for serial tests.)
#
# See TxAddEngineTest (txcmake/TxEngTestMacros.cmake) for more arguments

set(di2abCylinder2s
  INFILE_NAME di2abCylinder
  RESTART_ARGS -r 200
)

set(di2abCylinder2p
  INFILE_NAME di2abCylinder
  RESTART_ARGS -r 200
  NP 2
)

set(di2abHomoACyl2s
  INFILE_NAME di2abHomoACyl
  RESTART_ARGS -r 200
)

set(di2abHomoACyl2p
  INFILE_NAME di2abHomoACyl
  RESTART_ARGS -r 200
  NP 2
)

set(di2abSolCylinder2s
  INFILE_NAME di2abSolCylinder
  RESTART_ARGS -r 200
)

set(di2abSolCylinder2p
  INFILE_NAME di2abSolCylinder
  RESTART_ARGS -r 200
  NP 2
)

set(nanoPtclWall2s
  INFILE_NAME nanoPtclWall
  RESTART_ARGS -r 1000
)

set(nanoPtclWall2p
  INFILE_NAME nanoPtclWall
  RESTART_ARGS -r 1000
  NP 2
)

set(cza2s
  INFILE_NAME cza
  RESTART_ARGS -r 200
)

set(cza2p
  INFILE_NAME cza
  RESTART_ARGS -r 200
  NP 2
)

#
# Analyzers
#

######################################################################
# Set Results Directory
######################################################################

set(PSBOUNDARY_RESULTS_DIR ${PS_RESULTS_DIR}/boundary)

######################################################################
# Add each test
######################################################################
foreach (pstst ${PSBOUNDARY_TESTS})

# Add Common Arguments
  list(APPEND
    ${pstst}
    NAME ${pstst}
    RESULTS_DIR ${PSBOUNDARY_RESULTS_DIR}/${pstst}
    ENGINE Polyswift
    SUBDIR boundary
  )

  TxAddEngineTest(${${pstst}})

endforeach()
