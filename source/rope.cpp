/***************************************************************************
                         rope.cpp  -  description
                             -------------------
    begin                : Fri Mar 3 2000
    copyright            : (C) 1993..2006 by Manfred Morgner
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

#include "rope.h"
#include "atom.h"
#include "odb.h"

using namespace odb;

//------------------------------------------------------------------//
// Construction/Destruction
//------------------------------------------------------------------//

IMPLEMENT_RTTI(Rope, Root, 0x20000321)

CRope::CRope(CAtom* poStartpoint, const std::string& sName)
  : inherited(sName)
  {
  m_poStartpoint = poStartpoint;
  } // CRope::CRope(CAtom* poStartpoint)

CRope::CRope(long lid)
  : inherited(lid)
  {
  } // CRope::CRope(long lid)

CAtom* CRope::StartpointGet()
  {
  return m_poStartpoint;
  } // CAtom* CRope::StartpointGet()

/// Save
CStream& CRope::Save(CStream& oStream)
  {
  oStream.Open("ROPE");
  inherited::Save(oStream);
  if ( m_poStartpoint != 0)
    {
    oStream.Element("start", m_poStartpoint->ID());
    } // if ( m_poStartpoint != 0)
  oStream.Close();
  return oStream;
  } // CStream& CRope::Save(CStream& oStream)

/// Load
CStream& CRope::Load(CStream& oStream)
  {
  long lPtrStartpoint;
  oStream >> lPtrStartpoint; m_poStartpoint = (CAtom*)lPtrStartpoint;
  return inherited::Load(oStream);
  } // CStream& CRope::Load(CStream& oStream)

bool CRope::ResolveIDs(CODB& oDB)
  {
  m_poStartpoint = oDB.Id2Ptr(m_poStartpoint);
  inherited::ResolveIDs(oDB);
  return true;
  } // bool CRope::ResolveIDs(CODB& oDB)
