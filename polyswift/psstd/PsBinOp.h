/**
 *
 * @file    PsBinOp.h
 *
 * @brief   Defines interfaces for binary operations
 *
 * @version $Id: PsBinOp.h 19382 2012-04-20 00:24:11Z wernerg $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation
 */

#ifndef PS_BIN_OP_H
#define PS_BIN_OP_H

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <txc_std.h>
#include <txc_math.h>
#include <iostream>
#include <string>
#include <list>
#include <map>
#include <cctype>

// txbase includes
#include <TxDebugExcept.h>

// vpstd includes
#include <PsRandom.h>
#include <PsPTXGen.h>


/**
 * Prototype for binary operator. Will  be used in the symbol table. Avoids
 * the use of a void* ptr.
 */
template <class FLOATTYPE>
class PsBinOp {

  public:

/**
 * Constructor
 */
   PsBinOp() {
     isConst = true;
     hasShortCut = false;
     shortCutValue = 0;
     shortCutResult = 0;
   }

/**
 * Destrutor
 */
    virtual ~PsBinOp() {}

/**
 * Evaluate the binary operator
 *
 * @param x
 * @param y
 */
    virtual FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) = 0;


/**
 ** Evaluate the binary operator
 **
 ** @param x
 ** @param y
 **/
    virtual int generatePTX(int x, int y) = 0;

    virtual void setPTXGen(PsPTXGen* p ) { ptxGen = p; };

/**
 * Tells if the result of the operator can be constant for
 * constant arguments
 */
    bool isConst;

/**
 * Tells if operator can avoid evaluation of second argument
 * if first argument is known
 */
    bool hasShortCut;

/**
 * The value that the first arugment has to have to take the shortcut
 */
    FLOATTYPE shortCutValue;

/**
 * The result of the shortcut
 */
    FLOATTYPE shortCutResult;

/**
 * The code generator
 */
    PsPTXGen* ptxGen;
};


/**
 * binary operator class, implementing lessThan
 */
template <class FLOATTYPE>
class PsBinOpLessThan : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) { return x < y; }
    int generatePTX(int x, int y) {
      int oneReg = this->ptxGen->allocateReg();
      int zeroReg = this->ptxGen->allocateReg();
      int pReg = this->ptxGen->allocateRegP();
      this->ptxGen->ptx("mov", this->ptxGen->regR(oneReg), (double) 1.0);
      this->ptxGen->ptx("mov", this->ptxGen->regR(zeroReg), (double) 0.0);
      this->ptxGen->ptx("setp.lt", this->ptxGen->regP(pReg),
                        this->ptxGen->regR(x),
                        this->ptxGen->regR(y));
      int res = this->ptxGen->allocateReg();
      this->ptxGen->ptx("selp", this->ptxGen->regR(res),
             this->ptxGen->regR(oneReg), this->ptxGen->regR(zeroReg),
             this->ptxGen->regP(pReg));
      return res;
   }
};

/*
template <> int PsBinOpLessThan<float>::generatePTX(int x, int y) {
      int oneReg = this->ptxGen->allocateReg();
      int zeroReg = this->ptxGen->allocateReg();
      this->ptxGen->ptx("mov.f32", this->ptxGen->regR(oneReg), (double) 1.0);
      this->ptxGen->ptx("mov.f32", this->ptxGen->regR(zeroReg), (double) 0.0);
      this->ptxGen->ptx("setp.lt.f32", this->ptxGen->regP(2),
                        this->ptxGen->regR(x),
                        this->ptxGen->regR(y));
      int res = this->ptxGen->allocateReg();
      this->ptxGen->ptx("selp.f32", this->ptxGen->regR(res),
             this->ptxGen->regR(oneReg), this->ptxGen->regR(zeroReg), this->ptxGen->regP(2));
      return res;
};

template <> int PsBinOpLessThan<double>::generatePTX(int x, int y) {
      int oneReg = this->ptxGen->allocateReg();
      int zeroReg = this->ptxGen->allocateReg();
      this->ptxGen->ptx("mov.f64", this->ptxGen->regR(oneReg), (double) 1.0);
      this->ptxGen->ptx("mov.f64", this->ptxGen->regR(zeroReg), (double) 0.0);
      this->ptxGen->ptx("setp.lt.f64", this->ptxGen->regP(2),
                        this->ptxGen->regR(x),
                        this->ptxGen->regR(y));
      int res = this->ptxGen->allocateReg();
      this->ptxGen->ptx("selp.f64", this->ptxGen->regR(res),
             this->ptxGen->regR(oneReg), this->ptxGen->regR(zeroReg), this->ptxGen->regP(2));
      return res;
};
*/

