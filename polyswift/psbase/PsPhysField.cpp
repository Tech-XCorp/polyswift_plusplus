/**
 * @file    PsPhysField.cpp
 *
 * @brief   Interface for physical "observable" fields
 *
 * @version $Id: PsPhysField.cpp 8257 2007-09-12 22:05:20Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsPhysField.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsPhysField<FLOATTYPE, NDIM>::PsPhysField() {
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsPhysField<FLOATTYPE, NDIM>::~PsPhysField() {
}

// Instantiation
template class PsPhysField<float, 1>;
template class PsPhysField<float, 2>;
template class PsPhysField<float, 3>;

template class PsPhysField<double, 1>;
template class PsPhysField<double, 2>;
template class PsPhysField<double, 3>;

