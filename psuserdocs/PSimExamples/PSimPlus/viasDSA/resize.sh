#!/bin/bash
######################################################################
#
# @file    resize.sh
#
# @brief   Documentation goes here.
#
# @version $Id$
#
# Copyright &copy; 2017-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

# convert  viasDSA_ProbDescription1.png  -resize 250x250\!  viasDSATn.png
# convert  Screen.png                    -resize 400x500\!  viasDSA.png

convert Setup.png -resize 900x596\! viasDSASetupWin.png
convert Run.png   -resize 900x596\! viasDSARunWin.png
convert Viz.png   -resize 900x596\! viasDSAVizWin.png
