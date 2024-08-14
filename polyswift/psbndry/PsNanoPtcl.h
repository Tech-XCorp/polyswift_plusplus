/**
 * @file    PsNanoPtcl.h
 *
 * @brief   Contains common particle data/methods common to
 *          particle "types".
 *
 * @version $Id: PsNanoPtcl.h 8199 2007-09-05 05:07:11Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_NANO_PTCL_H
#define PS_NANO_PTCL_H

// standard headers
#include <set>
#include <vector>
#include <algorithm> // Needed for find()

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#undef HAVE_CONFIG_H
#endif

// txbase headers
#include <TxDebugExcept.h>

// psstd includes
#include <PsSTFunc.h>
#include <PsRandom.h>

// psbase includes
#include <PsDynObj.h>
#include <PsPhysField.h>
#include <PsBndryBase.h>
#include <PsBndryDataBase.h>
#include <PsFFTBase.h>

/**
 * A PsNanoPtcl object contains particle data/methods common to
 * particle "types". Holds a vector of pointers to particle data
 * base interfaces (PsBndryDataBase) in a 'local' list ptclGroup.
 *
 * Derived classes will contain operations particular to particle
 * types. These derived classes will cast to appropriate derived
 * data classes (from PsBndryDataBase) when necessary.
 *
 * @param FLOATTYPE numeric type of the data.
 * @param NDIM dimensionality of the physical space
 */
template <class FLOATTYPE, size_t NDIM>
class PsNanoPtcl : public virtual PsBndryBase<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor
 *
 */
    PsNanoPtcl();

/**
 * Destructor must destroy nanoptcls
 */
    virtual ~PsNanoPtcl();

//
// Accessors
//

/**
 * Set the parameters
 *
 * @param tas the parameters
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Build the solvers structures, fields etc.
 */
    virtual void buildSolvers();

/**
 * Build the data, such as updaters, initial condition setters, ...
 */
    virtual void buildData();

/**
 * Initialize object before update steps
 */
    virtual void initialize() = 0;

/**
 * Dump particle type info, for development
 *
 * @param txIoPtr IO object poiner
 */
    virtual void dump(TxIoBase* txIoPtr);

/**
 * Update the object:
 *   Add particles to depField then call base class
 *   to set the appropriate physical fields
 *
 * @param t update time
 */
    virtual void update(double t);

  protected:

    /**
     * Returns value of particle field
     *
     * @param rvec global position vector
     */
    virtual FLOATTYPE ptclFunc(PsTinyVector<int, NDIM>& rvec) = 0;

    /**
     * Add particle to group list and update base class list
     *
     * @param ptclPtr pointer to particle data interface
     */
    void addPtcl(PsBndryDataBase<FLOATTYPE, NDIM>* ptclPtr);

    /**
     * Remove particle from group list... this is untested
     * This makes scoping call to base class to remove those copies,
     * but this method is responsible for memory
     *
     * @param ptclPtr pointer to particle data interface
     */
    void removePtcl(PsBndryDataBase<FLOATTYPE, NDIM>* ptclPtr);

    /**
     * Enforces use of global random number generator so all particle
     * data is sync-ed across all procs. Scaled to [-1, 1]
     *
     * @return random floattype values
     */
    FLOATTYPE getRand();

    /**
     * Get random vector with each component scaled between [-maxVal, maxVal]
     * Uses local getRand that returns globally sync-ed random values
     *
     * @param maxVal scaling factor
     * @return a random tiny vector
     */
    PsTinyVector<FLOATTYPE, NDIM> getRandVec(FLOATTYPE maxVal);

    /** Total number of particles to be loaded */
    size_t maxNumPtcls;

    /** Global fields space */
    PsGridField<FLOATTYPE, NDIM> globalCavField;

    /** Global dimensions of ptcl data field workspace */
    std::vector<size_t> globalPtclDims;

    /** Vector of pointers to particle base interfaces */
    std::vector< PsBndryDataBase<FLOATTYPE, NDIM>* > ptclGroup;

    //
    // Pseudo-spectral grad operator members
    //

    /** Global simulation size, scale factor for transform */
    FLOATTYPE scaleFFT;

    /** Pointer to FFT interface object */
    PsFFTBase<FLOATTYPE, NDIM>* fftObjPtr;

    /** Size of the k2, wfac lists to be FFT'd */
    size_t fftSize;

    /** Size of bytes of the k2, wfac lists to be FFT'd */
    size_t fftMemSize;

    /** Component-wise gradient of particle cavity function on whole domain */
    std::vector< PsGridField<FLOATTYPE, NDIM> > gradFieldVec;

    /** new IO object */
    TxIoBase* txIoPtr;

  private:

    /** FFT object name */
    std::string fftKind;

    /**
     * Private method to initialize particle field
     */
    void setPtclData();

    /**
     * Local method to deposit ptclGroup info to depField for
     * particle field
     */
    void sendPtclsToDepField();

    /** Private method to calculate grad of ptcl cavity func */
    void setGradFieldVec();

    /**
     * Append metadata to a dataset common to all PhysFields
     *
     * @param txIoPtr IO object pointer
     * @param fn file node
     * @param datasetName name string for dataset to append metadata
     */
    void appendMetaDataset(TxIoBase* txIoPtr,
                        TxIoNodeType fn, std::string datasetName);

    /**
     * Append vizschema GROUP attributes
     *
     * @param txIoPtr IO object pointer
     * @param fn file node
     */
    void appendMetaGroup(TxIoBase* txIoPtr,
                         TxIoNodeType fn);

    /** Particle cavity function on entire local domain */
    PsGridField<FLOATTYPE, NDIM> cavPtclField;

    /** Make private to prevent use */
    PsNanoPtcl(const PsNanoPtcl<FLOATTYPE, NDIM>& psnp);

    /** Make private to prevent use */
    PsNanoPtcl<FLOATTYPE, NDIM>& operator=(
        const PsNanoPtcl<FLOATTYPE, NDIM>& psnp);

};

#endif  // PS_NANO_PTCL_H
