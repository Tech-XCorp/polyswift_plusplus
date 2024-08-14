/**
 * @file    PsPolyDisperseBCP.h
 *
 * @brief   Class for polydisperse block copolymers
 *
 * @version $Id: PsPolyDisperseBCP.h 6319 2006-11-14 22:39:46Z swsides $
 *
 * Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_POLY_DISPERSE_BCP_H
#define PS_POLY_DISPERSE_BCP_H

// std includes
#include <vector>
#include <map>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// pspolymer includes
#include <PsBlockCopolymer.h>
#include <PsGaussLagQuad.h>

/**
 * Solves for block copolymers, container class for PsBlock-s
 * Generates 'copies' of appropriate blocks for quadrature points
 * in an integration approx over a length distribution

 * @param FLOATTYPE the data type of simulation
 * @param NDIM the dimensionality of simulation
 */
template <class FLOATTYPE, size_t NDIM>
class PsPolyDisperseBCP : public PsBlockCopolymer<FLOATTYPE, NDIM> {

  public:

/**
 * constructor
 */
  PsPolyDisperseBCP();

/**
 * Destructor
 */
  virtual ~PsPolyDisperseBCP();

/**
 * Store the data needed to build this object.
 * Takes attributes for <Block> objects and copies/edits
 * for attributes for 'quadrature block' points
 *
 * After this call the following parent data members are
 * reset to new 'quad' values
 *   - this->numBlocks    = numSeedBlocks*n_g;
 *   - this->blockNames   = quadBlockNames;
 *   - this->blockAttribs = quadBlockAttribs;
 *
 * Sets initial BlockGrp data std::vector<BlockGrp> blockGroups
 * except for copies to PsBlockBase objects which must be done
 * at the buildData step.
 *
 * @param tas the attribute set for <Polymer> object
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Creates block objects (now for all the quad block attributes)
 * First section is almost identical to PsBlockCopolymer::buildData
 * but section at end is different for length fraction sum checks.
 */
    virtual void buildData();

/**
 * Update polydisperse copolymer
 * ...
 *
 * @param t simulation time
 */
    virtual void update(double t);

/**
 * Get the natural-log of the single-chain partition function
 * normalization value. For polydispserse BCP this is a weighted
 * sum of log(Q) 's
 *
 * @return ~sum[log(bigQ)]
 */
    virtual FLOATTYPE getLogBigQ();

  protected:

  private:

    /** Single chain partition function */
    //    std::vector<FLOATTYPE> bigQVec;

    /** Number of quadrature points */
    size_t n_g;

    /** exponent for Schulz distribution */
    FLOATTYPE alpha;

    /** Pointer to generalized Gauss-Laguerre quadrature calculator */
    PsGaussLagQuad* quadGL;

    /** Name of polydisperse block */
    std::string polyBlockName;

    /** vector of scaled lengths for entire polydisperse chain */
    std::vector<FLOATTYPE> mk_vec;

    /** vector of scaled lengths for polydisperse block */
    std::vector<FLOATTYPE> fmk_vec;

/**
 * Calculate average contribution to overall length fraction
 * Correct scaling factor for polydisperse block and monodisperse block
 *
 * @param  blkName name of block
 * @param  wt quadrature weight used to calculate length weight
 * @return length weight
 */
    virtual FLOATTYPE calcLengthWeight(const std::string& blkName,
                                       FLOATTYPE wt);

/**
 * Checks if attribute set is from the polydispserse block
 * Needs 'polyBlockName' set
 *
 * @param attrib block attribute set
 * @return bool for is polydisperse block
 */
    bool isAttribPoly(const TxHierAttribSetIntDbl& attrib);

/**
 * Takes polydisperse block attributes and calculates
 * quadrature parameters, especially the scaled length fractions
 * of 'generated' block attributes for quadrature points
 *
 * @param blockAttrib attribute set of polydisperse block
 */
    void calcPolyParams(const TxHierAttribSetIntDbl& blockAttrib);

/**
 * Helper method check new attributes. Uses txbase 'writeBody'
 * method to dump attribute sets to file and check length fractions
 */
    void checkBlkGroups();

/**
 * Helper method to convert block name to quadrature block names
 * and create attribute sets for these internal <Block> objects
 *
 * @param  blockAttrib
 * @param  quadIndex quadrature weight index
 * @return copied attribute set corresponding to a quadrature block spec
 */
    TxHierAttribSetIntDbl makeQuadAttrib(const TxHierAttribSetIntDbl& blockAttrib,
                                   size_t quadIndex);

/**
 * Make block name copy for quadrature contribution.
 * eg blockA ---> blockA_ng_0 (or blockB_ng_2....)
 *
 * @param  objName name of 'template' block
 * @param  index   index of quadrature point
 * @return new string name for block
 */
    std::string makeQuadName(const std::string& objName, size_t index);

/**
 * Structure that maps original block name and attrib
 * to vectors of quad block name, attribs and auxillary values
 *
 * Members:
 *    quadIndex   - quadrature index
 *    origName    - original block name used as template
 *    origAttrib  - original attrib set used as template
 *    quadNames   - vec of quad block names
 *    quadAttribs - vec of quad attribs
 *    quadBigQs   - vec of bigQ values per quad block
 *    quadBlkPtrs - pointers to quad blocks (copied in base class)
 */
    struct BlockGrp {
      std::string                                  origName;
      TxHierAttribSetIntDbl                              origAttrib;
      std::vector<std::string>                     quadNames;
      std::vector<TxHierAttribSetIntDbl>                 quadAttribs;
      std::vector<FLOATTYPE>                       quadBigQs;
      std::vector< PsBlockBase<FLOATTYPE, NDIM>* > quadBlkPtrs;
    };

    /** Vector of block info struc-s*/
    std::vector<BlockGrp> blockGroups;

    /** Number of orignal seed blocks */
    size_t numSeedBlocks;

    /** List of seed(orignal) block names */
    std::vector< std::string > seedBlockNames;

    /** List of attrib sets for seed blocks */
    std::vector< TxHierAttribSetIntDbl > seedBlockAttribs;

    /** List of block names */
    std::vector< std::string > quadBlockNames;

    /** List of attrib sets for blocks */
    std::vector< TxHierAttribSetIntDbl > quadBlockAttribs;

    /** Constructor private to prevent use */
    PsPolyDisperseBCP(const PsPolyDisperseBCP<FLOATTYPE, NDIM>& psbcp);

   /** Assignment private to prevent use */
    PsPolyDisperseBCP<FLOATTYPE, NDIM>& operator=(
      const PsPolyDisperseBCP<FLOATTYPE, NDIM>& psbcp);
};

#endif // PS_POLY_DISPERSE_BCP_H
