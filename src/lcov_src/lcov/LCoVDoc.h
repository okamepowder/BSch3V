/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

#pragma once

#include <list>
#include <string>
using namespace std;
#include "complib.h"

class SLCoVDoc
{
public:
	SLCoVDoc(void);
	virtual ~SLCoVDoc(void);

	SCompLib m_CompLib;
	bool m_bDirty;

};
