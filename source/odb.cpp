/***************************************************************************
                          odb.cpp  -  description
                             -------------------
    begin                : Fri Mar 3 2000
    copyright            : (C) 1993..2014 by Manfred Morgner
    email                : manfred@morgner.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *                                                                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place Suite 330,                                            *
 *   Boston, MA  02111-1307, USA.                                          *
 *                                                                         *
 ***************************************************************************/

#include "generic.h"

#include "odb.h"
#include "stream.h"
#include "transaction.h"

using namespace odb;

/**
 * The comparsion operator for 'timeval' values.
 * used for lists sorted by the modification time of 'things'. This may be
 * necessary in the case someone needs to know what things are changed
 * after a specific moment. Typically used for synchronisation.
 */
bool operator > (const timeval& t1, const timeval& t2)
  {
  return ( (t1.tv_sec > t2.tv_sec) || ( (t1.tv_sec == t2.tv_sec) && (t1.tv_usec > t2.tv_usec) ) );
  } // operator > (timeval& t1, timeval& t2)

//------------------------------------------------------------------//
// Construction/Destruction
//------------------------------------------------------------------//

/**
 * The odb constructor.
 * initializes all base values for a odb instance. The maximum values are used
 * to give 'things' a identification.
 * @param sFileName The filename the odb is stored to and/or read from.
 */
CODB::CODB(const std::string& sFileName)
  {
  m_sFileName = sFileName;
  m_lMaxClass  = -1;
  m_lMaxAtom   = -1;
  m_lMaxObject = -1;
  m_lMaxReason = -1;
  } // CODB::CODB(const std::string& sFileName)

/**
 * The odb destructor.
 */
CODB::~CODB()
  {
  } // CODB::~CODB()

/**
 * Dump.
 * Dumps the whole database to std::cout and concludes the output with a
 * simple overview of how many of which objects are exsiting in the db.
 */
void CODB::Dump()
  {
  for (auto const &it : *this)
    {
    if ( it->InfoGetRtti() == _Object )
      {
      ((CObject*)it)->Dump();
      } // if ( it->InfoGetRtti() == _Object )
    } // for (auto const &it : *this)


  std::cout << "SUMMARY" << std::endl;
  std::cout << "~~~~~~~" << std::endl;

  std::cout << "entries:" << size() << std::endl;
  std::cout << "Classes:" << m_moId2PtrClass.size()  << std::endl;
  std::cout << "Objects:" << m_moId2PtrObject.size() << std::endl;
  std::cout << "Atoms  :" << m_moId2PtrAtom.size()   << std::endl;
  std::cout << "Reason :" << m_moId2PtrReason.size() << std::endl;
  std::cout << "Ropes  :" << m_moId2PtrRope.size()   << std::endl;

  } // void CODB::Dump()

bool CODB::Save(const std::string& sFileName)
  {
  if ( sFileName.length() > 0 )
    {
    m_sFileName = sFileName;
    }
  if ( m_sFileName.length() == 0 )
    {
    return false;
    }

  try
    {
    CStream oStream(m_sFileName, true);

    oStream << *(long*)"ODBF";
    oStream << m_lMaxClass;
    oStream << m_lMaxAtom;
    oStream << m_lMaxObject;
    oStream << m_lMaxReason;
    oStream << (long)size();

    for (auto const &itElement : *this)
      {
      oStream << (long)itElement->InfoGetRtti();
      oStream << (long)itElement->InfoGetRelease();
      oStream << (long)itElement->ID();
      oStream << *itElement;
      } // for (auto const &itElement : *this)
    }
  catch (...)
    {
    return false;
    }

  return true;
  } // bool CODB::Save(const std::string& sFileName)

