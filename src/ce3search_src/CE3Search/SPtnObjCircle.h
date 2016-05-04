/****************************************************************************
    BSch3V schematic capture
	Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#pragma once
#include "sptnobj.h"

class SPtnObjCircle :
	public SPtnObj
{
public:
//共通のインターフェース
	SPtnObjCircle(void);
	~SPtnObjCircle(void);
	int id(){return VECTPTN_C; }
	SPtnObj* duplicate();								//複製を作る
	bool qValid();									//有効なデータかどうかを返す。
	//グリップの数を返す
	int gripCount(){return 2;}
	//グリップの位置を返す
	SPoint gripPos(int grip);
	//グリップの位置を動かす
	void setGripPos(int grip,int x,int y);

	void move(int offsetx,int offsety);				//位置を動かす
	void mirrorH(int x); 							//X座標を指定して左右反転を行う
	void mirrorH();									//左右反転を行う
	void rotate(const SPoint& p);					//XY中心座標を指定して、回転を行う
	void rotate();									//回転を行う
	bool testIntersect(const SRect& );				//範囲を指定して選択チェックを行う
	bool testIntersect(const SPoint& );				//点を指定して選択チェックを行う
	SRect area();									//占有範囲を返す
	bool readCe3(SReadCE3& rce3);
	bool writeCe3(SWriteCE3& wce3,SPoint* pOrigin=NULL);
public:
	SPoint m_p0;
	SPoint m_p1;
	//int m_nFill;
};
