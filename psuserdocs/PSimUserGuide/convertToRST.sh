#!/bin/bash
######################################################################
#
# @file    convertToRST.sh
#
# @brief   Documentation goes here.
#
# @version $Id$
#
# Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

#!/bin/bash

type=html
ext=html

case $# in
  0|3) ;;
  1|2) echo "Usage $0 [-t type extension]"
       exit 0;;
esac

case $1 in
  -t) type=$2
      ext=$3
esac

for file in *.$ext; do
  base=`basename $file .$ext`
  pandoc -f $type -t rst -o $base.rst $file
done