bool CODB::SaveXML(const std::string& sFileName)
  {
  if ( sFileName.length() > 0 )
    {
    m_sFileName = sFileName;
    }
  if ( m_sFileName.length() == 0 )
    {
    return false;
    }

  try
    {
    CStream    oStream(m_sFileName, true);
//    CStreamXML oStream(m_sFileName, true);
    
    oStream.Element("type", "ODBF"); 
    oStream.Open("odb");
      oStream.Open("maximum");
        oStream.Element("class",   m_lMaxClass);
        oStream.Element("atom",    m_lMaxAtom);
        oStream.Element("object",  m_lMaxObject);
        oStream.Element("reason",  m_lMaxReason);
        oStream.Close();
      oStream.Open("count");
        oStream.Element("things", (long)size());
        oStream.Close();

    for (auto const &itElement : *this)
      {
      itElement->Save(oStream);
      } // for (auto const &itElement : *this)
    oStream.Close();
    }
  catch (...)
    {
    return false;
    }

  return true;
  } // bool CODB::SaveXML(const std::string& sFileName)

bool CODB::Load(const std::string& sFileName)
  {
  if ( sFileName.length() > 0 )
    {
    m_sFileName = sFileName;
    }
  if ( m_sFileName.length() == 0 )
    {
    return false;
    }

  // this is for Id2Ptr-Resolution if Id is -1 
  // -> not present during saving while loading
  // -1 in the file means "NULL-Pointer"
  m_moId2PtrClass [-1] = 0; // (CClass* )NULL
  m_moId2PtrAtom  [-1] = 0; // (CAtom*  )NULL
  m_moId2PtrObject[-1] = 0; // (CObject*)NULL
  m_moId2PtrReason[-1] = 0; // (CReason*)NULL
  m_moId2PtrRope  [-1] = 0; // (CRope*  )NULL
  try
    {
    CStream oStream(m_sFileName);
    long       lRTTI;
    long       lSign;
    long       lCount;
    CRoot*  poEntry = 0;

    oStream >> lSign;
    oStream >> m_lMaxClass;
    oStream >> m_lMaxAtom;
    oStream >> m_lMaxObject;
    oStream >> m_lMaxReason;
    oStream >> lCount;

    for (long l = 0; l < lCount; l++)
      {
      long lRelease;
      long lid;

/*
<!--
 -->
 
<odb>
  <defaults release="1">
  <class id="7" name="Person" format="%s" class="6" omal="10"/>
  <object id="74" name="me" format="%s" class="21" omal="11">
    <object id="34">
    <atom id="123"/>
    <atom id="124"/>
    </object>
  <atom id="123" ../>
  <atom id="124" ../>
  </odb>
*/

      oStream >> lRTTI;
      oStream >> lRelease;
      oStream >> lid;

      switch (lRTTI)
        {
        case _Root:
          throw -1; // kill me -> if we are here, anything is wrong
          break;

        case _Class:
          poEntry = Add(new CClass(lid));
          break;
        case _Object:
          poEntry = Add(new CObject(lid));
          break;
        case _Atom:
          poEntry = Add(new CAtom(lid));
          break;
        case _Reason:
          poEntry = Add(new CReason(lid));
          break;
        case _Rope:
          poEntry = Add(new CRope(lid));
          break;
        } // switch (lRTTI)

      if (poEntry)
        {
        oStream >> *poEntry;
        }
      else
        {
        break;
        }
      poEntry = 0;
      } // for (;;)
    }
  catch (...)
    {
    return false;
    }

  // resolve id's to pointers in link tables
  long xxxx = size();
  for (auto const &itElement : *this)
    {
    xxxx--;
    itElement->ResolveIDs(*this);
    } // for (auto const &itElement : *this)

  return true;
  } // bool CODB::Save(const std::string& sFileName)

//////////////////////////////////////////////////////////////////////////////
// Add methodes
//////////////////////////////////////////////////////////////////////////////

CClass* CODB::Add(CClass* poSrc)
  {
  if (poSrc == 0)
    {
    return NULL;
    }
  if (poSrc->IsValid() == false)
    {
    // try to make it valid

    poSrc->Validate(*this);
    // test ob this was successfull
    if (poSrc->IsValid() == false)
      {
      return NULL;
      }
    } // if (poSrc->IsValid() == false)

  if (m_moId2PtrClass.find(poSrc->ID()) == m_moId2PtrClass.end())
    {
    m_moId2PtrClass[poSrc->ID()] = poSrc;
    insert(poSrc);
    KeyNameIndexItemAdd(m_soIndexNameClass, poSrc);
    }
  return poSrc;
  } // CClass* CODB::Add(CClass* poSrc)

