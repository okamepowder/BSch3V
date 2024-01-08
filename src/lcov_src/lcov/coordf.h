/****************************************************************************
    BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

#ifndef COORDF_H
#define COORDF_H

#include "coord.h"

//点座標クラス
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

	//XY中心座標を指定して、回転を行う
	SPointF rotatePoint(SPointF pt,SPointF pCenter);

	//X座標を指定して左右反転を行う
	SPointF mirrorPoint(SPointF pt,double xCenter);
};


bool operator == (const SPointF& p1,const SPointF& p2);
bool operator != (const SPointF& p1,const SPointF& p2);


/*
//XY中心座標を指定して、回転を行う
SPoint SPtnObj::rotatePoint(SPoint pt,SPoint pCenter)
{
	int newX,newY;
	newY = pCenter.y() + (pt.x() - pCenter.x()) ;
	newX = pCenter.x() - (pt.y() - pCenter.y()) ;

	return SPoint(newX,newY);
}

//X座標を指定して左右反転を行う
SPoint SPtnObj::mirrorPoint(SPoint pt,int xCenter)
{
	int newX;
	newX = xCenter*2 -pt.x();
	return SPoint(newX,pt.y());
}
*/

#endif