/**
 * binary operator class, implementing moreThan
 */
template <class FLOATTYPE>
class PsBinOpMoreThan : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) { return x > y; }
    int generatePTX(int x, int y) {
      int oneReg = this-> ptxGen->allocateReg();
      int zeroReg = this-> ptxGen->allocateReg();
      int pReg = this->ptxGen->allocateRegP();
      this->ptxGen->ptx("mov", this->ptxGen->regR(oneReg), (double) 1.0);
      this->ptxGen->ptx("mov", this->ptxGen->regR(zeroReg), (double) 0.0);
      this->ptxGen->ptx("setp.gt", this->ptxGen->regP(pReg),
                        this->ptxGen->regR(x),
                        this->ptxGen->regR(y));
      int res = this->ptxGen->allocateReg();
      this->ptxGen->ptx("selp", this->ptxGen->regR(res),
             this->ptxGen->regR(oneReg), this->ptxGen->regR(zeroReg),
             this->ptxGen->regP(pReg));
      return res;
    }
};

/**
 * binary operator class, implementing lessEqual
 */
template <class FLOATTYPE>
class PsBinOpLessEqual : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) { return x <= y; }
    int generatePTX(int x, int y) {
      int oneReg = this-> ptxGen->allocateReg();
      int zeroReg = this-> ptxGen->allocateReg();
      int pReg = this->ptxGen->allocateRegP();
      this->ptxGen->ptx("mov", this->ptxGen->regR(oneReg), (double) 1.0);
      this->ptxGen->ptx("mov", this->ptxGen->regR(zeroReg), (double) 0.0);
      this->ptxGen->ptx("setp.lt", this->ptxGen->regP(pReg),
                        this->ptxGen->regR(x),
                        this->ptxGen->regR(y));
      int res = this->ptxGen->allocateReg();
      this->ptxGen->ptx("selp", this->ptxGen->regR(res),
             this->ptxGen->regR(oneReg), this->ptxGen->regR(zeroReg),
             this->ptxGen->regP(pReg));
      return res;
    }
};

/**
 * binary operator class, implementing MoreEqual
 */
template <class FLOATTYPE>
class PsBinOpMoreEqual : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) { return x >= y; }
    int generatePTX(int x, int y) {
      int oneReg = this-> ptxGen->allocateReg();
      int zeroReg = this-> ptxGen->allocateReg();
      int pReg = this->ptxGen->allocateRegP();
      this->ptxGen->ptx("mov", this->ptxGen->regR(oneReg), (double) 1.0);
      this->ptxGen->ptx("mov", this->ptxGen->regR(zeroReg), (double) 0.0);
      this->ptxGen->ptx("setp.ge", this->ptxGen->regP(pReg),
                        this->ptxGen->regR(x),
                        this->ptxGen->regR(y));
      int res = this->ptxGen->allocateReg();
      this->ptxGen->ptx("selp", this->ptxGen->regR(res),
             this->ptxGen->regR(oneReg), this->ptxGen->regR(zeroReg),
             this->ptxGen->regP(pReg));
      return res;
    }
};

/**
 * binary operator class, implementing equal
 */
template <class FLOATTYPE>
class PsBinOpEqual : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) { return x == y; }
    int generatePTX(int x, int y) {
      int oneReg = this-> ptxGen->allocateReg();
      int zeroReg = this-> ptxGen->allocateReg();
      int pReg = this->ptxGen->allocateRegP();
      this->ptxGen->ptx("mov", this->ptxGen->regR(oneReg), (double) 1.0);
      this->ptxGen->ptx("mov", this->ptxGen->regR(zeroReg), (double) 0.0);
      this->ptxGen->ptx("setp.eq", this->ptxGen->regP(pReg),
                        this->ptxGen->regR(x),
                        this->ptxGen->regR(y));
      int res = this->ptxGen->allocateReg();
      this->ptxGen->ptx("selp", this->ptxGen->regR(res),
             this->ptxGen->regR(oneReg), this->ptxGen->regR(zeroReg),
             this->ptxGen->regP(pReg));
      return res;
    }
};

/**
 * binary operator class, implementing not equal
 */
