/***************************************************************************
                           atomdata.cpp
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

#include <string.h> // memcpy

#include "atom/data.h"

using namespace odb;

std::string CAtomData::s_sEmptyString = "";

//------------------------------------------------------------------//
// Construction/Destruction
//------------------------------------------------------------------//

CAtomData::CAtomData()
  {
  InitBasics();
  } // CAtomData::CAtomData()

CAtomData::CAtomData(CAtomData& src)
  {
  InitBasics();
  *this = src;
  } // CAtomData::CAtomData(CAtomData& src)

CAtomData::~CAtomData()
  {
  } // CAtomData::~CAtomData()

void CAtomData::InitBasics()
  {
  TimeStampUpdate();
  } // CAtomData::InitBasics()


std::string CAtomData::UIFormat(const std::string& sFormat)
  {
  return s_sEmptyString;
  } // string CAtomData::UIFormat(const string& sFormat)

CAtomData& CAtomData::operator = (CAtomData& src)
  {
  return *this;
  } // CAtomData& CAtomData::operator = (CAtomData& src)

CAtomData* CAtomData::Copy()
  {
  return new CAtomData;
  } // CAtomData* CAtomData::Copy()

CAtomData& CAtomData::Assign(CAtomData& src)
  {
  return *this = src;
  } // CAtomData* CAtomData::Assign(CAtomData& src)

CStream& CAtomData::Save(CStream& oStream)
  {
  oStream.Open("content");
  oStream.Element("timestamp", m_tTimeStamp);
  oStream.Element("data", UIFormat(""));
  oStream.Close();
  return oStream;
  } // CStream& CAtomData::Save(CStream& oStream)

CStream& CAtomData::Load(CStream& oStream)
  {
  return oStream;
  } // CStream& CAtomData::Load(CStream& oStream)


timeval CAtomData::TimeStampGet()
  {
  return m_tTimeStamp;
  } // time_t CAtomData::TimeStampGet()

bool CAtomData::TimeStampUpdate()
  {
  return gettimeofday(&m_tTimeStamp, 0) == 0;
  } // bool CAtomData::TimeStampUpdate()

long CAtomData::BinaryDataGet(void* pData, long nSizeOfBuffer)
  {
	long nSize = BinarySizeGet();

  if ( (nSize > 0) && (nSizeOfBuffer >= nSize) )
    {
    try
      {
      memcpy(pData, BinaryBufferGet(), nSize);
      }
    catch(...)
      {
      // GPF -> logging ?
      }
    } // if ( rnSize >= nSize )
  return nSize;
  } // long CAtomData::BinaryDataGet(void* pData, long nSizeOfBuffer)

long CAtomData::BinarySizeGet()
  {
  return 0;
  } // long CAtomData::BinarySizeGet()

const void* CAtomData::BinaryBufferGet()
  {
  return 0;
  } // void* CAtomData::BinaryBufferGet()