CAtom* CODB::Add(CAtom* poSrc)
  {
  if (poSrc == 0)
    {
    return NULL;
    }
  if (poSrc->IsValid() == false)
    {
    poSrc->Validate(*this);
    }
  if (poSrc->IsValid() == false)
    {
    return NULL;
    }
  if (m_moId2PtrAtom.find(poSrc->ID()) == m_moId2PtrAtom.end())
    {
    m_moId2PtrAtom[poSrc->ID()] = poSrc;
    insert(poSrc);
    KeyNameIndexItemAdd(m_soIndexNameAtom, poSrc);
    }
  return poSrc;
  }

CObject* CODB::Add(CObject* poSrc)
  {
  if (poSrc == 0)
    {
    return NULL;
    }
  if (poSrc->IsValid() == false)
    {
    poSrc->Validate(*this);
    }
  if (poSrc->IsValid() == false)
    {
    return NULL;
    }
  if (m_moId2PtrObject.find(poSrc->ID()) == m_moId2PtrObject.end())
    {
    m_moId2PtrObject[poSrc->ID()] = poSrc;
    insert(poSrc);
    KeyNameIndexItemAdd(m_soIndexNameObject, poSrc);
    }
  return poSrc;
  }

CReason* CODB::Add(CReason* poSrc)
  {
  if (poSrc == 0)
    {
    return NULL;
    }
  if (poSrc->IsValid() == false)
    {
    poSrc->Validate(*this);
    }
  if (poSrc->IsValid() == false)
    {
    return NULL;
    }
  if (m_moId2PtrReason.find(poSrc->ID()) == m_moId2PtrReason.end())
    {
    m_moId2PtrReason[poSrc->ID()] = poSrc;
    insert(poSrc);
    KeyNameIndexItemAdd(m_soIndexNameReason, poSrc);
    }
  return poSrc;
  }

CRope* CODB::Add(CRope* poSrc)
  {
  if (poSrc == 0)
    {
    return NULL;
    }
  if (poSrc->IsValid() == false)
    {
    // try to make it valid

    poSrc->Validate(*this);
    // test ob this was successfull
    if (poSrc->IsValid() == false)
      {
      return NULL;
      }
    } // if (poSrc->IsValid() == false)
  if (m_moId2PtrRope.find(poSrc->ID()) == m_moId2PtrRope.end())
    {
    m_moId2PtrRope[poSrc->ID()] = poSrc;
    insert(poSrc);
    KeyNameIndexItemAdd(m_soIndexNameRope, poSrc);
    }
  return poSrc;
  }

long CODB::ChangeName(CClass* poSrc, const std::string& sNewName)
  {
  return KeyNameIndexItemChangeName(m_soIndexNameClass, poSrc, sNewName);
  } //

long CODB::ChangeName(CAtom* poSrc, const std::string& sNewName)
  {
  return KeyNameIndexItemChangeName(m_soIndexNameAtom, poSrc, sNewName);
  } //

long CODB::ChangeName(CObject* poSrc, const std::string& sNewName)
  {
  return KeyNameIndexItemChangeName(m_soIndexNameObject, poSrc, sNewName);
  } //

long CODB::ChangeName(CReason* poSrc, const std::string& sNewName)
  {
  return KeyNameIndexItemChangeName(m_soIndexNameReason, poSrc, sNewName);
  } //

long CODB::ChangeName(CRope* poSrc, const std::string& sNewName)
  {
  return KeyNameIndexItemChangeName(m_soIndexNameRope, poSrc, sNewName);
  } //

//////////////////////////////////////////////////////////////////////////////
// Search Index Managing Methods
//////////////////////////////////////////////////////////////////////////////

