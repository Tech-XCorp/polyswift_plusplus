/**
 *
 * @file    PsParser.cpp
 *
 * @brief   Implementation of an arbitrary expression parser
 *
 * @version $Id: PsParser.cpp 19046 2012-01-31 21:55:34Z dws $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation
 */

#include <iostream>
#include <string>
#include <list>
#include <map>
#include <cctype>

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <txc_std.h>
#include <txc_streams.h>
#include <txc_math.h>

#include <PsParser.h>
#include <PsLex.h>


//*********************************************************************
// class PsSymbolTable
//*********************************************************************
template <class FLOATTYPE>
void PsSymbolTable<FLOATTYPE>::addOperator(std::string a,
     PsBinOp<FLOATTYPE>* op) {
  PsSymbolEntry<FLOATTYPE> s;
  s.kind = kindOPERATOR;
  s.binOp = op;
  symbols[a] = s;
}

template <class FLOATTYPE>
void PsSymbolTable<FLOATTYPE>::addExpression(std::string a, PsExprNode<FLOATTYPE>* expr) {
  PsSymbolEntry<FLOATTYPE> s;
  s.kind = kindEXPRESSION;
  s.expr = expr;
  symbols[a] = s;
}

template <class FLOATTYPE>
void PsSymbolTable<FLOATTYPE>::addVariable(std::string a, FLOATTYPE* v) {
  PsSymbolEntry<FLOATTYPE> s;
  s.kind = kindVARIABLE;
  s.varPtr = v;
  symbols[a] = s;
}

template <class FLOATTYPE>
PsBinOp<FLOATTYPE>* PsSymbolTable<FLOATTYPE>::getOperatorRef(std::string a) {
  typename std::map<std::string, PsSymbolEntry<FLOATTYPE> >::iterator p = symbols.find(a);
  if (p != symbols.end() && p->second.kind == kindOPERATOR) {
    return p->second.binOp;
  }
  return NULL;
}

template <class FLOATTYPE>
PsExprNode<FLOATTYPE>* PsSymbolTable<FLOATTYPE>::getExpressionRef(std::string a) {
  typename std::map<std::string, PsSymbolEntry<FLOATTYPE> >::iterator p = symbols.find(a);
  if (p != symbols.end() && p->second.kind == kindEXPRESSION) {
    return p->second.expr;
  }
  return NULL;
}

template <class FLOATTYPE>
FLOATTYPE* PsSymbolTable<FLOATTYPE>::getVariableRef(std::string a) {
  typename std::map<std::string, PsSymbolEntry<FLOATTYPE> >::iterator p = symbols.find(a);
  if (p != symbols.end() && p->second.kind == kindVARIABLE) {
    return p->second.varPtr;
  }
  return NULL;
}

template <class FLOATTYPE>
void PsSymbolTable<FLOATTYPE>::checkUndefVar(std::map<std::string, int> varMap) {
  typename std::map<std::string, PsSymbolEntry<FLOATTYPE> >::iterator p = symbols.begin();
  for (; p != symbols.end(); p++) {
    if (p->second.kind == kindVARIABLE) {
      if ( varMap.find(p->first) == varMap.end()) {
          TxDebugExcept tde("PsSymbolTable::checkUndefVar: Undefined symbol '");
          tde << p->first << "'";
          throw tde;
      }
    }
  }
}


//**********************************************************************
// end class PsSymbolTable
//**********************************************************************

//*******************************************************************
//  class PsParser
//*******************************************************************
template <class FLOATTYPE>
PsParser<FLOATTYPE>::PsParser() {
  expRoot = NULL;
  isDirty = true;
  useCache = true;
}

template <class FLOATTYPE>
PsParser<FLOATTYPE>::PsParser(std::string a) {
  expRoot = NULL;
  isDirty  = true;
  useCache = true;

  try {
    setExpression(a);
  }
  catch (TxDebugExcept& tde) {
    // clean up
    for (i=l.begin(); i!= l.end(); i++)
      delete *i;
    TxDebugExcept tde2("\nError while parsing expression `");
    tde2 << a << "':\n" << tde.getMessage();
    throw tde2;
  }
}

template <class FLOATTYPE>
PsParser<FLOATTYPE>::~PsParser() {
  for (i=l.begin(); i!= l.end(); i++)
    delete *i;
}

template <class FLOATTYPE>
void PsParser<FLOATTYPE>::setExpression(std::string a) {
#undef PRINTTREE
#ifdef PRINTTREE
  std::cerr << " Building the tree " << std::endl;
#endif
  buildTokenList(a);
  buildEvalTree();
  // printTokenList();
  buildDefSymbTable();
  buildSymbTable();
  pruneTree();
  optimizeCSE();
  expRoot->setPTXGen(ptxGen);
#ifdef PRINTTREE
  expRoot->printEvalTree();
  std::cerr << " finished building the tree " << std::endl;
#endif
}

