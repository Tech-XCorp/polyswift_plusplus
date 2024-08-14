/**
 *
 * @file    PsLicensing.h
 *
 * @brief   PolySwift++ package/module mapping info
 *
 * @version $Id: PsLicensing.h 421 2012-07-29 01:05:00Z swsides $
 *
 * Copyright &copy; 2012-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */
#ifndef PS_LICENSING_H
#define PS_LICENSING_H

#include "TxLicensing.h"

/**
 * A PsLicensing object contains info for
 * PolySwift++ name mapping
 */
class PsLicensing : public virtual TxLicensing {

  public:

/**
 * Constructor
 */
  PsLicensing() {

    // Base License
    std::string baseClasses(" \
      fieldF1R fieldF2R fieldF3R fieldF4R fieldF5R fieldF6R         \
      fieldD1R fieldD2R fieldD3R fieldD4R fieldD5R fieldD6R         \
      mpiComm normalfftw transposefftw uniCartGrid Domain3D Domain3F \
      freeEnergy regular fftw canonicalMF flory steepestDescent     \
      floryConstChi floryChiAtPoint monomerDens constraint          \
      flexPseudoSpec blockCopolymer simpleSolvent                   \
      expression cutExpression chiCutExpression pyfunc              \
      simpleSpecFilter multiSpecFilter movTanhSlab switchMovTanhSlab");
    std::string baseMacrofiles("pseudoSpecSetup");
    this->set("PSimBase", baseClasses, baseMacrofiles);
    this->add("PSimBase"); // adds by default

    // Plus License
    std::string plusClasses(" \
      sphereData wallData fixedWall floryWall                       \
      interactingSphere probeSphere                                 \
      chomboF2R chomboF3R chomboF4R chomboF5R chomboF6R             \
      chomboD1R chomboD2R chomboD3R chomboD4R chomboD5R chomboD6R   \
      chomboGrid chomboMonomerDens chomboConstraint flexChomboSplit \
      polyDisperseBCP");
    std::string plusMacrofiles("polyDisperse");
    this->set("PSimPlus", plusClasses, plusMacrofiles);

    // All Functionality License
    std::string allPkgClasses("allobjects");
    std::string allPkgMacrofiles("allmacrofiles");
    this->set("All", allPkgClasses, allPkgMacrofiles);
  }

/**
 * Destructor
 */
  virtual ~PsLicensing() {}

  protected:

  private:

    /** Make private to prevent use */
    PsLicensing(const PsLicensing& txm);
    /** Make private to prevent use */
    PsLicensing& operator=(const PsLicensing& txm);
};

#endif // PS_LICENSING_H
