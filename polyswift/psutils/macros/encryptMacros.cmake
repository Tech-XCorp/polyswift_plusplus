######################################################################
#
# @file    encryptMacros.cmake
#
# @brief   Documentation goes here.
#
# @version $Id$
#
# Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

######################################################################
#
# encryptMacros.cmake for macros
#
# $Id: CMakeLists.txt 19902 2012-07-31 21:20:01Z dws $
#
######################################################################
message(STATUS " ")

##################
# Set arguments
##################

set(TXLICMGR_FOUND ${VAR1})
set(TxLicMgr_EXEC  ${VAR2})
string(REPLACE " " ";" macroList ${VAR3} ) # Parsing out spaces that are introduced
set(source_dir     ${VAR4})
set(build_dir      ${VAR5})
#message(STATUS "VAR1 = ${VAR1}")
#message(STATUS "VAR2 = ${VAR2}")
#message(STATUS "macroList = ${macroList}")
#message(STATUS "VAR4 = ${VAR4}")
#message(STATUS "VAR5 = ${VAR5}")

if (TXLICMGR_FOUND)

   #  The executable is set to obfuscate-encrypt.py in txlicmgr/crypto. Go there
   #  and 'obfuscate-encrypt.py -h" will explain options
   #  encryptedMacro.dat file below is needed by custom_target command
   #
   # This was previous code. During build "CURRENT" resolves always to build dirs (!)
   #  --indir  ${CMAKE_CURRENT_SOURCE_DIR}
   #  --outdir ${CMAKE_CURRENT_BINARY_DIR}

   if (macroList)

     foreach (mac ${macroList})
       message(STATUS "------- Encrpyting macro ${mac} -------")
         execute_process(
             COMMAND python ${TxLicMgr_EXEC}
                   --indir  ${source_dir} --outdir ${build_dir}
                   -e -m ${mac} )
     endforeach ()
     message(STATUS "------- Encrpyting finished -------\n")

   endif ()

endif ()