template <class FLOATTYPE>
void PsParser<FLOATTYPE>::buildTokenList(std::string a) {
 size_t pos = 0;
 PsExprNode<FLOATTYPE> *node;
 std::string token;
 int kind;

 PsLex::removeWhiteSpaces(a);
 while (pos < a.length()) {
   PsLex::getNextToken(a, pos, token, kind);
   node = new PsExprNode<FLOATTYPE>(token, kind);
   node->setPTXGen(ptxGen);
   l.push_back(node);
 }
}

template <class FLOATTYPE>
PsExprNode<FLOATTYPE>* PsParser<FLOATTYPE>::parseExpr() {
  //std::cout << "parseExpr 1: " << (**i).symbol << std::endl;
  PsExprNode<FLOATTYPE>* root = parseSum();  // parse the first expression
  while (i != l.end() && ((*i)->symbol == "<" || (*i)->symbol == ">"
      || (*i)->symbol == "<=" || (*i)->symbol == ">="
      || (*i)->symbol == "==" || (*i)->symbol == "!=")) {
    (*i)->left = root;                       // set first expression to LHS
    root = *i;                               // set root to operator token
    ++i;                                     // move on in token list
    root->right = parseSum();               // set next expression to RHS
    if (root->right == NULL) {
      TxDebugExcept tde("PsParser::parseExpr: empty right operand for logical operator.");
      throw tde;
   }
  }
  if ((i == l.end()) || ((*i)->symbol == ")") || ((*i)->symbol == ","))
    return root;
  else {
    TxDebugExcept tde("PsParser::parseExpr: unexpected symbol: ");
    tde << (*i)->symbol;
    throw tde;
  }
}

template <class FLOATTYPE>
PsExprNode<FLOATTYPE>* PsParser<FLOATTYPE>::parseSum() {
  //std::cout << "parseExpr 1: " << (**i).symbol << std::endl;
  PsExprNode<FLOATTYPE>* root = parseTerm();  // parse the first expression
  while (i != l.end() && ((*i)->symbol == "+" || (*i)->symbol == "-")) {
    (*i)->left = root;                       // set first expression to LHS
    root = *i;                               // set root to operator token
    ++i;                                     // move on in token list
    root->right = parseTerm();               // set next expression to RHS
    if (root->right == NULL) {
      TxDebugExcept tde("PsParser::parseSum: empty right operand for operator + or -.");
      throw tde;
   }
  }
  return root;
  //  if ((i == l.end()) || ((*i)->symbol == ")") || ((*i)->symbol == ","))
  //  return root;
  //else {
  //  TxDebugExcept tde("PsParser::parseExpr: unexpected symbol: ");
  //  tde << (*i)->symbol;
  //  throw tde;
  // }
}


template <class FLOATTYPE>
PsExprNode<FLOATTYPE>* PsParser<FLOATTYPE>::parseTerm() {
 PsExprNode<FLOATTYPE>* root = parseFact();      // parse first factor
 while (i != l.end() && ((*i)->symbol == "*" || (*i)->symbol == "/")) {
   (*i)->left = root;                 // set first factor to LHS
   root = *i;                         // set root to operator token
   ++i;                               // move to next token in list
   root->right = parseFact();         // set the next factor to RHS
   if (root->right == NULL) {
     TxDebugExcept tde("PsParser::parseTerm: empty right operand for operator * or /.");
     throw tde;
   }
 }
 return root;
}

template <class FLOATTYPE>
PsExprNode<FLOATTYPE>* PsParser<FLOATTYPE>::parseFact() {
  PsExprNode<FLOATTYPE>* root = parseSymb();
  if (i == l.end()) return root;
  while (i!=l.end() && ((*i)->symbol == "^" || (*i)->symbol == "**")) {
    (*i)->left = root;
    root = *i;
    ++i;
    if (i == l.end()) {
      std::cout << "PARSE ERROR: Missing exponent!" << std::endl;
    }
    root->right = parseFact();
    if (root->right == NULL) {
      TxDebugExcept tde("PsParser::parseFact: empty right operand for operator ^.");
      throw tde;
    }
  }
  return root;
}