template <class FLOATTYPE>
class PsBinOpNotEqual : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) { return x != y; }
    int generatePTX(int x, int y) {
      int oneReg = this-> ptxGen->allocateReg();
      int zeroReg = this-> ptxGen->allocateReg();
      int pReg = this->ptxGen->allocateRegP();
      this->ptxGen->ptx("mov", this->ptxGen->regR(oneReg), (double) 1.0);
      this->ptxGen->ptx("mov", this->ptxGen->regR(zeroReg), (double) 0.0);
      this->ptxGen->ptx("setp.ne", this->ptxGen->regP(pReg),
                        this->ptxGen->regR(x),
                        this->ptxGen->regR(y));
      int res = this->ptxGen->allocateReg();
      this->ptxGen->ptx("selp", this->ptxGen->regR(res),
             this->ptxGen->regR(oneReg), this->ptxGen->regR(zeroReg),
             this->ptxGen->regP(pReg));
      return res;
    }
};


/**
 * binary operator class, implementing addition
 */
template <class FLOATTYPE>
class PsBinOpPlus : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) { return x + y; }
    int generatePTX(int x, int y) {
         int res = this->ptxGen->allocateReg();
         this->ptxGen->ptx("add", this->ptxGen->regR(res),
                           this->ptxGen->regR(x), this->ptxGen->regR(y));
         return res;
    }
};

/**
 * binary operator class, implementing subtraction
 */
template <class FLOATTYPE>
class PsBinOpMinus : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) { return x - y; }
    int generatePTX(int x, int y) {
         int res = this->ptxGen->allocateReg();
         this->ptxGen->ptx("sub", this->ptxGen->regR(res),
                           this->ptxGen->regR(x), this->ptxGen->regR(y));
         return res;
    }
};

/**
 * binary operator class, implementing multiplication
 */
template <class FLOATTYPE>
class PsBinOpTimes : public PsBinOp<FLOATTYPE> {
  public:
    PsBinOpTimes() : PsBinOp<FLOATTYPE>() {
      this->hasShortCut = true;
      this->shortCutValue = 0.;
      this->shortCutResult = 0.;
    }

    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) { return x * y; }
    int generatePTX(int x, int y) {
         int res = this->ptxGen->allocateReg();
         this->ptxGen->ptx("mul", this->ptxGen->regR(res),
                           this->ptxGen->regR(x), this->ptxGen->regR(y));
         return res;
    }

};

/**
 * binary operator class, implementing division
 */
template <class FLOATTYPE>
class PsBinOpDiv : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) { return x / y; }
    int generatePTX(int x, int y) {
      int res = this->ptxGen->allocateReg();
      if (this->ptxGen->isDouble()) {
         this->ptxGen->ptx("div.rn", this->ptxGen->regR(res),
                           this->ptxGen->regR(x), this->ptxGen->regR(y));
      }
      else {
         this->ptxGen->ptx("div.approx", this->ptxGen->regR(res),
                           this->ptxGen->regR(x), this->ptxGen->regR(y));
      }
      return res;
    }
};

/**
 * binary operator class, implementing modulo 1 operation
 */
template <class FLOATTYPE>
class PsBinOpMod : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) {
      return fmod(x, y);
    }
    int generatePTX(int x, int y) { this->ptxGen->ptx("Invalid Operation"); return -1;}
};

/**
 * binary operator class, implementing modulo 1 operation, then
 * putting into the (-1/2, 1/2] interval
 */
template <class FLOATTYPE>
class PsBinOpModMod : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) {
      FLOATTYPE res = fmod(x, (FLOATTYPE)1.);
      if (res > 0.5) res -= 1.;
      else if (res <= -0.5) res += 1.;
      return res;
    }
    int generatePTX(int x, int y) { this->ptxGen->ptx("Invalid Operation"); return -1;}
};

/**
 * binary operator class, implementing exponentiation
 */
