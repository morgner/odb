/***************************************************************************
                        transaction.h  -  description
                             -------------------
    begin                : Fri Mar 3 2000
    copyright            : (C) 1993..2000 by Manfred Morgner
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

#ifndef __TRANSACTION_H
#define __TRANSACTION_H


/**
 * @file
 * Transaction.
 */
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "root.h"
#include "class.h"
#include "atom.h"
#include "object.h"
#include "reason.h"

namespace odb {

class CTransaction  
  {
  protected:
    class CSnapShot;

    const CRoot*  m_poCaller;
    const CRoot*  m_poObject;
          CSnapShot* m_poSnapShot;

  public:
             CTransaction( CRoot* poCaller = 0,
                              CRoot* poObject = 0 );
    virtual ~CTransaction();

    long Open  ();
    long Commit();
    long Abort ();

  protected:
    class CSnapShot
      {
      protected:
              CRoot*  m_poObject;
        const CSnapShot* m_poPrev;


      public:
        CSnapShot( CRoot* poObject, const CSnapShot* poPrev = 0 )
          {
          m_poObject = poObject->Copy();
          m_poPrev   = poPrev;
          }

        ~CSnapShot()
          {
          delete m_poObject;
          };

        const CSnapShot* Previouse()
          {
          return m_poPrev;
          }

        long Append( const CRoot* poObject, const CSnapShot* poPrev )
          {
          return ODB_FAILURE;
          } // CTransaction::CSnapShot::Add()

        long Abort( CRoot* poObject )
          {
          if (poObject)
            {
            *m_poObject = *poObject;
            }
          return ODB_FAILURE;
          } // CTransaction::CSnapShot::Add()


      }; // class CSnapShot
  }; // class CTransaction

} // namespace odb

#endif // __TRANSACTION_H
