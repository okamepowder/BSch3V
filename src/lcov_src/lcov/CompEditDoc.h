/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

#pragma once

#include <list>
#include <string>
using namespace std;

#include "complib.h"
#include "CompEditElm.h"

typedef list<SCompEditElm> SCompEditElmList;
typedef list<SCompEditElm>::iterator SCompEditElmListIterator;
typedef list<SCompEditElm>::reverse_iterator SCompEditElmListRevIterator;

class SCompEditDoc
{
public:
	static bool m_noChechAreaOnSave;
public:
	enum{
		DATATYPE_PIN = 0,
		DATATYPE_PTN = 1
	};
protected:
	int m_nOpCount;

public:
	SCompEditElmList m_listMain;
	SCompEditElmList m_listTemp;
	bool m_dirtyFlag;
	bool m_tempDirtyFlag;
	SSize m_size;		//グリッド単位でのサイズ
	int  m_block;
	SRect m_tempArea;

	//string m_prop_string;

	wstring m_lastPinNumber;
	wstring m_lastPinName;

	void setPin(const PSPin* pPinArray, int count);
	void setPtnObj(const PSPtnObj* pPtnArray, int count);
	bool releaseTemp();
	bool isPinSelected();
	bool delTemp();
	void setupTempArea();
	bool isOnTmpData(const SPoint& pt);
	//int isOnGrip(const SPoint& pt);
	int incrementOpCount(){m_nOpCount++; return m_nOpCount;}
	int decrementOpCount(){m_nOpCount--; return m_nOpCount;}

	int opCount(){return m_nOpCount;}

	bool addPinToTemp(SPin* pPin);//一時データへのピンの追加
	bool addPtnToTemp(SPtnObj* pPtn);//一時データへのパターンの追加

public:
	SCompEditDoc(void);
	virtual ~SCompEditDoc(void);
	void init();
	void loadCompInfo(const SCompInfo* pComp);
	void saveCompInfo(SCompInfo* pComp);

	int doSelect(const SPoint& pt);
	int doSelect(const SRect& rc);
	int doSelect(int ltrb,int offset);
	
	bool copy();
	bool canUndo();
	bool undo();

	bool move(int offsetx,int offsety);
	bool mirror();
	bool rotate();


	void pinPlacePoint(const SPoint& pt,int &ltrb,int &offset);	//ptに最も近いピン配置座標を得る
	SRect pinArea(int ltrb,int offset);//ピン座標から有効な占有範囲を求める
	SPin* pinConflict(const SPin* pPin);//既存ピンと同じ場所なら、既存ピンのポインタを返す
	SPoint pinPosition(int ltrb,int offset); //ピン座標から部品編集座標を得る

	static wstring nextString(const wstring&  str);

};
