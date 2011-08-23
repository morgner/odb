/***************************************************************************
                          class.h  -  description
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

#ifndef __CLASS_H
#define __CLASS_H

/**
 * @file
 * The Class.
 */

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "root.h"

namespace odb {

class CObject;

/**
 * The Class.
 */
class CClass : public CRoot  
  {
    DECLARE_RTTI

  private:
    typedef CRoot inherited;

  protected:
    CObject*  m_poOMAL; /**< object master atom list (master object). */

          /**
           * Copyconstructor.
           * @param src the class to be copied from.
           */
             CClass(CClass& src);
  public:
          /**
           * Constructor.
           */
             CClass();

          /**
           * Constructor.
           * @param lid an intended id for the class.
           */
             CClass(long lid);

          /**
           * Constructor.
           * @param the class' name.
           * @param poOMAL a pointer to the object master atom list.
           */
             CClass(const std::string& sName, CObject* poOMAL = 0);

          /** 
           * Destructor.
           */
    virtual ~CClass();

            CClass* operator = (const CClass* src);
    virtual CRoot* Copy();

    virtual CStream& Save(CStream& oStream);
    virtual CStream& Load(CStream& oStream);
    /**
     * resolves the old link-pointer (as stored) to the new instance-pointer (as needed) after "load()".
     * @param oDB the database where to do that.
     */
    virtual bool        ResolveIDs(CODB& oDB);

    virtual CObject* OmalGet() const ;
    virtual bool        OmalSet( const CObject* poOMAL, bool bOverwrite = false );

            /**
             * inserts or replaces the OMAL for this class.
             */
            CObject* operator = (const CObject* poOMAL);
            /**
             * returns the OMAL for this class.
             */
                     operator CObject*() const;

    /**
     * sets the "parent class" for this class..
     * @param poClass the parent class.
     * @return TODO.
     */
    CClass* ClassSet(const CClass* poClass);
    
    /**
     * Get the parent-class.
     * @return the "parent class" of this class.
     */
    CClass* ClassGet() const;

  protected:
    /**
     * called from the constructor(s) to initialize all member variable.
     */
    void InitBasics();

  protected:
    /**
     * the "parent class".
     */
    CClass* m_poClass;
  }; // class CClass

  } // namespace odb

#endif // __ODB_CLASS_H
