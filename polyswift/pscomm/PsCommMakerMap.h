/**
 *
 * @file    PsCommMakerMap.h
 *
 * @brief   Interface for class to register all communication makers
 *
 * @version $Id: PsCommMakerMap.h 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_COMM_MAKER_MAP_H
#define PS_COMM_MAKER_MAP_H

#include <TxMaker.h>

// psbase includes
#include <PsCommBase.h>

/**
 * Class that holds all of the makers of fluids
 */
template <class FLOATTYPE, size_t NDIM>
class PsCommMakerMap {

  public:

/**
 * Constructor: create all of the makers
 */
    PsCommMakerMap();

/**
 * Destructor
 */
    virtual ~PsCommMakerMap();

};

#endif // PS_COMM_MAKER_MAP_H
