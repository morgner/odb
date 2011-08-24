/***************************************************************************
                          datafile.cpp
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

#include <string.h> // strlen

#include "atom/datafile.h"

using namespace odb;

//------------------------------------------------------------------//
// Construction/Destruction
//------------------------------------------------------------------//

CAtomDataFile::CAtomDataFile()
  : inherited()
  {
  InitBasics();
  } // CAtomDataFile::CAtomDataFile()

CAtomDataFile::CAtomDataFile(CAtomDataFile& src)
  : inherited(src)
  {
  InitBasics();
  *this = src;
  } // CAtomDataFile::CAtomDataFile(CAtomDataFile& src)
/*
CAtomDataFile::CAtomDataFile(const fstream& src)
  : inherited()
  {
  InitBasics();

	m_sFileName = src.GetFileName();
	m_sFilePath = src.GetFilePath();
  m_lFileSize = src.GetLength();

  CFileStatus oFStatus;
  if ( src.GetStatus(oFStatus) )
    {
    m_tCreation     = oFStatus.m_ctime.GetTime();
    m_tModification = oFStatus.m_mtime.GetTime();
    }
  } // CAtomDataFile::CAtomDataFile(const CFile& src)
*/
CAtomDataFile::~CAtomDataFile()
  {
  if (m_Data)
    {
    delete [] m_Data;
    m_Data = 0;
    }
  } // CAtomDataFile::~CAtomDataFile()

void CAtomDataFile::InitBasics()
  {
  m_Data      = 0;
  m_nSize     = 0;  // size of m_pBuffer
	m_sFileName = ""; // filename
  } // void CAtomDataFile::InitBasics()

CAtomDataFile& CAtomDataFile::operator = (CAtomDataFile& src)
  {
  if (&src == this)
    {
    return src;
    }
  TimeStampUpdate();
/* 2do implementation
  m_Data      = src.m_Data;
  m_nSize     = src.m_nSize;
	m_sFileName = src.m_sFileName;
*/
  return *this;
  } // CAtomDataFile& CAtomDataFile::operator = (CAtomDataFile& src)

CAtomData* CAtomDataFile::Copy()
  {
  return new CAtomDataFile(*this);
  } // CAtomData* CAtomDataFile::Copy()

CStream& CAtomDataFile::Save(CStream& oStream)
  {
  if (m_Data == 0)
    {
    m_nSize = m_nFileSize;
    m_Data  = new unsigned char[m_nSize];
/*!!
    CFile oFile(m_sFilePath, CFile::modeRead);
    oFile.Read(m_Data, m_nSize);
*/
    }

  oStream << m_nSize;
  oStream << m_sFileName;
  oStream << m_sFilePath;      // original file path
  oStream << m_nFileSize;      // original file size
  oStream << m_tCreation;      // date of creation
  oStream << m_tModification;  // date of last modification

  oStream.Write(m_Data, m_nSize); // throw( CFileException );

  return inherited::Save(oStream);
  } // CStream& CAtomDataFile::Save(CStream& oStream)

CStream& CAtomDataFile::Load(CStream& oStream)
  {
  oStream >> m_nSize;
  oStream >> m_sFileName;
  oStream >> m_sFilePath;      // original file path
  oStream >> m_nFileSize;      // original file size
  oStream >> m_tCreation;      // date of creation
  oStream >> m_tModification;  // date of last modification

  if (m_Data)
    {
    delete [] m_Data;
    }

  m_Data = new unsigned char[m_nSize];
  oStream.Read(m_Data, m_nSize); // throw( CFileException );

  return inherited::Load(oStream);
  } // CStream& CAtomDataFile::Load(CStream& oStream)

#define S_FORMAT "Link: %s"
std::string CAtomDataFile::UIFormat(const std::string& sFormat)
  {
  char* psz;
  if ( sFormat.length() == 0 )
    {
    psz = new char(m_sFileName.length() + strlen(S_FORMAT));
    sprintf(psz, S_FORMAT, m_sFileName.c_str());
    }
  else
    {
    psz = new char(m_sFileName.length() + sFormat.length());
    sprintf(psz, sFormat.c_str(), m_sFileName.c_str());
    } // if ( sFormat.length() == 0 )

  std::string s = psz;
  delete [] psz;
  return s;
  } // CString CAtomDataFile::UIFormat(const CString& sFormat)
#undef S_FORMAT

long CAtomDataFile::BinarySizeGet()
  {
  return m_nSize;
  } // long CAtomDataFile::BinarySizeGet()

const void* CAtomDataFile::BinaryBufferGet()
  {
  return &m_Data;
  } // void* CAtomDataFile::BinaryBufferGet()