template <class FLOATTYPE>
class PsBinOpPow : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) {
      return pow(x, y);
    }

    int generatePTX(int x, int y) {
         int tmp2 = this->ptxGen->allocateReg32();
         int tmp3 = this->ptxGen->allocateReg32();
         int tmp4 = this->ptxGen->allocateReg32();
         int res = this->ptxGen->allocateReg();

         if (this->ptxGen->isDouble()) {
            int tmp1 = this->ptxGen->allocateReg32();
            int tmp5 = this->ptxGen->allocateReg32();
            int tmp6 = this->ptxGen->allocateReg32();

            this->ptxGen->ptxNoPrec("cvt.rn.f32.f64",
                         this->ptxGen->regR32(tmp1), this->ptxGen->regR(x));

            this->ptxGen->ptxNoPrec("cvt.rn.f32.f64",
                         this->ptxGen->regR32(tmp2), this->ptxGen->regR(y));

            this->ptxGen->ptxNoPrec("abs.f32", this->ptxGen->regR32(tmp6), this->ptxGen->regR32(tmp1));

            this->ptxGen->ptxNoPrec("lg2.approx.f32",
                         this->ptxGen->regR32(tmp3), this->ptxGen->regR32(tmp6));

            this->ptxGen->ptxNoPrec("mul.f32",
                         this->ptxGen->regR32(tmp4), this->ptxGen->regR32(tmp3),
                         this->ptxGen->regR32(tmp2));

            this->ptxGen->ptxNoPrec("ex2.approx.f32",
                         this->ptxGen->regR32(tmp5),
                         this->ptxGen->regR32(tmp4));

            this->ptxGen->ptxNoPrec("cvt.f64.f32",
                         this->ptxGen->regR(res), this->ptxGen->regR32(tmp5));
         }
         else {
            this->ptxGen->ptxNoPrec("abs.f32", this->ptxGen->regR32(tmp2), this->ptxGen->regR(x));

            this->ptxGen->ptxNoPrec("lg2.approx.f32",
                         this->ptxGen->regR32(tmp3), this->ptxGen->regR32(tmp2));

            this->ptxGen->ptxNoPrec("mul.f32",
                         this->ptxGen->regR32(tmp4), this->ptxGen->regR32(tmp3),
                         this->ptxGen->regR(y));

            this->ptxGen->ptxNoPrec("ex2.approx.f32",
                         this->ptxGen->regR(res),
                         this->ptxGen->regR32(tmp4));
         }
         return res;
    }
};

/**
 * binary operator class, implementing a sign change
 */
template <class FLOATTYPE>
class PsBinOpInv : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) { return -x; }
    int generatePTX(int x, int y) {
         int res = this->ptxGen->allocateReg();
         this->ptxGen->ptx("neg", this->ptxGen->regR(res), this->ptxGen->regR(x));
         return res;
    }
};

/**
 * binary operator class, implementing sin
 */
template <class FLOATTYPE>
class PsBinOpSin : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) { return sin(x); }
    int generatePTX(int x, int y) {
         int res = this->ptxGen->allocateReg();
         if (this->ptxGen->isDouble()) {
            int tmp1 = this->ptxGen->allocateReg32();
            int tmp2 = this->ptxGen->allocateReg32();
            this->ptxGen->ptxNoPrec("cvt.rn.f32.f64",
                       this->ptxGen->regR32(tmp1), this->ptxGen->regR(x));
            this->ptxGen->ptxNoPrec("sin.approx.f32",
                       this->ptxGen->regR32(tmp2), this->ptxGen->regR32(tmp1));
            this->ptxGen->ptxNoPrec("cvt.f64.f32",
                       this->ptxGen->regR(res), this->ptxGen->regR32(tmp2));
         }
         else {
            this->ptxGen->ptxNoPrec("sin.approx.f32",
                       this->ptxGen->regR(res), this->ptxGen->regR(x));
         }
         return res;
    }
};

/**
 * binary operator class, implementing cos
 */
template <class FLOATTYPE>
class PsBinOpCos : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) { return cos(x); }
    int generatePTX(int x, int y) {
         int res = this->ptxGen->allocateReg();
         if (this->ptxGen->isDouble()) {
            int tmp1 = this->ptxGen->allocateReg32();
            int tmp2 = this->ptxGen->allocateReg32();
            this->ptxGen->ptxNoPrec("cvt.rn.f32.f64",
                       this->ptxGen->regR32(tmp1), this->ptxGen->regR(x));
            this->ptxGen->ptxNoPrec("cos.approx.f32",
                       this->ptxGen->regR32(tmp2), this->ptxGen->regR32(tmp1));
            this->ptxGen->ptxNoPrec("cvt.f64.f32",
                       this->ptxGen->regR(res), this->ptxGen->regR32(tmp2));
         }
         else {
            this->ptxGen->ptxNoPrec("cos.approx.f32",
                       this->ptxGen->regR(res), this->ptxGen->regR(x));
         }
         return res;
    }
};

