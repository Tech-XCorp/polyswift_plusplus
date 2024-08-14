/**
 *
 * @file    PsPTXGen.h
 *
 * @brief   Defines interface for the PsPTXgen class.
 *
 * @version $Id: PsPTXGen.h 19392 2012-04-21 13:29:07Z cary $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation
 */

#ifndef PS_PTX_GEN_H
#define PS_PTX_GEN_H

#include "config.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <sstream>
#include <string>

class PsPTXGen {

  public:


   int currentReg;

   int currentReg32;

   int currentRegP;

   void resetReg();

   void resetReg32();

   void resetRegP();

   void setReg(int i);

   int allocateReg();

   int allocateReg32();

   int allocateRegP();

   int getNumReg();

   int getNumReg32();

   int getNumRegP();

   int precision;

   std::string precString;

   void setPrecision(int s) {
     //std::cout << "setting the precision  s = " << s << std::endl;
    if (s <= 4) {
       precision = 4;
       precString = ".f32";
    }
    else {
       precision = 8;
       precString = ".f64";
    }
    //    std::cout << " precision = " << precision << " precStr = " << precString << std::endl;
   };


   bool isDouble() { return (precision == 8); }

   std::string formOpCode(std::string str);

   std::string formIndir(std::string str, int offset);

   std::string formReg(std::string str, int reg);

   std::string regR(int reg);

   std::string regR32(int reg);

   std::string regP(int reg);

   void ptxNoEol(std::string str);

   void ptx(std::string str);

   void ptx(std::string opCode, std::string str1);

   void ptx(std::string opCode, std::string str1, std::string str2);

   void ptxNoPrec(std::string opCode, std::string str1, std::string str2);

   void ptx(std::string opCode, std::string str1, std::string str2,
            std::string str3);

   void ptxNoPrec(std::string opCode, std::string str1, std::string str2,
            std::string str3);

   void ptx(std::string opCode, std::string str1, std::string str2,
            std::string str3, std::string str4);

   void ptxNoPrec(std::string opCode, std::string str1, std::string str2,
            std::string str3, std::string str4);

   void ptx(std::string opCode, std::string str1, double value);

   void ptxNoPrec(std::string opCode, std::string str1, double value);

   void ptx(std::string opCode, double value);

   void ptx(std::string opCode, int value);

   void ptxReplace(std::string string1, const int n);

   void prologe(std::string kernelName, int writeComp,
                double dx[3], double offset[3], int xbump, int nzPitch);

   void epiloge(int reg, int writeComp, int offset, std::string);

   void setOutFile(std::ostringstream& o) { outfile = &o; }

   std::ostringstream* outfile;

};

#endif

