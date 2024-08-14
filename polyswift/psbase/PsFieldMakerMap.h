/**
 *
 * @file    PsFieldMakerMap.h
 *
 * @brief   Interface for class to register all field makers
 *
 * @version $Id: PsFieldMakerMap.h 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 *
 */

#ifndef PS_FIELD_MAKER_MAP_H
#define PS_FIELD_MAKER_MAP_H

#include <stdio.h>

#include <TxMaker.h>

// psphysf includes
#include <PsFieldBase.h>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/**
 * Class that holds all of the makers of field data structures
 */
template <class FLOATTYPE, size_t NDIM>
class PsFieldMakerMap {

  public:

/**
 * Constructor: create all of the makers
 */
    PsFieldMakerMap();

/**
 * Destructor
 */
    virtual ~PsFieldMakerMap();

};

#endif // PS_FIELD_MAKER_MAP_H
