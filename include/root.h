#ifndef __ROOT_H
#define __ROOT_H

/***************************************************************************
                          root.h  -  description
                             -------------------
    begin                : Fri Mar 3 2000
    copyright            : (C) 1993..2011 by Manfred Morgner
    email                : manfred@morgner.com
 ***************************************************************************/


/**
 * @file
 * Root.
 */

#include "rtti.h"
#include "stream.h"

#include <sys/time.h>

#include <vector>
#include <set>
#include <map>

namespace odb {

  class CODB;
  class CClass;
  class CAtom;
  class CObject;
  class CReason;
  class CRope;

  typedef std::set<CClass*>             CListClass;
  typedef std::set<CAtom*>              CListAtom;
  typedef std::set<CObject*>            CListObject;
  typedef std::set<CReason*>            CListReason;

  typedef std::set<CRope*>              CListRope;
  typedef std::set<CRoot*>              CListRoot;

  typedef std::map<CRoot*, long>        CMapReferencing;
  typedef std::map<CRope*, CAtom*>      CMapRope;

  typedef std::vector<CAtom*>           CVectorAtom;

/**
 * The mighty Root.
 * represents the base class for any 'thing' in the odb
 */
class CRoot
  {
    DECLARE_RTTI

  friend class CODB;

  protected:

         long m_lid;            /**< the identity of the instance */

    std::string m_sName;       /**< the UI-name of the instance */
         long   m_lidDatabase; /**< the ID of the database the object come from (for checkin/checkout) */
    /**
     * The name of the registrytion server.
     * only databases with the same Reg-Server are allowed to exchange objects
     */
    std::string  m_sURLRegistrationServer;

    bool m_bDeleted; /**< the object autmaticaly will be deleted; if the reference counter reaches ZERO; no new link is allowed */

    timeval m_tTimeStamp; /**< the time of the last modification */

    CMapReferencing m_moReferencingInstances;

             CRoot(const CRoot& src);
  public:
             CRoot();
             CRoot(long  lid);
             CRoot(const std::string& sName);
             CRoot(const CRoot* src);

    virtual ~CRoot();

             CRoot* operator = (const CRoot* src);
    virtual  CRoot* Copy();

    virtual bool Validate(CODB& oODatabase);
    virtual bool IsValid() const;
    virtual long ID() const;

    virtual timeval TimeStampGet();
    virtual bool    TimeStampUpdate();

    /// stores instance data to oStream
    virtual CStream& Save(CStream& oStream);
    /// loads instance data from oStream
    virtual CStream& Load(CStream& oStream);
    /// resolves the old link-pointer (as stored) to the new instance-pointer (as needed) after "load()"
    virtual bool        ResolveIDs(CODB& oDB);
    /// register that there was a reference mad to this object; also register WHO has made this reference
    virtual bool        ReferenceMade   (CRoot* oReferencingInstance);
    /// forget a specific reference
    virtual bool        ReferenceRemoved(CRoot* oReferencingInstance);
    /// the count of instances referencing to this instance, including multilinks using multiple reasons
    virtual const long ParentCount();
    /// returns the parentslist
    virtual const CMapReferencing& ParentGet();

    /// deletes the instance; if no reference is in the register, the instance immediatly will disapear;
    /// otherwise it will destroyed if the reference-counter is ZERO
    virtual bool Delete();

    /// set's the new name for the instance; names have not to be unique
    virtual const std::string& NameSet(const std::string& sName);
    /// returns the current name of the instance
    virtual const std::string& NameGet() const;

  protected:
    /// called from the constructor(s) to initialize all member variable
    void InitBasics();
  };

} // namespace odb
 // __ROOT_H
#endif
