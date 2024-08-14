/**
 *
 * @file    PsFFTMakerMap.h
 *
 * @brief   Interface for class to register all communication makers
 *
 * @version $Id: PsFFTMakerMap.h 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_FFT_MAKER_MAP_H
#define PS_FFT_MAKER_MAP_H

#include <TxMaker.h>

// psbase includes
#include <PsFFTBase.h>

/**
 * Class that holds all of the makers of fluids
 */
template <class FLOATTYPE, size_t NDIM>
class PsFFTMakerMap {

  public:

/**
 * Constructor: create all of the makers
 */
    PsFFTMakerMap();

/**
 * Destructor
 */
    virtual ~PsFFTMakerMap();

};

#endif // PS_FFT_MAKER_MAP_H
