/**
 * @file    PsRandom.h
 *
 * @brief   Interfaces and implementations (!) for cross-platform random
 *          number generator classes PsRandom and PsSrandom.
 *
 * @version $Id: PsRandom.h 6423 2007-01-11 20:32:39Z sizemore $
 * Copyright &copy; 2007-2020, Tech-X Corporation
 */

#ifndef PS_RANDOM_H
#define PS_RANDOM_H

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// standard includes
#include <cstdlib>
#include <climits>

// txbase includes
#include <txc_math.h>

// psstd includes
#include <PsNoArgFunc.h>


/**
 * Get a random number between 0 and 1, using the
 * global random number generator.
 *
 * FLOATTYPE is the type of floating point number
 *
 * @return a random number between 0 and 1
 */
template <class FLOATTYPE> FLOATTYPE psRandom();
template <class FLOATTYPE> FLOATTYPE psRandomGaussian();

/**
 * Initialize seed for the global random number generator.
 *
 * @param seed the random number seed
 */
void psSrandom(size_t seed);

/**
 * @class PsRandom
 *
 * A functor that takes no arguments and returns a random
 * floating-point number in the interval [0, 1). It contains
 * an implementation of the Mersenne Twister by Takuji Nishimura
 * and Makoto Matsumoto, as well as contributions by Shawn Cokus
 * and Matthew Bellew. See accompanying file PsRandom.cpp for
 * copyright notice and licence.
 */
template <class FLOATTYPE>
class PsRandom : public PsNoArgFunc<FLOATTYPE> {

  public:

/**
 * Default and only constructor
 */
    PsRandom() {
      static size_t globalSeed = 0;
      size_t init[4]={0x123, 0x234, 0x345, 0x456}, length=4;
//      if (rank != -1) init[2] += rank;
      init[3] += globalSeed;
      init_by_array(init, length);
      globalSeed++;

      iset = 0;
      gset = 0.0;
    }

/**
 * Destructor
 */
    virtual ~PsRandom() {
    }

/**
 * Set up the functor from data in an attribute set
 *
 * @param tas An attribute set containing the parameters of the
 * functor.
 *
 * The attributes relvant to this class within tas are:
 *
 * - @c seed (real scalar): the seed for this random number generator
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);



/**
 * Set iset
 *
 * @param isett iset
 */
    virtual void setIset(size_t isett) {
      iset = isett;
    }

/**
 * Set gset
 *
 * @param gsett gset
 */
    virtual void setGset(FLOATTYPE gsett) {
      gset = gsett;
    }


    /////////////////////////////////////////////


/**
 * Get iset
 */
    virtual size_t getIset() {
      return iset;
    }

/**
 * Get gset
 */
    virtual FLOATTYPE getGset() {
      return gset;
    }

/**
 * Set the rank for this random number generator
 *
 * @param r the rank
    virtual void setRank(int r) {
      rank = r;
    }
 */

/**
 * Set a seed for this functor (useful for pseudo-random number generators)
 *
 * @param sd the seed
 */
    virtual void setSeed(size_t sd) {
        init_genrand(sd);
    };

/**
 * Generate a random integer in the interval [0:0xFFFFFFFF]
 *
 * @return a random integer
 */
    inline size_t int32(void) const;

/**
 * Generate a random floating point number in the interval [0, 1)
 * with 32-bit resolution.
 *
 * @return a random floating point number
 */
    inline FLOATTYPE real(void) const {
#if (LONG_MAX == 2147483647L)
      return (FLOATTYPE)(0.5+((long)(int32()^LONG_MIN))*(1.0/4294967296.0));
#else
      return (FLOATTYPE)((long)int32())*(1.0/4294967296.0);
#endif
    }

/**
 * Generate a random floating point number in the interval [0, 1)
 * with 53-bit resolution. Written 2002/01/09 by Isaku Wada.
 *
 * @return a random floating point number
 */
    inline double real53(void) {
      long a = int32() >> 5, b = int32() >> 6;
      return(a*67108864.0+b)*(1.0/9007199254740992.0);
    }

/**
 * Generate a random floating point number in the interval [0, 1)
 * with 32-bit resolution.
 *
 * @return a random floating point number
 */
    virtual FLOATTYPE operator()() const {
// JRC mods
      FLOATTYPE res = real();
      while (res == 1.) res = real();
      return res;
    }

