/**
 *
 * @file    PsExprNode.cpp
 *
 * @brief   Implementation of an expression node - something that can be
 *          evaluated.
 *
 * @version $Id: PsExprNode.cpp 19382 2012-04-20 00:24:11Z wernerg $
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

// vpstd includes
#include <PsBinOp.h>
#include <PsExprNode.h>

// **********************************************************************
// class PsExprNode
// **********************************************************************
template <class FLOATTYPE>
PsExprNode<FLOATTYPE>::PsExprNode() {
  symbol = "";
  value = 0.;
  kind = kindUNDEF;
  left = NULL;
  right = NULL;
  redirect = NULL;
  isMandatory = false;
  nodeReg = 0;
}

template <class FLOATTYPE>
PsExprNode<FLOATTYPE>::PsExprNode(std::string a, int k) {
  value = 0.;
  symbol = a;
  kind = k;
  left = NULL;
  right= NULL;
  redirect = NULL;
  isMandatory = false;
  nodeReg = 0;
}

template <class FLOATTYPE>
FLOATTYPE PsExprNode<FLOATTYPE>::stringToValue() {
  FLOATTYPE val = 0.;
  //std::cout << "evalValue = " << symbol << " "
  //     << symbol.c_str() << " " << atof(symbol.c_str())<< std::endl;
  switch (kind) {
  case kindNUMBER : val = atof(symbol.c_str()); break;
  case kindALPHA  : val = -1; break;
 }
 return val;
}

template <class FLOATTYPE>
FLOATTYPE  PsExprNode<FLOATTYPE>::evaluate() {
  FLOATTYPE valueLeft = 0;
  FLOATTYPE valueRight = 0;

  if (kind == kindNUMBER) return value;
  if (kind == kindVARIABLE) return *valuePtr;

  if (left != NULL) valueLeft = left->evaluate();

  FLOATTYPE val;

// check if we can shortcut the evaluation
  if ( binOp->hasShortCut) {
    if ( valueLeft == binOp->shortCutValue) {
      if (right != NULL && !right->isMandatory) {
// perform evaluation shortcut
        value = binOp->shortCutResult;
        return value;
      }
    }
  }

  if (right != NULL) valueRight = right->evaluate();
  val =  binOp->evaluate(valueLeft, valueRight);
  value = val;
  return val;
}



template <class FLOATTYPE>
int   PsExprNode<FLOATTYPE>::generatePTX() {
  int regLeft = 0;
  int regRight = 0;


  if (kind == kindNUMBER) {
    nodeReg = ptxGen->allocateReg();
    ptxGen->ptx("mov", ptxGen->regR(nodeReg), (double) value);
    return nodeReg;
  }

  if (kind == kindVARIABLE) {
    nodeReg = ptxGen->allocateReg();
    std::string src;
    if (redirect != NULL) {
        // the following would register used for the previously computed
        // value. This could increase register pressure unnecessarily, so
        // instead we just assign the actual value.
        src = ptxGen->regR(redirect->nodeReg);
        ptxGen->ptx("mov", ptxGen->regR(nodeReg), src);
        //ptxGen->ptx(std::string("mov.f64 "), ptxGen->regR(nodeReg),
        //                                         redirect->value);
    }
    else {
        src = "%reg" + std::string(symbol);
        if (ptxGen->isDouble()) {
          ptxGen->ptx("mov", ptxGen->regR(nodeReg), src);
        }
        else {
          ptxGen->ptx("mov", ptxGen->regR(nodeReg), src);
        }
    }
    return nodeReg;
  }

  if (left != NULL) regLeft = left->generatePTX();

  if (right != NULL) regRight = right->generatePTX();

  binOp->setPTXGen(ptxGen);
  nodeReg  =  binOp->generatePTX(regLeft, regRight);

  return nodeReg;
}

//
// Print this node to a tree
template <class FLOATTYPE>
void PsExprNode<FLOATTYPE>::printEvalTree(std::ostream& ostr) {
  ostr << symbol << "(";
  if (left != NULL) left->printEvalTree(ostr);
  ostr << "|";
  if (right!= NULL) right->printEvalTree(ostr);
  ostr << ")";
}

template <class FLOATTYPE>
bool PsExprNode<FLOATTYPE>::isConst() {
  if (kind == kindVARIABLE) return false;
  if (kind == kindNUMBER) return true;

  if (!binOp->isConst) return false;

  bool isConstLeft = true;
  bool isConstRight = true;
  if (left != NULL) isConstLeft = left->isConst();
  if (right !=NULL) isConstRight = right->isConst();
  return (isConstLeft & isConstRight);
}

template <class FLOATTYPE>
void PsExprNode<FLOATTYPE>::prune() {
  if (left != NULL)
    left->prune();
  if (right != NULL)
    right->prune();

  if (!isConst()) return;
  if (left == NULL && right == NULL) return;

  value = evaluate();

// Should be done with a diagnostic level
  kind = kindNUMBER;
  left = NULL;
  right = NULL;
}

template <class FLOATTYPE>
bool PsExprNode<FLOATTYPE>::compareTrees(PsExprNode* tree1, PsExprNode* tree2) {

// compare the two nodes
  if (tree1->kind != tree2->kind)     return false;

  if (tree1->kind == kindNUMBER &&
     (tree1->value != tree2->value)) return false;
  if (tree1->kind == kindVARIABLE &&
     (tree1->valuePtr != tree2->valuePtr)) return false;
  if (tree1->kind == kindOPERATOR &&
     (tree1->binOp != tree2->binOp)) return false;

// trees can never be identical if they contain non-const operators
// like 'print' or 'rand'
  if ((tree1->kind == kindOPERATOR) &&
     (!tree1->binOp->isConst)) return false;

// compare the sub-trees
  if ((tree1->left == NULL) ^ (tree2->left == NULL)) return false;
  if ((tree1->right == NULL) ^ (tree2->right == NULL)) return false;

  bool equal = true;
  if ((tree1->left != NULL) && (tree2->left != NULL))
    equal = equal & compareTrees(tree1->left, tree2->left);

  if (!equal) return false;

  if ((tree1->right !=NULL) && (tree2->right != NULL))
    equal = equal & compareTrees(tree1->right, tree2->right);

  return equal;
}

template <class FLOATTYPE>
PsExprNode<FLOATTYPE>* PsExprNode<FLOATTYPE>::findIdenticalTree(PsExprNode* node,
    PsExprNode* stopRoot) {

  if (node == stopRoot ) {
    return stopRoot;
  }

  bool ident = compareTrees(node, stopRoot);


  if (ident) {
    stopRoot->redirect = node;
    node->isMandatory = true;
    return stopRoot;
  }

  if (node->left != NULL) {
    PsExprNode* res = findIdenticalTree(node->left, stopRoot);
    if (res == stopRoot) return stopRoot;
  }

  if (node->right!= NULL) {
    PsExprNode* res = findIdenticalTree(node->right, stopRoot);
    if (res == stopRoot) return stopRoot;
  }

  return NULL;
}

template <class FLOATTYPE>
void PsExprNode<FLOATTYPE>::optimizeCSE(PsExprNode* exprRoot) {

  if (left != NULL) {
    left->optimizeCSE(exprRoot);
  }

  if (right != NULL) {
    right->optimizeCSE(exprRoot);
  }

  findIdenticalTree(exprRoot, this);
}


template <class FLOATTYPE>
void PsExprNode<FLOATTYPE>::removeCSE() {
  if (redirect != NULL) {
    kind  = kindVARIABLE;
    valuePtr = &(redirect->value);
    left = NULL;
    right = NULL;
  }
  else {
    if (left != NULL)
      left->removeCSE();
    if (right !=NULL)
      right->removeCSE();
  }
}

template <class FLOATTYPE>
bool PsExprNode<FLOATTYPE>::markMandatory() {
  if (left != NULL)
    isMandatory |= left->markMandatory();
  if (right !=NULL)
    isMandatory |= right->markMandatory();

  return isMandatory;
}




// Instantiate
template class PsExprNode<float>;
template class PsExprNode<double>;

