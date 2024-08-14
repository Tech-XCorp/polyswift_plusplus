######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building psexamples.
#
# @version $Id$
#
# Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

######################################################################
#
# Set up project and initialize if standalone
#
######################################################################

# Project information
project(psexamples)

# Set version
if (POLYSWIFT_PROJVERSION)
  set(PROJECT_VERSION ${POLYSWIFT_PROJVERSION})
else ()
  set(VERSION_MAJOR "1")
  set(VERSION_MINOR "8")
  set(VERSION_PATCH "0dev")
  set(PROJECT_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
endif ()
set(PSEXAMPLES_VERSION "${PROJECT_VERSION}")

# Required version
cmake_minimum_required(VERSION 2.8.4)

if (NOT EMBEDDED_EXAMPLES_SUBDIR)
# Initialize scimake modules
  message(STATUS "Running psexamples/CMakeLists.txt code...")
  set(NOFORTRAN TRUE)
  include(${CMAKE_SOURCE_DIR}/scimake/SciInit.cmake)
  include(${CMAKE_SOURCE_DIR}/txcmake/TxInit.cmake)
  set(PSEXAMPLES_REV "${PROJECT_REV}")
  include(${CMAKE_SOURCE_DIR}/scimake/Modules/SciSphinxFunctions.cmake)
# This might be confusing, but the revision is that of psexamples, not polyswift
  set(PROJECT_VERSION ${PROJECT_VERSION}-r${PSEXAMPLES_REV})
  set(PSEXAMPLES_VERSION "${PROJECT_VERSION}")
  SciPrintVar(PSEXAMPLES_VERSION)
endif()
get_filename_component(EXAMPLES_TOPDIR ${CMAKE_CURRENT_SOURCE_DIR} NAME)

######################################################################
#
# Options
#
######################################################################

option(ENABLE_BETA "Enable beta examples." FALSE)
if (EMBEDDED_EXAMPLES_SUBDIR)
  set(ENABLE_BETA FALSE)
endif ()

######################################################################
#
# Find Sphinx and MathJax
# MATHJAX
#
######################################################################

if (NOT EMBEDDED_EXAMPLES_SUBDIR)
  find_package(SciSphinx REQUIRED)
  if (NOT "${MATHJAXJS}" MATCHES "^http")
    find_package(SciMathJax REQUIRED)
    set(MATHJAXJS "${MathJax_MathJax_js}")
  endif ()
endif ()

######################################################################
#
# install at top
#
######################################################################

if (NOT EXAMPLES_DESIRED_INSTALL_SUBDIR)
  set(EXAMPLES_DESIRED_INSTALL_SUBDIR .)
endif ()

######################################################################
#
# Go into subdirectories and let them copy
#
######################################################################

unset(GLOBAL_EXAMPLES_RUN_FILES)
unset(GLOBAL_EXAMPLES_DOC_FILES)
unset(ALL_EXAMPLE_GROUPS)

set(CONF_PY_IN ${CMAKE_SOURCE_DIR}/zzdocsconf/conf.py.in)
set(SCI_BIBTEX_REFS ${CMAKE_CURRENT_SOURCE_DIR}/zzdocsconf/psim.bib)

# add_subdirectory(codeFiles)
add_subdirectory(customizations)
set(PSEXAMPLES_SETVARS)
# Initialize
set(PSIMBASE_BETA_EXCLUDE "[]")
set(PSIMPLUS_BETA_EXCLUDE "[]")
set(SPHINX_DOCUMENT_CLASS howto) # Removes extra pages
# Add subdirs
add_subdirectory(PSimBase)
add_subdirectory(PSimPlus)

# Create variables needed by psuserdocs
# message(STATUS "Cleaning up variables.")
separate_arguments(ALL_EXAMPLE_GROUPS)
message(STATUS "ALL_EXAMPLE_GROUPS = ${ALL_EXAMPLE_GROUPS}.")
set(PSIMEX_CMAKE_FILE ${CMAKE_BINARY_DIR}/psexamples.cmake)
file(REMOVE ${PSIMEX_CMAKE_FILE})
foreach (exgrp ${ALL_EXAMPLE_GROUPS})
  string(TOUPPER "${exgrp}" exgrpuc)
  string(REGEX REPLACE "[/ ]" "_" exgrpuc ${exgrpuc})
  # message(STATUS "exgrpuc = ${exgrpuc}.")
  # message(STATUS "Cleaning\n${${exgrpuc}_RST_FILES}")
  set(exgrpvar ${exgrpuc}_RST_FILES)
  if (EMBEDDED_EXAMPLES_SUBDIR)
    string(REPLACE "${EMBEDDED_EXAMPLES_SUBDIR}/" "" ${exgrpvar} "${${exgrpvar}}")
  endif ()
  # set(${exgrpuc}_RST_FILES "${${exgrprstvar}}")
  if (${exgrpvar})
    message(STATUS "${exgrpvar} =\n${${exgrpvar}}")
  endif ()
  file(APPEND ${PSIMEX_CMAKE_FILE} "set(${exgrpvar} \"${${exgrpvar}}\")\n")
endforeach ()
install(FILES ${PSIMEX_CMAKE_FILE} DESTINATION .)

set(allpkgs "PSimBase PSimPlus")

######################################################################
#
# Dynamically generate conf.py, PSimExamples.rst, and
# build_index.html.
#
######################################################################

set(ALL_HTML_OUTPUT)
set(RST_FILE_BASE PSimExamples)
set(RST_AUTHOR "The PolySwift++ and Composer Teams")
set(RST_TITLE  "PSim Examples")
set(RST_MAIN_FILE ${RST_FILE_BASE}) # Used in conf.py for master_doc
set(RST_MAIN_TITLE ${RST_TITLE}) # Used in conf.py for master_doc
TxCheckSphinxVars()

set(SPHINX_DOCUMENT_CLASS manual)
configure_file(${CONF_PY_IN} conf.py)

# Configure sphinx for combined document
configure_file(${RST_MAIN_FILE}.rst.in ${CMAKE_CURRENT_SOURCE_DIR}/${RST_MAIN_FILE}.rst)

# JRC: I spent alot of time chasing an error that was the result
# of this file still being present.
if (EXISTS ${CMAKE_SOURCE_DIR}/index.rst)
  file(REMOVE ${CMAKE_SOURCE_DIR}/index.rst)
endif ()

configure_file(build_index.html.in build_index.html)

######################################################################
#
# This generates a file that shows all of the directories we used
#
######################################################################

set(psdirlist ${CMAKE_SOURCE_DIR}/codeFiles/PsExamplesDirList.txt)
file(WRITE ${psdirlist} "")
string(REPLACE " " ";" allpkgsl "${allpkgs}")
foreach (pkg ${allpkgsl})
  string(TOUPPER "${pkg}" upkg)
  message(STATUS "------- pkg   = ${pkg}")
  message(STATUS "------- upkg = ${upkg}")
  string(REPLACE " " ";" flist ${${upkg}_RST_FILES})
  foreach (f ${flist})
    get_filename_component(fpath ${f} PATH)
    file(APPEND ${psdirlist} "${fpath}\n")
  endforeach ()
endforeach ()

######################################################################
#
# Handle miscellaneous installs
#
######################################################################

install(FILES ${varsetcmake}
  DESTINATION ${EXAMPLES_DESIRED_INSTALL_SUBDIR}
  PERMISSIONS OWNER_WRITE OWNER_READ
              GROUP_WRITE GROUP_READ
              WORLD_READ
)

# Copy the top-level description.ini and rst file
# The .rst.in file is needed for psuserdocs to build.
install(FILES description.ini ${RST_MAIN_FILE}.rst ${RST_MAIN_FILE}.rst.in
  DESTINATION ${EXAMPLES_DESIRED_INSTALL_SUBDIR}
  PERMISSIONS OWNER_WRITE OWNER_READ
              GROUP_WRITE GROUP_READ
              WORLD_READ
)

install(DIRECTORY Introduction
  DESTINATION ${CMAKE_INSTALL_PREFIX}
  PATTERN ".svn" EXCLUDE
  PERMISSIONS OWNER_WRITE OWNER_READ GROUP_WRITE GROUP_READ WORLD_READ
)

install(DIRECTORY Trademark
  DESTINATION ${CMAKE_INSTALL_PREFIX}
  PATTERN ".svn" EXCLUDE
  PERMISSIONS OWNER_WRITE OWNER_READ GROUP_WRITE GROUP_READ WORLD_READ
)

######################################################################
#
# This defines the sphinx build targets that can be seen by `make help`
#
######################################################################

if (EMBEDDED_EXAMPLES_SUBDIR)
  set(gdfs ${GLOBAL_EXAMPLES_DOC_FILES})
  set(GLOBAL_EXAMPLES_DOC_FILES)
  foreach(gdf ${gdfs})
    string(REPLACE "${EMBEDDED_EXAMPLES_SUBDIR}/" "" gdf "${gdf}")
    list(APPEND GLOBAL_EXAMPLES_DOC_FILES ${gdf})
  endforeach ()
endif ()
if (DEBUG_CMAKE)
  message(STATUS "GLOBAL_EXAMPLES_DOC_FILES = ${GLOBAL_EXAMPLES_DOC_FILES}.")
endif ()
SciSphinxTarget(TARGET ${RST_FILE_BASE}
    SPHINX_ADDL_OPTS ${SPHINX_MATHARG} -tmaindoc
    RST_FILE_BASE ${RST_FILE_BASE}
    FILE_DEPS ${GLOBAL_EXAMPLES_DOC_FILES}
)
add_custom_target(${RST_FILE_BASE} ALL
    DEPENDS ${RST_FILE_BASE}-html ${RST_FILE_BASE}-pdf
)

# Clean: https://cmake.org/pipermail/cmake/2004-June/005192.html
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES
  "html;pdf"
)