  public:

/** Length of the state vector.  */
    enum { N = 624 };

/**
 * Structure type to store the state of the random number generator
 */
    typedef struct {

/**
 * State array
 */
      size_t state[N];

/**
 * Current position in state array
 */
      int left;
    } state_t;

/**
 * Get the current state of the random number generator
 *
 * @return Pointer to a structure containing the current state
 */
    state_t* getState(void);

/**
 * Set the state of the random number generator, e.g. to
 * restore a previously saved state
 *
 * @param st Pointer to a structure containing the new state
 */
    void setState(state_t* st);

  private:

/**
 * Initialize the random number generator with a single 32-bit
 * seed.
 *
 * @param s the seed
 */
    void init_genrand(size_t s) const;

/**
 * Initialize the random number generator with an array of
 * 32-bit integers.
 *
 * @param init_key array to seed the generator with
 * @param key_length length of the seed array
 */
    void init_by_array(size_t init_key[], size_t key_length);

/**
 * Compute a new state vector once all elements of the current
 * state vector have been used.
 */
    void next_state(void) const;

/**
 * For internal use only.
 */
    size_t twist(size_t m, size_t u, size_t v) const {
      return m ^ (mixbits(u, v) >> 1) ^ (-(v&1UL) & 0x9908b0dfUL);
    }

/**
 * For internal use only.
 */
    size_t mixbits(size_t u, size_t v) const {
      return ((u) & 0x80000000UL) | ((v) & 0x7fffffffUL);
    }

/** the rank for parallel computing */
//    static int rank;

/** Period parameter.  */
    enum { M = 397 };


/**
 * State vector.
 */
    mutable size_t state[N];

/**
 * Pointer to next element to be taken from state vector.
 */
    mutable size_t* next;

/**
 * Number of unused values left in state vector.
 */
    mutable int left;

/** Flag for gaussian generator */
    size_t iset;

/** Gaussian rand number */
    FLOATTYPE gset;

};


// generates a random number on [0, 0xffffffff]-interval
template <class FLOATTYPE>
inline size_t PsRandom<FLOATTYPE>::int32(void) const {
    size_t y;

    if (--left == 0) next_state();
    y = *next++;

    // Tempering
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}

/** A global (but per/proc) PsRandom<> object for use with psRandom<>() */
extern PsRandom<double> theRng;

/** A global and sync-ed PsRandom<> object for use with psRandom<>() */
extern PsRandom<double> theRngGlobal;

template <class FLOATTYPE>
inline FLOATTYPE psRandom() {
  return (FLOATTYPE) theRng.real();
}


template <class FLOATTYPE>
inline FLOATTYPE psRandomGlobal() {
  return (FLOATTYPE) theRngGlobal.real();
}


template <class FLOATTYPE>
inline FLOATTYPE psRandomGaussian() {

  FLOATTYPE v1, v2, gset;
  FLOATTYPE uniRand, r, fac, gasdev;
  size_t rint;
  size_t iset = theRng.getIset();

  if (iset == 0) {

    do {
      uniRand = (FLOATTYPE) theRng.real();
      v1 = (2.0*uniRand) - 1.0;
      uniRand = (FLOATTYPE) theRng.real();
      v2 = (2.0*uniRand) - 1.0;
      r = (v1*v1) + (v2*v2);
    }
    while ((r >= 1.0) || (r == 0.0) );

    fac = sqrt(-2.0*log(r)/r);
    gset = v1*fac;
    theRng.setGset(gset);

    gasdev = v2*fac;
    iset = 1;
    theRng.setIset(iset);
  }

  else {
    gasdev = theRng.getGset();
    iset = 0;
    theRng.setIset(iset);
  }

  return gasdev;
}

inline void psSrandom(size_t seed) {
  theRng.setSeed(seed);
}

inline void psSrandomGlobal(size_t seed) {
  theRngGlobal.setSeed(seed);
}


#endif // PS_RANDOM_H
