/***************************************************************************
                          odbobject.cpp  -  description
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
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "generic.h"

#include "object.h"
#include "odb.h"

using namespace odb;

//------------------------------------------------------------------//
// Construction/Destruction
//------------------------------------------------------------------//

IMPLEMENT_RTTI(Object, Root, 0x00980430)

/// constructor
CObject::CObject(bool bOmal)
  :inherited()
  {
  InitBasics(bOmal);
  } // CObject::CObject(bool bOmal)

CObject::CObject(long lid, bool bOmal)
  :inherited(lid)
  {
  InitBasics(bOmal);
  } // CObject::CObject(long lid, bool bOmal)

/// constructor
CObject::CObject(const CClass* poClass, bool bOmal)
  :inherited()
  {
  InitBasics(bOmal);
  ClassSet(poClass);
  // ensure the assignment does not overwrite "bOmal"
  m_bOmal = bOmal;
  } // CObject::CObject(CClass& oClass, bool bOmal)

/// constructor
CObject::CObject(const std::string& sName, bool bOmal)
  :inherited(sName)
  {
  InitBasics(bOmal);
  } // CObject::CObject(const std::string& sName, bool bOmal)

/// constructor
CObject::CObject(const std::string& sName, const CClass* poClass, bool bOmal)
  :inherited(sName)
  {
  InitBasics(bOmal);
  ClassSet(poClass);
  } // CObject::CObject(const std::string& sName, CClass& oClass)

/// constructor
CObject::CObject(const CObject* src, bool bOmal)
  :inherited()
  {
  InitBasics(bOmal);
  operator = (src);
  // ensure the assignment operation does not overwrite "bOmal"
  m_bOmal = bOmal;
  } // CObject::CObject(CObject& src)

/// destructor
CObject::~CObject()
  {
  } // CObject::~CObject()

/// InitBasics()
void CObject::InitBasics(bool bOmal)
  {
  m_bOmal   = bOmal;
  m_poClass = 0;
  m_poOmal  = 0;
  } // void CObject::InitBasics()

/// operator = (CObject*)
const CObject* CObject::operator = (const CObject* src)
  {
  if (src == this)
    {
    return this;
    }
  if (src == 0)
    {
    return this;
    }

  inherited::operator=( *(inherited*)src );

  m_bOmal   = src->m_bOmal;
  m_poOmal  = src->m_poOmal;
  m_poClass = src->m_poClass;

  m_moChildren = src->m_moChildren;
  m_aoAtoms    = src->m_aoAtoms;

  return this;
  } // CObject& CObject::operator = (CObject& src)

CRoot* CObject::Copy()
  {
  return new CObject(this);
  }

/// set the Omal
const CObject* CObject::OmalSet(const CObject* poOMAL)
  {
  // see  "operator = (CClass*))
  m_poOmal = const_cast<CObject*>(poOMAL);
  // may be, the object is an OMAL (m_bOmal == true), we got this OMAL as a parent
  //   => inheritance
  TimeStampUpdate();
  return m_poOmal;
  } // CObject::CObject* OmalSet(const CObject* poOMAL)

/// operator = (CClass*)
const CClass* CObject::operator = (const CClass* src)
  {
  // we do not have to set the OMAL from the class, because the class knows the OMAL itself
  // if the OMAL is not "0", we ignore this because:
  //    if the OMAL is queried, we use the OMAL from the class
  //    if the class is removed, we "remember" the original OMAL ;-)
  //    if the class has no OMAL, we use the private OMAL of the object
  return ClassSet(src);
  } // const CClass* CObject::operator = (const CClass* src)

/// Save the instance data to the given stream
CStream& CObject::Save(CStream& oStream)
  {
  oStream.Open("OBJECT");
  inherited::Save(oStream);
  // do we have a class?
  if ( m_poClass != 0)
    {
    // if so, put it to the stream
    oStream.Element("class", m_poClass->ID());
    }
  if (m_bOmal)
    {
    oStream.Element("isOMAL", "yes");
    }

  if ( m_aoAtoms.size() > 0 )
    {
    // atom list
    oStream.Open("atoms");
//    oStream.Element("count", (long) m_aoAtoms.size());
    for ( CVectorAtom::iterator itAtom  = m_aoAtoms.begin();
                                itAtom != m_aoAtoms.end();
                              ++itAtom )
      {
      oStream.Element("atom", (*itAtom)->ID());
      } // for ( CListAtoms::iterator itAtoms  = m_aoAtoms.begin;..
    oStream.Close();
    } // if ( m_aoAtoms.size() > 0 )

  if ( m_moChildren.size() > 0 )
    {
    // children map
    oStream.Open("children");
//    oStream.Element("count", (long)m_moChildren.size());

    for (CMapChildren::iterator it  = m_moChildren.begin();
                                it != m_moChildren.end();
                              ++it)
      {
      oStream.Open("child");
        oStream.Element("id", it->first->ID());    // the child
        oStream.Open("reasons");
//          oStream.Element("count", (long)it->second->size()); // the count of link reasons for this cild
          for (CListReason::iterator itReason  = it->second->begin();
                                     itReason != it->second->end();
                                   ++itReason)
            {
            oStream.Element("reason", (*itReason)->ID());
            } // for (CListReasons::iterator itReasons  = it->begin();..
          oStream.Close();
        oStream.Close();
      } // for (CMapChildren::iterator it  = m_moChildren.begin();..
    oStream.Close();
    } // if ( m_moChildren.size() > 0 )

  oStream.Close();
  return oStream;
  } // CStream& CObject::Save(CStream& oStream)

/// Load the object from the given stream
CStream& CObject::Load(CStream& oStream)
  {
  long lPtrClass;
  // please dont wonder! we read "-1" if there is no class this we be solved later, in ResolveIDs()
  oStream >> lPtrClass; m_poClass = (CClass*)lPtrClass;
  oStream >> m_bOmal;

  long l;
  // the count of atoms we habe to read.
  long lCount;
  // atom list
  oStream >> lCount;
  for (l = 0; l < lCount; l++)
    {
    long lPtrAtom;
    oStream >> lPtrAtom;
    m_aoAtoms.push_back( (CAtom*)lPtrAtom );
    }

  // children map
  oStream >> lCount;
  for (l = 0; l < lCount; l++)
    {
    long lPtrChild;
    oStream >> lPtrChild;

    long lCountReason;
    oStream >> lCountReason;
    CListReason* ploReason = new CListReason;
    for (long k = 0; k < lCountReason; k++)
      {
      long lPtrReason;
      oStream >> lPtrReason;
      ploReason->insert( (CReason*)lPtrReason );
      }
    m_moChildren[(CObject*)lPtrChild] = ploReason;
    }
  return inherited::Load(oStream);
  }

bool CObject::ResolveIDs(CODB& oDB)
  {
  m_poClass = oDB.Id2Ptr(m_poClass);

  // remap the atom list
  CVectorAtom aoAtoms;
  for ( CVectorAtom::iterator itAtom  = m_aoAtoms.begin();
                              itAtom != m_aoAtoms.end();
                            ++itAtom )
    {
    // replace the surrogate with the real pointer
    aoAtoms.push_back( oDB.Id2Ptr(*itAtom) );
    } // for ( CListAtoms::iterator itAtoms  = m_aoAtoms.begin;..
  m_aoAtoms.swap(aoAtoms);

  // remap the child-reasons-map
  //
  // DO NOT TRY TO REMAP THE TRUE MAP IN ONE STEP USING THE ITERATOR !!
  // because this will be an unsure operation. The map is is a "unique-map"
  // while remapping may appear some duplicate key:
  // the "surrogate-key" will be replaced by the true pointer, this
  // translates a long value to a pointer value = 32bit int -> 32bit int
  CMapChildren moChildren;
/*
  for ( CMapChildren::iterator it  = m_moChildren.begin();
                               it != m_moChildren.end();
                             ++it)
*/

  for ( CMapChildren::iterator it  = m_moChildren.begin();
                               it != m_moChildren.end();
                             ++it)
    {
    // remap the list of reasons
    CListReason* ploReasons = new CListReason;
    for ( CListReason::iterator itReason  = it->second->begin();
                                itReason != it->second->end();
                              ++itReason)
      {
      ploReasons->insert( oDB.Id2Ptr(*itReason) );
      } // for ( CListReason::iterator itReason  = it->second->begin();..
    // delete it->second because it was allocated while loading

    delete it->second;
    it->second = 0;

    // append the remapped child with his remapped (link-)reasons
    //  to the temporary map
    moChildren[oDB.Id2Ptr(it->first)] = ploReasons;
    } // for ( CMapChildren::iterator it  = m_moChildren.begin();..
  // assign the temporary cild-reasons-map to the true one

  m_moChildren = moChildren;

  inherited::ResolveIDs(oDB);
  return true;
  } // bool CObject::ResolveIDs(CODB& oDB)

