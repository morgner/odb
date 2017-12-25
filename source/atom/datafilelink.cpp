/***************************************************************************
                          datafilelink.cpp
                          -------------------
    begin                : Fri Mar 3 2000
    copyright            : (C) 1993..2011 by Manfred Morgner
    email                : manfred@morgner.com
 ***************************************************************************/


#include "generic.h"

#include <string.h> // strlen

#include "atom/datafilelink.h"

using namespace odb;

//------------------------------------------------------------------//
// Construction/Destruction
//------------------------------------------------------------------//

CAtomDataFileLink::CAtomDataFileLink()
  : inherited()
  {
  InitBasics();
  }

CAtomDataFileLink::CAtomDataFileLink(CAtomDataFileLink& src)
  : inherited(src)
  {
  InitBasics();
  *this = src;
  }
/*
CAtomDataFileLink::CAtomDataFileLink(const fstream& src)
  : inherited()
  {
  InitBasics();

  m_Data  = "filename"; // ?? src.GetFilePath();
  m_lSize = 0;          // ?? src.GetLength();

  CFileStatus oFStatus;
  if ( src.GetStatus(oFStatus) )
    {
    m_tCreation     = oFStatus.m_ctime.GetTime();
    m_tModification = oFStatus.m_mtime.GetTime();
    }
  } // CAtomDataFileLink::CAtomDataFileLink(const fstream& src)
*/

CAtomDataFileLink::~CAtomDataFileLink()
  {
  }

void CAtomDataFileLink::InitBasics()
  {
  m_Data          = "";

  m_nSize         = 0;
  m_tCreation     = 0;
  m_tModification = 0;
  } // CAtomDataFileLink::InitBasics()

CAtomDataFileLink& CAtomDataFileLink::operator = (CAtomDataFileLink& src)
  {
  if (&src == this)
    {
    return src;
    }
  TimeStampUpdate();
  m_Data          = src.m_Data;
  m_nSize         = src.m_nSize;
  m_tCreation     = src.m_tCreation;
  m_tModification = src.m_tModification;
  return *this;
  } // CAtomDataFileLink& CAtomDataFileLink::operator = (CAtomDataFileLink& src)

CAtomData* CAtomDataFileLink::Copy()
  {
  return new CAtomDataFileLink(*this);
  } // CAtomData* CAtomDataFileLink::Copy()

CStream& CAtomDataFileLink::Save(CStream& oStream)
  {
  oStream << m_Data;

  return inherited::Save(oStream);
  } // CStream& CAtomDataFileLink::Save(CStream& oStream)

CStream& CAtomDataFileLink::Load(CStream& oStream)
  {
  oStream >> m_Data;

  return inherited::Load(oStream);
  } // CStream& CAtomDataFileLink::Load(CStream& oStream)

#define S_FORMAT "Link: %s"
std::string CAtomDataFileLink::UIFormat(const std::string& sFormat)
  {
  char* psz;
  if ( sFormat.length() == 0 )
    {
    psz = new char(m_Data.length() + strlen(S_FORMAT));
    sprintf(psz, S_FORMAT, m_Data.c_str());
    }
  else
    {
    psz = new char(m_Data.length() + sFormat.length());
    sprintf(psz, sFormat.c_str(), m_Data.c_str());
    } // if ( sFormat.length() == 0 )

  std::string s = psz;
  delete [] psz;
  return s;
  } // CString CAtomDataFileLink::UIFormat(const CString& sFormat)
#undef S_FORMAT

long CAtomDataFileLink::BinarySizeGet()
  {
  return m_nSize;
  } // long CAtomDataFileLink::BinarySizeGet()

const void* CAtomDataFileLink::BinaryBufferGet()
  {
  return &m_Data;
  } // void* CAtomDataFileLink::BinaryBufferGet()
