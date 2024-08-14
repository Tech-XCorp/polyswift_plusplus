/**
 *
 * @file    PsNamedObject.h
 *
 * @brief   A subclass for objects which can be referred to by name,
 *          making them accessible from an input file.
 *
 * @version $Id: PsNamedObject.h 2137 2020-04-16 20:37:29Z alexanda $
 *
 * Copyright &copy; 2007-2020, Tech-X Corporation
 */

#ifndef PS_NAMED_OBJECT_H
#define PS_NAMED_OBJECT_H

// configure stuff
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// Standard library includes
#include <string>
#include <map>

// txbase includes
#include <TxDebugExcept.h>

/**
 * A class for objects that can be referred to by name, making them
 * accessible from the input file.  Objects which are created directly
 * from input file settings should derive from this class.
 */
class PsNamedObject {

  public:


/**
 * Default constructor: Does nothing.  Most derived classes will
 * use this one.
 */
    PsNamedObject() :
            name(""),
            owner(0) { }

/**
 * Constructor with name
 */
    PsNamedObject(const std::string& nm) :
            name(nm),
            owner(0) { }

/**
 * Destructor
 */
    virtual ~PsNamedObject() { }

/**
 * Accessor for the name
 */
    virtual std::string getName() const { return name; }

/**
 * @return the fully qualified name of this object
 */
    virtual std::string getFullName() const;

/**
 * Accessor for the owner
 */
    virtual const PsNamedObject* getOwner() const { return owner; }

/**
 * Accessor for the owner, needed for correctly setting
 * sub-objects
 */
    virtual PsNamedObject* getNonConstOwner() {
      PsNamedObject* obj = const_cast<PsNamedObject*> (owner);
      return obj;
    }
    // SWS: reorg this.. so not needed?

/**
 * Modifier for the name
 */
    virtual void setName(const std::string& nm) { name = nm; }

/**
 * Modifier for the owner
 */
    virtual void setOwner(const PsNamedObject* obj) { owner = obj; }

/**
 * Place the given object into this object's directory, i.e. the given
 * object becomes a child of this one.  The child object's set name is
 * used.
 */
    virtual void makeAvail(PsNamedObject* obj);

/**
 * Put the given object into this object's directory with the given
 * name.  This does not set the given object's namem since the policy
 * is that the object can be in the directory with multiple names.
 */
    virtual void makeAvail(PsNamedObject* obj, const std::string& nm);

/**
 * Find out whether this object's directory or any of its ancestors'
 * directories contain an object of the given name
 */
    virtual bool hasObject(const std::string& nm) const;

/**
 * Return an object of the given type.  If there's a type mismatch or
 * the object isn't present in this or an ancestor's directory, throw
 * an exception.  Define here so that it can be explicitly
 * instantiated in any layer.
 */
    template <class T> T* getObject(const std::string& nm) const {
      PsNamedObject* obj = findObject(nm);
      if (obj) {
        T* result = dynamic_cast<T*>(obj);
        if (result) {
          return result;
        }
        else {
          TxDebugExcept tde;
          tde << "Object " << getName()
              << ": Encountered type mismatch in finding object "
              << nm << ".";
          throw tde;
        }
      }
      else {
        TxDebugExcept tde;
        tde << "Object " << getName()
            << ": Could not locate object "
            << nm << " in the input file object hierarchy.";
        throw tde;
      }
    }


  protected:

/**
 * Find an object with the given name in the local directory.
 */
    virtual PsNamedObject* findLocalObject(const std::string& nm) const;

/**
 * Find an object in this or an ancestor's directory and return an
 * iterator.  Returns 0 if an object is not found.
 */
    virtual PsNamedObject* findObject(const std::string& nm) const;


  private:

/**
 * The name of the object, given in the input file
 */
    std::string name;

/**
 * The owner object.  Top level objects are children of PsDomain.
 */
    const PsNamedObject* owner;

/**
 * The local object directory.  This directory contains the object's
 * children,
 */
    std::map<std::string, PsNamedObject*> objectDirectory;


/**
 * A PsNamedObject should not be copied, since names should be unique.
 */
    PsNamedObject(const PsNamedObject&);

/**
 * A PsNamedObject should not be assigned, since names must be unique.
 */
    PsNamedObject& operator=(const PsNamedObject&);

};

#endif // PS_NAMED_OBJECT_H
