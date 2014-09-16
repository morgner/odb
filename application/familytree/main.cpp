/***************************************************************************
                          main.cpp
                          -------------------
    begin                : Fri Mar 3 18:49:53 CET 2000
    copyright            : (C) 1993..2014 by Manfred Morgner
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "generic.h"

#include "odb.h"

using namespace odb;

void SearchAtoms(CODB& odb, const std::string& sSearchKey)
  {
  std::cout << "Search for atoms named " << sSearchKey << "'" << std::endl;

  CVectorRoot oVector = odb.GetAtom( sSearchKey );
  for (auto it : oVector)
    {
    std::cout << "ATOM-ID=" << it->ID() << ", content=" << ((CAtom*)it)->UIFormat() << std::endl;
    } // for (auto it : oVector)
  std::cout << std::endl;
  } // void SearchAtoms(CODB& odb, const std::string& sSearchKey)


void SearchObjects(CODB& odb, const std::string& sSearchKey)
  {
  std::cout << "Search for objects named '" << sSearchKey << "'" << std::endl;

  CVectorRoot oVector = odb.GetObject( sSearchKey );
  for (auto it : oVector)
    {
    std::cout << "OBJ-ID=" << it->ID() << ", name=" << it->NameGet() << std::endl;
    } // for (auto itVR : oVector)
  std::cout << std::endl;
  } // void SearchObjects(CODB& odb, const std::string& sSearchKey)

void uiPaintMenu()
  {
  std::cout << "0) Database -> INIT (clear)" << std::endl;
  std::cout << "1) Database -> ADD DATA (auto)" << std::endl;
  std::cout << "2) Database -> DUMP ALL OBJECTS" << std::endl;
  std::cout << "3) SAVE" << std::endl;
  std::cout << "4) LOAD (additive)" << std::endl;
  std::cout << "5) Add binary Data" << std::endl;
  std::cout << "6) Search Atoms by Name inside a known single object" << std::endl;
  std::cout << "7) Search Atoms by UserSign inside a known single object" << std::endl;
  std::cout << "8) Get binary data from an atom" << std::endl;
  std::cout << "9) Add objects with fixed IDs (to the empty odb)" << std::endl;
  std::cout << "a) Search odb for Atoms by Atom name" << std::endl;
  std::cout << "b) Search odb for Objects by Object name" << std::endl;
  std::cout << "------------------------------" << std::endl;
  std::cout << "Q) Quit" << std::endl;
  std::cout << "X) eXit" << std::endl;
  }

void uiSearchAtomInObjectByName()
  {
  std::cout << "3 named Atoms on one Object, Hit for Atom 2 and 3" << std::endl;
  std::cout << ">>-------------- creating  ---------------->>" << std::endl;

  CObject o(_TEXT("Django"));
  o.AtomAdd( new CAtom( "kalando",  1.201) );
  o.AtomAdd( new CAtom( "kalender", 1.202 ) );
  o.AtomAdd( new CAtom( "sender",   1.203 ) );

  CVectorAtom& oAllAtoms = const_cast<CVectorAtom&>(o.AtomsGet());
  for (auto itAtom : oAllAtoms)
    {
    std::cout << "Atom : "  << itAtom->UIFormat()
              << "; Name: " << itAtom->NameGet()
              << std::endl;
    } // for (auto itAtom : oAllAtoms)

  std::cout << "<<-------------- searching ----------------<<" << std::endl;
  std::cout << "Searching for 'ender' in Atom names" << std::endl;


  CVectorAtom oVA = o.AtomGet("ender", true);

  std::cout << "<<-------------- dumping   ----------------<<" << std::endl;

  for (auto it : oVA)
    {
    std::cout << "Atom : "  << it->UIFormat()
              << "; Name: " << it->NameGet()
              << std::endl;
    } // for (auto it : oVA)

  std::cout << "<<-------------- end.      ----------------<<" << std::endl;
  std::cout << std::endl;
  }

void uiSearchAtomInObjectByUserSign()
  {
  std::cout << "3 UserMarked Atoms on one Object, Hit for Atom 2 and 3" << std::endl;
  std::cout << ">>-------------- creating  ---------------->>" << std::endl;

  CObject o(_TEXT("Django"));
  CAtom* poAtom;
  poAtom = new CAtom( "A", 17L);  o.AtomAdd( poAtom );  poAtom->UserSignSet(0x0813);
  poAtom = new CAtom( "B", 1.7);  o.AtomAdd( poAtom );  poAtom->UserSignSet(0x0F03);
  poAtom = new CAtom( "C"     );  o.AtomAdd( poAtom );  poAtom->UserSignSet(0x0713);

  CVectorAtom& oAllAtoms = const_cast<CVectorAtom&>(o.AtomsGet());
  for (auto itAtom : oAllAtoms)
    {
    CMapReferencing& oRM = const_cast<CMapReferencing&>(itAtom->ParentGet());
    for (auto itRObj : oRM)
      {
      std::cout << "Atom: "     << itAtom->NameGet()
                << "; RTTI : "  << itAtom->InfoGetRtti()
                << "; Objekt: " << itRObj.first->NameGet()
                << "; RTTI : "  << itRObj.first->InfoGetRtti()
                << "; Check: "  << (itAtom->UserSignGet() & 0x0700)
                << std::endl;
      } // for (auto itRObj : oRM)
    } // for (auto itAtom : oAllAtoms)

  std::cout << "<<-------------- searching ----------------<<" << std::endl;
  std::cout << "Searching for 0x0700 in Atom UserSigns" << std::endl;

// simple:  CVectorAtom oVA = o.AtomGetBySign((short)0x0F03);

  // queries atoms by specified BITs in the UserSign
  CVectorAtom oVA = o.AtomGetBySign(0x0700, 0x0700);

  std::cout << "<<-------------- dumping   ----------------<<" << std::endl;

  for (auto itAtom : oVA)
    {
    std::cout << itAtom->NameGet() << " - " << itAtom->UIFormat() << std::endl;
    }

  std::cout << "<<-------------- end.      ----------------<<" << std::endl;
  std::cout << std::endl;
  }

void uiOdbDump(CODB& odb)
  {
  std::cout << "" << std::endl;
  std::cout << ">>--------------odb dump---------------->>" << std::endl;
  odb.Dump();
  std::cout << "<<--------------odb dump----------------<<" << std::endl;
  std::cout << std::endl;
  }

void uiOdbAddData(CODB& odb)
  {
  CClass*  poClassAdr = odb.NewClass (_TEXT("Person"));
  CReason* poReason1  = odb.NewReason(_TEXT("Mutter"),     poClassAdr, poClassAdr);
  CReason* poReason2  = odb.NewReason(_TEXT("Vater"),      poClassAdr, poClassAdr);
  CReason* poReason3  = odb.NewReason(_TEXT("Kind"),       poClassAdr, poClassAdr);
  CReason* poReason4  = odb.NewReason(_TEXT("Ehepartner"), poClassAdr, poClassAdr);

  CObject* poObject = odb.NewObject(_TEXT("Mario Werder"));
  poObject->AtomAdd( odb.Add(new CAtom(_TEXT("Vorname"), _TEXT("Mario"))) );
  *poObject = poClassAdr;


  CObject* poObject2 = odb.NewObject(_TEXT("Ilse Werder-Gross"), poClassAdr);

  CAtom* poAtom = odb.NewAtom("Höhe");
  *poAtom = 1.67;
  poAtom->FormatSet("%.2f");
  poAtom->PrefixSet("+");
  poAtom->SuffixSet("m");

  poObject2->AtomAdd( poAtom );
  poObject2->AtomAdd( odb.Add(new CAtom(_TEXT("Vorname"), _TEXT("Ilse"))) );
  poObject2->AtomAdd( odb.Add(new CAtom(_TEXT("Name"),    _TEXT("Werder-Gross"))) );
  poObject2->AtomAdd( odb.Add(new CAtom(_TEXT("Strasse"), _TEXT("Kleine Gasse 1"))) );


  CObject* poObject3 = new CObject(_TEXT("Alfons Gross"), poClassAdr);
  poObject3->AtomAdd( odb.Add(new CAtom(_TEXT("Vorname"), _TEXT("Alfons"))) );
  poObject3->AtomAdd( odb.Add(new CAtom(_TEXT("Name"), _TEXT("Gross"))) );

  odb.Add(poObject3);

  poObject ->Link(poObject2, poReason1);
  poObject ->Link(poObject2, poReason1); // test: double entry should be ignored
  poObject ->Link(poObject3, poReason2);
  poObject2->Link(poObject,  poReason3);
  poObject3->Link(poObject,  poReason3);

  poObject2->Link(poObject3, poReason4);
  poObject3->Link(poObject2, poReason4);
  } // void uiOdbAddData(CODB& odb)

int main (int argc, char * const argv[])
//int main(int argc, char *argv[])
  {
  char asz[255];
  asz[0] = '0';
  CODB* pOdb = new CODB;
  bool bRun = true;
  while (bRun == true)
    {
    uiPaintMenu();
    std::cin >> asz[0];
    std::cout << std::endl;
    switch ( asz[0] )
      {
      case '0': // INIT
        std::cout << "Initialize odb" << std::endl;
        delete pOdb;
        pOdb = new CODB;
        break;

      case '1': // ADD DATA
        std::cout << "Add data to odb" << std::endl;
        try
          {
          uiOdbAddData(*pOdb);
          }
        catch(...)
          {
          std::cout << "EXECPTION CATCHED (GPF)" << std::endl << std::endl;
          }
        break;

      case '2': // DUMP
        std::cout << "Dump Objects in odb" << std::endl;
        try
          {
          uiOdbDump(*pOdb);
          }
        catch(...)
          {
          std::cout << "EXECPTION CATCHED (GPF)" << std::endl << std::endl;
          }
        break;

      case '3': // SAVE
        std::cout << "Save odb" << std::endl;
        try
          {
          pOdb->Save("torre.odb");
          }
        catch(...)
          {
          std::cout << "EXECPTION CATCHED (GPF)" << std::endl << std::endl;
          }
        break;

      case '4': // LOAD
        std::cout << "Load odb" << std::endl;
        try
          {
          delete pOdb;
          pOdb = new CODB;
          pOdb->Load("torre.odb");
          }
        catch(...)
          {
          std::cout << "EXECPTION CATCHED (GPF)" << std::endl << std::endl;
          }
        break;

      case '5': // LOAD
        std::cout << "Add binary data (4096 bytes)" << std::endl;
        try
          {
          char ac[4096];
          long nSize = 4096;
          pOdb->Add( new CAtom( _TEXT("BINAER"), ac, nSize) );
          }
        catch(...)
          {
          std::cout << "EXECPTION CATCHED (GPF)" << std::endl << std::endl;
          }
        break;

      case '6': // SEARCH ATOM(s) IN OBJECT BY NAME
        std::cout << "Search Atoms by Name inside a known single object" << std::endl;
        try
          {
          uiSearchAtomInObjectByName();
          }
        catch(...)
          {
          std::cout << "EXECPTION CATCHED (GPF)" << std::endl << std::endl;
          }
        break;

      case '7': // SEARCH ATOM(s) IN OBJECT BY USER-SIGN
        std::cout << "Search Atoms by UserSign inside a known single object" << std::endl;
        try
          {
          uiSearchAtomInObjectByUserSign();
          }
        catch(...)
          {
          std::cout << "EXECPTION CATCHED (GPF)" << std::endl << std::endl;
          }
        break;

      case '8': // GET BINARY DATA FROM AN ATOM
        std::cout << "Get binary data from an atom" << std::endl;
        try
          {
          // make an atom
					CAtom oAtom;
          // add data
          oAtom = _TEXT("'these are the binary data, in this case a string'");

          std::cout << "Data bytes: " << oAtom.BinarySizeGet() << std::endl;

          // create an buffer
          const long nSizeBuffer = 512;
          unsigned char aucBuffer[nSizeBuffer];

          long nSizeGivenData = oAtom.BinaryDataGet(aucBuffer, nSizeBuffer);

          std::cout << "Given size: " << nSizeGivenData << std::endl;
          std::cout << "Given data: " << (char*)aucBuffer << std::endl << std::endl;
          }
        catch(...)
          {
          std::cout << "EXECPTION CATCHED (GPF)" << std::endl << std::endl;
          }
        break;

      case '9': // ADD OBJECT USING PREDEFINED ID
        std::cout << "Add objects with fixed IDs (to the empty odb)" << std::endl << std::endl;
        try
          {
          delete pOdb;
          pOdb = new CODB;

          CObject* poObject = new CObject(1234, false);
          std::cout << "  1) ID before Add().....: " << poObject->ID() << std::endl;

          poObject = pOdb->Add( poObject );
          std::cout << "  2) ID after Add()......: " << poObject->ID() << std::endl;
          std::cout << "  3) Is2PtrObject( 1234 ): " << pOdb->Id2PtrObject( 1234 ) << std::endl << std::endl;

          poObject = pOdb->Add( new CObject(4711, false) );
          std::cout << "  a) ID after Add( new )..: " << poObject->ID() << std::endl;
          std::cout << "  b) Is2PtrObject( 4711 ): " << pOdb->Id2PtrObject( 4711 ) << std::endl << std::endl;
          }
        catch(...)
          {
          std::cout << "EXECPTION CATCHED (GPF)" << std::endl << std::endl;
          }
        break;

      case 'a': // SEARCH ATOMS BY NAME
        SearchAtoms(*pOdb, _TEXT("Vorname") );
        SearchAtoms(*pOdb, _TEXT("Name") );
        break;

      case 'b': // SEARCH OBJECTS BY NAME
        SearchObjects(*pOdb, _TEXT("Ilse Werder-Gross") );
        SearchObjects(*pOdb, _TEXT("Mario Werder") );
        break;

              
      case 'Q':
      case 'q':
      case 'X':
      case 'x':
        std::cout << "Leave demo" << std::endl;
        bRun = false;
        break;
      } // switch asz[0]
    } // while (1)

/*
  SearchAtoms(odb, _TEXT("Vorname") );
  SearchAtoms(odb, _TEXT("Name") );

  SearchObjects(odb, _TEXT("Ilse Werder-Gross") );
  SearchObjects(odb, _TEXT("Mario Werder") );


  CObject* poFO = odb.Id2PtrObject(2);

  if ( poFO != 0 )
    {
    poFO->Dump();
    }

  odb.ObjectAddAtom(poObject, odb.Add( new CAtom( _TEXT("ACE-0705"),  1798L)));
                              odb.Add( new CAtom( _TEXT("ACE-0705"),    23L));
                              odb.Add( new CAtom( _TEXT("ACE-0705"), 31992L));
                              odb.Add( new CAtom( _TEXT("ACE-0705"),   419L));

  std::cout << "building rope '0750'" << std::endl;
  CVectorRoot voAtoms = odb.GetAtom( _TEXT("ACE-0705") );
  CRope* poRope = odb.NewRope( _TEXT("0705"), (CAtom*) voAtoms[0] );
  for (uint iAR = 0; iAR < voAtoms.size() -1; iAR++)
    {
    CAtom* poAtom1 = (CAtom*)voAtoms[iAR];
    CAtom* poAtom2 = (CAtom*)voAtoms[iAR+1];
    std::cout << poAtom1->UIFormat() << " -> " << poAtom2->UIFormat() << std::endl;
    poAtom1->RopeLinkAdd( poRope, poAtom2 );
    }

//  odb.Save("terro.odb");

  std::cout << "Atoms in 'poObject': " << poObject->AtomCountGet() << std::endl;

  const CVectorAtom& coVA = poObject->AtomsGet();
  std::cout << std::endl << "Atoms of object " << poObject->NameGet() << std::endl;
  for (uint nVA = 0; nVA < coVA.size(); nVA++)
    {
    std::cout << coVA[nVA]->NameGet() << ": " << coVA[nVA]->UIFormat() << std::endl;
    }

  CAtom* poEA = poObject->AtomGet(1);
  std::cout << std::endl << "Atoms 1 of object " << poObject->NameGet() << std::endl;
  std::cout << poEA->NameGet() << ": " << poEA->UIFormat() << std::endl;
*/
  return 0;
  } // int main(int argc, char *argv[])
