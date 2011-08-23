/***************************************************************************
                          reason.h  -  description
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

#ifndef __REASON_H
#define __REASON_H

/**
 * @file 
 * Reason.
 */

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "root.h"
#include "class.h"

namespace odb {

class CObject;

/**
 * The Reason.
 */
class CReason : public CRoot  
  {
    DECLARE_RTTI

  private:
    typedef CRoot inherited;

  protected:
    CClass* m_poClassParent;
    CClass* m_poClassChild;

             CReason(CReason& src);
  public:
             CReason();
             CReason(long lid);
             CReason(const std::string& sName, CReason& src);
             CReason(const std::string& sName, const CClass*  poClassParent,  const CClass*  poClassChild);
             CReason(const std::string& sName, const CObject* poObjectParent, const CObject* poObjectChild);
    virtual ~CReason();


            CReason& operator = (CReason& src);
    virtual CRoot* Copy();

    virtual CStream& Save(CStream& oStream);
    virtual CStream& Load(CStream& oStream);
    /// resolves the old link-pointer (as stored) to the new instance-pointer (as needed) after "load()"
    virtual bool ResolveIDs(CODB& oDB);

    virtual bool Check(const CObject* poParent, const CObject* poChild) const;

  protected:
    /// called from the constructor(s) to initialize all member variable

    void InitBasics();
  };

  } // namespace odb

#endif // __REASON_H
