/**
 *
 * @file    PsSwitchMovTanhSlab.cpp
 *
 * @brief   Implementation of a functor for time part of moving tanh zone
 *
 * @version $Id: PsSwitchMovTanhSlab.cpp 2137 2020-04-16 20:37:29Z alexanda $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation
 */

// psstd includes
#include <PsSwitchMovTanhSlab.h>

#include <iterator>
#include <algorithm>

template <class FLOATTYPE, int NDIM>
PsSwitchMovTanhSlab<FLOATTYPE, NDIM>::PsSwitchMovTanhSlab() {

  // Allocate tmp space for type conversion
  xvec = new FLOATTYPE[NDIM];

  // Total for center pos for integer number of dir sweeps
  totalPosDist = 0.0;
  maxSweeps = 0;
  sweepNumber = 0;
}



template <class FLOATTYPE, int NDIM>
void PsSwitchMovTanhSlab<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Scoping call for base class
  PsExpression<FLOATTYPE, NDIM>::setAttrib(tas);

  // Set edge width
  if (tas.hasParam("widthParam")) {
    widthParam = tas.getParam("widthParam");
  }
  else {
    TxDebugExcept tde("PsSwitchMovTanhSlab::setAttrib: widthParam not found");
    throw tde;
  }

  // Set width of region
  if (tas.hasParam("zoneSize")) {
    zoneSize = tas.getParam("zoneSize");
  }
  else {
    TxDebugExcept tde("PsSwitchMovTanhSlab::setAttrib: zoneSize not found");
    throw tde;
  }

  // Set chi value
  if (tas.hasParam("chiNmax")) {
    chiNmax = tas.getParam("chiNmax");
  }
  else {
    TxDebugExcept tde("PsSwitchMovTanhSlab::setAttrib: chiNmax not found");
    throw tde;
  }

  // Set chi value
  if (tas.hasParam("chiNmin")) {
    chiNmin = tas.getParam("chiNmin");
  }
  else {
    TxDebugExcept tde("PsSwitchMovTanhSlab::setAttrib: chiNmin not found");
    throw tde;
  }

  // Distance buffers factors for switching zone anneal direction
  if (tas.hasPrmVec("zoneBuffers") ) {
    std::vector<double> tmp;
    tmp = tas.getPrmVec("zoneBuffers");
    for (size_t n=0; n<tmp.size(); ++n) {
      zoneBuffers.push_back((FLOATTYPE)tmp[n]);
    }
    tmp.clear();
  }
  else {
    TxDebugExcept tde("PsSwitchMovTanhSlab::setAttrib zoneBuffers not set");
    throw tde;
  }

  // List of components and center starting positions
  size_t numDirs = zoneBuffers.size(); // Kludge for NDIM
  for (size_t idim=0; idim<numDirs; ++idim) {
    pcompList.push_back((int)idim);
  }

  // Cutoff for switch
  if (tas.hasOption("maxSweeps")) {
    turnOffFlag = true;
    maxSweeps = tas.getOption("maxSweeps");
  }
  else {
    turnOffFlag = false;
  }

}


//
template <class FLOATTYPE, int NDIM>
FLOATTYPE PsSwitchMovTanhSlab<FLOATTYPE, NDIM>::operator()(
        PsTinyVector<int, NDIM>& rvec, FLOATTYPE t) const {

  for (size_t idim=0; idim<NDIM; ++idim)
    xvec[idim] = (FLOATTYPE)rvec[idim];

  return PsSwitchMovTanhSlab<FLOATTYPE, NDIM>::operator()(xvec, t);
}


template <class FLOATTYPE, int NDIM>
FLOATTYPE PsSwitchMovTanhSlab<FLOATTYPE, NDIM>::operator()(
  const FLOATTYPE* x, FLOATTYPE t) const {

  // This is running total from expression for center position
  // through all direction sweeps
  FLOATTYPE centerPosRaw = PsExpression<FLOATTYPE, NDIM>::operator()(x,t);

  // center position within a dir sweep
  FLOATTYPE centerPos = centerPosRaw - totalPosDist;

  size_t pcomp = (size_t)pcompList[0];
  if (centerPos > zoneBuffers[pcomp] ) {

    //std::cout << "sweepNumber = " << sweepNumber << std::endl;
    totalPosDist = totalPosDist + zoneBuffers[pcomp];
    rotate(pcompList.begin(),
    pcompList.begin() + 1,
    pcompList.end());
    sweepNumber = sweepNumber + 1;
    centerPos = 0.0;
  }


  //  if (switchType == "cycle") {}
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

  if ((sweepNumber >= maxSweeps) && turnOffFlag)
    return chiNmin;
  else
    return val;
}

// Instantiations

template class PsSwitchMovTanhSlab<float, 1>;
template class PsSwitchMovTanhSlab<float, 2>;
template class PsSwitchMovTanhSlab<float, 3>;

template class PsSwitchMovTanhSlab<double, 1>;
template class PsSwitchMovTanhSlab<double, 2>;
template class PsSwitchMovTanhSlab<double, 3>;

