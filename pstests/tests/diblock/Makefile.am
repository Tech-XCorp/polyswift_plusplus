## ##########################################################################
##
## File:        Makefile.am
##
## Purpose:     For specifying extra files to be part of the distribution.
##
## $Id: Makefile.am 1477 2007-12-12 22:23:16Z dws $
##
## ##########################################################################

REGRESSION_TESTS_SER = diblock2s triblock2s multispecf2s tri3abc2s abSolventMix2s star3ab2s
REGRESSION_TESTS_PAR = diblock2p triblock2p multispecf2p tri3abc2p abSolventMix2p star3ab2p polydBulk2p

EXTRA_DIST = \
        polydBulk.pre                      polydBulk2p.sh \
        chargedAB.pre    chargedAB2s.sh    chargedAB2p.sh \
        abSolventMix.pre abSolventMix2s.sh abSolventMix2p.sh \
        diblock.pre      diblock2s.sh    diblock2p.sh \
        triblock.pre     triblock2s.sh   triblock2p.sh \
        star3ab.pre      star3ab2s.sh    star3ab2p.sh \
        tri3abc.pre      tri3abc2s.sh    tri3abc2p.sh  \
        multispecf.pre   multispecf2s.sh multispecf2p.sh \
        abHomopMix.pre   abHomopMix.sh

include ../pstests.am

## #######
##
## Other targets
##
## #######

include $(top_srcdir)/txtest/modify-output.am

# Note new stuff
include $(top_srcdir)/txtest/testdiff.am
