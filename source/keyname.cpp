/***************************************************************************
                          keyname.cpp  -  description
                             -------------------
    begin                : Thu Mar 23 2000
    copyright            : (C) 2000..2005 by Manfred Morgner
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
#include "keyname.h"

using namespace odb;

CKeyName::CKeyName(const std::string& sName)
  : m_sName(sName)
  {
  } // CKeyName::CKeyName(const string& sName)

CKeyName::~CKeyName()
  {
  } // CKeyName::~CKeyName()

CVectorRoot::iterator CKeyName::find(CRoot* poItem)
  {
  for ( iterator it = begin(); it != end(); ++it )
    {
    if ( *it == poItem )
      {
      return it;
      }
    }
  return end();
  } // CKeyName::iterator CKeyName::find(CRoot* poItem)