long CODB::KeyNameIndexItemAdd(CKeyNameIndex& rsoKeyNameIndex, CRoot* poItem)
  {
  CKeyName oKeyName( poItem->NameGet() );
  auto it = rsoKeyNameIndex.find( &oKeyName );
  if ( it == rsoKeyNameIndex.end() )
    {
    CKeyName* poKeyName = new CKeyName( oKeyName );
    std::pair<CKeyNameIndex::iterator, bool> oResult = rsoKeyNameIndex.insert( poKeyName );
    it = oResult.first;
    if ( oResult.second == false )
      {
      delete poKeyName;
      }
    } // if ( it == rsoKeyNameIndex.end() )

  CKeyName::iterator itItem = (*it)->find(poItem);
  if ( itItem == (*it)->end() )
    {
    (*it)->push_back(poItem);
    } // if ( itItem == it->end() )

  return ODB_SUCCESS;
  } // long CODB::KeyNameIndexItemAdd(CKeyNameIndex& rsoKeyNameIndex, CRoot* poItem)

long CODB::KeyNameIndexItemRemove(CKeyNameIndex& rsoKeyNameIndex, CRoot* poItem)
  {
  CKeyName oKeyName( poItem->NameGet() );
  CKeyNameIndex::iterator it = rsoKeyNameIndex.find( &oKeyName );
  if ( it == rsoKeyNameIndex.end() )
    {
    return ODB_SUCCESS;
    } // if ( it == rsoKeyNameIndex.end() )

  CKeyName::iterator itItem = (*it)->find(poItem);
  if ( itItem != (*it)->end() )
    {
    (*it)->erase(itItem);
    if ( (*it)->size() == 0 )
      {
      rsoKeyNameIndex.erase(it);
      }
    } // if ( itItem == it->end() )

  return ODB_SUCCESS;
  } // long CODB::KeyNameIndexItemAdd(CKeyNameIndex& rsoKeyNameIndex, CRoot* poItem)

long CODB::KeyNameIndexItemChangeName(CKeyNameIndex& rsoKeyNameIndex, CRoot* poItem, const std::string& sNewName)
  {
  if ( find(poItem) == end() )
    {
    return ODB_UNKNOWN_INSTANCE;
    }
  KeyNameIndexItemRemove(rsoKeyNameIndex, poItem);
  poItem->m_sName = sNewName;
  KeyNameIndexItemAdd   (rsoKeyNameIndex, poItem);
  return ODB_SUCCESS;
  } // long CODB::KeyNameIndexItemChangeName(CKeyNameIndex& rsoKeyNameIndex, CRoot* poItem, const std::string& sNewName)

//////////////////////////////////////////////////////////////////////////////
// Mapping Instance-ID to Instance-Pointer
//////////////////////////////////////////////////////////////////////////////

CClass* CODB::Id2PtrClass(long lid) const
  {
	CMapId2PtrClass::const_iterator it = m_moId2PtrClass.find(lid);
  if (it == m_moId2PtrClass.end())
    {
		return 0;
    }
  return it->second;
  } // CClass* CODB::Id2PtrClass(long lid) const

CAtom* CODB::Id2PtrAtom(long lid) const
  {
	CMapId2PtrAtom::const_iterator it = m_moId2PtrAtom.find(lid);
  if (it == m_moId2PtrAtom.end())
    {
		return 0;
    }
  return it->second;
  } // CAtom* CODB::Id2PtrAtom(long lid) const

CObject* CODB::Id2PtrObject(long lid) const
  {
	CMapId2PtrObject::const_iterator it = m_moId2PtrObject.find(lid);
  if (it == m_moId2PtrObject.end())
    {
		return 0;
    }
  return it->second;
  } // CObject* CODB::Id2PtrObject(long lid) const

CReason* CODB::Id2PtrReason(long lid) const
  {
	CMapId2PtrReason::const_iterator it = m_moId2PtrReason.find(lid);
  if (it == m_moId2PtrReason.end())
    {
		return 0;
    }
  return it->second;
  } // CReason* CODB::Id2PtrReason(long lid) const

