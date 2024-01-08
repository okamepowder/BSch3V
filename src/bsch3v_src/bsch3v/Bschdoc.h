/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// BSchDoc.h : CBSchDoc クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////
#ifndef BSCHDOC_H
#define BSCHDOC_H

#include "xbschdoc.h"
#include "xbschdocsch.h"

#define	MAX_MAINDATA	1	//メインデータの数


//m_fTmpDataの値
//TMPDATA_SQAREA と TMPDATA_SINGLE	は排他的
//TMPDATA_MOVED  と TMPDATA_NEW		は排他的	
#define	TMPDATA_SQAREA	1	//一時データは矩形範囲を持つ
#define	TMPDATA_SINGLE	2	//一時データは単一図面オブジェクト
#define	TMPDATA_MOVED	4	//一時データはm_listMainDataから移動したもの
#define	TMPDATA_NEW		8	//一時データは新規配置データ
#define TMPDATA_DIRTY	16	//一時データを設定後、移動や回転などの操作を行った
#define TMPDATA_FOR_DRAG 32 //一時データはドラッグのためのもの
#define TMPDATA_NO_FRAME 64 //一時データの描画の際に点線枠を表示しない
			
//IsOnTmpData()の返値
//#define ONTMPDATA		1	//指定した点は一時データの上
//#define ONTMPDATA_NAME	2	//指定した点は部品名の上
//#define ONTMPDATA_NUM	4	//指定した点は部品番号の上



#include "xbschdocsch.h"

class CBSchDoc : public CDocument
{
protected: // シリアライズ機能のみから作成します。
	CBSchDoc();
	DECLARE_DYNCREATE(CBSchDoc)

//////////////////////////////////////////////////////////////////
// プライベート変数
private:

	SXBSchDocSch xbschdocsch;
	static SIZE m_tablesizeSheet[MAX_SHEETSIZE];

public:
	//レイヤーに関わるもの
	//WORD		m_wShowLayer;
	//int			m_nEditLayer;
	//BOOL		m_fEditHighLight;

	unsigned int visibleLayer(){return xbschdocsch.visibleLayer();}
	void setVisibleLayer(unsigned int n){xbschdocsch.setVisibleLayer(n);}
	int editLayer(){return xbschdocsch.editLayer();}
	void setEditLayer(int n){xbschdocsch.setEditLayer(n);}
	void setNcPinFlag(){xbschdocsch.setNcPinFlag();}
	const TCHAR* layerName(int i) { return xbschdocsch.layerName(i); }
	void setLayerName(int i, const TCHAR* lpcsz) { xbschdocsch.setLayerName(i, lpcsz); }


public:
	void setTotalPages(int n){xbschdocsch.setTotalPages(n);}
	void setPage(int n){xbschdocsch.setPage(n);}
	void setProjectName(const TCHAR* psz){xbschdocsch.setProjectName(psz);}
	void setInitPos(int n){xbschdocsch.setInitPos(n);}
	
	int totalPages(){return xbschdocsch.totalPages();}
	int page(){return xbschdocsch.page();}
	const TCHAR* projectName(){return xbschdocsch.projectName();}
	int dataVersion(){return xbschdocsch.dataVersion();}
	int initPos(){return xbschdocsch.initPos();}




private:


// オペレーション
public:
//	void	IncrementDataDepth();	//有効なm_listMainDataの数のインクリメント
//									//最大数になったら古いデータの統合を自動で行う
	void	FixSelection(int option);			//選択の解除
	int		SetSel(CRect* prc,SelectOption& refSelOption);		//矩形範囲での選択
	int		SetSel(CPoint* pp,SelectOption& refSelOption);		//点指定での選択
	int		SetDrag(CRect* prc,bool bCutOffPinWire);	//矩形範囲でのドラッグモード選択
	int		SetDrag(CPoint* pp,bool bCutOffPinWire);	//点指定でのドラッグモード選択
	bool	isSelectedForDtag(){return xbschdocsch.isSelectedForDtag();}
	int		GetTmpDataCount(){return xbschdocsch.m_listTemp.size();} //一時データの数を返す。

//	UINT	GetTmpDataFlag(){return m_fTmpData;}//現在の一時データの識別フラグを返す
	XBSchObjList* GetTmpList();			//一時データのリストを得る
	SXBSchObj* GetTmp();				//一時データが単一図面オブジェクトのとき、それを返す
	CPoint  GetTmpOrigin();				//一時データの原点を返す
	CRect	GetTmpRect();				//一時データの占有範囲を返す
	//void 	SetTmpRect(CRect* prc){m_rcTmpData=*prc;}	//一時データの占有範囲外部から設定する
	int		IsOnTmpData(CPoint* pp,int nOption,SXBSchObj** ppObj,int* pRetOption);//指定した点が一時データの上かどうか

	BOOL	IsEmpty();				//データが空かどうかを返す
	void	Paste(CPoint* pOrigin); //ペースト
	void	Cut();					//カット
	void	Copy();					//コピー
	void	Clear();				//クリア
	void	Undo();					//アンドゥ
	bool	Redo();					//アンドゥ
	void	Rotate();				//回転する
	void	Mirror();				//左右反転する
	void	Shift(CPoint* pOrigin);	//移動する
	void	Shift(CSize*  psizeOffset);	//移動する
//	BOOL	DoAttributeDialog();	//アトリビュート変更ダイアログの起動
	void	SetDirty();				//ダーティフラグをセットする
	void	SetTempDirty();				//一時データのダーティフラグをセットする
	BOOL	CanEdit();				//選択されたものが存在するかどうか
	BOOL	CanPaste();				//ペースト可能かどうか
	BOOL	CanUndo();				//アンドゥ可能かどうか
	BOOL	CanRedo();				//アンドゥ可能かどうか
	BOOL	CanAttribute();			//アトリビュートの変更が可能かどうか
	BOOL	CanRotate();			//回転が可能かどうか
	BOOL	CanMirror();			//左右反転が可能かどうか
	BOOL	CanEditPoints();		//点の編集が可能か
	BOOL	CanChangeOrder();		//順序の変更が可能か？

