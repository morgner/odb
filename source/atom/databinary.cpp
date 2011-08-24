/***************************************************************************
                           databinary.cpp
                           -------------------
    begin                : Wed Apr 15 2000
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

#include "atom/databinary.h"

using namespace odb;

std::string CAtomDataBinary::s_sUIText = "<binary>";

//------------------------------------------------------------------//
// Construction/Destruction
//------------------------------------------------------------------//

CAtomDataBinary::CAtomDataBinary()
  : inherited()
  {
  InitBasics();
  } // CAtomDataBinary::CAtomDataBinary()

CAtomDataBinary::CAtomDataBinary(void* pData, long& nSizeInBytes)
  : inherited()
  {
  InitBasics();
  DataSet(pData, nSizeInBytes);
  } // CAtomDataBinary::CAtomDataBinary()

CAtomDataBinary::~CAtomDataBinary()
  {
  DataFree();
  } // CAtomDataBinary::~CAtomDataBinary()

void CAtomDataBinary::InitBasics()
  {
  m_Data  = 0;
  m_nSize = 0;
  } // void CAtomDataBinary::InitBasics()


CAtomDataBinary& CAtomDataBinary::operator = (CAtomDataBinary& src)
  {
  if (&src == this)
    {
    return src;
    }
  TimeStampUpdate();
  DataSet(src.m_Data, src.m_nSize);
  return *this;
  } // CAtomDataBinary& CAtomDataBinary::operator = (CAtomDataBinary& src)

CAtomData* CAtomDataBinary::Copy()
  {
  return new CAtomDataBinary(*this);
  } // inherited* CAtomDataBinary::Copy()

CStream& CAtomDataBinary::Save(CStream& oStream)
  {
  oStream << m_nSize;
  if (m_nSize > 0)
    {
    oStream.Write(m_Data, m_nSize);
    } // if (m_nSize > 0)
  return inherited::Save(oStream);
  } // CStream& CAtomDataBinary::Save(CStream& oStream)

CStream& CAtomDataBinary::Load(CStream& oStream)
  {
  DataFree();
  oStream >> m_nSize;

  if (m_nSize > 0)
    {
    m_Data = new unsigned char[m_nSize];
    oStream.Read(m_Data, m_nSize);
    } // if (m_nSize > 0)
  return inherited::Load(oStream);
  } // CStream& CAtomDataBinary::Load(CStream& oStream)

std::string CAtomDataBinary::UIFormat(const std::string& sFormat)
  {
  return s_sUIText;
  } // string CAtomDataBinary::UIFormat(const string& sFormat)

bool CAtomDataBinary::DataSet(void* pData, long& nSizeInBytes)
  {
  DataFree();
  try
    {
    m_Data = new unsigned char[nSizeInBytes];
    memcpy(m_Data, pData, nSizeInBytes);
    m_nSize = nSizeInBytes;
    }
  /*
   * we prevent the system from crashing if
   * anything is invalid with the data
   */
  catch(...) // an GPF
    {
    nSizeInBytes = 0;
    return false;
    } // catch(...)
  return true;
  } // bool CAtomDataBinary::DataSet(void* pData, long& nSizeInBytes)

bool CAtomDataBinary::DataFree()
  {
  // this is for multithreading
  unsigned char* puc = m_Data;
  m_Data = 0;
  delete puc;
  TimeStampUpdate();
  return true;
  } // bool CAtomDataBinary::DataFree()

long CAtomDataBinary::BinarySizeGet()
  {
  return m_nSize;
  } // long CAtomDataBinary::BinarySizeGet()

const void* CAtomDataBinary::BinaryBufferGet()
  {
  return m_Data;
  } // void* CAtomDataBinary::BinaryBufferGet()
