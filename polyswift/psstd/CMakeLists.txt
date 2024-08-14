######################################################################
#
# CMakeLists.txt for psstd
#
# $Id: CMakeLists.txt 2167 2021-10-04 16:30:58Z alexanda $
#
######################################################################

set (PSSTD_SOURCES
  PsLex.cpp
  PsPTXGen.cpp
  PsSTFuncMakerMap.cpp
  PsNamedObject.cpp
  PsMovTanhSlabExpression.cpp
  PsSwitchMovTanhSlab.cpp
  PsCutExpression.cpp
  PsChiCutExpression.cpp
  PsExprNode.cpp
  PsExpression.cpp
  PsParser.cpp
  PsPhysConsts.cpp
  PsRandom.cpp
  PsSTFunc.cpp
  PsSTPyFunc.cpp
  PsTinyMatrix.cpp
  PsTinyVector.cpp
)

set (PSSTD_HEADERS
  PsLex.h
  PsPTXGen.h
  PsSTFuncMakerMap.h
  PsNamedObject.h
  PsMovTanhSlabExpression.h
  PsSwitchMovTanhSlab.h
  PsBinOp.h
  PsCompileTimeAssert.h
  PsChiCutExpression.h
  PsCutExpression.h
  PsExprNode.h
  PsExpression.h
  PsNoArgFunc.h
  PsParser.h
  PsPhysConsts.h
  PsRandom.h
  PsSTFunc.h
  PsSTPyFunc.h
  PsTinyMatrix.h
  PsTinyVector.h
)

include_directories (
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}
  ${Python_INCLUDE_DIRS}
  ${TxBase_INCLUDE_DIRS}
  ${TxUtils_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
)

if (HAVE_SECURITY)
  include_directories(
    ${TxLicMgr_INCLUDE_DIRS}
    ${OpenSsl_INCLUDE_DIRS}
  )
endif ()

if (PYTHONLIBS_FOUND)
  include_directories(${Python_INCLUDE_DIRS})
endif ()

######################################################################
#
# Library
#
######################################################################

set(psstd_projlibs)

# Add libs here in case we make this shared some day
set(psstd_extlibs
  ${TxUtils_txutils_LIBRARY}
)

TxAddSysCudaLicLibs(psstd CUDA_LIBS cudart_static ADD_SECURITY_LIBS
  ADD_PYTHON_LIBS)
list(APPEND psstd_extlibs
  ${TxBase_LIBRARIES}
  ${Boost_boost_filesystem_LIBRARY}
  ${Boost_boost_system_LIBRARY}
)

SciPrintVar(psstd_extlibs)

# Create the library
TxCreateProjLibrary(PsStd Polyswift)


