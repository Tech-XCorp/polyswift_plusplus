/**
 *
 * @file    PsUpdaterMakerMap.h
 *
 * @brief   Interface for class to register all updater makers
 *
 * @version $Id: PsUpdaterMakerMap.h 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_UPDATER_MAKER_MAP_H
#define PS_UPDATER_MAKER_MAP_H

#include <TxMaker.h>

// psbase includes
#include <PsUpdater.h>

template <class FLOATTYPE, size_t NDIM>
class PsUpdaterMakerMap {

  public:

/**
 * Constructor: create all of the makers
 */
    PsUpdaterMakerMap();

/**
 * Destructor
 */
    virtual ~PsUpdaterMakerMap();

};

#endif // PS_UPDATER_MAKER_MAP_H
