/****************************************************************************
    NL3W netlist utility for BSch3V
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/
#include "stdafx.h"

#include <stdio.h>
#include <assert.h>
#include <limits.h>

#include "xbsch.h"
#include "xbschstringlist.h"
#include "ce3io.h"
#include "xbschobj.h"
#include "xbschline.h"
#include "xbschwirenl.h"


SXBSchWireNl::SXBSchWireNl(SXBSchWire& w)
{
	setP1(w.p1());
	setP2(w.p2());
}

SXBSchWireNl* SXBSchWireNl::divide(const SPoint& pt)
{
	SXBSchWireNl* pNewWire = NULL;
	if(pt.x() == m_p1.x() && pt.x() == m_p2.x()){
		if(pt.y() >= m_p1.y() && pt.y() >= m_p2.y()) return NULL;
		if(pt.y() <= m_p1.y() && pt.y() <= m_p2.y()) return NULL;
		pNewWire = new SXBSchWireNl(*this);
	}else if(pt.y() == m_p1.y() && pt.y() == m_p2.y()){
		if(pt.x() >= m_p1.x() && pt.x() >= m_p2.x()) return NULL;
		if(pt.x() <= m_p1.x() && pt.x() <= m_p2.x()) return NULL;
		pNewWire = new SXBSchWireNl(*this);
	}
	if(pNewWire != NULL){
		m_p2=pt;
		pNewWire->setP1(pt);
	}
	return pNewWire;
}
