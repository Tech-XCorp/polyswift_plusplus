/**
 * @file    PsFieldHist.cpp
 *
 * @brief   Base class for field diagnostics
 *
 * @version $Id: PsFieldHist.cpp 11989 2009-03-25 05:14:55Z dws $
 *
 * Copyright &copy; 2009-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// vpbase includes
#include <PsGridField.h>
#include <PsFldMsgCntr.h>

//
// constructor
//
template <class FLOATTYPE, size_t NDIM, class ELEMENTTYPE, class DATATYPE>
PsFieldHist<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::PsFieldHist() {
  fieldHistPtr = 0;
}

//
// destructor
//
template <class FLOATTYPE, size_t NDIM, class ELEMENTTYPE, class DATATYPE>
PsFieldHist<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::~PsFieldHist() {
}

//
//
template <class FLOATTYPE, size_t NDIM, class ELEMENTTYPE, class DATATYPE>
void PsFieldHist<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::buildSolvers() {

  // Scoping call to buildSolvers
  PsHistory<FLOATTYPE, NDIM, ELEMENTTYPE, DATATYPE>::buildSolvers();

  // Look for a single field, set if there, otherwise exception
  if (this->getAttrib().hasString("field")) {

    std::string fieldName = this->getAttrib().getString("field");
    try {
      fieldHistPtr
        = PsNamedObject::getObject<PsGridField<FLOATTYPE, NDIM> >(fieldName);
    }
    catch (TxDebugExcept & tde2) {
      tde2 << "\nPsFieldHist::buildSolvers: unknown field " <<
        fieldName << " requested in <History " << this->getName() << " >"
        "\n(If field is in a <MultiField> or <EmField>, is it made available "
        "using the attribute 'publicFields = [...]'?)";
      throw;
    }

    fieldPtrList.push_back(fieldHistPtr);
  }

  // Look for many fields, set pointer list if there, otherwise exception
  else if (this->getAttrib().hasStrVec("fields")) {

    std::vector<std::string> const & fieldNames =
      this->getAttrib().getStrVec("fields");

    // Store PsGridField pointers in vector
    for (size_t i=0; i<fieldNames.size(); ++i) {

      try {
        fieldHistPtr = PsNamedObject
          ::getObject<PsGridField<FLOATTYPE, NDIM> >(fieldNames[i]);
      }
      catch (TxDebugExcept & tde2) {
        tde2 << "\nPsFieldHist::buildSolvers: unknown field " <<
          fieldNames[i] << " requested in <History " << this->getName() << " >"
        "\n(If field is in a <MultiField> or <EmField>, is it made available "
        "using the attribute 'publicFields = [...]' within the MultiField?)";
        throw;
      }

      fieldPtrList.push_back(fieldHistPtr);
    }

  }
  else {
    TxDebugExcept tde("PsFieldHist::buildSolvers: attribute set "
      "doesn't specify 'field' or 'fields'");
    tde << " in <History " << this->getName() << " >";
    throw tde;
  }
}

// Instantiate history scalar classes rank = 0
template class PsFieldHist<float, 1, float, float>;
template class PsFieldHist<float, 2, float, float>;
template class PsFieldHist<float, 3, float, float>;

template class PsFieldHist<double, 1, double, double>;
template class PsFieldHist<double, 2, double, double>;
template class PsFieldHist<double, 3, double, double>;

// Instantiate history field classes rank = 1
template class PsFieldHist<float, 1, float, TxTensor<float, 1> >;
template class PsFieldHist<float, 2, float, TxTensor<float, 1> >;
template class PsFieldHist<float, 3, float, TxTensor<float, 1> >;

template class PsFieldHist<double, 1, double, TxTensor<double, 1> >;
template class PsFieldHist<double, 2, double, TxTensor<double, 1> >;
template class PsFieldHist<double, 3, double, TxTensor<double, 1> >;

// Instantiate history field classes rank = 2
template class PsFieldHist<float, 1, float, TxTensor<float, 2> >;
template class PsFieldHist<float, 2, float, TxTensor<float, 2> >;
template class PsFieldHist<float, 3, float, TxTensor<float, 2> >;

template class PsFieldHist<double, 1, double, TxTensor<double, 2> >;
template class PsFieldHist<double, 2, double, TxTensor<double, 2> >;
template class PsFieldHist<double, 3, double, TxTensor<double, 2> >;

// Instantiate history field classes rank = 3
template class PsFieldHist<float, 2, float, TxTensor<float, 3> >;
template class PsFieldHist<double, 2, double, TxTensor<double, 3> >;

template class PsFieldHist<float, 3, float, TxTensor<float, 3> >;
template class PsFieldHist<double, 3, double, TxTensor<double, 3> >;
