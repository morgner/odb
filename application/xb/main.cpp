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

CKeyNameIndex g_soWordIndex;

long WordIndexItemAdd(CKeyNameIndex& rsoKeyNameIndex, CAtom* poAtom)
  {
  CKeyName oKeyName(poAtom->UIFormat());
  CKeyNameIndex::iterator it = rsoKeyNameIndex.find( &oKeyName );
  if ( it == rsoKeyNameIndex.end() )
    {
    CKeyName* poKeyName = new CKeyName(poAtom->UIFormat());
    pair<CKeyNameIndex::iterator, bool> oResult = rsoKeyNameIndex.insert( poKeyName );
    it = oResult.first;
    if ( oResult.second == false )
      {
      delete poKeyName;
      }
    } // if ( it == rsoKeyNameIndex.end() )

  CKeyName::iterator itItem = (*it)->find(poAtom);
  if ( itItem == (*it)->end() )
    {
    (*it)->push_back(poAtom);
    } // if ( itItem == it->end() )

  return ODB_SUCCESS;
  } // long WordIndexItemAdd(CKeyNameIndex& rsoKeyNameIndex, CAtom* poAtom)





void uiPaintMenu()
  {
  cout << "0) Database -> INIT (clear)" << endl;
  cout << "1) Database -> ADD DATA (from deutsch.txt)" << endl;
  cout << "2) Database -> DUMP ALL OBJECTS" << endl;
  cout << "3) SAVE" << endl;
  cout << "4) LOAD (additive)" << endl;
  cout << "5) Search word" << endl;
  cout << "------------------------------" << endl;
  cout << "Q) Quit" << endl;
  cout << "X) eXit" << endl;
 }

void uiSearchWord(const string& sSearchKey)
  {
  CKeyName oKeyName(sSearchKey);
  CKeyNameIndex::iterator it = g_soWordIndex.find( &oKeyName );

  if ( it == g_soWordIndex.end() )
    {
    cout << "not found" << endl;
    } // if ( it == g_soWordIndex.end() )
  else
    {
//    cout << "found Atom " << ((CAtom*)*it)->ID()<< endl;
    cout << "found Atom(s) " << endl;

  for (CKeyName::iterator itItem = (*it)->begin(); itItem != (*it)->end(); ++itItem)
    {
    cout << "  Atom: " << (*itItem)->ID() << endl;
    } // if ( itItem == it->end() )



    } // else if ( it == g_soWordIndex.end() )
  } // void SearchAtoms(const string& sSearchKey)

/*
void uiSearchWord( CODB& odb, const string& sString )
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
*/

void uiOdbDump(CODB& odb)
  {
  cout << "" << endl;
  cout << ">>--------------odb dump---------------->>" << endl;
  odb.Dump();
  cout << "<<--------------odb dump----------------<<" << endl;
  cout << endl;
  }

bool parseWordFile ( const string& sFilename, CODB& odb )
{
    ifstream in(sFilename.c_str());
    if (!in)
    {
        std::cerr << sFilename << ": error while opening" << std::endl;
        return false;
    }

    CClass*  poClassLANG         = odb.NewClass ("Sprache");
    CObject* poObjectDE          = odb.NewObject("Deutsch", poClassLANG);
    CAtom*   poADOTranscription  = odb.Add(new CAtom(_T("Transcription"), true));
    string sWord;

    std::cout << "Loading... " << endl;

    while (!in.eof())
      {
      in >> sWord;
      if ( (sWord[0] == '.') || (sWord[0] == '#') || (sWord[0] == '-') ) continue;
      std::cout << "Adding " << sWord << endl;
      CAtom* poAtom = new CAtom(_T("W"), _T(sWord), poADOTranscription);
      odb.Add(poAtom);
      odb.ObjectAddAtom(poObjectDE, poAtom);

      WordIndexItemAdd(g_soWordIndex, poAtom);

/*
        CObject* poObject = odb.NewObject(_T(sWord),poClass);
        poObject->AtomAdd(odb.Add(new CAtom(_T("Transcription"),_T(sWord))));
*/
      }

    std::cout << endl << "Done. " << endl;

    return true;
}

void uiOdbAddData(CODB& odb)
  {
  parseWordFile ( "deutsch.txt", odb );
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

      case '5': // Search a word
        {
        cout << "Searching for this word: "; // << endl;
	string sWord;
	cin >> sWord;
	uiSearchWord(sWord);
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

