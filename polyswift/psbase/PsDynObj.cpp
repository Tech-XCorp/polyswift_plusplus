/**
 *
 * @file    PsDynObj.cpp
 *
 * @brief   Instantiation of base class for dynamic objects that have IO
 *
 * @version $Id: PsDynObj.cpp 8061 2007-08-16 15:48:09Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// psbase includes
#include <PsDynObj.h>

//
// Default constructor
//
template <class FLOATTYPE, size_t NDIM>
PsDynObj<FLOATTYPE, NDIM>::PsDynObj() {
  curTime = 0.;
  dumpPeriod = 1;
  dbStatus = PSDB_OFF;
  domSingsPtr = NULL;
}

//
// Constructor with name setting
//
template <class FLOATTYPE, size_t NDIM>
PsDynObj<FLOATTYPE, NDIM>::PsDynObj(std::string nm) : PsDynObjBase(nm) {
  curTime = 0.;
  dumpPeriod = 1;
}

//
// Delete all unused resources
//
template <class FLOATTYPE, size_t NDIM>
PsDynObj<FLOATTYPE, NDIM>::~PsDynObj() {
}

//
// Copy Constructor
//
template <class FLOATTYPE, size_t NDIM>
PsDynObj<FLOATTYPE, NDIM>::PsDynObj(const PsDynObj<FLOATTYPE, NDIM>& dobj) : PsDynObjBase(dobj) {
  domSingsPtr = dobj.domSingsPtr;
  curTime = dobj.curTime;
  dumpPeriod = dobj.dumpPeriod;
}

//
// assigment
//
template <class FLOATTYPE, size_t NDIM>
PsDynObj<FLOATTYPE, NDIM>& PsDynObj<FLOATTYPE, NDIM>::operator=(
    const PsDynObj<FLOATTYPE, NDIM>& dobj) {

  // use member by member == in case of future virtual inheritance
  //  if (*this == dobj) return *this;
  curTime = dobj.curTime;
  domSingsPtr = dobj.domSingsPtr;
  dumpPeriod = dobj.dumpPeriod;
  return *this;
}

//
// equality operator
//
template <class FLOATTYPE, size_t NDIM>
void PsDynObj<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Base class
  PsDynObjBase::setAttrib(tas);

  if (tas.hasOption("dumpPeriod"))
    dumpPeriod = tas.getOption("dumpPeriod");
}

// build data
template <class FLOATTYPE, size_t NDIM>
void PsDynObj<FLOATTYPE, NDIM>::buildData() {}
// build solvers
template <class FLOATTYPE, size_t NDIM>
void PsDynObj<FLOATTYPE, NDIM>::buildSolvers() {}

//
// print flagged debug messages to single stream
//
template <class FLOATTYPE, size_t NDIM>
void PsDynObj<FLOATTYPE, NDIM>::dbprt(std::string msgStr) {

  //  std::cout << "dbStatus in dbprt PsDynObj set to = " << dbStatus << std::endl;
  //  std::cout << "enum ON  = " << ON << std::endl;
  //  std::cout << "enum OFF = " << OFF << std::endl;
  if (dbStatus==PSDB_ON) {
    size_t thisRank = domSingsPtr->getDomRank();
    if (!thisRank ) {
      // std::cout << "i am printing out dbStatus = " << dbStatus << std::endl;
      std::cout << msgStr << std::endl;
    }}
}

//
// print flagged debug messages to single stream
//
template <class FLOATTYPE, size_t NDIM>
void PsDynObj<FLOATTYPE, NDIM>::dbprt(std::string msgStr1,
    std::string msgStr2) {

  msgStr1.append(msgStr2);
  dbprt(msgStr1);
}

//
// print flagged debug messages to single stream
//
template <class FLOATTYPE, size_t NDIM>
void PsDynObj<FLOATTYPE, NDIM>::dbprt(std::string msgStr1,
    int inum) {

  //  int f = 23;
  //  std::string f_str = std::to_string(f);
  //  std::cout << f_str << '\n';

  std::string intStr = std::to_string(inum);

  // Pre C++11 version (release 1.0 code)
  //  std::string intStr =
  //    static_cast<std::ostringstream*>(&(std::ostringstream()<< inum) )->str();

  //  char istr[10];
  //  std::string msgStr2;
  // Convert integer to char string
  //  sprintf(istr, "%i", inum);
  //  msgStr2 = istr;

  msgStr1.append(intStr);
  dbprt(msgStr1);
}

//
// print flagged debug messages to single stream
//
template <class FLOATTYPE, size_t NDIM>
void PsDynObj<FLOATTYPE, NDIM>::dbprt(std::string msgStr1,
    FLOATTYPE fnum) {

  //  FLOATTYPE floatVal = fnum;
  //  std::string floatStr =
  //    static_cast<std::ostringstream*>(&(std::ostringstream()<< fnum) )->str();
  // Pre C++11 version

  std::string floatStr = std::to_string(fnum);

  //  char istr[10];
  //  std::string msgStr2;
  // Convert integer to char string
  //  sprintf(istr, "%.4g", fnum);
  //  msgStr2 = istr;

  msgStr1.append(floatStr);
  dbprt(msgStr1);
}

//
// print flagged debug messages to single stream
//
template <class FLOATTYPE, size_t NDIM>
void PsDynObj<FLOATTYPE, NDIM>::pprt(std::string msgStr1,
    FLOATTYPE fnum) {

  //  std::string floatStr =
  //    static_cast<std::ostringstream*>(&(std::ostringstream()<< fnum) )->str();
  // Pre C++11 version

  std::string floatStr = std::to_string(fnum);

  //  char istr[10];
  //  std::string msgStr2;
  // Convert integer to char string
  //  sprintf(istr, "%.4g", fnum);
  //  msgStr2 = istr;

  msgStr1.append(floatStr);
  pprt(msgStr1);
}

//
// print flagged debug messages to single stream
//
template <class FLOATTYPE, size_t NDIM>
void PsDynObj<FLOATTYPE, NDIM>::pprt(std::string msgStr1,
    int inum) {

  //  std::string intStr =
  //    static_cast<std::ostringstream*>(&(std::ostringstream()<< inum) )->str();
  // Pre C++11 version

  std::string intStr = std::to_string(inum);

  //  char istr[10];
  //  std::string msgStr2;
  // Convert integer to char string
  //  sprintf(istr, "%i", inum);
  //  msgStr2 = istr;

  msgStr1.append(intStr);
  pprt(msgStr1);
}

//
// print flagged debug messages to single stream
//
template <class FLOATTYPE, size_t NDIM>
void PsDynObj<FLOATTYPE, NDIM>::dbprt(std::string msgStr1,
    std::string msgStr2, std::string msgStr3) {

  msgStr1.append(msgStr2);
  msgStr1.append(msgStr3);
  dbprt(msgStr1);
}

//
// print flagged debug messages to single stream
//
template <class FLOATTYPE, size_t NDIM>
void PsDynObj<FLOATTYPE, NDIM>::dbprt(std::string msgStr1,
    std::string msgStr2, std::string msgStr3, std::string msgStr4) {

  msgStr1.append(msgStr2);
  msgStr1.append(msgStr3);
  msgStr1.append(msgStr4);
  dbprt(msgStr1);
}

//
// print flagged debug messages to single stream
//
template <class FLOATTYPE, size_t NDIM>
void PsDynObj<FLOATTYPE, NDIM>::pprt(std::string msgStr) {

  size_t thisRank = domSingsPtr->getDomRank();
  if (!thisRank ) {
    std::cout << msgStr << std::endl;
  }
}

//
// print flagged debug messages to single stream
//
template <class FLOATTYPE, size_t NDIM>
void PsDynObj<FLOATTYPE, NDIM>::pprt(std::string msgStr1,
    std::string msgStr2) {

  msgStr1.append(msgStr2);
  pprt(msgStr1);
}

//
// print flagged debug messages to single stream
//
template <class FLOATTYPE, size_t NDIM>
void PsDynObj<FLOATTYPE, NDIM>::pprt(std::string msgStr1,
    std::string msgStr2, std::string msgStr3) {

  msgStr1.append(msgStr2);
  msgStr1.append(msgStr3);
  pprt(msgStr1);
}

//
// print flagged debug messages to single stream
//
template <class FLOATTYPE, size_t NDIM>
void PsDynObj<FLOATTYPE, NDIM>::pprt(std::string msgStr1,
    std::string msgStr2, std::string msgStr3, std::string msgStr4) {

  msgStr1.append(msgStr2);
  msgStr1.append(msgStr3);
  msgStr1.append(msgStr4);
  pprt(msgStr1);
}

//
// dump system
//
template <class FLOATTYPE, size_t NDIM>
void PsDynObj<FLOATTYPE, NDIM>::dump() {}

//
// equality operator
//
template <class FLOATTYPE, size_t NDIM>
void PsDynObj<FLOATTYPE, NDIM>::setDomainSingletons(
    PsDomainSingletons<FLOATTYPE, NDIM>& ds) {
  domSingsPtr = &ds;
}

template class PsDynObj<float, 1>;
template class PsDynObj<float, 2>;
template class PsDynObj<float, 3>;

template class PsDynObj<double, 1>;
template class PsDynObj<double, 2>;
template class PsDynObj<double, 3>;
