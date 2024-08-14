/**
 *
 * @file    PsPTXGen.h
 *
 * @brief   Defines interface for the PsPTXgen class.
 *
 * @version $Id: PsPTXGen.cpp 19392 2012-04-21 13:29:07Z cary $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation
 */

#include <iostream>
#include <iomanip>
#include <sstream>

#include "PsPTXGen.h"

#ifdef HAVE_GPU
// cuda include
#include <cuda.h>
#include <driver_types.h>
#include <cuda_runtime.h>
#endif


void PsPTXGen::resetReg() {
  currentReg = 0;
}

void PsPTXGen::resetReg32() {
  currentReg32 = 0;
}

void PsPTXGen::resetRegP() {
  currentRegP = 0;
}

void PsPTXGen::setReg(int i) {
  currentReg = i;
}

int PsPTXGen::allocateReg() {
  return ++currentReg;
}

int PsPTXGen::allocateReg32() {
  return ++currentReg32;
}

int PsPTXGen::allocateRegP() {
  return ++currentRegP;
}

int PsPTXGen::getNumRegP() {
  return currentRegP;
}

int PsPTXGen::getNumReg() {
  return currentReg;
}

int PsPTXGen::getNumReg32() {
  return currentReg32;
}

std::string PsPTXGen::formOpCode(std::string opCode)
{
   std::stringstream s;

   s << "\t\t" << std::left << std::setw(19) << opCode;
   return s.str();
}

std::string PsPTXGen::formIndir(std::string str, int offset)
{
 std::stringstream s;
 s << "[" << str << "+" << offset << "]";
 return s.str();
}

std::string PsPTXGen::formReg(std::string name, int id)
{
   std::stringstream s;
   s << "%" << name << id;
   return s.str();
}

std::string PsPTXGen::regR(int id)
{
   return formReg("r", id);
}

std::string PsPTXGen::regR32(int id)
{
   return formReg("rf", id);
}



std::string PsPTXGen::regP(int id)
{
  return formReg("p", id);
}


void PsPTXGen::ptxNoEol(std::string cmd)
{
   *outfile << formOpCode(cmd) << std::endl;
}


void PsPTXGen::ptx(std::string cmd)
{
   ptxNoEol(cmd + ";");
}

void PsPTXGen::ptx(std::string opCode, std::string str)
{
   *outfile << formOpCode(opCode+precString) << str << ";" << std::endl;
}

void PsPTXGen::ptx(std::string opCode, std::string str1,
                   std::string str2 )
{
   *outfile << formOpCode(opCode+precString) << str1 << ", " << str2
            << ";" << std::endl;
}

void PsPTXGen::ptxNoPrec(std::string opCode, std::string str1,
                   std::string str2 )
{
   *outfile << formOpCode(opCode) << str1 << ", " << str2
            << ";" << std::endl;
}


void PsPTXGen::ptx(std::string opCode, std::string str1,
                   std::string str2, std::string str3 )
{
   *outfile << formOpCode(opCode+precString) << str1 << ", " << str2
            << ", " << str3 << ";" << std::endl;
}

void PsPTXGen::ptxNoPrec(std::string opCode, std::string str1,
                   std::string str2, std::string str3 )
{
   *outfile << formOpCode(opCode) << str1 << ", " << str2
            << ", " << str3 << ";" << std::endl;
}


void PsPTXGen::ptx(std::string opCode, std::string str1,
                   std::string str2, std::string str3, std::string str4 )
{
   *outfile << formOpCode(opCode+precString) << str1 << ", " << str2
            << ", " << str3  << ", " << str4 << ";" << std::endl;
}

void PsPTXGen::ptxNoPrec(std::string opCode, std::string str1,
                   std::string str2, std::string str3, std::string str4 )
{
   *outfile << formOpCode(opCode) << str1 << ", " << str2
            << ", " << str3  << ", " << str4 << ";" << std::endl;
}

