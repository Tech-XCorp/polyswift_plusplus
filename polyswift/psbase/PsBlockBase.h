/**
 *
 * @file    PsBlockBase.h
 *
 * @brief   Base class for contiguous monomer blocks.
 *
 * @version $Id: PsBlockBase.h 8199 2007-09-05 05:07:11Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_BLOCK_BASE_H
#define PS_BLOCK_BASE_H

// standard headers
#include <list>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#undef HAVE_CONFIG_H
#endif

// txbase headers
#include <TxDebugExcept.h>

// psbase includes
#include <PsFunctionalQ.h>
#include <PsPhysField.h>
#include <PsPolymer.h>
#include <PsDecompBase.h>
#include <PsGridBase.h>
#include <PsCommBase.h>

// pspolymer includes
#include <PsBlockTypes.h>

// BlockBase boundary condition specification
enum BcType {INITIAL, FINAL};

// BlockBase end specification
enum BlockEndType {HEAD, TAIL};

/**
 * A PsBlockBase object is the main base/interface class
 * for polymer blocks. Contains common data/methods that
 * DO NOT depend on either the rank of q(X) or the model.
 *
 * This includes the concept of a block having ends (HEAD/TAIL)
 * and that blocks can be connected to other blocks and
 * have q(X) values set at ends that constitute boundary
 * conditions for other blocks.
 *
 * @param FLOATTYPE numeric type of the data.
 * @param NDIM dimensionality of the physical space
 */

template <class FLOATTYPE, size_t NDIM>
class PsBlockBase : public virtual PsFunctionalQ<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor
 */
    PsBlockBase();

/**
 * Destructor must destroy blocks
 */
    virtual ~PsBlockBase();

/**
 * Set the parameters
 *
 * @param tas the parameters
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Allocate data spaces
 */
    virtual void buildData();

/**
 * Build the solvers structures
 * Set the connected block head/tail lists and junction switches
 * This sets the length of the map junction switches. The derived
 * block classes use this in the buildSolvers() step to iterate over
 * the qXXXXJnts maps and set keys, and initial values
 */
    virtual void buildSolvers();

/**
 * Convert to appropriate number of integer block steps
 * Perform consistency checks. Note: in general, blockSteps set
 * by taking scaled length of block and dividing by ds. This
 * is not guaranteed to be an integer, so this method performs
 * conversion. The flog forceBlockSteps controls logic in this method
 *
 * @param bs float number of block steps
 * @return integer number of block steps
 */
    size_t setBlockSteps(FLOATTYPE bs);

/**
 * Reset switches/initial Q values before update methods
 */
    virtual void reset();

/**
 * Update forward/backward propagators
 *
 * @param solveFromEnd block end to start calculation from
 */
    virtual void solveQ(BlockEndType solveFromEnd) = 0;

/**
 * Integrate [ q(X,s)*qt(X,s) ds ] and set the
 * QTYPE qqtIntegral data member
 *
 * @param bQ normalization factor for propagator (ie. bigQ)
 */
    virtual void setCalcQQTIntegral(FLOATTYPE bQ) = 0;

/**
 * Integrate [q(X,s')*qt(X,s') dX ]
 *
 * @return normalization Q value
 */
    virtual FLOATTYPE calcBigQ() = 0;

/**
 * Use the qqtIntegral result to calculate polymer chain
 * observables from block
 */
    virtual void setPhysFields() = 0;

/**
 * Set the name of the polymer in which this block is contained
 *
 * @param name polymer string name
 */
    void setPolymerName(std::string name) {
      polymerName = name;
    }

/**
 * Get the name of the polymer in which this block is contained
 */
    std::string getPolymerName() {
      return polymerName;
    }

/**
 * Build the static block name map
 *
 * @param blockNames vector of block name strings
 */
    static void buildNameMap(std::vector<std::string> blockNames);

/**
 * Checks and set connected type to TAIL,
 * because initialized to HEAD
 *
 * @param end       type of list (head/tail) to set
 * @param nblock    index of block to set
 */
    void buildCntToTail(BlockEndType end, size_t nblock);

/**
 * Return block type of "other" end of block (ie HEAD-->TAIL)
 *
 * @param end block-end to "flip"
 */
    virtual BlockEndType getOtherEnd(BlockEndType end) {
      if (end == HEAD) {return TAIL;}
      else             {return HEAD;}
    }

/**
 * Return string used to identify derived class
 *
 * @return block Type string
 */
    virtual std::string getBlockType() {
      return blockTypeStr;
    }

/**
 * Return size of fractional length of block
 *
 * @return length fraction
 */
    virtual FLOATTYPE getLengthFrac() {
      return lengthFrac;
    }

/**
 * Get weight. Default is 1.0 for monodisperse models
 * Polydisperse models need to adjust this weight by
 * reading from attributue set (Note no setWeight method
 * to enforce this requirement)
 *
 * @return quadrature weight
 */
    virtual FLOATTYPE getWeight() {
      return weight;
    }

/**
 * Get length fraction weight. Default is 1.0 for monodisperse models
 * Polydisperse models need to adjust this weight through the
 * appropriate holder class. Used for calculating contribution to
 * physical field averages (eg monomer density average)
 *
 * @return quadrature length fraction weight
 */
    virtual FLOATTYPE getLengthWeight() {
      return lengthWeight;
    }

/**
 * Get density observable weight. Default is 1.0 for monodisperse models
 * Polydisperse models need to adjust this weight through the
 * appropriate holder class. Used for calculating contribution to
 * observables (eg monomer density)
 *
 * @return quadrature length fraction weight
 */
    virtual FLOATTYPE getDensityWeight() {
      return densityWeight;
    }

