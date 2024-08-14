/**
 * @file    PsBndryMakerMap.h
 *
 * @brief   Interface for class to register all bndry makers
 *
 * @version $Id: PsBndryMakerMap.h 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_BNDRY_MAKER_MAP_H
#define PS_BNDRY_MAKER_MAP_H

#include <TxMaker.h>

// psbase includes
#include <PsBndryBase.h>

/**
 * Class that holds all of the makers of fluids
 */
template <class FLOATTYPE, size_t NDIM>
class PsBndryMakerMap {

  public:

/**
 * Constructor: create all of the makers
 */
    PsBndryMakerMap();

/**
 * Destructor
 */
    virtual ~PsBndryMakerMap();

};

#endif // PS_BNDRY_MAKER_MAP_H
