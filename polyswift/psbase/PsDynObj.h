/**
 * @file    PsDynObj.h
 *
 * @brief   Base class for dynamic objects
 *
 * @version $Id: PsDynObj.h 8197 2007-09-04 22:23:46Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_DYN_OBJ_H
#define PS_DYN_OBJ_H

#include <cstring> // for memcpy()

// std includes
#include <string>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// txbase includes
//#include <TxThroughStream.h>
#include <TxDebugExcept.h>
#include <TxNotImplExcept.h>

// txio includes
#include <TxIoBase.h>

// psbase includes
#include <PsDynObjBase.h>
#include <PsDomainSingletons.h>

// psstd includes
#include <PsSTFunc.h>
#include <PsNamedObject.h>

/**
 * A PsDynObj is a dynamic object that has some implementation
 * of time step saving.  It also knows about the domain singletons.
 */
template <class FLOATTYPE, size_t NDIM>
class PsDynObj : public PsDynObjBase , public PsNamedObject {

  public:

/**
 * Constructor.  However, object is not valid until
 * setDomainSingletons has been called.
 */
    PsDynObj();

/**
 * Constructor
 */
    PsDynObj(std::string nm);

/**
 * Copy Constructor
 *
 * @param dobj object to construct from
 */
    PsDynObj(const PsDynObj<FLOATTYPE, NDIM>& dobj);

/**
 * Destructor
 */
    virtual ~PsDynObj();

/**
 * Set from an attribute set.
 *
 * Stores attribute set and sets name.  By storing the settings
 * in the base class, any derived class has the information it
 * needs to rebuild itself.
 *
 * @param tas the attribute set
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Build the data for this object
 */
    virtual void buildData();

/**
 * Build the solvers structures for this object
 */
    virtual void buildSolvers();

/**
 * dump system
 */
    virtual void dump();

/**
 * update system
 *
 * @param time the new time to update to
 */
    virtual void update(double time) {};

/**
 * assigment
 *
 * @param dobj the dynanmic object class
 */
    PsDynObj<FLOATTYPE, NDIM>& operator=(const PsDynObj<FLOATTYPE, NDIM>& dobj);

/**
 * Set the domain singletons
 *
 * @param ds the reference to the new domain singletons holder
 */
    virtual void setDomainSingletons(PsDomainSingletons<FLOATTYPE, NDIM>& ds);

/**
 * Get the domain singletons
 *
 * @return a reference to the domain singletons
 */
    PsDomainSingletons<FLOATTYPE, NDIM>& getDomainSingletons() {
      if (!domSingsPtr) {
        TxDebugExcept tde("PsDynObj::getDomainSingletons: domSingsPtr not set");
        throw tde;
      }
      return *domSingsPtr;
    }

/**
 * Get a const reference to the domain singletons.
 *
 * @return a const reference to the domain singletons
 */
    const PsDomainSingletons<FLOATTYPE, NDIM>& getDomainSingletons() const {
      if (!domSingsPtr) {
        TxDebugExcept tde("PsDynObj::getDomainSingletons: domSingsPtr not set");
        throw tde;
      }
      return *domSingsPtr;
    }

/**
 * Get the grid from singletons
 *
 * @return the grid
 */
    virtual PsGridBase<FLOATTYPE, NDIM>& getGridBase() {
      if (!domSingsPtr) {
        TxDebugExcept tde("PsDynObj::getGridBase: domSingsPtr not set");
        throw tde;
      }
      return domSingsPtr->getGridBase();
    }

/**
 * Get the comm from singletons
 *
 * @return the comm.
 */
    virtual PsCommBase<FLOATTYPE, NDIM>& getCommBase() {
      if (!domSingsPtr) {
        TxDebugExcept tde("PsDynObj::getCommBase: domSingsPtr not set");
        throw tde;
      }
      return domSingsPtr->getCommBase();
    }

/**
 * Get the output file prefix
 *
 * @return the output file prefix
 */
    virtual std::string& getOutputFilePrefix() {
      if (!domSingsPtr) {
        TxDebugExcept tde("PsDynObj::getOutputFilePrefix: domSingsPtr not set");
        throw tde;
      }
      return domSingsPtr->getOutputFilePrefix();
    }

/**
 * Print debug messages to single stream using std::cout
 *
 * @param msgStr the message string
 */
    virtual void pprt(std::string msgStr);

/**
 * Print debug messages to single stream using std::cout
 *
 * @param msgStr1 the message string
 * @param msgStr2 the message string
 */
    virtual void pprt(std::string msgStr1,
                      std::string msgStr2);

/**
 * Print debug messages to single stream using std::cout
 *
 * @param msgStr1 the message string
 * @param msgStr2 the message string
 * @param msgStr3 the message string
 */
    virtual void pprt(std::string msgStr1,
                      std::string msgStr2,
                      std::string msgStr3);

/**
 * Print debug messages to single stream using std::cout
 *
 * @param msgStr1 the message string
 * @param msgStr2 the message string
 * @param msgStr3 the message string
 * @param msgStr4 the message string
 */
    virtual void pprt(std::string msgStr1,
                      std::string msgStr2,
                      std::string msgStr3,
                      std::string msgStr4);

/**
 * Print debug messages to single stream using std::cout
 *
 * @param msgStr the message string
 */
    virtual void dbprt(std::string msgStr);

/**
 * Print debug messages to single stream using std::cout
 *
 * @param msgStr1 the message string
 * @param msgStr2 the message string
 */
    virtual void dbprt(std::string msgStr1,
                       std::string msgStr2);

/**
 * Print debug messages to single stream using std::cout
 *
 * @param msgStr1  the message string
 * @param inum     int number
 */
    virtual void dbprt(std::string msgStr1, int inum);

/**
 * Print debug messages to single stream using std::cout
 *
 * @param msgStr1  the message string
 * @param fnum     float number
 */
    virtual void dbprt(std::string msgStr1, FLOATTYPE fnum);

/**
 * Print debug messages to single stream using std::cout
 *
 * @param msgStr1 the message string
 * @param fnum     float number
 */
    virtual void pprt(std::string msgStr1, FLOATTYPE fnum);

/**
 * Print messages to single stream using std::cout
 *
 * @param msgStr1 the message string
 * @param inum     int number
 */
    virtual void pprt(std::string msgStr1, int inum);

/**
 * Print debug messages to single stream using std::cout
 *
 * @param msgStr1 the message string
 * @param msgStr2 the message string
 * @param msgStr3 the message string
 */
    virtual void dbprt(std::string msgStr1,
                       std::string msgStr2,
                       std::string msgStr3);

/**
 * Print debug messages to single stream using std::cout
 *
 * @param msgStr1 the message string
 * @param msgStr2 the message string
 * @param msgStr3 the message string
 * @param msgStr4 the message string
 */
    virtual void dbprt(std::string msgStr1,
                       std::string msgStr2,
                       std::string msgStr3,
                       std::string msgStr4);

  protected:

    /** Local debug flag */
    DebugPrint dbStatus;

    /** The current time of update */
    FLOATTYPE curTime;

  private:

    /** The domain singletons */
    PsDomainSingletons<FLOATTYPE, NDIM>* domSingsPtr;

    // Time difference private so it is done consistently.
    /** The difference between this time and last time */
    FLOATTYPE delTime;

/**
 * dumpPeriod is initialized by default to 1. It is for use with
 * input file data to set it to a positive value larger than 1 to
 * allow dumping for an object that derives from PsDynObj only
 * every dumpPeriod time when the dump() function of the derived
 * class is called.
 */
    size_t dumpPeriod;
};

#endif // PS_DYN_OBJ_H
