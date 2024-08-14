## ##########################################################################
##
## File:        Makefile.am
##
## Purpose:     For specifying extra files to be part of the distribution.
##
## $Id: Makefile.am 1477 2007-12-12 22:23:16Z dws $
##
## ##########################################################################

REGRESSION_TESTS_SER = diblock2Level3s
REGRESSION_TESTS_PAR = diblock2Level3p

EXTRA_DIST = \
        diblock2Level.pre diblock2Level3s.sh diblock2Level3p.sh

include ../pstests.am

## #######
##
## Other targets
##
## #######

include $(top_srcdir)/txtest/modify-output.am

# Note new stuff
include $(top_srcdir)/txtest/testdiff.am
