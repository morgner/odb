#ifndef __ATOMDATA_BINARY_H
#define __ATOMDATA_BINARY_H

/***************************************************************************
                          databinary.h
                          -------------------
    begin                : Wef Apr 15 2000
    copyright            : (C) 1993..2011 by Manfred Morgner
    email                : manfred@morgner.com
 ***************************************************************************/


/**
 * @file
 * Binary Atomdata.
 */

#include "atom/data.h"

namespace odb {

/**
 * The Binary AtomData.
 */
class CAtomDataBinary : public CAtomData
  {
  private:
    typedef CAtomData inherited;
    static std::string s_sUIText;

  public:
          /** 
           * Constructor.
           */
             CAtomDataBinary();

          /**
           * Constructor.
           * @param pData the source of the binary data.
           * @param nSizeInBytes number of bytes to be copied.
           */
             CAtomDataBinary(void* pData, long& nSizeInBytes);

          /**
           * Destructor.
           */
    virtual ~CAtomDataBinary();

    virtual long Type() { return DT_BINARY; }

            CAtomDataBinary& operator = (CAtomDataBinary& src);
    virtual inherited* Copy();

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
    bool DataSet(void* pData, long& nSizeInBytes);
    bool DataFree();

  protected:
      unsigned char*  m_Data;  /**< binary data */
      long            m_nSize; /**< size of m_Data */

  }; // class CAtomDataBinary : public CAtomData

} // namespace odb

// __ATOMDATA_BINARY_H
#endif
