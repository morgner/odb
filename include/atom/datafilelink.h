#ifndef __ATOMDATA_FILELINK_H
#define __ATOMDATA_FILELINK_H

/***************************************************************************
                          datafilelink.h
                          -------------------
    begin                : Fri Mar 3 2000
    copyright            : (C) 1993..2011 by Manfred Morgner
    email                : manfred@morgner.com
 ***************************************************************************/


/**
 * @file
 * FileLink AtomData.
 */

#include "atom/data.h"

namespace odb {
/**
 * The AtomData File-Link.
 */
class CAtomDataFileLink : public CAtomData
  {
  private:
    typedef CAtomData inherited;

  public:
          /**
           * Constructor.
           */
             CAtomDataFileLink();
          /**
           * Copyconstructor.
           * @param src the atom to be copied from.
           */
             CAtomDataFileLink(CAtomDataFileLink& src);
// ??        CAtomDataFileLink(const fstream& src);

          /**
           * Destructor.
           */
    virtual ~CAtomDataFileLink();

    virtual long Type() { return DT_FILELINK; }

            CAtomDataFileLink& operator = (CAtomDataFileLink& src);
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
    std::string  m_Data;           /**< filename */
    long         m_nSize;
    time_t       m_tCreation;
    time_t       m_tModification;
  };
} // namespace odb

// __ATOMDATA_FILELINK_H
#endif