/**
 * binary operator class, implementing tan
 */
template <class FLOATTYPE>
class PsBinOpTan : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) { return tan(x); }
    int generatePTX(int x, int y) {
         int tmp2 = this->ptxGen->allocateReg32();
         int tmp3 = this->ptxGen->allocateReg32();
         int res = this->ptxGen->allocateReg();

         if (this->ptxGen->isDouble()) {
            int tmp1 = this->ptxGen->allocateReg32();
            int tmp4 = this->ptxGen->allocateReg32();
            this->ptxGen->ptxNoPrec("cvt.rn.f32.f64",
                         this->ptxGen->regR32(tmp1), this->ptxGen->regR(x));
            this->ptxGen->ptxNoPrec("cos.approx.f32",
                         this->ptxGen->regR32(tmp2), this->ptxGen->regR32(tmp1));
            this->ptxGen->ptxNoPrec("sin.approx.f32",
                         this->ptxGen->regR32(tmp3), this->ptxGen->regR32(tmp1));
            this->ptxGen->ptxNoPrec("div.approx.f32",
                         this->ptxGen->regR32(tmp4),
                         this->ptxGen->regR32(tmp3), this->ptxGen->regR32(tmp2));
            this->ptxGen->ptxNoPrec("cvt.f64.f32",
                         this->ptxGen->regR(res), this->ptxGen->regR32(tmp4));
         }
         else {
            this->ptxGen->ptxNoPrec("cos.approx.f32",
                         this->ptxGen->regR32(tmp2), this->ptxGen->regR(x));
            this->ptxGen->ptxNoPrec("sin.approx.f32",
                         this->ptxGen->regR32(tmp3), this->ptxGen->regR(x));
            this->ptxGen->ptxNoPrec("div.approx.f32",
                         this->ptxGen->regR(res),
                         this->ptxGen->regR32(tmp3), this->ptxGen->regR32(tmp2));


         }
         return res;
   }

};

/**
 * binary operator class, implementing exp
 */
template <class FLOATTYPE>
class PsBinOpExp : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) { return exp(x); }
    int generatePTX(int x, int y) {
         int tmp2 = this->ptxGen->allocateReg32();
         int tmp3 = this->ptxGen->allocateReg32();
         int res = this->ptxGen->allocateReg();

         if (this->ptxGen->isDouble()) {
           int tmp1 = this->ptxGen->allocateReg32();
           int tmp4 = this->ptxGen->allocateReg32();
           this->ptxGen->ptxNoPrec("cvt.rn.f32.f64",
                         this->ptxGen->regR32(tmp1), this->ptxGen->regR(x));
           this->ptxGen->ptxNoPrec("mov.f32",
                         this->ptxGen->regR32(tmp2), "0f3fb8aa3b");
           this->ptxGen->ptxNoPrec("mul.f32",
                         this->ptxGen->regR32(tmp3),
                         this->ptxGen->regR32(tmp2), this->ptxGen->regR32(tmp1));
           this->ptxGen->ptxNoPrec("ex2.approx.f32",
                         this->ptxGen->regR32(tmp4), this->ptxGen->regR32(tmp3));

           this->ptxGen->ptxNoPrec("cvt.f64.f32",
                         this->ptxGen->regR(res), this->ptxGen->regR32(tmp4));
         }
         else {
           this->ptxGen->ptxNoPrec("mov.f32",
                         this->ptxGen->regR32(tmp2), "0f3fb8aa3b");
           this->ptxGen->ptxNoPrec("mul.f32",
                         this->ptxGen->regR32(tmp3),
                         this->ptxGen->regR32(tmp2), this->ptxGen->regR(x));
           this->ptxGen->ptxNoPrec("ex2.approx.f32",
                         this->ptxGen->regR(res), this->ptxGen->regR32(tmp3));

         }
         return res;
    }
};

/**
 * binary operator class, implementing asin
 */
template <class FLOATTYPE>
class PsBinOpAsin : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) { return asin(x); }
    int generatePTX(int x, int y) { this->ptxGen->ptx("Invalid Operation"); return -1;}
};

/**
 * binary operator class, implementing acos
 */
template <class FLOATTYPE>
class PsBinOpAcos : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) { return acos(x); }
    int generatePTX(int x, int y) { this->ptxGen->ptx("Invalid Operation"); return -1;}
};

/**
 * binary operator class, implementing sinh
 */
