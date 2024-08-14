/*
   A C-program for MT19937, with initialization improved 2002/2/10.
   Coded by Takuji Nishimura and Makoto Matsumoto.
   This is a faster version by taking Shawn Cokus's optimization,
   Matthe Bellew's simplification, Isaku Wada's real version.

   Before using, initialize the state by using init_genrand(seed)
   or init_by_array(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote
        products derived from this software without specific prior written
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   Any feedback is very welcome.
   http://www.math.keio.ac.jp/matumoto/emt.html
   email: matumoto@math.keio.ac.jp
*/

//#include <PsDebug.h>
#include <PsRandom.h>

template <class FLOATTYPE>
void PsRandom<FLOATTYPE>::setAttrib(const TxHierAttribSetIntDbl& tas) {
  size_t sd;

  if (tas.hasOption("seed")) {
    sd = (size_t) tas.getOption("seed");
    setSeed(sd);
  }
}

template <class FLOATTYPE>
void PsRandom<FLOATTYPE>::next_state(void) const {
    size_t* p=state;
    int j;

    // if init_genrand() has not been called,
    // a default initial seed is used
    // Not needed here, as initialization is always
    // done by the class constructor
    // if (initf==0) init_genrand(5489UL);

    left = N;
    next = state;

    for (j=N-M+1; --j; p++)
        *p = twist(p[M], p[0], p[1]);

    for (j=M; --j; p++)
        *p = twist(p[M-N], p[0], p[1]);

    *p = twist(p[M-N], p[0], state[0]);
}

/* initializes state[N] with a seed */

template <class FLOATTYPE>
void PsRandom<FLOATTYPE>::init_genrand(size_t s) const {
    int j;
    state[0]= s & 0xffffffffUL;
    for (j=1; j<N; j++) {
        state[j] = (1812433253UL * (state[j-1] ^ (state[j-1] >> 30)) + j);
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        /* In the previous versions, MSBs of the seed affect */
        /* only MSBs of the array state[].                        */
        /* 2002/01/09 modified by Makoto Matsumoto */
        state[j] &= 0xffffffffUL;  /* for >32 bit machines */
    }
    left = 1; // initf = 1;
}

/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* key_length is its length */

template <class FLOATTYPE>
void PsRandom<FLOATTYPE>::init_by_array (size_t init_key[], size_t key_length) {
    size_t i, j, k;
    init_genrand(19650218UL);
    i=1; j=0;
    k = (N>key_length ? N : key_length);
    for (; k; k--) {
        state[i] = (state[i] ^ ((state[i-1] ^ (state[i-1] >> 30)) * 1664525UL))
          + init_key[j] + j; /* non linear */
        state[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++; j++;
        if (i>=N) { state[0] = state[N-1]; i=1; }
        if (j>=key_length) j=0;
    }
    for (k=N-1; k; k--) {
    state[i] = (state[i] ^ ((state[i-1] ^ (state[i-1] >> 30)) *
        1566083941UL)) - i; /* non linear */
        state[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++;
        if (i>=N) { state[0] = state[N-1]; i=1; }
    }

    state[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */
    left = 1; // initf = 1;
}

/* Get or set the state of the RNG object */
template <class FLOATTYPE>
typename PsRandom<FLOATTYPE>::state_t* PsRandom<FLOATTYPE>::getState() {
  state_t* st = new state_t;
  for (size_t i=0; i<N; i++) st->state[i] = state[i];
  st->left = left;
  return st;
}

template <class FLOATTYPE>
void PsRandom<FLOATTYPE>::setState(state_t* st) {
  for (size_t i=0; i<N; i++) state[i] = st->state[i];
  left = st->left;
  next = &state[0]+(N+1-st->left);
}


// Instantiations

// #ifdef HAVE_GENERALLY_DECLARED_STATICS
// template <class FLOATTYPE> int PsRandom<FLOATTYPE>::rank = 0;
// #else
// template <> int PsRandom<float>::rank = 0;
// template <> int PsRandom<double>::rank = 0;
// #endif
template class PsRandom<float>;
template class PsRandom<double>;

// A global PsRandom<> object for use with psRandom<>()

PsRandom<double> theRng;
PsRandom<double> theRngGlobal;
