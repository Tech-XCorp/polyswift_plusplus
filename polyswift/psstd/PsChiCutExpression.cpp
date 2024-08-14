/**
 *
 * @file    PsChiCutExpression.cpp
 *
 * @brief   Implementation of a functor for arbitrary expression
 *
 * @version $Id: PsChiCutExpression.cpp 2137 2020-04-16 20:37:29Z alexanda $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation
 */

// psstd includes
#include <PsChiCutExpression.h>

template <class FLOATTYPE, int NDIM>
PsChiCutExpression<FLOATTYPE, NDIM>::PsChiCutExpression() {
}

template <class FLOATTYPE, int NDIM>
void PsChiCutExpression<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  this->lowerValInputName = "chi_lower";
  this->upperValInputName = "chi_upper";
  this->exprNameStr = "chi";

  // Scoping call for base class
  PsCutExpression<FLOATTYPE, NDIM>::setAttrib(tas);
}


// Instantiations

template class PsChiCutExpression<float, 1>;
template class PsChiCutExpression<float, 2>;
template class PsChiCutExpression<float, 3>;

template class PsChiCutExpression<double, 1>;
template class PsChiCutExpression<double, 2>;
template class PsChiCutExpression<double, 3>;

