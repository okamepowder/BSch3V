/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

#ifndef COORD_H
#define COORD_H


//サイズクラス
class SSize
{
protected:
	int m_w;
	int m_h;
public:
	SSize();
	SSize(int w,int h);
	void setW(int w);
	void setH(int h);
	int w() const;
	int h() const;
};

bool operator == (const SSize& s1,const SSize& s2);
bool operator != (const SSize& s1,const SSize& s2);


//点座標クラス
class SPoint
{
protected:
	int m_x;
	int m_y;
public:
	SPoint();
	SPoint(int x,int y);
	void setX(int x);
	void setY(int y);
	int x() const;
	int y() const;
	SPoint& operator += (const SPoint&);
	SPoint& operator -= (const SPoint&);
};

bool operator == (const SPoint& p1,const SPoint& p2);
bool operator != (const SPoint& p1,const SPoint& p2);



//矩形座標クラス
class SRect
{
protected:
	int m_l;
	int m_t;
	int m_r;
	int m_b;
public:
	SRect();
	SRect(int l,int t,int w,int h);
	SRect(const SPoint& p1,const SPoint& p2);
	void setL(int n);
	void setT(int n);
	void setR(int n);
	void setB(int n);
	void setW(int n);
	void setH(int n);
	int l() const;
	int t() const;
	int r() const;
	int b() const;
	int w() const;
	int h() const;
	void normalize();
	SPoint center() const;
	bool intersect(const SRect& rc) const;//重なっていたらtrueを返す
	bool intersect(const SPoint& pt) const;//重なっていたらtrueを返す
	SRect& operator |= (const SRect&);
	SRect& operator |= (const SPoint&);
	SRect& operator += (const SPoint&);
};

#endif