CRope* CODB::Id2PtrRope(long lid) const
  {
	CMapId2PtrRope::const_iterator it = m_moId2PtrRope.find(lid);
  if (it == m_moId2PtrRope.end())
    {
		return 0;
    }
  return it->second;
  } // CRope* CODB::Id2PtrRope(long lid) const

//////////////////////////////////////////////////////////////////////////////
// Mapping Instance-ID to Instance-Pointer
//////////////////////////////////////////////////////////////////////////////

/*
   this methods uarte use while loading a DB from file
   it is IMPOSSIBLE, that any "pointer" is unable to resolve
   if this appears, the DB is inconsistent (e.g. file read error)
   and the loading has to be aborted!
*/

CClass* CODB::Id2Ptr( const CClass* poSrc ) const
  {
  CClass* poClass = Id2PtrClass( (const long)poSrc );
  if ( poClass == 0 )
    {
//		throw -1; // kill me - I'm mad
    }
  return poClass;
  } // template <class T> T* CODB::Is2Ptr( T & oSrc )

CAtom* CODB::Id2Ptr( const CAtom* poSrc ) const

  {
  CAtom* poAtom = Id2PtrAtom( (const long)poSrc );
  if ( poAtom == 0 )
    {
//		throw -1; // kill me - I'm mad
    }
  return poAtom;
  } // template <class T> T* CODB::Is2Ptr( T & oSrc )

CObject* CODB::Id2Ptr( const CObject* poSrc ) const
  {
  CObject* poObject = Id2PtrObject( (const long)poSrc );
  if ( poObject == 0 )
    {
//		throw -1; // kill me - I'm mad
    }
  return poObject;
  } // template <class T> T* CODB::Is2Ptr( T & oSrc )

CReason* CODB::Id2Ptr( const CReason* poSrc ) const
  {
  CReason* poReason = Id2PtrReason( (const long)poSrc );
  if ( poReason == 0 )
    {
//		throw -1; // kill me - I'm mad
    }
  return poReason;
  } // template <class T> T* CODB::Is2Ptr( T & oSrc )

CRope* CODB::Id2Ptr( const CRope* poSrc ) const
  {
  CRope* poRope = Id2PtrRope( (const long)poSrc );
  if ( poRope == 0 )
    {
//		throw -1; // kill me - I'm mad
    }
  return poRope;
  } // template <class T> T* CODB::Is2Ptr( T & oSrc )

//////////////////////////////////////////////////////////////////////////////
// Validate instances
//////////////////////////////////////////////////////////////////////////////

long CODB::IdValidate(const CRoot* poInstance)
  {
  switch ( poInstance->InfoGetRtti() )
    {
    case _Class:  return ++m_lMaxClass;
    case _Atom:   return ++m_lMaxAtom;
    case _Object: return ++m_lMaxObject;
    case _Reason: return ++m_lMaxReason;
    case _Rope:   return ++m_lMaxRope;

    default:
      return -1;
    } // switch ( oInstance.InfoGetType() )
  } // long CODB::GetId(CClass& poClass)

//////////////////////////////////////////////////////////////////////////////
// GetMaps methode
//////////////////////////////////////////////////////////////////////////////

/*
#define FILL_LIST(c)                                    \
  bool CODB::FillList##c(CList##c & oList)        \
    {                                                   \
    oList.RemoveAll();                                  \
    POSITION pos = m_mo##c.GetStartPosition();          \
    while ( pos != NULL )                               \
      {                                                 \
      long      lKey;                                   \
      CODB##c * poInstance;                             \
      m_mo##c.GetNextAssoc( pos, lKey, poInstance );    \
      if ( poInstance != 0 )                            \
        oList.AddTail((CODB##c *)poInstance);           \
      }                                                 \
    return true;                                        \
    } // bool CODB::FillList##c(CList##c & oList)

FILL_LIST(Class);
FILL_LIST(Atom);
FILL_LIST(Object);
FILL_LIST(Reason);

#undef FILL_LIST
*/