template <class FLOATTYPE>
class PsBinOpSinH : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) { return sinh(x); }
    int generatePTX(int x, int y) { this->ptxGen->ptx("Invalid Operation"); return -1;}
};

/**
 * binary operator class, implementing cosh
 */
template <class FLOATTYPE>
class PsBinOpCosH : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) { return cosh(x); }
    int generatePTX(int x, int y) { this->ptxGen->ptx("Invalid Operation"); return -1;}
};

/**
 * binary operator class, implementing tanh
 */
template <class FLOATTYPE>
class PsBinOpTanH : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) { return tanh(x); }
    int generatePTX(int x, int y) { this->ptxGen->ptx("Invalid Operation"); return -1;}
};

/**
 * binary operator class, implementing alog
 */
template <class FLOATTYPE>
class PsBinOpLn : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) { return log(x); }
    int generatePTX(int x, int y) {
         int tmp2 = this->ptxGen->allocateReg32();
         int tmp3 = this->ptxGen->allocateReg32();
         int res = this->ptxGen->allocateReg();

         if (this->ptxGen->isDouble()) {
           int tmp1 = this->ptxGen->allocateReg32();
           int tmp4 = this->ptxGen->allocateReg32();

           this->ptxGen->ptxNoPrec("cvt.rn.f32.f64",
                         this->ptxGen->regR32(tmp1), this->ptxGen->regR(x));
           this->ptxGen->ptxNoPrec("lg2.approx.f32",
                         this->ptxGen->regR32(tmp2), this->ptxGen->regR32(tmp1));
           this->ptxGen->ptxNoPrec("mov.f32",
                         this->ptxGen->regR32(tmp3), "0f3f317218");
           this->ptxGen->ptxNoPrec("mul.f32",
                         this->ptxGen->regR32(tmp4),
                         this->ptxGen->regR32(tmp3), this->ptxGen->regR32(tmp2));
           this->ptxGen->ptxNoPrec("cvt.f64.f32",
                         this->ptxGen->regR(res), this->ptxGen->regR32(tmp4));
         }
         else {

           this->ptxGen->ptxNoPrec("lg2.approx.f32",
                         this->ptxGen->regR32(tmp2), this->ptxGen->regR(x));
           this->ptxGen->ptxNoPrec("mov.f32",
                         this->ptxGen->regR32(tmp3), "0f3f317218");
           this->ptxGen->ptxNoPrec("mul.f32",
                         this->ptxGen->regR(res),
                         this->ptxGen->regR32(tmp3), this->ptxGen->regR32(tmp2));
         }

         return res;
    }
};

/**
 * binary operator class, implementing atan
 */
template <class FLOATTYPE>
class PsBinOpAtan : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) { return atan(x); }
    int generatePTX(int x, int y) { this->ptxGen->ptx("Invalid Operation"); return -1;}
};

/**
 * binary operator class, implementing atan2(y, x), returning angle in
 * correct quadrant
 */
template <class FLOATTYPE>
class PsBinOpAtan2 : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE y, FLOATTYPE x) { return atan2(y, x); }
    int generatePTX(int x, int y) { this->ptxGen->ptx("Invalid Operation"); return -1;}
};

/**
 *  binary operator class, implementing heaviside
 */
template <class FLOATTYPE>
class PsBinOpH : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) {
      if (x > 0.) return 1.;
      if (x < 0.) return 0.;
      return 0.5;
    }
    int generatePTX(int x, int y) {
       int tmp2  = this->ptxGen->allocateReg();
       int tmp3  = this->ptxGen->allocateReg();
       int tmp4  = this->ptxGen->allocateReg();
       int tmpP1 = this->ptxGen->allocateRegP();
       int tmpP2 = this->ptxGen->allocateRegP();

       this->ptxGen->ptx("mov", this->ptxGen->regR(tmp2), double(1.0));
       this->ptxGen->ptx("mov", this->ptxGen->regR(tmp3), double(0.0));
       this->ptxGen->ptx("mov", this->ptxGen->regR(tmp4), double(0.5));
       this->ptxGen->ptx("setp.lt",
                       this->ptxGen->regP(tmpP1),
                       this->ptxGen->regR(x), this->ptxGen->regR(tmp3));
       int tmp6  = this->ptxGen->allocateReg();
       this->ptxGen->ptx("selp",
                       this->ptxGen->regR(tmp6), this->ptxGen->regR(tmp3),
                       this->ptxGen->regR(tmp4), this->ptxGen->regP(tmpP1));
       this->ptxGen->ptx("setp.gt", this->ptxGen->regP(tmpP2),
                       this->ptxGen->regR(x), this->ptxGen->regR(tmp3));
       int res  = this->ptxGen->allocateReg();
       this->ptxGen->ptx("selp",
                       this->ptxGen->regR(res), this->ptxGen->regR(tmp2),
                       this->ptxGen->regR(tmp6), this->ptxGen->regP(tmpP2));
       return res;
    }
};

