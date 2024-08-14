/**
 * @file    PsFixedWall.h
 *
 * @brief   Class containing info for fixed wall
 *
 * @version $Id: PsFixedWall.h 6319 2006-11-14 22:39:46Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_FIXED_WALL_H
#define PS_FIXED_WALL_H

// std includes
#include <vector>
#include <map>
#include <fstream> // SWS: for input of boundary data

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psstd includes
#include <PsSTFunc.h>

// psbase includes
#include <PsFieldBase.h>
// #include <PsGridField.h>
#include <PsBndryBase.h>
#include <PsBndryDataBase.h>

// psptcl includes
#include <PsWallData.h>

// Flag for choosing subroutine to set wall data
enum SET_WALL_TYPE { FROM_PYFUNC, FROM_FILE};

/**
 * Class for fixed wall boundary. Set class data describing
 * global field values corresponding to wall. Implements
 * a method to then update the necessary physical fields
 * on each domain.
 *
 * @param FLOATTYPE the data type of simulation
 * @param NDIM the dimensionality of simulation
 */
template <class FLOATTYPE, size_t NDIM>
class PsFixedWall : public PsBndryBase<FLOATTYPE, NDIM> {

  public:

/**
 * constructor
 */
  PsFixedWall();

/**
 * Destructor
 */
  virtual ~PsFixedWall();

/**
 * Store the data needed to build this object.
 *
 * @param tas the attribute set containing the initial conditions
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Build the solvers for this object
 */
    virtual void buildSolvers();

/**
 * Build the data for this object
 */
    virtual void buildData();

/**
 * Initialize this object
 */
    virtual void initialize();

/**
 * Restore this object
 *
 * @param txIoPtr IO object pointer
 */
    virtual void restore(TxIoBase* txIoPtr) {};

/**
 * Update the object
 *
 * @param t update time
 */
    virtual void update(double t);

/**
 * Dump info about this object
 * Empty for now
 *
 * @param txIoPtr IO object pointer
 */
    virtual void dump(TxIoBase* txIoPtr) {};

  protected:

  private:

    /** Flag for threshold wall value settings */
    std::string thresholdFlag;

    /** Flag for choosing between setting with STFunc or external file */
    SET_WALL_TYPE setWallMethod;

    /** Value of wall field to treat as an "edge" in dist calcs */
    FLOATTYPE wallEdgeThreshold;

    /** Input hierarchy set for STFunc-s */
    TxHierAttribSetIntDbl funcAttrib;

    /** Names of STFuncs */
    std::vector<std::string> funcNames;

    /** Name of external file containing 'raster' wall data */
    std::string dataFileName;

    /** Function for setting boundary field */
    PsSTFunc<FLOATTYPE, NDIM>* wallSTFunc;

    /** List of wall inside points */
    std::vector< PsTinyVector<int, NDIM> > wallInsidePts;

    /** Map of coords to wall values from data file */
    std::map< PsTinyVector<int, NDIM>, double> wallDataMap;

    /** Stores wall field set initially from STFunc */
    PsWallData<FLOATTYPE, NDIM> wallData;

    /** Stores wall field set initially from STFunc */
    PsWallData<FLOATTYPE, NDIM> localWallData;

    /** Interface to boundary data member */
    PsBndryDataBase<FLOATTYPE, NDIM>* bndryPtr;

    /** Interface to local boundary data member */
    PsBndryDataBase<FLOATTYPE, NDIM>* locBndryPtr;

    //
    // Methods
    //

    /** Local method to set wallData field from STFunc */
    void setWallField();

    /** Local method to set wallData field from external file */
    void setWallFieldFromFile();

    /** Open, read and store wall field values from external file */
    void readWallFieldFile();

    /**
     * Local method to deposit wall field info to bndryDepField
     * taking care of mapping to local domain
     */
    void sendWallToDepField();

    /** Constructor private to prevent use */
    PsFixedWall(const PsFixedWall<FLOATTYPE, NDIM>& psbcp);

    /** Assignment private to prevent use */
    PsFixedWall<FLOATTYPE, NDIM>& operator=(
      const PsFixedWall<FLOATTYPE, NDIM>& psbcp);
};
#endif // PS_FIXED_WALL_H
