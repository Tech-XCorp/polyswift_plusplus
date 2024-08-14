/**
 *
 * @file    PsPhysFieldHldr.cpp
 *
 * @brief   Base class owning instances of base class PsPhysField
 *
 * @version $Id: PsPhysFieldHldr.cpp 6319 2007-01-24 22:39:46Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// psphysf includes
#include <PsPhysFieldHldr.h>

// txbase includes
#include <TxMakerMap.h>

template <class FLOATTYPE, size_t NDIM>
void PsPhysFieldHldr<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Look for PhysField blocks in input file
  physFieldNames = tas.getNamesOfType("PhysField");

  // Set the number of PhysField blocks found
  numPhysFields = physFieldNames.size();

  // Put each physField attribute set into local list
  for (size_t i=0; i<numPhysFields; ++i) {

    this->pprt("Found <PhysField> block: ", physFieldNames[i]);
    TxHierAttribSetIntDbl attribs = tas.getAttrib(physFieldNames[i]);
    physFieldAttribs.push_back(attribs);
  }
}

//
//
template <class FLOATTYPE, size_t  NDIM>
void PsPhysFieldHldr<FLOATTYPE, NDIM>::buildData(PsNamedObject* inOwner) {

  this->dbprt("PsPhysFieldHldr::buildData() entered");

  // Loop over physField names and build
  for (size_t i=0; i<physFieldNames.size(); ++i) {

    // For each PhysField look for kind
    std::string kind = physFieldAttribs[i].getString("kind");

    // Makermap gets ptr to a derived object of type PsPhysField
    PsPhysField<FLOATTYPE, NDIM>* physFieldPtr =
      TxMakerMap< PsPhysField<FLOATTYPE, NDIM> >::getNew(kind);
    if (!physFieldPtr) {
      TxDebugExcept tde("PsPhysFieldHldr::buildData: PhysField ptr not set");
      throw tde;
    }

    // Set params and attribs for a physField and build
    physFieldPtr->setName(physFieldNames[i]);
    physFieldPtr->setDomainSingletons( this->getDomainSingletons() );
    physFieldPtr->setAttrib(physFieldAttribs[i]);

    // New name registration and build
    physFieldPtr->setOwner(inOwner);
    inOwner->makeAvail(physFieldPtr, physFieldNames[i]);
    physFieldPtr->buildData();

    // Push pointer into physFields structure
    physFields.push_back(physFieldPtr);
  }
}

template <class FLOATTYPE, size_t  NDIM>
void PsPhysFieldHldr<FLOATTYPE, NDIM>::buildSolvers() {

  this->dbprt("PsPhysFieldHldr::buildSolvers()");

  // Loop over vector of physField pointers and call buildSolvers()
  for (iphys = physFields.begin(); iphys != physFields.end(); ++iphys) {
    this->dbprt("  buildSolvers for physField ", (*iphys)->getName());
    (*iphys)->buildSolvers();
  }

}

template <class FLOATTYPE, size_t  NDIM>
void PsPhysFieldHldr<FLOATTYPE, NDIM>::initialize() {

  this->dbprt("PsPhysFieldHldr::initialize() ");
  check();

  // Loop over vector of physField pointers and call initialize()
  for (iphys = physFields.begin(); iphys != physFields.end(); ++iphys) {
    this->dbprt("initialize for physField ", (*iphys)->getName());
    (*iphys)->initialize();
  }
}

//
// Clear density values (observables) in PhysFields
// as they are "counters" for density contributions
// Monomer densities update other data members
//
template <class FLOATTYPE, size_t  NDIM>
void PsPhysFieldHldr<FLOATTYPE, NDIM>::update(double t) {

  this->dbprt("PsPhysFieldHldr::update(t) ");
  check();

  // Loop over vector of physField pointers and call update()
  for (iphys = physFields.begin(); iphys != physFields.end(); ++iphys) {
    this->dbprt("update for physField ", (*iphys)->getName());
    (*iphys)->update(t);
  }
}

template <class FLOATTYPE, size_t  NDIM>
void PsPhysFieldHldr<FLOATTYPE, NDIM>::restore(TxIoBase* txIoPtr) {

  this->dbprt("PsPhysFieldHldr::restore() ");
  check();

  // Loop over vector of physField pointers and call restore()
  for (iphys = physFields.begin(); iphys != physFields.end(); ++iphys) {
    this->dbprt("restore for physField ", (*iphys)->getName());
    (*iphys)->restore(txIoPtr);
  }
}

template <class FLOATTYPE, size_t  NDIM>
void PsPhysFieldHldr<FLOATTYPE, NDIM>::dump(TxIoBase* txIoPtr) {

  // Dump the holder (not needed for now)
  PsDynObj<FLOATTYPE, NDIM>::dump();

  this->dbprt("PsPhysFieldHldr::dump() ");

  // Get simulation timestep
  size_t timeStep = PsDynObjBase::getCurrDomainStep();

  // Loop over vector of physField pointers and call dump()
  for (iphys = physFields.begin(); iphys != physFields.end(); ++iphys) {

    // Decide whether to dump
    bool madeOneDump   = (*iphys)->hasMadeOneDump();
    bool onlyFirstDump = (*iphys)->getOnlyFirstDump();
    if ( (onlyFirstDump)&&(madeOneDump) ) {
      continue;
    }
    else {
      this->dbprt("Dumping physField ", (*iphys)->getName());
      (*iphys)->dump(txIoPtr);
    }

  }
}

//
// Driver for check routines and printing
// out results
//
template <class FLOATTYPE, size_t  NDIM>
void PsPhysFieldHldr<FLOATTYPE, NDIM>::check() {

  FLOATTYPE vfTot = checkListsVolFrac(false);
  //FLOATTYPE vfTot = checkListsVolFrac(true);
  checkSum(vfTot);
}

//
// Private helper method:
// Check member name lists and component volume frac.
//
template <class FLOATTYPE, size_t  NDIM>
FLOATTYPE PsPhysFieldHldr<FLOATTYPE, NDIM>::checkListsVolFrac(bool prtFlag) {

  // Loop over vector of physField pointers and call initialize()
  std::vector<std::string> interactionNames;
  std::vector<std::string> blockNames;
  std::vector<std::string> solventNames;
  std::string psName;

  PsBlockBase<FLOATTYPE, NDIM>* blockObjPtr;
  PsPolymer<FLOATTYPE, NDIM>*   polymerObjPtr;
  PsSolvent<FLOATTYPE, NDIM>*   solventObjPtr;

  FLOATTYPE totMonoAvg = 0.0;

  for (iphys = physFields.begin(); iphys != physFields.end(); ++iphys) {

    psName           = (*iphys)->getName();
    interactionNames = (*iphys)->getInteractionNames();
    blockNames       = (*iphys)->getBlockNames();
    solventNames     = (*iphys)->getSolventNames();

    if (prtFlag) this->pprt("\n <PhysField ", psName, " > contains:");

    for (size_t n=0; n<interactionNames.size(); ++n)
      if (prtFlag) this->pprt("    interaction -> ", interactionNames[n]);

    for (size_t n=0; n<solventNames.size(); ++n) {
      solventObjPtr =
        PsNamedObject::getObject<PsSolvent<FLOATTYPE, NDIM> >(solventNames[n]);
      FLOATTYPE volfrac = solventObjPtr->getVolfrac();
      if (prtFlag) {
        this->pprt("    solvent -> ", solventNames[n]);
        this->pprt("    volfrac =  ", volfrac);
      }
      totMonoAvg += volfrac;
    } // solvents

    for (size_t n=0; n<blockNames.size(); ++n) {

      std::string blkName = blockNames[n];
      blockObjPtr =
        PsNamedObject::getObject<PsBlockBase<FLOATTYPE, NDIM> >(blkName);
      FLOATTYPE lenfrac = blockObjPtr->getLengthFrac();
      FLOATTYPE weight  = blockObjPtr->getWeight();  // 1.0 for monodisperse
      std::string polyName = blockObjPtr->getPolymerName();
      polymerObjPtr =
        PsNamedObject::getObject<PsPolymer<FLOATTYPE, NDIM> >(polyName);
      FLOATTYPE volfrac = polymerObjPtr->getVolfrac();
      FLOATTYPE lenWt = blockObjPtr->getLengthWeight();

      totMonoAvg += (lenWt*lenfrac*volfrac);
      if (prtFlag) {
        this->pprt("    block   -> ", blockNames[n]);
        this->pprt("    lenfrac      =  ", lenfrac);
        this->pprt("    weight       =  ", weight);
        this->pprt("    lengthWeight =  ", weight);
        this->pprt("    volfrac      =  ", volfrac);
        this->pprt("    totMonoAvg   =  ", totMonoAvg);
        this->pprt("\n");
      }
    } // blocks

  } // loop on PhysFields

  return totMonoAvg;
}

//
// Checksum
//
template <class FLOATTYPE, size_t  NDIM>
void PsPhysFieldHldr<FLOATTYPE, NDIM>::checkSum(FLOATTYPE tot) {

  FLOATTYPE tolerance = 0.00001;
  FLOATTYPE volTarget = 1.0;
  FLOATTYPE tmp;

  if ( fabs(tot - volTarget) > tolerance) {
    tmp = checkListsVolFrac(true); // prints out previous calculations
    TxDebugExcept tde("PsPhysFieldHldr::checkListsVolFrac: |totalVolFrac - 1.0| ");
    tde << fabs(tot - volTarget) << " > tolerance \n";
    tde << "Total volume fraction of all components does not = 1.0 \n";
    tde << "  either the volume fraction are not set correctly in the pre file \n";
    tde << "  or the STFunc that vary the volfrac parameters need to be adjusted \n";
    throw tde;
  }
}

/*
//
// Private helper method
// Check volume fraction totals
//
template <class FLOATTYPE, size_t  NDIM>
void PsPhysFieldHldr<FLOATTYPE, NDIM>::checkVolFrac() {

  // Loop over vector of physField pointers and call initialize()
  std::vector<std::string> blockNames;
  std::vector<std::string> solventNames;
  std::string psName;

  PsBlockBase<FLOATTYPE, NDIM>* blockObjPtr;
  PsPolymer<FLOATTYPE, NDIM>*   polymerObjPtr;
  PsSolvent<FLOATTYPE, NDIM>*   solventObjPtr;

  FLOATTYPE totMonoAvg = 0.0;

  for (iphys = physFields.begin(); iphys != physFields.end(); ++iphys) {

    psName       = (*iphys)->getName();
    blockNames   = (*iphys)->getBlockNames();
    solventNames = (*iphys)->getSolventNames();

    this->dbprt("\n <PhysField ", psName, " > contains:");

    for (size_t n=0; n<solventNames.size(); ++n) {
      solventObjPtr =
        PsNamedObject::getObject<PsSolvent<FLOATTYPE, NDIM> >(solventNames[n]);
      FLOATTYPE volfrac = solventObjPtr->getVolfrac();
      this->dbprt("    solvent -> ", solventNames[n]);
      this->dbprt("    volfrac =  ", volfrac);
      totMonoAvg += volfrac;
    }

    for (size_t n=0; n<blockNames.size(); ++n) {
      blockObjPtr =
        PsNamedObject::getObject<PsBlockBase<FLOATTYPE, NDIM> >(blockNames[n]);
      FLOATTYPE lenfrac = blockObjPtr->getLengthFrac();
      std::string polyName = blockObjPtr->getPolymerName();
      polymerObjPtr =
        PsNamedObject::getObject<PsPolymer<FLOATTYPE, NDIM> >(polyName);
      FLOATTYPE volfrac = polymerObjPtr->getVolfrac();
      this->dbprt("    block   -> ", blockNames[n]);
      this->dbprt("    lenfrac =  ", lenfrac);
      this->dbprt("    volfrac =  ", volfrac);
      totMonoAvg += (lenfrac*volfrac);
    }
  } // loop on PhysFields

  //
  // Checksum
  //
  FLOATTYPE tolerance = 0.00001;
  FLOATTYPE volTarget = 1.0;
  if ( fabs(totMonoAvg - volTarget) > tolerance) {
    checkLists();
    TxDebugExcept tde("PsPhysFieldHldr::checkVolFrac: |totalVolFrac - 1.0| ");
    tde << fabs(totMonoAvg - volTarget) << " > tolerance";
    throw tde;
  }

}
*/

// Instantiate base physField holder classes
template class PsPhysFieldHldr<float, 1>;
template class PsPhysFieldHldr<float, 2>;
template class PsPhysFieldHldr<float, 3>;

template class PsPhysFieldHldr<double, 1>;
template class PsPhysFieldHldr<double, 2>;
template class PsPhysFieldHldr<double, 3>;