template <class FLOATTYPE>
PsExprNode<FLOATTYPE>* PsParser<FLOATTYPE>::parseSymb() {

  PsExprNode<FLOATTYPE>* root;

  if (i == l.end()) goto endofexp;
  if ((*i)->symbol == "(") {
    ++i;
    root = parseExpr();
    if (i == l.end()) goto endofexp;
    if ((*i)->symbol != ")") {
      TxDebugExcept tde("PsParser::parseSymb: unbalanced parentheses.");
      tde << "  '(' closed by " << (*i)->symbol;
      throw tde;
    }
    ++i;
    return root;
  }

  if ((*i)->symbol == "-") {
    (*i)->symbol = "inv";
    root = *i;
    ++i;
    PsExprNode<FLOATTYPE>* le = parseFact();
    if (le == NULL) {
      TxDebugExcept tde("PsParser::parseSymb: empty operand for unary operator -.");
      throw tde;
    }
    root->left = le;
    //    root->right = NULL;
    return root;
  }

  if ((*i)->symbol == "+") {
    ++i;
    root = parseFact();
    return root;
  }

  if ((*i)->kind == kindALPHA) {
    // std::cout<<"alpha terminal found!" << std::endl;
    root = *i;
    //root->left = NULL;
    //root->right= NULL;
    ++i;
    if (i == l.end()) return root;
    if ((*i)->symbol == "(") {
      ++i;
      PsExprNode<FLOATTYPE>* le = parseExpr();
      root->left = le;

      if (i == l.end()) goto endofexp;
      if ((*i)->symbol == ",") { // second argument to the BinOp?
        ++i;
        PsExprNode<FLOATTYPE>* re = parseExpr();
        root->right = re;
        if ((root->left == NULL) || (root->right == NULL)) {
          TxDebugExcept tde("PsParser::parseSymb: empty argument for two-argument function.");
          throw tde;
        }
      }

      if (i == l.end()) goto endofexp;
      if ((*i)->symbol != ")") {
        TxDebugExcept tde("PsParser::parseSymb: unbalanced parentheses after"
            " function name for ");
        std::stringstream sstr;
        root->printEvalTree(sstr);
        std::string s;
        sstr >> s;
        tde << s;
        tde << ".  '(' closed by " << (*i)->symbol;
        throw tde;
      }
      ++i;
    }
    return root;
  }

  if ((*i)->kind == kindNUMBER) {
    root = *i;
    // std::cout << " number found = " << (**i).symbol << std::endl;
    ++i;
    return root;
  }

 // std::cout << "parseTerminal :: parse error!" << std::endl;
  if (((*i)->symbol == ")") || ((*i)->symbol == ",")) return NULL;
  else {
    TxDebugExcept tde("PsParser::parseSymb: unexpected symbol: ");
    tde << (*i)->symbol;
    throw tde;
  }

endofexp:
  TxDebugExcept tde("PsParser::parseSymb: unexpected end of expression.");
  throw tde;

  // return NULL;
}

template <class FLOATTYPE>
void PsParser<FLOATTYPE>::buildEvalTree() {
 TxDebugExcept tde("PsParser::buildEvalTree: ");

 i = l.begin();
 expRoot = parseExpr();

 if (i != l.end()) {
   if ((*i)->symbol == ")") tde << "too many ')'s.";
   else tde << "unexpected symbol: " << (*i)->symbol;
   throw tde;
 }
 if (expRoot == NULL) {
   tde << "empty expression.";
   throw tde;
 }
}

template <class FLOATTYPE>
void PsParser<FLOATTYPE>::printTokenList() {
  for (i = l.begin(); i != l.end(); i++) {
    std::cout << (**i).symbol << "  node=" << (*i) << "  left="
        << (**i).left << " right=" << (**i).right <<std::endl;
  }
}

template <class FLOATTYPE>
void PsParser<FLOATTYPE>::assignValue(std::string a, FLOATTYPE val) {
  FLOATTYPE* varPtr = symbolTable.getVariableRef(a);
  if (varPtr != NULL) {
    *varPtr = val;
  }
  else {
    std::cout << "assign Value: Variable " << a << " not found!" << std::endl;
  }
}

template <class FLOATTYPE>
FLOATTYPE* PsParser<FLOATTYPE>::getValuePtr(std::string a) {
 return  symbolTable.getVariableRef(a);
}

template <class FLOATTYPE>
void PsParser<FLOATTYPE>::checkUndefVar(std::map<std::string, int>& varMap) {
 symbolTable.checkUndefVar(varMap);
}

template <class FLOATTYPE>
FLOATTYPE PsParser<FLOATTYPE>::evaluate() {
  if ( isDirty) {
    value = expRoot->evaluate();
    if ( useCache) {
       isDirty = false;
    }
  } //else {
    //std::cout <<  "reusing cached value " << std::endl;
    //}
  return value;
}

