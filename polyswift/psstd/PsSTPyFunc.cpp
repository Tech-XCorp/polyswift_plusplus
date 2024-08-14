/**
 *
 * @file    PsSTPyFunc.cpp
 *
 * @brief   Define interface for the functor, PsSTPyFunc.
 *
 * @version $Id: PsSTPyFunc.cpp 6322 2006-11-16 00:30:04Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation
 */

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psstd includes
#include <PsSTPyFunc.h>

template <class FLOATTYPE, size_t NDIM>
PsSTPyFunc<FLOATTYPE, NDIM>::PsSTPyFunc() {

  // Allocate tmp space
  xvec = new FLOATTYPE[NDIM];

// Start by setting all to vanish
#ifdef HAVE_PYTHON
// The real work.  Find mainModule if not defined.
#else
#endif
}

template <class FLOATTYPE, size_t NDIM>
PsSTPyFunc<FLOATTYPE, NDIM>::~PsSTPyFunc() {
  paramValues.clear();
  paramKeys.clear();
  delete[] xvec;
}

template <class FLOATTYPE, size_t NDIM>
void PsSTPyFunc<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  //  std::cout << "PsSTPyFunc::setAttrib entered " << std::endl;

#ifdef HAVE_PYTHON

  // List of key strings for dictionary
  if (tas.hasStrVec("paramKeys")) {
    paramKeys = tas.getStrVec("paramKeys");
  }

  // List of key values for dictionary
  if (tas.hasPrmVec("paramValues")) {
    paramValues = tas.getPrmVec("paramValues");
  }

  paramDict = PyDict_New();
  PyObject* pstring;
  PyObject* pdouble;
  int errDict;

  // Convert to dictionary format
  if (paramKeys.size() != 0) {
    if (paramValues.size() == paramKeys.size() ) {

      for (size_t n=0; n<paramValues.size(); ++n) {
        const char* cstr = paramKeys[n].c_str();
        pdouble = PyFloat_FromDouble(paramValues[n]);
        errDict = PyDict_SetItemString(paramDict, cstr, pdouble);
        if (errDict != 0) {
          TxDebugExcept tde("PsSTPyFunc::setAttrib: error w/dictionary");
          throw tde;
        }
      }
    }

    else {
      TxDebugExcept tde("PsSTPyFunc::setAttrib: keys != values length");
      tde << "Number of paramKeys   = " << paramKeys.size()
          << "Number of paramValues = " << paramValues.size();
      throw tde;
    }
  }


  // Get a reference to the main module and the global dictionary
  if (!mainModule) {
    mainModule = PyImport_AddModule("__main__");
    if (!mainModule) {
      TxDebugExcept tde("PsSTPyFunc::setAttrib: mainModule not found");
      throw tde;
    }
    globalDict = PyModule_GetDict(mainModule);
  }

  // Get the function name
  if (tas.hasString("name")) {
    name = tas.getString("name");
  }
  else {
    TxDebugExcept tde("PsSTPyFunc::setAttrib: 'name' attribute not found");
    throw tde;
  }

  // Find the function
  char* tempStr = strdup(name.c_str());
  pyFunc = PyDict_GetItemString(globalDict, tempStr);
  free(tempStr);
  if (!pyFunc) {
    TxDebugExcept tde("PsSTPyFunc::setAttrib: Python function named '");
    tde << name << "' not found";
    throw tde;
  }


#else

  TxDebugExcept tde("PsSTPyFunc::setAttrib: Python not compiled in. "
      "Cannot use function named '");
  tde << name << "'.";
  throw tde;

#endif
}




template <class FLOATTYPE, size_t NDIM>
FLOATTYPE PsSTPyFunc<FLOATTYPE, NDIM>::operator()(
        PsTinyVector<size_t, NDIM>& rvec, FLOATTYPE t) const {

  for (size_t idim=0; idim<NDIM; ++idim) {
    xvec[idim] = (FLOATTYPE)rvec[idim];
  }

  return this->operator()(xvec, t);
}


template <class FLOATTYPE, size_t NDIM>
FLOATTYPE PsSTPyFunc<FLOATTYPE, NDIM>::operator()(
        PsTinyVector<int, NDIM>& rvec, FLOATTYPE t) const {

  for (size_t idim=0; idim<NDIM; ++idim) {
    xvec[idim] = (FLOATTYPE)rvec[idim];
  }

  FLOATTYPE val = this->operator()(xvec, t);
  return val;
}


template <class FLOATTYPE, size_t NDIM>
FLOATTYPE PsSTPyFunc<FLOATTYPE, NDIM>::operator()(const FLOATTYPE* x, FLOATTYPE t) const {

#ifdef HAVE_PYTHON

  // Set variables
  FLOATTYPE res = 0;
  double pos[3] = {0, 0, 0};
  for (size_t i=0; i<NDIM; ++i) {
    pos[i] = (double)x[i];
  }

  // size_t dictSize = (size_t)PyDict_Size(paramDict);
  // std::cout << "dictSize = " << dictSize << std::endl;

  // Format strings
  char argstr1[] = "dddd";  // (double, double , double, double)
  char argstr2[] = "ddddO"; // (double, double , double, double, PyObject)

  PyObject* pyResPtr = 0;
  if (paramKeys.size() == 0 ) {
    pyResPtr = PyObject_CallFunction(pyFunc, argstr1,
               pos[0], pos[1], pos[2], (double) t);
  }
  else {
    // std::string keyStr  = paramKeys[0];
    // const char* keyChar = keyStr.c_str();
    // PyObject* pyVal = PyDict_GetItemString(paramDict, keyChar);
    // std::cout << "pyVal = " << PyFloat_AS_DOUBLE(pyVal) << std::endl;
    pyResPtr = PyObject_CallFunction(pyFunc, argstr2,
        pos[0], pos[1], pos[2], (double) t, paramDict);
  }

  //  std::cout << "pyResPtr = " << pyResPtr << std::endl;
  if (!pyResPtr) {
    TxDebugExcept tde("PsSTPyFunc::operator(): Python function named '");
    tde << name << "' returned null pointer. Check python version?";
    throw tde;
  }

  // Extract result
  if (PyFloat_Check(pyResPtr)) {
    res = (FLOATTYPE) PyFloat_AsDouble(pyResPtr);
  }
  else {
    Py_DECREF(pyResPtr);
    TxDebugExcept tde("PsSTPyFunc::operator(): Python function named '");
    tde << name << "' failed.  Does function return a double?";
    // Need to catch any exception messages and put them in our exception
    throw tde;
  }

  // Done
  Py_DECREF(pyResPtr);
  return res;

#else

  TxDebugExcept tde("PsSTPyFunc::operator(): Python not compiled in. "
      "Cannot use function named '");
  tde << name << "'.";
  throw tde;
  return 0;

#endif
}



// Static data
template <class FLOATTYPE, size_t NDIM>
PyObject* PsSTPyFunc<FLOATTYPE, NDIM>::mainModule = 0;
template <class FLOATTYPE, size_t NDIM>
PyObject* PsSTPyFunc<FLOATTYPE, NDIM>::globalDict = 0;

// Instantiations

template class PsSTPyFunc<float, 1>;
template class PsSTPyFunc<float, 2>;
template class PsSTPyFunc<float, 3>;

template class PsSTPyFunc<double, 1>;
template class PsSTPyFunc<double, 2>;
template class PsSTPyFunc<double, 3>;