/// Link
bool CObject::Link(const CObject* poChildObject, const CReason* poReason)
  {
  if ( const_cast<CReason*>(poReason)->Check(this, poChildObject) == false )
    {
    return false;
    } // if ( oReason.Allow(*this, oReason) == false )

  CListReason*           ploReason = 0;
  CMapChildren::iterator it        = m_moChildren.find( const_cast<CObject*>(poChildObject) );

  if ( it == m_moChildren.end() )
    {
    // new link
    ploReason = new CListReason;
    m_moChildren[const_cast<CObject*>(poChildObject)] = ploReason;
    }
  else // if ( it == m_moChildren.end() )
    {
    ploReason = it->second;
    // any link to THIS child exists
    // find if the reason is doubled
    for ( CListReason::iterator itReason  = ploReason->begin();
                                itReason != ploReason->end();
                              ++itReason)
      if ( *itReason == poReason )
        {
        // second linkage of an object with the same reason
        // is not allowed -we do nothing, but the link exists
        // so the result has to be true:
        return true;
        } // if ( *itReason == &oReason )
    } // else if ( it == m_moChildren.end() )

  ploReason->insert( const_cast<CReason*>(poReason) );
  const_cast<CObject*>(poChildObject)->ReferenceMade(this);
  const_cast<CReason*>(poReason     )->ReferenceMade(this);
  TimeStampUpdate();
  return true;
  } // bool CObject::Link(CObject& oChildObject, long lOrdinal)

