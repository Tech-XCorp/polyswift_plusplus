/**
 * @file    PsPolymer.h
 *
 * @brief   Base class interface for polymer chains
 *
 * @version $Id: PsPolymer.h 8199 2007-09-05 05:07:11Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_POLYMER_H
#define PS_POLYMER_H

// standard headers
#include <set>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
// #undef HAVE_CONFIG_H
#endif

// txbase headers
#include <TxDebugExcept.h>

// psstd includes
#include <PsSTFunc.h>

// psbase includes
#include <PsDynObj.h>

/**
 * A PsPolymer object contains the spatially dependent
 * polymer chain observables. These include, density fields,
 * orientation fields, stress fields, electric potential etc.
 *
 * @param FLOATTYPE numeric type of the data.
 * @param NDIM dimensionality of the physical space
 */

template <class FLOATTYPE, size_t NDIM>
class PsPolymer : public virtual PsDynObj<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor
 */
    PsPolymer();

/**
 * Destructor must destroy polymers
 */
    virtual ~PsPolymer();

/**
 * Set the parameters
 *
 * @param tas the parameters
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Build the data, such as updaters, parses input etc.
 */
    virtual void buildData();

/**
 * Build the solvers structures, fields etc.
 */
    virtual void buildSolvers();

/**
 * Update the object
 *
 * @param t update time
 */
    virtual void update(double t);

/**
 * Get the natural-log of the single-chain partition function
 * normalization value
 *
 * @return log(bigQ)
 */
    virtual FLOATTYPE getLogBigQ() = 0;

/**
 * Get the overall volume fraction for this polymer
 *
 * @return volume fraction
 */
    virtual FLOATTYPE getVolfrac() {
      return volfrac;
    }

/**
 * Get length (polymerization index) for THIS polymer
 *
 * @return volume fraction
 */
    virtual size_t getPolymerLength() {
      return length;
    }

/**
 * Set the length ratio
 *
 * @param lr ratio of polymer length/scale length
 */
    void setLengthRatio(FLOATTYPE lr) {
      lengthRatio = lr;
    }

/**
 * Get the length ratio
 *
 * @return ratio of polymer length/scale length
 */
    FLOATTYPE getLengthRatio() {
      return lengthRatio;
    }

/**
 * Set the static value of polymer length for scaling
 * quantities from ALL polymer objects
 *
 * @param scalelen polymer length to set as scale value
 */
    static void setScaleLength(size_t scalelen);

/**
 * Get the static value of polymer length for scaling
 * polymer quantities
 *
 * @return static polymer scale length
 */
    static size_t getScaleLength();

/**
  * Access all strings for created objects
  *
  * @return static vector of name strings
  */
    static std::vector<std::string> getPolymerNames();

/**
  * Access all strings for created objects
  *
  * @return static vector of name strings
  */
    static void setPolymerName(std::string pName);

  protected:

    /** Volume fraction of component */
    FLOATTYPE volfrac;

    /** Polymer length divided by scaleLength */
    FLOATTYPE lengthRatio;

    /** Number of statistical units in polymer */
    size_t length;

    /** Scale length for overall polymer model */
    static size_t scaleLength;

    /** Input hierarchy set for STFunc-s */
    TxHierAttribSetIntDbl funcAttrib;

    /** Names of STFuncs */
    std::vector<std::string> funcNames;

    /** Function for updating volume frac */
    PsSTFunc<FLOATTYPE, NDIM>* vfSTFunc;

    /** Flag for vfSTFunc */
    bool hasVfSTFunc;

  private:

    /** Make private to prevent use */
    PsPolymer(const PsPolymer<FLOATTYPE, NDIM>& vphh);

    /** Make private to prevent use */
    PsPolymer<FLOATTYPE, NDIM>& operator=(
       const PsPolymer<FLOATTYPE, NDIM>& vphh);

    /** All strings for these objects */
    static std::vector<std::string> allPolymerNames;
};

#endif // PS_POLYMER_H