/**
 *  binary operator class, implementing bessel function or order zero
 */
template <class FLOATTYPE>
class PsBinOpJ0 : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) {
#ifdef _WIN32
      return _j0(x);
#else
      return j0(x);
#endif
    }
    int generatePTX(int x, int y) { this->ptxGen->ptx("Invalid Operation"); return -1;}
};

/**
 *  binary operator class, implementing bessel function or order one
 */
template <class FLOATTYPE>
class PsBinOpJ1 : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) {
#ifdef _WIN32
      return _j1(x);
#else
      return j1(x);
#endif
    }
    int generatePTX(int x, int y) { this->ptxGen->ptx("Invalid Operation"); return -1;}
};

/**
 *  binary operator class, implementing bessel function or order two
 */
template <class FLOATTYPE>
class PsBinOpJ2 : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) {
#ifdef _WIN32
      return _jn(2, x);
#else
      return jn(2, x);
#endif
    }
    int generatePTX(int x, int y) { this->ptxGen->ptx("Invalid Operation"); return -1;}
};

/**
 *  binary operator class, implementing bessel function or order three
 */
template <class FLOATTYPE>
class PsBinOpJ3 : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) {
#ifdef _WIN32
      return _jn(3, x);
#else
      return jn(3, x);
#endif
    }
    int generatePTX(int x, int y) { this->ptxGen->ptx("Invalid Operation"); return -1;}
};

/**
 *  binary operator class, implementing absolute value
 */
template <class FLOATTYPE>
class PsBinOpAbs : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) {
      return fabs(x);
    }
    int generatePTX(int x, int y) {
       int res  = this->ptxGen->allocateReg();
       this->ptxGen->ptx("abs", this->ptxGen->regR(res), this->ptxGen->regR(x));
       return res;
    }
};

/**
 *  binary operator class, implementing sqrare root
 */
template <class FLOATTYPE>
class PsBinOpSqrt : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) {
      return sqrt(x);
    }

    int generatePTX(int x, int y) {
       int res = this->ptxGen->allocateReg();
       if (this->ptxGen->isDouble()) {
         int tmp1 = this->ptxGen->allocateReg32();
         int tmp2 = this->ptxGen->allocateReg32();
         this->ptxGen->ptxNoPrec("cvt.rn.f32.f64",
                       this->ptxGen->regR32(tmp1), this->ptxGen->regR(x));
         this->ptxGen->ptxNoPrec("sqrt.approx.f32",
                       this->ptxGen->regR32(tmp2), this->ptxGen->regR32(tmp1));
         this->ptxGen->ptxNoPrec("cvt.f64.f32",
                       this->ptxGen->regR(res), this->ptxGen->regR32(tmp2));
       }
       else {
         this->ptxGen->ptxNoPrec("sqrt.approx.f32",
                       this->ptxGen->regR(res), this->ptxGen->regR32(x));

       }
       return res;
   }
};

/**
 *  binary operator class, implementing a uniform random number
 */
template <class FLOATTYPE>
class PsBinOpRand : public PsBinOp<FLOATTYPE> {
 public:
    PsBinOpRand() : PsBinOp<FLOATTYPE>() {
      this->isConst = false;
    }

    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) {
      return psRandom<FLOATTYPE>();
    }
    int generatePTX(int x, int y) { this->ptxGen->ptx("Invalid Operation"); return -1;}
};

/**
 *  binary operator class, implementing a gaussian random number
 */
template <class FLOATTYPE>
class PsBinOpGauss : public PsBinOp<FLOATTYPE> {
 public:
    PsBinOpGauss() : PsBinOp<FLOATTYPE>() {
      this->isConst = false;
    }

    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) {
      FLOATTYPE R = psRandom<FLOATTYPE>();
      FLOATTYPE r = x * sqrt(-2. * log(1.0 - R));
      FLOATTYPE theta = 2. * M_PI * psRandom<FLOATTYPE>();
      return r * cos(theta) + y;
    }
    int generatePTX(int x, int y) { this->ptxGen->ptx("Invalid Operation"); return -1;}
};

