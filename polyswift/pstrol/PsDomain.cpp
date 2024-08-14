/**
 * @file    PsDomain.cpp
 *
 * @brief   To setup the simulation on individual processors.
 *
 * @version $Id: PsDomain.cpp 8244 2007-09-11 18:47:22Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// std includes
#include <string>

// psstd includes
#include <PsRandom.h>

// mpi includes
#ifdef HAVE_MPI
#define MPICH_IGNORE_CXX_SEEK
#include <mpi.h>
#endif

// pstrol includes
#include <PsDomain.h>

// constructor
template <class FLOATTYPE, size_t NDIM>
PsDomain<FLOATTYPE, NDIM>::PsDomain() : PsDomainBase() {

/// Instantiate the dom maker maps
  PsDomMakerMaps<FLOATTYPE, NDIM>::getInstance();

  int myRank = 0;
  #ifdef HAVE_MPI
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
  #endif
  thisRank = myRank;

  #ifdef HAVE_MPI
  comm = new TxMpiBase();
  #else
  comm = new TxSelfBase();
  #endif

  dbStatus = PSDB_OFF;
}

// destructor
template <class FLOATTYPE, size_t NDIM>
PsDomain<FLOATTYPE, NDIM>::~PsDomain() {

  // Cleanup comm for txio
  delete comm;
}

// setup the domain
template <class FLOATTYPE, size_t NDIM>
void PsDomain<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tha,
                                          size_t restoreNum,
                                          std::string runName) {

  baseName = runName;   // Set basename
  domainSettings = tha; // Store later use throughout PsDomain

  // Debug flag
  if (tha.hasString("printdebug") ) {
    std::string dbPrint = tha.getString("printdebug");
    if (dbPrint == "on") dbStatus = PSDB_ON;
  }

  // Default grid to be set in domain singletons
  if (tha.hasString("defaultGrid") ) {
    defaultGrid = tha.getString("defaultGrid");
  }
  else {
    TxDebugExcept tde("PsDomain::setAttrib: default grid (domSings) not set");
    throw tde;
  }

  // Initialize the sequence number to the dump number
  // Initialize domain step to the dump number
  //  seqNumber = 0;
  seqNumber = restoreNum;
  PsDynObjBase::setCurrDomainStep(restoreNum);

  if (dbStatus==PSDB_ON) {
    if (thisRank == 0) {
      std::cout << "Entering PsDomain::setAttrib()"
                << std::endl;
      std::cout << "Restore number in PsDomain::setAttrib = "
                << restoreNum
                << std::endl;
      std::cout << "PsDomain::setAttrib current domain step = "
                << PsDynObjBase::getCurrDomainStep()
                << std::endl;
    }
  }

  // Number of saddle-point timesteps
  // This is set again (main sets also) from the attribute set
  if (domainSettings.hasOption("nsteps") ) {
    nsteps = domainSettings.getOption("nsteps");
    if (dbStatus==PSDB_ON) {
      std::cout << "nsteps = " << nsteps << std::endl;
    }
  }
  PsDynObjBase::setNsteps(nsteps);

  // Number of steps between "big" data output dumps
  if (domainSettings.hasOption("dumpPeriodicity") ) {
    dumpPeriodicity = domainSettings.getOption("dumpPeriodicity");
    if (dbStatus==PSDB_ON) {
      std::cout << "dump period = " << dumpPeriodicity
                << std::endl;
    }
  }

  // Seed (global for now)
  if (domainSettings.hasOption("randomSeed") ) {
    randomSeed = domainSettings.getOption("randomSeed");
    if (dbStatus==PSDB_ON) {
      std::cout << "random seed = " << randomSeed
                << std::endl;
    }
  }

// ***************************************************
// The comm must be set and singletons set so prt
// methods in dyn base classes can work.
// ***************************************************

  // Set up comms completely
  commHldr.setAttrib(domainSettings);
  commHldr.build(this);

  // Get Comm method pointer
  PsCommBase<FLOATTYPE, NDIM>* commPtr =
    PsNamedObject::getObject<PsCommBase<FLOATTYPE, NDIM> >("defaultComm");
  domSings.setCommBase(commPtr);
  domSings.setDomRank(commPtr->getRank());
// ***************************************************

// ***************************************************
// The following  objects must be completely built
//    before domain singletons can be set
// ***************************************************

  // Set up grids
  gridHldr.setDomainSingletons(domSings);
  gridHldr.setAttrib(domainSettings);
  gridHldr.buildData(this);

  // Set default singletons object before decomps so decomp
  // can get global info
  PsGridBase<FLOATTYPE, NDIM>* gridObjPtr =
    PsNamedObject::getObject<PsGridBase<FLOATTYPE, NDIM> >(defaultGrid);
  domSings.setGridBase(gridObjPtr);

  // Set up decomps
  decompHldr.setDomainSingletons(domSings);
  decompHldr.setAttrib(domainSettings);
  decompHldr.build(this);

  // Finish grid build now that decomp is set
  gridHldr.buildSolvers();

  // Set name
  domSings.setOutputFilePrefix(runName);

  // for safety
  commPtr->barrier();

// ********************************************************
//  Domain singletons set... so other objects' setup
//  can now be started
// ********************************************************

  fftHldr.setDomainSingletons(domSings);
  polymerHldr.setDomainSingletons(domSings);
  solventHldr.setDomainSingletons(domSings);
  physFieldHldr.setDomainSingletons(domSings);
  effHamilHldr.setDomainSingletons(domSings);
  bndryHldr.setDomainSingletons(domSings);
  histHldr.setDomainSingletons(domSings);

  fftHldr.setAttrib(domainSettings);
  polymerHldr.setAttrib(domainSettings);
  solventHldr.setAttrib(domainSettings);
  physFieldHldr.setAttrib(domainSettings);
  effHamilHldr.setAttrib(domainSettings);
  bndryHldr.setAttrib(domainSettings);
  histHldr.setAttrib(domainSettings);

  if (thisRank == 0) std::cout << "\n" << std::endl;
}

//
// build Data
//
template <class FLOATTYPE, size_t NDIM>
void PsDomain<FLOATTYPE, NDIM>::buildData() {

  // Setting random seed on each proc and
  //  setting global seed for all procs for which seed is sync-ed.
  if (thisRank == 0)
    std::cout << "Setting local random seed" << std::endl;
  psSrandom(randomSeed+thisRank);
  psSrandomGlobal(randomSeed);

// *******************************************************************
// SWS: this check should be re-done... grids should
// SWS: have comm and do this check...
// SWS: but only for defaults?

  size_t procRank  = domSings.getCommBase().getRank();
  size_t numRanks  = domSings.getCommBase().getSize();
  size_t xCellNums = domSings.getGridBase().getNumCellsGlobal()[0];
  size_t yCellNums = domSings.getGridBase().getNumCellsGlobal()[1];
  if (dbStatus==PSDB_ON) {
    if (thisRank == 0) {
      std::cout << "numRanks  = " << numRanks  << std::endl;
      std::cout << "xCellNums = " << xCellNums << std::endl;
    }
  }
  if ( xCellNums % numRanks != 0 ) {
    TxDebugExcept tde("PsDomain::buildData: ");
    tde << "number of x-cells not evenly divisible by # of procs";
    throw tde;
  }
  if ( yCellNums % numRanks != 0 ) {
    TxDebugExcept tde("PsDomain::buildData: ");
    tde << "number of y-cells not evenly divisible by # of procs";
    throw tde;
  }
// *******************************************************************

  // ***********************************
  //            Build objects
  // ***********************************
  fftHldr.buildData(this);

  // Needed for name lookup in hldr
  physFieldHldr.setOwner(this);
  this->makeAvail(&physFieldHldr);
  physFieldHldr.buildData(this);

  bndryHldr.buildData(this);
  polymerHldr.buildData(this);
  solventHldr.buildData(this);
  effHamilHldr.buildData(this);

  // Needed for name lookup in hldr
  histHldr.setOwner(this);
  this->makeAvail(&histHldr);
  histHldr.buildData(this);
  // *******************************

  // Build IO object
  // Create/set io object parameters
  txIoPtr = new TxHdf5Base(comm);
  txIoPtr->setBaseName(baseName);
  txIoPtr->setDumpNo(seqNumber);
  //  txIoPtr->setDumpNoFlag(true); // for polyswift
}

//
// build Solvers
//
template <class FLOATTYPE, size_t NDIM>
void PsDomain<FLOATTYPE, NDIM>::buildSolvers() {

  fftHldr.buildSolvers();
  physFieldHldr.buildSolvers();
  bndryHldr.buildSolvers();
  polymerHldr.buildSolvers();
  solventHldr.buildSolvers();
  effHamilHldr.buildSolvers();
  histHldr.buildSolvers();
}

//
// initialize the domain:
//
template <class FLOATTYPE, size_t NDIM>
void PsDomain<FLOATTYPE, NDIM>::initialize() {

  // Kludge for time .. SWS: can get rid of ??? 02/19/10
  size_t tmp = 0;
  PsDynObjBase::setCurrDomainStep(tmp);

  // Initialize all total physical fields from "Entities"
  // (ie monomer densities, charge densities, stress etc...)
  physFieldHldr.initialize();

  // Initialize boundaries
  bndryHldr.initialize();
}

//
// restore the domain:
//
template <class FLOATTYPE, size_t NDIM>
void PsDomain<FLOATTYPE, NDIM>::restore() {

// Update dump timestep for io object
  double tt = (double)PsDynObjBase::getCurrDomainStep();
  int tstep = (int)tt;
  txIoPtr->setDumpNo(tstep);

// Restore all total physical fields from "Entities"
// (ie monomer densities, charge densities, stress etc...)
  physFieldHldr.restore(txIoPtr);

// Restore boundaries
  bndryHldr.restore(txIoPtr);

// Bump the sequence once (keep this in new txbasIO)
  bumpDumpNum();
  txIoPtr->setDumpNo(seqNumber);
}

//
// update the domain:
//
template <class FLOATTYPE, size_t NDIM>
void PsDomain<FLOATTYPE, NDIM>::update(double t) {

  // Update the static update-step parameter
  //  PsDynObjBase::bumpCurrDomainStep();

  // Update the static update-step parameter
  // Convert t directly
  size_t itt = (size_t)t;
  PsDynObjBase::setCurrDomainStep(itt);

  double tt = (double)PsDynObjBase::getCurrDomainStep();
  double tnext = tt + 1.0;

  // Update dump timestep for io object
  int tstep = (int)tt;
  txIoPtr->setDumpNo(tstep);

  // Clear density values (observables) in PhysFields
  // they are "counters" for dens contribs from "Entities" below
  // Careful because density for boundaries set above....
  // Re-calculates member data eg mono dens aver if components
  // volume fractions change
  physFieldHldr.update(t);

  // Update boundary
  bndryHldr.update(t);

  // Update states of all transformed monomer "Entities"
  // SWS: Will be slightly off of update for phys MonoFields
  solventHldr.update(t);
  polymerHldr.update(t);

  // Update all SCFT model holders
  effHamilHldr.update(t);

  // Update nanoparticles by updating all ptclGroup info
  bndryHldr.update(tnext);

  // Update Histories
  histHldr.update(t);
}

// dump the domain:
template <class FLOATTYPE, size_t NDIM>
void PsDomain<FLOATTYPE, NDIM>::dump() {

// Setting sequence number for IO object
//  txIoPtr->setDumpNo(seqNumber);

// Dump all histories
  histHldr.dump(txIoPtr);

// Dump all total physical fields from "Entities"
// (ie monomer densities, charge densities, stress etc...)
  physFieldHldr.dump(txIoPtr);
  // SWS: chombo  physFieldHldr.dump(chomboIo?);

// Dump all fields from nanoparticle types, particle
// template field, gradient particle field walls, etc
  bndryHldr.dump(txIoPtr);

// Setting seq number for IO object and increment the sequence num
  bumpDumpNum();
  txIoPtr->setDumpNo(seqNumber);
}

//
// Instantiate the templates
//
template class PsDomain<float, 1>;
template class PsDomain<float, 2>;
template class PsDomain<float, 3>;

template class PsDomain<double, 1>;
template class PsDomain<double, 2>;
template class PsDomain<double, 3>;
