/**
 * @file    PsFixedWall.cpp
 *
 * @brief   Class containing info for fixed wall
 *
 * @version $Id: PsFixedWall.cpp 6319 2007-01-24 22:39:46Z swsides $
 *
 * Copyright &copy; 2008-2020, Tech-X Corporation, Boulder, CO.
 * All rights reserved.
 */

// psgrid includes
#include <PsFixedWall.h>

// txbase includes
#include <TxStringParse.h>
#include <TxMakerMap.h>

// Constructor
template <class FLOATTYPE, size_t NDIM>
PsFixedWall<FLOATTYPE, NDIM>::PsFixedWall() {
  wallEdgeThreshold = 0.50;

  wallSTFunc = NULL;
  bndryPtr = NULL;
  locBndryPtr = NULL;
  dataFileName = "";
}

// Destructor
template <class FLOATTYPE, size_t NDIM>
PsFixedWall<FLOATTYPE, NDIM>::~PsFixedWall() {
  delete wallSTFunc;
  delete bndryPtr;
  delete locBndryPtr;
  wallDataMap.clear();
}

//
// set attributes
//
template <class FLOATTYPE, size_t NDIM>
void PsFixedWall<FLOATTYPE, NDIM>::setAttrib(const TxHierAttribSetIntDbl& tas) {

  // Scoping call to base class
  PsBndryBase<FLOATTYPE, NDIM>::setAttrib(tas);
  this->dbprt("PsFixedWall::setAttrib() ");

  // Parse input parameters for STFunc's, one for now
  funcNames = tas.getNamesOfType("STFunc");
  if (funcNames.size() > 0) {
    funcAttrib = tas.getAttrib(funcNames[0]);
  }

  // Check for either STFunc or data-file
  if ( (funcNames.size() > 0) && tas.hasString("dataFileName")) {
    TxDebugExcept tde("PsFixedWall::setAttrib: error in setting data method");
    tde << " in <Boundary " << this->getName() << " > \n";
    tde << " Set only STFunc block OR an external dataFileName";
    throw tde;
  } // both
  if ( (funcNames.size() < 1) && !tas.hasString("dataFileName")) {
    TxDebugExcept tde("PsFixedWall::setAttrib: error in setting data method");
    tde << " in <Boundary " << this->getName() << " > \n";
    tde << " Neither STFunc block OR an external dataFileName are set. Pick one";
    throw tde;
  } // neither

  // Raster data file name (if it exists) and set method flag
  if (tas.hasString("dataFileName")) {
    dataFileName = tas.getString("dataFileName");
    std::cout << "dataFileName = " << dataFileName << std::endl;
    setWallMethod = FROM_FILE;
  }
  else
    setWallMethod = FROM_PYFUNC;

  // Wall edge threshold for dist calc
  if (tas.hasParam("wallEdgeThreshold")) {
    wallEdgeThreshold = tas.getParam("wallEdgeThreshold");
  }

  // Wall edge threshold for dist calc
  if (tas.hasString("thresholdFlag")) {
    thresholdFlag = tas.getString("thresholdFlag");
  }
  else {
    thresholdFlag = "old";
  }

}

template <class FLOATTYPE, size_t NDIM>
void PsFixedWall<FLOATTYPE, NDIM>::buildData() {

  // Scoping call to base class
  PsBndryBase<FLOATTYPE, NDIM>::buildData();
  this->dbprt("PsFixedWall::buildData() ");

  // Parse input parameters for STFunc's, one for now
  if (funcNames.size() > 0) {
    std::string kind = funcAttrib.getString("kind");
    wallSTFunc = TxMakerMap< PsSTFunc<FLOATTYPE, NDIM> >::getNew(kind);
  }

  // Check for PyFunc
  if (setWallMethod == FROM_PYFUNC) {
    if (!wallSTFunc) {
      TxDebugExcept tde("PsFixedWall::buildData: no STFunc object found");
      tde << " in <Boundary " << this->getName() << " >";
      throw tde;
    }
    else {
      this->dbprt("Found STFunc for PsFixedWall: setting the function...");
      wallSTFunc->setAttrib(funcAttrib);
    }
  }

  // Check if external datafile exists
  if (setWallMethod == FROM_FILE) {
    FILE* spFile = fopen(dataFileName.c_str(), "r");
    if (!spFile) {
      fclose(spFile);
      TxDebugExcept tde("PsFixedWall::buildData: wall field file ");
      tde << dataFileName << " not found";
      tde << " in <Boundary " << this->getName() << " >";
      throw tde;
    }
    else {
      fclose(spFile);
      std::string msg = "Found wall field file " + dataFileName;
      this->pprt(msg);
    }
  }

  // Have makermap return a pointer to a boundary data objects
  bndryPtr    = TxMakerMap< PsBndryDataBase<FLOATTYPE, NDIM> >::getNew("wallData");
  locBndryPtr = TxMakerMap< PsBndryDataBase<FLOATTYPE, NDIM> >::getNew("wallData");
}

