/***************************************************************************
                          datafile.h
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

#ifndef __ATOMDATA_FILE_H
#define __ATOMDATA_FILE_H

/**
 * @file
 * File-AtomData.
 */

#include "atom/data.h"

namespace odb {

/**
 * The File-AtomData.
 */
class CAtomDataFile : public CAtomData  
  {
  private:
    typedef CAtomData inherited;

  public:
          /**
           * Constructor.
           */
             CAtomDataFile();

          /**
           * Copyconstructor.
           * @param src the atom to be copied from.
           */
             CAtomDataFile(CAtomDataFile& src);
// ??        CAtomDataFile(const fstream& src);

          /**
           * Destructor.
           */
    virtual ~CAtomDataFile();

    virtual long Type() { return DT_FILE; }

            CAtomDataFile& operator = (CAtomDataFile& src);
    virtual CAtomData* Copy();

    virtual CStream& Save(CStream& oStream);
    virtual CStream& Load(CStream& oStream);

    virtual std::string UIFormat(const std::string& sFormat);

    virtual long BinarySizeGet();

  protected:
    virtual const void* BinaryBufferGet();

  protected:
    /**
     * called from the constructor(s) to initialize all member variable.
     */
    void InitBasics();

  protected:
	unsigned char*    m_Data;           /**< binary data */
	std::string       m_sFileName;      /**< original file name */
	std::string       m_sFilePath;      /**< original file path */
    long            m_nFileSize;      /**< original file size */
    long            m_nSize;          /**< size of m_Data */
    time_t          m_tCreation;      /**< date of creation */
    time_t          m_tModification;  /**< date of last modification */
  };

} // namespace odb

#endif // __ATOMDATA_FILE_H
