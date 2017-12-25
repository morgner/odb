/***************************************************************************
                          datastring.cpp
                          -------------------
    begin                : Fri Mar 3 2000
    copyright            : (C) 1993..2011 by Manfred Morgner
    email                : manfred@morgner.com
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
  delete psz;

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
