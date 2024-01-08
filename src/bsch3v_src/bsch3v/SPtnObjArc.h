/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#pragma once
#include "sptnobj.h"

class SPtnObjArc :
	public SPtnObj
{
public:
	//共通のインターフェース
	SPtnObjArc(void);
	~SPtnObjArc(void);
	int id(){return VECTPTN_AR;}
//	SPtnObjArc(const SPtnObjArc&);					//コピーコンストラクタ
//	SPtnObjArc& operator=(const SPtnObjArc&);		//代入演算子
	SPtnObj* duplicate();							//複製を作る
	bool qValid();									//有効なデータかどうかを返す。

	//グリップの数を返す
	int gripCount(){return 7;}
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
	
	void setBeginPoint(int x,int y);			//開始角を座標で設定。先に中心を決めておくこと
	void setEndPoint(int x,int y);				//終了角を座標で設定。先に中心を決めておくこと
	SPoint AngleToPoint(int angle);				//角度(deg*16)から円周上の点を求める
	SPoint AngleToPoint500(int angle);			//角度(deg*16)から半径500の円周上の点を求める


public:
	SPoint m_ptC;		//中心座標
	int    m_nR;		//半径
	int	   m_nBegin;	//開始角
	int	   m_nEnd;		//終了角

	SPoint m_ptBegin;
	SPoint m_ptEnd;


protected:
	void normalizeAngle();
public:
	//static bool  Conv3PointArcToAR(SPoint pE0,SPoint pE1,SPoint p,SPoint& center,int& r,int& begin, int& end);
	static double  PointRad(double cx,double cy,double r,double x0,double y0);
};
