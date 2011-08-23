/***************************************************************************
                          datareal.h
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

#ifndef __ATOMDATA_REAL_H
#define __ATOMDATA_REAL_H

/**
 * @file 
 * Atomdata Real TODO.
 */

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "atom/data.h"

namespace odb {

class CAtomDataReal : public CAtomData  
  {
  private:
    typedef CAtomData inherited;

  public:
             CAtomDataReal();
             CAtomDataReal(CAtomDataReal& src);
             CAtomDataReal(double src);
    virtual ~CAtomDataReal();

    virtual long Type() { return DT_REAL; }

            CAtomDataReal& operator = (CAtomDataReal& src);
    virtual CAtomData* Copy();

    virtual CStream& Save(CStream& oStream);
    virtual CStream& Load(CStream& oStream);

    virtual std::string UIFormat(const std::string& sFormat);

    virtual long BinarySizeGet();

  protected:
    virtual const void* BinaryBufferGet();

  protected:
    /// called from the constructor(s) to initialize all member variable
    void InitBasics();

  protected:
	  double m_Data;
  };

} // namespace odb

#endif // __ATOMDATA_REAL_H
