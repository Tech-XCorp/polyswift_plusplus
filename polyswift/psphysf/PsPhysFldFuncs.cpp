/**
 * @file    PsPhysFldFuncs.cpp
 *
 * @brief   Intermeidate class for physical "observable" fields
 *
 * @version $Id: PsPhysFldFuncs.cpp 2147 2020-08-04 16:00:35Z smillie $
 *
 * Copyright &copy; 2011-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// psphysf includes
#include <PsPhysFldFuncs.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsPhysFldFuncs<FLOATTYPE, NDIM>::PsPhysFldFuncs() {
  onlyFirstDump = false;
  madeOneDump   = false;
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsPhysFldFuncs<FLOATTYPE, NDIM>::~PsPhysFldFuncs() {}

template <class FLOATTYPE, size_t NDIM>
void PsPhysFldFuncs<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Using base class to set attributes for base polymer class
  PsDynObjBase::setAttrib(tas);

  // Debug flag
  if (tas.hasString("printdebug")) {
    std::string dbPrint = tas.getString("printdebug");
    if (dbPrint == "on") this->dbStatus = PSDB_ON;
  }

  this->dbprt("PsPhysFldFuncs::setAttrib() ");

  // Data structure rank set by type spec
  if (tas.hasString("type")) {
    fieldType = tas.getString("type");
  }

  // Flag for dumping field
  if (tas.hasString("onlyFirstDump")) {
    std::string strFlag = tas.getString("onlyFirstDump");
    if (strFlag == "on") onlyFirstDump = true;
  }

}

//
template <class FLOATTYPE, size_t NDIM>
void PsPhysFldFuncs<FLOATTYPE, NDIM>::buildData() {

  this->dbprt("PsPhysFldFuncs::buildData() ");
  // Check physical size of density fields list
  //  std::vector<size_t> fldL = this->getGridBase().getDecomp().getNumCells Local();
  //  this->dbprt(" PsPhysFldFuncs::buildData flen0 = ", (int) fldL[0]);
  //  this->dbprt(" PsPhysFldFuncs::buildData flen1 = ", (int) fldL[1]);
  //  this->dbprt(" PsPhysFldFuncs::buildData flen2 = ", (int) fldL[2]);
}

//
template <class FLOATTYPE, size_t NDIM>
void PsPhysFldFuncs<FLOATTYPE, NDIM>::buildSolvers() {

  this->dbprt("PsPhysFldFuncs::buildSolvers() ");

  // Check
  for (size_t n=0; n<interactionMembers.size(); ++n) {
    this->pprt("Physical field ", this->getName());
    this->pprt("  contains interaction -> ", interactionMembers[n]);
  }
  // Check
  for (size_t n=0; n<blockMembers.size(); ++n) {
    this->pprt("Physical field ", this->getName());
    this->pprt("  contains block -> ", blockMembers[n]);
  }

// **************************************************
// txbase-IO data members

// Global/local sizes
  dataSize = this->getGridBase().getNumCellsGlobal();
  dataLen  = this->getGridBase().getDecomp().getNumCellsLocal();

// Depends on data decomp... assuming slab
  size_t offset = dataLen[0]*this->getCommBase().getRank();
  dataBeg.push_back(offset);
  dataBeg.push_back(0);
  dataBeg.push_back(0);

// Set 'effective' dimension
  if (dataSize.back() == 1) idim = 2;
  else                      idim = 3;

// Effectively pad with scaler length
// If 2D we use this as scaler length messy... needs major refactor
  if (idim == 2)
    size_t x=0;
  else {
    dataSize.push_back(1);
    dataLen.push_back(1);
    dataBeg.push_back(0);
  }
// **************************************************

}

//
template <class FLOATTYPE, size_t NDIM>
void
PsPhysFldFuncs<FLOATTYPE, NDIM>::
dump(TxIoBase* txIoPtr) {

  madeOneDump = true;

  // Generate string file name
  std::string fileName = txIoPtr->getDumpFileName(this->getName());

  // Check if filename already exists
  if (!(txIoPtr->fileExists(fileName)) ) {

    // create a file
    TxIoNodeType fn = txIoPtr->createFile(fileName);

    // Call base class dump method
    dumpField(txIoPtr, fn, densFieldName, this->getDensField() );
    dumpField(txIoPtr, fn, conjFieldName, this->getConjgField() );

    // Append the vizschema attributes
    appendMetaDataset(txIoPtr, fn, densFieldName);
    appendMetaDataset(txIoPtr, fn, conjFieldName);
    appendMetaGroup(txIoPtr, fn);
    txIoPtr->closeFile(fn);

  } // file exists

}

//
//
template <class FLOATTYPE, size_t NDIM>
void PsPhysFldFuncs<FLOATTYPE, NDIM>::restore(TxIoBase* txIoPtr) {

  // Generate sring file name
  std::string fileName = txIoPtr->getDumpFileName(this->getName());

  this->dbprt("File ", fileName, " will be searched for");

  // Check if filename already exists
  if ((txIoPtr->fileExists(fileName)) ) {

    TxIoNodeType fn = txIoPtr->openFile(fileName, "r");
    readField(txIoPtr, fn, densFieldName, this->getDensField()  );
    readField(txIoPtr, fn, conjFieldName, this->getConjgField() );
    txIoPtr->closeFile(fn);

  } // file exists
}

//
template <class FLOATTYPE, size_t NDIM>
std::string PsPhysFldFuncs<FLOATTYPE, NDIM>::getFieldType() {

  std::string dfs = this->getDensField().getFieldType();
  std::string cfs = this->getConjgField().getFieldType();
  if (dfs != cfs) {
    TxDebugExcept tde("PsPhysFldFuncs::getFieldType types not equal");
    throw tde;
  }

  return dfs;
}

//
template <class FLOATTYPE, size_t NDIM>
size_t PsPhysFldFuncs<FLOATTYPE, NDIM>::getFieldRank() {

  size_t dRank = this->getDensField().getTensorRank();
  size_t cRank = this->getConjgField().getTensorRank();
  if (dRank != cRank) {
    TxDebugExcept tde("PsPhysFldFuncs::getFieldRank ranks not equal");
    throw tde;
  }

  return dRank;
}

//
template <class FLOATTYPE, size_t NDIM>
size_t PsPhysFldFuncs<FLOATTYPE, NDIM>::getFieldSize() {

  size_t dSize = this->getDensField().getSize();
  size_t cSize = this->getConjgField().getSize();
  if (dSize != cSize) {
    TxDebugExcept tde("PsPhysFldFuncs::getFieldSize sizes not equal");
    throw tde;
  }

  return dSize;
}

//
// helper method for dumping field
//
template <class FLOATTYPE, size_t NDIM>
void
PsPhysFldFuncs<FLOATTYPE, NDIM>::
dumpField(TxIoBase* txIoPtr,
          TxIoNodeType fn,
          const std::string name,
          PsFieldBase<FLOATTYPE>& fld) {

  //  Write data to the file
  FLOATTYPE* data = fld.getDataPtr();
  TxIoNodeType dw = txIoPtr->writeDataSet(fn, name,
                                          dataSize, dataBeg, dataLen,
                                          data);

  // close nodes
  txIoPtr->closeDataSet(dw);
}

//
// helper method for reading individual field
//
template <class FLOATTYPE, size_t NDIM>
void PsPhysFldFuncs<FLOATTYPE, NDIM>::readField(TxIoBase* txIoPtr,
                                                TxIoNodeType fn,
                                                const std::string name,
                                                PsFieldBase<FLOATTYPE>& fld) {

  // Access data in field
  FLOATTYPE* data = fld.getDataPtr();
  TxIoNodeType dw = txIoPtr->readDataSet(fn, name,
                                         dataBeg, dataLen,
                                         data);
  // close nodes
  txIoPtr->closeDataSet(dw);
}

template <class FLOATTYPE, size_t NDIM>
void PsPhysFldFuncs<FLOATTYPE, NDIM>::appendMetaDataset(TxIoBase* txIoPtr,
                                                        TxIoNodeType fn,
                                                        std::string datasetName) {

// Dataset attributes
  std::string meshName   = this->getName() + "Global";
  std::string limitsName = this->getName() + "GlobalLimits";

// Labels
  std::string vsLabels;
  if (idim == 2) vsLabels = "x,y";
  if (idim == 3) vsLabels = "x,y,z";

// Open data set and write
  TxIoNodeType dn = txIoPtr->openDataSet(fn, datasetName);
  std::vector<FLOATTYPE> cellSizes = this->getGridBase().getCellSizes();

  txIoPtr->writeAttribute(dn, "vsType", "variable");
  txIoPtr->writeAttribute(dn, "vsMesh", meshName);
  txIoPtr->writeAttribute(dn, "vsLimits", limitsName);
  txIoPtr->writeAttribute(dn, "GridSizes", cellSizes);

  txIoPtr->writeAttribute(dn, "vsCentering", "zonal");   // SWS successful?
  //  txIoPtr->writeAttribute(dn, "vsCentering", "natural");
  //  txIoPtr->writeAttribute(dn, "vsCentering", "none");
  //  txIoPtr->writeAttribute(dn, "vsCentering", "nodal");
  //  txIoPtr->writeAttribute(dn, "vsCentering", "face");
  //  txIoPtr->writeAttribute(dn, "vsCentering", "edge");

  txIoPtr->writeAttribute(dn, "vsLabels",     vsLabels);
  txIoPtr->writeAttribute(dn, "vsAxisLabels", vsLabels);
  txIoPtr->writeAttribute(dn, "vsTimeGroup", "time");

  txIoPtr->closeDataSet(dn);
}

//
// vizschema group requirements
//
template <class FLOATTYPE, size_t NDIM>
void PsPhysFldFuncs<FLOATTYPE, NDIM>::appendMetaGroup(TxIoBase* txIoPtr,
                                                      TxIoNodeType fn) {

// Dataset attributes
  std::string meshName   = this->getName() + "Global";
  std::string limitsName = this->getName() + "GlobalLimits";
  std::string coordKind = "Cartesian";

// Convert to int
  std::vector<int> numCells(idim), startCell(idim);
  for (size_t n=0; n<numCells.size(); ++n) {
    startCell[n] = 0;
    numCells[n] = (int)this->getGridBase().getNumCellsGlobalInDir(n);
  }

  std::vector<FLOATTYPE> lowerBFlt, upperBFlt;
  lowerBFlt = this->getGridBase().getOrigin();
  upperBFlt = this->getGridBase().getGlobalLengths();

// Convert to double
  std::vector<double> lowerB(idim), upperB(idim);
  for (size_t n=0; n<lowerB.size(); ++n) {
    lowerB[n] = (double)lowerBFlt[n];
    upperB[n] = (double)upperBFlt[n];
  }

// append GlobalLimits
  TxIoNodeType lgn;
  if (txIoPtr->groupExists(fn, limitsName))
    lgn = txIoPtr->openGroup(fn, limitsName);
  else
    lgn = txIoPtr->createGroup(fn, limitsName);

  txIoPtr->writeAttribute(lgn, "vsType", "limits");
  txIoPtr->writeAttribute(lgn, "vsKind", coordKind);
  txIoPtr->writeAttribute(lgn, "vsLowerBounds", lowerB);
  txIoPtr->writeAttribute(lgn, "vsUpperBounds", upperB);
  txIoPtr->closeGroup(lgn);

// append Global
  TxIoNodeType ggn;
  if (txIoPtr->groupExists(fn, meshName))
    ggn = txIoPtr->openGroup(fn, meshName);
  else
    ggn = txIoPtr->createGroup(fn, meshName);

  txIoPtr->writeAttribute(ggn, "vsKind", "uniform");
  txIoPtr->writeAttribute(ggn, "vsType", "mesh");
  txIoPtr->writeAttribute(ggn, "vsLowerBounds", lowerB);
  txIoPtr->writeAttribute(ggn, "vsUpperBounds", upperB);
  txIoPtr->writeAttribute(ggn, "vsNumCells", numCells);
  txIoPtr->writeAttribute(ggn, "vsStartCell", startCell);
  txIoPtr->closeGroup(ggn);

// appendTime
  size_t timeStep = PsDynObjBase::getCurrDomainStep();
  FLOATTYPE timeVal = (FLOATTYPE)timeStep;
  TxIoNodeType tgn = txIoPtr->createGroup(fn, "time");
  txIoPtr->writeAttribute(tgn, "vsType", "time");
  txIoPtr->writeAttribute(tgn, "vsStep", timeStep);
  txIoPtr->writeAttribute(tgn, "vsTime", timeVal);
}

// Instantiation
template class PsPhysFldFuncs<float, 1>;
template class PsPhysFldFuncs<float, 2>;
template class PsPhysFldFuncs<float, 3>;

template class PsPhysFldFuncs<double, 1>;
template class PsPhysFldFuncs<double, 2>;
template class PsPhysFldFuncs<double, 3>;