//// LinkRemove
bool CObject::LinkRemove(const CObject* poChildObject, const CReason* poReason)
  {
  if ( (poChildObject == 0) || (poReason == 0) )
    {
    return false;
    }

  // find the child that has to remove
  CMapChildren::iterator it = m_moChildren.find(const_cast<CObject*>(poChildObject));
  if ( it == m_moChildren.end() )
    {
    // the link could not removed because the reason was not found
    return false;
    } //   if ( it == m_moChildren.end() )

  CListReason* ploReason = it->second;
  for ( CListReason::iterator itReason  = ploReason->begin();
                              itReason != ploReason->end();
                            ++itReason)
    {
    if ( *itReason == poReason )
      {
      ploReason->erase(itReason);
      const_cast<CReason*>(poReason     )->ReferenceRemoved(this);
      const_cast<CObject*>(poChildObject)->ReferenceRemoved(this);
      // if no reason left -> clear the child-link entry
      if ( ploReason->size() == 0 )
        {
        delete [] ploReason;
        // erase the child-link because no reason is left to hold the link
        m_moChildren.erase(it);
        } // if ( ploReason->size() == 0 )
      // success - the link-reason was found and removed
      TimeStampUpdate();
      return true;
      } // if ( *itReason == &oReason )
    } // if ( m_moObjectChildren.Lookup(&oChildObject, ploReason) )
  // the link could not removed because the reason was not found
  return false;
  } // bool CObject::LinkRemove(CObject& oChildObject)

/// operator +=
const CAtom* CObject::operator += (CAtom* src)
  {
  AtomAdd(src);
  return src;
  } // CAtom& CObject::operator += (CAtom& src)

/// AtomAdd
const CAtom* CObject::AtomAdd(CAtom* poAtom, long lOrdinal)
  {
  if (poAtom == 0)
    {
    return NULL;
    }

  for ( CVectorAtom::iterator it  = m_aoAtoms.begin();
                              it != m_aoAtoms.end();
                            ++it)
    {
    if ( *it == poAtom )
      {
      if (lOrdinal >= 0)
        {
        // the atom is already in the list an has a special position
        // thats why we have to remove it from its current position
        // to insert it to the correct position
        m_aoAtoms.erase(it);
        (const_cast<CAtom*>(poAtom))->ReferenceRemoved(this);
        break;
        }
      else
        {
        // the atom is already in the list an has no special position
        // all things are ok -> return the atom as if it were inserted
        // may be, the position is wrong -> let it be
        return poAtom;
        }
      } // if ( *it == &oAtom )
    } // for ( CListAtom::iterator it  = m_aoAtoms.begin();

  if ( (lOrdinal >= 0) && (lOrdinal < (long) m_aoAtoms.size()) )
    {
    CVectorAtom::iterator itInsertPos = m_aoAtoms.begin();
    while ( (lOrdinal > 0) && (itInsertPos != m_aoAtoms.end()) )
      {
      ++itInsertPos;
      lOrdinal--;
      } // while (lOrdinal > 0)
    m_aoAtoms.insert( itInsertPos, poAtom);
    }
  else // if ( lOrdinal >= 0 )
    {
    m_aoAtoms.push_back(poAtom);
    }

  (const_cast<CAtom*>(poAtom))->ReferenceMade(this);
  TimeStampUpdate();
  return poAtom;
  } // long CObject::AtomAdd(CAtom& oAtom, long lOrdinal)

