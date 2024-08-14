#!/bin/bash
######################################################################
#
# @file    cleanps.sh
#
# @brief   Documentation goes here.
#
# @version $Id$
#
# Copyright &copy; 2009-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

#!/bin/bash

# RUN_NAME="triblock2s"
RUN_NAME="psData"
#RUN_NAME="di2abHomoACyl2s"

rm -rf $RUN_NAME*.h5
rm -rf $RUN_NAME*.png
rm -rf $RUN_NAME*.gif
rm -rf $RUN_NAME*_.visit

rm -rf ps*.err
rm -rf polyswift*.in
rm -rf *Vars.py
rm -rf *.pyc
rm -rf ps*.out
rm -rf stats*.dat
rm -rf poly.dat
