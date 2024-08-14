#!/bin/bash
######################################################################
#
# @file    nanoPtcl2p.sh
#
# @brief   Cross-platform, batch script for executing POLYSWIFT++
#
# @version $Id$
#
# Copyright &copy; 2013-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

#!/bin/sh
######################################################################
#
# Purpose:  Cross-platform, batch script for executing POLYSWIFT++
#
# Copyright Tech-X, 2006
# All rights reserved.
#
######################################################################

#########################
# Create the input file
#########################

BASENAME=nanoPtcl
RUN=${BASENAME}2p

# Get location of test paths
if test ! -f ../../path.sh; then
    echo "Can't find ../../path.sh. Exiting."
    exit 1
fi
. ../../path.sh

################################
# Run PolySwift++ check result
################################

cmd="\"$MPILAUNCHER\" $MPIOPTS -np 2 $ABS_PARALLEL_EXEC -i ${BASENAME}.pre -o ${RUN}"
echo $cmd
eval $cmd

res=$?
echo $ABS_SERIAL_EXEC finished with result $res
exit $res
