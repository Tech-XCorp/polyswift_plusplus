######################################################################
#
# @file    CMakeLists.txt
#
# @brief   For building psuserdocs/PSimReferenceManual.
#
# @version $Id$
#
# Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
# All rights reserved.
#
######################################################################

######################################################################
#
# CMakeLists.txt for reference_manual
#
# $Id: CMakeLists.txt 645 2012-12-29 22:28:25Z cary $
#
######################################################################

set(RST_FILE_BASE PSimReferenceManual)
set(RST_AUTHOR "The PolySwift++ and Composer Teams")
set(RST_TITLE  "PSim Reference Manual")
set(RST_MAIN_FILE ${RST_FILE_BASE}) # Used in conf.py for master_doc
set(RST_MAIN_TITLE ${RST_TITLE}) # Used in conf.py for master_doc

# It's better to copy customizations so that we only have one copy
file(COPY ${CMAKE_SOURCE_DIR}/customizations DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")

# Pull together all ReST files for reference manual
file(GLOB REFMAN_DEPS *.rst */*.rst)
set(REFMAN_DEPS ${REFMAN_DEPS} PARENT_SCOPE)

# This sets the targets for each build (html, pdf, ...)
configure_file(${CONF_PY_IN} conf.py)  # 1 copy
SciSphinxTarget(TARGET ${RST_FILE_BASE}
    SPHINX_ADDL_OPTS ${SPHINX_MATHARG} -tmaindoc
    RST_FILE_BASE ${RST_FILE_BASE}
    FILE_DEPS ${REFMAN_DEPS}
)

# Clean: https://cmake.org/pipermail/cmake/2004-June/005192.html
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES
  "html;pdf"
)

