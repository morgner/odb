/***************************************************************************
                           odb.h  -  description
                             -------------------
    begin                : Fri Mar 3 2000
    copyright            : (C) 1993..2011 by Manfred Morgner
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

#ifndef __ODB_H
#define __ODB_H

/* *
 * @file
 * The ODB.
 */

#include "root.h"
#include "class.h"
#include "atom.h"
#include "object.h"
#include "reason.h"
#include "rope.h"

#include "keyname.h"

#include <sys/time.h>

namespace odb {

/**
 * The Object Data Base.
 * a container of collections and sorted lists to handle and keep anything
 * necessary for the odb. CODB also provides API's and several interfaces
 * for applications.
 * This class is a friend of any 'thing' to enable free access to the things for administrative purpose.
 */
class CODB : public CListRoot
  {
  /*
     "friend" declarations
     necessary for access to protected members which will be necessary for some
     internal (automatic) operations (as Id2Ptr) which are protected because
     wrong use crashes the whole database
  */
  friend class odb::CRoot;
  friend class odb::CClass;
  friend class odb::CAtom;
  friend class odb::CObject;
  friend class odb::CReason;
  friend class odb::CRope;

  private:
    typedef CListRoot inherited;

  protected:
    std::string m_sFileName;

  public:
             CODB( const std::string& sFileName = _TEXT("") );
    virtual ~CODB();

    void Dump();

    bool Save( const std::string& sFileName = _TEXT("") );
    bool Load( const std::string& sFileName = _TEXT("") );

    bool SaveXML( const std::string& sFileName = _TEXT("") );

    long IdValidate( const CRoot* poInstance );

    CClass*  Add( CClass*  poSrc );
    CAtom*   Add( CAtom*   poSrc );
    CObject* Add( CObject* poSrc );
    CReason* Add( CReason* poSrc );
    CRope*   Add( CRope*   poSrc );

    CClass*  Id2PtrClass ( long lid ) const;
    CAtom*   Id2PtrAtom  ( long lid ) const;
    CObject* Id2PtrObject( long lid ) const;
    CReason* Id2PtrReason( long lid ) const;
    CRope*   Id2PtrRope  ( long lid ) const;

/*
    bool FillListClass ( const CListClass*  poList ) const;
    bool FillListAtom  ( const CListAtom*   poList ) const;
    bool FillListObject( const CListObject* poList ) const;
    bool FillListReason( const CListReason* poList ) const;
*/
    // EXTERNAL INTERFACE USED FOR CLIENT/SERVER CONNECTIONS

    /*********/
    /* N E W */
    /*********/
    CClass*  NewClass ( const std::string& sName );
    CAtom*   NewAtom  ( const std::string& sName );
    CObject* NewObject( const std::string& sName, const CClass* poClass = 0);
    CReason* NewReason( const std::string& sName, const CClass* poClassParent, const CClass* poClassChild );
    CRope*   NewRope  ( const std::string& sName,       CAtom*  poStartpoint );

    /*********/
    /* G E T */
    /*********/
    CVectorRoot GetClass ( const std::string& sName );
    CVectorRoot GetAtom  ( const std::string& sName );
    CVectorRoot GetObject( const std::string& sName );
    CVectorRoot GetReason( const std::string& sName );
    CVectorRoot GetThingsChanged( timeval tFromTime );

    CVectorRoot GetRope  ( const std::string& sName );

    long TransactionOpen     ( const CRoot* poCaller );
    long TransactionCommit   ( const CRoot* poCaller );
    long TransactionCommitAll( const CRoot* poCaller );
    long TransactionAbort    ( const CRoot* poCaller );

    /*************/
    /* C L A S S */
    /*************/
    long        ClassSetOmal(       CClass*  poClass,
                                    CObject* poObject,
                                    bool        bOverwrite = false);
    CObject* ClassGetOmal( const CClass*  poClass ) const;

