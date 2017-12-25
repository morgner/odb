#ifndef __ATOMDATA_REAL_H
#define __ATOMDATA_REAL_H

/***************************************************************************
                          datareal.h
                          -------------------
    begin                : Fri Mar 3 2000
    copyright            : (C) 1993..2011 by Manfred Morgner
    email                : manfred@morgner.com
 ***************************************************************************/


/**
 * @file 
 * Atomdata Real TODO.
 */

#include "atom/data.h"

namespace odb {

class CAtomDataReal : public CAtomData  
  {
  private:
    typedef CAtomData inherited;

  public:
             CAtomDataReal();
             CAtomDataReal(CAtomDataReal& src);
             CAtomDataReal(double src);
    virtual ~CAtomDataReal();

    virtual long Type() { return DT_REAL; }

            CAtomDataReal& operator = (CAtomDataReal& src);
    virtual CAtomData* Copy();

    virtual CStream& Save(CStream& oStream);
    virtual CStream& Load(CStream& oStream);

    virtual std::string UIFormat(const std::string& sFormat);

    virtual long BinarySizeGet();

  protected:
    virtual const void* BinaryBufferGet();

  protected:
    /// called from the constructor(s) to initialize all member variable
    void InitBasics();

  protected:
	  double m_Data;
  };

} // namespace odb

// __ATOMDATA_REAL_H
#endif
