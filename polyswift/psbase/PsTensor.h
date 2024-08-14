/**
 *
 * @file    PsTensor.h
 *
 * @brief   Base class Tensors that expands on PsTensor by adding
 *          resize methods.
 *
 * @version $Id: PsTensor.h 8202 2007-09-05 10:03:47Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef PS_TENSOR
#define PS_TENSOR

// psbase
#include <PrTensor.h>

template <class FLOATTYPE, size_t NRANK>
class PsTensor : public PrTensor<FLOATTYPE, NRANK> {

  public:

#ifdef ASSERT
    FLOATTYPE operator()(size_t* indices) const {
      for (size_t i=0;i<NRANK;++i) {
        if (indices[i] >= this->lengths[i]) {
          TxDebugExcept tde;
          tde << "ERROR: Attempt to access outside tensor range";
          throw tde;
          std::cerr << "ERROR: Attempt to access outside tensor range"
                    << std::endl;
        }
      }
      return PrTensor<FLOATTYPE, NRANK>::operator()(indices);
    }

    FLOATTYPE operator()(size_t i) const {
      if (i >= this->size[0]) {
        TxDebugExcept tde;
        tde << "ERROR: Attempt to access outside tensor range";
        throw tde;
        std::cerr << "ERROR: Attempt to access outside tensor range"
                  << std::endl;
      }
      return PrTensor<FLOATTYPE, NRANK>::operator()(i);
    }

    FLOATTYPE& operator()(size_t* indices) {
      for (size_t i=0;i<NRANK;++i) {
        if (indices[i] >= this->lengths[i]) {
          TxDebugExcept tde;
          tde << "ERROR: Attempt to access outside tensor range";
          throw tde;
          std::cerr << "ERROR: Attempt to access outside tensor range"
                    << std::endl;
        }
      }
      return PrTensor<FLOATTYPE, NRANK>::operator()(indices);
    }

    FLOATTYPE& operator()(size_t i) {
      if (i >= this->size[0]) {
        TxDebugExcept tde;
        tde << "ERROR: Attempt to access outside tensor range";
        throw tde;
        std::cerr << "ERROR: Attempt to access outside tensor range"
                  << std::endl;
      }
      return PrTensor<FLOATTYPE, NRANK>::operator()(i);
    }
#endif

/**
 * get the length of a given dimension
 *
 * @param i the given dimension
 */
    size_t getLength(size_t i) const {
      return PrTensor<FLOATTYPE, NRANK>::getLength(i);
    }

/**
 * Total size of tensor
 */
    size_t getSize() const {
      return PrTensor<FLOATTYPE, NRANK>::getSize();
    }

/**
 * get the size of a given dimension
 *
 * @param i the given dimension
 */
    size_t getSize(size_t i=0) const {
      return PrTensor<FLOATTYPE, NRANK>::getSize(i);
    }

  protected:

  private:

};

#endif

