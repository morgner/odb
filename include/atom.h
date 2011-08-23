/***************************************************************************
                          atom.h
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

#ifndef __ATOM_H
#define __ATOM_H

/**
 * @file 
 * Atom.
 */

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "root.h"
#include "rope.h"
#include "atom/data.h"

namespace odb {

#define ATOM_HAS_RESID      0x00000001
#define ATOM_HAS_FORMAT     0x00000002
#define ATOM_HAS_PREFIX     0x00000004
#define ATOM_HAS_SUFFIX     0x00000008
#define ATOM_HAS_DATA       0x00000010
#define ATOM_HAS_USER_SIGN  0x00000020

class CAtomDataBinary;


/**
 * The Atom.
 */

class CAtom : public CRoot
  {
    DECLARE_RTTI
  private:
    typedef CRoot inherited;

  protected:
    /**
     * The data, the atom handles. It's derived from -> CAtomData.
     */
    CAtomData* m_poData;

    /**
     * Is the current instance an Atom Definition Object? A template for other atoms?.
     */
    bool      m_bADO; // ADO = Atom Definition Object
    /**
     * If not NULL, the master atom, describing any expected element of the current instance.
     */
    CAtom*    m_poADO;
    /**
     * a bitmaped register, signalizing the usage of the member variables.
     */
    long      m_lMaskFields;
    /**
     * the id to assign a specific resource in the user interface.
     */
    long      m_lDisplayResourceId;
    /**
     * the textual data prefix for the user interface.
     */
    std::string    m_sPrefix;
    /**
     * the textual data suffix for the user interface.
     */
    std::string    m_sSuffix;
    /**
     * the format string to format the value for the user interface.
     */
    std::string    m_sFormat;
// ??    bool    m_bIs2Link;

    /**
     * a sign, for use by clients (unknown purpose).
     */
    long m_nUserSign;

    /**
     * an empty string as "special" return argument.
     */
    static std::string s_sEmptyString;

            /**
             * Copyconstructor.
             */
	           CAtom(CAtom& src);
  public:
            /**
             * Constructor.
             */
	           CAtom();

            /**
             * Constructor.
             * @param lid the id the atom shall get.
             */
	           CAtom(long lid);

            /**
             * Copyconstructor.
             * @param src the atom to be copied.
             * @param bADO if this is changed to 'true', this atom surves as a tamplate (like a OMAL).
             */
	           CAtom(CAtom& src, bool bADO);

            /**
             * Constructor.
             * @param sName The atom's name.
             * @param bADO if this is changed to 'true', this atom surves as a tamplate (like a OMAL).
             */
               CAtom(const std::string& sName, bool bADO = false);

            /**
             * Constructor.
             * @param sName The atom's name.
             * @param bADO if this is changed to 'true', this atom surves as a tamplate (like a OMAL).
             * @param oADO the ADO, wich describes this atom (like an OBAL for objects).
             */
	           CAtom(const std::string& sName, CAtom& oADO, bool bADO = false);
             // assignement operators
             //

           /**
            * Constructor. Makes it possible to fill the atom with data at instanciation.
            * @param sName The atom's name.
            * @param src The data which the atom shall represent.
            * @param bADO if this is changed to 'true', this atom surves as a tamplate (like a OMAL).
            */
             CAtom(const std::string& sName, long   src, bool bADO = false);

           /**
            * Constructor. Makes it possible to fill the atom with data at instanciation.
            * @param sName The atom's name.
            * @param src The data which the atom shall represent.
            * @param bADO if this is changed to 'true', this atom surves as a tamplate (like a OMAL).
            */
             CAtom(const std::string& sName, double src, bool bADO = false);

           /**
            * Constructor. Makes it possible to fill the atom with data at instanciation.
            * @param sName The atom's name.
            * @param src The data which the atom shall represent.
            * @param bADO if this is changed to 'true', this atom surves as a tamplate (like a OMAL).
            */
             CAtom(const std::string& sName, float  src, bool bADO = false);

