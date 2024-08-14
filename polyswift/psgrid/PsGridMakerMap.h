/**
 *
 * @file    PsGridMakerMap.h
 *
 * @brief   Interface for class to register all grid makers
 *
 * @version $Id: PsGridMakerMap.h 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_GRID_MAKER_MAP_H
#define PS_GRID_MAKER_MAP_H

#include <TxMaker.h>

// psbase includes
#include <PsGridBase.h>

template <class FLOATTYPE, size_t NDIM>
class PsGridMakerMap {

  public:

/**
 * Constructor: create all of the makers
 */
    PsGridMakerMap();

/**
 * Destructor
 */
    virtual ~PsGridMakerMap();

};

#endif // PS_GRID_MAKER_MAP_H
