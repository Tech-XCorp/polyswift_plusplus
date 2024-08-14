/**
 * @file    PsMovTanhSlabExpression.cpp
 *
 * @brief   Implementation of a functor for arbitrary time part of moving tanh zone
 *
 * @version $Id: PsMovTanhSlabExpression.cpp 2137 2020-04-16 20:37:29Z alexanda $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation
 */

// psstd includes
#include <PsMovTanhSlabExpression.h>

template <class FLOATTYPE, int NDIM>
PsMovTanhSlabExpression<FLOATTYPE, NDIM>::PsMovTanhSlabExpression() {

  // Allocate tmp space for type conversion
  xvec = new FLOATTYPE[NDIM];

  // Default x-dir for zone
  pcomp = 0;
}



template <class FLOATTYPE, int NDIM>
void PsMovTanhSlabExpression<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Scoping call for base class
  PsExpression<FLOATTYPE, NDIM>::setAttrib(tas);

  // Set edge width
  if (tas.hasParam("widthParam")) {
    widthParam = tas.getParam("widthParam");
  }
  else {
    TxDebugExcept tde("PsMovTanhSlabExpression::setAttrib: widthParam not found");
    throw tde;
  }

  // Set width of region
  if (tas.hasParam("zoneSize")) {
    zoneSize = tas.getParam("zoneSize");
  }
  else {
    TxDebugExcept tde("PsMovTanhSlabExpression::setAttrib: zoneSize not found");
    throw tde;
  }

  // Set chi value
  if (tas.hasParam("chiNmax")) {
    chiNmax = tas.getParam("chiNmax");
  }
  else {
    TxDebugExcept tde("PsMovTanhSlabExpression::setAttrib: chiNmax not found");
    throw tde;
  }

  // Set chi value
  if (tas.hasParam("chiNmin")) {
    chiNmin = tas.getParam("chiNmin");
  }
  else {
    TxDebugExcept tde("PsMovTanhSlabExpression::setAttrib: chiNmin not found");
    throw tde;
  }

  // Spatial component to move zone
  if (tas.hasOption("pcomp")) {
    pcomp = tas.getOption("pcomp");
  }

}


//
template <class FLOATTYPE, int NDIM>
FLOATTYPE PsMovTanhSlabExpression<FLOATTYPE, NDIM>::operator()(
        PsTinyVector<int, NDIM>& rvec, FLOATTYPE t) const {

  for (size_t idim=0; idim<NDIM; ++idim) {
    xvec[idim] = (FLOATTYPE)rvec[idim];
  }

  return PsMovTanhSlabExpression<FLOATTYPE, NDIM>::operator()(xvec, t);
}



template <class FLOATTYPE, int NDIM>
FLOATTYPE PsMovTanhSlabExpression<FLOATTYPE, NDIM>::operator()(
  const FLOATTYPE* x,
  FLOATTYPE t) const {

  // Scoping call to non-cut expression
  FLOATTYPE centerPos = PsExpression<FLOATTYPE, NDIM>::operator()(x,t);

  // Select zone anneal orientation
  FLOATTYPE r0 = x[pcomp];

  // Construct zone value
  FLOATTYPE ll = r0 - (centerPos - zoneSize);
  FLOATTYPE rr = (centerPos + zoneSize) - r0;
  FLOATTYPE tmpLeft  = ll/widthParam;
  FLOATTYPE tmpRight = rr/widthParam;
  FLOATTYPE tanhLeft  = (FLOATTYPE) std::tanh(tmpLeft);
  FLOATTYPE tanhRight = (FLOATTYPE) std::tanh(tmpRight);
  FLOATTYPE tmp = 0.5 * (tanhLeft+tanhRight);
  FLOATTYPE val = (tmp*(chiNmax-chiNmin))+chiNmin;

  return val;
}

// Instantiations

template class PsMovTanhSlabExpression<float, 1>;
template class PsMovTanhSlabExpression<float, 2>;
template class PsMovTanhSlabExpression<float, 3>;

template class PsMovTanhSlabExpression<double, 1>;
template class PsMovTanhSlabExpression<double, 2>;
template class PsMovTanhSlabExpression<double, 3>;