           /**
            * Constructor. Makes it possible to fill the atom with data at instanciation.
            * @param sName The atom's name.
            * @param src The data which the atom shall represent.
            * @param bADO if this is changed to 'true', this atom surves as a tamplate (like a OMAL).
            */
             CAtom(const std::string& sName, const  std::string& src, bool bADO = false);
// ??        CAtom(const std::string& sName, const  std::fstream& src, bool bADO = false);

           /**
            * Constructor. Makes it possible to fill the atom with data at instanciation.
            * @param sName The atom's name.
            * @param pData The data which the atom shall represent.
            * @param nSizeInBytes the length of the data.
            * @param bADO if this is changed to 'true', this atom surves as a tamplate (like a OMAL).
            */
             CAtom(const std::string& sName, void*  pData, long& nSizeInBytes, bool bADO = false);

           /**
            * Constructor. Makes it possible to fill the atom with data at instanciation.
            * @param sName The atom's name.
            * @param poData another atom's binarydata which the atom shall represent as well.
            * @param bADO if this is changed to 'true', this atom surves as a tamplate (like a OMAL).
            */
             CAtom(const std::string& sName, CAtomDataBinary* poData, bool bADO = false);

           /**
            * Destructor.
            */
	  virtual ~CAtom();

            CAtom& operator = (CAtom& src);
    virtual inherited* Copy();

    virtual timeval TimeStampGet();

    /**
     * Get the binary Size.
     * @return the size of the encapsulated data in octets (8bit - bytes).
     */
    virtual long BinarySizeGet();
    
    /**
     * Copies the data from the AtomData instance to the given buffer - if possible.
     * @return the size of the data in octets (8bit - bytes).
     */
    virtual long BinaryDataGet(void* pData, long nSizeOfBuffer);

    virtual CStream& Save(CStream& oStream);
    virtual CStream& Load(CStream& oStream);
    
    /**
     * resolves the old link-pointer (as stored) to the new instance-pointer (as needed) after "load()".
     * @return success? TODO.
     */
    virtual bool ResolveIDs(CODB& oDB);

            CAtom& operator = (            long             src);
            CAtom& operator = (            double           src);
            CAtom& operator = (            float            src);
            CAtom& operator = (const  std::string&          src);
            CAtom& operator = (const  std::fstream&         src);
            CAtom& operator = (            CAtomDataBinary* poData);

    virtual const      long    ResIdSet (      long     lid);
    virtual const std::string& FormatSet(const std::string& sFormat);
    virtual const std::string& PrefixSet(const std::string& sPrefix);
    virtual const std::string& SuffixSet(const std::string& sSuffix);

    virtual const      long    ResIdGet () const;
    virtual const std::string& FormatGet() const;
    virtual const std::string& PrefixGet() const;
    virtual const std::string& SuffixGet() const;

    /**
     * Prepair and return the data in a user friendly text format.
     * @return the data in a user friendly text format.
     */
    virtual std::string UIFormat();

    /**
     * set user sign
     * @param nSign a user-defined sign.
     * @return success? TODO.
     */
    bool UserSignSet(long nSign);
    
    /**
     * get user sign
     * @return the previously set user-sign of the atom.
     */
    long UserSignGet();

    /**
     * Link atoms per rope (return value is an error code).
     * @param poRope the rope the atom shall become part of.
     * @param poAtomNext the neighbour atom.
     */
    virtual       long      RopeLinkAdd   (CRope* poRope, CAtom* poAtomNext);
    virtual       long      RopeLinkRemove(CRope* poRope);
    virtual       CAtom*    RopeLinkNext  (CRope* poRope);
    virtual const CMapRope& RopeMapGet() const;

  protected:
    /**
     * The ropes the atom is part of.
     */
    CMapRope m_moRope;

  // non virtual members
  protected:
    /**
     * called from the constructor(s) to initialize all member variable.
     * @param bAtom if this is changed to 'true', this atom surves as a tamplate (like a OMAL)..
     */
    void InitBasics(bool bADO);

  // other members
  public:

  };

} // namespace odb

#endif // __ATOM_H
