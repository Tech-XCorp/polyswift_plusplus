#!/bin/bash
######################################################################
#
# @file    generatePyFiles.sh
#
# @brief   Documentation goes here.
#
# @version $Id$
#
# Copyright &copy; 2013-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

#!/bin/sh
######################################################################
# Developer script for generating Python geometry files
# from template etc.
######################################################################

INPUT_NAMES="nanoPtclWall di2abCylinder di2abSolCylinder di2abHomoACyl zoneAnneal cza probePtcl"

# Copy other needed files
echo "Copying geometry.py and defaults.pre to tests" ${name}.py
cp ../../../polyswift/psutils/python/geometry.py .

for name in $INPUT_NAMES
do
    echo "Copying polyswift.py to" ${name}.py
    cp ../../../polyswift/psutils/python/polyswift.py ${name}.py
done