/// returns the Atom-Vector
const CVectorAtom& CObject::AtomsGet() const
  {
  return m_aoAtoms;
  } // const CListAtom& CObject::AtomList() const

/// returns the Atom by index, CAN RETURN "NULL" !
CAtom* CObject::AtomGet(long nIndex)
  {
  if (nIndex >= (long)m_aoAtoms.size())
    {
    return NULL;
    }
  return m_aoAtoms[nIndex];
  } // CAtom* CObject::AtomGet(long nIndex)

/// returns the Atom by user-sign
CVectorAtom CObject::AtomGetBySign(long nSign, long nSignMask)
  {
  CVectorAtom oVectorAtom;
  for ( CVectorAtom::iterator it = m_aoAtoms.begin(); it != m_aoAtoms.end(); ++it )
    {
    if ( ((*it)->UserSignGet() & nSignMask) == nSign )
      {
      oVectorAtom.push_back(*it);
      } // if ( (*it)->UserSignGet() & nSignMask == nSign )
    } // for ( CVectorAtom::iterator it = m_aoAtoms.begin(); it != m_aoAtoms.end(); ++it )

  // this copies the result, because we have to be thread safe.
  return oVectorAtom;
  } // CAtom* CObject::AtomGetBySign(short nSign, short nSignMask)

/// returns a list of Atoms found by Name
CVectorAtom CObject::AtomGet(const std::string& sName, bool bPartial)
  {
  CVectorAtom oVectorAtom;
  for ( CVectorAtom::iterator it = m_aoAtoms.begin(); it != m_aoAtoms.end(); ++it )
    {
    if ( bPartial == true )
      {
      if ( (*it)->NameGet().find(sName) != std::string::npos )
        {
        oVectorAtom.push_back(*it);
        }
      }
    else // if ( bPartial == true )
      {
      if ( (*it)->NameGet() == sName )
        {
        oVectorAtom.push_back(*it);
        }
      } // else if ( bPartial == true )
    } // for ( CVectorAtom::iterator it = m_aoAtoms.begin(); it != m_aoAtoms.end(); ++it )

  // this copies the result, because we have to be thread safe.
  return oVectorAtom;
  } // CVectorAtom CObject::AtomGet(const std::string& sName, bool bPartial, bool bCaseSensitive)


/// returns the amount of atoms in the object
long CObject::AtomCountGet() const
  {
  return m_aoAtoms.size();
  }

/// operator -=
const CAtom* CObject::operator -= (CAtom* src)
  {
  AtomRemove(src);
  return src;
  } // CAtom& CObject::operator -= (CAtom& src)

/// AtomRemove
const CAtom* CObject::AtomRemove(CAtom* poAtom)
  {
  for ( CVectorAtom::iterator it  = m_aoAtoms.begin();
                              it != m_aoAtoms.end();
                            ++it)
    {
    if ( *it == poAtom )
      {
      m_aoAtoms.erase(it);
	    poAtom->ReferenceRemoved(this);
      TimeStampUpdate();
      return poAtom;
      } // if ( *it == &oAtom )
    } // for ( CListAtom::iterator it  = m_aoAtoms.begin();

  return NULL;
  } // long CObject::AtomRemove(CAtom& oAtom)

/// returns the Child-Vector
const CMapChildren& CObject::ChildrenGet() const
  {
  return m_moChildren;
  } // const CMapChildren& CObject::ChildrenGet() const

/// returns the Child by index, CAN RETURN "NULL" !
CObject* CObject::ChildGet(long nIndex)
  {
  return ChildPairGet(nIndex).first;
  } // CObject* CObject::ChildGet(long nIndex)

/// returns the Child-Pair by index, CAN RETURN "NULL" !
CPairChild CObject::ChildPairGet(long nIndex)
  {
  if ( nIndex >= (long) m_moChildren.size() )
    {
    return CPairChild(0, 0);
    }
  CMapChildren::iterator it = m_moChildren.begin();
  for ( long n = 0; n < nIndex; ++it, n++ );
  return *it;
  } // virtual CPairChild CObject::ChildPairGet(long nIndex)

/// returns the amount of Childs in the object
long CObject::ChildCountGet() const
  {
  return m_moChildren.size();
  } // long CObject::ChildCountGet() const


