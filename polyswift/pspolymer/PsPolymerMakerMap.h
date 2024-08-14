/**
 * @file    PsPolymerMakerMap.h
 *
 * @brief   Interface for class to register all polymer makers
 *
 * @version $Id: PsPolymerMakerMap.h 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_POLYMER_MAKER_MAP_H
#define PS_POLYMER_MAKER_MAP_H

#include <TxMaker.h>

// psphysf includes
#include <PsPolymer.h>

/**
 * Class that holds all of the makers of fluids
 */
template <class FLOATTYPE, size_t NDIM>
class PsPolymerMakerMap {

  public:

/**
 * Constructor: create all of the makers
 */
    PsPolymerMakerMap();

/**
 * Destructor
 */
    virtual ~PsPolymerMakerMap();

};
#endif // PS_POLYMER_MAKER_MAP_H