//
// build solvers
//
template <class FLOATTYPE, size_t NDIM>
void PsFixedWall<FLOATTYPE, NDIM>::buildSolvers() {

  this->dbprt("PsFixedWall::buildSolvers() ");

  // Populate global wall field and set boundary data member
  if (setWallMethod == FROM_PYFUNC)
    setWallField();
  else {
    readWallFieldFile();
    setWallFieldFromFile();
  }

  // Setup ptclData interface
  bndryPtr->setDomainSingletons(this->getDomainSingletons() );
  bndryPtr->buildData();
  bndryPtr->buildSolvers();
  bndryPtr->setBndryData(wallData);

  // Setup ptclData interface for local data
  // to speed simulation
  locBndryPtr->setDomainSingletons(this->getDomainSingletons() );
  locBndryPtr->buildData();
  locBndryPtr->buildSolvers();
  locBndryPtr->setBndryData(localWallData);

  // Cast to wall data class for specific data
  // access and set wall values
  PsWallData<FLOATTYPE, NDIM>* wallBndryPtr =
     dynamic_cast< PsWallData<FLOATTYPE, NDIM>* >(bndryPtr);

  // Set elements of "inside" wall as defined above
  wallBndryPtr->setWallInside(wallInsidePts);

  // Scoping call to base class
  PsBndryBase<FLOATTYPE, NDIM>::buildSolvers();

  // Call static member to add to base static list for all boundaries
  // Global list is added here for checking distance with nanoparticles
  PsBndryBase<FLOATTYPE, NDIM>::addBndry(bndryPtr);
}

//
template <class FLOATTYPE, size_t NDIM>
void PsFixedWall<FLOATTYPE, NDIM>::initialize() {
}

//
// update
//
template <class FLOATTYPE, size_t NDIM>
void PsFixedWall<FLOATTYPE, NDIM>::update(double t) {

  this->dbprt("PsFixedWall::update(t): ");

  // Populate depField with wall values
  sendWallToDepField();

  // Scoping call to base class so depField sets phys fields
  PsBndryBase<FLOATTYPE, NDIM>::update(t);
}

//
// ***********************************************************************
//                        Private helper methods
// ***********************************************************************
//

//
// helper function to set member data describing
// global field data for a fixed wall from an external python module.
// This initializes the wallData member
//
template <class FLOATTYPE, size_t NDIM>
void PsFixedWall<FLOATTYPE, NDIM>::setWallField() {

  this->dbprt("PsFixedWall::setWallField");

  // Local coordinate variables
  FLOATTYPE time=1.0;
  FLOATTYPE val=0.0;

  // Global dimensions of wall field
  std::vector<size_t> globalWallDims = this->getGridBase().getNumCellsGlobal();

  // Load wall values
  for (size_t x=0; x<globalWallDims[0]; ++x) {
    for (size_t y=0; y<globalWallDims[1]; ++y) {
      for (size_t z=0; z<globalWallDims[2]; ++z) {

        // Calculates value at global position
        PsTinyVector<size_t, NDIM> posVec(x,y,z);
        PsTinyVector<int, NDIM> posIntVec(x,y,z);

        val = wallSTFunc->operator()(posVec, time);
        if ( (val > 1.0) || (val < 0.0) ) {
          TxDebugExcept tde("PsFixedWall::setWallField:");
          tde << "boundary value outside accepted bounds [0,1]";
          throw tde;
        }

        // Apply threshold boundary
        if (val > (1.0 - this->bndryFieldThreshold) ) {
          if (thresholdFlag == "new") val = 1.0 - this->bndryFieldThreshold;
          else val = 1.0;
        }

        // Set wall boundary data
        // If above threshold then record as inside wall edge
        if (val > this->bndryFieldThreshold) {

          wallData.addFieldData(posIntVec, val);
          bool ptLocal = this->getGridBase().getDecomp().hasPosition(posIntVec);

          if (ptLocal) {
            PsTinyVector<int, NDIM> locPosVec = this->getGridBase().mapToLocalVec(posIntVec);
            localWallData.addFieldData(locPosVec, val);
          }

          if ( (val >= wallEdgeThreshold) ) {
            wallInsidePts.push_back(posIntVec);
          }
        } // if any part of wall

      }}}

  // ****************************************************************************
  // Check
  if (wallData.getNumFieldElements() == 0) {
    TxDebugExcept tde("PsFixedWall::setWallField:: # field elements = 0");
    tde << " in <PsFixedWall " << this->getName() << " >";
    throw tde;
  }
  else {
    std::string msg = "\n Number of wall elements in " + this->getName() + " = ";
    this->pprt(msg, (int)wallData.getNumFieldElements() );
  }

  // Walls need "inside edges" so they can interact with other boundaries
  if (wallInsidePts.size() == 0) {
    TxDebugExcept tde("PsFixedWall::setWallField:: # wall inside elements = 0");
    tde << " in <PsFixedWall " << this->getName() << " >";
    throw tde;
  }
  // ****************************************************************************

  // Set debug flag
  wallData.setDebugFlag(this->dbStatus);
  localWallData.setDebugFlag(this->dbStatus);
}

