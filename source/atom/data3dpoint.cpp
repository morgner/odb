/***************************************************************************
                          data3dpoint.cpp
                          -------------------
    begin                : Fri Mar 3 2000
    copyright            : (C) 1993..2011 by Manfred Morgner
    email                : manfred@morgner.com
 ***************************************************************************/


#include "generic.h"

#include "atom/data3dpoint.h"

using namespace odb;

//------------------------------------------------------------------//
// Construction/Destruction
//------------------------------------------------------------------//

CAtomData3DPoint::CAtomData3DPoint()
  : inherited()
  {
  InitBasics();
  } // CAtomData3DPoint::CAtomData3DPoint()

CAtomData3DPoint::CAtomData3DPoint(G3DPOINT& src)
  : inherited()
  {
  InitBasics();
  *this = src;
  m_Data = src;
  } // CAtomData3DPoint::CAtomData3DPoint(G3DPOINT& src)
  
CAtomData3DPoint::CAtomData3DPoint(double dX, double dY, double dZ)
  : inherited()
  {
  InitBasics();
  m_Data.p[0] = dX;
  m_Data.p[1] = dY;
  m_Data.p[2] = dZ;
  } // CAtomData3DPoint::CAtomData3DPoint(double dX, double dY, double dZ)
  
CAtomData3DPoint::CAtomData3DPoint(CAtomData3DPoint& src)
  : inherited(src)
  {
  InitBasics();
  *this = src;
  } // CAtomData3DPoint::CAtomData3DPoint(CAtomData3DPoint& src)

CAtomData3DPoint::~CAtomData3DPoint()
  {
  } // CAtomData3DPoint::~CAtomData3DPoint()

void CAtomData3DPoint::InitBasics()
  {
  m_Data.p[0] = 0.0;
  m_Data.p[1] = 0.0;
  m_Data.p[2] = 0.0;
  } // CAtomData3DPoint::InitBasics()

CAtomData3DPoint& CAtomData3DPoint::operator = (CAtomData3DPoint& src)
  {
  if (&src == this)
    {
    return src;
    }
  TimeStampUpdate();
  m_Data.p[0] = src.m_Data.p[0];
  m_Data.p[1] = src.m_Data.p[1];
  m_Data.p[2] = src.m_Data.p[2];
  return *this;
  } // CAtomData3DPoint& CAtomData3DPoint::operator = (CAtomData3DPoint& src)


CAtomData3DPoint& CAtomData3DPoint::operator = (G3DPOINT& src)
  {
  m_Data = src;
  return *this;
  } // CAtomData3DPoint& CAtomData3DPoint::operator = (G3DPOINT& src)


CAtomData* CAtomData3DPoint::Copy()
  {
  return new CAtomData3DPoint(*this);
  } // inherited* CAtomData3DPoint::Copy()

CStream& CAtomData3DPoint::Save(CStream& oStream)
  {
  oStream << m_Data.p[0];
  oStream << m_Data.p[1];
  oStream << m_Data.p[2];

  return inherited::Save(oStream);
  } // CStream& CAtomData3DPoint::Save(CStream& oStream)

CStream& CAtomData3DPoint::Load(CStream& oStream)
  {
  oStream >> m_Data.p[0];
  oStream >> m_Data.p[1];
  oStream >> m_Data.p[2];

  return inherited::Load(oStream);
  } // CStream& CAtomData3DPoint::Load(CStream& oStream)

std::string CAtomData3DPoint::UIFormat(const std::string& sFormat)
  {
  char asz[64];

  if ( sFormat.length() == 0 )
    {
    sprintf(asz, "<%.3f; %.3f;%.3f>", m_Data.p[0], m_Data.p[1], m_Data.p[2]);
    }
  else
    {
    sprintf(asz, sFormat.c_str(), m_Data);
    }
  return asz;
  } // CString CAtomData3DPoint::UIFormat(const CString& sFormat)

long CAtomData3DPoint::BinarySizeGet()
  {
  return sizeof(m_Data);
  } // long CAtomData3DPoint::BinarySizeGet()

const void* CAtomData3DPoint::BinaryBufferGet()
  {
  return &m_Data;
  } // void* CAtomData3DPoint::BinaryBufferGet()
