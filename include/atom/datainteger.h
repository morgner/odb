/***************************************************************************
                          datainteger.h
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

#ifndef __ATOMDATA_INTEGER_H
#define __ATOMDATA_INTEGER_H


/**
 * @file 
 * AtomData Integer.
 */

#include "atom/data.h"

namespace odb {

/**
 * The Integer Atom-Data.
 */
class CAtomDataInteger : public CAtomData
  {
  private:
    typedef CAtomData inherited;

  public:
          /** 
           * Constructor.
           */
             CAtomDataInteger();

          /** 
           * Copyconstructor.
           * @param src the atom to be copied from.
           */
             CAtomDataInteger(CAtomDataInteger& src);

          /**
           * Constructor.
           * @param src the integer the atom shall represent.
           */
             CAtomDataInteger(long src);

          /**
           * Destructor.
           */
    virtual ~CAtomDataInteger();

    virtual long Type() { return DT_INTEGER; }

            CAtomDataInteger& operator = (CAtomDataInteger& src);
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
	  long m_Data;
  };

} // namespace odb

#endif // __ATOMDATA_INTEGER_H