//////////////////////////////////////////////////////////////////////////////
// EXTERNAL INTERFACE USED FOR CLIENT/SERVER CONNECTIONS
//////////////////////////////////////////////////////////////////////////////

/*********/
/* N E W */
/*********/

CClass* CODB::NewClass( const std::string& sName )
  {
  return Add( new CClass(sName) );
  }

CAtom* CODB::NewAtom( const std::string& sName )
  {
  return Add( new CAtom(sName) );
  }

CObject* CODB::NewObject( const std::string& sName, const CClass* poClass )
  {
  return Add( new CObject(sName, poClass) );
  }

CReason* CODB::NewReason( const std::string& sName, const CClass* poClassParent, const CClass* poClassChild )
  {
  return Add( new CReason(sName, poClassParent, poClassChild) );
  }

CRope* CODB::NewRope( const std::string& sName, CAtom* poStartpoint )
  {
  return Add( new CRope(poStartpoint, sName) );
  }

/*********/
/* G E T */
/*********/
CVectorRoot CODB::GetClass(const std::string& sName)
  {
  #define INDEX m_soIndexNameClass
  CKeyName oKeyName(sName);
  CKeyNameIndex::iterator it = INDEX.find( &oKeyName );

  if ( it == INDEX.end() )
    {
    return  CVectorRoot();
    } // if ( it != INDEX.end() )

  return **it;
  #undef INDEX
  } // long CODB::GetClass(const std::string& sName)

CVectorRoot CODB::GetAtom(const std::string& sName)
  {
  #define INDEX m_soIndexNameAtom
  CKeyName oKeyName(sName);
  CKeyNameIndex::iterator it = INDEX.find( &oKeyName );

  if ( it == INDEX.end() )
    {
    return  CVectorRoot();
    } // if ( it != INDEX.end() )

  return **it;
  #undef INDEX
  } // long CODB::GetAtom (const std::string& sName)


CVectorRoot CODB::GetObject(const std::string& sName)
  {
  #define INDEX m_soIndexNameObject
  CKeyName oKeyName(sName);
  CKeyNameIndex::iterator it = INDEX.find( &oKeyName );

  if ( it == INDEX.end() )
    {
    return  CVectorRoot();
    } // if ( it != INDEX.end() )

  return **it;
  #undef INDEX
  } // long CODB::GetObject(const std::string& sName)

CVectorRoot CODB::GetReason(const std::string& sName)
  {
  #define INDEX m_soIndexNameReason
  CKeyName oKeyName(sName);
  CKeyNameIndex::iterator it = INDEX.find( &oKeyName );

  if ( it == INDEX.end() )
    {
    return  CVectorRoot();
    } // if ( it != INDEX.end() )

  return **it;
  #undef INDEX
  } // long CODB::GetReason(string& sName)

CVectorRoot CODB::GetRope(const std::string& sName)
  {
  #define INDEX m_soIndexNameRope
  CKeyName oKeyName(sName);
  CKeyNameIndex::iterator it = INDEX.find( &oKeyName );

  if ( it == INDEX.end() )
    {
    return  CVectorRoot();
    } // if ( it != INDEX.end() )

  return **it;
  #undef INDEX
  } // long CODB::GetReason(string& sName)

/*************************/
/* T R A N S A C T I O N */
/*************************/
long CODB::TransactionOpen(const CRoot* poCaller)
  {
  return ODB_FAILURE;
  } // long CODB::TransactionOpen(CRoot* poCaller)

long CODB::TransactionCommit(const CRoot* poCaller)
  {
  return ODB_FAILURE;
  } // long CODB::TransactionCommit(CRoot* poCaller)

long CODB::TransactionCommitAll(const CRoot* poCaller)
  {
  return ODB_FAILURE;
  } // long CODB::TransactionCommitAll(CRoot* poCaller)

long CODB::TransactionAbort(const CRoot* poCaller)
  {
  return ODB_FAILURE;
  } // long CODB::TransactionAbort(CRoot* poCaller)

