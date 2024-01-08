/****************************************************************************
    NL3W netlist utility for BSch3V
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#ifndef XBSCHSTRINGLIST_H
#define XBSCHSTRINGLIST_H
#ifdef _MSVC
	#pragma warning (disable: 4786)
#endif
#include <string>
#include <list>
using namespace std;
typedef list<wstring> wstringList;
typedef list<wstring>::iterator wstringListIterator;
typedef list<string> stringList;
typedef list<string>::iterator stringListIterator;


#endif