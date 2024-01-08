/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

/****************************************************************************
** XBSch図面要素エントリークラス
****************************************************************************/
#ifndef XBSCHENTRY_H
#define XBSCHENTRY_H

#include "xbsch.h"
#include "xbschobj.h"
#include "xbschline.h"


class SXBSchEntry:public SXBSchLine
{
public:
	enum ENTRY_ASPECT
	{
		RIGHT_DOWN=0,
		LEFT_DOWN
	};
protected:
	//m_p1 が m_p2の下になるように設定する。
	void normalize();
public:
	//コンストラクタ
	SXBSchEntry();
	//デストラクタ
	virtual ~SXBSchEntry(){};
/*
	//ストリームからの読み込み ～ -ENTRY
	bool readStream(SCsvIStream* pStrm,const SPoint* pptOrigin);
	//ストリームへの書き込み　+ENTRY ～ -ENTRY
	bool writeStream(SCsvOStream* pStrm,const SPoint* pptOrigin);
*/
	bool readCe3(SReadCE3&,const SPoint* ,SXBSchDoc*);
	bool writeCe3(SWriteCE3& ,const SPoint* );


	//IDの取得
	unsigned id(){return ID_ENTRY;}
	unsigned objectFilterValue(){return OBJMASK_WENTRY;} 
public:
	//複製を行う
	SXBSchObj* duplicate();
	//座標の設定  
	//m_p1への設定を行うとm_p2への設定を完了する。
	void setX1(int);
	void setY1(int);
	void setP1(const SPoint&);
	//m_p2への設定を無効にしている
	void setX2(int){}
	void setY2(int){}
	void setP2(const SPoint&){}
	//範囲を選択してドラッグ選択を行う
	unsigned testSelectionForDrag(const SRect& rc){return testSelection(rc);} 
	//エントリーの右下がり、左下がりを取得する
	unsigned aspect();
	//エントリーの右下がり、左下がりを設定する
	void setAspect(int aspect);
};


class SXBSchBusEntry:public SXBSchEntry
{
public:
	//コンストラクタ
	SXBSchBusEntry(){}
	//デストラクタ
	virtual ~SXBSchBusEntry(){}
/*
	//ストリームからの読み込み ～ -BENTRY
	bool readStream(SCsvIStream* pStrm,const SPoint* pptOrigin);
	//ストリームへの書き込み　+BENTRY ～ -BENTRY
	bool writeStream(SCsvOStream* pStrm,const SPoint* pptOrigin);
*/
	bool readCe3(SReadCE3&,const SPoint* ,SXBSchDoc*);
	bool writeCe3(SWriteCE3& ,const SPoint* );

	//IDの取得
	unsigned id(){return ID_BENTRY;}
	unsigned objectFilterValue(){return OBJMASK_BENTRY;} 
public:
	//複製を行う
	SXBSchObj* duplicate();
};

#endif