    /***********/
    /* A T O M */
    /***********/
    long      AtomFormatSet(       CAtom* poAtom,
                             const std::string&   sFormat,
                             const std::string&   sPrefix = _TEXT(""),
                             const std::string&   sSuffix = _TEXT("") );
    long      AtomPrefixSet(       CAtom* poAtom,
                             const std::string&   sPrefix );
    long      AtomSuffixSet(       CAtom* poAtom,
                             const std::string&   sSuffix );

    std::string     AtomFormatGet ( const CAtom* poAtom ) const;

          CRope*    AtomRopeNew   (       CAtom* poAtom );
          long      AtomRopeAppend(       CAtom* poAtom,
                                    const CRope* poRope,
                                          CAtom* poAtomNext );
          CAtom*    AtomRopeNext  ( const CAtom* poAtom,
                                    const CRope* poRope );
    const CMapRope& AtomRopeMapGet( const CAtom* poAtom );

    CVectorRoot     AtomGetChanged( timeval tFromTime );

    /****************/
    /* O B  J E C T */
    /****************/
    long        ObjectAddAtom    (       CObject* poObject,
                                         CAtom*   poAtom    );
    long        ObjectLinkObject (       CObject* poParent,
                                         CReason* poReason,
                                         CObject* poChild   );
    long        ObjectSetClass   (       CObject* poObject,
                                         CClass*  poClass   );
    CClass*     ObjectGetClass   ( const CObject* poObject  );

    CVectorRoot ObjectGetChanged ( timeval tFromTime );
/*
    ObjectGetChildren();
    ObjectGetParents();
*/
  protected:
    /// called from the constructor(s) to initialize all member variable
    void InitBasics();

  protected:
    /// used while loading the DB
    CClass*  Id2Ptr( const CClass*  poSrc ) const;
    CAtom*   Id2Ptr( const CAtom*   poSrc ) const;
    CObject* Id2Ptr( const CObject* poSrc ) const;
    CReason* Id2Ptr( const CReason* poSrc ) const;
    CRope*   Id2Ptr( const CRope*   poSrc ) const;

  protected:
    long m_lMaxClass;
    long m_lMaxAtom;
    long m_lMaxObject;
    long m_lMaxReason;
    long m_lMaxRope;

  protected:
    typedef std::map<long, CClass*>  CMapId2PtrClass;
    typedef std::map<long, CAtom*>   CMapId2PtrAtom;
    typedef std::map<long, CObject*> CMapId2PtrObject;
    typedef std::map<long, CReason*> CMapId2PtrReason;
    typedef std::map<long, CRope*>   CMapId2PtrRope;

    CMapId2PtrClass   m_moId2PtrClass;
    CMapId2PtrAtom    m_moId2PtrAtom;
    CMapId2PtrObject  m_moId2PtrObject;
    CMapId2PtrReason  m_moId2PtrReason;
    CMapId2PtrRope    m_moId2PtrRope;

  // indizes over things _name_
  protected:
    long KeyNameIndexItemAdd       (CKeyNameIndex& rsoKeyNameIndex, CRoot* poItem);
    long KeyNameIndexItemRemove    (CKeyNameIndex& rsoKeyNameIndex, CRoot* poItem);
    long KeyNameIndexItemChangeName(CKeyNameIndex& rsoKeyNameIndex, CRoot* poItem, const std::string& sNewName);

  public:
    long ChangeName(CClass*  poSrc, const std::string& sNewName);
    long ChangeName(CAtom*   poSrc, const std::string& sNewName);
    long ChangeName(CObject* poSrc, const std::string& sNewName);
    long ChangeName(CReason* poSrc, const std::string& sNewName);
    long ChangeName(CRope*   poSrc, const std::string& sNewName);


  protected:
    CKeyNameIndex     m_soIndexNameClass;
    CKeyNameIndex     m_soIndexNameAtom;
    CKeyNameIndex     m_soIndexNameObject;
    CKeyNameIndex     m_soIndexNameReason;
    CKeyNameIndex     m_soIndexNameRope;

  }; // class CODB : public CListRoot

} // namespace odb

#endif // __ODB_H
