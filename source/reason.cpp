/***************************************************************************
                          reason.cpp  -  description
                             -------------------
    begin                : Fri Mar 3 2000
    copyright            : (C) 1993..2000 by Manfred Morgner
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

#include "reason.h"
#include "object.h"
#include "odb.h"

using namespace odb;

//------------------------------------------------------------------//
// Construction/Destruction
//------------------------------------------------------------------//

IMPLEMENT_RTTI(Reason, Root, 0x00980430)


CReason::CReason()
  :CRoot()
  {
  InitBasics();
  } // CReason::CReason()

CReason::CReason(long lid)
  :CRoot(lid)
  {
  InitBasics();
  } // CReason::CReason(long lid)

CReason::CReason(const std::string& sName, CReason& src)
  :CRoot(sName)
  {
  InitBasics();

  m_poClassChild  = src.m_poClassChild;
  m_poClassParent = src.m_poClassParent;
  } // CReason::CReason(const std::string& sName, CReason& src)

CReason::CReason(const std::string& sName, const CClass* poClassParent, const CClass* poClassChild)
  :CRoot(sName)
  {
  InitBasics();

  m_poClassChild  = const_cast<CClass*>(poClassChild);
  m_poClassParent = const_cast<CClass*>(poClassParent);
  } // CReason::CReason(const std::string& sName, CClass& oClassParent, CClass& oClassChild)

CReason::CReason(const std::string& sName, const CObject* poObjectParent, const CObject* poObjectChild)
  :CRoot(sName)
  {
  InitBasics();

  m_poClassChild  = const_cast<CClass*>( poObjectChild->ClassGet() );
  m_poClassParent = const_cast<CClass*>( poObjectParent->ClassGet() );
  } // CReason(const std::string& sName, CObject& oObjectParent, CObject& oObjectChild)

CReason::CReason(CReason& src)
  :CRoot()
  {
  InitBasics();
  *this = src;
  } // CReason::CReason(CReason& src)

CReason::~CReason()
  {
  } // CReason::~CReason()

CRoot* CReason::Copy()
  {
  return new CReason(*this);
  } // CRoot* CReason::Copy()

void CReason::InitBasics()
  {
  m_poClassParent = NULL;
  m_poClassChild  = NULL;
  } // void CReason::InitBasics()

CReason& CReason::operator = (CReason& src)
  {
  CRoot::operator=( *(CRoot*)&src );

  m_poClassChild  = src.m_poClassChild;
  m_poClassParent = src.m_poClassParent;

  return *this;
  } // CReason& CReason::operator = (CReason& src)
 
CStream& CReason::Save(CStream& oStream)
  {
  oStream.Open("REASON");
  inherited::Save(oStream);
  oStream.Element("child", m_poClassChild ->ID());
  oStream.Element("parent", m_poClassParent->ID());
  oStream.Close();
  return oStream;
  } // CStream& CReason::Save(CStream& oStream)

CStream& CReason::Load(CStream& oStream)
  {
  long lPtrClass;
  oStream >> lPtrClass; m_poClassChild  = (CClass*)lPtrClass;
  oStream >> lPtrClass; m_poClassParent = (CClass*)lPtrClass;

  return CRoot::Load(oStream);
  } // CStream& CReason::Load(CStream& oStream)

bool CReason::ResolveIDs(CODB& oDB)
  {
  m_poClassChild  = oDB.Id2Ptr(m_poClassChild);
  m_poClassParent = oDB.Id2Ptr(m_poClassParent);
  CRoot::ResolveIDs(oDB);
  return true;
  } // bool CReason::ResolveIDs(CODB& oDB)

//------------------------------------------------------------------//
// operations
//------------------------------------------------------------------//

bool CReason::Check(const CObject* poParent, const CObject* poChild) const
  {
  return (   ( m_poClassParent == poParent->ClassGet() )
          && ( m_poClassChild  == poChild ->ClassGet() ) );
  } // bool CReason::Check(CObject& oParent, CObject& oChild)
