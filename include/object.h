/***************************************************************************
                          odbobject.h  -  description
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

#ifndef __OBJECT_H
#define __OBJECT_H

/**
 * @file 
 * Object.
 */

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "root.h"
#include "class.h"
#include "atom.h"
#include "reason.h"

namespace odb {

/**
 * The map of child relations.
 * holding the child object.A child is an object that is referenced by the
 * parent object for a specific reason. A child link with a specific reason can only
 * estqblished ones per parent object.
 */
typedef std::map <CObject*, CListReason*> CMapChildren;

/**
 * The child relation.
 * holding a specific relation from the current object to one of hist childs.
 * It combines the child with the reason why this child objct is referenced by the
 * current object.
 */
typedef std::pair<CObject*, CListReason*> CPairChild;

/**
 * The backlink to a parent object relation
 * holding the backward relatioon to a parent object. This element is given by the
 * parent object by connecting us as a child object. AT this point we know, that we
 * are referenced by this parent object and we have a count, how often this parent
 * links to us. Each object is allowed to reference each other object multiple time
 * using different reasons.
 */
typedef std::pair<CObject*, long>         CPairParent;

/**
 * The Object.
 * is the representation of a single real world object in the odb. This basic
 * structure is one of the for main components of the odb. An object is able to handle
 * the following things:
 *
 *  - holding 'unlimited' data atoms as object attributes
 *  - keeping relations to an 'unlimited' amount of other objects for specific reasons
 *  - knowing which objects are refrencing to itself
 *  - knowing a template object (OMAL), describing the 'normal' elements of itself
 *  - being assigned to a class that is able to hold a object template (OMAL)
 */
class CObject : public CRoot
  {
    DECLARE_RTTI

  private:
    typedef CRoot inherited;

  protected:
    bool         m_bOmal;      /**< true if ths object is a object template. */

    CClass*      m_poClass;    /**< the class the object is assigned to. */
    CObject*     m_poOmal ;    /**< the "object master atom list", the object layout template. */
    CMapChildren m_moChildren; /**< the list of objectes this object references to. */
    CVectorAtom  m_aoAtoms;    /**< the data containers. */

          /**
           * Copyconstructor.
           */
             CObject(const CObject& src);
  public:
          /**
           * Constructor.
           * @param bOmal true if the object becomes an object master atom list (template).
           */
             CObject(bool bOmal = false);
          /**
           * Constructor.
           * @param lid the intended object's id.
           * @param bOmal true if the object becomes an object master atom list (template).
           */
             CObject(long lid, bool bOmal = false);

          /**
           * Constructor.
           * @param poClass the class the object shall 'inherit' from.
           * @param bOmal true if the object becomes an object master atom list (template).
           */
             CObject(const CClass* poClass, bool bOmal = false);

          /**
           * Constructor.
           * @param sName the object's name.
           * @param bOmal true if the object becomes an object master atom list (template).
           */
             CObject(const std::string& sName, bool bOmal = false);

          /**
           * Constructor.
           * @param sName the object's name.
           * @param poClass the class the object shall 'inherit' from.
           * @param bOmal true if the object becomes an object master atom list (template).
           */
             CObject(const std::string& sName, const CClass* poClass, bool bOmal = false);

          /**
           * Copyconstructor.
           * @param src the object to copy from.
           * @param bOmal true if the object becomes an object master atom list (template).
           */
             CObject(const CObject* src, bool bOmal = false);

          /**
           * Destructor.
           */
    virtual ~CObject();

    /**
     * assignes the object data from the given source object.
     * @param  src the source object that is to copy into the current instance
     * @return a pointer to itself fromt the same type as the input parameter.
     */
    const CObject* operator = (const CObject* src);

    /**
     * create a copy of the object.
     * @return a copy of the current object.
     */
    virtual CRoot* Copy();

    /**
     * assignes the OMAL given.
     * @param  poOMAL get the OMAL from this object.
     * @return the given input parameter.
     */
    virtual const CObject* OmalSet(const CObject* poOMAL);

    /**
     * assigns a class to the object.
     * @param  src the class the object is to assigned to.
     * @return the input parameter.
     */
    const CClass* operator = (const CClass* src);