/*************/
/* C L A S S */
/*************/
long CODB::ClassSetOmal(CClass*  poClass,
                              CObject* poObject,
                              bool        bOverwrite)
  {
  // test, if the poClass argument is valid
  const_iterator it = find(poClass);
  if (it == end() )
    {
    Add(poClass);
    }
  // test, if the poObject argument is valid
  it = find( const_cast<CObject*>(poObject) );
  if (it == end() )
    {
    Add(poObject);
    }
  // if the class alread has an OMAL, is could only be over-
  // written, if bOverwrite is "true"
  if ( (poClass->OmalGet() != 0) && (bOverwrite == false) )
    {
    return ODB_FAILURE;
    }
  poClass->OmalSet(poObject);
  return ODB_SUCCESS;
  } // long CODB::ClassSetOmal(CClass*  poClass,..

CObject* CODB::ClassGetOmal(const CClass* poClass) const
  {
  // test, if the poClass argument is valid
  const_iterator it = find( const_cast<CClass*>(poClass) );
  if (it == end() )
    {
    return NULL;
    }
  return poClass->OmalGet();
  } // CObject* CODB::ClassGetOmal(CClass* poClass)

/***********/
/* A T O M */
/***********/
long CODB::AtomFormatSet(      CAtom* poAtom,
                               const std::string&   sFormat,
                               const std::string&   sPrefix,
                               const std::string&   sSuffix)
  {
  // test, if the poAtom argument is valid
  const_iterator it = find(poAtom);
  if (it == end() )
    {
    Add(poAtom);
    }
  poAtom->FormatSet(sFormat);
  if (sPrefix.length() > 0)
    {
    poAtom->PrefixSet(sPrefix);
    }
  if (sSuffix.length() > 0)
    {
    poAtom->SuffixSet(sSuffix);
    }
  return ODB_SUCCESS;
  } // long CODB::AtomFormatSet(CAtom* poAtom,.)

long CODB::AtomPrefixSet(       CAtom* poAtom,
                               const std::string&   sPrefix )
  {
  // test, if the poAtom argument is valid
  const_iterator it = find(poAtom);
  if (it == end() )
    {
    Add(poAtom);
    }
  poAtom->PrefixSet(sPrefix);
  return ODB_SUCCESS;
  } // long CODB::AtomPrefixSet(CAtom* poAtom,.)

long CODB::AtomSuffixSet(       CAtom* poAtom,
                               const std::string&   sSuffix )
  {
  // test, if the poAtom argument is valid
  const_iterator it = find(poAtom);
  if (it == end() )
    {
    Add(poAtom);
    }
  poAtom->SuffixSet(sSuffix);
  return ODB_SUCCESS;
  } // long CODB::AtomSuffixSet(CAtom* poAtom,.)

std::string CODB::AtomFormatGet(const CAtom* poAtom) const
  {
  // test, if the poAtom argument is valid
  const_iterator it = find( const_cast<CAtom*>(poAtom) );
  if ( it == end() )
    {
    return 0;
    }
  return poAtom->FormatGet();
  } // string CODB::AtomGetFormat(const CObject* poAtom)

CRope* CODB::AtomRopeNew( CAtom* poAtom )
  {
  return Add( new CRope(poAtom) );
  } // CRope* CODB::AtomRopeNew( CAtom* poAtom )

long CODB::AtomRopeAppend( CAtom* poAtom, const CRope* poRope, CAtom* poAtomNext )
  {
  return poAtom->RopeLinkAdd( const_cast<CRope*>(poRope), poAtomNext );
  } // long CODB::AtomRopeAppend( CAtom* poAtom, const CRope* poRope, CAtom* poAtomNext )

CAtom* CODB::AtomRopeNext( const CAtom* poAtom, const CRope* poRope )
  {
  return (const_cast<CAtom*>(poAtom))->RopeLinkNext( const_cast<CRope*>(poRope) );
  } // CAtom* CODB::AtomRopeNext( const CAtom* poAtom, const CRope* poRope )

const CMapRope& CODB::AtomRopeMapGet( const CAtom* poAtom )
  {
  return poAtom->RopeMapGet();
  } // const CMapRope& CODB::AtomRopeMapGet( const CAtom* poAtom )

