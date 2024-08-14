/**
 * @file    PsBlock.h
 *
 * @brief   Class for contiguous monomer blocks. Manages block
 *          connectivity that is templated over propagator type
 *
 * @version $Id: PsBlock.h 8199 2007-09-05 05:07:11Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_BLOCK_H
#define PS_BLOCK_H

// standard headers
#include <list>
#include <map>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#undef HAVE_CONFIG_H
#endif

// txbase headers
#include <TxDebugExcept.h>

// pspolymer includes
#include <PsBlockBase.h>

/**
 * A PsBlock object contains the spatial part of a block.
 * This class derives from PsBlockBase and contains data/methods
 * that depend on the rank/structure of the q(X) propagator, but
 * does NOT depend on the details of the block model itself
 * (flexible, semi-flexible etc.)
 *
 * @param FLOATTYPE numeric type of the data.
 * @param NDIM dimensionality of the physical space
 * @param QTYPE PsField type for the q(X) part of q(X,s)
 */

template <class FLOATTYPE, size_t NDIM, class QTYPE>
class PsBlock : public virtual PsBlockBase<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor
 */
    PsBlock();

/**
 * Destructor must destroy blocks
 */
    virtual ~PsBlock();

/**
 * Set the parameters
 *
 * @param tas the parameters
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Build the data
 */
    virtual void buildData();

/**
 * Build the solvers structures
 */
    virtual void buildSolvers();

/**
 * Reset switches and common initial q values
 * before update methods
 */
    virtual void reset();

/**
 * Integrate [ q(X,s)*qt(X,s) ds ] and set the
 * QTYPE qqtIntegral data member
 *
 * @param bQ normalization factor for propagator (ie. bigQ)
 */
    virtual void setCalcQQTIntegral(FLOATTYPE bQ);

/**
 * Integrate [q(X,s')*qt(X,s') dX ]
 *
 * @return normalization Q value
 */
    virtual FLOATTYPE calcBigQ();

/**
 * Use the qqtIntegral result to calculate polymer chain
 * observables from block (this sets monomer density for now)
 */
    virtual void setPhysFields();

/**
 * Get final q(X) field for head/tail
 *
 * @param end endtype
 * @return    pointer to a data to transfer q(X) values
 */
    virtual PsFieldBase<FLOATTYPE>* getFinalQ(BlockEndType end);

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
                         PsBlockBase<FLOATTYPE, NDIM>* blockB,
                         BlockEndType blockBend,
                         size_t jntKey);

/**
 * Calculate constrained initial q values from all junction values
 * at head/tail. Uses setInitialQ to set "junctions"
 *
 * @param end head or tail
 */
    virtual void combineSetJnt(BlockEndType end);

  protected:

    // SWS: set by derived, for build-cycle purposes ??
    /** Dimensions of q(X): */
    std::vector<size_t> qDims;

    /** Total size of q(X): */
    size_t qTotalSize;

    /** Work space for initial q values */
    QTYPE q0;

    /** "Forward" q(s,X) propagator: for flexible --> q(r,s) */
    std::vector< QTYPE > qs;

    /** "Backward" q(s,X) propagator for flexible --> qt(r,s) */
    std::vector< QTYPE > qts;

/**
 * Get initial condition for q for head/tail
 *
 * @param end endtype
 * @return value of q needed to start calculation from end
 */
    virtual QTYPE& getInitialQ(BlockEndType end) {
      if (end == HEAD) return qHeadInitial;
      else             return qTailInitial;
    }

/**
 * Set final condition for q for head/tail and set switches
 * This is protected because derived classes set initial values
 * based on derived type.
 *
 * @param end head or tail
 * @param qf  q value from calculation to be set
 */
    virtual void setFinalQ(BlockEndType end, QTYPE& qf) {
      if (end == HEAD) {
        qHeadFinal = qf;
        this->qHeadFinalSet = 1;
      }
      else {
        qTailFinal = qf;
        this->qTailFinalSet = 1;
      }
    }

  private:

    /** Result holder for junctions */
    QTYPE qInit;

    /** Result holder for junctions */
    QTYPE qprod;

    /** Value of int [q(X,s)*qt(X,s) ds] */
    QTYPE qqtIntegral;

    /** Value of scaled qqtSum value */
    QTYPE qqtDens;

    /** Value of q at head as a result of a solve: to be sent */
    QTYPE qHeadFinal;

    /** Value of q at tail as a result of a solve: to be sent */
    QTYPE qTailFinal;

    /**
     * Value of q at head needed for a solve: to be received
     * Single q values after constraining for any other junction values
     */
    QTYPE qHeadInitial;

    /**
     * Value of q at tail needed for a solve: to be received
     * Single q values after constraining for any other junction values
     */
    QTYPE qTailInitial;

    /**
     * Value of q's from connnected blocks. Needs to be 'collasped'
     * into a single qHeadInitial value to account for constraints
     * due to junctions at head
     */
    std::map<size_t, QTYPE> qHeadJnts;

    /**
     * Value of q's from connnected blocks. Needs to be 'collasped'
     * into a single qTailInitial value to account for constraints
     * due to junctions at tail
     */
    std::map<size_t, QTYPE> qTailJnts;

/**
 * Set initial condition for q for head/tail
 *
 * @param end endtype
 * @param q0  q value to start calculation at end
 */
    virtual void setInitialQ(BlockEndType end, QTYPE& q0) {

      if (end == HEAD) {
        qHeadInitial = q0;
        this->qHeadInitialSet = 1;
      }
      else {
        qTailInitial = q0;
        this->qTailInitialSet = 1;
      }
    }

    /** Constructor private to prevent use */
    PsBlock(const PsBlock<FLOATTYPE, NDIM, QTYPE>& psb);

   /** Assignment private to prevent use */
    PsBlock<FLOATTYPE, NDIM, QTYPE>& operator=(
       const PsBlock<FLOATTYPE, NDIM, QTYPE>& psb);
};

#endif  // PS_BLOCK_H
