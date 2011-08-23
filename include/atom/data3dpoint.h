/***************************************************************************
                          data3dpoint.h
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

#ifndef __ATOMDATA_3D_POINT_H
#define __ATOMDATA_3D_POINT_H

/**
 * @file
 * AtomData 3D Point.
 */

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "atom/data.h"

namespace odb {

/**
 * The 3D point structure.
 * stores the coordinates of a mathimatical 3D point
 */
typedef
  struct tagG3DPOINT
    {
    double p[3]; // x, y, z
    } G3DPOINT;

/**
 * The 3D-Point.
 * a non-simple data class for an atom.
 */
class CAtomData3DPoint : public CAtomData
  {
  private:
    // inheritance
    typedef CAtomData inherited;

  public:
          /**
           * Constructor.
           */
             CAtomData3DPoint();

          /**
           * Constructor.
           * @param rst3DPoint coordinates the atom shall represent.
           */
             CAtomData3DPoint(G3DPOINT &rst3DPoint);

          /**
           * Constructor.
           * @param dX X-coordinate the atom shall represent.
           * @param dY Y-coordinate the atom shall represent.
           * @param dZ Z-coordinate the atom shall represent.
           */
             CAtomData3DPoint(double dX, double dY, double dZ);

          /**
           * Copyconstructor.
           * @param src the other atom to be copied from.
           */
             CAtomData3DPoint(CAtomData3DPoint& src);

          /**
           * Destructor.
           */
    virtual ~CAtomData3DPoint();

    virtual long Type() { return DT_G3DPOINT; }

            CAtomData3DPoint& operator = (CAtomData3DPoint& src);
            CAtomData3DPoint& operator = (G3DPOINT& src);
            
    virtual inherited* Copy();

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
    G3DPOINT m_Data;
  }; // class CAtomData3DPoint : public inherited

} // namespace odb

#endif // __ATOMDATA_3D_POINT_H
