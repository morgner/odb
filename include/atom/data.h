/***************************************************************************
                          data.h
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

#ifndef __ATOMDATA_H
#define __ATOMDATA_H

/**
 * @file
 * Atomdata.
 */

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "stream.h"

#ifdef WIN32
  #include "winsock.h"
  #else
  #include <sys/time.h>
  #endif // WIN32

// for sprintf(..) in method "UIFormat(..)"
#include "stdio.h"

namespace odb {

#define DT_NONE     0x0000
#define DT_INTEGER  0x0001
#define DT_REAL     0x0002
#define DT_STRING   0x0003
#define DT_FILE     0x0004
#define DT_FILELINK 0x0005
#define DT_G3DPOINT 0x0006
#define DT_BINARY   0x0007


/**
 * The AtomData Class.
 */
class CAtomData
  {
  protected:
    static std::string s_sEmptyString;

  protected:
    timeval  m_tTimeStamp;

  public:
                
            /**
             * Constructor.
             */
	           CAtomData();
               
            /**
             * Copyconstructor.
             * @param src the atomdata to be copied from.
             */
	           CAtomData(CAtomData& src);
               
            /**
             * Destructor.
             */
	  virtual ~CAtomData();

            /**
             * Get the Type of AtomData.
             * @return the Type.
             */
    virtual long Type() { return DT_NONE; }

            CAtomData& operator = (CAtomData& src);
    virtual CAtomData* Copy();
    virtual CAtomData& Assign(CAtomData& src);

    virtual CStream& Save(CStream& oStream);
    virtual CStream& Load(CStream& oStream);

    virtual std::string UIFormat(const std::string& sFormat);

    virtual timeval TimeStampGet();
    virtual bool    TimeStampUpdate();

    /**
     * copies the data to the given buffer - if possible.
     * @return the size of the data in octets (8bit - bytes).
     */
    virtual long BinaryDataGet(void* pData, long nSizeOfBuffer);
    virtual long BinarySizeGet();

  protected:
    virtual const void* BinaryBufferGet();

  protected:
    /**
     * called from the constructor(s) to initialize all member variable.
     */
    void InitBasics();

  };

} // namespace odb

#endif // __ATOMDATA_H
