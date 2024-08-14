/**
 *
 * @file    PsDomainSingletons.cpp
 *
 * @brief   Implementation and instantiation of the
 *          PsDomainSingletons class.
 *
 * @version $Id: PsDomainSingletons.cpp 6319 2006-11-14 22:39:46Z sizemore $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// psbase includes
#include <PsDomainSingletons.h>

//
// Constructor
//
template <class FLOATTYPE, size_t NDIM>
PsDomainSingletons<FLOATTYPE, NDIM>::PsDomainSingletons() {
  commPtr = NULL;
  gridObjPtr = NULL;
}

//
// Destructor
//
template <class FLOATTYPE, size_t NDIM>
PsDomainSingletons<FLOATTYPE, NDIM>::~PsDomainSingletons() {}

//
// Instantiations
//

template class PsDomainSingletons<float, 1>;
template class PsDomainSingletons<float, 2>;
template class PsDomainSingletons<float, 3>;

template class PsDomainSingletons<double, 1>;
template class PsDomainSingletons<double, 2>;
template class PsDomainSingletons<double, 3>;

