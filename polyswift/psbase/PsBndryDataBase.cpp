/**
 *
 * @file    PsBndryDataBase.cpp
 *
 * @brief   Base class interface for boundary data
 *
 * @version $Id: PsBndryDataBase.cpp 8257 2007-09-12 22:05:20Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsBndryDataBase.h>

//
// Constructor
//
template <class FLOATTYPE, size_t NDIM>
PsBndryDataBase<FLOATTYPE, NDIM>::PsBndryDataBase() {
}

//
// Destructor
//
template <class FLOATTYPE, size_t NDIM>
PsBndryDataBase<FLOATTYPE, NDIM>::~PsBndryDataBase() {
}

// Instantiation
template class PsBndryDataBase<float, 1>;
template class PsBndryDataBase<float, 2>;
template class PsBndryDataBase<float, 3>;

template class PsBndryDataBase<double, 1>;
template class PsBndryDataBase<double, 2>;
template class PsBndryDataBase<double, 3>;

