######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building psuserdocs/PSimCustomization.
#
# @version $Id$
#
# Copyright &copy; 2018-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

######################################################################
#
# CMakeLists.txt for PSimCustomization
#
# $Id: CMakeLists.txt 2266 2018-09-02 17:10:31Z cary $
#
######################################################################

# It's better to copy customizations so that we only have one copy
file(COPY ${CMAKE_SOURCE_DIR}/customizations DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")

set(RST_FILE_BASE PSimCustomization)
set(RST_AUTHOR "The Polyswift and Composer Teams")
set(RST_TITLE  "PSim Customization")
set(RST_MAIN_FILE ${RST_FILE_BASE}) # Used in conf.py for master_doc
set(RST_MAIN_TITLE ${RST_TITLE}) # Used in conf.py for master_doc

# Pull together all ReST files for the PSim Customization
file(GLOB CUSTMAN_DEPS *.rst)
set(CUSTMAN_DEPS ${CUSTMAN_DEPS} PARENT_SCOPE)

# Configure after RST_ variables set
configure_file(${CONF_PY_IN} conf.py)  # 1 copy
SciSphinxTarget(TARGET ${RST_FILE_BASE}
    SPHINX_ADDL_OPTS ${SPHINX_MATHARG} -tmaindoc
    RST_FILE_BASE PSimCustomization
    FILE_DEPS ${CUSTMAN_DEPS}
)
add_custom_target(${RST_FILE_BASE} ALL
    DEPENDS ${RST_FILE_BASE}-html ${RST_FILE_BASE}-pdf
)

# Clean: https://cmake.org/pipermail/cmake/2004-June/005192.html
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES
  "html;pdf"
)

