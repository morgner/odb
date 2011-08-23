/***************************************************************************
                          main.cpp
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "generic.h"
#include "odb.h"

using namespace odb;
using namespace std;

void SearchAtoms(CODB& odb, const string& sSearchKey)
  {
  CVectorRoot oVector = odb.GetAtom( sSearchKey );

  cout << "SEARCH FOR ATOMS NAMED " << sSearchKey << "; RESULT:" << endl;
  for ( CVectorRoot::iterator itVR = oVector.begin(); itVR != oVector.end(); ++itVR)
    {
    cout << "ATOM-ID=" << (*itVR)->ID() << ", content= " << ((CAtom*)(*itVR))->UIFormat() << endl;
    }
  cout << endl;
  } // void SearchAtoms(const string& sSearchKey)


void SearchObjects(CODB& odb, const string& sSearchKey)
  {
  CVectorRoot oVector = odb.GetObject( sSearchKey );

  cout << "SEARCH FOR OBJECTS NAMED " << sSearchKey << "; RESULT:" << endl;
  for ( CVectorRoot::iterator itVR = oVector.begin(); itVR != oVector.end(); ++itVR)
    {
    cout << "ID=" << (*itVR)->ID() << endl;
    }
  cout << endl;
  } // void SearchAtoms(const string& sSearchKey)

void uiPaintMenu()
  {
  cout << "0) Database -> INIT (clear)" << endl;
  cout << "1) Database -> ADD DATA (auto)" << endl;
  cout << "2) Database -> DUMP ALL OBJECTS" << endl;
  cout << "3) SAVE" << endl;
  cout << "4) LOAD (additive)" << endl;
  cout << "5) Add binary Data" << endl;
  cout << "6) Search Atoms by Name inside a known single object" << endl;
  cout << "7) Search Atoms by UserSign inside a known single object" << endl;
  cout << "8) Get binary data from an atom" << endl;
  cout << "9) Add objects with fixed IDs (to the empty odb)" << endl;
  cout << "------------------------------" << endl;
  cout << "Q) Quit" << endl;
  cout << "X) eXit" << endl;
 }

void uiSearchAtomInObjectByName( CODB& odb, const string& sString )
  {
  cout << "" << endl;

  cout << "<<-------------- searching ----------------<<" << endl;

  CVectorRoot oVA = odb.GetAtom(_T(sString));

  cout << "<<-------------- dumping   ----------------<<" << endl;

  for ( CVectorRoot::iterator it = oVA.begin(); it != oVA.end(); ++it )
    {
    cout << dynamic_cast<CAtom*>(*it)->UIFormat() << endl;
    }

  cout << "<<-------------- end.      ----------------<<" << endl;
  cout << endl;
  }

void uiSearchAtomInObjectByUserSign()
  {
  cout << "" << endl;
  cout << ">>-------------- creating  ---------------->>" << endl;

  CObject o(_T("Django"));
  CAtom* poAtom;
  poAtom = new CAtom( "A", 17L);  o.AtomAdd( poAtom );  poAtom->UserSignSet(0x0813);
  poAtom = new CAtom( "B", 1.7);  o.AtomAdd( poAtom );  poAtom->UserSignSet(0x0F03);
  poAtom = new CAtom( "C"     );  o.AtomAdd( poAtom );  poAtom->UserSignSet(0x0713);

  CVectorAtom&          oAllAtoms = const_cast<CVectorAtom&>(o.AtomsGet());
  CVectorAtom::iterator it;
  for ( it  = oAllAtoms.begin(); it != oAllAtoms.end(); ++it )
    {
    CMapReferencing& cRM = const_cast<CMapReferencing&>((*it)->ParentGet());
    for ( CMapReferencing::iterator itO = cRM.begin(); itO != cRM.end(); ++itO )
      {
      cout << "Atom: " << (*it)->NameGet() << "; RTTI : " << (*it)->InfoGetRtti()
           << "; Objekt: " << itO->first->NameGet() << "; RTTI : " << itO->first->InfoGetRtti() << endl;
      } // for ( CMapReferencing::iterator itO = cRM.begin(); itO != cRM.end(); ++itO )
    } // for ( it  = oAllAtoms.begin(); it != oAllAtoms.end(); ++it )

  cout << "<<-------------- searching ----------------<<" << endl;

// simple:  CVectorAtom oVA = o.AtomGetBySign((short)0x0F03);

  // queries atoms by specified BITs in the UserSign
  CVectorAtom oVA = o.AtomGetBySign(0x0700, 0x0700);

  cout << "<<-------------- dumping   ----------------<<" << endl;

  for ( it = oVA.begin(); it != oVA.end(); ++it )
    {
    cout << (*it)->UIFormat() << endl;
    }

  cout << "<<-------------- end.      ----------------<<" << endl;
  cout << endl;
  }

void uiOdbDump(CODB& odb)
  {
  cout << "" << endl;
  cout << ">>--------------odb dump---------------->>" << endl;
  odb.Dump();
  cout << "<<--------------odb dump----------------<<" << endl;
  cout << endl;
  }

bool parseWordFile ( const string& sFilename, CODB& odb, CClass* poClass )
{
    ifstream in(sFilename.c_str());
    if (!in)
      {  
      std::cerr << sFilename << ": error while opening" << std::endl;
      return false;
      }
    
    string sWord;
    
    while (!in.eof())
      {
      in >> sWord;
      if ( (sWord[0] == '.') || (sWord[0] == '#') || (sWord[0] == '-') ) continue;
      std::cout << "Adding " << sWord << endl;
      CObject* poObject = odb.NewObject(_T(sWord),poClass);
      poObject->AtomAdd(odb.Add(new CAtom(_T("Transcription"),_T(sWord))));
      }
    return true;
}

void uiOdbAddData(CODB& odb)
  {
  CClass*  poClassAdrDE = odb.NewClass (_T("Deutsch"));
  CClass*  poClassAdrEN = odb.NewClass (_T("English"));

//CReason* poReason1=odb.NewReason(_T("DESynonym"),poClassAdrDE, poClassAdrDE);
//  CReason* poReason2=odb.NewReason(_T("DeInEn"),poClassAdrDE,poClassAdrEN);

  parseWordFile ( "deutsch.txt", odb, poClassAdrDE );    
  parseWordFile ( "english.txt", odb, poClassAdrEN );    
  } // void uiOdbAddData(CODB& odb)

int main(int argc, char *argv[])
  {
  char asz[255];
  asz[0] = '0';
  CODB* pOdb = new CODB;
  bool bRun = true;
  while (bRun == true)
    {
    uiPaintMenu();
    cin >> asz[0];
    cout << endl;
    switch ( asz[0] )
      {
      case '0': // INIT
        cout << "Initialize odb" << endl;
        delete pOdb;
        pOdb = new CODB;
        break;

      case '1': // ADD DATA
        cout << "Add data to odb" << endl;
        try
          {
          uiOdbAddData(*pOdb);
          }
        catch(...)
          {
          cout << "EXECPTION CATCHED (GPF)" << endl << endl;
          }
        break;

      case '2': // DUMP
        cout << "Dump Objects in odb" << endl;
        try
          {
          uiOdbDump(*pOdb);
          }
        catch(...)
          {
          cout << "EXECPTION CATCHED (GPF)" << endl << endl;
          }
        break;

      case '3': // SAVE
        cout << "Save odb" << endl;
        try
          {
          pOdb->Save("torre.odb");
          }
        catch(...)
          {
          cout << "EXECPTION CATCHED (GPF)" << endl << endl;
          }
        break;

      case '4': // LOAD
        cout << "Load odb" << endl;
        try
          {
          delete pOdb;
          pOdb = new CODB;
          pOdb->Load("torre.odb");
          }
        catch(...)
          {
          cout << "EXECPTION CATCHED (GPF)" << endl << endl;
          }
        break;

      case '5': // LOAD
        cout << "Add binary data (4096 bytes)" << endl;
        try
          {
          char ac[4096];
          long nSize = 4096;
          pOdb->Add( new CAtom( _T("BINAER"), ac, nSize) );
          }
        catch(...)
          {
          cout << "EXECPTION CATCHED (GPF)" << endl << endl;
          }
        break;

      case '6': // SEARCH ATOM(s) IN OBJECT BY NAME
        cout << "Search Atoms by Name inside a known single object" << endl;
        try
          {
          std::string sSearch;
          std::cout << "Searchstring: " << std::flush;
          std::cin >> sSearch;
          
          uiSearchAtomInObjectByName(*pOdb, sSearch);
          }
        catch(...)
          {
          cout << "EXECPTION CATCHED (GPF)" << endl << endl;
          }
        break;

      case '7': // SEARCH ATOM(s) IN OBJECT BY USER-SIGN
        cout << "Search Atoms by UserSign inside a known single object" << endl;
        try
          {
          uiSearchAtomInObjectByUserSign();
          }
        catch(...)
          {
          cout << "EXECPTION CATCHED (GPF)" << endl << endl;
          }
        break;

      case '8': // GET BINARY DATA FROM AN ATOM
        cout << "Get binary data from an atom" << endl;
        try
          {
          // make an atom
          CAtom oAtom;
          // add data
          oAtom = _T("'these are the binary data, in this case a string'");

          cout << "Data bytes: " << oAtom.BinarySizeGet() << endl;

          // create an buffer
          const long nSizeBuffer = 512;
          unsigned char aucBuffer[nSizeBuffer];

          long nSizeGivenData = oAtom.BinaryDataGet(aucBuffer, nSizeBuffer);

          cout << "Given size: " << nSizeGivenData << endl;
          cout << "Given data: " << (char*)aucBuffer << endl << endl;
          }
        catch(...)
          {
          cout << "EXECPTION CATCHED (GPF)" << endl << endl;
          }
        break;

      case '9': // ADD OBJECT USING PREDEFINED ID
        cout << "Add objects with fixed IDs (to the empty odb)" << endl << endl;
        try
          {
          delete pOdb;
          pOdb = new CODB;

          CObject* poObject = new CObject(1234, false);
          cout << "  1) ID before Add().....: " << poObject->ID() << endl;

          poObject = pOdb->Add( poObject );
          cout << "  2) ID after Add()......: " << poObject->ID() << endl;
          cout << "  3) Is2PtrObject( 1234 ): " << pOdb->Id2PtrObject( 1234 ) << endl << endl;

          poObject = pOdb->Add( new CObject(4711, false) );
          cout << "  a) ID after Add( new )..: " << poObject->ID() << endl;
          cout << "  b) Is2PtrObject( 4711 ): " << pOdb->Id2PtrObject( 4711 ) << endl << endl;
          }
        catch(...)
          {
          cout << "EXECPTION CATCHED (GPF)" << endl << endl;
          }
        break;

      case 'Q':
      case 'q':
      case 'X':
      case 'x':
        cout << "Leave demo" << endl;
        bRun = false;
        break;
      } // switch asz[0]
    } // while (1)

  return 0;
  } // int main(int argc, char *argv[])
