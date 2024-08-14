######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building psuserdocs.
#
# @version $Id$
#
# Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

######################################################################
#
# CMakeLists.txt for psuserdocs
#
# $Id: CMakeLists.txt 434 2012-11-13 00:04:19Z swsides $
#
######################################################################

######################################################################
#
# Set up project and initialize if standalone
#
######################################################################

# Project information
project(psuserdocs)
set(VERSION_MAJOR "1")
set(VERSION_MINOR "8")
set(VERSION_PATCH "0dev")
set(PROJECT_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

# Required version
cmake_minimum_required(VERSION 2.8.4)

set(NOFORTRAN TRUE)
include(${CMAKE_SOURCE_DIR}/scimake/SciInit.cmake)
include(${CMAKE_SOURCE_DIR}/txcmake/TxInit.cmake)
set(PSUSERDOCS_VERSION "${PROJECT_VERSION}")
set(PSUSERDOCS_REV "${PROJECT_REV}")

find_package(SciSphinx REQUIRED)
include(${CMAKE_SOURCE_DIR}/scimake/Modules/SciSphinxFunctions.cmake)

######################################################################
#
# Useful variables
#
######################################################################

set(SRCDIR ${CMAKE_CURRENT_SOURCE_DIR})
set(BLDDIR ${CMAKE_CURRENT_BINARY_DIR})

######################################################################
#
# Find psexamples  and get the needed files from them
#
######################################################################
find_package(TxPsexamples REQUIRED)

######################################################################
#
# Handle arguments to sphinx, especially mathjax, and define a variable
# to automatically invoke sphinx.
#
######################################################################

# Copy MathJax into the build dir, I gues because Sphinx is not
# smart enough to point to a file and so to look at the build
# one needs a local copy?
# Or is it because at installation time, this makes it easier
# on VC by letting it grab mathjax from the documentation?
# If the latter, why not just do this at install time?
# Any any case, not needed for url builds
if ("${MATHJAXJS}" MATCHES "^http")
  set(SPHINX_MATHARG "-d mathjax_path=${MATHJAXJS}")
else ()
  find_package(SciMathJax REQUIRED)
  set(MATHJAXJS "MathJax/MathJax.js?config=mathjaxconf")
# Right now we copy so that the build has the same structure as
# the eventual installation, with a reference to the top-level
# customizations directory.
# This is for composer.  Annoying otherwise
# This does not work, as one should be able to make clean
  if (FALSE)
  message(STATUS "Copying ${MathJax_ROOT_DIR} to ${BLDDIR}/html/_static/MathJax.")
  file(COPY ${MathJax_ROOT_DIR}/ DESTINATION ${BLDDIR}/html/_static/MathJax)
  message(STATUS "Copy of ${MathJax_ROOT_DIR} completed.")
# copy our custom mathjax configuration file to the config directory
# custom config file gets rid of the accessibility-menu of TeX-AMS-MML_SVG
  file(COPY customizations/mathjaxconf.js DESTINATION "${BLDDIR}/html/_static/MathJax/config")
  endif ()
endif ()

######################################################################
#
# Useful variables
#
######################################################################
set(RST_FILE_BASE PSimDocumentation)
set(RST_FILE_TITLE PSim)
set(RST_AUTHOR "The PolySwift++ and Composer Teams")
set(RST_TITLE  "PSim Documentation")
set(RST_MAIN_FILE ${RST_FILE_BASE}) # Used in conf.py for master_doc
set(USERDOCS_INSTALLDIR ${CMAKE_INSTALL_PREFIX})
set(DOCS_FILES ${CMAKE_CURRENT_BINARY_DIR}/build_index.html)

######################################################################
#
# Subdirectories with different manuals
#
######################################################################

set(CONF_PY_IN ${CMAKE_SOURCE_DIR}/zzdocsconf/conf.py.in)
file(COPY customizations DESTINATION "${BLDDIR}/")

set(DOCUMENT_CLASS manual) # Sets style

set(PSIMBASE_BETA_EXCLUDE "[]")
set(PSIMPLUS_BETA_EXCLUDE "[]")
set(EMBEDDED_EXAMPLES_SUBDIR PSimExamples) # To remove this name from vars
add_subdirectory(PSimExamples)
# Location of refs changes after PSimExamples
set(SCI_BIBTEX_REFS ${CMAKE_CURRENT_SOURCE_DIR}/zzdocsconf/psim.bib)
add_subdirectory(PSimInstallation)
add_subdirectory(PSimUserGuide)
add_subdirectory(PSimCustomization)
add_subdirectory(PSimReferenceManual)

######################################################################
#
# Configure files
#
######################################################################

configure_file(build_index.html.in build_index.html)

configure_file(${CONF_PY_IN} conf.py)
# Configure sphinx for combined document
configure_file(${RST_MAIN_FILE}.rst.in ${CMAKE_CURRENT_SOURCE_DIR}/${RST_MAIN_FILE}.rst)

######################################################################
#
# This handles the building of the documentation that includes:
#   psim installation
#   psim user guide
#   psim examples
#   psim customization
#   psim reference manual
#
######################################################################

set(POLYSWIFTDOC_DEPS ${EXAMPLES_DEPS} ${USERGUIDE_DEPS} ${REFMAN_DEPS} ${CUSTMAN_DEPS})
set(POLYSWIFTDOC_DEPS ${POLYSWIFTDOC_DEPS} ${INSTALLATION_DEPS})
set(POLYSWIFTDOC_DEPS ${POLYSWIFTDOC_DEPS} ${RST_FILE_BASE}.rst)
SciSphinxTarget(TARGET PSimDocumentation
    SPHINX_ADDL_OPTS ${SPHINX_MATHARG}
    RST_FILE_BASE ${RST_FILE_BASE}
    FILE_DEPS ${POLYSWIFTDOC_DEPS}
)

# Add top level targets
add_custom_target(userdocs ALL DEPENDS PSimExamples-pdf PSimInstallation-pdf PSimUserGuide-pdf PSimCustomization-pdf PSimReferenceManual-pdf PSimDocumentation-html)

# Clean: https://cmake.org/pipermail/cmake/2004-June/005192.html
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES
  "doctrees;html;pdf"
)

######################################################################
#
# Finalize project if standalone
#
######################################################################

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Polyswift Documentation")
set(CONFIG_FILES)  # Clear out
include(${CMAKE_SOURCE_DIR}/scimake/SciFinalize.cmake)