/// returns the Parent-Vector
const CMapReferencing& CObject::ParentsGet() const
  {
  return m_moReferencingInstances;
  } // const CMapReferencing& CObject::ParentsGet() const

/// returns the Parent by index, CAN RETURN "NULL" !
CObject* CObject::ParentGet(long nIndex)
  {
  return ParentPairGet(nIndex).first;
  } // CObject* CObject::ParentGet(long nIndex)

CPairParent CObject::ParentPairGet(long nIndex)
  {
  if ( nIndex >= (long) m_moReferencingInstances.size() )
    {
    return CPairParent(0, 0);
    }
  CMapReferencing::iterator it = m_moReferencingInstances.begin();
  for ( long n = 0; n < nIndex; ++it, n++ );
  // we know: in m_moReferencingInstances of an Object are ONLY Objects!
  return CPairParent((CObject*)it->first, it->second);
  } // CPairParent CObject::ParentPairGet(long nIndex)

/// returns the amount of Parents in the object
long CObject::ParentCountGet() const
  {
  return m_moReferencingInstances.size();
  } // long CObject::ParentCountGet() const


/// ClassSet()
CClass* CObject::ClassSet(const CClass* poClass)
  {
  TimeStampUpdate();
  return m_poClass = const_cast<CClass*>(poClass);
  } // const CClass* CObject::ClassSet()

/// ClassGet()
CClass* CObject::ClassGet() const
  {
  return m_poClass;
  } // const CClass* CObject::ClassGet()

void CObject::Dump()
  {

	const char l1[] = "OBJECT...: id-O = ";
	const char l2[] = "           name = ";
	const char l3[] = "           class= ";
	const char l4[] = "   OMAL..: ";
	const char l5[] = " ->LINK..: id-O = ";
	const char l6[] = "           name = ";
	const char l7[] = "           class= ";
	const char l8[] = "           REASON..: id-R = ";
	const char l9[] = "                     name = ";
	const char l0[] = "   ATOMLIST";
	const char la[] = " <-LINK..: id-O = ";
	const char lb[] = "           name = ";
	const char lc[] = "           class= ";
/*
	const char ld[] = "           REASON..: id-R = ";
	const char le[] = "                     name = ";
*/
  std::cout << l1 << ID() << std::endl;
  std::cout << l2 << NameGet() << std::endl;

  if (m_poClass)
    {
    std::cout << l3 << m_poClass->NameGet() << std::endl;

    if ((CObject*)*m_poClass)
      {
      std::cout << l4 << (*(CObject*)*m_poClass).NameGet() << std::endl;
      }
    } // if (m_poClass)

  // children list
  for (CMapChildren::iterator it  = m_moChildren.begin();
                              it != m_moChildren.end();
                            ++it)
    {
    std::cout << l5 << it->first->ID()                                 << std::endl;
    std::cout << l6 << it->first->NameGet()                            << std::endl;
    std::cout << l7 << ((CObject*)it->first)->ClassGet()->NameGet() << std::endl;

    for (CListReason::iterator itReason  = it->second->begin();
                               itReason != it->second->end();
                             ++itReason)
      {
      std::cout << l8 << (*itReason)->ID()      << std::endl;
      std::cout << l9 << (*itReason)->NameGet() << std::endl;
      } // for (CListReason::iterator itReason  = it->second->begin();..
    } // for (CMapChildren::iterator it  = m_moChildren.begin();..

  // parent list
  for (CMapReferencing::iterator itR  = m_moReferencingInstances.begin();
                                 itR != m_moReferencingInstances.end();
                               ++itR)
    {
    std::cout << la << itR->first->ID()                                 << std::endl;
    std::cout << lb << itR->first->NameGet()                            << std::endl;
    std::cout << lc << ((CObject*)itR->first)->ClassGet()->NameGet() << std::endl;
    } // for (CMapChildren::iterator it  = m_moChildren.begin();..

  // atom list
  std::cout << l0 << std::endl;
  for ( CVectorAtom::iterator itAtom  = m_aoAtoms.begin();
                              itAtom != m_aoAtoms.end();
                            ++itAtom )
    {
    std::cout << "        " << (*itAtom)->NameGet() << "\t";
    std::cout << "<"        << (*itAtom)->PrefixGet() << "> ";
    std::cout               << (*itAtom)->UIFormat();
    std::cout << " ["       << (*itAtom)->SuffixGet() << "]" << std::endl;
    } // for ( CListAtom::iterator itAtom  = m_aoAtoms.begin();

  std::cout << std::endl;
  std::cout.flush();
  } // void CObject::Dump()
