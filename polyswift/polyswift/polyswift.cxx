/**
 *  @file    polyswift.cxx
 *
 *  @brief      Main program - create simulation object and let it run
 *
 * @version $Id: polyswift.cxx 7789 2007-07-04 03:38:40Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#include <configrev.h>
#undef HAVE_CONFIG_H
#endif

// Python.h defines this
#undef HAVE_SYS_TYPES_H

// include MPI
#ifdef HAVE_MPI
#define MPICH_IGNORE_CXX_SEEK
#include <mpi.h>
#endif

// Python includes.  Must come first to avoid _POSIX_C_SOURCE warning.
#ifdef HAVE_PYTHON
// #undef _POSIX_C_SOURCE
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wregister"
#include <Python.h>
#pragma clang diagnostic pop
// Get rid of some python defines that should not be defined
#ifdef copysign
#undef copysign
#endif
#ifdef _XOPEN_SOURCE
#undef _XOPEN_SOURCE
#endif
#endif

// txbase
#include <TxHierAttribSetTmpl.h>
#include <TxMakerMap.h>

// txutils includes
#include <TxuTimer.h>
#include <txpp.h>
#include <txppio.h>

// pstrol includes
#include <PolyswiftCmdLineArgs.h>
#include <PsDomain.h>
#include <PsDomainMakerMap.h>

// txpy includes
#include <TxInternalExecute.h>

// licmgrall includes
#ifdef HAVE_SECURITY
#include <TxLicMgr.h>
#include <TxLicStatusObserver.h>
#include <TxStringParseD.h>
#include <TxLicensing.h>
#include <txppioD.h>
#include "PsLicensing.h"
#endif

// system includes
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <istream>
#include <algorithm>
#include <signal.h>
#include <sys/stat.h>

// OS defines
#ifdef _WIN32
#include <Windows.h>
#if defined(__MINGW32__)
#include <unistd.h>
#endif
#else
#include <unistd.h>
#endif

std::string outName;
PolyswiftCmdLineArgs cmdLineArgs;


// Main attribute set
TxHierAttribSetIntDbl tha("simulation");

// Used to generate macro import lookup path
std::string execFullPath;

// The single domain created: needed for catching signals
static PsDomainBase* domainPtr = 0;

// proc rank
int myRank = 0;

#ifdef HAVE_SECURITY
bool psObsLicStatusBad = false;
TxLicMgr* licenseManager = nullptr;
std::string licenseErrorMessage = "";
#endif

// Used as argument to quitPolyswift()
enum PS_ERR {
  PS_OK,             // 0
  PS_PYTHON_ERR,     // 1
  PS_CMDLINE_ERR,    // 2
  PS_ATTRIB_ERR,     // 3
  PS_RUN_ERR,        // 4
  PS_LICENSE_ERR,    // 5
  PS_INPUT_ERR,      // 6
  PS_GPU_ERR,        // 7
  PS_UNHANDLED_ERR   // 8
};

void usagePolyswift() {
  std::cout
    << "\n"
    << "Usage: polyswift -i filename [-o filename] [-r num]  [-d num] \n"
    << "\n"
    << "     -i   filename     : read input from file \'filename\'\n"
    << "     -o   filename     : output file prefix (jobname) \'filename\'\n"
    << "     -n   nsteps       : run for nsteps steps\n"
    << "     -r   num          : restart polyswift++ from dump \'num\'\n"
    << "     -d   num          : dump data every \'num\' steps\n"
    << "  iargs   txpp options : pass options (including substitutions) to internal txpp\n"
    << "                         (eg -iargs NDIM=2,DX=0.1 \n"
#ifdef HAVE_SECURITY
    << "         --license-path <path>  : specify by full path the license file to use\n"
#else
    << "-indump                : dump processed .pre file (unlicensed only)\n"
#endif
    << "\n"
    << "Note: Command-line opts will override input file variables.\n"
    << std::endl;
}

/**
 * Shut down various utilities running in Polyswift++
 */