	//部品が1つだけ単独で選択されているか
	const SXBSchComponent* isComponentSelectedSingly();

	//カーソルが部品の上にあるかどうか
	const SXBSchComponent* isOnComponent(CPoint *pp);


	//パーツが選択されているか
	bool isComponentSelected(){return xbschdocsch.isComponentSelected();}
	//装飾線が選択されているか
	bool isDecorationlineSelected(){return xbschdocsch.isDecorationlineSelected();}
	//マーカーラインが選択されているか
	bool isMarkerlineSelected(){return xbschdocsch.isMarkerlineSelected();}
	//タグが選択されているか
	bool isTagSelected(){return xbschdocsch.isTagSelected();}
	//ラベルが選択されているか
	bool isLabelSelected(){return xbschdocsch.isLabelSelected();}
	//コメントが選択されているか
	bool isCommentSelected(){return xbschdocsch.isCommentSelected();}




	//検索まわりはつぎはぎだらけ。旧検索の機能群を流用したせい。
	BOOL	CanFindNext();			//「次の検索」が可能かどうか
	const SXBSchObj* FindStart(const TCHAR* sz,bool bMatchWhole,bool bCaseSensitive,unsigned int targetFlags,bool selectWhenFound);
	const SXBSchObj* Find();
	void GetFoundText(CString& str);
	unsigned int GetFountType(){ return xbschdocsch.m_foundType;}
	SPoint GetFoundPoint(){ return xbschdocsch.m_foundPoint;}



	void setTidyUpEnable(bool en){xbschdocsch.m_bDoTidyUp = en;}
	bool tidyUp(){return xbschdocsch.m_bDoTidyUp;}

	//バルクアトリビュート 部品情報の置換
	void bulkAttributeReplaceComponentInfo(const SCompIndex* pPartIndex){xbschdocsch.bulkAttributeReplaceComponentInfo(pPartIndex);}

	//バルクアトリビュート 部品名の非表示設定
	void bulkAttributeHideComponentVal(bool hide){xbschdocsch.bulkAttributeHideComponentVal(hide);}

	//バルクアトリビュート 部品番号の非表示設定
	void bulkAttributeHideComponentNum(bool hide){xbschdocsch.bulkAttributeHideComponentNum(hide);}

	//バルクアトリビュート コメントフォントの変更
	void bulkAttributeCommentFont(SXBSchComment& refComment){xbschdocsch.bulkAttributeCommentFont(refComment);}

	//バルクアトリビュート マーカーラインの変更
	void bulkAttributeMarkerLine(int width,unsigned nColor){xbschdocsch.bulkAttributeMarkerLine(width,nColor);}

	//バルクアトリビュート 装飾線の変更
	void bulkAttributeDecorationline(SXBSchDash& refDecoline){xbschdocsch.bulkAttributeDecorationLine(refDecoline);}


	//バルクアトリビュート タグ枠の変更
	void bulkAttributeChangeTagFrameTo(int n){xbschdocsch.bulkAttributeChangeTagFrameTo(n);}

	//コメントをラベルに変換
	void bulkConvertComment2Label(){xbschdocsch.bulkConvertComment2Label();}

	//コメントをラベルに変換
	void bulkConvertComment2Tag(){xbschdocsch.bulkConvertComment2Tag();}

	//ラベルをタグに変換
	void bulkConvertLabel2Tag(){xbschdocsch.bulkConvertLabel2Tag();}

	//ラベルをコメントに変換
	void bulkConvertLabel2Comment(){xbschdocsch.bulkConvertLabel2Comment();}

	//タグをラベルに変換
	void bulkConvertTag2Label(){xbschdocsch.bulkConvertTag2Label();}

	//タグをコメントに変換
	void bulkConvertTag2Comment(){xbschdocsch.bulkConvertTag2Comment();}







	int		opCount(){return xbschdocsch.opCount();}

	BOOL	AddData(SXBSchObj* pObj);//図面オブジェクトの仮追加
	XBSchObjList* GetMainData(int n=0);	//先頭からn個目のデータのリストへのポインタを得る
	

	void	SetSheetSize(int n,int w,int h);	//図面サイズを番号でセットする
	CSize	GetSheetSize();//図面サイズを返す
	int		GetSheetSizeIndex();//図面サイズを返す

	//レジストリから図面サイズを得る
	void  IniReadSheetSize(CSize* psize);
	//int		IniReadSheetSize();		//レジストリから図面サイズを得る
	//void	IniWriteSheetSize(int nSize);//レジストリに図面サイズを書き込む
	//レジストリに図面サイズを書き込む
	void IniWriteSheetSize(CSize* pSize);

	void	MoveLayer(int nLayer);	//レイヤーの移動
	void	setupRcTempArea(){xbschdocsch.setupRcTempArea();}

	BOOL SaveFile(){return DoFileSave();}	//2005/10/16
	BOOL ReloadFile();

//	void	setPointTmpData(CPoint& point){m_pointTmpData=point;}

//オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CBSchDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CBSchDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
public:
	BOOL IsModified();

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CBSchDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual void SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE);
};

/////////////////////////////////////////////////////////////////////////////

#endif
