/**
 *
 * @file    PsLex.h
 *
 * @brief   Defines interface for the PsLex and the enum SymbolKind
 *
 * @version $Id: PsLex.h 19046 2012-01-31 21:55:34Z dws $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation
 */

#ifndef PS_LEX_H
#define PS_LEX_H

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// txbase includes
#include <TxDebugExcept.h>

// vpstd includes

/**
 * Enumerating symbol kinds
 * :TODO: put this inside PsLex namespace
 */
enum SymbolKind {
// only alpha, number, punct are used by PsLex; others used by PsExprNode
  kindUNDEF,  // what an ExprNode is immediately after default construction
  kindALPHA, // a token beginning with a letter or '_', e.g. "sin", "x"
  kindNUMBER, // a token beginning with a digit or '.', e.g. "1e-7"
              // or an exprNode for a number
  kindPUNCT, // a token that's not kindALPHA or kindNUMBER, e.g. "==" or "+"
             // or "," or "("
  kindOPERATOR, // an ExprNode for a function, e.g. "sin" or "+"
  kindVARIABLE, // an ExprNode for a variable
  kindEXPRESSION // I don't think this is used anywhere
};

/**
 * @namespace PsLex
 * @brief class implementing a simple lexical analyzer
 */
namespace PsLex {

/**
 * remove all white spaces in a string. This assumes that tokens are not
 * separated by white spaces only.
 *
 * @param a the string from which white space is to be removed.
 */
    void removeWhiteSpaces(std::string& a);

/**
 * advances the input stream by one alpha token
 * [a-zA-Z]([a-zA-Z])*
 *
 * @param a the string under consideration
 * @param pos the present position on call, the position of the
 * next alpha token upon return
 */
    void advanceAlpha(std::string a, size_t& pos);

/**
 * advances the input stream by one integer token
 * [0-9](0-9)*
 * @param a the string under consideration
 * @param pos the present position on call, the position of the
 * next alpha token upon return
 */
    void advanceInt(std::string a, size_t& pos);

/**
 * advances the input stream by one number token
 * integer(.integer)([eE]integer)
 * @param a the string under consideration
 * @param pos the present position on call, the position of the
 * next alpha token upon return
 */
    void advanceNumber(std::string a, size_t& pos);

/**
 * advance the input stream by a punctuation token
 * (currently just advances by a single character)
 * @param a the string under consideration
 * @param pos the present position on call, the position of the
 * next alpha token upon return
 */
    void advancePunct(std::string a, size_t& pos);

/**
 * returns the next token and its kind. Advances the position to the
 * beginning of the next token
 * @param a the string under consideration
 * @param pos the present position on call, the position of the
 * next alpha token upon return
 * @param str
 * @param kind
 */
    void getNextToken(std::string a, size_t& pos, std::string& str,
        int& kind);

};

#endif