template <class FLOATTYPE>
int PsParser<FLOATTYPE>::generatePTX() {
  int reg = expRoot->generatePTX();
  return reg;
}



template <class FLOATTYPE>
void PsParser<FLOATTYPE>::pruneTree() {
  expRoot->prune();
}

template <class FLOATTYPE>
void PsParser<FLOATTYPE>::optimizeCSE() {
  expRoot->optimizeCSE(expRoot);
  expRoot->removeCSE();
  expRoot->markMandatory();
}

template <class FLOATTYPE>
void PsParser<FLOATTYPE>::buildDefSymbTable() {

  symbolTable.addOperator("<",  &opLessThan);
  symbolTable.addOperator(">",  &opMoreThan);
  symbolTable.addOperator("<=", &opLessEqual);
  symbolTable.addOperator(">=", &opMoreEqual);
  symbolTable.addOperator("==", &opEqual);
  symbolTable.addOperator("!=", &opNotEqual);
  symbolTable.addOperator("+", &opPlus);
  symbolTable.addOperator("-", &opMinus);
  symbolTable.addOperator("*", &opTimes);
  symbolTable.addOperator("/", &opDiv);
  symbolTable.addOperator("^", &opPow);
  symbolTable.addOperator("**", &opPow);
  symbolTable.addOperator("pow", &opPow);
  symbolTable.addOperator("sin", &opSin);
  symbolTable.addOperator("cos", &opCos);
  symbolTable.addOperator("exp", &opExp);
  symbolTable.addOperator("tan", &opTan);
  symbolTable.addOperator("acos", &opAcos);
  symbolTable.addOperator("asin", &opAsin);
  symbolTable.addOperator("atan", &opAtan);
  symbolTable.addOperator("atan2", &opAtan2);
  symbolTable.addOperator("sinh", &opSinH);
  symbolTable.addOperator("cosh", &opCosH);
  symbolTable.addOperator("tanh", &opTanH);
  symbolTable.addOperator("ln", &opLn);
  symbolTable.addOperator("mod", &opMod);
  symbolTable.addOperator("modmod", &opModMod);
  symbolTable.addOperator("inv", &opInv);
  symbolTable.addOperator("H",   &opH);
  symbolTable.addOperator("J0", &opJ0);
  symbolTable.addOperator("J1", &opJ1);
  symbolTable.addOperator("J2", &opJ2);
  symbolTable.addOperator("J3", &opJ3);
  symbolTable.addOperator("abs", &opAbs);
  symbolTable.addOperator("sqrt", &opSqrt);
  symbolTable.addOperator("rand", &opRand);
  symbolTable.addOperator("gauss", &opGauss);
#ifndef _WIN32
  symbolTable.addOperator("erf", &opErf);
#endif
  symbolTable.addOperator("print", &opPrint);
  symbolTable.addOperator("ceil", &opCeil);
  symbolTable.addOperator("floor", &opFloor);
  symbolTable.addOperator("min", &opMin);
  symbolTable.addOperator("max", &opMax);
}

template <class FLOATTYPE>
void PsParser<FLOATTYPE>::buildSymbTable() {

  for (i=l.begin(); i!=l.end(); i++) {

    //     std::cout << "symbol = " << (*i)->symbol << std::endl;

    if ((*i)->kind == kindNUMBER) {
      (*i)->value = (*i)->stringToValue();
      continue;
    }


    PsExprNode<FLOATTYPE>* en = symbolTable.getExpressionRef((*i)->symbol);
    if (en != NULL) {
      (*i)->kind = kindEXPRESSION;
      (*i)->left = en;
      (*i)->right = NULL;
    }

    PsBinOp<FLOATTYPE>* bo = symbolTable.getOperatorRef((*i)->symbol);
    if (bo != NULL) {

      // std::cout << " found operator " << std::endl;
      (*i)->binOp = bo;
      (*i)->kind = kindOPERATOR;
      if ( !(*bo).isConst ) {
         useCache = false;
      }
      continue;
    }

    if ((*i)->kind == kindPUNCT) {
      continue;
    }

    (*i)->kind = kindVARIABLE;
    FLOATTYPE* vp = symbolTable.getVariableRef((*i)->symbol);
    if (vp != NULL) {
      (*i)->valuePtr = vp;
    }
    else {
      FLOATTYPE* vPtr = &((*i)->value);
      symbolTable.addVariable((*i)->symbol, vPtr);
      (*i)->valuePtr = vPtr;
    }
  }
}

//*************************************************************
// end class parser
//*************************************************************

// some instantiations..

template class PsParser<float>;
template class PsParser<double>;

