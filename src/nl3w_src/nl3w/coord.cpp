/****************************************************************************
    BSch3V schematic capture
   Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#include "stdafx.h"
#include <limits.h>
#include "coord.h"

//////////////////////////
//点座標クラス
SPoint::SPoint()
{
	m_x=0;
	m_y=0;
}

SPoint::SPoint(int x,int y)
{
	m_x=x;
	m_y=y;
}

void SPoint::setX(int x)
{
	m_x=x;
}


void SPoint::setY(int y)
{
	m_y=y;
}


int SPoint::x() const
{
	return m_x;
}


int SPoint::y() const
{
	return m_y;
}

SPoint& SPoint::operator += (const SPoint& p)
{
	m_x+=p.x();
	m_y+=p.y();
	return *this;
}

SPoint& SPoint::operator -= (const SPoint& p)
{
	m_x-=p.x();
	m_y-=p.y();
	return *this;
}



bool operator == (const SSize& s1,const SSize& s2)
{
	return (s1.w()==s2.w() && s1.h()==s2.h());
}

bool operator != (const SSize& s1,const SSize& s2)
{
	return (s1.w()!=s2.w() || s1.h()!=s2.h());
}

bool operator == (const SPoint& p1,const SPoint& p2)
{
	return (p1.x()==p2.x() && p1.y()==p2.y());
}

bool operator != (const SPoint& p1,const SPoint& p2)
{
	return (p1.x()!=p2.x() || p1.y()!=p2.y());
}
	



//////////////////////////
//サイズクラス
SSize::SSize()
{
	m_w=0;
	m_h=0;
}

SSize::SSize(int w,int h)
{
	m_w=w;
	m_h=h;
}

void SSize::setW(int w)
{
	m_w=w;
}

void SSize::setH(int h)
{
	m_h=h;
}

int SSize::w() const
{
	return m_w;
}

int SSize::h() const
{
	return m_h;
}

//pとrcAreaの内外コードを返す
unsigned int g_setOutCode(const SPoint& p,const SRect& rcArea)
{
	unsigned outcode=0;											//  UL  | U | UR
	if		(p.x() < rcArea.l() )	outcode |= OUTCODE_LEFT;	//  ----+---+----
	else if (p.x() > rcArea.r() )	outcode |= OUTCODE_RIGHT;	//   L  | 0 |  R
	if		(p.y() < rcArea.t() )	outcode |= OUTCODE_UPPER;	//  ----+---+----
	else if	(p.y() > rcArea.b() )	outcode |= OUTCODE_LOWER;	//  LL  | L | LR
	return outcode;
}

//r が a0～a1 ならtrue
bool g_in_range(double r, double a0, double a1)
{
	if(r<a0 && r<a1) return false;
	if(r>a0 && r>a1) return false;
	return 	true;
}


bool g_range_continue(double a0, double a1, double b0, double b1)
{
	if(a0<b0 && a1<b0 && a0<b1 && a1<b1) return false;
	if(a0>b0 && a1>b0 && a0>b1 && a1>b1) return false;
	return true;
}


bool g_linecross(const SPoint& p00,const SPoint& p01,const SPoint& p10,const SPoint& p11)
{
	double a0;
	double b0;
	double a1;
	double b1;
	double y0;
	double y1;
	double xc;

	if(!g_range_continue(p00.x(), p01.x(), p10.x(), p11.x())) return false;
	if(!g_range_continue(p00.y(), p01.y(), p10.y(), p11.y())) return false;


	if(p00.x()==p01.x()){	//線分0が垂直
		if(p10.x()==p11.x()){	//線分1も垂直
			return (p00.x() == p10.x());
		}
		a1 = ((double)(p11.y()-p10.y()))/(p11.x()-p10.x());
		b1 = p10.y()-a1*p10.x();
		y1 = a1*p00.x()+b1;
		return g_in_range(y1, p00.y(), p01.y());
	}

	if(p10.x()==p11.x()){	//線分1が垂直
		a0 = ((double)(p01.y()-p00.y()))/(p01.x()-p00.x());
		b0 = p00.y()-a0*p00.x();
		y0 = a0*p10.x()+b0;
		return g_in_range(y0, p10.y(), p11.y());
	}

	a0 = ((double)(p01.y()-p00.y()))/(p01.x()-p00.x());
	b0 = p00.y()-a0*p00.x();

	a1 = ((double)(p11.y()-p10.y()))/(p11.x()-p10.x());
	b1 = p10.y()-a1*p10.x();

	if(a0==a1){
		return (b0==b1);
	}

	xc = (b1-b0)/(a0-a1);
	return g_in_range(xc, p00.x(), p01.x()) && g_in_range(xc, p10.x(), p11.x());
}



//////////////////////////
//矩形座標クラス
SRect::SRect()
{
	m_l=INT_MAX;
	m_t=INT_MAX;
	m_r=INT_MIN;
	m_b=INT_MIN;
}

SRect::SRect(int l,int t,int w,int h)
{
	m_l=l;
	m_t=t;
	m_r=l+w;
	m_b=t+h;
}

SRect::SRect(const SPoint& p1,const SPoint& p2)
{
	m_l=p1.x();
	m_t=p1.y();
	m_r=p2.x();
	m_b=p2.y();
}

void SRect::setL(int n)
{
	m_l=n;
}

void SRect::setT(int n)
{
	m_t=n;
}

void SRect::setR(int n)
{
	m_r=n;
}

void SRect::setB(int n)
{
	m_b=n;
}

void SRect::setW(int n)
{
	m_r=m_l+n;
}

void SRect::setH(int n)
{
	m_b=m_t+n;
}

int SRect::l() const
{
	return m_l;
}

int SRect::t() const
{
	return m_t;
}

int SRect::r() const
{
	return m_r;
}

int SRect::b() const
{
	return m_b;
}

int SRect::w() const
{
	return m_r-m_l;
}

int SRect::h() const
{
	return m_b-m_t;
}

void SRect::normalize()
{
	int n;
	if(m_l>m_r){
		n	=m_r;
		m_r	=m_l;
		m_l	=n;
	}
	if(m_t>m_b){
		n	=m_b;
		m_b	=m_t;
		m_t	=n;
	}
}

SPoint SRect::center() const
{
	return SPoint((m_l+m_r)/2,(m_t+m_b)/2);
}


//重なっていたらtrueを返す
bool SRect::intersect(const SRect& rc) const
{
	if(rc.l()>m_r) return false;
	if(rc.r()<m_l) return false;
	if(rc.t()>m_b) return false;
	if(rc.b()<m_t) return false;
	return true;
}

//重なっていたらtrueを返す
bool SRect::intersect(const SPoint& pt) const
{
	if(pt.x()>m_r) return false;
	if(pt.x()<m_l) return false;
	if(pt.y()>m_b) return false;
	if(pt.y()<m_t) return false;
	return true;
}




//矩形と線 重なっていたらtrueを返す
bool SRect::intersect(const SPoint& pt1,const SPoint& pt2,unsigned& nOutcode1,unsigned& nOutcode2) const
{

	nOutcode1 = g_setOutCode(pt1,*this);
	nOutcode2 = g_setOutCode(pt2,*this);
	if(nOutcode1 & nOutcode2) return 0;
	if((nOutcode1 == 0 )||(nOutcode2 == 0 )) return true;
	unsigned nOutcode = nOutcode1 | nOutcode2;
	if(nOutcode == (OUTCODE_LEFT  | OUTCODE_RIGHT)) return true;
	if(nOutcode == (OUTCODE_UPPER | OUTCODE_LOWER)) return true;

	int x,y;
	int x1 = pt1.x();
	int y1 = pt1.y();
	int x2 = pt2.x();
	int y2 = pt2.y();

	if(nOutcode & OUTCODE_LEFT){	//片方が左に出ている
		x=m_l;
		y=y1+((x-x1)*(y2-y1))/(x2-x1);
		if(y >= m_t && y <= m_b) return true;
	}
	if(nOutcode & OUTCODE_RIGHT){	//片方が右に出ている
		x=m_r;
		y=y1+((x-x1)*(y2-y1))/(x2-x1);
		if(y >= m_t && y <= m_b) return true;
	}
	if(nOutcode & OUTCODE_UPPER){	//片方が上に出ている
		y=m_t;
		x=x1+((y-y1)*(x2-x1))/(y2-y1);
		if(x >= m_l && x <= m_r) return true;
	}
	if(nOutcode & OUTCODE_LOWER){	//片方が下に出ている
		y=m_b;
		x=x1+((y-y1)*(x2-x1))/(y2-y1);
		if(x >= m_l && x <= m_r) return true;
	}
	return 0;
}








	//if(intersect(pt1)) return true;
	//if(intersect(pt2)) return true;

	//SPoint p0;
	//SPoint p1;

	//p0=SPoint(m_r,m_t);
	//p1=SPoint(m_l,m_t);
	//if(g_linecross(pt1,pt2,p0,p1))return true;

	//p0=SPoint(m_l,m_b);
	//if(g_linecross(pt1,pt2,p0,p1))return true;

	//p1=SPoint(m_r,m_b);
	//if(g_linecross(pt1,pt2,p0,p1))return true;

	//p0=SPoint(m_r,m_t);
	//if(g_linecross(pt1,pt2,p0,p1))return true;

	//return false;
//}




SRect& SRect::operator |= (const SRect& rc)
{
	if(m_l>rc.l()) m_l=rc.l();
	if(m_r<rc.r()) m_r=rc.r();
	if(m_t>rc.t()) m_t=rc.t();
	if(m_b<rc.b()) m_b=rc.b();
	return *this;
}

SRect& SRect::operator |= (const SPoint& pt)
{
	if(m_l>pt.x()) m_l=pt.x();
	if(m_r<pt.x()) m_r=pt.x();
	if(m_t>pt.y()) m_t=pt.y();
	if(m_b<pt.y()) m_b=pt.y();
	return *this;
}


SRect& SRect::operator += (const SPoint& pt)
{
	m_l += pt.x();
	m_t += pt.y();
	m_r += pt.x();
	m_b += pt.y();
	return *this;
}

void SRect::increase(int n)
{
	normalize();
	m_l -= n;
	m_t -= n;
	m_r += n;
	m_b += n;
}