#!/bin/bash
######################################################################
#
# @file    nanoPtcl2s.sh
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
RUN=${BASENAME}2s

# Get location of test paths
if test ! -f ../../path.sh; then
    echo "Can't find ../../path.sh. Exiting."
    exit 1
fi
. ../../path.sh

################################
# Run PolySwift++ check result
################################

cmd="$ABS_SERIAL_EXEC -i ${BASENAME}.pre -o ${RUN}"
echo $cmd
$cmd

res=$?
echo $ABS_SERIAL_EXEC finished with result $res
exit $res
