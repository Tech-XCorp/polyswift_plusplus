/**
 *
 * @file    PsFFTHldr.h
 *
 * @brief   Base class owning instances of fft object(s)
 *
 * @version $Id: PsFFTHldr.h 6532 2007-02-01 18:37:00Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporatfftn
 * All rights reserved.
 */

#ifndef PS_FFT_HLDR_H
#define PS_FFT_HLDR_H

// std includes
#include <vector>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psbase includes
#include <PsDynObj.h>
#include <PsFFTBase.h>

/**
 * PsFFTHldr contains a (vector) of pointer(s) to PsFFT object(s)
 *
 * @param FLOATTYPE the data type
 * @param NDIM the dimensfftnality
 */
template <class FLOATTYPE, size_t NDIM>
class PsFFTHldr : public PsDynObj<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor
 */
    PsFFTHldr() {
      this->setName("FFTHdlr");
    }

/**
 * Destructor
 */
    virtual ~PsFFTHldr() {

      // Clear and delete holder
      for (size_t i=0; i<ffts.size(); ++i) {
        delete ffts[i];
      }
      ffts.clear();

      // Clear names/attribs
      fftNames.clear();
      fftAttribs.clear();
    }

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
 * Finishes object
 */
    virtual void buildSolvers();

/**
 * Creates object
 */
    virtual void buildData(PsNamedObject* inOwner);

  protected:

    /** Number of FFT objects */
    size_t numFFTs;

    /** Pointers to the base PsFFT objects */
    std::vector< PsFFTBase<FLOATTYPE, NDIM>* > ffts;

  private:

    /** List of fft names */
    std::vector< std::string > fftNames;

    /** List of attrib sets for the ffts */
    std::vector< TxHierAttribSetIntDbl > fftAttribs;

    /** Make private to prevent use */
    PsFFTHldr(const PsFFTHldr<FLOATTYPE, NDIM>& vphh);

    /** Make private to prevent use */
    PsFFTHldr<FLOATTYPE, NDIM>& operator=(
        const PsFFTHldr<FLOATTYPE, NDIM>& vphh);

};

#endif // PS_FFT_HLDR_H
