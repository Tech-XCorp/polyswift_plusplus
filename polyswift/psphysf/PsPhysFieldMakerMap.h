/**
 *
 * @file    PsPhysFieldMakerMap.h
 *
 * @brief   Interface for class to register all fluid makers
 *
 * @version $Id: PsPhysFieldMakerMap.h 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_PHYSFIELD_MAKER_MAP_H
#define PS_PHYSFIELD_MAKER_MAP_H

#include <TxMaker.h>

// psphysf includes
#include <PsPhysField.h>

/**
 * Class that holds all of the makers of fluids
 */
template <class FLOATTYPE, size_t NDIM>
class PsPhysFieldMakerMap {

  public:

/**
 * Constructor: create all of the makers
 */
    PsPhysFieldMakerMap();

/**
 * Destructor
 */
    virtual ~PsPhysFieldMakerMap();

};

#endif // PS_PHYSFIELD_MAKER_MAP_H
