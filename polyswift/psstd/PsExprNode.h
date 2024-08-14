/**
 *
 * @file    PsExprNode.h
 *
 * @brief   Defines interface for PsExprNode class and the SymbolKind
 *          enum.
 *
 * @version $Id: PsExprNode.h 19382 2012-04-20 00:24:11Z wernerg $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation
 */

#ifndef PS_EXPR_NODE_H
#define PS_EXPR_NODE_H

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
#include <PsLex.h>
#include <PsPTXGen.h>

template <class FLOATTYPE> class PsBinOp;

/**
 * @class PsExprNode
 * @brief class for an Expression node
 */
template <class FLOATTYPE>
class PsExprNode {

  public:

/**
 * Constructor
 */
    PsExprNode();

/**
 * constructs an exprNode with a symbol and a kind
 *
 * @param s the name of the node?
 * @param i who knows what this is?
 */
    PsExprNode(std::string s, int i);

/**
 * converts the symbol string into a float/double value
 */
    FLOATTYPE stringToValue();

/**
 * evaluates an exprNode. Includes evaluation of all
 * dependent nodes
 */
    FLOATTYPE evaluate();


/**
 * generates PTX code for an expression node.
 */
    int generatePTX();


/**
 * sets the PTX generato
 */
    void setPTXGen(PsPTXGen* p) { ptxGen = p; };

/**
 * shows the evaluation tree
 *
 * @param ostr the stream to print to
 */
    void printEvalTree(std::ostream& ostr);

/**
 * determines if an exprNode is constant. An expression is
 * constant, if it is constant itself, and all dependent nodes
 * are constant.
 */
    bool isConst();

/**
 * if both dependent exprNodes are constant and the exprNode itself
 * is constant, evaluates the expressions and replaces the exprNode
 * by a constant
 */
    void prune();


/**
 * determines whether the evaluation of two trees will yield the
 * same result.
 *
 * @param tree1 pointer to the root node of the first tree
 * @param tree2 pointer to the root node of the second tree
 * @return indicates if both trees compute the same value
 */
    bool compareTrees(PsExprNode* tree1, PsExprNode* tree2);

/**
 * traversers the evaluation tree and detects the first node is the
 * root of an identical subtree to the tree pointed to by stopRoot.
 * The tree traversal stops once an identical subtree is found (returns the
 * node of that tree), once the stopRoot is encountered (returns stopRoot),
 * or if it hit the leaves (returns NULL)
 *
 * @param node the root node from which to start the search for an
 *             identical tree
 * @param stopRoot the root node which will terminate the search
 * @return root node of a previously evaluated tree, stopRoot if no identical
 *       tree was found or NULL if the leaves were hit.
 */
    PsExprNode* findIdenticalTree(PsExprNode* node, PsExprNode* stopRoot);

/**
 * find common subexpressions that could be elliminated. Each tree that can
 * be precomputed sets the redirect pointer to the value variable of the
 * root node of the suttree that computes the value.
 *
 * @param exprRoot the root from which to start the optimization
 */
    void optimizeCSE(PsExprNode* exprRoot);

/**
 * traverses the tree and removes all the sub-trees that do not need
 * recomputation.
 */
    void removeCSE();


/**
 * traverses the tree and marks all subtrees that require evaluation
 * under any circumstances (mainly subtrees that contain subexpressions
 * used in other parts of the tree). Mandatory trees will not be
 * skipped by evaluation shortcutting.
 *
 * @return indicates whether the subtree is mandatory or not.
 */
    bool markMandatory();

/**
 * the symbol represented by this node
 */
    std::string symbol;

/**
 * the kind of this node
 */
    int kind;

/**
 * pointer to the binary operator used to evaluate this node or the
 * memory location where to find the data
 */
    union {
      PsBinOp<FLOATTYPE>* binOp;
      FLOATTYPE* valuePtr;
    };

/**
 * value represented by the node.
 */
    FLOATTYPE value;

/**
 * left argument
 */
    PsExprNode<FLOATTYPE> *left;

/**
 * right argument
 */
    PsExprNode<FLOATTYPE> *right;

/**
 * redirect: pointer to the expression node that
 * evaluates the tree below us.
 */
    PsExprNode<FLOATTYPE>* redirect;

 /**
  * indicates that this node has to be evaluated under
  * all circumstances (e.g. cannot be skipped via evaluation
  * shortcutting).
  */
    bool isMandatory;

 /**
  * the code generator
  */
    PsPTXGen* ptxGen;

    int nodeReg;

};

#endif

