#ifndef __TRANSACTION_H
#define __TRANSACTION_H

/***************************************************************************
                           transaction.h
                           -------------------
    begin                : Fri Mar 3 2000
    copyright            : (C) 1993..2011 by Manfred Morgner
    email                : manfred@morgner.com
 ***************************************************************************/


/**
 * @file
 * Transaction.
 */

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
          }

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
 // __TRANSACTION_H
#endif
