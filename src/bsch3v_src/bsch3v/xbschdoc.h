/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#ifndef XBSCHDOC_H
#define XBSCHDOC_H


#include <list>
#include <stack>
using namespace std;

#include "SelectOption.h"
#include "ObjFilter.h"

#include "xbschobj.h"
#include "xbschjunc.h"
#include "xbschlabel.h"
#include "xbschcomment.h"
#include "xbschtag.h"
#include "xbschcomponent.h"

#include "xbschline.h"
#include "xbschdashline.h"
#include "xbschentry.h"
#include "xbschdelobj.h"
#include "xbschsheetinfo.h"
#include "xbschimageobj.h"
#include "ce3io.h"


//typedef list<string> stringList;
//typedef list<string>::iterator stringListIterator;

typedef list<SXBSchObj*> XBSchObjList;
typedef list<SXBSchObj*>::iterator XBSchObjListIterator;
typedef list<SXBSchObj*>::reverse_iterator XBSchObjListRevIterator;




typedef list<SPoint> SPointList;
typedef list<SPoint>::iterator SPointListIterator;

class SXBSchSheetInfo;
class SXBSchWire;

enum FIND_TARGET_FLAGS
{
	FINDTGT_TAG				=0x00000001,
	FINDTGT_LABEL			=0x00000002,
	FINDTGT_COMMENT			=0x00000004,
	FINDTGT_NAME			=0x00000008,
	FINDTGT_NAMELIB			=0x00000010,
	FINDTGT_NUM				=0x00000020,
	FINDTGT_MANUFACTURE		=0x00000040,
	FINDTGT_MANUFACTUREPN	=0x00000080,
	FINDTGT_PACKAGE			=0x00000100,
	FINDTGT_NOTE			=0x00000200
};




class SRedoInfo
{
public:
	enum UndoType
	{
		UndoAddMoveToStack = 0,
		UndoDeleteCopyToStack
	};

	SXBSchObj* m_pObj;
	int m_undoCount;
	UndoType m_undoType;

	SRedoInfo(){
		m_pObj = NULL;
		m_undoCount = -1;
		m_undoType = UndoAddMoveToStack;
	}
};

typedef stack<SRedoInfo> SRedoInfoStack;


class SPointEmum
{
public:
	SPointEmum();
	virtual ~SPointEmum();
	bool addSPoint(SPoint point);

	SPointList m_list;
};

class SLayer
{
public:
		static unsigned  wLayerTable[];
		//={	0x0001,0x0002,0x0004,0x0008,
		//							0x0010,0x0020,0x0040,0x0080,
		//							0x0100,0x0200,0x0400,0x0800,
		//						0x1000,0x2000,0x4000,0x8000};
};

class SXBSchDoc
{
public:
	SXBSchDoc();
	virtual ~SXBSchDoc();
	bool m_bDoTidyUp;

public:
	enum RESET_SELECT_OPTION
	{
		RESET_SELECT_NORMAL	 = 0,
		RESET_SELECT_DELETE	 = 1,
		RESET_SELECT_BRING_TO_FRONT = 2,
		RESET_SELECT_BRING_TO_BACK = 3
	};



protected:


	bool m_bDirty;			//ドキュメントに対して変更が発生したときtrueになる。
	bool m_bTempDirty;		//一時データに対して変更が発生したときtrueになる。
	bool m_bTempNewPlace;	//一時データが新規配置したものかペーストデータのときtrue

	//int m_resetSelectOption;



	SSize m_sizeSheet;	//シートのサイズを保存する
	int m_nOpCount;				//操作番号
	int m_nOldestOpCount;		//もっとも古い操作番号

	int m_nRedoActive;			// Redoが有効なとき、Undoが始まったときのopCountを保持する。

	SRect m_rcTempArea;			//	
	SRect m_rcRotateRect;		//回転や反転で中心位置を求めるためのエリア
	SPoint m_ptTempSnapPoint;	//一時オブジェクトをスナップさせる場合はこの点がグリッドに乗るようにする
	bool m_bSelectForDrag;
	unsigned m_visibleLayer;
	int m_editLayer;
	wstring m_strLayerName[SHEET_LAYERS];

