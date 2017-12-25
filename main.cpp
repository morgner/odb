/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Thu Mai 10 01:56:53 CEST 2005
    copyright            : (C) 1993..2005 by Manfred Morgner
    email                : manfred@morgner.com
 ***************************************************************************/


#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include "generic.h"

#include "odb.h"

using namespace odb;

int main (int argc, char * const argv[])
  {
  CODB odb;

//  CClass*  poClassAdr = odb.NewClass (_TEXT("Person"));
  CClass*  poClassWrd = odb.NewClass (_TEXT("Word"));

  for (int n=0; n < 40; n++)
    {
    CObject* poObject = odb.NewObject(_TEXT("Wort"), poClassWrd);
    poObject->AtomAdd( odb.Add(new CAtom(_TEXT("en"), _TEXT("Cinema"))) );
    poObject->AtomAdd( odb.Add(new CAtom(_TEXT("de"), _TEXT("Kino"))) );
    }

  odb.SaveXML("wb-40.odb");

  return 0;
  } // int main (int argc, char * const argv[])

