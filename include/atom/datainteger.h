#ifndef __ATOMDATA_INTEGER_H
#define __ATOMDATA_INTEGER_H

/***************************************************************************
                          datainteger.h
                          -------------------
    begin                : Fri Mar 3 2000
    copyright            : (C) 1993..2011 by Manfred Morgner
    email                : manfred@morgner.com
 ***************************************************************************/


/**
 * @file 
 * AtomData Integer.
 */

#include "atom/data.h"

namespace odb {

/**
 * The Integer Atom-Data.
 */
class CAtomDataInteger : public CAtomData
  {
  private:
    typedef CAtomData inherited;

  public:
          /** 
           * Constructor.
           */
             CAtomDataInteger();

          /** 
           * Copyconstructor.
           * @param src the atom to be copied from.
           */
             CAtomDataInteger(CAtomDataInteger& src);

          /**
           * Constructor.
           * @param src the integer the atom shall represent.
           */
             CAtomDataInteger(long src);

          /**
           * Destructor.
           */
    virtual ~CAtomDataInteger();

    virtual long Type() { return DT_INTEGER; }

            CAtomDataInteger& operator = (CAtomDataInteger& src);
    virtual CAtomData* Copy();

    virtual CStream& Save(CStream& oStream);
    virtual CStream& Load(CStream& oStream);

    virtual std::string UIFormat(const std::string& sFormat);

    virtual long BinarySizeGet();

  protected:
    virtual const void* BinaryBufferGet();

  protected:
    /**
     * called from the constructor(s) to initialize all member variable.
     */
    void InitBasics();

  protected:
	  long m_Data;
  };

} // namespace odb

// __ATOMDATA_INTEGER_H
#endif