#ifndef _WIN32
/**
 *  binary operator class, implementing the error function =
 * (2/sqrt(pi))*Integral(0, x, exp(-t^2))
 */
template <class FLOATTYPE>
class PsBinOpErf : public PsBinOp<FLOATTYPE> {
  public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) {
      return erf(x);
    }
    int generatePTX(int x, int y) { this->ptxGen->ptx("Invalid Operation"); return -1;}
};
#endif

/**
 *  binary operator class, implementing a simple print method
 */
template <class FLOATTYPE>
class PsBinOpPrint : public PsBinOp<FLOATTYPE> {
 public:
    PsBinOpPrint() : PsBinOp<FLOATTYPE>() {
      this->isConst = false;
    }

    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) {
      std::cout << "x = " << x << std::endl;
      return x;
    }
    int generatePTX(int x, int y) { this->ptxGen->ptx("Invalid Operation"); return -1;}
};

/**
 *  binary operator class, implementing a ceil round
 */
template <class FLOATTYPE>
class PsBinOpCeil : public PsBinOp<FLOATTYPE> {
 public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) { return ceil( x); }
    int generatePTX(int x, int y) {
         int res = this->ptxGen->allocateReg();
         if (this->ptxGen->isDouble()) {
           int tmp1 = this->ptxGen->allocateReg32();
           int tmp2 = this->ptxGen->allocateReg32();
           this->ptxGen->ptxNoPrec("cvt.rn.f32.f64",
                       this->ptxGen->regR32(tmp1), this->ptxGen->regR(x));
           this->ptxGen->ptxNoPrec("cvt.rpi.f32.f32",
                       this->ptxGen->regR32(tmp2), this->ptxGen->regR32(tmp1));
           this->ptxGen->ptxNoPrec("cvt.f64.f32",
                       this->ptxGen->regR(res), this->ptxGen->regR32(tmp2));
         }
         else {
           this->ptxGen->ptxNoPrec("cvt.rpi.f32.f32",
                       this->ptxGen->regR(res), this->ptxGen->regR(x));
         }
         return res;
    }
};

/**
 *  binary operator class, implementing a floor round
 */
template <class FLOATTYPE>
class PsBinOpFloor : public PsBinOp<FLOATTYPE> {
 public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) { return floor( x); }
    int generatePTX(int x, int y) {
         int res = this->ptxGen->allocateReg();
         if (this->ptxGen->isDouble()) {
           int tmp1 = this->ptxGen->allocateReg32();
           int tmp2 = this->ptxGen->allocateReg32();
           this->ptxGen->ptxNoPrec("cvt.rn.f32.f64",
                       this->ptxGen->regR32(tmp1), this->ptxGen->regR(x));
           this->ptxGen->ptxNoPrec("cvt.rmi.f32.f32",
                       this->ptxGen->regR32(tmp2), this->ptxGen->regR32(tmp1));
           this->ptxGen->ptxNoPrec("cvt.f64.f32",
                       this->ptxGen->regR(res), this->ptxGen->regR32(tmp2));
         }
         else {
           this->ptxGen->ptxNoPrec("cvt.rmi.f32.f32",
                       this->ptxGen->regR(res), this->ptxGen->regR(x));
         }
         return res;
    }
};

/**
 *  binary operator class, implementing a minimum
 */
template <class FLOATTYPE>
class PsBinOpMin : public PsBinOp<FLOATTYPE> {
 public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) { return x<y ? x : y; }
    int generatePTX(int x, int y) {
         int res = this->ptxGen->allocateReg();
         this->ptxGen->ptx("min", this->ptxGen->regR(res),
                              this->ptxGen->regR(x), this->ptxGen->regR(y));
         return res;
    }
};

/**
 *  binary operator class, implementing a maximum
 */
template <class FLOATTYPE>
class PsBinOpMax : public PsBinOp<FLOATTYPE> {
 public:
    FLOATTYPE evaluate(FLOATTYPE x, FLOATTYPE y) { return x>y ? x : y; }
    int generatePTX(int x, int y) {
         int res = this->ptxGen->allocateReg();
         this->ptxGen->ptx("max", this->ptxGen->regR(res),
                              this->ptxGen->regR(x), this->ptxGen->regR(y));
         return res;
    }
};

#endif

