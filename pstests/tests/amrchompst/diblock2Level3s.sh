#!/bin/sh
######################################################################
#
# $Id: diblock2Level.sh 1142 2007-09-25 02:16:27Z txqaauto $
#
# Purpose: Cross-platform, batch/interactive script for executing POLYSWIFT++
#
# Copyright &copy; 2013-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

BASENAME=diblock2Level
RUN=${BASENAME}3s

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

cmd="$ABS_SERIAL_EXEC -i ${BASENAME}.pre -o ${RUN}"
echo $cmd
$cmd

echo ""
echo "Restart not yet implemented ${BASENAME}"
echo ""

res=$?
echo $ABS_SERIAL_EXEC finished with result $res
exit $res
