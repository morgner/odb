/***************************************************************************
                          datareal.cpp
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

#include "atom/datareal.h"

using namespace odb;

//------------------------------------------------------------------//
// Construction/Destruction
//------------------------------------------------------------------//

CAtomDataReal::CAtomDataReal()
  : inherited()
  {
  InitBasics();
  }

CAtomDataReal::CAtomDataReal(CAtomDataReal& src)
  : inherited(src)
  {
  InitBasics();
  *this = src;
  }

CAtomDataReal::CAtomDataReal(double src)
  : inherited()
  {
  InitBasics();
  m_Data = src;
  }

CAtomDataReal::~CAtomDataReal()
  {
  }

void CAtomDataReal::InitBasics()
  {
  m_Data = 0.0;
  } // CAtomDataReal::InitBasics()

CAtomDataReal& CAtomDataReal::operator = (CAtomDataReal& src)
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
  } // CAtomDataReal& CAtomDataReal::operator = (CAtomDataReal& src)

CAtomData* CAtomDataReal::Copy()
  {
  return new CAtomDataReal(*this);
  } // CAtomData* CAtomDataReal::Copy()

CStream& CAtomDataReal::Save(CStream& oStream)
  {
  oStream << m_Data;

  return inherited::Save(oStream);
  } // CStream& CAtomDataReal::Save(CStream& oStream)

CStream& CAtomDataReal::Load(CStream& oStream)
  {
  oStream >> m_Data;

  return inherited::Load(oStream);
  } // CStream& CAtomDataReal::Load(CStream& oStream)

std::string CAtomDataReal::UIFormat(const std::string& sFormat)
  {
  char asz[64];

  if ( sFormat.length() == 0 )
    {
    sprintf(asz, "%0.3f", m_Data);
    }
  else
    {
    sprintf(asz, sFormat.c_str(), m_Data);
    }
  return asz;
  } // CString CAtomDataReal::UIFormat(const CString& sFormat)

long CAtomDataReal::BinarySizeGet()
  {
  return sizeof(m_Data);
  } // long CAtomDataReal::BinarySizeGet()

const void* CAtomDataReal::BinaryBufferGet()
  {
  return &m_Data;
  } // void* CAtomDataReal::BinaryBufferGet()
