/***************************************************************************
                        transaction.cpp  -  description
                             -------------------
    begin                : Fri Mar 3 2000
    copyright            : (C) 1993..2005 by Manfred Morgner
    email                : manfred@morgner.com
 ***************************************************************************/


#include "generic.h"

#include "transaction.h"

using namespace odb;

//------------------------------------------------------------------//
// Construction/Destruction
//------------------------------------------------------------------//

CTransaction::CTransaction(CRoot* poCaller,
                           CRoot* poObject )
  {

  m_poCaller   = poCaller;
  m_poObject   = poObject;
  m_poSnapShot = new CSnapShot(poObject);

  } // CTransaction::CTransaction(const CRoot* poCaller, const CRoot* poObjectOriginal )

CTransaction::~CTransaction()
  {
  } // CTransaction::~CTransaction()

long CTransaction::Open()
  {
  return ODB_FAILURE;
  } // long CTransaction::Open()

long CTransaction::Commit()
  {
  return ODB_FAILURE;
  } // long CTransaction::Commit()

long CTransaction::Abort()
  {
  return ODB_FAILURE;
  } // long CTransaction::Abort()