int quitPolyswift(PS_ERR pserr);

/**
 * Read the python file, should it exist.
 *
 * @param inFileName the name of the input file.
 * @param rank determines mpi rank - so whether to print.
 *
 * Python file comes from this with .in -> .py.
 */
void readPython(std::string& inFileName,
                int rank,
                int numRanks);

/**
 * These are flags that cause information about the executable
 * to be printed and the run to quit.
 *
 * Note: this should be called before other parts of the command
 *       line are parsed because one wants the info whether other
 *       parameters are set.
 */
bool showInfoArgs(PolyswiftCmdLineArgs& cla);

/** Parse info needed for internal txpp */
// Need:  myRank, execFullPath global
void runInternalTxpp(PolyswiftCmdLineArgs& cla, std::string& fileName);


#ifdef HAVE_SECURITY
// This class is needed because main() can extend the status observer class
class PsLicObserver : public TxLicStatusObserver {
  public:
    void licenseStatusChanged(bool statusOk, std::string msg) {
      if (!statusOk) {
        psObsLicStatusBad = true;
        licenseErrorMessage = msg;
      }
    }
};
#endif

/* *************************************************************************** */
/*                             MAIN PROGRAM                                    */
/* *************************************************************************** */
int main(int argc, char** argv) {

// Get the MPI variables
  int numRanks = 1;

#ifdef HAVE_MPI
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
  MPI_Comm_size(MPI_COMM_WORLD, &numRanks);
  if (myRank == 0) {
    std::cout << "Using MPI" << std::endl;
    std::cout << "Called MPI_INIT and MPI_Comm_rank" << std::endl;
  }
#endif

// Get full path of PolySwift++ executable.
// On Windows, convert backslashes to forward slashes to simplify later use.
  std::string tmp(argv[0]);
  execFullPath = tmp;

#ifdef _WIN32
  replace(execFullPath.begin(), execFullPath.end(), '\\', '/');
  execFullPath="\"" + execFullPath + "\"";
#else
  size_t pos = 0;
  while ((pos = execFullPath.find(" ", pos)) != std::string::npos) {
    execFullPath.replace(pos, 1, "\\ ");
    pos += 2;
  }
#endif

// Start up python
#ifdef HAVE_PYTHON
  if (!Py_IsInitialized()) {
    Py_NoSiteFlag = 1;
    Py_NoUserSiteDirectory = 1;
// initsigs = 0 means do not initialize signal handlers, so Python can
// be intialized after signal handlers are are set.
    Py_InitializeEx(0);
  }
#endif

// Get the command-line args as attrib set
  std::ostream* os = NULL;
  if (!myRank) {
    os = &std::cout;
  }
  cmdLineArgs.setFromCmdLine(argc, argv, os);

// Flags for printing executable info only
  if (showInfoArgs(cmdLineArgs)) {
    return quitPolyswift(PS_OK);
  }

#ifdef HAVE_SECURITY
// products that this executable should work with
  std::vector<std::string> productNames;
  productNames.push_back("PSim");
  std::string expectedCompName = "polyswift";
  PsLicensing* psLicensing = new PsLicensing;
  bool licenseStatus = true;
  std::string licenseMsg = "";
// Pull out value of license path argument and print error or split
// into directory and filename.
  std::string specifiedLicPath = "";
  if (cmdLineArgs.hasString("-license-path")) {
    specifiedLicPath= cmdLineArgs.getString("-license-path");
    if (specifiedLicPath.size() != 0) {
#ifdef _WIN32
      struct _stat sbuf;
      if (_stat(specifiedLicPath.c_str(), &sbuf) < 0) {
#else
      struct stat sbuf;
      if (stat(specifiedLicPath.c_str(), &sbuf) < 0) {
#endif
        if (myRank == 0) {
          std::cerr << "License file, " << specifiedLicPath <<
              ", does not exist. Please verify file exists.";
        }
        return quitPolyswift(PS_CMDLINE_ERR);
      }
      replace(specifiedLicPath.begin(), specifiedLicPath.end(), '\\', '/');
    }
  }
// Construct license manager object
  licenseManager = new TxLicMgr(argv[0], productNames, expectedCompName,
      TXLICSUP_COMPONENT::TXLICSUP_ENGINE, licenseStatus, licenseMsg,
      psLicensing, specifiedLicPath);
// Set an object in this file to be the obverser of the heartbeat.
  licenseManager->setObserver(new PsLicObserver);
  if (!licenseStatus && (myRank == 0)) {
    std::cerr << "Error initializing polyswift license: " << licenseMsg;
  }

// Making licenseStatus uniform across processors. This affects how
// MPI_Finalize or MPI_Abort is called in TxsDevices
#ifdef HAVE_MPI
  MPI_Bcast(&licenseStatus, 1, MPI_INT, 0, MPI_COMM_WORLD);
#endif

  if (!licenseStatus) {
    return quitPolyswift(PS_LICENSE_ERR);
  }
#endif  // end of if HAVE_SECURITY

// Determine the input file
  std::string inFileName = cmdLineArgs.getString("i");
  if (inFileName == "") {
    if (myRank == 0) {
      std::cout << "No input file.  Quitting." << std::endl;
    }
    usagePolyswift();
    return quitPolyswift(PS_CMDLINE_ERR);
  }

// Determine the output prefix
  std::string outName = cmdLineArgs.getString("o");
  if (outName == "") {
    outName = inFileName.substr(0, inFileName.size() - 4);
    if (myRank == 0) {
      std::cout << "No output prefix, setting to input file name." << std::endl;
      std::cout << "outName from command line  = " << outName << std::endl;
    }
  }

//
// Use txpp to process input file
//
  bool usingPreFile = cmdLineArgs.isPreFile();
  if (usingPreFile) {
    runInternalTxpp(cmdLineArgs, inFileName);
  } else {
// Set input file name
    std::ifstream inFile(inFileName.c_str());
    if (inFile.bad()) {
      if (myRank == 0) {
        std::cout << "Input file '" << inFileName
                  << "' missing or unreadable.  Quitting."
                  << std::endl;
      }
      usagePolyswift();
      return quitPolyswift(PS_CMDLINE_ERR);
    }
// Read the input stream into a hierarchical attribute set
    inFile >> tha;
  }

// Store input file name into attribute set // SWS: needed?
  if (tha.hasString("inFileName")) {
    tha.setString("inFileName", inFileName);
  } else {
    tha.appendString("inFileName", inFileName);
  }

// Determine the domain kind and read dimension/precision
  std::string domainKind = "Domain";
  size_t dimension;
  if (tha.hasOption("dimension")) {
    dimension = tha.getOption("dimension");
  } else {
    dimension = 3;
  }

  std::string floattype;
  if (tha.hasString("floattype")) {
    floattype = tha.getString("floattype");
  } else {
    floattype = "double";
  }

  PsDomainMakerMap domainMakerMap;

// Create a controllable domain
  std::ostringstream ostr;
  ostr << domainKind << dimension;
  if (floattype == "float") {
    ostr << "F";
  } else {
    ostr << "D";
  }
  domainKind = ostr.str();
  if (myRank == 0) {
    std::cout << "Creating domain of kind '"
         << domainKind << "'" << std::endl;
  }
  try {
    domainPtr = TxMakerMap<PsDomainBase>::getNew(domainKind);
  } catch (TxDebugExcept tde) {
    if (myRank == 0) {
      std::cerr << "\n Unable to create domain. Exception was: " <<
           tde << std::endl;
    }
    return quitPolyswift(PS_INPUT_ERR);
  }

// Safety for Python ?
#ifdef HAVE_MPI
  MPI_Barrier(MPI_COMM_WORLD);
#endif

//
// Read the python environment
//
  try {
    readPython(inFileName, myRank, numRanks);
  } catch (TxDebugExcept& tde) {
    if (myRank == 0) {
      std::cout << "\n Exception caught while reading Python file:\n ";
      std::cout << tde << std::endl;
    }
    return quitPolyswift(PS_INPUT_ERR);
  }

// Safety for Python ?
#ifdef HAVE_MPI
  MPI_Barrier(MPI_COMM_WORLD);
#endif

// Determine the number of steps
  int nsteps = cmdLineArgs.getOption("n");
  //int nsteps = 0;
  if (nsteps == 0) {
    if (tha.hasOption("nsteps")) {
      nsteps = tha.getOption("nsteps");
    } else {
      if (myRank == 0) {
        std::cout << "\n Number of timesteps (nsteps) not set \n"
            << std::endl;
      }
      return quitPolyswift(PS_CMDLINE_ERR);
    }
  }

// Determine the print periodicity of domain
  size_t prtPeriodicity;
  if (tha.hasOption("prtPeriodicity")) {
    prtPeriodicity = tha.getOption("prtPeriodicity");
  } else {
    prtPeriodicity = 100;
    if (myRank == 0) {
      std::cout << "\n Print periodicity default = "
                << prtPeriodicity << "\n"
                << std::endl;
    }
  }

// Determine if the run is a restart
  int restoreDump = cmdLineArgs.getOption("r");

// Determine the dump periodicity
  int dumpPeriodicity = cmdLineArgs.getOption("d");
  if (dumpPeriodicity==0) {
    if (tha.hasOption("dumpPeriodicity")) {
      dumpPeriodicity = tha.getOption("dumpPeriodicity");
    } else  {
      dumpPeriodicity = nsteps;
    }
  }

/* ************************************** */
/*    Begin accessing domain methods      */
/* ************************************** */

// Set domain attributes
  try {
    domainPtr->setAttrib(tha, restoreDump, outName);
  } catch (TxDebugExcept& txde) {
    if (myRank == 0) {
      std::cout << "\n setAttrib exception: " << std::endl;
      std::cout << txde << std::endl;
    }
    return quitPolyswift(PS_INPUT_ERR);
  }

// Build steps
  try {
    domainPtr->buildData();
  } catch (TxDebugExcept& txde) {
    if (myRank == 0) {
      std::cout << "\n buildData exception: " << std::endl;
      std::cout << txde << std::endl;
    }
    return quitPolyswift(PS_INPUT_ERR);
  }

  try {
    domainPtr->buildSolvers();
  } catch (TxDebugExcept& txde) {
    if (myRank == 0) {
      std::cout << "\n buildSolvers exception: " << std::endl;
      std::cout << txde << std::endl;
    }
    return quitPolyswift(PS_INPUT_ERR);
  }

// Initialize domain OR restore from last dump if necessary
  size_t timeStep = 0;
  if (restoreDump > 0) {

// Restore the domain
    try {
      domainPtr->restore();
    } catch (TxDebugExcept& txde) {
      if (myRank == 0) {
        std::cout << "Restore exception: \n " << std::endl;
        std::cout << txde << std::endl;
      }
      delete domainPtr;
      return quitPolyswift(PS_INPUT_ERR);
    }

    timeStep = restoreDump;
    std::cout << "check the timeStep = restoreDump " << std::endl;

    if (myRank == 0) {
      std::cout << "\n Restored all from dump "
                << restoreDump << "\n" << std::endl;
    }
  } else {    // if (restoreDump > 0)

    if (myRank == 0) {
      std::cout << "\n Initializing at start \n" << std::endl;
    }

// Initialize the domain
    timeStep = 0;
    try {
      domainPtr->initialize();
    } catch (TxDebugExcept& txde) {
      if (myRank == 0) {
        std::cout << "Initialize exception: \n" << std::endl;
        std::cout << txde << std::endl;
      }
      delete domainPtr;
      return quitPolyswift(PS_INPUT_ERR);
    }

  }
// *************************************************************************

// *****************************************************************
// Run the simulation on the domain

  TxuTimer tmr;
  double totUpdateTime = 0.0;
  tmr.reset();

  for (int i=0; i<nsteps; ++i) {

#ifdef HAVE_SECURITY
    if (psObsLicStatusBad) {
      if (!licenseErrorMessage.empty()) {
        std::cerr << "Error with licensing. This run could not be validated.\n"
           << licenseErrorMessage;
      }
      return quitPolyswift(PS_LICENSE_ERR);
    }
#endif

    timeStep += 1;

// Print time step (1-10)-->prtPeriod
    int prtTst = timeStep % prtPeriodicity;
// Check and dump the domain
    int dmpTst = timeStep % dumpPeriodicity;
    if ( (timeStep <= 10) && (myRank ==0) ) {
      std::cout << "timeStep = " << timeStep << std::endl;
    }
    if ( (timeStep > 10) && (prtTst ==  0 ) && (myRank ==  0 ) ) {
      std::cout << "timeStep = " << timeStep << std::endl;
    }

// Update the domain
    try {
      tmr.reset(); tmr.start();
      domainPtr->update((double) timeStep);
      tmr.stop();
      totUpdateTime = totUpdateTime + tmr.getElapsedWallSec();
    } catch (TxDebugExcept& txde) {
      if (myRank == 0) {
        std::cout << "\n Update exception: \n" << txde << std::endl;
      }
      delete domainPtr;
      return quitPolyswift(PS_RUN_ERR);
    }

// Dump data files
    if (dmpTst == 0) {
      domainPtr->dump();
    }

  } // nsteps Loop
// *****************************************************************

// Gather timing data
#ifdef HAVE_MPI
  double totProcUpTime = 0.0;
  MPI_Allreduce(&totUpdateTime, &totProcUpTime, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
  totUpdateTime = totProcUpTime / (double)numRanks;
#endif
  if (myRank == 0) {
    std::cout << "\n (# of procs, update time/proc) = "
              << numRanks << " " << totUpdateTime << std::endl;
  }

// Successful quit
  return quitPolyswift(PS_OK);
}

void runInternalTxpp(PolyswiftCmdLineArgs& cla, std::string& fileName) {

// Debug info flag
  int dbgFlag = cla.getOption("dbgtxpp");

  if (myRank == 0 && dbgFlag == 1) {
    std::cout << "\n------ Internal preprocessor psinfo ------ " << std::endl;
  }

// Passed to internal txpp pre-processor for string substitution
  std::string iargsStr;
  if (cla.hasString("iargs")) {
    iargsStr = cla.getString("iargs");
  }
  std::vector<std::string> iargsVec;
  splitDelimitedStr(iargsVec, iargsStr, ','); // get vector of options wo commas

  if (myRank == 0 && dbgFlag == 1) {
    std::cout << "from cmd line -iargs = " << std::endl;
    for (size_t n=0; n<iargsVec.size(); ++n) {
      std::cout << "   " << iargsVec[n] << std::endl;
    }
  }

// This logic allows for eg
//    * no iargs
//    * iargs NDIM=2
//    * iargs NDIM=2,DX=0.20
//    * iargs 'NDIM=2 DX=0.20'
//    * iargs 'NDIM=2,DX=0.20'
//
// assemble split string vector into single string with spaces
// then remove any remaining spaces
  std::string iargsStr2;
  for (size_t n=0; n<iargsVec.size(); ++n) {
    iargsStr2 += iargsVec[n] + " ";
  }
  iargsVec.clear();
  splitDelimitedStr(iargsVec, iargsStr2, ' ');

  if (myRank == 0 && dbgFlag == 1) {
    std::cout << "iargsVec final = " << std::endl;
    for (size_t n=0; n<iargsVec.size(); ++n) {
      std::cout << "   " << iargsVec[n] << std::endl;
    }
  }

//
// Passed to internal txpp pre-processor
// Import path to macros etc
// If specified from polyswift cmd line then this is passed to txpp
// If -im is not used then find relative path from location of executable
//
  std::string txppImportDir;
  if (cla.hasString("im")) {
    txppImportDir = cla.getString("im");
    if (txppImportDir.length() > 0) {
      txppImportDir = "--import="   + txppImportDir;
    } else {
      stripPathStr(execFullPath);
      std::string psbasedir = execFullPath;
#ifdef _WIN32
// Remove quotes on Windows.
      const char* quotechar = "\"";
      if (strncmp(psbasedir.c_str(),quotechar,2)) {
        psbasedir.erase(psbasedir.end() - 1);
        psbasedir.erase(psbasedir.begin());
      }
      const std::string ospathsep(";");
#else
      const std::string ospathsep(":");
#endif
      std::string macdirs = psbasedir + "/../psutils/macros" + ospathsep;
      macdirs += psbasedir + "/../psutils/vpmacros" + ospathsep;
#ifdef TxUtils_MACRO_DIR
      macdirs += psbasedir + "/../share/macros" + ospathsep;
      macdirs += TxUtils_MACRO_DIR;
#else
      macdirs += psbasedir + "/../share/macros";
#endif
      txppImportDir = "--import=" + macdirs;
    }
  }

// Preprocessor flag for dumping processed input file
// only available in un-licensed version
  std::string txppInputDump = "--nodump-in-file";
#ifndef HAVE_SECURITY
  int indumpFlag = cla.getOption("indump");
  if (indumpFlag) {
    txppInputDump = " ";
  }
#endif

// Build up vector of parameters strings
  std::vector< std::string> paramVec;
  paramVec.push_back(fileName);
  paramVec.push_back(txppImportDir);
  for (size_t n=0; n<iargsVec.size(); ++n) {
    paramVec.push_back(iargsVec[n]);
  }
  if (txppInputDump.size() != 0) {
    paramVec.push_back(txppInputDump);
  }

  if (myRank == 0 && dbgFlag == 1) {
    std::cout << "paramVec = " << std::endl;
    for (size_t n=0; n<paramVec.size(); ++n) {
      std::cout << "   " << paramVec[n] << std::endl;
    }
  }

// *****************************************************************************
// Construct complete txpp module string

  std::string initStr = getPythonInitStr(paramVec, myRank);
  std::string ioStr   = convertHeader2Str(&ioBlocks[0], ioNumBlocks);
  std::string txppStr = convertHeader2Str(&txppBlocks[0], txppNumBlocks);
  std::string ioDStr;
#ifdef HAVE_SECURITY
  ioDStr = licenseManager->getPyMacroInfo();
  ioDStr += convertHeader2Str(&ioDBlocks[0], ioDNumBlocks,
      ioDCharsPerBlk, ioDLastBlkSize, licenseManager->getPyBitShift());
#endif
  std::string totalStr = initStr + ioStr + ioDStr + txppStr;
  std::string inString = runPythonString(totalStr);
  std::istringstream inFile(inString);
// *****************************************************************************

// Set global attribute set from input file and
// store input file name into attribute set
  inFile >> tha;

  if (myRank == 0 && dbgFlag == 1) {
    std::cout << "------ Internal preprocessor finished psinfo ------ \n" << std::endl;
  }
}

/**
 * Shuts down external lib calls and
 * return PS exit error code
 */
int quitPolyswift(PS_ERR pserr) {
// Finish python
#ifdef HAVE_PYTHON
  try {
    if (PyErr_Occurred()) {
      std::cout << "An exception was raised by the python interpreter:" << std::endl;
      // PyErr_Print() prints to stdout, unfortunately
      PyErr_Print();
      PyErr_Clear();
    }
    Py_Finalize();
  } catch (std::exception & e) {
    std::cerr << "An exception occurred while quitting python: " <<
      e.what() << std::endl;
  } catch (...) {
    std::cerr << "An exception occurred while quitting python." << std::endl;
  }
#endif

// Finish up MPI.
// We used to call MPI_Abort() if there was an error, but there is no reason
// for that, given that these errors all have to do with parsing the command
// line, building the domain, etc.

#ifdef HAVE_SECURITY
  delete licenseManager;
  licenseManager = nullptr;
#endif

#ifdef HAVE_MPI
  if (pserr == PS_RUN_ERR) {
    MPI_Abort(MPI_COMM_WORLD, (int) pserr);
  } else {
    MPI_Finalize();
  }
#endif

// Stop counting.  With this after MPI_Finalize returns.
  return (int) pserr;
}

/**
 * Read the python file, should it exist.
 *
 * @param inFileName the name of the input file.
 * @param rank determines mpi rank - so whether to print.
 *
 * Python file comes from this with .in -> .py.
 */
void readPython(std::string& inFileName,
                int rank,
                int numRanks) {

#ifdef HAVE_PYTHON

// Get file name
  std::string pyFileName;
  bool usingPreFile = cmdLineArgs.isPreFile();
  if (usingPreFile) pyFileName = inFileName.substr(0, inFileName.size() - 3) + "py";
  else pyFileName = inFileName.substr(0, inFileName.size() - 2) + "py";

// Open the file
  std::ifstream pystrm(pyFileName);
  if (!pystrm && rank == 0) {
    std::cout << "File, " << pyFileName << ", did not open.  "
        "Assume not present."  << std::endl;
// Clear the python error indicator since PyFile_FromString returned null
    PyErr_Clear();
  }

// Read the python file (aka, slurp on the web)
  std::ostringstream buf;
  buf << pystrm.rdbuf();
  std::string pystring = buf.str();

// Make sure sys.argv[0] is defined within the ensuing python session
  const size_t strSize = strlen(pyFileName.c_str())+1;
  wchar_t* tempStr = new wchar_t[strSize]{0};
  mbstowcs(tempStr, pyFileName.c_str(), strSize);
  int pyargc = 1;
  wchar_t* pyargv[1] = {tempStr};
  PySys_SetArgv(pyargc, pyargv);
  delete [] tempStr;

// Run the python: https://docs.python.org/3/c-api/veryhigh.html
  int pyFileRes = PyRun_SimpleString(pystring.c_str());
  if (pyFileRes) {
    TxDebugExcept tde("readPython(): Python file did not parse.");
    throw tde;
  }
  else if (rank == 0) {
    std::cout << pyFileName << " parsed on rank "
          << rank << "." << std::endl;
  }
#else
  std::cout << "Python not compiled in so not available." << std::endl;
#endif
}

/**
 * These are flags that cause information about the executable
 * to be printed and the run to quit.
 *
 * Note: this should be called before other parts of the command
 *       line are parsed because one wants the info whether other
 *       parameters are set.
 *
 *      << "PSim " << POLYSWIFT_VERSION
 */
bool showInfoArgs(PolyswiftCmdLineArgs& cla) {

  if (cla.hasOption("v") || cla.hasOption("-version")) {
    if (cla.getOption("v") || cla.getOption("-version")) {
      std::cerr
        << "POLYSWIFT " << POLYSWIFT_VERSION
        << std::endl;
      return true;
    }
  }

  if (cla.hasOption("svn") || cla.hasOption("-svn")) {
    if (cla.getOption("svn") || cla.getOption("-svn")) {
      std::cerr << POLYSWIFT_REV << std::endl;
      return true;
    }
  }
// No argument info flags
  return false;
}