	unsigned m_selectedTypeFlag;
	void updateSelectedTypeFlag();
	enum SELECTED_TYPE_FLAGS
	{
		SELECTED_TYPE_TAG			=1,
		SELECTED_TYPE_LABEL			=2,
		SELECTED_TYPE_COMMENT		=4,
		SELECTED_TYPE_COMPONENT		=8,
		SELECTED_TYPE_MARKER		=16,
		SELECTED_TYPE_DECOLINE		=32
	};

	int m_totalPages;
	int m_page;
	wstring m_strProject;
	int m_version;
	int m_initPos;
public:
	void setTotalPages(int n){m_totalPages = n;}
	void setPage(int n){m_page = n;}
	void setProjectName(const TCHAR* psz){m_strProject = psz;}
	void setInitPos(int n){m_initPos = n;}
	
	int totalPages(){return m_totalPages;}
	int page(){return m_page;}
	const TCHAR* projectName(){return m_strProject.c_str();}
	int dataVersion(){return m_version;}
	int initPos(){return m_initPos;}

	bool createdNewerVersion(){return (m_version>SXBSchSheetInfo::m_nConstDataVersion);}



protected:
	//選択の解除 bDel が true のときは、削除を行う
	//public の resetSelect() と del() から呼ばれる。
	//ドキュメントに変更があったときtrueを返す
	//bool resetSelect_1(bool bDel);
	bool resetSelect_1(int option);

	//一時データをメインデータに移して、一時データをクリアする
	void moveTempDataToMain(bool newPlace,int option);

	void pickupJunction(const SRect& rc);//すべての接合点を m_listWorkJunc に抽出する。
	void selectForDragJunction(int scanMax,const SRect& rc);	//範囲内の接合点の選択。すべての接合点をm_listWorkJunc に抽出する。
	int divideWire(SXBSchWire* swire,const SRect& rc,SXBSchWire** dwire);
	void selectForDragWire(int scanMax,const SRect& rc);		//ドラッグのためのワイヤー選択。
	void selectForDragExceptJuncWire(int scanMax,const SRect& rc);//ドラッグのための接合点ワイヤー以外の選択。
	void selectForDragPinConnection(int scanMax,const SRect& rc,bool bCutOffPinWire);//ドラッグのためのピン結合選択
	bool selectForDragPin(int scanMax,const SPoint pt);//指定した点にピンが見つかったらゼロ長ワイヤーを配置する

	void selectForDragReCheckSelectedLine(const SRect& rc);//既存の選択線に、新たな範囲や追加選択されたタグ、部品のピンに接続された未選択端点がないかチェック

	

public:  //ストリームI/O関連
	bool writeMinimumLb3(SWriteCE3& wce3,XBSchObjList* pList,bool bOwnOnly);
	bool writeCe3(SWriteCE3& wce3,XBSchObjList* pList,bool bAllObj,const SPoint* pptOrigin);
	bool readCe3(	SReadCE3& rce3,
					XBSchObjList* pList,
					const SPoint* pptOrigin,
					SXBSchSheetInfo* pSheetInfo);

public:
	XBSchObjList m_listMain;
	XBSchObjList m_listTemp;
protected:
	XBSchObjList m_listWorkJunc;			//一時作業用のワークエリア ドラッグ選択で使用
	XBSchObjList m_listWorkWire;			//一時作業用のワークエリア ドラッグ選択で使用
	XBSchObjList m_listWorkTagComponent;	//一時作業用のワークエリア ドラッグ選択で使用

protected:
	SRedoInfoStack m_stackRedoInfo;

//----- find -----
protected:
	virtual bool findStringCompare(const TCHAR* szTarget,const TCHAR* sz,bool bMatchWhole,bool bCaseSensitive);
public:
	const SXBSchObj*  findStart(const TCHAR* sz,bool bMatchWhole,bool bCaseSeinsitive,unsigned int targetFlags,bool selectWhenFound);
	//const SXBSchObj*  find(bool bStart=false);
	const SXBSchObj*  find(bool bStart);
	//「次の検索」が可能か?
	bool canFindNext(){return m_bFindCommandActive;}

