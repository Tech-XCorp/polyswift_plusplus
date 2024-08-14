######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building psuserdocs/PSimInstallation.
#
# @version $Id$
#
# Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

######################################################################
#
# CMakeLists.txt for PSimInstallation
#
# $Id: CMakeLists.txt 532 2012-11-18 09:41:27Z cary $
#
######################################################################

add_subdirectory(customizations)

set(RST_FILE_BASE PSimInstallation)
set(RST_AUTHOR "The Polyswift Team")
set(RST_TITLE  "PSim Installation and Release Notes")
set(RST_MAIN_FILE ${RST_FILE_BASE}) # Used in conf.py for master_doc
set(RST_MAIN_TITLE ${RST_TITLE}) # Used in conf.py for master_doc

file(GLOB INSTALLATION_DEPS *.rst)
set(INSTALLATION_DEPS ${INSTALLATION_DEPS} PARENT_SCOPE)

# It's better to copy customizations so taht we only have one copy
file(COPY ${CMAKE_SOURCE_DIR}/customizations DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")

# Pull together all ReST files for the installation manual
file(GLOB INSTALLATION_DEPS *.rst)
set(INSTALLATION_DEPS ${INSTALLATION_DEPS} PARENT_SCOPE)

# Configure after RST_ variables set
configure_file(${CONF_PY_IN} conf.py)  # 1 copy
SciSphinxTarget(TARGET ${RST_FILE_BASE}
    SPHINX_ADDL_OPTS ${SPHINX_MATHARG} -tmaindoc
    RST_FILE_BASE ${RST_FILE_BASE}
    FILE_DEPS ${INSTALLATION_DEPS}
)
add_custom_target(${RST_FILE_BASE} ALL
    DEPENDS ${RST_FILE_BASE}-html ${RST_FILE_BASE}-pdf
)

# Clean: https://cmake.org/pipermail/cmake/2004-June/005192.html
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES
  "html;pdf"
)
