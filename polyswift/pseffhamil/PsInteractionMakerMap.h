/**
 *
 * @file    PsInteractionMakerMap.h
 *
 * @brief   Interface for class to register all solvent makers
 *
 * @version $Id: PsInteractionMakerMap.h 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_INTERACTION_MAKER_MAP_H
#define PS_INTERACTION_MAKER_MAP_H

#include <TxMaker.h>

// psbase includes
#include <PsInteraction.h>

template <class FLOATTYPE, size_t NDIM>
class PsInteractionMakerMap {

  public:

/**
 * Constructor: create all of the makers
 */
    PsInteractionMakerMap();

/**
 * Destructor
 */
    virtual ~PsInteractionMakerMap();

};

#endif // PS_INTERACTION_MAKER_MAP_H