	//検索で見つかったオブジェクトの情報
	wstring m_foundString;	//文字列
	unsigned int m_foundType;
	SPoint m_foundPoint;

protected:
	bool m_bFindCommandActive;
	wstring	m_strFind;//検索対象文字列
	bool	m_bFindMatchWhole;
	bool	m_bCaseSeinsitive;
	unsigned int m_nTargetFlags;
	bool m_selectWhenFound;
	XBSchObjListRevIterator m_reviteFind;
	int m_nFindStep;
//----- find -----


public:
	void setDirty(bool flag = true){m_bDirty = flag;}
	void setTempDirty(bool flag = true){m_bTempDirty = flag;}

	bool isDirty(){return (m_bDirty || m_bTempDirty || m_bTempNewPlace);}	//最後の保存のあとに変更が入ったかどうかを返す
	bool isEmpty();	//図面オブジェクトが一つでもあればfalseを返す
	void SetSheetSize(const SSize& size);
	SSize SheetSize(){return m_sizeSheet;}

public:
	enum ISTEMPDATA_OPTION{
		EDITPOINTS_MODE = 1
	};

public:
//20060603	//図面要素の追加 操作番号を返す
//20060603	int add(SXBSchObj* pObj);
	
	//図面要素をメインリストにpush_backする。
	void pushBackToMainList(SXBSchObj* pObj,bool doTidyUp);

	//ジャンクションの重複チェックを行う
	//bool checkRedundancyJunction(SXBSchJunc* pObj);

	//ジャンクション、エントリーの重複チェックを行う
	bool checkRedundancy(SXBSchObj* pObj);



	//ワイヤー,バスの結合整理
	void tidyUpWireBus(SXBSchLine* pObj);

	//ワイヤー,ジャンクションの統合整理 ステップ1
	void tidyUpWireJunction_1(int opCount);
	//ワイヤー,ジャンクションの統合整理 ステップ2
	void tidyUpWireJunction_2(int opCount);
	//ワイヤー,ジャンクションの統合整理 ステップ3
	void tidyUpWireJunction_3(int opCount);


	//選択状態で追加図面要素の追加 操作番号を返す
	int addSelect(SXBSchObj* pObj);

	//点を指定しての選択
	int select(const SPoint& pt,SelectOption& refSelOption);
protected:
	bool	m_bValidContinuousSingleSelect;
	XBSchObjListIterator m_singleSelIte;
	SPoint	m_ptBiginSingleSelectAt;


public:
	//範囲を指定しての選択
	int select(const SRect& rc,SelectOption& refSelOption);

	int selectForDrag(const SRect& rc,bool bCutOffPinWire);
	bool isSelectedForDtag(){return this->m_bSelectForDrag;}


	void setNcPinFlag();


	//m_rcTempArea と m_ptTempSnapPoint を一時オブジェクトのリストから設定する。
	void setupRcTempArea();
	void setupRotateRect();

	
	
	//選択の解除
	//ドキュメントに変更があったときtrueを返す
	bool resetSelect(int option);
	//一時オブジェクトの範囲を取得する
	SRect tempArea(){return m_rcTempArea;}
	//一時オブジェクトのスナップポイントを取得する
	SPoint tempSnapPoint(){return m_ptTempSnapPoint;}
	//初期化
	void init();

	enum CE3_FILE_CHECK_RESULT{
		CE3FCHK_ERROR		=0,
		CE3FCHK_V070_OR_E	=1,
		CE3FCHK_V080_OR_L	=2,
		CE3FCHK_EMPTY		=3
	};
	

	//バージョン判定
	int ce3FileCheck(const TCHAR* fname);

	//ファイルからの読み込み
	int readFile(const TCHAR* fname);
	//int readFile(FILE* fp);
	//ファイルへの書き込み
	int writeFile(const TCHAR* fname);
	int writeFile(FILE* fp);




	//***** 編集 *****

	//コピー
	//コピーが行われたときにtrueを返す
	virtual bool copy(){return false;}
	//貼り付け
	//ドキュメントに変更があったときtrueを返す
	virtual bool paste(const SPoint&){return false;}
	//貼り付け可能か？
	virtual bool canPaste(){return false;}

	//削除
	//ドキュメントに変更があったときtrueを返す
	bool del();

	//切り取り
	//ドキュメントに変更があったときtrueを返す
	bool cut();


	//Undo
	//ドキュメントに変更があったときtrueを返す
	bool undo();

	bool redo();
protected:
	void discardRedo();