    /**
     * adds an atom to the atomslist.
     * @param  src the atom to add to the atomlist.
     * @return the input parameter.
     */
    const CAtom* operator += (CAtom* src);
    /**
     * removes an atom from the atomlist.
     * @param  src the atom to remove from the atomlist.
     * @return the input parameter.
     */
    const CAtom* operator -= (CAtom* src);

    /**
     * strores the object instance data to the stream.
     */
    virtual CStream& Save(CStream& oStream);
    /**
     * loads the object instance data from the stream.
     */
    virtual CStream& Load(CStream& oStream);
    /**
     * resolves the old link-pointer (as stored) to the new instance-pointer (as needed) after "load()".
     * this methode needs to explicitely be called after a load operation.
     */
    virtual bool ResolveIDs(CODB& oDB);

    /**
     * creates a parent link to an object instance.
     * @param poChildObject the child object to link to.
     * @param poReason the reason for the connection.
     * @return true if successfull. this means, the link could be established or existed before.
     */
    virtual bool Link       (const CObject* poChildObject, const CReason* poReason);
    /**
     * removes a parent link to an object instance.
     * @param poChildObject the child object to which the link shall be removed.
     * @param poReason the reason of the link to remove (there can be multiple).
     * @return true if successfull. this means, "anything was realy removed".
     */
    virtual bool LinkRemove (const CObject* poChildObject, const CReason* poReason);

    /**
     * adds an atom to the atomslist.
     * @param poAtom the atom to be added.
     * @param lOrdinal the GUI-ordinal number if any (typical not set).
     * @return the added atom.
     */
    virtual const CAtom* AtomAdd(CAtom* poAtom, long lOrdinal = -1);
    /**
     * removes an atom from the atomlist.
     * @param poAtom the atom to be removed.
     * @return the removed atom.
     */
    virtual const CAtom* AtomRemove(CAtom* poAtom);

    /**
     * Get the Atom-Vector.
     * @return the object's atom-vector.
     */
    virtual const CVectorAtom& AtomsGet() const;
    /**
     * Get the Atom by Index.
     * @return the Atom by index, CAN RETURN "NULL".
     */
    virtual CAtom*  AtomGet(long nIndex);
    /**
     * Get the Atoms by user-sign.
     * @param nSigh the user-sign the atom looked for is marked with.
     * @param nSignMask the mask.
     * @return the Atoms marked with the user-sign.
     */
    virtual CVectorAtom AtomGetBySign(long nSign, long nSignMask = 0xFFFFFFFFL );
    /**
     * Get a list of Atoms found by Name.
     * @param sName the name to search the atoms.
     * @param bPartial TODO.
     * @return a list of Atoms found using sName.
     */
    virtual CVectorAtom AtomGet(const std::string& sName, bool bPartial = false);
    /**
     * Get the amount of atoms in the object.
     * @return the number of atoms.
     */
    virtual long AtomCountGet() const;

    /**
     * returns the Child-Vector
     */
    virtual const CMapChildren& ChildrenGet() const;
    /**
     * returns the Child by index, CAN RETURN "NULL".
     * @param nIndex the index of the child.
     */
    virtual CObject*  ChildGet(long nIndex);
    /**
     * returns the Child-Pair by index. PAIR CAN CONTAIN "0".
     * @param nIndex the index of the pair.
     */
    virtual CPairChild ChildPairGet(long nIndex);
    /**
     * returns the amount of Childs in the object.
     */
    virtual long ChildCountGet() const;


    /**
     * returns the Parent-Vector.
     */
    virtual const CMapReferencing& ParentsGet() const;
    /**
     * returns the Parent by index, CAN RETURN "NULL".
     * @param the index of the parent.
     */
    virtual CObject* ParentGet(long nIndex);
    /**
     * returns the Parent-Pair by index. PAIR CAN CONTAIN "0".
     * @param nIndex the index of the parent-pair.
     */
    virtual CPairParent ParentPairGet(long nIndex);
    /**
     * returns the amount of Parents in the object.
     */
    virtual long ParentCountGet() const;

    /**
     * sets the "logical class" for this object.
     */
    CClass* ClassSet(const CClass* poClass);
    /**
     * returns the "logical class" of this object.
     */
    CClass* ClassGet() const;
    /**
     * dumps the object (mainly for debugging).
     */
    virtual void Dump();

  protected:
    /**
     * called from the constructor(s) to initialize all member variable..
     */
    void InitBasics(bool bOmal);
  }; // class CObject

} // namespace odb

#endif // __OBJECT_H
