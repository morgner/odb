#ifndef __REASON_H
#define __REASON_H

/***************************************************************************
                          reason.h  -  description
                             -------------------
    begin                : Fri Mar 3 2000
    copyright            : (C) 1993..2011 by Manfred Morgner
    email                : manfred@morgner.com
 ***************************************************************************/


/**
 * @file
 * Reason.
 */

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
 // __REASON_H
#endif
