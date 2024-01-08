/****************************************************************************
    BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

#ifndef COORDF_H
#define COORDF_H

#include "coord.h"

//�_���W�N���X
class SPointF
{
protected:
	double m_x;
	double m_y;
public:
	SPointF();
	SPointF(double x,double y);
	void setX(double x);
	void setY(double y);
	double x() const;
	double y() const;
	SPointF& operator += (const SPointF&);
	SPointF& operator -= (const SPointF&);
	SPointF& operator += (const SPoint&);
	SPointF& operator -= (const SPoint&);

	//XY���S���W���w�肵�āA��]���s��
	SPointF rotatePoint(SPointF pt,SPointF pCenter);

	//X���W���w�肵�č��E���]���s��
	SPointF mirrorPoint(SPointF pt,double xCenter);
};


bool operator == (const SPointF& p1,const SPointF& p2);
bool operator != (const SPointF& p1,const SPointF& p2);


/*
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
*/

#endif