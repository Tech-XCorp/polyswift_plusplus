#!/bin/bash
######################################################################
#
# @file    anim-tst.sh
#
# @brief   Documentation goes here.
#
# @version $Id$
#
# Copyright &copy; 2009-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

#!/bin/tcsh
#
####################################################################
## Runs convert to trim and compose figures
##
## Author : Scott W. Sides
## Date   : 06/13/08
####################################################################

echo "Usage: file1.png file2.png file3.png"
echo "Default: takes files named panel0.png panel1.png panel2.png"
echo " "

if ($# == 0) then
    set file1 = "panel0.png"
    set file2 = "panel1.png"
    set file3 = "panel2.png"
else
    set file1 = $argv[1]
    set file2 = $argv[2]
    set file3 = $argv[3]
endif

# Create background
convert -size 1578x536 xc:skyblue composite.png

# Trim whitespace
convert $file1 -trim +repage trim1.png
convert $file2 -trim +repage trim2.png
convert $file3 -trim +repage trim3.png

# Compose three figures side-by-side
composite -geometry +0+0    trim1.png composite.png composite.png
composite -geometry +526+0  trim2.png composite.png composite.png
composite -geometry +1052+0 trim3.png composite.png composite.png

# Cleaning files
rm -rf trim*png
rm -rf panel*png
