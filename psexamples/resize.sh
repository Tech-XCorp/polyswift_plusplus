#!/bin/bash
######################################################################
#
# @file    resize.sh
#
# @brief   Documentation goes here.
#
# @version $Id$
#
# Copyright &copy; 2013-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

#!/bin/bash

PNGFILES=`ls -1 */*/*Win*.png`
echo "PNGFILES = ${PNGFILES}"

for pngfile in ${PNGFILES}; do
    cmd="sips --resampleWidth 900 $pngfile"
    echo $cmd
    $cmd
done

PNGFILES=`ls -1 Introduction/images/*.png`
echo "PNGFILES = ${PNGFILES}"

for pngfile in ${PNGFILES}; do
    cmd="sips --resampleWidth 900 $pngfile"
    echo $cmd
    $cmd
done

# cmd="sips --resampleHeight 766 $pngfile"
