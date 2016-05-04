/****************************************************************************
    BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/


#include "stdafx.h"
#include <limits.h>
#include "coordf.h"



//////////////////////////
//点座標クラス
SPointF::SPointF()
{
	m_x=0.0;
	m_y=0.0;
}

SPointF::SPointF(double x,double y)
{
	m_x=x;
	m_y=y;
}

void SPointF::setX(double x)
{
	m_x=x;
}


void SPointF::setY(double y)
{
	m_y=y;
}


double SPointF::x() const
{
	return m_x;
}


double SPointF::y() const
{
	return m_y;
}

SPointF& SPointF::operator += (const SPointF& p)
{
	m_x+=p.x();
	m_y+=p.y();
	return *this;
}

SPointF& SPointF::operator -= (const SPointF& p)
{
	m_x-=p.x();
	m_y-=p.y();
	return *this;
}

SPointF& SPointF::operator += (const SPoint& p)
{
	m_x+=p.x();
	m_y+=p.y();
	return *this;
}

SPointF& SPointF::operator -= (const SPoint& p)
{
	m_x-=p.x();
	m_y-=p.y();
	return *this;
}


bool operator == (const SPointF& p1,const SPointF& p2)
{
	return (p1.x()==p2.x() && p1.y()==p2.y());
}

bool operator != (const SPointF& p1,const SPointF& p2)
{
	return (p1.x()!=p2.x() || p1.y()!=p2.y());
}
	