/**
 * Get length fraction weight. Default is 1.0 for monodisperse models
 * Polydisperse models need to adjust this weight through the
 * appropriate holder class. Used for calculating contribution to
 * physical field averages (eg monomer density average)
 *
 * @param quadrature length fraction weight
 */
    virtual void setLengthWeight(FLOATTYPE wt) {
      lengthWeight = wt;
    }

/**
 * Get density observable weight. Default is 1.0 for monodisperse models
 * Polydisperse models need to adjust this weight through the
 * appropriate holder class. Used for calculating contribution to
 * observables (eg monomer density)
 *
 * @return quadrature length fraction weight
 */
    virtual void setDensityWeight(FLOATTYPE wt) {
      densityWeight = wt;
    }

/**
 * Get connected head/tail list
 *
 * @param end head/tail
 * @return    either head/tail connected to list
 */
    virtual std::vector<std::pair<size_t, BlockEndType> >
      getCntTo(BlockEndType end);

/**
 * Calculate constrained initial q values from all junction values
 * at head/tail. Call setInitialQ afterwards.
 *
 * @param end head or tail
 */
    virtual void combineSetJnt(BlockEndType end) = 0;

/**
 * Check if initial/final Q set
 *
 * @param bc  boundary condition type initial/final
 * @param end head or tail
 * @return    is q set either because its
 *            free or resulting from calculation, from junctions
 */
    virtual bool isQSet(BcType bc, BlockEndType end);

/**
 * Check if all junction Q's set at head/tail
 *
 * @param  end  head or tail
 * @return are connected junction values set at end
 */
    virtual bool areJntsSet(BlockEndType end);

/**
 * Check if both forward/backward q functions updated
 */
    virtual bool isBlockUpdated();

/**
 * Main Block interface method: only called
 * by container (PsBlockCopolymer) in order to convert q(X) types:
 * Get final q value for head/tail
 *
 * @param end endtype
 * @return    data pointer type to transfer q(X) values
 */
    virtual PsFieldBase<FLOATTYPE>* getFinalQ(BlockEndType end) = 0;

/**
 * Main Block interface method: only called
 * by container (PsBlockCopolymer) that expects a conversion for q(X) types:
 * Set q for head/tail at appropriate junction holder
 *
 * @param end       endtype for 'this' object (receiving data)
 * @param blockB    pointer to block interface sending data
 * @param blockBend endtype for blockB (internal 'getFinalQ' call needs)
 * @param jntKey    key index of block that junction is connected to
 */
    virtual void setJctQ(BlockEndType end,
        PsBlockBase<FLOATTYPE, NDIM>* blockB, BlockEndType blockBend,
        size_t jntKey) = 0;

  protected:

    /*
      weight        -- raw quadrature weight
      lengthWeight  -- weight w/gamma[alpha] factor for polydisperse block
                       and 1/ng weight for constant block
                       (all 1.0 for pure monodisperse)
      densityWeight -- weight w/gamma[alpha] factor for all blocks in
                       polydisperse model
     */

    /** Raw quadrature weight */
    FLOATTYPE weight;

    /** Weight used to get average length */
    FLOATTYPE lengthWeight;

    /** Weight used to get average density */
    FLOATTYPE densityWeight;

    /** Block index */
    size_t ngIndex;

    /** Temp volume */
    FLOATTYPE volLocal;

    /** Fractional length */
    FLOATTYPE lengthFrac;

    /**
     * Flag for foring logic
     * in calculating blockSteps [see setBlockSteps()]
     */
    bool forceBlockSteps;

    /** Number of steps on block */
    size_t blockSteps;

    /** Discretization size of segment length */
    FLOATTYPE ds;

    /** String type identifier set by derived class */
    std::string blockTypeStr;

    /** Polymer container object pointer */
    PsPolymer<FLOATTYPE, NDIM>* polymerObjPtr;

/**
 * Set the connected block head/tail lists and junction switches
 * This sets the length of the map junction switches. The derived
 * block classes use this in buildSolvers() step to iterate over
 * the qXXXXJnts maps and set keys, and initial values
 */
    virtual void setCntToLists();

    /** List of block indices/endtype connected to heads */
    std::vector< std::pair<size_t, BlockEndType> > headCntTo;

    /** List of block indices/endtype connected to tails */
    std::vector< std::pair<size_t, BlockEndType> > tailCntTo;

    //
    // Switches (not qX dependent)
    //

    /** Is initial condition for q set at head */
    std::map<size_t, bool> qHeadJntsSet;

    /** Is initial condition for q set at tail */
    std::map<size_t, bool> qTailJntsSet;

    /** Is initial condition for q set at head */
    bool qHeadInitialSet;

    /** Is initial condition for q set at tail */
    bool qTailInitialSet;

    /** Has final q value been calculated at head */
    bool qHeadFinalSet;

    /** Has final q value been calculated at tail */
    bool qTailFinalSet;

  private:

    /** Minimum required number of contour steps block */
    size_t minBlockSteps;

    /** Name of polymer that contains this block */
    std::string polymerName;

    /** List of block names one end is connected */
    std::vector<std::string> headBlockNames;

    /** List of block names 'other' end is connected */
    std::vector<std::string> tailBlockNames;

    /** Map of names to location in block list */
    static std::map<std::string, size_t> blockNameMap;

    /** Constructor private to prevent use */
    PsBlockBase(const PsBlockBase<FLOATTYPE, NDIM>& psb);

   /** Assignment private to prevent use */
    PsBlockBase<FLOATTYPE, NDIM>& operator=(
       const PsBlockBase<FLOATTYPE, NDIM>& psb);
};

#endif  // PS_BLOCK_BASE_H
