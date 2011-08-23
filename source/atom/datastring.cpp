/***************************************************************************
                          datastring.cpp
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

#include "atom/datastring.h"

using namespace odb;

//------------------------------------------------------------------//
// Construction/Destruction
//------------------------------------------------------------------//

CAtomDataString::CAtomDataString()
  : inherited()
  {
  InitBasics();
  }

CAtomDataString::CAtomDataString(CAtomDataString& src)
  : inherited(src)
  {
  InitBasics();
  *this = src;
  }

CAtomDataString::CAtomDataString(const std::string& src)
  : inherited()
  {
  InitBasics();
  m_Data = src;
  }

CAtomDataString::~CAtomDataString()
  {
  }

void CAtomDataString::InitBasics()
  {
  m_Data.erase();
  } // CAtomDataString::InitBasics()

CAtomDataString& CAtomDataString::operator = (CAtomDataString& src)
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
  } // CAtomDataString& CAtomDataString::operator = (CAtomDataString& src)

CAtomData* CAtomDataString::Copy()
  {
  return new CAtomDataString(*this);
  } // CAtomData* CAtomDataString::Copy()

/*
CStream& CAtomDataString::Save(CStream& oStream)
  {
  oStream << m_Data;

  return inherited::Save(oStream);
  } // CStream& CAtomDataString::Save(CStream& oStream)
*/

CStream& CAtomDataString::Load(CStream& oStream)
  {
  oStream >> m_Data;

  return inherited::Load(oStream);
  } // CStream& CAtomDataString::Load(CStream& oStream)

std::string CAtomDataString::UIFormat(const std::string& sFormat)
  {
  if ( sFormat.length() == 0 )
    {
    return m_Data;
    }

  char* psz = new char(m_Data.length() + sFormat.length());
  sprintf(psz, sFormat.c_str(), m_Data.c_str());
  std::string s = psz;
  delete [] psz;

  return s;
  } // string CAtomDataString::UIFormat(const string& sFormat)

long CAtomDataString::BinarySizeGet()
  {
  return m_Data.length() +1;
  } // long CAtomDataString::BinarySizeGet()

const void* CAtomDataString::BinaryBufferGet()
  {
  return m_Data.c_str();
  } // void* CAtomDataString::BinaryBufferGet()
