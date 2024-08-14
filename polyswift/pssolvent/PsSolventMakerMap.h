/**
 *
 * @file    PsSolventMakerMap.h
 *
 * @brief   Interface for class to register all solvent makers
 *
 * @version $Id: PsSolventMakerMap.h 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_SOLVENT_MAKER_MAP_H
#define PS_SOLVENT_MAKER_MAP_H

#include <TxMaker.h>

// psbase includes
#include <PsSolvent.h>

template <class FLOATTYPE, size_t NDIM>
class PsSolventMakerMap {

  public:

/**
 * Constructor: create all of the makers
 */
    PsSolventMakerMap();

/**
 * Destructor
 */
    virtual ~PsSolventMakerMap();

};

#endif // PS_SOLVENT_MAKER_MAP_H
