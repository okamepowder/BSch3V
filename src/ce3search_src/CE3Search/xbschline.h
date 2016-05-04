/****************************************************************************
    BSch3V schematic capture
	Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

/****************************************************************************
** XBSch図面要素線クラス
****************************************************************************/
#ifndef XBSCHLINE_H
#define XBSCHLINE_H

#include "xbsch.h"
#include "xbschobj.h"
#include "SBezier.h"
//#include <SPoint.h>
//#include <SRect.h>

class SXBSchLine:public SXBSchObj
{
public:
	//コンストラクタ
	SXBSchLine();
	//デストラクタ
	virtual ~SXBSchLine(){}
	//選択状態のときのスナップポイントを取得する
	SPoint snapPoint();

	enum EDITPOINT{
		EDITPOINT_P1			=0,
		EDITPOINT_P2			=1,
		EDITPOINT_C1			=2,
		EDITPOINT_C2			=3
	};


public:
	//点を指定して選択重要度を返す
	int qSelectWeight(const SPoint&);
	//範囲を指定して選択を行う
	unsigned testSelection(const SRect& );
	//範囲を選択してドラッグ選択を行う
	unsigned testSelectionForDrag(const SRect& );
	//描画が必要かどうかを返す
	bool qRedraw(const SRect& );
	//ピン数を返す
	//int pinCount(){return 2;}
	//n番目のピンの位置を得る。
	//SPoint pinPosition(int);
	SRect area();
	SRect rotateRect();
	//有効なオブジェクトならtrueを返す。長さのない線分は有効ではない。
	virtual bool qValid(){return(m_p1 != m_p2);}

	virtual bool isHorizontal(){return (m_p1.y() == m_p2.y());}	//2008/06/26
	virtual bool isVertical(){return (m_p1.x() == m_p2.x());}	//2008/06/26

		//単独選択のときにミラー可能かどうかを返す
	bool canMirrorSingleSelected() {return (!isHorizontal() && !isVertical()); }//2008/06/26

	//X座標を指定して左右反転を行う
	void mirrorH(int x);
	//左右反転を行う
	void mirrorH();
	//XY中心座標を指定して、回転を行う
	void rotate(const SPoint& p);
	//回転を行う
	void rotate();

protected:
	//ストリームからの読み込み
	//bool readStreamEx(SCsvIStream* pStrm,const SPoint* pptOrigin,const TCHAR* pTag);
	//ストリームへの書き込み
	//bool writeStreamEx(SCsvOStream* pStrm,const SPoint* pptOrigin,const TCHAR* pTag);

	//ストリームからの読み込み
	virtual bool readCe3Ex(SReadCE3& rce3,const SPoint* pptOrigin,const TCHAR* pTag);
	//ストリームへの書き込み
	virtual bool writeCe3Ex(SWriteCE3& wce3,const SPoint* pptOrigin,const TCHAR* pTag);

};

//ワイヤークラス
class SXBSchWire:public SXBSchLine
{
public:
	//コンストラクタ
	SXBSchWire(){}
	//デストラクタ
	~SXBSchWire(){}
/*
	//ストリームからの読み込み ～ -WIRE
	bool readStream(SCsvIStream* pStrm,const SPoint* pptOrigin);
	//ストリームへの書き込み　+WIRE ～ -WIRE
	bool writeStream(SCsvOStream* pStrm,const SPoint* pptOrigin);
*/
	bool readCe3(SReadCE3&,const SPoint* ,SXBSchDoc*);
	bool writeCe3(SWriteCE3& ,const SPoint* );

	//IDの取得
	unsigned id(){return ID_WIRE;}
	unsigned objectFilterValue(){return OBJMASK_WIRE;} 
	//複製を行う
	SXBSchObj* duplicate();
};

//バスクラス
class SXBSchBus:public SXBSchLine
{
public:
	//コンストラクタ
	SXBSchBus(){}
	//デストラクタ
	~SXBSchBus(){}
	//IDの取得
	unsigned id(){return ID_BUS;}
	unsigned objectFilterValue(){return OBJMASK_BUS;}
/*
	//ストリームからの読み込み ～ -BUS
	bool readStream(SCsvIStream* pStrm,const SPoint* pptOrigin);
	//ストリームへの書き込み　+BUS ～ -BUS
	bool writeStream(SCsvOStream* pStrm,const SPoint* pptOrigin);
*/
	bool readCe3(SReadCE3&,const SPoint* ,SXBSchDoc*);
	bool writeCe3(SWriteCE3& ,const SPoint* );

	//複製を行う
	SXBSchObj* duplicate();
};


//#define MIN_MARKER_WIDTH 3
//#define MAX_MARKER_WIDTH 101


//マーカーラインクラス
class SXBSchMarkerLine:public SXBSchLine
{
public:

	static const unsigned int DEFAULT_COLOR = 8454016;
	static const int MIN_MARKER_WIDTH = 3;
	static const int DEFAULT_MARKER_WIDTH = 7;
	static const int MAX_MARKER_WIDTH = 100;


	//コンストラクタ
	SXBSchMarkerLine(){
		m_width = DEFAULT_MARKER_WIDTH;
		m_nColor = DEFAULT_COLOR;
	}
	//デストラクタ
	~SXBSchMarkerLine(){}

	//範囲を指定して選択を行う
	unsigned testSelection(const SRect& );
	//範囲を選択してドラッグ選択を行う
	unsigned testSelectionForDrag(const SRect& );


	
/*
	//ストリームからの読み込み ～ -DASH
	bool readStream(SCsvIStream* pStrm,const SPoint* pptOrigin);
	//ストリームへの書き込み　+DASH ～ -DASH
	bool writeStream(SCsvOStream* pStrm,const SPoint* pptOrigin);
*/
	bool readCe3(SReadCE3&,const SPoint* ,SXBSchDoc*);
	bool writeCe3(SWriteCE3& ,const SPoint* );

	//マーカーラインの外周を形成する4つのポイントの座標を得る
	void outlinePoint(SPoint*);

	SRect area();

	//アトリビュートの変更が可能な場合はtrueを返す。
	bool canAttribute() {return true;}

	bool pointIsInside(const SPoint& ,const SPoint* );

	
	//IDの取得
	unsigned id(){return ID_MARKER;}
	unsigned objectFilterValue(){return OBJMASK_MARKER;} 
	//複製を行う
	SXBSchObj* duplicate();

	int m_width;
	unsigned int m_nColor;
public:
	bool keepOrder(){return true;}


};



#endif
