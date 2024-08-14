/**
 * @file    PsBlockCopolymer.h
 *
 * @brief   Class for block copolymers
 *
 * @version $Id: PsBlockCopolymer.h 6319 2006-11-14 22:39:46Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_BLOCK_COPOLYMER_H
#define PS_BLOCK_COPOLYMER_H

// std includes
#include <vector>
#include <map>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsPolymer.h>

// pspolymer includes
#include <PsBlockBase.h>
#include <PsBlockTypes.h>

/**
 * Solves for block copolymers, container class for PsBlock-s
 *
 * @param FLOATTYPE the data type of simulation
 * @param NDIM the dimensionality of simulation
 */
template <class FLOATTYPE, size_t NDIM>
class PsBlockCopolymer : public PsPolymer<FLOATTYPE, NDIM> {

  public:

/**
 * constructor
 */
  PsBlockCopolymer();

/**
 * Destructor
 */
  virtual ~PsBlockCopolymer();

/**
 * Store the data needed to build this object.
 *
 * @param tas the attribute set containing the initial conditions
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Build the data for this object
 */
    virtual void buildData();

/**
 * Build the solvers for this object
 */
    virtual void buildSolvers();

/**
 * Update copolymer, eg calculate q, q+ for all
 * member blocks... a driver for updateBlockQ()
 * and the normalization (bigQ) factor
 *
 * @param t simulation time
 */
    virtual void update(double t);

/**
 * Make other blocks aware of solution results on block
 * corresponding to block[fromIndex] at its end
 * This is the method that communicates q(X) values from
 * one block to another must know about type declarations
 * on all kinds of blocks.
 *
 * Example: if a flexible block is connected to a semiflexible
 *   block this implies a type conversion:
 *   PsField<FLOATTYPE, NDIM> --> PsField<FLOATTYPE, 2*NDIM-1>
 *   and vice-versa. A get call and subsequent set call for q(X)
 *   on different blocks is made here. Careful, the typical
 *   containership relation is being broken.
 *
 * @param end       head or tail
 * @param fromIndex index of block publishing results
 */
    virtual void publishQFrom(BlockEndType end, size_t fromIndex);

/**
 * Convert q(X) values depending on block block types
 * Code must be added to this method depending on the kinds
 * of derived block types.
 *
 * @param fromBlockPtr pointer to block from which to obtain Q values
 * @param toBlockPtr   pointer to block to   which to send   Q values
 */
    virtual void convertQ(PsBlockBase<FLOATTYPE, NDIM>* fromBlockPtr,
                          PsBlockBase<FLOATTYPE, NDIM>* toBlockPtr);

/**
 * Get the natural-log of the single-chain partition function
 * normalization value for monodisperse BCP
 *
 * @return log(bigQ)
 */
    virtual FLOATTYPE getLogBigQ() {
      return (FLOATTYPE) std::log(bigQ);
    }

  protected:

    /** Number of blocks */
    size_t numBlocks;

    /** List of block pointers */
    std::vector< PsBlockBase<FLOATTYPE, NDIM>* > blocks;

    /** List of block names */
    std::vector< std::string > blockNames;

    /** List of attrib sets for blocks */
    std::vector< TxHierAttribSetIntDbl > blockAttribs;

    /**
     * Check if all blocks are updated, helper method for update()
     *
     * @return are blocks solved for forward/backward propagators
     */
    virtual bool areAllBlocksUpdated() {
      bool allBlocksUpdated = 1; // initial set to yes
      for (size_t n=0; n<numBlocks; ++n)
        if (!blocks[n]->isBlockUpdated()) allBlocksUpdated = 0;
      return allBlocksUpdated;
    }

    /**
     * Update single block in copolymer, performing all
     * checks, solves, and updating q values, helper for update()
     *
     * @param nblock index of block to update
     * @param end    head / tail of blocks[nblock]
     */
    virtual void updateBlockQ(size_t nblock, BlockEndType end);

  private:

    /** Single chain partition function */
    FLOATTYPE bigQ;

    /**
     * Build connected block types: helper method for buildSolvers()
     *
     * @param iblock block index
     * @param end    head or tail
     */
    virtual void buildCntBlockType(size_t iblock, BlockEndType end);

    /** Constructor private to prevent use */
    PsBlockCopolymer(const PsBlockCopolymer<FLOATTYPE, NDIM>& psbcp);

   /** Assignment private to prevent use */
    PsBlockCopolymer<FLOATTYPE, NDIM>& operator=(
      const PsBlockCopolymer<FLOATTYPE, NDIM>& psbcp);
};

#endif // PS_BLOCK_COPOLYMER_H
