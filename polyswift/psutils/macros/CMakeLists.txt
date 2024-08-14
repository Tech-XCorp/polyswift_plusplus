######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building polyswift//psutils/macros.
#
# @version $Id$
#
# Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

######################################################################
#
# CMakeLists.txt for macros
#
# $Id: CMakeLists.txt 19902 2012-07-31 21:20:01Z dws $
#
######################################################################

# ##################################
#                 Engine specific section
# ##################################

SET(UNENCRYPTED_MACROS
  pseudoSpecSetup
  verbosity
  polyDisperse
)

SET(ENCRYPTED_MACROS
)
###################################

#################################################################
#
# For licensed version:
#    Copy unencrypted macros into build dir. Encrypted macros
#    are already copied by obfuscate-encrypt.py --outdir above.
#
# For unlicensed version:
#    Copy ALL macros into build dir.
#
#################################################################

SET(ALL_MACROS ${UNENCRYPTED_MACROS} ${ENCRYPTED_MACROS})
set(MACROS)

if (TXLICMGR_FOUND)
   foreach (macro ${UNENCRYPTED_MACROS})
     set(MACROS ${MACROS} ${macro}.mac)
   endforeach ()
else ()
   foreach (macro ${ALL_MACROS})
     set(MACROS ${MACROS} ${macro}.mac)
   endforeach ()
endif()

##########################################################
#
#  Custom build target for encrypted macros
#  For now dependency on any macro is set so they rebuild if any
#  macros are edited
#
#  Note: VAR3 must have quotes so passed as list
#
# message(STATUS "CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")
# message(STATUS "MACROS = ${MACROS}" )
# message(STATUS "ENCRYPTED_MACROS = ${ENCRYPTED_MACROS}")
##########################################################

#add_custom_command(
#        OUTPUT encryptedMacros
#        COMMAND ${CMAKE_COMMAND}
#                    -DVAR1=${TXLICMGR_FOUND}
#                    -DVAR2=${TxLicMgr_obfuscate_encrypt_py}
#                    -DVAR3="${ENCRYPTED_MACROS}"
#                    -DVAR4=${CMAKE_CURRENT_SOURCE_DIR}
#                    -DVAR5=${CMAKE_CURRENT_BINARY_DIR}
#                    -P ${CMAKE_CURRENT_SOURCE_DIR}/encryptMacros.cmake
#)
#add_custom_target(encryptMacros ALL DEPENDS encryptedMacros)

##########################################################
#
# Install macros. Unencrypted ones come from the SOURCE
# dir, whereas encryped ones come from the BUILD dir.
#
##########################################################

#
# Install unlicensed macros
#  First: copy  .mac macros in build directory because any .cryp
#  are automatically in build directory through the encryption command
#
set(ALL_MACRO_FILES)
foreach (macro ${ALL_MACROS})
   set(ALL_MACROS_FILES ${ALL_MACROS_FILES} ${macro}.mac)
endforeach ()
# message(STATUS "ALL_MACROS_FILES = ${ALL_MACROS_FILES}" )
file(COPY ${ALL_MACROS_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

message(STATUS
   "Edit ENCRYPTED_MACROS macros/CMakeList.txt to set which macros are encrypted")

#
# Plain install macros
#
message (STATUS "Plain     macros to be installed = ${MACROS}")
INSTALL(FILES
   ${MACROS}
   DESTINATION share/macros
)

#
# Now grab encrypted macros from build dir.
# and install
#
set(LIC_MACROS)
foreach (macro ${ENCRYPTED_MACROS})
  if (EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${macro}.cryp)
    set(LIC_MACROS ${LIC_MACROS} ${macro}.cryp)
    set(LIC_MACROS_PATHS ${LIC_MACROS_PATHS}
        ${CMAKE_CURRENT_BINARY_DIR}/${macro}.cryp)
  endif ()
endforeach ()

if (TXLICMGR_FOUND)
    message(STATUS "Encrypted macros to be installed = ${LIC_MACROS}")
    INSTALL(FILES  ${LIC_MACROS_PATHS}  DESTINATION share/macros)
endif ()
