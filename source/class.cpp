/***************************************************************************
                         class.cpp  -  description
                             -------------------
    begin                : Fri Mar 3 2000
    copyright            : (C) 1993..2006 by Manfred Morgner
    email                : manfred@morgner.com
 ***************************************************************************/


#include "generic.h"

#include "class.h"
#include "object.h"
#include "odb.h"

using namespace odb;

//------------------------------------------------------------------//
// Construction/Destruction
//------------------------------------------------------------------//

IMPLEMENT_RTTI(Class, Root, 0x00980430)

CClass::CClass()
  : inherited()
  {
  InitBasics();
  } // CClass::CClass()

CClass::CClass(long lid)
  : inherited(lid)
  {
  InitBasics();
  } // CClass::CClass(lid)

CClass::CClass(const std::string& sName, CObject* poOMAL)
  : inherited(sName)
  {
  InitBasics();

  if ( poOMAL != 0 )
    {
    m_poOMAL = poOMAL;
    } // if ( poOMAL != 0 )
  } // CClass::CClass(const string& sName, CObject* poOMAL)

CClass::CClass(CClass& src)
  {
  *this = src;
  } // CClass::CClass(CClass& src)

CClass::~CClass()
  {
  } // CClass::~CClass()

void CClass::InitBasics()
  {
  m_poOMAL  = 0;
  m_poClass = 0;
  } // void CClass::InitBasics()

CClass* CClass::operator = (const CClass* src)
  {
  if (src == this)
    {
    return this;
    }
  m_poOMAL = src->m_poOMAL;
  TimeStampUpdate();
  return this;
  } // CClass* CClass::operator = (const CClass* src)

CRoot* CClass::Copy()
  {
  return new CClass(*this);
  } // CRoot* CClass::Copy()

CStream& CClass::Save(CStream& oStream)
  {
  oStream.Open("CLASS");
  inherited::Save(oStream);
  if ( m_poOMAL != 0 )
    {
    oStream.Element("omal", m_poOMAL->ID() );
    }
  oStream.Close();
  return oStream;
  } // CStream& CClass::Save(CStream& oStream)

CStream& CClass::Load(CStream& oStream)
  {
  long lPtrOMAL;
  oStream >> lPtrOMAL; m_poOMAL = (CObject*)lPtrOMAL;
  return inherited::Load(oStream);
  } // CStream& CClass::Load(CStream& oStream)

bool CClass::ResolveIDs(CODB& oDB)
  {
  m_poOMAL = oDB.Id2Ptr(m_poOMAL);
  inherited::ResolveIDs(oDB);
  return true;
  } // bool CClass::ResolveIDs(CODB& oDB)

CObject* CClass::OmalGet() const
  {
  return m_poOMAL;
  } // CObject* CClass::OmalGet() const

bool CClass::OmalSet( const CObject* poOMAL, bool bOverwrite )
  {
  if ( (m_poOMAL != 0) || (bOverwrite == false) )
    {
    // OMAL already exists
    // return true, if it is the same as in the method argument
    return poOMAL == m_poOMAL;
    }
  m_poOMAL = const_cast<CObject*>(poOMAL);
  TimeStampUpdate();
  return true;
  } // bool CClass::OmalSet( const CObject* poOMAL, bool bOverwrite )

CObject* CClass::operator = (const CObject* poOMAL)
  {
  OmalSet( poOMAL, false );
  return m_poOMAL;
  } // CObject* CClass::operator = (CObject* poOMAL)

CClass::operator CObject*() const
  {
  return m_poOMAL;
  } // CObject* CClass::operator = (CObject* poOMAL)


/// ClassSet()
CClass* CClass::ClassSet(const CClass* poClass)
  {
  return m_poClass = const_cast<CClass*>(poClass);
  } // const CClass* CClass::ClassSet()

/// ClassGet()
CClass* CClass::ClassGet() const
  {
  return m_poClass;
  } // const CClass* CClass::ClassGet()
