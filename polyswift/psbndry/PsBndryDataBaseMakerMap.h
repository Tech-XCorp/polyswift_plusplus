/**
 * @file    PsBndryDataBaseMakerMap.h
 *
 * @brief   Interface for class to register all bndry DB makers
 *
 * @version $Id: PsBndryDataBaseMakerMap.h 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_BNDRY_DATABASE_MAKER_MAP_H
#define PS_BNDRY_DATABASE_MAKER_MAP_H

#include <TxMaker.h>

// psbase includes
#include <PsBndryDataBase.h>

/**
 * Class that holds all of the makers of boundary data classes
 */
template <class FLOATTYPE, size_t NDIM>
class PsBndryDataBaseMakerMap {

  public:

/**
 * Constructor: create all of the makers
 */
    PsBndryDataBaseMakerMap();

/**
 * Destructor
 */
    virtual ~PsBndryDataBaseMakerMap();

};

#endif // PS_BNDRY_DATABASE_MAKER_MAP_H
