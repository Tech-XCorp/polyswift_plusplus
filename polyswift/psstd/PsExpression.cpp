/**
 *
 * @file    PsExpression.cpp
 *
 * @brief   Implementation of a functor for arbitrary expression
 *
 * @version $Id: PsExpression.cpp 6901 2007-03-06 11:36:56Z cary $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation
 */

// standard includes
#include <txc_math.h>
#include <PsExpression.h>


template <class FLOATTYPE, int NDIM>
PsExpression<FLOATTYPE, NDIM>::PsExpression() {
  exprNameStr = "expression";
  expression = "0.";
  dummyVar = 0.;
}

template <class FLOATTYPE, int NDIM>
void PsExpression<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

// Set diagnoseLevel to nonzero to print this out
  if (PsSTFunc<FLOATTYPE, NDIM>::diagnoseLevel) {
    std::cout << "PsExpression::setAttrib: entered." <<std::endl;
  }


// Get the expresson string
  if (tas.hasString(exprNameStr))
    expression = tas.getString(exprNameStr);


// allow expressions like 'expression = 1.0'
  if (tas.hasParam(exprNameStr)) {
     std::ostringstream oss;
     oss << tas.getParam(exprNameStr);
     expression = oss.str();
  }

  if (PsSTFunc<FLOATTYPE, NDIM>::diagnoseLevel) {
    std::cout << " found expression = " << expression << std::endl;
    std::cout << " expression set " << std::endl;
  }

  parser = new PsParser<FLOATTYPE>(expression);

  switch (NDIM) {
  case 3:
    xPtr[2] = parser->getValuePtr("z");
  case 2:
    xPtr[1] = parser->getValuePtr("y");
  case 1:
    xPtr[0] = parser->getValuePtr("x");
  }

  for (size_t i=0; i<NDIM; ++i)
    if (xPtr[i] == NULL) xPtr[i] = &dummyVar;

  tPtr = parser->getValuePtr("t");
  if (tPtr == NULL) tPtr = &dummyVar;

  std::map<std::string, int> varMap;
  varMap["x"] = 1;
  varMap["t"] = 1;
  varMap["y"] = 1;
  varMap["z"] = 1;
  parser->checkUndefVar(varMap);

}

template <class FLOATTYPE, int NDIM>
FLOATTYPE PsExpression<FLOATTYPE, NDIM>::operator()(const FLOATTYPE* x,
    FLOATTYPE t) const {

// copy the arguments into the parse tree
// (or into dummyVar, if not used in parse Tree
//  for (size_t i=0; i<NDIM; ++i) *(xPtr[i]) = x[i];
//  *tPtr = t;

  for (size_t i=0; i<NDIM; ++i) {

     if (xPtr[i] != &dummyVar) {
      if (*(xPtr[i]) != x[i]) {
         *(xPtr[i]) = x[i];
         parser->setDirty();
      }
     }
  }

  if ( tPtr != &dummyVar) {
     if ( *tPtr != t) {
        *tPtr = t;
        parser->setDirty();
     }
  }

  FLOATTYPE res = parser->evaluate();
  return res;
}

// Instantiations

template class PsExpression<float, 1>;
template class PsExpression<float, 2>;
template class PsExpression<float, 3>;

template class PsExpression<double, 1>;
template class PsExpression<double, 2>;
template class PsExpression<double, 3>;