/****************/
/* O B J E C T */
/****************/
long CODB::ObjectAddAtom(CObject* poObject,
                               CAtom*   poAtom)
  {
  // test, if the poObject argument is valid
  const_iterator it = find(poObject);
  if ( it == end() )
    {
    Add(poObject);
    }
  // test, if the poAtom argument is valid
  it = find( const_cast<CAtom*>(poAtom) );
  if ( it == end() )
    {
    Add(poAtom);
    }
  *poObject += poAtom;
  return ODB_SUCCESS;

  } //


long CODB::ObjectLinkObject(CObject* poParent,
                                  CReason* poReason,
                                  CObject* poChild)
  {
  // test, if the poParent argument is valid
  const_iterator it = find(poParent);
  if (it == end() )
    {
    Add(poParent);
    }
  // test, if the poReason argument is valid
  it = find( const_cast<CReason*>(poReason) );
  if (it == end() )
    {
    Add(poReason);
    }
  // test, if the poChild argument is valid
  it = find( const_cast<CObject*>(poChild) );
  if (it == end() )
    {
    Add(poChild);
    }
  return poParent->Link(poChild, poReason);
  } // long CODB::ObjectAddObject(CObject* poParent,..


long CODB::ObjectSetClass(CObject* poObject,
                                CClass*  poClass)
  {
  // test, if the poObject argument is valid
  const_iterator it = find(poObject);
  if (it == end() )
    {
    Add(poObject);
    }
  // test, if the poClass argument is valid
  it = find( const_cast<CClass*>(poClass) );
  if (it == end() )
    {
    Add(poClass);
    }
  *poObject = *poClass;
  return ODB_SUCCESS;
  } // long CODB::ObjectSetClass (CObject* poObject, CClass*  poClass)

CClass* CODB::ObjectGetClass (const CObject* poObject)
  {
  // test, if the poObject argument is valid
  const_iterator it = find( const_cast<CObject*>(poObject) );
  if (it == end() )
    {
    return NULL;
    }
  return poObject->ClassGet();
  } // CClass* CODB::ObjectGetClass (CObject* poObject)

CVectorRoot CODB::ObjectGetChanged( timeval tFromTime )
  {
  CVectorRoot voResult;

  for (auto const &it : m_moId2PtrObject)
    {
    // ID = -1 is the "NULL-Instance"
    if ( it.first == -1 ) continue;
    // it->second is the pointer to the instance
    if ( it.second->TimeStampGet() > tFromTime )
      {
      voResult.push_back( it.second );
      }
    } // for (auto const &it : m_moId2PtrObject)

  // this makes a copy - I know - we will become thread safe ;-)
  return voResult;
  } // CVectorRoot CODB::ObjectGetChanged( time_t tFromTime )

CVectorRoot CODB::AtomGetChanged( timeval tFromTime )
  {
  CVectorRoot voResult;

  for (auto const &it : m_moId2PtrAtom)
    {
    // ID = -1 is the "NULL-Instance"
    if ( it.first == -1 ) continue;
    // it->second is the pointer to the instance
    if ( it.second->TimeStampGet() > tFromTime )
      {
      voResult.push_back( it.second );
      }
    } // for (auto const &it : m_moId2PtrAtom)

  // this makes a copy - I know - we will become thread safe ;-)
  return voResult;
  } // CVectorRoot CODB::AtomGetChanged( timeval tFromTime )

CVectorRoot CODB::GetThingsChanged( timeval tFromTime )
  {
  CVectorRoot voResult;

  for (auto const &it : *this)
    {
    // ID = -1 is the "NULL-Instance"
    if ( it->ID() == -1 ) continue;
    // it->second is the pointer to the instance
    if ( it->TimeStampGet() > tFromTime )
      {
      voResult.push_back( it );
      }
    } // for (auto const &it : *this)

  // this makes a copy - I know - we will become thread safe ;-)
  return voResult;
  } // CVectorRoot CODB::GetThingsChanged( timeval tFromTime )
