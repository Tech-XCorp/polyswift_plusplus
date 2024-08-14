#!/bin/sh
######################################################################
#
# $Id: diblock2Level2p.sh 1142 2007-09-25 02:16:27Z txqaauto $
#
# Purpose:  Cross-platform, batch/interactive script for executing POLYSWIFT++
#
# Copyright &copy; 2013-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

BASENAME=diblock2Level
RUN=${BASENAME}3p

# Get paths
if test ! -f ../../path.sh; then
    echo "Can't find ../../path.sh. Exiting."
    exit 1
fi
. ../../path.sh

##########
#
# Run polyswift++ and check result code.
#
###########

cmd="\"$MPILAUNCHER\" $MPIOPTS -np 2 $ABS_PARALLEL_EXEC -i ${BASENAME}.pre -o ${RUN}"
echo $cmd
eval $cmd

echo ""
echo "Restart not yet implemented ${BASENAME}"
echo ""

res=$?
echo $ABS_PARALLEL_EXEC finished with result $res
exit $res
