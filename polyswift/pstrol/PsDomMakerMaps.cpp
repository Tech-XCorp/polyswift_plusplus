/**
 *
 * @file    PsDomMakerMaps.cpp
 *
 * @brief   Implementation and instantiation of the PsDomMakerMaps class.
 *
 * @version $Id: PsDomMakerMaps.cpp 6562 2007-02-05 12:57:10Z cary $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// psbase includes
#include <PsDomMakerMaps.h>

//
// Instantiate the maker maps
//
template class PsDomMakerMaps<float, 1>;
template class PsDomMakerMaps<float, 2>;
template class PsDomMakerMaps<float, 3>;

template class PsDomMakerMaps<double, 1>;
template class PsDomMakerMaps<double, 2>;
template class PsDomMakerMaps<double, 3>;
