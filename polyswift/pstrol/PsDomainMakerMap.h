/**
 *
 * @file    PsDomainMakerMap.h
 *
 * @brief   Interface for class to register domain
 *
 * @version $Id: PsDomainMakerMap.h 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2020-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_DOMAIN_BASE_MAKER_MAP_H
#define PS_DOMAIN_BASE_MAKER_MAP_H

// std includes
#include <vector>

// vpstd includes
#include <TxMaker.h>
#include <TxMakerMap.h>

// vpbase includes
#include <PsDomainBase.h>

/**
 * Class that holds all of the makers of domains
 */
class PsDomainMakerMap {

  public:

/// Constructor: create all of the makers
    PsDomainMakerMap();

/// No copy constructor
    PsDomainMakerMap(const PsDomainMakerMap&) = delete;

/// Destructor
    virtual ~PsDomainMakerMap();

/// No assignment
    PsDomainMakerMap& operator=(const PsDomainMakerMap&) = delete;

};

#endif // PS_DOMAIN_BASE_MAKER_MAP_H

