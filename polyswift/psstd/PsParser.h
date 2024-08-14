/**
 *
 * @file    PsParser.h
 *
 * @brief   Defines interface for the PsParser, PsSymbolEntry,
 *          and PsSymbolTable classes.
 *
 * @version $Id: PsParser.h 19046 2012-01-31 21:55:34Z dws $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation
 */

#ifndef PS_PARSER_H
#define PS_PARSER_H

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
#include <PsPTXGen.h>
#include <PsBinOp.h>
#include <PsExprNode.h>


/**
 * @class PsSymbolEntry
 * @brief Class for an entry in the symbol table.
 */
template <class FLOATTYPE>
class PsSymbolEntry {
  public:
    int kind;                         // what kind is the symbol?
    union {
      PsBinOp<FLOATTYPE>* binOp;
      PsExprNode<FLOATTYPE>* expr;
      FLOATTYPE* varPtr;                 // pointer to the variable
    };
};

/**
 * @class PsSymbolTable
 * @brief Base class for the actual symbol table.
 *
 * Consists of a map between the symbol and a PsSymbolEntry.
 */
template <class FLOATTYPE>
class PsSymbolTable {

  public:

/**
 * Add an operator to the symbol table
 * @param a
 * @param op
 */
    void addOperator(std::string a, PsBinOp<FLOATTYPE>* op);

/**
 * Add an operator to the symbol table
 * @param a
 * @param op
 */
    void addExpression(std::string a, PsExprNode<FLOATTYPE>* op);

/**
 * add a variable to the symbol table
 * @param a
 * @param v
 */
    void addVariable(std::string a, FLOATTYPE* v);

/**
 * get the operator reference (function pointer) for a given symbol
 * @param a
 * @return
 */
    PsBinOp<FLOATTYPE>* getOperatorRef(std::string a);

/**
 * This method needs a description!
 * @param a
 * @return
 */
    PsExprNode<FLOATTYPE>* getExpressionRef(std::string a);

/**
 * get the variable reference for a given symbol
 * @param a
 * @return
 */
    FLOATTYPE* getVariableRef(std::string a);


/**
 * checks for undefined variables in the symbol table
 *
 * @param varMap a map of variables known to the parser
 */
    void checkUndefVar(std::map<std::string, int> varMap);

  private:

/**
 * the map representing the actual symbol table
 */
    std::map<std::string, PsSymbolEntry<FLOATTYPE> > symbols;

};

/**
 * @class PsParser
 * @brief parser class
 *
 * The parser takes a expression and splits it into tokens (done
 * by the class 'lex').  The tokens are stored in a linear list
 * for simple handling. Tokens are indentified to be ALPHA symbols
 * or NUMBER symbols.
 *
 * The token list is then parsed, based on the grammar for
 * mathematical expressions and a logical evaluation tree is
 * built (Data is not actually copied, so everything remains in
 * the list. However, for each element, pointers to the left-
 * and right-argument are set up.
 *
 * In a next step, the symbol table is searched for all non-number
 * symbols. If an operator symbol is found, a function pointer to
 * the corresponding function is inserted in the node. Otherwiese,
 * it is assumed that the symbol is a varible. If no entry for the
 * variable exists in the symbol table, a new entry is created. The
 * space provide in the ExprNode is used as storage for the
 * variable and a reference to this piece of data is inserted in
 * the symbol table. If the variable is found in the symbol table,
 * the reference is set to the value in the symbol table.
 */
template <class FLOATTYPE>
class PsParser {

  public:

/**
 * concstructor
 */
    PsParser();

/**
 * constructs a parser for a given expression
 * @param a
 */
    PsParser(std::string a);

/**
 * descructor, removes the parse tree
 */
    virtual ~PsParser();

/**
 * builds a parser for a given expression
 * @param a
 */
    void setExpression(std::string a);

/**
 * assigns a value to a symbol/variable used in an expression
 * @param a
 * @param val
 */
    void assignValue(std::string a, FLOATTYPE val);

/**
 * crudely prints the list of tokens
 */
    void printTokenList();

/**
 * evaluates the parse tree
 * @return
 */
    FLOATTYPE evaluate();

/**
 * generates PTX assembly
 * @return
 */
   int generatePTX();


/**
 *  sets the PtxGenerato
 */
   void setPTXGen(PsPTXGen* p) {ptxGen = p;};

/**
 * returns a pointer to symbol/variable used in expressions
 * @param a
 * @return
 */
    FLOATTYPE* getValuePtr(std::string a);

/**
 * checks for undefined symbols in the symbol table
 *
 * @param varMap map of variables known to the parser
 */
    void checkUndefVar(std::map<std::string, int>& varMap);

/**
 * indicates the tree needs re-evaluation
 */
    void setDirty() { isDirty = true; };
/**
 * value of the last evaluation
 */
    FLOATTYPE value;

/**
 * indicates if tree needs evaluation
 */
    bool isDirty;

/**
 *  Indicate whether cache usage is possible. Expressions
 *  that contain non-constant operators can not use cache
 */
    bool useCache;

