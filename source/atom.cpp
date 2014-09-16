/***************************************************************************
                          atom.cpp  -  description
                             -------------------
    begin                : Fri Mar 3 2000
    copyright            : (C) 1993..2014 by Manfred Morgner
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

#include "odb.h"
#include "atom.h"
// atom data classes
#include "atom/datafile.h"
#include "atom/datafilelink.h"
#include "atom/data3dpoint.h"
#include "atom/datainteger.h"
#include "atom/datareal.h"
#include "atom/datastring.h"
#include "atom/databinary.h"

using namespace odb;

std::string CAtom::s_sEmptyString = "";

//------------------------------------------------------------------//
// Construction/Destruction
//------------------------------------------------------------------//

IMPLEMENT_RTTI(Atom, Root, 0x00000420)

/// default constructor
CAtom::CAtom()
  : inherited()
  {
  InitBasics(false);
  } // CAtom::CAtom()

CAtom::CAtom(long lid)
  : inherited(lid)
  {
  InitBasics(false);
  } // CAtom::CAtom(long lid)

CAtom::CAtom(const std::string& sName, bool bADO)
  : inherited(sName)
  {
  InitBasics(bADO);
  } // CAtom::CAtom(const string& sName, bool bADO)

CAtom::CAtom(CAtom& src, bool bADO)
  : inherited()
  {
  InitBasics(bADO);
  operator = ( src );
  // to ensure, the copy operation does not overwrite "bADO"
  m_bADO = bADO;
  } // CAtom::CAtom(const CAtom& src, bool bADO)

CAtom::CAtom(CAtom& src)
  : inherited()
  {
  InitBasics(false);
  operator = ( src );
  } // CAtom::CAtom(const CAtom& src)

CAtom::CAtom(const std::string& sName, CAtom& oADO, bool bADO)
  : inherited(sName)
  {
  InitBasics(bADO);
  if (this != &oADO)
    {
    operator = ( oADO );
    m_poADO = &oADO;
    }
  // to ensure, the copy operation does not overwrite "bADO"
  m_bADO = bADO;
  } // CAtom::CAtom(const std::string& sName, CAtom& oADO, bool bADO)

/// assignement constructor
CAtom::CAtom(const std::string& sName, long src, bool bADO)
  : inherited(sName)
  {
  InitBasics(bADO);
  operator = ( src );
  } // CAtom::CAtom(const std::string& sName, long src, bool bADO)

/// assignement constructor
CAtom::CAtom(const std::string& sName, double src, bool bADO)
  : inherited(sName)
  {
  InitBasics(bADO);
  operator = ( src );
  } // CAtom::CAtom(const std::string& sName, double src, bool bADO)

/// assignement constructor
CAtom::CAtom(const std::string& sName, float src, bool bADO)
  : inherited(sName)
  {
  InitBasics(bADO);
  operator = ( src );
  } // CAtom::CAtom(const std::string& sName, float src, bool bADO)

/// assignement constructor
CAtom::CAtom(const std::string& sName, const std::string& src, bool bADO)
  : inherited(sName)
  {
  InitBasics(bADO);
  operator = ( src );
  } // CAtom::CAtom(const std::string& sName, const std::string& src, bool bADO)

/*
/// assignement constructor
CAtom::CAtom(const string& sName, const fstream& src, bool bADO)
  : inherited(sName)
  {
  InitBasics(bADO);
  *this = src;
  } // CAtom::CAtom(const string& sName, const fstream& src, bool bADO)
*/

/// assignement constructor
CAtom::CAtom(const std::string& sName, void* pData, long& nSizeInBytes, bool bADO)
  {
  InitBasics(bADO);
  operator = ( new CAtomDataBinary(pData, nSizeInBytes) );
  } // CAtom::CAtom(const std::string& sName, void* pData, long& nSizeInBytes, bool bADO)

/// assignement constructor
CAtom::CAtom(const std::string& sName, CAtomDataBinary* poData, bool bADO)
  {
  InitBasics(bADO);
  operator = ( poData );
  } // CAtom::CAtom(const std::string& sName, CAtomDataBinary* poData, bool bADO)


