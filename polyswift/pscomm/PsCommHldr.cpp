/**
 *
 * @file    PsCommHldr.cpp
 *
 * @brief   Base class owning instances of input/output object(s)
 *
 * @version $Id: PsCommHldr.cpp 6319 2007-01-24 22:39:46Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// pscomm includes
#include <PsCommHldr.h>

// txbase includes
#include <TxMakerMap.h>

template <class FLOATTYPE, size_t NDIM>
void PsCommHldr<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Look for Comm blocks in input file
  commNames = tas.getNamesOfType("Comm");

  // Set the number of Comm blocks found
  numComms = commNames.size();

  // Put each comm attribute set into local list
  for (size_t i=0; i<numComms; ++i) {
    TxHierAttribSetIntDbl attribs = tas.getAttrib(commNames[i]);
    commAttribs.push_back(attribs);
  }
}

// Only build method
template <class FLOATTYPE, size_t  NDIM>
void PsCommHldr<FLOATTYPE, NDIM>::build(PsNamedObject* inOwner) {

  for (size_t i=0; i<commNames.size(); ++i) {

    // For each Comm look for kind
    std::string kind = commAttribs[i].getString("kind");

    // Makermap returns pointer to a derived object of type PsComm
    PsCommBase<FLOATTYPE, NDIM>* commPtr =
      TxMakerMap< PsCommBase<FLOATTYPE, NDIM> >::getNew(kind);
    if (!commPtr) {
      TxDebugExcept tde("PsCommHldr::buildData: Comm pointer not set");
      throw tde;
    }

    // Set parameters and attributes for particular comm and build
    commPtr->setName(commNames[i]);
    commPtr->setAttrib(commAttribs[i]);

    // New name registration and build
    commPtr->setOwner(inOwner);
    inOwner->makeAvail(commPtr, commNames[i]);

    // Push pointer into comms structure
    comms.push_back(commPtr);
  }
}

template <class FLOATTYPE, size_t  NDIM>
void PsCommHldr<FLOATTYPE, NDIM>::dump() {

  // Loop over vector of comm pointers and call dump()
  typename std::vector< PsCommBase<FLOATTYPE, NDIM>* >::iterator i;
  for (i = comms.begin(); i != comms.end(); ++i) {
    //    std::cout << "Dumping comm " << (*i)->getName() << std::endl;
    // (*i)->dump();
  }
}

// Instantiate base comm holder classes
template class PsCommHldr<float, 1>;
template class PsCommHldr<float, 2>;
template class PsCommHldr<float, 3>;

template class PsCommHldr<double, 1>;
template class PsCommHldr<double, 2>;
template class PsCommHldr<double, 3>;

