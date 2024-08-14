/**
 * @file    PsHistoryMakerMap.h
 *
 * @brief   Interface for class to register all history makers
 *
 * @version $Id: PsHistoryMakerMap.h 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_HISTORY_MAKER_MAP_H
#define PS_HISTORY_MAKER_MAP_H

#include <TxMaker.h>

// psbase includes
#include <PsHistoryBase.h>

template <class FLOATTYPE, size_t NDIM>
class PsHistoryMakerMap {

  public:

/**
 * Constructor: create all of the makers
 */
    PsHistoryMakerMap();

/**
 * Destructor
 */
    virtual ~PsHistoryMakerMap();

};

#endif // PS_HISTORY_MAKER_MAP_H