/// destructor
CAtom::~CAtom()
  {
  if (m_poData)
    {
    delete m_poData;
    }
  } // CAtom::~CAtom()

void CAtom::InitBasics(bool bADO)
  {
  m_poData      = 0;
  m_bADO        = bADO;
  m_lMaskFields = 0;
  m_poADO       = 0;

  m_lDisplayResourceId = -1;

  m_sPrefix.erase();
  m_sSuffix.erase();
  m_sFormat.erase();
//  m_bIs2Link = false;

  m_nUserSign = 0;
  } // void CAtom::InitBasics()

CAtom& CAtom::operator = (CAtom& src)
  {
  inherited::operator=( *(inherited*)&src );

  m_bADO = src.m_bADO;
  if (m_poADO != src.m_poADO)
    {
    m_poADO = src.m_poADO;
    }
  m_lMaskFields        = src.m_lMaskFields;
  m_lDisplayResourceId = src.m_lDisplayResourceId;
  m_sPrefix            = src.m_sPrefix;
  m_sSuffix            = src.m_sSuffix;
  m_sFormat            = src.m_sFormat;
  m_nUserSign          = src.m_nUserSign;

  TimeStampUpdate();
  return *this;
  } // const CAtom& CAtom::operator = (const CAtom& src)

CRoot* CAtom::Copy()
  {
  return new CAtom(*this);
  } // inherited* CAtom::Copy()

CStream& CAtom::Save(CStream& oStream)
  {
  oStream.Open("ATOM");
  inherited::Save(oStream);

  m_lMaskFields = 0;
  if ( m_lDisplayResourceId != -1 ) m_lMaskFields |= ATOM_HAS_RESID;
  if ( m_sFormat.length()    >  0 ) m_lMaskFields |= ATOM_HAS_FORMAT;
  if ( m_sPrefix.length()    >  0 ) m_lMaskFields |= ATOM_HAS_PREFIX;
  if ( m_sSuffix.length()    >  0 ) m_lMaskFields |= ATOM_HAS_SUFFIX;
  if ( m_poData != 0 )              m_lMaskFields |= ATOM_HAS_DATA;
  if ( m_nUserSign != 0 )           m_lMaskFields |= ATOM_HAS_USER_SIGN;

  if (m_bADO)
    {
    oStream.Element("isADO", "yes");
    }
//  oStream << m_lMaskFields;
  if ( (m_lMaskFields & ATOM_HAS_RESID    ) == ATOM_HAS_RESID     ) oStream.Element("idResource", m_lDisplayResourceId);
  if ( (m_lMaskFields & ATOM_HAS_FORMAT   ) == ATOM_HAS_FORMAT    ) oStream.Element("Format", m_sFormat);
  if ( (m_lMaskFields & ATOM_HAS_PREFIX   ) == ATOM_HAS_PREFIX    ) oStream.Element("Prefix", m_sPrefix);
  if ( (m_lMaskFields & ATOM_HAS_SUFFIX   ) == ATOM_HAS_SUFFIX    ) oStream.Element("Suffix", m_sSuffix);
  if ( (m_lMaskFields & ATOM_HAS_USER_SIGN) == ATOM_HAS_USER_SIGN ) oStream.Element("signUser", m_nUserSign);

  if ( (m_lMaskFields & ATOM_HAS_DATA) == ATOM_HAS_DATA)
    {
//    oStream.Element("type", m_poData->Type());
    m_poData->Save(oStream);
    }

  oStream.Close();
  return oStream;
  } // CStream& CAtom::Save(CStream& oStream)

