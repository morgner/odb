/***************************************************************************
                          keyname.h  -  description
                             -------------------
    begin                : Thu Mar 23 2000
    copyright            : (C) 2000 by Manfred Morgner
    email                : manfred@morgner.com
    $Id: keyname.h,v 1.4 2004/04/02 05:07:01 morgner Exp $
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

#ifndef __KEY_NAME_H
#define __KEY_NAME_H

/**
 * @file
 * KeyName.
 */

#include "generic.h"

// STL
#include <vector>

namespace odb {

class CRoot;

/**
 * A simple vector of 'things'
 * to collect things, extracted from the odb.
 */
typedef std::vector<CRoot*> CVectorRoot;

/**
 * The KeyName Class.
 * A class, that provides an 2D-Index from "Names" of whatever to instances of whatever.
 */
class CKeyName : public CVectorRoot
  {
  private:
    typedef CVectorRoot inherited;

  protected:
    /** 
     * Constructor.
     */
     CKeyName() {};
  public:

     /**
      * Constructor.
      * @param sName the name.
      */
     CKeyName(const std::string& sName);

     /**
      * Destructor.
      */
    ~CKeyName();

    CVectorRoot::iterator find(CRoot* poItem);

    operator const std::string& () const { return m_sName; }

  protected:
    std::string m_sName;
  }; // class CKeyName

struct KeyName_less
  {
  bool operator()(const CKeyName* poKey1, const CKeyName* poKey2) const
    {
    return ((std::string)*poKey1) < ((std::string)*poKey2);
    }
  }; // struct KeyName_less

typedef std::set<CKeyName*, KeyName_less> CKeyNameIndex;

  } // namespace odb

#endif // __KEY_NAME_H
