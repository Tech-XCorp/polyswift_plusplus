/**
 *
 * @file    PsEffHamilHldr.cpp
 *
 * @brief   Base class owning instances of base class PsEffHamil
 *
 * @version $Id: PsEffHamilHldr.cpp 6319 2007-01-24 22:39:46Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// pseffhamil includes
#include <PsEffHamilHldr.h>

// txbase includes
#include <TxMakerMap.h>

template <class FLOATTYPE, size_t NDIM>
void PsEffHamilHldr<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Look for EffHamil blocks in input file
  effhamilNames = tas.getNamesOfType("EffHamil");

  // Set the number of EffHamil blocks found
  numEffHamils = effhamilNames.size();

  // Put each effhamil attribute set into local list
  for (size_t i=0; i<numEffHamils; ++i) {

    this->pprt("Found <EffHamil> block:  ", effhamilNames[i]);
    TxHierAttribSetIntDbl attribs = tas.getAttrib(effhamilNames[i]);
    effhamilAttribs.push_back(attribs);
  }
}

template <class FLOATTYPE, size_t  NDIM>
void PsEffHamilHldr<FLOATTYPE, NDIM>::buildData(PsNamedObject* inOwner) {

  this->dbprt("PsEffHamilHldr::buildData()  ");

  for (size_t i=0; i<effhamilNames.size(); ++i) {

    // For each EffHamil look for kind
    std::string kind = effhamilAttribs[i].getString("kind");

    // Makermap returns pointer to a derived object of type PsEffHamil
    PsEffHamilBase<FLOATTYPE, NDIM>* effhamilPtr =
      TxMakerMap< PsEffHamilBase<FLOATTYPE, NDIM> >::getNew(kind);
    if (!effhamilPtr) {
      TxDebugExcept tde("PsEffHamilHldr::buildData: EffHamil pointer not set");
      throw tde;
    }

    // Set parameters and attributes for particular effhamil and build
    effhamilPtr->setName(effhamilNames[i]);
    effhamilPtr->setDomainSingletons( this->getDomainSingletons() );
    effhamilPtr->setAttrib(effhamilAttribs[i]);

    // Set name/regis and build
    effhamilPtr->setOwner(inOwner);
    inOwner->makeAvail(effhamilPtr, effhamilNames[i]);
    effhamilPtr->buildData();

    // Push pointer into effhamils structure
    effhamils.push_back(effhamilPtr);
  }
}

template <class FLOATTYPE, size_t  NDIM>
void PsEffHamilHldr<FLOATTYPE, NDIM>::buildSolvers() {

  this->dbprt("PsEffHamilHldr::buildSolvers()  ");

  // Loop over vector of effhamil pointers and call buildSolvers()
  typename std::vector< PsEffHamilBase<FLOATTYPE, NDIM>* >::iterator ipoly;
  for (ipoly = effhamils.begin(); ipoly != effhamils.end(); ++ipoly) {
    (*ipoly)->buildSolvers();
  }

}

template <class FLOATTYPE, size_t  NDIM>
void PsEffHamilHldr<FLOATTYPE, NDIM>::update(double t) {

  // Update the holder
  PsDynObj<FLOATTYPE, NDIM>::update(t);

  this->dbprt("PsEffHamilHldr::update()  ");

  // Loop over vector of effhamil pointers and call update(t)
  typename std::vector< PsEffHamilBase<FLOATTYPE, NDIM>* >::iterator ipoly;
  for (ipoly = effhamils.begin(); ipoly != effhamils.end(); ++ipoly) {
    (*ipoly)->update(t);
  }
}

//
//
template <class FLOATTYPE, size_t  NDIM>
void PsEffHamilHldr<FLOATTYPE, NDIM>::dump() {

  // Update the holder
  PsDynObj<FLOATTYPE, NDIM>::dump();

  this->dbprt("PsEffHamilHldr::dump()  ");

  // Loop over vector of effhamil pointers and call dump()
  // SWS: replace with one iterator in .h file
  typename std::vector< PsEffHamilBase<FLOATTYPE, NDIM>* >::iterator ieffh;
  for (ieffh = effhamils.begin(); ieffh != effhamils.end(); ++ieffh) {
    (*ieffh)->dump();
  }
}

// Instantiate base effhamil holder classes
template class PsEffHamilHldr<float, 1>;
template class PsEffHamilHldr<float, 2>;
template class PsEffHamilHldr<float, 3>;

template class PsEffHamilHldr<double, 1>;
template class PsEffHamilHldr<double, 2>;
template class PsEffHamilHldr<double, 3>;

