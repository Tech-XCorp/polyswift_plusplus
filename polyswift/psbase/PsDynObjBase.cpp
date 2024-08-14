/**
 *
 * @file    PsDynObjBase.cpp
 *
 * @brief   Instantiation of base class for dynamic objects that have IO
 *
 * @version $Id: PsDynObjBase.cpp 6319 2006-11-14 22:39:46Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// psbase includes
#include <PsDynObjBase.h>

//
// Default constructor
//
PsDynObjBase::PsDynObjBase() {
}

//
// Construct with name
//
PsDynObjBase::PsDynObjBase(const std::string& nm) {
}

//
// Copy Constructor
//
PsDynObjBase::PsDynObjBase(const PsDynObjBase& dobj) {
  setAttrib(dobj.getAttrib());
}

//
// assigment
//
PsDynObjBase& PsDynObjBase::operator=(const PsDynObjBase& dobj) {
  setAttrib(dobj.getAttrib());
  return *this;
}

//
// Destructor
//
PsDynObjBase::~PsDynObjBase() {
}

//
// Set attribute set
//
void PsDynObjBase::setAttrib(const TxHierAttribSetIntDbl& tas) {
  settings = tas;
}

//
// Declare static data
//
size_t PsDynObjBase::currDomainStep;
size_t PsDynObjBase::totNumDomainStep;

