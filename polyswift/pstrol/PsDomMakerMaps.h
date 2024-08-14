/**
 *
 * @file    PsDomMakerMaps.h
 *
 * @brief   Interface to the PsDomMakerMaps class.
 *
 * @version $Id: PsDomMakerMaps.h 8263 2007-09-13 14:36:29Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_DOM_MAKER_MAPS_H
#define PS_DOM_MAKER_MAPS_H

// std includes
#include <vector>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// system includes
#include <iostream>
#include <fstream>
#include <sstream>

// txbase
#include <TxHierAttribSetTmpl.h>

// psbase includes
#include <PsFieldMakerMap.h>

// psphysf includes
#include <PsPhysFieldMakerMap.h>

// psdecomp includes
#include <PsDecompMakerMap.h>

// pscomm includes
#include <PsCommMakerMap.h>

// psgrid includes
#include <PsGridMakerMap.h>

// psfft includes
#include <PsFFTMakerMap.h>

// pshist includes
#include <PsHistoryMakerMap.h>

// pseffhamil includes
#include <PsEffHamilMakerMap.h>
#include <PsInteractionMakerMap.h>
#include <PsUpdaterMakerMap.h>

// pspolymer includes
#include <PsPolymerMakerMap.h>
#include <PsBlockMakerMap.h>

// pssolvent includes
#include <PsSolventMakerMap.h>

// psstd includes
#include <PsSTFuncMakerMap.h>

// psbndry includes
#include <PsBndryMakerMap.h>
#include <PsBndryDataBaseMakerMap.h>

/**
 * A PsDomMakerMaps object holds one instance of each of the maker
 * map objects.  Only one  of these should be created.
 */
template <class FLOATTYPE, size_t NDIM>
class PsDomMakerMaps {

  public:

    static PsDomMakerMaps<FLOATTYPE, NDIM>& getInstance(){
      if (domMakerMapsPtr.get() == NULL) domMakerMapsPtr =
          std::unique_ptr<PsDomMakerMaps<FLOATTYPE, NDIM> >(
              new PsDomMakerMaps<FLOATTYPE, NDIM>());
      return *domMakerMapsPtr;
    }

/**
 * Destructor
 */
    virtual ~PsDomMakerMaps(){}

  protected:

  private:

/**
 * Default constructor
 */
    PsDomMakerMaps() {}

/// Pointer to singleton
    static std::unique_ptr<PsDomMakerMaps<FLOATTYPE, NDIM> > domMakerMapsPtr;

/// No copy constructor
    PsDomMakerMaps(const PsDomMakerMaps<FLOATTYPE, NDIM>& vds) = delete;

/// No assignment
    PsDomMakerMaps<FLOATTYPE, NDIM>& operator=(
        const PsDomMakerMaps<FLOATTYPE, NDIM>& vds) = delete;

/// physical field maker map
    PsPhysFieldMakerMap<FLOATTYPE, NDIM> physFieldMakerMap;

/// Solvent maker map
    PsSolventMakerMap<FLOATTYPE, NDIM> solventMakerMap;

/// polymer maker map
    PsPolymerMakerMap<FLOATTYPE, NDIM> polymerMakerMap;

/// block maker map
    PsBlockMakerMap<FLOATTYPE, NDIM> blockMakerMap;

/// boundary maker map
    PsBndryMakerMap<FLOATTYPE, NDIM> bndryMakerMap;

/// boundary DB maker map
    PsBndryDataBaseMakerMap<FLOATTYPE, NDIM> bndryDataMakerMap;

/// field base maker map
    PsFieldMakerMap<FLOATTYPE, NDIM> fieldMakerMap;

/// comm maker map
    PsCommMakerMap<FLOATTYPE, NDIM> commMakerMap;

/// decomp maker map
    PsDecompMakerMap<FLOATTYPE, NDIM> decompMakerMap;

/// grid maker map
    PsGridMakerMap<FLOATTYPE, NDIM> gridMakerMap;

/// fft maker map
    PsFFTMakerMap<FLOATTYPE, NDIM> fftMakerMap;

/// History maker map
    PsHistoryMakerMap<FLOATTYPE, NDIM> historyMakerMap;

/// STFunc maker map
    PsSTFuncMakerMap<FLOATTYPE, NDIM> stfuncMakerMap;

/// EH maker map
    PsEffHamilMakerMap<FLOATTYPE, NDIM> ehMakerMap;

/// Interaction maker map
    PsInteractionMakerMap<FLOATTYPE, NDIM> interactionMakerMap;

/// Updater maker map
    PsUpdaterMakerMap<FLOATTYPE, NDIM> updaterMakerMap;

};

template <class FLOATTYPE, size_t NDIM>
std::unique_ptr<PsDomMakerMaps<FLOATTYPE, NDIM> >
PsDomMakerMaps<FLOATTYPE, NDIM>::domMakerMapsPtr;

#endif // PS_DOM_MAKER_MAPS_H

