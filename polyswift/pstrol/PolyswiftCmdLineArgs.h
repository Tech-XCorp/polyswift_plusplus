/**
 *
 * @file    PolyswiftCmdLineArgs.h
 *
 * @brief   Holder of the arguments on a command line that Polyswift++ understands
 *
 * @version $Id: PolyswiftCmdLineArgs.h 6319 2006-11-14 22:39:46Z swsides $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifndef POLYSWIFT_CMD_LINE_ARGS_H
#define POLYSWIFT_CMD_LINE_ARGS_H

// system includes
#include <TxCmdLineArgs.h>

/**
 * A PolyswiftCmdLineArgs parses out the command line data from the command line
 * arguments specifically for Polyswift.
 */
class PolyswiftCmdLineArgs : public virtual TxCmdLineArgs {

  public:

/**
 * Default constructor: set the known arguments
 */
    PolyswiftCmdLineArgs();

/**
 * Destructor
 */
    virtual ~PolyswiftCmdLineArgs() {}

  private:
// To prevent use
    PolyswiftCmdLineArgs(const PolyswiftCmdLineArgs&);
    PolyswiftCmdLineArgs& operator=(const PolyswiftCmdLineArgs&);

};

#endif  // POLYSWIFT_CMD_LINE_ARGS_H

