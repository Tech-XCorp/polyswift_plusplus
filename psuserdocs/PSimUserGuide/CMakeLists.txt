######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building psuserdocs/PSimUserGuide.
#
# @version $Id$
#
# Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

######################################################################
#
# CMakeLists.txt for in_depth
#
# $Id: CMakeLists.txt 434 2012-11-13 00:04:19Z swsides $
#
######################################################################

set(RST_FILE_BASE PSimUserGuide)
set(RST_AUTHOR "The PolySwift++ and Composer Teams")
set(RST_TITLE  "PSim User Guide")
set(RST_MAIN_FILE ${RST_FILE_BASE}) # Used in conf.py for master_doc
set(RST_MAIN_TITLE ${RST_TITLE}) # Used in conf.py for master_doc

# Copy customizations so that we only have one copy
file(COPY ${CMAKE_SOURCE_DIR}/customizations DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")

# Pull together all ReST files for the PSim User Guide
file(GLOB USERGUIDE_DEPS *.rst)
set(USERGUIDE_DEPS ${USERGUIDE_DEPS} PARENT_SCOPE)

# Configure after RST_ variables set
configure_file(${CONF_PY_IN} conf.py)  # 1 copy
SciSphinxTarget(TARGET ${RST_FILE_BASE}
    SPHINX_ADDL_OPTS ${SPHINX_MATHARG} -tmaindoc
    RST_FILE_BASE ${RST_FILE_BASE}
    FILE_DEPS ${USERGUIDE_DEPS}
)

# Clean: https://cmake.org/pipermail/cmake/2004-June/005192.html
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES
  "html;pdf"
)