CStream& CAtom::Load(CStream& oStream)
  {
  oStream >> m_bADO;
  oStream >> m_lMaskFields;

  if ( (m_lMaskFields & ATOM_HAS_RESID    ) == ATOM_HAS_RESID     ) oStream >> m_lDisplayResourceId;
  if ( (m_lMaskFields & ATOM_HAS_FORMAT   ) == ATOM_HAS_FORMAT    ) oStream >> m_sFormat;
  if ( (m_lMaskFields & ATOM_HAS_PREFIX   ) == ATOM_HAS_PREFIX    ) oStream >> m_sPrefix;
  if ( (m_lMaskFields & ATOM_HAS_SUFFIX   ) == ATOM_HAS_SUFFIX    ) oStream >> m_sSuffix;
  if ( (m_lMaskFields & ATOM_HAS_USER_SIGN) == ATOM_HAS_USER_SIGN ) oStream >> m_nUserSign;

  if ( (m_lMaskFields & ATOM_HAS_DATA) == ATOM_HAS_DATA)
    {
    if ( m_poData != 0 )
      {
      delete m_poData;
      }
    long lType;
    oStream >> lType;
    switch (lType)
      {
      case DT_NONE:
        m_poData = new CAtomData;
        break;
      case DT_INTEGER:
        m_poData = new CAtomDataInteger;
        break;
      case DT_REAL:
        m_poData = new CAtomDataReal;
        break;
      case DT_STRING:
        m_poData = new CAtomDataString;
        break;
      case DT_FILE:
        m_poData = new CAtomDataFile;
        break;
      case DT_FILELINK:
        m_poData = new CAtomDataFileLink;
        break;
      case DT_G3DPOINT:
        m_poData = new CAtomData3DPoint;
        break;
      case DT_BINARY:
        m_poData = new CAtomDataBinary;
        break;
      } // switch (lType)
    m_poData->Load(oStream);
    }

  return inherited::Load(oStream);
  } // CStream& CAtom::Load(CStream& oStream)

bool CAtom::ResolveIDs(CODB& oDB)
  {
  m_poADO = oDB.Id2Ptr(m_poADO);
  if (m_poADO == this)
    {
    m_poADO = 0;
    }
  inherited::ResolveIDs(oDB);
  return true;
  } // bool CAtom::ResolveIDs(CODB& oDB)


//------------------------------------------------------------------//
// assignement operators
//------------------------------------------------------------------//

CAtom& CAtom::operator = (long src)
  {
  if (m_poData)
    {
    delete m_poData;
    }
  m_poData = new CAtomDataInteger(src);
  TimeStampUpdate();
  return *this;
  } // CAtom& CAtom::operator = (long src)


CAtom& CAtom::operator = (double src)
  {
  if (m_poData)
    {
    delete m_poData;
    }
  m_poData = new CAtomDataReal(src);
  TimeStampUpdate();
  return *this;
  } // CAtom& CAtom::operator = (double src)

CAtom& CAtom::operator = (float src)
  {
  return *this = (double)src;
  } // CAtom& CAtom::operator = (float src)

CAtom& CAtom::operator = (const std::string& src)
  {
  if (m_poData)
    {
    delete m_poData;
    }
  m_poData = new CAtomDataString(src);
  TimeStampUpdate();
  return *this;
  } //
/*
CAtom& CAtom::operator = (const fstream& src)
  {
  if (m_poData)

    {
    delete m_poData;
    }
  m_poData = new CAtomDataFile(src);
  return *this;
  } //
*/

CAtom& CAtom::operator = (CAtomDataBinary* poData)
  {
  if (m_poData)
    {
    delete m_poData;
    }
  m_poData = poData;
  TimeStampUpdate();
  return *this;
  } // CAtom& CAtom::operator = (CAtomDataBinary* poData)

//------------------------------------------------------------------//
// Member access
//------------------------------------------------------------------//
const long CAtom::ResIdSet(long lid)
  {
  if ( m_lDisplayResourceId != lid )
    {
    TimeStampUpdate();
    }
  return m_lDisplayResourceId = lid;
  } // const long CAtom::ResIdSet(long lid)

const std::string& CAtom::FormatSet(const std::string& sFormat)
  {
  TimeStampUpdate();
  return m_sFormat = sFormat;
  } // const string& CAtom::FormatSet(const string& sFormat)

const std::string& CAtom::PrefixSet(const std::string& sPrefix)
  {
  TimeStampUpdate();
  return m_sPrefix = sPrefix;
  } // const string& CAtom::PrefixSet(const string& sPrefix)