//
// helper function to set member data describing
// global field data for a fixed wall from an external file.
// This initializes the wallData  member
//
template <class FLOATTYPE, size_t NDIM>
void PsFixedWall<FLOATTYPE, NDIM>::setWallFieldFromFile() {

  this->dbprt("PsFixedWall::setWallFieldFromFile");

  // Local coordinate variables
  FLOATTYPE time=1.0;
  FLOATTYPE val=0.0;

  // Global dimensions of wall field
  std::vector<size_t> globalWallDims = this->getGridBase().getNumCellsGlobal();

  // Load wall values
  for (size_t x=0; x<globalWallDims[0]; ++x) {
    for (size_t y=0; y<globalWallDims[1]; ++y) {
      for (size_t z=0; z<globalWallDims[2]; ++z) {

        // Calculates value at global position
        PsTinyVector<size_t, NDIM> posVec(x,y,z);
        PsTinyVector<int, NDIM> posIntVec(x,y,z);

        val = wallDataMap[posIntVec];
        // val = 1.4394;
        // val = wallSTFunc->operator()(posVec, time);

        if ( (val > 1.0) || (val < 0.0) ) {
          TxDebugExcept tde("PsFixedWall::setWallFieldFromFile:");
          tde << "boundary value outside accepted bounds [0,1]";
          throw tde;
        }

        // Apply threshold boundary
        if (val > (1.0 - this->bndryFieldThreshold) ) {
          if (thresholdFlag == "new") val = 1.0 - this->bndryFieldThreshold;
          else val = 1.0;
        }

        // Set wall boundary data
        // If above threshold then record as inside wall edge
        if (val > this->bndryFieldThreshold) {

          wallData.addFieldData(posIntVec, val);
          bool ptLocal = this->getGridBase().getDecomp().hasPosition(posIntVec);

          if (ptLocal) {
            PsTinyVector<int, NDIM> locPosVec = this->getGridBase().mapToLocalVec(posIntVec);
            localWallData.addFieldData(locPosVec, val);
          }

          if ( (val >= wallEdgeThreshold) ) {
            wallInsidePts.push_back(posIntVec);
          }
        } // if any part of wall

      }}}

  // ****************************************************************************
  // Check
  if (wallData.getNumFieldElements() == 0) {
    TxDebugExcept tde("PsFixedWall::setWallFieldFromFile:: # field elements = 0");
    tde << " in <PsFixedWall " << this->getName() << " >";
    throw tde;
  }
  else {
    std::string msg = "\n Number of wall elements in " + this->getName() + " = ";
    this->pprt(msg, (int)wallData.getNumFieldElements() );
  }

  // Walls need "inside edges" so they can interact with other boundaries
  if (wallInsidePts.size() == 0) {
    TxDebugExcept tde("PsFixedWall::setWallFieldFromFile:: # wall inside elements = 0");
    tde << " in <PsFixedWall " << this->getName() << " >";
    throw tde;
  }
  // ****************************************************************************

  // Set debug flag
  wallData.setDebugFlag(this->dbStatus);
  localWallData.setDebugFlag(this->dbStatus);
}

//
// Takes wall field info and deposits into local depField
// (now private after refactor in Hldr). Maps to local domain in
// bndryDepField
//
template <class FLOATTYPE, size_t NDIM>
void PsFixedWall<FLOATTYPE, NDIM>::sendWallToDepField() {

  this->dbprt("PsFixedWall::sendWallToDepField");

  // Resetting local wall dep field
  this->bndryDepField.reset(0.0);

  // Loop on field elements
  for (size_t ff=0; ff<locBndryPtr->getNumFieldElements(); ++ff) {

    PsTinyVector<int, NDIM> pos = locBndryPtr->getFieldPos(ff);
    FLOATTYPE val = locBndryPtr->getFieldVal(ff);
    FLOATTYPE val0 = this->bndryDepField(pos[0], pos[1], pos[2], 0);
    this->bndryDepField(pos[0], pos[1], pos[2], 0) = val0 + val;

  }

}

