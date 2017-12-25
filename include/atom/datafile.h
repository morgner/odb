#ifndef __ATOMDATA_FILE_H
#define __ATOMDATA_FILE_H

/***************************************************************************
                          datafile.h
                          -------------------
    begin                : Fri Mar 3 2000
    copyright            : (C) 1993..2011 by Manfred Morgner
    email                : manfred@morgner.com
 ***************************************************************************/


/**
 * @file
 * File-AtomData.
 */

#include "atom/data.h"

namespace odb {

/**
 * The File-AtomData.
 */
class CAtomDataFile : public CAtomData  
  {
  private:
    typedef CAtomData inherited;

  public:
          /**
           * Constructor.
           */
             CAtomDataFile();

          /**
           * Copyconstructor.
           * @param src the atom to be copied from.
           */
             CAtomDataFile(CAtomDataFile& src);
// ??        CAtomDataFile(const fstream& src);

          /**
           * Destructor.
           */
    virtual ~CAtomDataFile();

    virtual long Type() { return DT_FILE; }

            CAtomDataFile& operator = (CAtomDataFile& src);
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
	unsigned char*    m_Data;           /**< binary data */
	std::string       m_sFileName;      /**< original file name */
	std::string       m_sFilePath;      /**< original file path */
    long            m_nFileSize;      /**< original file size */
    long            m_nSize;          /**< size of m_Data */
    time_t          m_tCreation;      /**< date of creation */
    time_t          m_tModification;  /**< date of last modification */
  };

} // namespace odb

// __ATOMDATA_FILE_H
#endif
