/**
 *
 * @file    PsDecompHldr.cpp
 *
 * @brief   Base class owning instances of decomp object(s)
 *
 * @version $Id: PsDecompHldr.cpp 6319 2007-01-24 22:39:46Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// psdecomp includes
#include <PsDecompHldr.h>

#include <PsCommBase.h>

// txbase includes
#include <TxMakerMap.h>

template <class FLOATTYPE, size_t NDIM>
void PsDecompHldr<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Look for Decomp blocks in input file
  decompNames = tas.getNamesOfType("Decomp");
  numDecomps = decompNames.size();

  // Put each decomp attribute set into local list
  for (size_t i=0; i<numDecomps; ++i) {
    this->pprt("Found <Decomp> block:  ", decompNames[i]);
    TxHierAttribSetIntDbl attribs = tas.getAttrib(decompNames[i]);
    decompAttribs.push_back(attribs);
  }
}

template <class FLOATTYPE, size_t  NDIM>
void PsDecompHldr<FLOATTYPE, NDIM>::build(PsNamedObject* inOwner) {

  this->dbprt("PsDecompHldr::build() ");

  for (size_t i=0; i<decompNames.size(); ++i) {

    // For each Decomp look for kind
    std::string kind = decompAttribs[i].getString("kind");

    // Makermap returns pointer to a derived object of type PsDecomp
    PsDecompBase<FLOATTYPE, NDIM>* decompPtr =
        TxMakerMap< PsDecompBase<FLOATTYPE, NDIM> >::getNew(kind);

    // Set parameters and attributes for particular decomp and build
    decompPtr->setName(decompNames[i]);
    decompPtr->setDomainSingletons( this->getDomainSingletons() );
    decompPtr->setAttrib(decompAttribs[i]);

    // New name registration and build
    decompPtr->setOwner(inOwner);
    inOwner->makeAvail(decompPtr, decompNames[i]);
    decompPtr->build();

    // Push pointer into decomps structure
    decomps.push_back(decompPtr);
  }

}

// Instantiate base decomp holder classes
template class PsDecompHldr<float, 1>;
template class PsDecompHldr<float, 2>;
template class PsDecompHldr<float, 3>;

template class PsDecompHldr<double, 1>;
template class PsDecompHldr<double, 2>;
template class PsDecompHldr<double, 3>;

