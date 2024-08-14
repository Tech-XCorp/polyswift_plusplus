/**
 *
 * @file    PsHistoryBase.h
 *
 * @brief   Interface base class for history diagnostics
 *
 * @version $Id: PsHistoryBase.h 7888 2007-07-25 10:32:45Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_HISTORY_BASE_H
#define PS_HISTORY_BASE_H

// std includes
#include <vector>
#include <map>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsDynObj.h>

/**
 * Interface base class for PsHistory.
 *
 * A history contains a time series data.  Examples
 * are free-energy, density field at points, etc.
 * At each time step the history is updated.
 * At dump time, the values from all processors are gathered together
 * and then written to the data file (runname)_History.h5
 *
 * @param FLOATTYPE the data type of simulation
 * @param NDIM the dimensionality of simulation
 */
template <class FLOATTYPE, size_t NDIM>
class PsHistoryBase : public virtual PsDynObj<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor
 */
    PsHistoryBase();

/**
 * Destructor
 */
    virtual ~PsHistoryBase() {}

/**
 * Read in attribute set
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Empty method for history base
 */
    virtual void buildSolvers();

/**
 * Empty method for history base
 */
    virtual void initialize();

/**
 * Builds PsHistory methods
 */
    virtual void buildData();

/**
 * Dump specific history data
 *
 * @param txIoPtr the IO object pointer
 * @param file node opened by holder object
 */
    virtual void dump(TxIoBase* txIoPtr, TxIoNodeType fnode) = 0;

/**
 * Gathers diagnostic information for all PsHistories
 */
    virtual void sync() = 0;

/**
 * Return length of the history data
 */
    virtual size_t getHistDataLength() = 0;

/**
 * Erase data histData (ie after dumping to file)
 */
    virtual void clearData() = 0;

/**
 * History updater
 */
    virtual void update(double t);

/**
 * Keep track of simulation time step
 */
    virtual void updateSimTimeStep(size_t tstep) {
      this->simTimeStep = tstep;
    }

/**
 * Set global history attributes
 */
    virtual void setHistAttributes();

/**
 * Number of timesteps between history updates
 */
    size_t updatePeriod;

/**
 * Number of timesteps before history starts to update
 */
    size_t tstepsBeforeStart;

/**
 * Number of timesteps after which history stops updating
 */
    size_t tstepsBeforeFinish;

/**
  * Present simulation time step
  */
    size_t simTimeStep;

  protected:

    /** Real simulation time for history data */
    std::vector<double> histTimes;

    /** Current time at history update */
    double currentHistTime;

    /** The time attribute */
    double t0;

    /** The time-step attribute */
    double dt;

    /** Map of attribute names and data */
    std::map<std::string, std::string> histAttrString;

    /** Map of attribute names and data */
    std::map<std::string, FLOATTYPE> histAttributes;

    /** Map of attribute names and data */
    std::map<std::string, std::vector<FLOATTYPE> > histAttrVec;

    /** Map of attribute name and data */
    std::map<std::string, std::vector<int> > histAttrIntVec;

/**
 * Write all specific maps of attributes for a history
 */
    void appendHistAttribs(TxIoBase* txIoPtr, TxIoNodeType dn,
        bool barrierFlag);

  private:

    /** Private copy constructor to prevent use */
    PsHistoryBase(const PsHistoryBase<FLOATTYPE, NDIM>& psh);

    /** Private assignment to prevent use */
    PsHistoryBase<FLOATTYPE, NDIM>& operator=(
        const PsHistoryBase<FLOATTYPE, NDIM>& psh);
};

#endif // PS_HISTORY_BASE_H

