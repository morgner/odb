/***************************************************************************
                        transaction.cpp  -  description
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