  private:

/**
 * parses an expression.
 * expr = term (['<'|'>'|'<='|'>='|'=='] term)*
 * @return
 */
    PsExprNode<FLOATTYPE>* parseExpr();

/**
 * parses a summand.
 * expr = Sum (['+'|'-'] Sum)*
 * @return
 */
    PsExprNode<FLOATTYPE>* parseSum();

/**
 * parses a term
 * term = fact (['*'|'/'] fact)*
 * @return
 */
    PsExprNode<FLOATTYPE>* parseTerm();

/**
 * parses a fact
 * fact = symb ('^' symb)*
 * @return
 */
    PsExprNode<FLOATTYPE>* parseFact();

/**
 * parses a symbol
 * symb = ('-'|'+') fact
 *      = '(' expr ')'
 *      = alphaTerm ('(') Expr ')')
 *      = number
 * @return
 */
    PsExprNode<FLOATTYPE>* parseSymb();

/**
 * builds a linear list of ExprNodes based on the input tokens
 * @param a
 */
    void buildTokenList(std::string a);

/**
 * transforms the list structure in an evaluation tree
 */
    void buildEvalTree();

/**
 * builds the default symbol table, setting up the predefined symbols
 * liken 'sin','cos', etc
 */
    void buildDefSymbTable();

/**
 * traverses the tree and finds the undefined symbols. Assigns space for
 * the variables
 */
    void buildSymbTable();

/**
 * prunes the evaluation tree by constant subexpression elimination
 */
    void pruneTree();

/**
 * perform common sub-expression ellimination on the expression tree
 */
    void optimizeCSE();


/** The plus binary operator */
    PsBinOpLessThan<FLOATTYPE> opLessThan;
    PsBinOpMoreThan<FLOATTYPE> opMoreThan;
    PsBinOpLessEqual<FLOATTYPE> opLessEqual;
    PsBinOpMoreEqual<FLOATTYPE> opMoreEqual;
    PsBinOpEqual<FLOATTYPE>     opEqual;
    PsBinOpNotEqual<FLOATTYPE>  opNotEqual;
    PsBinOpPlus<FLOATTYPE>  opPlus;
/// The minus binary operator
    PsBinOpMinus<FLOATTYPE> opMinus;
    PsBinOpTimes<FLOATTYPE> opTimes;
    PsBinOpDiv<FLOATTYPE>   opDiv;
    PsBinOpMod<FLOATTYPE>   opMod;
    PsBinOpModMod<FLOATTYPE>   opModMod;
    PsBinOpPow<FLOATTYPE>   opPow;
    PsBinOpInv<FLOATTYPE>   opInv;
    PsBinOpSin<FLOATTYPE>   opSin;
    PsBinOpCos<FLOATTYPE>   opCos;
    PsBinOpTan<FLOATTYPE>   opTan;
    PsBinOpExp<FLOATTYPE>   opExp;
    PsBinOpAsin<FLOATTYPE>  opAsin;
    PsBinOpAcos<FLOATTYPE>  opAcos;
    PsBinOpAtan<FLOATTYPE>  opAtan;
    PsBinOpAtan2<FLOATTYPE> opAtan2;
    PsBinOpSinH<FLOATTYPE>  opSinH;
    PsBinOpCosH<FLOATTYPE>  opCosH;
    PsBinOpTanH<FLOATTYPE>  opTanH;
    PsBinOpLn<FLOATTYPE>    opLn;
/// Heaviside operator.  H(0) = 0.5
    PsBinOpH<FLOATTYPE>     opH;
    PsBinOpJ0<FLOATTYPE>    opJ0;
    PsBinOpJ1<FLOATTYPE>    opJ1;
    PsBinOpJ2<FLOATTYPE>    opJ2;
    PsBinOpJ3<FLOATTYPE>    opJ3;
    PsBinOpAbs<FLOATTYPE>   opAbs;
    PsBinOpSqrt<FLOATTYPE>  opSqrt;
    PsBinOpRand<FLOATTYPE>  opRand;
    PsBinOpGauss<FLOATTYPE> opGauss;
// Error function
#ifndef _WIN32
    PsBinOpErf<FLOATTYPE>   opErf;
#endif
    PsBinOpPrint<FLOATTYPE> opPrint;
    PsBinOpCeil<FLOATTYPE>  opCeil;
    PsBinOpFloor<FLOATTYPE> opFloor;
    PsBinOpMin<FLOATTYPE> opMin;
    PsBinOpMax<FLOATTYPE> opMax;

/**
 * list of the ExprNodes, holding the tokens
 */
    std::list<PsExprNode<FLOATTYPE>*> l;

/**
 * iterator over the exprNode list. Used when parsing the expression
 */
    typename std::list< PsExprNode<FLOATTYPE>* >::iterator i;

/**
 * root of the evaluation tree
 */
    PsExprNode<FLOATTYPE>* expRoot;

/**
 * the symbol table
 */
    PsSymbolTable<FLOATTYPE> symbolTable;

/**
 * the ptx generator
 */
    PsPTXGen* ptxGen;

};

#endif

