#ifndef __ROPE_H
#define __ROPE_H

/***************************************************************************
                           rope.h
                           -------------------
    begin                : Sun Mar 19 2000
    copyright            : (C) 1993..2011 by Manfred Morgner
    email                : manfred@morgner.com
 ***************************************************************************/


/**
 * @file
 * Rope.
 */

#include "root.h"

namespace odb {

class CAtom;

/**
 * The Rope.
 */
class CRope : public CRoot
  {
    DECLARE_RTTI

  private:
    typedef CRoot inherited;

  protected:
    CRope() {} // forbidden
  public:
    CRope(long lid);
    CRope(CAtom* poStartpoint, const std::string& sName = ""s);

    CAtom* StartpointGet();

    CStream& Save(CStream& oStream);
    CStream& Load(CStream& oStream);
    bool     ResolveIDs(CODB& oDB);


  protected:
    CAtom* m_poStartpoint;
  };

} // namespace odb
 // __ROPE_H
#endif
