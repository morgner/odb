/***************************************************************************
                          datainteger.cpp
                          -------------------
    begin                : Fri Mar 3 2000
    copyright            : (C) 1993..2011 by Manfred Morgner
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

#include "atom/datainteger.h"

using namespace odb;

//------------------------------------------------------------------//
// Construction/Destruction
//------------------------------------------------------------------//

CAtomDataInteger::CAtomDataInteger()
  : inherited()
  {
  InitBasics();
  }

CAtomDataInteger::CAtomDataInteger(CAtomDataInteger& src)
  : inherited(src)
  {
  InitBasics();
  *this = src;
  }

CAtomDataInteger::CAtomDataInteger(long src)
  : inherited()
  {
  InitBasics();
  m_Data = src;
  }

CAtomDataInteger::~CAtomDataInteger()
  {
  }

void CAtomDataInteger::InitBasics()
  {
  m_Data = 0;
  } // CAtomDataInteger::InitBasics()

CAtomDataInteger& CAtomDataInteger::operator = (CAtomDataInteger& src)
  {
  if (&src == this)
    {
    return src;
    }
  if ( m_Data != src.m_Data )
    {
    TimeStampUpdate();
    m_Data = src.m_Data;
    }
  return *this;
  } // CAtomDataInteger& CAtomDataInteger::operator = (CAtomDataInteger& src)

CAtomData* CAtomDataInteger::Copy()
  {
  return new CAtomDataInteger(*this);
  } // CAtomData* CAtomDataInteger::Copy()

CStream& CAtomDataInteger::Save(CStream& oStream)
  {
  oStream << m_Data;

  return inherited::Save(oStream);
  } // CStream& CAtomDataInteger::Save(CStream& oStream)

CStream& CAtomDataInteger::Load(CStream& oStream)
  {
  oStream >> m_Data;

  return inherited::Load(oStream);
  } // CStream& CAtomDataInteger::Load(CStream& oStream)

std::string CAtomDataInteger::UIFormat(const std::string& sFormat)
  {
  char asz[64];

  if ( sFormat.length() == 0 )
    {
    sprintf(asz, "%d", (int)m_Data);
    }
  else
    {
    sprintf(asz, sFormat.c_str(), m_Data);
    }
  return asz;
  } // CString CAtomDataInteger::UIFormat(const CString& sFormat)

long CAtomDataInteger::BinarySizeGet()
  {
  return sizeof(m_Data);
  } // long CAtomDataInteger::BinarySizeGet()

const void* CAtomDataInteger::BinaryBufferGet()
  {
  return &m_Data;
  } // void* CAtomDataInteger::BinaryBufferGet()
