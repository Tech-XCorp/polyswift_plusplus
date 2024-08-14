######################################################################
#
# CMakeLists.txt for pstests/tests/diblock
#
# $Id: CMakeLists.txt 417 2020-08-04 16:54:16Z smillie $
#
# Copyright &copy; 2020-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

message(STATUS "Adding tests of type diblock.")

######################################################################
# Create a list of all tests
######################################################################

# List of tests - each is defined below, but will only be tested if
# in this list (comment out tests that aren't ready yet)
set(PSDIBLOCK_TESTS
  diblock2s
  diblock2p
  triblock2s
  triblock2p
  multispecf2s
  multispecf2p
  tri3abc2s
  tri3abc2p
  abSolventMix2s
  abSolventMix2p
  star3ab2s
  star3ab2p
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

set(diblock2s
  INFILE_NAME diblock
  RESTART_ARGS -r 200
)

set(diblock2p
  INFILE_NAME diblock
  RESTART_ARGS -r 200
  NP 2
)

set(triblock2s
  INFILE_NAME triblock
  RESTART_ARGS -r 400
)

set(triblock2p
  INFILE_NAME triblock
  RESTART_ARGS -r 400
  NP 2
)

set(tri3abc2s
  INFILE_NAME tri3abc
  RESTART_ARGS -r 200
)

set(tri3abc2p
  INFILE_NAME tri3abc
  RESTART_ARGS -r 200
  NP 2
)

set(multispecf2s
  INFILE_NAME multispecf
  RESTART_ARGS -r 400
)

set(multispecf2p
  INFILE_NAME multispecf
  RESTART_ARGS -r 400
  NP 2
)

set(abSolventMix2s
  INFILE_NAME abSolventMix
  RESTART_ARGS -r 200
)

set(abSolventMix2p
  INFILE_NAME abSolventMix
  RESTART_ARGS -r 200
  NP 2
)

set(star3ab2s
  INFILE_NAME star3ab
  RESTART_ARGS -r 200
)

set(star3ab2p
  INFILE_NAME star3ab
  RESTART_ARGS -r 200
  NP 2
)

#
# Analyzers
#

######################################################################
# Set Results Directory
######################################################################

set(PSDIBLOCK_RESULTS_DIR ${PS_RESULTS_DIR}/diblock)

######################################################################
# Add each test
######################################################################
foreach (pstst ${PSDIBLOCK_TESTS})

# Add Common Arguments
  list(APPEND
    ${pstst}
    NAME ${pstst}
    RESULTS_DIR ${PSDIBLOCK_RESULTS_DIR}/${pstst}
    ENGINE Polyswift
    SUBDIR diblock
  )

  TxAddEngineTest(${${pstst}})

endforeach()
