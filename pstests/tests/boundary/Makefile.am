## ##########################################################################
##
## File:        Makefile.am
##
## Purpose:     For specifying extra files to be part of the distribution.
##
## $Id: Makefile.am 1477 2007-12-12 22:23:16Z dws $
##
## ##########################################################################

REGRESSION_TESTS_SER = di2abCylinder2s di2abSolCylinder2s di2abHomoACyl2s nanoPtclWall2s cza2s
REGRESSION_TESTS_PAR = di2abCylinder2p di2abSolCylinder2p di2abHomoACyl2p nanoPtclWall2p cza2p

EXTRA_DIST = \
        di2abCylinder2s.sh    di2abCylinder2p.sh    di2abCylinder.pre \
        di2abSolCylinder2s.sh di2abSolCylinder2p.sh di2abSolCylinder.pre \
        di2abHomoACyl2s.sh    di2abHomoACyl2p.sh    di2abHomoACyl.pre \
        nanoPtclWall2s.sh     nanoPtclWall2p.sh     nanoPtclWall.pre \
        cza2s.sh              cza2p.sh              cza.pre

include ../pstests.am

## #######
##
## Other targets
##
## #######

include $(top_srcdir)/txtest/modify-output.am

# Note new stuff
include $(top_srcdir)/txtest/testdiff.am
