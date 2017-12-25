#ifndef __KEY_NAME_H
#define __KEY_NAME_H

/***************************************************************************
                          keyname.h
                          -------------------
    begin                : Thu Mar 23 2000
    copyright            : (C) 2000..2011 by Manfred Morgner
    email                : manfred@morgner.com
 ***************************************************************************/


/**
 * @file
 * KeyName.
 */

#include "generic.h"

// STL
#include <vector>

namespace odb {

class CRoot;

/**
 * A simple vector of 'things'
 * to collect things, extracted from the odb.
 */
typedef std::vector<CRoot*> CVectorRoot;

/**
 * The KeyName Class.
 * A class, that provides an 2D-Index from "Names" of whatever to instances of whatever.
 */
class CKeyName : public CVectorRoot
  {
  private:
    typedef CVectorRoot inherited;

  protected:
    /**
     * Constructor.
     */
     CKeyName() {}
  public:

     /**
      * Constructor.
      * @param sName the name.
      */
     CKeyName(const std::string& sName);

     /**
      * Destructor.
      */
    ~CKeyName();

    CVectorRoot::iterator find(CRoot* poItem);

    operator const std::string& () const { return m_sName; }

  protected:
    std::string m_sName;
  }; // class CKeyName

struct KeyName_less
  {
  bool operator()(const CKeyName* poKey1, const CKeyName* poKey2) const
    {
    return ((std::string)*poKey1) < ((std::string)*poKey2);
    }
  }; // struct KeyName_less

typedef std::set<CKeyName*, KeyName_less> CKeyNameIndex;

  } // namespace odb
 // __KEY_NAME_H
#endif
