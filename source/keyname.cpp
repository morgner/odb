/***************************************************************************
                          keyname.cpp  -  description
                             -------------------
    begin                : Thu Mar 23 2000
    copyright            : (C) 2000..2014 by Manfred Morgner
    email                : manfred@morgner.com
 ***************************************************************************/


#include "generic.h"
#include "keyname.h"

using namespace odb;

CKeyName::CKeyName(const std::string& sName)
  : m_sName(sName)
  {
  } // CKeyName::CKeyName(const string& sName)

CKeyName::~CKeyName()
  {
  } // CKeyName::~CKeyName()

CVectorRoot::iterator CKeyName::find(CRoot* poItem)
  {
  for ( iterator it = begin(); it != end(); ++it )
    {
    if ( *it == poItem )
      {
      return it;
      }
    }
  return end();
  } // CKeyName::iterator CKeyName::find(CRoot* poItem)
