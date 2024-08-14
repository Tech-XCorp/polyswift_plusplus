/**
 *
 * @file    PsSTFuncMakerMap.h
 *
 * @brief   Interface for class to register all stfunc makers
 *
 * @version $Id: PsSTFuncMakerMap.h 2137 2020-04-16 20:37:29Z alexanda $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation
 */

#ifndef PS_STFUNC_MAKER_MAP_H
#define PS_STFUNC_MAKER_MAP_H

#include <TxMaker.h>

// psbase includes
#include <PsSTFunc.h>

template <class FLOATTYPE, size_t NDIM>
class PsSTFuncMakerMap {

  public:

/// Constructor: create all of the makers
    PsSTFuncMakerMap();

/// No copy constructor
    PsSTFuncMakerMap(const PsSTFuncMakerMap<FLOATTYPE, NDIM>&)
        = delete;

/// Destructor
    virtual ~PsSTFuncMakerMap();

/// No assignment
    PsSTFuncMakerMap<FLOATTYPE, NDIM>& operator=(
        const PsSTFuncMakerMap<FLOATTYPE, NDIM>&) = delete;

};

#endif // PS_STFUNC_MAKER_MAP_H
