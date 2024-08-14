/**
 *
 * @file    PsSTPyFunc.h
 *
 * @brief   Define interface for the class PsSTPyFunc and the PyObject
 *          struct.
 *
 * @version $Id: PsSTPyFunc.h 6319 2006-11-14 22:39:46Z sizemore $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation
 */

#ifndef PS_ST_PY_FUNC_H
#define PS_ST_PY_FUNC_H

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// Python.h (via TxPython.h) defines these
#ifdef HAVE_PYTHON
#include <TxPython.h>
// These are erroneously defined by pyport.h
// See http://bugs.python.org/issue10910
#ifdef toupper
#undef toupper
#undef tolower
#undef isspace
#undef isupper
#undef islower
#undef isalpha
#undef isalnum
#endif
#endif

// psstd includes
#include <PsSTFunc.h>

// JRC: the forward reference requires inclusion here, as this is a
// C style typedef.  Now not fixed when python changes.
// Making choice that forward ref more important than the maintenance hit
// this will incur.  Unfortunately consumes a generic name '_object'.
// Would consume more names if included all of Python.h, though.
typedef struct _object PyObject;

/**
 * @class PsSTPyFunc
 *
 * @brief A functor that determines its value by calling out to python
 * script.  This allows the user to script very complext functors outside
 * of VORPAL, then use those within.
 */
template <class FLOATTYPE, size_t NDIM>
class PsSTPyFunc : public PsSTFunc<FLOATTYPE, NDIM> {

  public:

/**
 * Constructor - sets default values of parameters
 */
    PsSTPyFunc();

/**
 * Destructor
 */
    virtual ~PsSTPyFunc();

/**
 * Set up the functor from data in an attribute set.  Does lookup
 * of name in the python namespace.
 *
 * @param tas An attribute set containing the parameters of the
 *            plane wave.
 *
 * The attribute relevant to this class within tas is:
 *
 * - @c name (string): the name of the function.
 */
    virtual void setAttrib(const TxHierAttribSetIntDbl& tas);

/**
 * Return value of the functor at this point in space-time
 *
 * @param x vector of position
 * @param t the time
 * @return the value of the operator
 */
    virtual FLOATTYPE operator()(const FLOATTYPE* x, FLOATTYPE t) const;

/**
 * Return value of the functor at this point in space-time
 *
 * @param rvec PsTinyVector of position
 * @param t the time
 * @return the value of the operator
 */
    virtual FLOATTYPE operator()(PsTinyVector<int, NDIM>& rvec, FLOATTYPE t) const;

/**
 * Return value of the functor at this point in space-time
 *
 * @param rvec PsTinyVector of position
 * @param t the time
 * @return the value of the operator
 */
    virtual FLOATTYPE operator()(PsTinyVector<size_t, NDIM>& rvec, FLOATTYPE t) const;



  protected:

    /** reference to python main module */
    static PyObject* mainModule;

    /** reference to python global dictionary */
    static PyObject* globalDict;

    /** The vector of strings for dictionary keys */
    std::vector<std::string> paramKeys;

    /** The vector of values (double) for dictionary keys */
    std::vector<double> paramValues;

    /** Dictionary of attributes */
    PyObject* paramDict;

    /** The name of the Python function */
    std::string name;

    /** reference to Python function */
    PyObject* pyFunc;


  private:

    /* Temp space */
    FLOATTYPE* xvec;

    /** Private to prevent use */
    PsSTPyFunc(const PsSTPyFunc<FLOATTYPE, NDIM>&);

    /** Private to prevent use */
    PsSTPyFunc<FLOATTYPE, NDIM>& operator=(const PsSTPyFunc<FLOATTYPE, NDIM>&);
};

#endif // PS_ST_PY_FUNC_H