void PsPTXGen::ptx(std::string opCode, std::string str1,  double value)
{
   *outfile << formOpCode(opCode+precString) << str1 << ", " << std::showpoint << value << ";" << std::endl;
}

void PsPTXGen::ptxNoPrec(std::string opCode, std::string str1,  double value)
{
   *outfile << formOpCode(opCode) << str1 << ", " << std::showpoint << value << ";" << std::endl;
}

void PsPTXGen::ptx(std::string opCode, double value)
{
   *outfile << formOpCode(opCode) << ", " << std::showpoint << value << ";" << std::endl;
}


void PsPTXGen::ptx(std::string opCode, int value)
{
   *outfile << formOpCode(opCode) << ", " << value << ";" << std::endl;
}

void PsPTXGen::ptxReplace(std::string str1, const int n)
{
  std::ostringstream tmp("");
  tmp<<n;
  std::string str=(*outfile).str();
  str.replace(str.find(str1),  str1.length(), tmp.str());
  (*outfile).str(str);
}

void PsPTXGen::prologe(std::string kernelName, int writeComp, double delta[3], double offset[3],
                      int xbump, int nzPitch)
{
  int major=1;
  int minor=0;

#ifdef HAVE_GPU
  cudaDeviceProp deviceProp;
  int deviceNum;
  cudaError_t err = cudaGetDevice(&deviceNum);
  err = cudaGetDeviceProperties(&deviceProp, deviceNum);
  major=deviceProp.major;
  minor=deviceProp.minor;
#endif

  ptxNoEol(".version 2.3");
  if (major==1 && minor<=2)
    ptxNoEol(".target sm_10");
  else if (major==1 && minor==3)
    ptxNoEol(".target sm_13");
  else
    ptxNoEol(".target sm_20");
  ptxNoEol(".address_size 64");

  ptxNoEol(".entry "+kernelName+ " (");

  if (writeComp > 0) ptxNoEol("\t.param .u64 result1, ");
  if (writeComp > 1) ptxNoEol("\t.param .u64 result2, ");
  if (writeComp > 2) ptxNoEol("\t.param .u64 result3, ");

  if (isDouble())
    ptxNoEol("\t.param .f64 t, ");
  else
    ptxNoEol("\t.param .f32 t, ");

  ptxNoEol("\t.param .u32 lb0, ");
  ptxNoEol("\t.param .u32 lb1, ");
  ptxNoEol("\t.param .u32 lb2, ");
  ptxNoEol("\t.param .u32 ub0, ");
  ptxNoEol("\t.param .u32 ub1, ");
  ptxNoEol("\t.param .u32 ub2 ) ");
  ptxNoEol("{");
  ptx(".reg .pred %p<NUMPREDICATES>");
  ptx(".reg .u64  %rd<12>");
  ptx(".reg .u32  %rs<25>");
  if (isDouble()) {
    ptx(".reg .f64  %f<10>");
    ptx(".reg .f64  %regx");
    ptx(".reg .f64  %regy");
    ptx(".reg .f64  %regz");
    ptx(".reg .f64  %regt");
  }
  else {
    ptx(".reg .f32  %fx<10>");
    ptx(".reg .f32  %fy<10>");
    ptx(".reg .f32  %fz<10>");
    ptx(".reg .f32  %regx");
    ptx(".reg .f32  %regy");
    ptx(".reg .f32  %regz");
    ptx(".reg .f32  %regt");
  }
  ptx(".reg .f32  %rf<NUMFLOATREGS>");

  if (isDouble()) {
    ptx(".reg .f64  %r<NUMREGS>");
  }
  else {
    ptx(".reg .f32  %r<NUMREGS>");
  }

  if (isDouble())
    ptx("ld.param.f64       %regt, [t]");
  else
    ptx("ld.param.f32       %regt, [t]");

  ptx("ld.param.s32       %rs1, [lb2]");
  ptx("cvt.s32.u16        %rs2,%tid.x");
  ptx("ld.param.s32       %rs3, [ub2]");
  ptx("sub.s32            %rs4, %rs3, %rs1");
  ptx("setp.ge.s32        %p1, %rs2, %rs4");
  ptx("@%p1 bra           $LT_exit");

  ptx("add.u32            %rs5, %rs1, %rs2");
  ptx("ld.param.s32       %rs6, [lb0]");
  ptx("cvt.u32.u16        %rs7,  %ctaid.y");
  ptx("add.u32            %rs8, %rs6, %rs7");
  ptx("ld.param.u64       %rd1, [result1]");
  ptx("cvt.u64.u32        %rd2, %rs8");
  ptx("mul.wide.u32       %rd3, %rs8", xbump);
  ptx("cvt.u32.u64        %rs9, %rd3");
  ptx("ld.param.s32       %rs10, [lb1]");
  ptx("cvt.u32.u16        %rs11, %ctaid.x");
  ptx("add.u32            %rs12, %rs10, %rs11");
  ptx("mul.lo.u32         %rs13, %rs12", nzPitch);
  ptx("add.u32            %rs14, %rs9, %rs13");
  ptx("add.u32            %rs15, %rs5, %rs14");
  ptx("cvt.s64.s32        %rd4, %rs15");
  ptx("mul.wide.s32       %rd5, %rs15", precision);       // conversion from words to bytes
  ptx("add.u64            %rd6, %rd1, %rd5");

  if (writeComp > 1) {
    ptx("ld.param.u64       %rd7, [result2]");
    ptx("add.u64            %rd8, %rd7, %rd5");
  }

  if (writeComp > 2) {
    ptx("ld.param.u64       %rd9, [result3]");
    ptx("add.u64            %rd10, %rd9, %rd5");
  }
  // at this point we have the index in rd6

  // compute x
  ptx("//Computing X");
  ptx("cvt.u32.u16        %rs16, %ctaid.y");
  ptx("add.u32            %rs17, %rs16, %rs6"); // lb0
  if (isDouble()) {
    ptx("mov.f64            %f1", offset[0]);
    ptx("cvt.rn.f64.u32    %f2, %rs17");
    ptx("mov.f64            %f3", delta[0]);
    ptx("mad.rn.f64         %regx, %f2, %f3, %f1");
  }
  else {
    ptx("mov.f32            %fx1", offset[0]);
    ptx("cvt.rn.f32.u32     %fx2, %rs17");
    ptx("mov.f32            %fx3", delta[0]);
    ptx("mul.f32            %fx4, %fx2, %fx3");
    ptx("add.f32            %regx, %fx4, %fx1");

    // if (cc<20)
    //   ptx("mad.f32          %regx, %f2, %f3, %f1");
    // else
    //   ptx("mad.ftz.f32      %regx, %f2, %f3, %f1");
  }


 // compute y
  ptx("//Computing Y");
  ptx("cvt.u32.u16        %rs18, %ctaid.x");
  ptx("add.u32            %rs19, %rs18, %rs10");  // lb1
  if (isDouble()) {
    ptx("mov.f64            %f4", offset[1]);
    ptx("cvt.rn.f64.u32     %f5, %rs19");
    ptx("mov.f64            %f6", delta[1]);
    ptx("mad.rn.f64         %regy, %f5, %f6, %f4");
  }
  else {
    ptx("mov.f32            %fy1", offset[1]);
    ptx("cvt.rn.f32.u32     %fy2, %rs19");
    ptx("mov.f32            %fy3", delta[1]);
    ptx("mul.f32            %fy4, %fy2, %fy3");
    ptx("add.f32            %regy, %fy4, %fy1");

    // ptx("mov.f32            %f4", offset[1]);
    // ptx("cvt.rn.f32.u32     %f5, %rs19");
    // ptx("mov.f32            %f6", delta[1]);

    // ptx("mul.f32            %f4, %f2, %f3");
    // ptx("add.f32            %regx, %f4, %f1");

    // if (cc<20)
    //   ptx("mad.f32          %regy, %f5, %f6, %f4");
    // else
    //   ptx("mad.ftz.f32      %regy, %f5, %f6, %f4");
  }

 // compute z
  ptx("//Computing Z");
  if (isDouble()) {
    ptx("mov.f64            %f7", offset[2]);
    ptx("add.u32            %rs20, %rs1, %rs2");  // lb2, tid.x
    ptx("cvt.rn.f64.u32    %f8, %rs20");
    ptx("mov.f64            %f9", delta[2]);
    ptx("mad.rn.f64         %regz, %f8, %f9, %f7");
  }
  else {
    ptx("mov.f32            %fz1", offset[2]);
    ptx("cvt.rn.f32.u32     %fz2, %rs20");
    ptx("mov.f32            %fz3", delta[2]);
    ptx("mul.f32            %fz4, %fz2, %fz3");
    ptx("add.f32            %regz, %fz4, %fz1");

    // ptx("mov.f32            %f7", offset[2]);
    // ptx("add.u32            %rs20, %rs1, %rs2");  // lb2, tid.x
    // ptx("cvt.rn.f32.u32     %f8, %rs20");
    // ptx("mov.f32            %f9", delta[2]);
    // if (cc<20)
    //   ptx("mad.f32          %regz, %f8, %f9, %f7");
    // else
    //   ptx("mad.ftz.f32      %regz, %f8, %f9, %f7");
  }
  ptx("//Now the expression");
}

