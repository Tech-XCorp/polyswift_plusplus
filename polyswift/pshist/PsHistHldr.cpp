/**
 * @file    PsHistHldr.cpp
 *
 * @brief   Base class owning instances of PsHistoryBase
 *
 * @version $Id: PsHistHldr.cpp 6319 2007-01-24 22:39:46Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// pshist includes
#include <PsHistHldr.h>

// txbase includes
#include <TxMakerMap.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsHistHldr<FLOATTYPE, NDIM>::PsHistHldr() {
  this->setName("History");
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsHistHldr<FLOATTYPE, NDIM>::~PsHistHldr() {

  for (size_t i=0; i<histories.size(); ++i) {
    delete histories[i];
  }
  histories.clear();
}

template <class FLOATTYPE, size_t NDIM>
void PsHistHldr<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Look for History blocks in input file
  historyNames = tas.getNamesOfType("History");

  // Set the number of History blocks found
  numHistories = historyNames.size();

  // Put each history attribute set into local list
  for (size_t i=0; i<numHistories; ++i) {

    this->pprt("Found <History> block: ", historyNames[i]);
    TxHierAttribSetIntDbl attribs = tas.getAttrib(historyNames[i]);
    historyAttribs.push_back(attribs);
  }
}

//
template <class FLOATTYPE, size_t  NDIM>
void PsHistHldr<FLOATTYPE, NDIM>::buildData(PsNamedObject* inOwner) {

  for (size_t i=0; i<historyNames.size(); ++i) {

    // For each History block look for kind of diagnostic
    std::string kind = historyAttribs[i].getString("kind");

    // Have makermap return a pointer to a PsHistory object
    PsHistoryBase<FLOATTYPE, NDIM>* histPtr =
        TxMakerMap< PsHistoryBase<FLOATTYPE, NDIM> >::getNew(kind);

    // Set parameters and attributes for particular history
    histPtr->setName(historyNames[i]);
    histPtr->setDomainSingletons( this->getDomainSingletons() );
    histPtr->setAttrib(historyAttribs[i]);

    // New name registration and build
    histPtr->setOwner(inOwner);
    inOwner->makeAvail(histPtr, historyNames[i]);
    histPtr->buildData();

    // Push pointer into histories structure
    histories.push_back(histPtr);
  }
}

//
//
template <class FLOATTYPE, size_t  NDIM>
void PsHistHldr<FLOATTYPE, NDIM>::buildSolvers() {

  for (size_t i=0; i<historyNames.size(); ++i)
    histories[i]->buildSolvers();
}

//
//
template <class FLOATTYPE, size_t  NDIM>
void PsHistHldr<FLOATTYPE, NDIM>::initialize() {
}

template <class FLOATTYPE, size_t  NDIM>
void PsHistHldr<FLOATTYPE, NDIM>::update(double t) {

  // Local time variables
  size_t updateTst;

  // Update the holder
  PsDynObj<FLOATTYPE, NDIM>::update(t);

  // Loop over vector of history pointers, check
  //   start time, update period, finish time
  //   and call each update() method
  typename std::vector< PsHistoryBase<FLOATTYPE, NDIM>* >::iterator ihist;
  for (ihist = histories.begin(); ihist != histories.end(); ++ihist) {

    // update the simulation time step
    (*ihist)->updateSimTimeStep(t);
    updateTst = (size_t)t % ( (*ihist)->updatePeriod );

    if ( t > (*ihist)->tstepsBeforeStart )
      if (updateTst == 0) (*ihist)->update(t);
  }

}

//
// Dump the data
//
template <class FLOATTYPE, size_t  NDIM>
void PsHistHldr<FLOATTYPE, NDIM>::dump(TxIoBase* txIoPtr) {

// Don't dump if doing a start dump
  size_t seqNumber = PsDynObjBase::getCurrDomainStep();

  if (numHistories == 0) return;

// Create/open filename
  std::string fileName;
  fileName = txIoPtr->getDumpFileName("History", false);
  bool histFileExists = txIoPtr->fileExists(fileName);
  TxIoNodeType fnode;

// No dump at start
  if (seqNumber != 0) {

    if (!histFileExists) fnode = txIoPtr->createFile(fileName);
    else                 fnode = txIoPtr->openFile(fileName, "rw");

// Define iterator for histories and loop over VpHistory-ies
    for (HistIter ihist = histories.begin(); ihist != histories.end(); ++ihist) {

// sync data on all processors
      (*ihist)->sync();

// Check for a zero length history and node 0 dumps
      size_t histLength = (*ihist)->getHistDataLength();
      if (histLength != 0) {
        (*ihist)->dump(txIoPtr, fnode);
      }

// Clear the history data
      (*ihist)->clearData();
    }

// Node 0 closes the diagnostics history file
    txIoPtr->closeFile(fnode);

  } // seqNumber check

}

// Instantiate base history holder classes
template class PsHistHldr<float, 1>;
template class PsHistHldr<float, 2>;
template class PsHistHldr<float, 3>;

template class PsHistHldr<double, 1>;
template class PsHistHldr<double, 2>;
template class PsHistHldr<double, 3>;
