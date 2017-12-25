#ifndef __GENERIC_H
#define __GENERIC_H

/***************************************************************************
                          generic.h  -  description
                             -------------------
    begin                : Mon Mar 13 2000
    copyright            : (C) 1993..2011 by Manfred Morgner
    email                : manfred@morgner.com
 ***************************************************************************/


#include <string>
#include <set>
#include <map>
#include <iostream>
#include <fstream>
#include <memory>   // make_unique
//#include <literals> // ""s

using namespace std::string_literals;

namespace odb
  {

  #define ODB_SUCCESS           0L
  #define ODB_FAILURE          -1L

  #define ODB_ROPE_RECURSION   -100L
  #define ODB_UNKNOWN_INSTANCE -101L

  } // namespace odb

#include <sys/time.h>

bool operator > (const timeval& t1, const timeval& t2);

/*
#ifndef _TEXT
#  define _TEXT(s) (const std::string&) s
#endif // _TEXT
*/
 // __GENERIC_H
#endif
