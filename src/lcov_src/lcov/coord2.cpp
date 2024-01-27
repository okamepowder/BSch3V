/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

#include "stdafx.h"
#include "coord.h"
#include "coord2.h"
//#include <math.h>

CSize SSize2CSize(const SSize& ssize)
{
	return CSize(ssize.w(),ssize.h());
}

SSize CSize2SSize(const CSize& csize)
{
	return SSize(csize.cx,csize.cy);
}

CPoint SPoint2CPoint(const SPoint& spoint)
{
	return CPoint(spoint.x(),spoint.y());
}

SPoint CPoint2SPoint(const CPoint& cpoint)
{
	return SPoint(cpoint.x,cpoint.y);
}

CRect SRect2CRect(const SRect& srect)
{
	return CRect(srect.l(),srect.t(),srect.r(),srect.b());
}

SRect CRect2SRect(const CRect& crect)
{
	return SRect(crect.left,crect.top,crect.Width(),crect.Height());
}


//線分側面に点が指定距離内にあるかどうかの判定
bool PointIsCloseToLineSide(const SPoint& ptPoint,const SPoint& ptLineBegin,const SPoint& ptLineEnd,int limit, SPoint& ptCross)
{
	int x1 = ptLineBegin.x();
	int y1 = ptLineBegin.y();
	int x2 = ptLineEnd.x();
	int y2 = ptLineEnd.y();

	int xp = ptPoint.x();
	int yp = ptPoint.y();

	//線分が垂直
	if(x1 == x2){
		if( (y1 <= yp && yp <= y2) || (y1 >= yp && yp >= y2) ){
			if(abs(x1-xp)<=limit){
				ptCross = SPoint(x1,yp);
				return true;
			}
		}
		return false;
	}
	//線分が水平
	if(y1 == y2){
		if( (x1 <= xp && xp <= x2) || (x1 >= xp && xp >= x2) ){
			if(abs(y1-yp)<=limit){
				ptCross = SPoint(xp,y1);
				return true;
			}
		}
		return false;
	}

	//線分のパラメータ
	double line_a = ((double)(y2-y1))/(x2-x1);
	double line_b = y2 - line_a * x2;

	//点を通って線分に直交する線のパラメータ
	double pline_a = -1/line_a;
	double pline_b = yp - pline_a * xp;

	//交点
	double cros_x = (pline_b - line_b)/(line_a - pline_a);

	//範囲内外チェックをして範囲内なら距離判定
	if( (x1 <= cros_x && cros_x <= x2) || (x1 >= cros_x && cros_x >= x2) ){
		double cros_y = line_a*cros_x + line_b;
		double dx =  (xp - cros_x);
		double dy =  (yp - cros_y);
		if(abs(dx>limit) || abs(dy>limit))return false;
		if((dx*dx+dy*dy)<=((double)limit*limit)){
			ptCross = SPoint((int)cros_x, (int)cros_y);
			return true;
		}
	}
	return false;
}
