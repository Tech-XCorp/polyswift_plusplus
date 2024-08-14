/**
 *
 * @file    PsNamedObject.cpp
 *
 * @brief   A subclass for objects which can be referred to by name,
 *          making them accessible from an input file.  This file is
 *          only necessary to define the static object directory map.
 *
 * @version $Id: PsNamedObject.cpp 2137 2020-04-16 20:37:29Z alexanda $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation
 */

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// Standard library includes
#include <string>
#include <map>
#include <sstream>

// This class
#include<PsNamedObject.h>

std::string PsNamedObject::getFullName() const {
  if (this->getOwner()) {
    std::string fullName = this->getOwner()->getFullName();
    fullName += '.';
    fullName += this->getName();
    return fullName;
  }
  else {
    return this->getName();
  }
}

//
// Place the given object into this object's directory with the given
// object's set name.
//
void PsNamedObject::makeAvail(PsNamedObject* obj) {
  makeAvail(obj, obj->getName());
}

//
// Place the given object into this object's directory with the given name.
//
void PsNamedObject::makeAvail(PsNamedObject* obj, const std::string& nm) {
  std::pair<std::map<std::string, PsNamedObject*>::iterator, bool> res
    = objectDirectory.insert(make_pair(nm, obj));
  if (!res.second) {
    TxDebugExcept tde;
    tde << nm << ": already exists";
    throw tde;
  }
}

//
// Determine if there is an object in the hierarchy with the given
// name
//
bool PsNamedObject::hasObject(const std::string& nm) const {
  return findObject(nm);
}

//
// Find an object in this directory
//
PsNamedObject* PsNamedObject::findLocalObject(const std::string& nm) const {
  std::map<std::string, PsNamedObject*>::const_iterator iter
    = objectDirectory.find(nm);
  if (iter != objectDirectory.end()) {
    return iter->second;
  }
  else {
    return 0;
  }
}

//
// Find an object in the hierarchy
//
PsNamedObject* PsNamedObject::findObject(const std::string& path) const {
// Create the stream for the path separated by '.'; read the top
// directory name
  std::istringstream pathStrm(path);
  std::string nm;
  std::getline(pathStrm, nm, '.');
// Find the top directory
  PsNamedObject* result = 0;
  for (const PsNamedObject* searchDir = this; searchDir && !result;
       searchDir = searchDir->getOwner()) {
    result = searchDir->findLocalObject(nm);
  }
  if (!result) return result;
// Find the requested object
  while (std::getline(pathStrm, nm, '.')) {
    result = result->findLocalObject(nm);
    if (!result) return result;
  }
  return result;
}
