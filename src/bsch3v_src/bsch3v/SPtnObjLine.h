/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

#pragma once
#include "sptnobj.h"

class SPtnObjLine :	public SPtnObj
{

protected:
	SPoint m_p0;
	SPoint m_p1;
	bool m_validArea;
	SRect m_area;
	bool m_bCurve;
	SPoint m_pC0;
	SPoint m_pC1;


public:
	//共通のインターフェース
	SPtnObjLine(void);
	~SPtnObjLine(void);
	int id(){return VECTPTN_L;}
//	SPtnObjLine(const SPtnObjLine&);				//コピーコンストラクタ
//	SPtnObjLine& operator=(const SPtnObjLine&);		//代入演算子
	SPtnObj* duplicate();								//複製を作る
	bool qValid();									//有効なデータかどうかを返す。

	//グリップの数を返す
	int gripCount();
	//グリップの位置を返す
	SPoint gripPos(int grip);
	//グリップの位置を動かす
	void setGripPos(int grip,int x,int y);




	enum GRIPPOINT{
		GRIPPOINT_P0			=0,
		GRIPPOINT_P1			=1,
		GRIPPOINT_C0			=2,
		GRIPPOINT_C1			=3
	};



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
	
	void SetP0(const SPoint&);
	void SetP1(const SPoint&);
	const SPoint& GetP0(){return m_p0;}
	const SPoint& GetP1(){return m_p1;}

	void SetPC0(const SPoint&);
	void SetPC1(const SPoint&);
	const SPoint& GetPC0(){return m_pC0;}
	const SPoint& GetPC1(){return m_pC1;}

	void setCurve(bool curve);	//曲線フラグの設定
	bool getCurve(){return m_bCurve;} //曲線フラグの取得

	void setDefaultCtrlPos();	//曲線の制御点をデフォルト位置に設定する。






};
