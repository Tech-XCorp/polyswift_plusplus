/**
 *
 * @file    PsLex.cpp
 *
 * @brief   Implementation of PsLex
 *
 * @version $Id: PsLex.cpp 19046 2012-01-31 21:55:34Z dws $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation
 */

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <PsLex.h>

void PsLex::removeWhiteSpaces(std::string& a) {
  size_t pos = 0;
  // at present, we only consider regular spaces
  while ((pos = a.find(" ", pos)) != std::string::npos)
  a.erase(pos, 1);
}

void PsLex::advanceAlpha(std::string a, size_t& pos) {
 while (pos < a.length() && (isalnum(a[pos]) || a[pos] == '_'))
   ++pos;
}

void PsLex::advanceInt(std::string a, size_t& pos) {
 while (pos < a.length() && isdigit(a[pos]))
    ++pos;
}

void PsLex::advanceNumber(std::string a, size_t& pos) {

  if (a[pos] != '.') {
    advanceInt(a, pos);
    if (pos >= a.length()) return;
  }
  else if (pos+1 >= a.length() || !isdigit(a[pos+1]))
    goto notanumber;

  if (a[pos] == '.') {
    ++pos;
    advanceInt(a, pos);
    if (pos >= a.length()) return;
  }

  if (a[pos] == 'e' || a[pos] == 'E') {
    ++pos;
    if (pos >= a.length()) goto notanumber;
    if (a[pos] =='+')  // allow signed exponent
      ++pos;
    else if (a[pos] == '-')
      ++pos;
    if (pos >= a.length() || !isdigit(a[pos]))
      goto notanumber;
    advanceInt(a, pos);
  }
  return;

 notanumber:
  size_t pos1 = pos;
  if (pos1 >= 10) {
    pos1 -= 10;
  }
  else {
    pos1 = 0;
  }
  size_t sublen = 21;
  if (sublen > a.length() - pos1) {
    sublen = a.length() - pos1;
  }
  TxDebugExcept tde("Error: failed to interpret expression '");
  tde << a << "'; the failure occurred at character position " <<
    pos << " in the expression, at which point the character is '" << a[pos] << "'; "
    "this character appears in the context '" <<
    a.substr(pos1, sublen) << "'.  Please rewrite the expression so that "
    "it can be unambiguously interpreted.";
  throw tde;
}

void PsLex::advancePunct(std::string a, size_t& pos) {
// up to now we only support single character punctuations
//  if ((pos < a.lentgh()-2) && ((a[pos] == '=') && (a[pos+1]== '=')))
//               ++pos;

//  if ((pos < a.length() - 2) && ((a[pos] == '*') && (a[pos+1]== '*'))) {
//      ++pos;
//  }

  if (pos< a.length() - 2) {
    if (((a[pos] == '*') && (a[pos+1]== '*')) ||
        ((a[pos] == '<' || a[pos] == '>' || a[pos]=='=' || a[pos]=='!')
        && (a[pos+1] == '='))) {
      ++pos;
    }
  }
  ++pos;
}

void PsLex::getNextToken(std::string a, size_t& pos, std::string& str, int& kind) {
  size_t posOld = pos;

  if (isalpha(a[pos]) || a[pos] == '_') {
       advanceAlpha(a, pos);
       kind = kindALPHA;
  }
  else if (isdigit(a[pos]) || a[pos] == '.') {
       advanceNumber(a, pos);
       kind = kindNUMBER;
  }
  else {
       advancePunct(a, pos);
       kind = kindPUNCT;
  }
  str = a.substr(posOld, pos-posOld);
}

