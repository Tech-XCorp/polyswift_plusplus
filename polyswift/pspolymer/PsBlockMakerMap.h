/**
 * @file    PsBlockMakerMap.h
 *
 * @brief   Interface for class to register all polymer makers
 *
 * @version $Id: PsBlockMakerMap.h 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_BLOCK_MAKER_MAP_H
#define PS_BLOCK_MAKER_MAP_H

#include <TxMaker.h>

// pspolymer includes
#include <PsBlockBase.h>

/**
 * Class that holds all of the makers of fluids
 */
template <class FLOATTYPE, size_t NDIM>
class PsBlockMakerMap {

  public:

/**
 * Constructor: create all of the makers
 */
    PsBlockMakerMap();

/**
 * Destructor
 */
    virtual ~PsBlockMakerMap();

};

#endif // PS_BLOCK_MAKER_MAP_H
