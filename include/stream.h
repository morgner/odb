/***************************************************************************
                          stream.h  -  description
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

#ifndef __STREAM_H
#define __STREAM_H

/**
 * @file
 * Stream.
 */

#include <fstream>
#include <deque>

namespace odb {

class CRoot;

/**
 * The Stream.
 * gives an abstract destination where to write data to or where to read from.
 * This implementation contains only basic function and operations. May be there
 * are some specialized stream classes coming.
 */
class CStream
  {
  protected:
    std::deque<std::string> m_qsOpenElements;
    int                     m_nInsertLevel;
/*
    FILE* fopen( sFileName.c_str(), "wb" );
    FILE* fopen( sFileName.c_str(), "rb" );
*/
    std::fstream m_oFile;
//    stream m_poFile;

  public:
             CStream();
             CStream(const std::string& sFileName, bool bWrite = false);
    virtual ~CStream();

    bool Seek(long lPosition);

    // reading operators
    CStream& operator >> (     uint32_t& data); /**< Reads a 'uint32_t' values from the stream */
    CStream& operator >> (     long&     data); /**< Reads a 'long' values from the stream */
    CStream& operator >> (     bool&     data); /**< Reads a 'bool' values from the stream */
    CStream& operator >> (odb::CRoot&    data); /**< Reads a another object from the stream */
    CStream& operator >> (std::string&   data); /**< Reads a std::string from the stream */
    CStream& operator >> (     double&   data); /**< Reads a 'double' values from the stream */
    CStream& operator >> (     timeval&  data); /**< Reads a 'timeval' values from the stream */

    CStream& Read(void* pData, unsigned long ulSize); /**< Reads binary data from the stream */

/*
    template<typename T_to_put>
      friend CStream& operator << (CStream& s, T_to_put p) { return s << p; };
*/

    /// writing operators
    CStream& operator << (           uint32_t data); /**< Writes a 'uint32_t' values to the stream */
    CStream& operator << (           long     data); /**< Writes a 'long' values to the stream */
//    CStream& operator << (           bool     data);  /**< Writes a 'bool' values to the stream */
    CStream& operator << (      odb::CRoot&   data); /**< Writes another object to the stream */
    CStream& operator << (const std::string&  data); /**< Writes any string to the stream */
    CStream& operator << (const      char*    data); /**< Writes any string to the stream */
    CStream& operator << (           double   data); /**< Writes a 'double' values to the stream */
    CStream& operator << (           timeval  data); /**< Writes a 'timeval' values to the stream */

    CStream& Write(void* pData, unsigned long ulSize); /**< Writes binary data to the stream */

    CStream& TabInsert();
    CStream& Open(const std::string& crsName);
    CStream& Close();
    CStream& Element(const std::string& crsName,            long    data);
//    CStream& Element(const std::string& crsName,            bool    data);
    CStream& Element(const std::string& crsName,       odb::CRoot&  data);
    CStream& Element(const std::string& crsName, const std::string& data);
    CStream& Element(const std::string& crsName,            double  data);
    CStream& Element(const std::string& crsName,            timeval data);

  };

} // namespace odb

#endif // __ODB_STREAM_H
