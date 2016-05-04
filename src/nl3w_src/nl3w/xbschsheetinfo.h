/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// Copyright (C) 2002 H.Okada(suigyodo) All rights reserved.
/****************************************************************************
                    XBSch図面情報クラス

  シートデータのバージョンはこのクラスで定義している。
  static const int m_nConstDataVersion = xx;



****************************************************************************/
#ifndef XBSCHSHEETINFO_H
#define XBSCHSHEETINFO_H

#include "xbsch.h"
#include "xbschobj.h"

class SXBSchSheetInfo:public SXBSchObj
{
public:
	//コンストラクタ
	SXBSchSheetInfo();
	
	//SXBSchSheetInfo(){m_p1.setX(1500);m_p1.setY(1000);m_Selection=0xFF;m_Layer=0;}
	//SXBSchJunc(const SXBSchJunc& junc);	//コピーコンストラクタ
	//デストラクタ
	virtual ~SXBSchSheetInfo(){};
/*
	//ストリームからの読み込み ～-SHEETINFO
	bool readStream(SCsvIStream* pStrm,const QPoint* pptOffset);
	//ストリームへの書き込み　+SHEETINFO～+SHEETINFO
	bool writeStream(SCsvOStream* pStrm,const QPoint* pptOffset);
*/

protected:
	int m_totalPages;
	int m_page;
	string m_strProject;
	int m_version;
	int m_initPos;

public:
	enum {
		INITPOS_LT = 0,
		INITPOS_LB = 1,
		INITPOS_RT = 2,
		INITPOS_RB = 3
	};


public:
	void setTotalPages(int n){m_totalPages = n;}
	void setPage(int n){m_page = n;}
	void setProjectName(const char* psz){m_strProject = psz;}
	void setInitPos(int n);
	
	int totalPages(){return m_totalPages;}
	int page(){return m_page;}
	const char* projectName(){return m_strProject.c_str();}
	int dataVersion(){return m_version;}
	int initPos(){return m_initPos;}


	bool readCe3(SReadCE3&,const SPoint* ,SXBSchDoc*);
	bool writeCe3(SWriteCE3& ,const SPoint* );

	//IDの取得
	unsigned id(){return ID_SHEETINFO;}
	unsigned objectFilterValue(){return 0;} 
public:
	static const int m_nConstDataVersion = 68;	//シートデータのバージョン

	void setWidth(int w) {m_p1.setX(w);}
	void setHeight(int h){m_p1.setY(h);}
	int width(){return x1();}
	int height(){return y1();}

	void setEditLayer(int n){m_Layer=n;}	
	int editLayer(){return m_Layer;}

	void setVisbleLayer(unsigned n){m_Selection = n;}
	unsigned visbleLayer(){return m_Selection;}
	
	void setX1(int){}
	void setY1(int){}
	void setP1(const SPoint&){}
	void setX2(int){}
	void setY2(int){}
	void setP2(const SPoint&){}

	//位置を動かす
	void move(int,int){};
	//X座標を指定して左右反転を行う
	void mirrorH(int){};
	//左右反転を行う
	void mirrorH(){};
	//XY中心座標を指定して、回転を行う
	void rotate(const SPoint&){};
	//回転を行う
	void rotate(){};
	//選択フラグをセットする
	void setSelection(unsigned){};
	//選択を解除する
	void resetSelection(){};
	//占有範囲を返す
	SRect area(){return SRect(0,0,m_p1.x(),m_p1.y());}

	//単独選択のときにミラー可能かどうかを返す
	bool canMirrorSingleSelected() {return false;}//2008/06/26
	//単独選択のときに回転可能かどうかを返す
	bool canRotateSingleSelected() {return false;}//2008/06/26

};

#endif
