/**
 *
 * @file    PolyswiftCmdLineArgs.cpp
 *
 * @brief   Holder of command line args as an attribute set
 *
 * @version $Id: PolyswiftCmdLineArgs.cpp 7547 2007-05-21 21:58:58Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// system includes
#include "PolyswiftCmdLineArgs.h"

// Constructor: set the known command line options
PolyswiftCmdLineArgs::PolyswiftCmdLineArgs() : TxCmdLineArgs("PolyswiftCmdLineArgs") {

  //
  // Accepted command line args
  //

  // The dump periodicity
  appendOption("d", 0);

  // Dimensionality override
  // SWS: dont think I need this
  appendOption("dim", 0);

  // The input file
  appendString("i", "");

  // The number of steps
  appendOption("n", 0);

  // The output file base name
  appendString("o", "");

  // The dump to restart from
  appendOption("r", 0);

  // display version number
  appendOption("v", 0);
  appendOption("-version", 0);

  // display svn revision of this build
  appendOption("svn", 0);
  appendOption("-svn", 0);

  // show license info
  appendOption("-license", 0);

  //
  // Arguments string (will be parsed and sent to txpp)
  //
  appendString("iargs", " ");

  // For macro import file
  appendString("im", "");

  // For dumping processed input file
  appendOption("indump", 0);

// For dumping debug info associated with internal txpp
  appendOption("dbgtxpp", 0);

  //
  // Help and license args
  //

// display help for pre file object
  appendString("h", "");

// print attribute/child information in xml
  appendOption("xml", 0);

  // run validation on in file and quit
  appendOption("validate", 0);
  appendOption("-validate", 0);

  // Alternate license location.
  appendString("-license-path", "");
}
