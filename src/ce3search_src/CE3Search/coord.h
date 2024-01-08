/****************************************************************************
    BSch3V schematic capture
	Copyright (C) 1997-2014 H.Okada
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

enum OUTCODE
{
	OUTCODE_CENTER	=	0x0000,
	OUTCODE_LEFT	=	0x0001,
	OUTCODE_UPPER	=	0x0002,
	OUTCODE_RIGHT	=	0x0004,
	OUTCODE_LOWER	=	0x0008
};




//r が a0～a1 ならtrue
bool g_in_range(double r, double a0, double a1);


//a0～a1, b0～b1が重なっていればtrue
bool g_range_continue(double a0, double a1, double b0, double b1);

bool g_linecross(const SPoint& p00,const SPoint& p01,const SPoint& p10,const SPoint& p11);

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
	bool intersect(const SRect& rc) const;//矩形と矩形 重なっていたらtrueを返す
	bool intersect(const SPoint& pt) const;//矩形と点 重なっていたらtrueを返す
	//bool intersect(const SPoint& pt1,const SPoint& pt2) const;//矩形と線 重なっていたらtrueを返す
	bool intersect(const SPoint& pt1,const SPoint& pt2,unsigned& outcodeP1,unsigned& coutcodeP2) const;//矩形と線 重なっていたらtrueを返す

	SRect& operator |= (const SRect&);
	SRect& operator |= (const SPoint&);
	SRect& operator += (const SPoint&);
	void increase(int n);
};

//pとrcAreaの内外コードを返す
unsigned int g_setOutCode(const SPoint& p,const SRect& rcArea);

#endif