	bool isOnGrid(SPoint pt);

public:
	//コピー可能か？
	bool canCopy();
	//削除可能か？
	bool canDel();
	//切り取り可能か？
	bool canCut();
	//Undo可能か？
	bool canUndo();
	//Redo可能か？
	bool canRedo();




	//部品が1つだけ単独で選択されているか
	const SXBSchComponent* isComponentSelectedSingly();

	//カーソルが部品の上にあるかどうか
	const SXBSchComponent* isOnComponent(const SPoint& pt);

	//パーツが選択されているか
	bool isComponentSelected();
	//装飾線が選択されているか
	bool isDecorationlineSelected();
	//マーカーラインが選択されているか
	bool isMarkerlineSelected();
	//タグが選択されているか
	bool isTagSelected();
	//ラベルが選択されているか
	bool isLabelSelected();
	//コメントが選択されているか
	bool isCommentSelected();




	//Attribute変更が可能か?
	bool canAttribute();

	//Rotate
	//ドキュメントに変更があったときtrueを返す
	bool rotate();
	//Mirror
	//ドキュメントに変更があったときtrueを返す
	bool mirror();

	//Rotate可能か？
	bool canRotate();
	//Mirror可能か？
	bool canMirror();

	//EditPointsが可能か
	bool canEditPoints();

	//順序の変更が可能か？
	bool canChangeOrder();


	//バルクアトリビュート 部品情報の置換
	void bulkAttributeReplaceComponentInfo(const SCompIndex* pPartIndex);

	//バルクアトリビュート 部品名の非表示設定
	void bulkAttributeHideComponentVal(bool hide);

	//バルクアトリビュート 部品番号の非表示設定
	void bulkAttributeHideComponentNum(bool hide);


	//バルクアトリビュート コメントフォントの変更
	void bulkAttributeCommentFont(SXBSchComment& refComment);

	//バルクアトリビュート 装飾線の変更
	void bulkAttributeDecorationLine(SXBSchDash& refDecoline);

	//バルクアトリビュート マーカーラインの変更
	void bulkAttributeMarkerLine(int width,unsigned nColor);

	//バルクアトリビュート タグ枠の変更
	void bulkAttributeChangeTagFrameTo(int n);

	//ラベルからタグを生成
	SXBSchTag* generateTagFromLabel(SXBSchLabel* pObj);
	//ラベルをタグに変換
	void bulkConvertLabel2Tag();

	//ラベルからコメントを生成
	SXBSchComment* generateCommentFromLabel(SXBSchLabel* pObj);
	//ラベルをコメントに変換
	void bulkConvertLabel2Comment();



	//タグからラベルを生成
	SXBSchLabel* generateLabelFromTag(SXBSchTag* pObj);
	//タグをラベルに変換
	void bulkConvertTag2Label();

	//タグからコメントを生成
	SXBSchComment* generateCommentFromTag(SXBSchTag* pObj);
	//タグをコメントに変換
	void bulkConvertTag2Comment();



	//コメントからラベルを生成
	//SXBSchLabel* generateLabelFromComment(SXBSchComment* pObj);
	//コメントをラベルに変換
	void bulkConvertComment2Label();

	//コメントからタグを生成
	//SXBSchTag* generateTagFromComment(SXBSchComment* pObj);
	//コメントをタグに変換
	void bulkConvertComment2Tag();





	//レイヤーの移動
	bool moveToLayer(int nLayer);
	unsigned int visibleLayer(){return m_visibleLayer;}
	void setVisibleLayer(unsigned int n){m_visibleLayer=n&0xff;}
	int editLayer(){return m_editLayer;}
	void setEditLayer(int n){if(n>=0&&n<8)m_editLayer=n;}
	const TCHAR* layerName(int i) { return m_strLayerName[i].c_str(); }
	void setLayerName(int i, const TCHAR* lpcsz) { m_strLayerName[i] = lpcsz; setDirty(true); }



	//操作番号を返す
	int opCount(){return m_nOpCount;}
	//古いデータの削除
	void delOldData(int nOpCount);
	//指定した座標が一時データの上かどうか
	int isOnTempData(const SPoint& pt,int nOption,SXBSchObj** ppObj,int* pOption);
	//一時データを移動する
	void moveTempData(int offsetx,int offsety);
};

#endif  //#ifndef XBSCHDOC_H