######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building polyswift//psdevdocs.
#
# @version $Id: CMakeLists.txt 2147 2020-08-04 16:00:35Z smillie $
#
# Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

######################################################################
#
# CMakeLists.txt for doxygen develdocs
#
# $Id: CMakeLists.txt 2147 2020-08-04 16:00:35Z smillie $
#
# We have no use of "install" in this file.  That prevents the
# developer documentation from ever being put in the package.
#
######################################################################

# For configuring polyswift.dox
set(top_srcdir ${CMAKE_SOURCE_DIR})

# Where this is installed
if (ENABLE_WEBDOCS)
  message(status "ENABLE_WEBDOCS ---> on")
  set(DEVELDOCS_INSTALLDIR ${CMAKE_INSTALL_PREFIX})
else ()
  message(status "ENABLE_WEBDOCS ---> off")
  set(DEVELDOCS_INSTALLDIR ${CMAKE_INSTALL_PREFIX}/docs/develdocs)
endif ()

# Substitute paths in polyswift.dox.in
if (ENABLE_DEVELDOCS)
  configure_file(polyswift.dox.in ./polyswift.dox)
endif ()

# Variables
set(DOXYGEN_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/html/index.html)
if (DEBUG_CMAKE)
  message(STATUS "PSSTD_HEADERS = ${PSSTD_HEADERS}.")
  message(STATUS "DOXYGEN_OUTPUT = ${DOXYGEN_OUTPUT}.")
endif ()

# Case with dependency.  Unable to get it to work so far.
# Should depend on all .h files, but that will take time to construct
# For add_custom_command, first command is prefaced by COMMAND
if (0)
add_custom_command(
  OUTPUT ${DOXYGEN_OUTPUT}
  COMMAND ${DOXYGEN_EXECUTABLE} polyswift.dox
  DEPENDS ${PSSTD_HEADERS} # ${CMAKE_CURRENT_BINARY_DIR}/polyswift.dox
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
endif ()

add_custom_target(apidocs DEPENDS ${DOXYGEN_OUTPUT})
add_custom_target(develdocs)
add_dependencies(develdocs apidocs)

# For add_custom_target , first command is NOT prefaced by COMMAND
add_custom_target(apidocs-force
  ${DOXYGEN_EXECUTABLE} polyswift.dox
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(install-apidocs
  COMMAND ${CMAKE_COMMAND} -E make_directory ${DEVELDOCS_INSTALLDIR}/html
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_BINARY_DIR}/html
    ${DEVELDOCS_INSTALLDIR}/html
  COMMAND ${CMAKE_COMMAND} -E make_directory ${DEVELDOCS_INSTALLDIR}/latex
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_BINARY_DIR}/latex
    ${DEVELDOCS_INSTALLDIR}/latex
  COMMAND ${CMAKE_COMMAND} -E make_directory ${DEVELDOCS_INSTALLDIR}/man
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_BINARY_DIR}/man
    ${DEVELDOCS_INSTALLDIR}/man
  COMMAND ${CMAKE_COMMAND} -E make_directory ${DEVELDOCS_INSTALLDIR}/rtf
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_BINARY_DIR}/rtf
    ${DEVELDOCS_INSTALLDIR}/rtf
)
add_custom_target(install-develdocs)
add_dependencies(install-develdocs install-apidocs)