const std::string& CAtom::SuffixSet(const std::string& sSuffix)
  {
  TimeStampUpdate();
  return m_sSuffix = sSuffix;
  } // const string& CAtom::SuffixSet(const string& sSuffix)

//------------------------------------------------------------------//
// Member query
//------------------------------------------------------------------//

const long CAtom::ResIdGet() const
  {
  return m_lDisplayResourceId;
  } // const long CAtom::ResIdGet() const

const std::string& CAtom::FormatGet() const
  {
  if ( (m_sFormat.length() == 0) && (m_poADO != 0) && (m_poADO != this) )
    {
    return m_poADO->FormatGet();
    }
  return m_sFormat;
  } // const string& CAtom::FormatGet() const

const std::string& CAtom::PrefixGet() const
  {
  if ( (m_sPrefix.length() == 0) && (m_poADO != 0) && (m_poADO != this) )
    {
    return m_poADO->PrefixGet();
    }
  return m_sPrefix;
  } // const string& CAtom::PrefixGet() const

const std::string& CAtom::SuffixGet() const
  {
  if ( (m_sSuffix.length() == 0) && (m_poADO != 0) && (m_poADO != this) )
    {
    return m_poADO->SuffixGet();
    }
  return m_sSuffix;
  } // const string& CAtom::SuffixGet() const

long CAtom::RopeLinkAdd(CRope* poRope, CAtom* poAtomNext)
  {
  // if THIS rope is already known, the search result is xx.end()
  if ( m_moRope.find(poRope) != m_moRope.end() )
    {
    // the rope is already linke by an other way
    // unable to connect more than ones
    return ODB_ROPE_RECURSION;
    }
  m_moRope[poRope] = poAtomNext;
  TimeStampUpdate();
  return ODB_SUCCESS;
  } // long CAtom::RopeLinkAdd(CRope* poRope, CAtom* poAtomNext)

long CAtom::RopeLinkRemove(CRope* poRope)
  {
  auto it = m_moRope.find(poRope);
  if ( it == m_moRope.end() )
    {
    //  the rope is unknown this means:
    // "removing" is successfull - nothings to do => success
    return ODB_SUCCESS;
    }
  m_moRope.erase(it);
  TimeStampUpdate();
  return ODB_SUCCESS;
  } // long CAtom::RopeLinkRemove(CRope* poRope)

CAtom* CAtom::RopeLinkNext(CRope* poRope)
  {
  auto it = m_moRope.find(poRope);
  if ( it == m_moRope.end() )
    {
    // rope unknown -> no link exists
    return NULL;
    }
  return it->second;
  } // CAtom* CAtom::RopeLinkNext(CRope* poRope)

const CMapRope& CAtom::RopeMapGet() const
  {
  return m_moRope;
  }

//------------------------------------------------------------------//
// Data query
//------------------------------------------------------------------//
std::string CAtom::UIFormat()
  {
  if (m_poData)
    {
    return m_poData->UIFormat( FormatGet() );
    }

  return _TEXT("<empty>");
  } // string CAtom::UIFormat()

//------------------------------------------------------------------//
// Client application info
//------------------------------------------------------------------//

/// set user sign
bool CAtom::UserSignSet(long nSign)
  {
  m_nUserSign = nSign;
  TimeStampUpdate();
  return true;
  } // bool CAtom::UserSignSet(long nSign)

/// get user sign
long CAtom::UserSignGet()
  {
  return m_nUserSign;
  } // long CAtom::UserSignGet()

timeval CAtom::TimeStampGet()
  {
  // todo: analysing the Ropes and the ADO ??
  return ( m_tTimeStamp > m_poData->TimeStampGet() ) ? m_tTimeStamp : m_poData->TimeStampGet();
  } // time_t CAtom::TimeStampGet()

long CAtom::BinarySizeGet()
  {
  if ( m_poData == 0 )
    {
    return 0;
    }
  return m_poData->BinarySizeGet();
  } // long CAtom::BinaryDataGet()

long CAtom::BinaryDataGet(void* pData, long nSizeOfBuffer)
  {
  if ( m_poData == 0 )
    {
    return 0;
    }
  return m_poData->BinaryDataGet(pData, nSizeOfBuffer);
  }
