/**
 *
 * @file    PsGridBaseItr.h
 *
 * @brief   Base interface for the grid without templates
 *
 * @version $Id: PsGridBaseItr.h 8199 2007-09-05 05:07:11Z swsides $
 *
 * Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_GRID_BASE_ITR_H
#define PS_GRID_BASE_ITR_H

// standard headers
#include <set>
#include <vector>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#undef HAVE_CONFIG_H
#endif

// txbase headers
#include <TxDebugExcept.h>

/**
 * A PsGridBaseItr object contains base Grid methods interface
 * without template parameters for use in PsFieldBase
 */

class PsGridBaseItr {

  public:

/**
 * Constructor
 *
 */
  PsGridBaseItr() {};

/**
 * Destructor
 */
  virtual ~PsGridBaseItr() {};

/**
 * Get number of physical cells in each global dimension
 *
 * @return vector of cell numbers
 */
    virtual std::vector<size_t> getNumCellsGlobal() = 0;

  protected:

  private:

    /** Make private to prevent use */
    PsGridBaseItr(const PsGridBaseItr& vphh);

    /** Make private to prevent use */
    PsGridBaseItr& operator=(const PsGridBaseItr& vphh);
};

#endif // PS_GRID_BASE_ITR_H
