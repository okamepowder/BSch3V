/****************************************************************************
    BSch3V schematic capture
	Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

/****************************************************************************
** XBSch図面要素ジャンクションクラス
****************************************************************************/
#ifndef XBSCHJUNC_H
#define XBSCHJUNC_H

#include "xbsch.h"
#include "xbschobj.h"

class SXBSchJunc:public SXBSchObj
{
public:
	//コンストラクタ
	SXBSchJunc();
	//デストラクタ
	virtual ~SXBSchJunc(){};

/*	//ストリームからの読み込み ～ -JUNCTION
	bool readStream(SCsvIStream* pStrm,const QPoint* pptOrigin);
	//ストリームへの書き込み　+JUNCTION ～ -JUNCTION
	bool writeStream(SCsvOStream* pStrm,const QPoint* pptOrigin);
*/
	bool readCe3(SReadCE3&,const SPoint* ,SXBSchDoc* );
	bool writeCe3(SWriteCE3& ,const SPoint* );

	
	
	//IDの取得
	unsigned id(){return ID_JUNCTION;}
	unsigned objectFilterValue(){return OBJMASK_JUNCTION;} 
public:
	//複製を行う
	SXBSchObj* duplicate();
	//座標の設定  m_p2への設定を無効にしている
	void setX2(int){}
	void setY2(int){}
	void setP2(const SPoint&){}
	//左右反転を行う
	void mirrorH(){}	//何もしない
	//回転を行う
	void rotate(){}		//何もしない
	//点を指定して選択重要度を返す
	int qSelectWeight(const SPoint& p);
	//範囲を指定して選択を行う
	unsigned testSelection(const SRect& rc);
	//範囲を選択してドラッグ選択を行う
	unsigned testSelectionForDrag(const SRect& rc);
	//描画が必要かどうかを返す
	bool qRedraw(const SRect& rc);
	//占有範囲を返す
	SRect area();
	SRect rotateRect();

	//単独選択のときにミラー可能かどうかを返す
	bool canMirrorSingleSelected() {return false;}//2008/06/26
	//単独選択のときに回転可能かどうかを返す
	bool canRotateSingleSelected() {return false;}//2008/06/26


};

#endif