void PsPTXGen::epiloge(int reg, int writeComp, int offset, std::string operation)
{

  if (operation=="+=") {
    int regTmp1 = allocateReg();
    int regTmp2 = allocateReg();

    ptx("ld.global", regR(regTmp1), formIndir("%rd6", offset));
    ptx("add",       regR(regTmp2), regR(regTmp1), regR(reg));
    ptx("st.global", formIndir("%rd6", offset), regR(regTmp2));

    if (writeComp > 1) {
      ptx("ld.global", regR(regTmp1), formIndir("%rd8", offset));
      ptx("add",       regR(regTmp2), regR(regTmp1), regR(reg));
      ptx("st.global", formIndir("%rd8", offset), regR(regTmp2));
    }
    if (writeComp > 2) {
      ptx("ld.global", regR(regTmp1), formIndir("%rd10", offset));
      ptx("add",       regR(regTmp2), regR(regTmp1), regR(reg));
      ptx("st.global", formIndir("%rd10", offset), regR(regTmp2));
    }
  }
  else if (operation=="*=") {
    int regTmp1 = allocateReg();
    int regTmp2 = allocateReg();

    ptx("ld.global", regR(regTmp1), formIndir("%rd6", offset));
    ptx("mul",       regR(regTmp2), regR(regTmp1), regR(reg));
    ptx("st.global", formIndir("%rd6", offset), regR(regTmp2));

    if (writeComp > 1) {
      ptx("ld.global", regR(regTmp1), formIndir("%rd8", offset));
      ptx("mul",       regR(regTmp2), regR(regTmp1), regR(reg));
      ptx("st.global", formIndir("%rd8", offset), regR(regTmp2));
    }
    if (writeComp > 2) {
      ptx("ld.global", regR(regTmp1), formIndir("%rd10", offset));
      ptx("mul",       regR(regTmp2), regR(regTmp1), regR(reg));
      ptx("st.global", formIndir("%rd10", offset), regR(regTmp2));
    }
  }
  else {
    ptx("st.global", formIndir("%rd6", offset), regR(reg));
    if (writeComp > 1) ptx("st.global", formIndir("%rd8", offset), regR(reg));
    if (writeComp > 2) ptx("st.global", formIndir("%rd10", offset), regR(reg));
  }


  ptxNoEol("$LT_exit:");
  ptx("exit");
  ptxNoEol("}");

  // this one needs a plus 1 because of the predicate in the tid<ub2-lb2 statement.
  ptxReplace("NUMPREDICATES", getNumRegP()+2);
  ptxReplace("NUMREGS", getNumReg()+1);
  ptxReplace("NUMFLOATREGS", getNumReg32()+1);
}

