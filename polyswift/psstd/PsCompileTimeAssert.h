/**
 *
 * @file    PsCompileTimeAssert.h
 *
 * @brief   Defines struct PsCompileTimeAssert
 *
 * @version $Id: PsCompileTimeAssert.h 10760 2008-11-02 13:05:11Z cary $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation
 */

#ifndef PS_COMPILE_TIME_ASSERT_H
#define PS_COMPILE_TIME_ASSERT_H

/**
 * @struct PsCompileTimeAssert
 *
 * Including an object PsCompileTimeAssert<true> will have
 * no effect, but including PsCompileTimeAssert<false> will
 * result in an error at compile time.
 *
 * Example: if it's an error to instantiate an Object<N>
 * with template parameter N = 0, then
 *
 * \code
 * template <size_t N> struct Object {
 *    Object() { PsCompileTimeAssert< (N!=0) >(); }
 * };
 * \endcode
 *
 * will yield a compile time error if anyone tries to
 * insantiate an Object<0>().
 */

template <bool> struct PsCompileTimeAssert;

// write specialization for true, but not for false
template <> struct PsCompileTimeAssert<true> {};

#endif  // PS_COMPILE_TIME_ASSERT_H

