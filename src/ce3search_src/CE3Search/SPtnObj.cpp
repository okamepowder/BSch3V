/****************************************************************************
    BSch3V schematic capture
	Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#include "stdafx.h"
#include "sptnobj.h"

SPtnObj::SPtnObj(void)
{
	m_style = 0;
	m_width = 1;
	m_nFill = -1;
}

SPtnObj::~SPtnObj(void)
{
}


//XY���S���W���w�肵�āA��]���s��
SPoint SPtnObj::rotatePoint(SPoint pt,SPoint pCenter)
{
	int newX,newY;
	newY = pCenter.y() + (pt.x() - pCenter.x()) ;
	newX = pCenter.x() - (pt.y() - pCenter.y()) ;

	return SPoint(newX,newY);
}

//X���W���w�肵�č��E���]���s��
SPoint SPtnObj::mirrorPoint(SPoint pt,int xCenter)
{
	int newX;
	newX = xCenter*2 -pt.x();
	return SPoint(newX,pt.y());
}

//p0��p1��[�_���W�Ƃ������p�̏d�Ȃ���e�X�g
bool SPtnObj::testIntersectLinePoint(const SPoint& p0,const SPoint& p1,const SPoint& p)
{
	int l=p.x()-1;
	int t=p.y()-1;
	int r=p.x()+1;
	int b=p.y()+1;
	SRect rcArea = SRect(p0,p1);
	rcArea.normalize();

	if( (l>rcArea.r()) ||  (t>rcArea.b()) || (r<rcArea.l()) ||  (b<rcArea.t()) ) return false;

	double dA =((double)p1.y()-(double)p0.y())/((double)p1.x()-(double)p0.x());
	double dB =(double)p0.y() - dA*(double)p0.x();

	double y0 =dA*l+dB;
	double y1 =dA*r+dB;

	if((y0>b && y1>b) || (y0<t && y1<t)) return false;

	return true;
}
