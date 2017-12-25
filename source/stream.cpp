/***************************************************************************
                           stream.cpp
                           -------------------
    begin                : Fri Mar 3 2000
    copyright            : (C) 1993..2011 by Manfred Morgner
    email                : manfred@morgner.com
 ***************************************************************************/


#include "generic.h"

#include "stream.h"
#include "root.h"

// #include <iostream> // std:err

using namespace odb;

//------------------------------------------------------------------//
// Construction/Destruction
//------------------------------------------------------------------//

CStream::CStream()
  {
  }

CStream::CStream(const std::string& sFileName, bool bWrite)
  {
  try
    {
    if (bWrite)
      {
      m_oFile.open(sFileName.c_str(), std::ios::out | std::ios::binary);
      }
    else
      {
      m_oFile.open(sFileName.c_str(), std::ios::in | std::ios::binary);
      }
    }
  catch (...) // (CFileException* e)
    {
    std::cerr << "ERROR: Can't open file\n";
    }
  } // CStream::CStream(const std::string& sFileName, bool bWrite)

CStream::~CStream()
  {
  while ( m_qsOpenElements.size() ) { Close(); };
  try
    {
    m_oFile.close();
    }
  catch (...)
    {
    std::cerr << "ERROR: Can't close file\n";
    }
  } // CStream::~CStream()

bool CStream::Seek(long lPosition)
  {
  try
    {
    m_oFile.seekg(lPosition);
    }
  catch (...)
    {
//  e->Delete();
    return false;
    }
  return true;
  } // bool CStream::Seek(long lPosition)

CStream& CStream::Read(void* pData, unsigned long ulSize)
  {
  try
    {
    m_oFile.read( (char*)pData, ulSize );
    }
  catch (...)
    {
//    e->Delete();
    }
  return *this;
  } // bool CStream::Read(void* pData, unsigned long ulSize)

CStream& CStream::Write(void* pData, unsigned long ulSize)
  {
  try
    {
    m_oFile.write( (char*)pData, ulSize );
    }
  catch (...)
    {
//    e->Delete();
    }
  return *this;
  } // bool CStream::Write(void* pData, unsigned long ulSize)

CStream& CStream::operator >> (uint32_t& data)
  {
  try
    {
    Read(&data, sizeof(data));
    }
  catch (...)
    {
    //    e->Delete();
    }
  return *this;
  }

CStream& CStream::operator << (uint32_t data)
  {
  try
    {
    Write(&data, sizeof(data));
    }
  catch (...)
    {
    //    e->Delete();
    }
  return *this;
  }

CStream& CStream::operator >> (long& data)
  {
  try
    {
    Read(&data, sizeof(data));
    }
  catch (...)
    {
//    e->Delete();
    }
  return *this;
  }

CStream& CStream::operator << (long data)
  {
  try
    {
    Write(&data, sizeof(data));
    }
  catch (...)
    {
//    e->Delete();
    }
  return *this;
  }

CStream& CStream::operator >> (bool& data)
  {
  try
    {
    Read(&data, sizeof(data));
    }
  catch (...)
    {
//    e->Delete();
    }
  return *this;
  }

/*
CStream& CStream::operator << (bool data)
  {
  try
    {
    Write(&data, sizeof(data));
    }
  catch (...)
    {
//    e->Delete();
    }
  return *this;
  }
*/

CStream& CStream::operator >> (CRoot& data)
  {
  return data.Load(*this);
  }

CStream& CStream::operator << (CRoot& data)
  {
  *this << "<thing>\n";
  /* return */ data.Save(*this);
  return *this << "</thing>\n";
  }

CStream& CStream::operator >> (std::string& data)
  {
  try
    {
    long size;
    Read(&size,  sizeof(size));
    char* psz = new char[size];
    Read(psz, size);
    data = psz;
    delete [] psz;
    }
  catch (...)
    {
//    e->Delete();
    }
  return *this;
  }

/*
CStream& CStream::operator << (const char* data)
  {
  return operator << ( (const std::string&) data);
  }
*/

CStream& CStream::operator << (const std::string& data)
  {
  try
    {
    static char c0 = 0;
    long size = data.length()+1;
//    Write(&size, sizeof(size));
    Write((void*)data.c_str(), size-1);
    Write((void*)&c0, 1);
    }
  catch (...)
    {
//    e->Delete();
    }
  return *this;
  }

CStream& CStream::operator >> (double& data)
  {
  try
    {
    Read(&data, sizeof(data));
    }
  catch (...)
    {
//    e->Delete();
    }
  return *this;
  }

CStream& CStream::operator << (double data)
  {
  try
    {
    Write(&data, sizeof(data));
    }
  catch (...)
    {
//    e->Delete();
    }
  return *this;
  }

CStream& CStream::operator >> (timeval& data)
  {
  try
    {
    Read(&data, sizeof(data));
    }
  catch (...)
    {
//    e->Delete();
    }
  return *this;
  }

CStream& CStream::operator << (timeval data)
  {
  try
    {
    Write(&data, sizeof(data));
    }
  catch (...)
    {
//    e->Delete();
    }
  return *this;
  }


CStream& CStream::TabInsert()
  {
  if ( m_qsOpenElements.size() > 0 )
    {
    std::string s;
    for (int n = m_qsOpenElements.size(); n > 0; --n)
      {
      s += "  ";
      }
    *this << s;
    }
  return *this;
  } // CStream& CStream::TabInsert()

CStream& CStream::Open(const std::string& crsName)
  {
  TabInsert();
  m_qsOpenElements.push_front(crsName);
  return *this << "<" << crsName << ">\n";
  } // 

CStream& CStream::Close()
  {
  if ( m_qsOpenElements.size() > 0 )
    {
    TabInsert();
    *this << "</" << m_qsOpenElements.front() << ">\n";
    m_qsOpenElements.pop_front();
    }
  return *this;
  } // 

CStream& CStream::Element(const std::string& crsName, long data)
  {
  char ac[1024];
  snprintf (ac, sizeof(ac), "%ld", data);
  return Element(crsName, ac);
  } // 

CStream& CStream::Element(const std::string& crsName, uint32_t data)
  {
  char ac[1024];
  snprintf (ac, sizeof(ac), "%ld", data);
  return Element(crsName, ac);
  } // 

/*
CStream& CStream::Element(const std::string& crsName, bool data)
  {
  TabInsert();
  std::string s = (data) ? "true" : "false";
  return *this << "<" << crsName << ">" << s << "</" << crsName << ">\n";
  } // 
*/

CStream& CStream::Element(const std::string& crsName, odb::CRoot& data)
  {
  TabInsert();
  *this << "<" << crsName << ">\n";
  data.Save(*this);
  return *this << "</" << crsName << ">\n";
  } // 

CStream& CStream::Element(const std::string& crsName, const std::string& data)
  {
  TabInsert();
  return *this << "<" << crsName << ">" << data << "</" << crsName << ">\n";
  } //

CStream& CStream::Element(const std::string& crsName, double data)
  {
  char ac[1024];
  snprintf (ac, sizeof(ac), "%f", data);
  return Element(crsName, ac);
  } // 

CStream& CStream::Element(const std::string& crsName, timeval data)
  {
  char ac[64];
  snprintf (ac, sizeof(ac), "%ld:%ld", data.tv_sec, (long)data.tv_usec);
  return Element(crsName, std::string(ac));
  } // 
