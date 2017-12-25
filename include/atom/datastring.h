#ifndef __ATOMDATA_STRING_H
#define __ATOMDATA_STRING_H

/***************************************************************************
                           datastring.h
                           -------------------
    begin                : Fri Mar 3 2000
    copyright            : (C) 1993..2011 by Manfred Morgner
    email                : manfred@morgner.com
 ***************************************************************************/


/**
 * @file
 * AtomData String TODO.
 */

#include "atom/data.h"

namespace odb {

class CAtomDataString : public CAtomData  
  {
  private:
    typedef CAtomData inherited;

  public:
             CAtomDataString();
             CAtomDataString(CAtomDataString& src);
             CAtomDataString(const std::string& src);
    virtual ~CAtomDataString();

    virtual long Type() { return DT_STRING; }

            CAtomDataString& operator = (CAtomDataString& src);
    virtual CAtomData* Copy();

//    virtual CStream& Save(CStream& oStream);
    virtual CStream& Load(CStream& oStream);

    virtual std::string UIFormat(const std::string& sFormat);

    virtual long BinarySizeGet();

  protected:
    virtual const void* BinaryBufferGet();

  protected:
    /// called from the constructor(s) to initialize all member variable
    void InitBasics();

  protected:
	  std::string m_Data;
  };

} // namespace odb

// __ATOMDATA_STRING_H
#endif