//
// Open, read and store raster file values from external file
// Reads specific file format as strings and converts
//
template <class FLOATTYPE, size_t NDIM>
void PsFixedWall<FLOATTYPE, NDIM>::readWallFieldFile() {

  this->dbprt("PsFixedWall::readWallFieldFile");

  size_t thisRank = this->getCommBase().getRank();
  size_t nproc    = this->getCommBase().getSize();
  std::string inName = dataFileName;

  //
  // Read in field data on each processor
  //
  std::string line;
  std::vector<int> rasterSizes;
  int rasterTotalSize;
  std::ifstream mainInFile(inName.c_str());
  std::vector<std::string> coordStr;

  for (size_t iproc=0; iproc<nproc; ++iproc) {

    // current proc reads data
    if (thisRank == iproc) {

      mainInFile.seekg(0);            // Reset file position

      // Process header
      std::getline(mainInFile, line);         // skip
      std::getline(mainInFile, line);         // skip
      std::getline(mainInFile, line);         // skip
      splitDelimitedStr(coordStr, line, ' '); // split coords line
      coordStr.pop_back();                    // remove junk last string
      std::getline(mainInFile, line);         // skip

      // Convert string input to int coord sizes (format specific)
      for (size_t n=0; n<coordStr.size(); ++n) {
        coordStr[n].erase(0,2);
        coordStr[n].pop_back();
        rasterSizes.push_back( (int) atof(coordStr[n].c_str()) );
        std::cout << "rasterSize[n= " << n << " ] " << rasterSizes[n] << std::endl;
      }
      rasterTotalSize = rasterSizes[0]*rasterSizes[1]*rasterSizes[2];
      this->pprt("rasterTotalSize = ", rasterTotalSize);

      // Check dimensions of simulation grid and raster file sizes
      std::vector<size_t> globalWallDims = this->getGridBase().getNumCellsGlobal();
      for (size_t n=0; n<globalWallDims.size()-1; ++n) {
        if (globalWallDims[n] != rasterSizes[n]) {
          TxDebugExcept tde("PsFixedWall::readWallFieldFile:");
          tde << "Data file field array sizes do not match those specified in <Grid>";
          tde << "for n " << n;
          tde << "globalWallDim = " << globalWallDims[n];
          tde << "rasterSize    = " << rasterSizes[n];
          throw tde;
        }
      }

      // If not 2D sim then z sizes must also match
      if ( (globalWallDims[2] != rasterSizes[2]) && (globalWallDims[2] != 1) ) {
        TxDebugExcept tde("PsFixedWall::readWallFieldFile:");
        tde << "Data file field array z-size do not match that specified in <Grid> for 3D example";
        tde << "globalWallDims[2] = " << globalWallDims[2];
        tde << "rasterSizes[2]    = " << rasterSizes[2];
        throw tde;
      }

      // Input wall data
      while( !mainInFile.eof() ) {

        std::getline(mainInFile, line);

        // Process string line
        std::vector<std::string> tokens;
        splitDelimitedStr(tokens, line, ' ');       // Split input line on spaces
        int ix     = (int) atof(tokens[0].c_str()); // x-coord
        int iy     = (int) atof(tokens[1].c_str()); // y-coord
        int iz     = (int) atof(tokens[2].c_str()); // z-coord
        double val =       atof(tokens[3].c_str()); // 4th entry is wall value

        // Build grid coord map
        PsTinyVector<int, NDIM> fileGridPos(ix,iy,iz);
        std::pair<PsTinyVector<int, NDIM>, double> mapElem =
          std::make_pair(fileGridPos, val);
        wallDataMap.insert(mapElem);
      }

      // Check data file size with data file grid size
      if (wallDataMap.size() != rasterTotalSize) {
        TxDebugExcept tde("PsFixedWall::readWallFieldFile: number of data lines ");
        tde << "does not equal header grid sizes";
        tde << " in <PsFixedWall " << this->getName() << " >";
        throw tde;
      }

    } // one proc
    this->getCommBase().barrier();
  }
  this->getCommBase().barrier();

}

// Instantiate
template class PsFixedWall<float, 1>;
template class PsFixedWall<float, 2>;
template class PsFixedWall<float, 3>;

template class PsFixedWall<double, 1>;
template class PsFixedWall<double, 2>;
template class PsFixedWall<double, 3>;
