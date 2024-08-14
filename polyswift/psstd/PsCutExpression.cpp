/**
 *
 * @file    PsCutExpression.cpp
 *
 * @brief   Implementation of a functor for arbitrary expression
 *
 * @version $Id: PsCutExpression.cpp 6901 2007-03-06 11:36:56Z cary $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation
 */

// psstd includes
#include <PsCutExpression.h>

template <class FLOATTYPE, int NDIM>
PsCutExpression<FLOATTYPE, NDIM>::PsCutExpression() {
  lowerValInputName = "lowerVal";
  upperValInputName = "upperVal";
}


template <class FLOATTYPE, int NDIM>
void PsCutExpression<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Scoping call for base class
  PsExpression<FLOATTYPE, NDIM>::setAttrib(tas);

  // Set lower clipping value for expression
  if (tas.hasParam(lowerValInputName)) {
    lowerVal = tas.getParam(lowerValInputName);
  }
  else {
    TxDebugExcept tde("PsCutExpression::setAttrib: no lowerVal set");
    tde << " in <STFunc >";
    throw tde;
  }

  // Set upper clipping value for expression
  if (tas.hasParam(upperValInputName)) {
    upperVal = tas.getParam(upperValInputName);
  }
  else {
    TxDebugExcept tde("PsCutExpression::setAttrib: no upperVal set");
    tde << " in <STFunc >";
    throw tde;
  }

}


template <class FLOATTYPE, int NDIM>
FLOATTYPE PsCutExpression<FLOATTYPE, NDIM>::operator()(const FLOATTYPE* x,
    FLOATTYPE t) const {

  // Scoping call to non-cut expression
  FLOATTYPE res = PsExpression<FLOATTYPE, NDIM>::operator()(x,t);

  if (res > upperVal) {
    res = upperVal;
  }
  if (res < lowerVal) {
    res = lowerVal;
  }

  return res;
}

// Instantiations

template class PsCutExpression<float, 1>;
template class PsCutExpression<float, 2>;
template class PsCutExpression<float, 3>;

template class PsCutExpression<double, 1>;
template class PsCutExpression<double, 2>;
template class PsCutExpression<double, 3>;

