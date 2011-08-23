/***************************************************************************
                           rtti.h  -  description
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

#ifndef __RTTI_H
#define __RTTI_H

/**
 * @file
 * Runtime Type Information.
 */

#include <string>


namespace odb {

#ifndef _T
  #define _T(s) (const std::string&) s
#endif

/**
 * The RTTI (runtime type information).
 * informs the 'visitor' of a generic object about the real type of the visited object.
 * This is the only portable way to get information about objects in a polymorphic
 * container.
 *
 * In some cases a list of object, all type casted to CRoot, is given as the result of
 * a methode call. The RTTI is the only way to find out what of what class a certain
 * object realy is.
 *
 * The RTTI is instanciated ones per class and each member of a class knows his RTTI record.
 */
typedef
  struct tagObjectInfo
    {
         long   lRtti;      /**< the RTTI of the object instance itself */
         long   lRttiRoot;  /**< the RTTI of the object, 'self' is inherited from */
    std::string sClassName; /**< the C++ class name for the object (e.g. CReason) */
         long   lRelease;   /**< the implementation release */
         long   lVersion;   /**< the implementation version */

    /// setting up the values, all at ones.
		tagObjectInfo(long c, long p, const std::string& psz, long r, long v)
      : lRtti(c),
        lRttiRoot(p),
        sClassName(psz),
        lRelease(r),
        lVersion(v)
      { /* all done outside */};

    } OBJECTINFO;

/**
 * RTTI Declaration Macro.
 * Declares the RTTI 'infrastructure' for the objects. This includes the data structure
 * and the acces mothodes for the RTTI elements.
 *
 * The methods need to be declared 'virtual' to ensure polymorphic access accesses the
 * correct information.
 */
#define DECLARE_RTTI\
  public:\
    static const OBJECTINFO s_stObjectInfo;\
    virtual long               InfoGetRtti()      const { return s_stObjectInfo.lRtti; }\
    virtual long               InfoGetRttiRoot()  const { return s_stObjectInfo.lRttiRoot; }\
    virtual const std::string& InfoGetClassName() const { return s_stObjectInfo.sClassName; }\
    virtual long               InfoGetRelease()   const { return s_stObjectInfo.lRelease; }\
    virtual long               InfoGetVersion()   const { return s_stObjectInfo.lVersion; }

/**
 * Instanciation of a specific RTTI information set.
 * used ones in each class implementation file.
 * @param c (in ##c and #c) ClassName / RTTI
 * @param p (in ##p) Parent Object (inherited from)
 * @param r Release
 */
#define IMPLEMENT_RTTI(c, p, r)\
  const OBJECTINFO odb::C##c::s_stObjectInfo( _##c, _##p, #c, r, 0x0701 );

// object rtti
#define _None      0x00000000

// inherited <none>
#define _Root      0x00000001

// inherited CODBRoot
#define _Class     0x00000101
#define _Object    0x00000201
#define _Atom      0x00000401

#define _Reason    0x00000801
#define _Rope      0x00001001

} // namespace odb

#endif // __RTTI_H
