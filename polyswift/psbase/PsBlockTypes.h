/**
 *
 * @file    PsBlockTypes.h
 *
 * @brief   Traits-like class for type definitions concerning blocks.
 *
 * @version $Id: PsBlockTypes.h 8199 2007-09-05 05:07:11Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_BLOCK_TYPES_H
#define PS_BLOCK_TYPES_H

// standard headers
#include <list>
#include <vector>
#include <map>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#undef HAVE_CONFIG_H
#endif

// txbase headers
#include <TxDebugExcept.h>

// psbase includes
#include <PsGridField.h>

/**
 * A PsBlockTypes object contains type definitions for the chain
 * propagator function. The different derived classes for blocks
 * implement models that can have different data types as well
 * as different implementation. The PsBlock and its dervied classes
 * are templated over QTYPE which is obtained in the class
 * instantiations by a PsBlockTypes::_____ type syntax. The container
 * class PsBlockCopolymer also includes these definitions thereby
 * syncronizing the type specifications.
 *
 * @param FLOATTYPE numeric type of the data.
 * @param NDIM dimensionality of the physical space
 * @param ARRAYBOX type of cell for AMR data types
 */

template <class FLOATTYPE, size_t NDIM>
class PsBlockTypes {

  public:

/**
 * Constructor
 */
  PsBlockTypes() {};

/**
 * Destructor must destroy blocks
 */
  virtual ~PsBlockTypes() {};

/**
 * q(X) type definition for flexible blocks
 */
  typedef PsGridField<FLOATTYPE, NDIM> flexQType;

/**
 * q(X) type definition for semi-flexible blocks
 */
  typedef PsGridField<FLOATTYPE, 2*NDIM-1> semiflexQType;

 protected:

 private:

  /** Constructor private to prevent use */
  PsBlockTypes(const PsBlockTypes<FLOATTYPE, NDIM>& psb);

  /** Assignment private to prevent use */
  PsBlockTypes<FLOATTYPE, NDIM>& operator=(
      const PsBlockTypes<FLOATTYPE, NDIM>& psb);

};

#endif // PS_BLOCK_TYPES_H
