/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// BSchView.h : CBSchView クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////
#ifndef BSCHVIEW_H
#define BSCHVIEW_H

#include <atlimage.h>


#define AUTOSCROLL_FRAME	2	//クライアントエリアの内側(AUTOSCROLL_FRAME)ドット
                                //より外側なら自動スクロールする
//以下の４つはプライベート関数のQueryAutoScroll()が返す値
#define AUTOSCROLL_LEFT		1
#define AUTOSCROLL_TOP		2
#define AUTOSCROLL_RIGHT	4
#define AUTOSCROLL_BOTTOM	8

// DrawListData()の 引数dwOptionで使う値


//#define EXCEPT_DRAG_LINE	512
//#define DRAG_LINE_ONLY		1024
#define DRAW_ALL_LAYER		2048
#define DRAW_TEMP_DATA		1024

#define MAX_ZOOM_STEP_NUM 8


#include "BSchDrawColor.h"

class CBSchDoc;

class CQuickProp;

class CDlgObjFilter;

class CFind2;

class CDispBlockNum;

class FindInfo
{
public:
	CString m_str;
	BOOL	m_bMatchWhole;
	BOOL	m_bCaseSensitive;
	unsigned int m_targetFlag;
};

class QuickPropSetting
{
public:
	bool m_enable;
	bool m_showOnMouseHover;
	bool m_num;
	bool m_val;
	bool m_mfr;
	bool m_mfrpn;
	bool m_package;
	bool m_note;
	bool m_nameinlib;
	QuickPropSetting(){
		m_num = false;
		m_val = false;
		m_enable = false;
		m_showOnMouseHover = false;
		m_mfr = false;
		m_mfrpn = false;
		m_package = false;
		m_note = false;
		m_nameinlib = false;
	};
	bool isEnabled(){
		return m_enable && (m_num || m_val || m_mfr || m_mfrpn || m_package || m_note || m_nameinlib);
	}

};

//左右ボタンによるマウスのキャプチャ状態を管理するクラス
class MouseCaptureInfo
{
public:
	enum CAPTUREBUTTON
	{
		MCI_LEFT = 1,
		MCI_RIGHT = 2
	};
protected:
	CWnd* m_pWnd;
	unsigned m_captureValue;
public:
	MouseCaptureInfo(){
		m_captureValue = 0;
		m_pWnd = NULL;
	}
	void Init(CWnd* pWnd){
		m_captureValue = 0;
		m_pWnd = pWnd;
	}
	unsigned SetCapture(CAPTUREBUTTON button);
	unsigned ReleaseCapture(CAPTUREBUTTON button);
	bool isLButtonCapture(){return (m_captureValue & (unsigned)MCI_LEFT);}
	bool isRButtonCapture(){return (m_captureValue & (unsigned)MCI_RIGHT);}
};



class CBSchView : public CScrollView
{
public:
	static SBSchDrawColor m_COL;

	//static COLORREF m_COL_BG;
	//static COLORREF m_COL_JUNC;
	//static COLORREF m_COL_WIRE;
	//static COLORREF m_COL_BUS;
	//static COLORREF m_COL_DASH;
	//static COLORREF m_COL_TAG;
	//static COLORREF m_COL_TAG_NAME;
	//static COLORREF m_COL_COMMENT;
	//static COLORREF m_COL_LABEL;
	//static COLORREF m_COL_XOR;
	//static COLORREF m_COL_PART;
	//static COLORREF m_COL_PART_PIN;
	//static COLORREF m_COL_PART_PINNAME;
	//static COLORREF m_COL_PART_PINNUM;
	//static COLORREF m_COL_PART_NAME;
	//static COLORREF m_COL_PART_NUM;
	//static COLORREF m_COL_PART_FRAME;
	//static COLORREF m_COL_SHEET_FRAME;
	//static COLORREF m_COL_SHEET_GRID;
	//static COLORREF m_COL_DARK;
	//static COLORREF m_COL_ZLPIN;
	//static COLORREF m_COL_NC_MARK;
	//static COLORREF m_COL_QUICKPROPVIEW;
	//static COLORREF m_COL_TEMPFRAME_FOCUSED;
	//static COLORREF m_COL_TEMPFRAME_NOT_FOCUSED;


	static void IniReadColor();
	static void IniWriteColor();

public:



protected:
	void IniReadMarkerLineInitValue();
	void IniWriteMarkerLineInitValue();
	void IniReadMarkerLineCustomColors(COLORREF* lpColors);
	void IniWriteMarkerLineCustomColors(COLORREF* lpColors);


//クイックプロパティ
protected:
	DISP_BLOCKNUM_TYPE m_dispBN;
	bool m_displayQuickPropComp;
	CQuickProp *m_pQuickPropComp;
	void ShowQuickPropComp(const SXBSchComponent* pObj);
	void HideQuickPropComp();
	CPoint m_ptDisplayQuickPropAt;
	QuickPropSetting m_quickPropSetting;
	void IniReadQuickPropSetting();
	void IniWriteQuickPropSetting();
	LOGFONT m_lfQuickPropFont;


//追加選択
protected:
	bool m_allowAppendSelect;
	BOOL IniReadAllowAppendSelect();
	void IniWriteAllowAppendSelect();




protected: // シリアライズ機能のみから作成します。
	CBSchView();
	DECLARE_DYNCREATE(CBSchView)

// アトリビュート
public:
	CBSchDoc* GetDocument();

	void IniReadFindInfo(FindInfo& fi);
	void IniWriteFindInfo(FindInfo& fi);

//////////////////////////////////////////////////////////////////
// プライベート変数
private:

	////////////////////////////////////////////////////
	//座標関連
	CPoint m_ptMouseDP;		//デバイス座標で表すマウスの座標(OnMouseMove(),OnLButtonDown(),OnLButtonUp()で変更)
	CPoint m_ptMouse;		//図面座標で表すマウスの座標(OnMouseMove(),OnLButtonDown(),OnLButtonUp()で変更)
	CPoint m_ptMoveFrom;	//キャプチャを始めた座標
	CPoint m_ptMoveTo;		//前回のマウスの図面座標
	CRect m_rcClip;			//図面座標で表したクリッピング領域（図面オブジェクトの描画の前に必ず設定)
							//OnPrepareDC()の中で設定している
	//CSize m_sizeClientArea;	//クライアントエリアのサイズ







	
	////////////////////////////////////////////////////
	//部品の選択の状態に関わるもの
	int m_nLastSelectLibrary;	//最後に選んだライブラリのリストボックスでの順序番号
	int m_nLastSelectPart;		//最後に選んだ部品のリストボックスでの順序番号



	////////////////////////////////////////////////////
	// 選択時のフィルター
	unsigned m_ObjectFileterValue;
	CDlgObjFilter* m_pObjFilterDlg;

	////////////////////////////////////////////////////
	// 検索
	//検索ダイアログ
	CFind2* m_pFind2Dlg;
	//検索関係変数
	bool m_FindMarkActive;
	CPoint m_ptFindMark;
	
	bool isFindMarkActive(); 



	//BOOL m_bUsePreview;

	BOOL m_fEditHighLight;
	////////////////////////////////////////////////////
	//ツール選択・操作・表示の状態に関わるもの
	int m_nCurrentTool;		//現在選択しているツール

	MouseCaptureInfo m_mouseCaptureInfo;

//20100521		BOOL m_bCaptureMouse;	//マウスをキャプチャしていたらTRUE
//20100521		BOOL m_bRBCaptureMouse;	//右ボタン操作でマウスをキャプチャしていたらTRUE


	BOOL m_bOnSelect;		//範囲選択中ならTRUE
	UINT m_nOnMove;			//非０のとき、何かを移動中。
							//値は CBSchDoc::IsOnTmpData()の返値を取る
							// ONTMPDATA      (1)	指定した点は一時データの上
							// ONTMPDATA_NAME (2)	指定した点は部品名の上
							// ONTMPDATA_NUM  (4)	指定した点は部品番号の上
	
	BOOL m_bSetWireDirection;	//これがTRUEのときはワイヤー位置の１本目の方向がセット済み
	BOOL m_bWireHorizon1st;		//これがTRUEのときはワイヤー位置の１本目が水平

	CPoint m_pointOccasional;	//臨時データの座標
	//m_bDrawXXXXXXはXORで描画するオブジェクトの描画フラグ
	//これがTUREのとき、OnDraw()はXORのオブジェクトを描画する
	//いったん描画した場所に再描画して消去する場合は、意図的にFALSEを設定すること
	//「描画の度にトグルする」というのは、部分再描画も有り得るのだからうまくいかない
	//これらはOnInitialUpdate()でクリアする。
	BOOL m_bDrawSelectFrame;	//XORで描画する選択枠			描いたらTRUE
	BOOL m_bDrawTmpData;		//XORで描画する一時データ		描いたらTRUE
	BOOL m_bDrawOccasional;		//XORで描画する臨時データ		描いたらTRUE
	BOOL m_bDrawTmpDataFrame;	//XORで描画する一時データの枠	描いたらTRUE
	
	CBitmap* m_pBmpBuff;	//オフスクリーンバッファ
//	CSize   m_sizeTmpCacheOffset;//キャッシュビットマップのオフセット

	BOOL m_bSnaptoGrid;			//グリッドへのスナップ。コンストラクタでTRUEに初期化。
	BOOL m_bTimerActive;		//移動処理時にタイマーが有効なときにTRUE
	BOOL m_bTimer2Active;		//自動スクロールのタイマーが有効なときにTRUE

	int m_nWHEEL_DELTA;


	int m_oldContentsX;
	int m_oldContentsY;
	int m_oldOpCount;
	int m_oldViewScale;
	bool m_clearTempRect;

	SXBSchMarkerLine m_tempMarkerLine;
	SXBSchDash m_tempDecoLine;

	bool m_bEditPointsMode;
	SXBSchObj* m_pObjDetectedOnTempData;
	int m_nRetOptionOnTempData;


	bool m_bClickClickLineMode;	//0.69 クリッククリックでラインを配置するモード
	int m_nPlaceSequence;		//0.69 クリッククリックで配置するときのシーケンス番号
	bool placeLineNow(){ return(m_bClickClickLineMode && m_nPlaceSequence>0); }


	////////////////////////////////////////////////////
	//選択オブジェクトのフォーカスに関わるもの
private:
	int m_nFocusedTo;
//public:
//	enum{
//		FOCUSED_TO_BODY = 0,
//		FOCUSED_TO_NUM = 1,
//		FOCUSED_TO_VAL = 2
//	};

	
public:
	////////////////////////////////////////////////////
	//検索結果の指定位置図面座標へのスクロール
	void ScrollToFoundPos(CPoint pt);
	void OnCloseFind2Dialog();


	////////////////////////////////////////////////////
	//右ボタンスクロールに関わるもの
private:
	int m_nRButtonMode;
	CPoint m_ptRBScrlStartMouse;
	CPoint m_ptRBScrlStartPosition;
	

	////////////////////////////////////////////////////
	//ズームに関わるもの
	int m_nVExt;			//表示倍率(View)
	int m_nWExt;			//表示倍率(Window)
	int m_nZoom;			//0～2のズーム値 メンバ関数SetZoomの引数
	CPoint m_pointUpdateCenter;
	BOOL m_bZoomUpdate;		//OnUpDate()でズーム中のときTRUE 1997/01/26
				//これがTRUEのとき、OnUpDate()で指定点へのスクロールを行う



	////////////////////////////////////////////////////
	//表示オプションに関わるもの
	BOOL m_bDisplayGrid;		//グリッドの表示
	BOOL m_bUseCrossCursor;		//クロスカーソルの使用
	BOOL m_bCoarseStepZoom;		//粗いステップのズーム
//	BOOL m_bDisplayPartFrame;	//部品枠の表示   1997/01/01 グローバル変数に移動
	
	/////////////////////////////////////////////////////
	//印刷に関わるもの
//	int m_nPrintMagX;			//水平方向倍率（分子）
//	int m_nPrintMagY;			//垂直方向倍率（分子）
//	int m_nPrintMagX2;			//水平方向倍率（分母）
//	int m_nPrintMagY2;			//垂直方向倍率（分母）
	int m_nPrintVExt;
	int m_nPrintWExt;
	bool m_bPrintColor;
	bool m_bPrintPageFit;


	CPoint m_pointPageLT;			//ページ印刷時の左上の用紙上のピクセル
	CSize m_sizePageDevicePixel;	//ページ印刷時の用紙上のピクセルサイズ
	CPoint m_pointPrintOrigin;	//ページ印刷時の左上の座標
	CSize m_sizePrintPixel;		//１ページに出力可能な図面上でのサイズ
	int m_nPageX;				//水平方向のページ数
	int m_nPageY;				//垂直方向のページ数

	int m_bPrintFrame;			//印刷時の図面枠出力

	SXBSchComment m_cmntFooter;
	bool m_bPrintFooter;

	void preparePrintVars();


	//印刷解像度設定 1998/07/04
	//int	 m_nPrintRes;					//手動設定の印刷解像度 0なら自動設定
	BOOL m_bBitmapPrint;
	int GetDefaultPrintRes(CDC* pDC);	//標準の解像度を得る

	bool m_bSetColor2Active;


	///////////////////////////////////////
	//キー操作に関わるもの
//	BOOL m_fShiftKeyDown;	//シフトキーの押下でセットされる
	int  m_nRepeatArrowKey;	//矢印キーのリピート数

public:
	void updateBuff(bool bForce = true);

//////////////////////////////////////////////////////////////////
// プライベート関数
private:
	void SetZoom(int n);			//ズーム値の設定
	void BSchPtoLP(LPPOINT pp,int nVExt,int nWExt);		//図面座標を論理座標に変換する(LPPOINT版)
	void BSchPtoLP(LPSIZE psize,int nVExt,int nWExt);	//図面座標を論理座標に変換する(LPSIZE版)
	void BSchPtoLP(LPRECT prc,int nVExt,int nWExt);		//図面座標を論理座標に変換する(LPRECT版)
	void LPtoBSchP(LPPOINT pp,int nVExt,int nWExt);		//論理座標を図面座標に変換する(LPPOINT版)
	void LPtoBSchP(LPSIZE psize,int nVExt,int nWExt);	//論理座標を図面座標に変換する(LPSIZE版)
	void LPtoBSchP(LPRECT prc,int nVExt,int nWExt);		//論理座標を図面座標に変換する(LPRECT版)
	void SnapToGrid(LPPOINT pp);	//グリッドへのスナップ
	CPoint GetClientCenter();		//クライアントエリアの中央の図面座標
	CPoint GetSheetLTforScroll(CPoint* ppointCenter);//指定した点を中央とするスクロールポジションを得る

	BOOL IniReadCoarseStepZoom(); //レジストリから粗いステップのズームを使うかどうかの設定を読み込む
	void IniWriteCoarseStepZoom(BOOL bCoarse);//レジストリに粗いステップのズームを使うかどうかの設定を書き込む

	BOOL IniReadUseCrossCursor();//レジストリからクロスカーソル表示の情報を得る
	void IniWriteUseCrossCursor(BOOL bDisplay);//レジストリにクロスカーソル表示の情報を書き込む

	DISP_BLOCKNUM_TYPE IniReadDisplayBlockNum();				//レジストリからブロック番号表示の情報を得る
	void IniWriteDisplayBlockNum(DISP_BLOCKNUM_TYPE dbnt);		//レジストリにブロック番号表示の情報を書き込む


	BOOL IniReadDisplayGrid();		//レジストリからグリッド表示の情報を得る
	void IniWriteDisplayGrid(BOOL bDisplay);//レジストリにグリッド表示の情報を書き込む

	BOOL IniReadDisplayPartFrame();	//レジストリから部品枠表示の情報を得る
	void IniWriteDisplayPartFrame(BOOL bDisplay);//レジストリに部品枠表示の情報を書き込む

	BOOL IniReadDisplayNcPinMark();	//レジストリからNCピンマーク表示の情報を得る
	void IniWriteDisplayNcPinMark(BOOL bDisplay);//レジストリにNCピンマーク表示の情報を書き込む

	BOOL IniReadDisplayHiddenPinNum();//レジストリから非表示ピン番号表示の情報を得る
	void IniWriteDisplayHiddenPinNum(BOOL bDisplay);//レジストリに非表示ピン番号表示の情報を書き込む

	BOOL IniReadUsePreview();//レジストリから部品プレビューの情報を得る
	void IniWriteUsePreview(BOOL bUsePreview);//レジストリに部品プレビューの情報を書き込む

	BOOL IniReadPrintPageFit();//レジストリから印刷ページフィット情報の読み込み
	void IniWritePrintPageFit(BOOL pagefit);//レジストリに印刷ページフィット情報の書き込み


	int  IniReadPrintRes();			//レジストリから印刷解像度の読み込み
	void IniWritePrintRes(int nRes);//レジストリに印刷解像度の書き込み
	int  IniReadExpImageMag();		//レジストリから画像出力倍率の読み込み 2016/05/03
	void IniWriteExpImageMag(int nMag);	//レジストリに画像出力倍率の書き込み 2016/05/03


	void IniReadPrintMargin(int& left,int& upper,int& right,int& lower);//レジストリから印刷マージンの読み込み
	void IniWritePrintMargin(int left,int upper,int right,int lower);//レジストリに印刷マージンの書き込み

	BOOL CBSchView::IniReadClickClickLineMode();	//レジストリからクリッククリックラインモードの情報を得る
	void CBSchView::IniWriteClickClickLineMode(BOOL bClickClick);	//レジストリにクリッククリックラインモードの情報を書き込む


	//コメントのクラスにデフォルトのフォントを設定する
	void SetDefaultFontSettingsToCommentClass();


	void OnSheetXXX(int n);			//イベントハンドラOnSheet640～3000()から呼ばれる
	
	void DrawSheetGrid(CDC* pDC,const CRect& rcClip);	//グリッドの描画 "BScView2.CPP"

	void DrawSheetFrameCharH(CDC* pDC,COLORREF col,int ny,int nWidth,UINT nMode,int nVExt,int nWExt);//図面枠の水平座標文字の描画
	void DrawSheetFrameCharV(CDC* pDC,COLORREF col,int nx,int nWidth,UINT nMode,int nVExt,int nWExt);//図面枠の垂直座標文字の描画
	void DrawSheetFrame(CDC* pDC,DWORD dwMode,int nVExt,int nWExt,const CRect& rcClip);	//図面枠の描画 "BScView2.CPP"

	void DrawPrintBg(CDC* pDC,DWORD dwMode,int nVExt,int nWExt,const CRect* prc);	//印刷時の背景色の描画 "BScView2.CPP"


	//void updateBuff(bool bForce = true);
	void drawBuffer(int x,int y,int w,int h,int clipx,int clipy, int clipw, int cliph );

	void DrawObj(							//図面要素を描画する
		CDC* pDC,							//描画先
		SXBSchObj* pObj,						//描画オブジェクト
		DWORD dwMode,						//描画モード
		int  nVExt,							//表示倍率(View)
		int  nWExt,							//表示倍率(Window)
		LPCRECT lprcClip=NULL);				//クリップ情報


	void DrawListData(CDC* pDC,XBSchObjList* pList,DWORD dwMode,
		int  nVExt,int  nWExt,
		const CRect& rcClip,DWORD dwOption,DWORD layerMask=0xFFFF);//指定リストデータの描画 "BScView2.CPP"
	void DrawMainData(CDC* pDC,CBSchDoc* pDoc,DWORD dwMode,
		int  nVExt,int  nWExt,
		const CRect& rcClip);//メインデータの描画 "BScView2.CPP"
	void DrawTmpData(CDC* pDC,CBSchDoc* pDoc,DWORD dwMode,
		int  nVExt,int  nWExt,
		const CRect& rcClip,BOOL bDraw=TRUE);	//一時データの描画 "BScView2.CPP"
	void DrawTmpDataFrame(CDC* pDC,CBSchDoc* pDoc,BOOL bDraw=TRUE);//一時データの枠の描画 "BScView2.CPP"
	void DrawSelectFrame(CDC* pDC,BOOL bDraw=TRUE);			//選択中の点線枠の描画 "BScView2.CPP"
//	BOOL PrepareBmpTmpCache();	//一時データ描画のためのキャッシュビットマップの準備 "BScView2.CPP"
	void DrawOccasional(CDC* pDC,BOOL bDraw=TRUE);	//臨時データの描画 "BScView2.CPP"

	void DrawCrossCursor(CDC* pDC);
	void DrawFindMark(CDC* pDC);

	void DisplayCurrentXY();		//m_ptMouseの値をツールバーに反映する。
	UINT QueryAutoScroll(CDC* pDC);	//自動スクロールの必要性を問う
	void AutoScroll(CDC* pDC,UINT fScroll);	//自動スクロールの発生
	void ChangeTool(int nOldTool,int nNewTool);//ツールを変えたときの共通処理

	void OnZoomInOutProc(BOOL bZoomIn,BOOL bKey =FALSE);	//ズームインアウトの処理

	void FixSelection(int option);	//選択の解除

	SXBSchComponent* CanEditWithLCoV();	//LCoVによる部品情報の編集が可能なとき、有効な部品オブジェクトを返す

	///////////////////////////////////////////////////
	//  メニューアイテムのイネーブルの可否を返す
	//    メニューアイテムのイネーブルの可否をメインメニューとコンテキストメニュー
	//  とで、同一のロジックで行うため
	//    1997/01/23 コンテキストメニューの追加とともにこの関数群を追加する
	BOOL MenuItemTest_Undo();		//元に戻す  1997/01/27 追加
	BOOL MenuItemTest_Redo();		//やりなおし 2008/01/13 追加
	BOOL MenuItemTest_Cut();		//切り取り
	BOOL MenuItemTest_Copy();		//コピー
	BOOL MenuItemTest_Paste();		//貼り付け
	BOOL MenuItemTest_Attribute();	//アトリビュート
	BOOL MenuItemTest_Mirror();		//ミラー
	BOOL MenuItemTest_Rotate();		//回転

	BOOL MenuItemTest_MoveToLayer(int nLayer);	//レイヤーの移動 1998/06/28
	
	//////////////////////////////////////
	//配置関連
	BOOL PlaceJunction(CBSchDoc* pDoc);//ジャンクションの配置 "PLACE.CPP"
	BOOL PlacePart(CBSchDoc* pDoc,CPoint* ppoint);//パーツの配置 "PLACE.CPP"
	BOOL PlaceEntry(CBSchDoc* pDoc);//エントリーの配置 "PLACE.CPP"
	BOOL PlaceLabel(CBSchDoc* pDoc,CPoint* ppoint);//ラベルの配置 "PLACE.CPP"
	BOOL PlaceComment(CBSchDoc* pDoc,CPoint* ppoint);//コメントの配置 "PLACE.CPP"
	BOOL PlaceTag(CBSchDoc* pDoc,CPoint* ppoint);//タグの配置 "PLACE.CPP"
	BOOL PlaceWire(CBSchDoc* pDoc);//ワイヤーの配置 "PLACE.CPP"
	BOOL PlaceBus(CBSchDoc* pDoc);//バスの配置 "PLACE.CPP"
	BOOL PlaceDash(CBSchDoc* pDoc);//破線の配置 "PLACE.CPP"
	BOOL PlaceMarkerLine(CBSchDoc* pDoc);//マーカーラインの配置 "PLACE.CPP"
	
	//////////////////////////////////////
	//レイヤー関連
	void MoveLayer(int n);	//1998/06/30  選択範囲を指定レイヤーに移動
	void OnUpdateDisplayLayer(CCmdUI *pCmdUI,int layer);
	void OnClickDisplayLayer(int layer);
	void OnUpdateEditLayer(CCmdUI *pCmdUI,int layer);
	void OnClickEditLayer(int layer);



	///////////////////////////////////////////////
	//印刷関連
	void SetPageSize(CDC* pDC,int nRes);	//印刷時の倍率、サイズを決める
	int  GetCountPage();		//総ページ数を得る
	void SaveToImage(CImage& image);
	BOOL ExportBitmapFile(LPCTSTR pszFileName);	//ビットマップファイル出力
	BOOL ExportToClipbord();
	//BOOL ExportBidFile(LPCSTR pszFileName);		//BIDファイル出力
	BOOL ExportEmf(LPCTSTR pszFileName);

	BOOL CopyEmf();

	
	void PrepareDrawFlagForPrint(CDC* pDC);	//印刷のための描画フラグの準備
	void PopDrawFlagForPrint();		//印刷前にPrepareDrawFlagForPrint()で操作した
	                                //描画フラグを元に戻す
	
	// オペレーション
	//アトリビュート変更ダイアログの起動
	BOOL DoAttributeDialog();

	//コメントのアトリビュート変更用のダイアログを起動
	BOOL DoAttributeDialogComment(SXBSchComment* pObj);
	//ラベルのアトリビュート変更用のダイアログを起動
	BOOL DoAttributeDialogLabel(SXBSchLabel* pObj);
	//タグのアトリビュート変更用のダイアログを起動
	BOOL DoAttributeDialogTag(SXBSchTag* pObj);
	//部品アトリビュート変更用のダイアログを起動
	BOOL DoAttributeDialogComponent(SXBSchComponent* pObj);
	//マーカーラインアトリビュート変更用のダイアログを起動
	BOOL DoAttributeDialogMarkerLine(SXBSchMarkerLine* pObj);
	//装飾線アトリビュート変更用のダイアログを起動
	BOOL DoAttributeDialogDecoLine(SXBSchDash* pObj);
	//イメージオブジェクトアトリビュート変更用のダイアログを起動
	BOOL DoAttributeDialogImageObj(SXBSchImageObj* pObj);

public:
	void DisplayMessageLine();

private:
	CString m_strMessageLine;


public:


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CBSchView)
	public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画する際にオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual void OnInitialUpdate(); // 構築後の最初の１度だけ呼び出されます。
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CBSchView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void OnExtMenuN(int n);
	void ExecExternalTool(const std::wstring& FileName);
	std::wstring GetAppPath();

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CBSchView)
	afx_msg void OnToolSelector();
	afx_msg void OnUpdateToolSelector(CCmdUI* pCmdUI);
	afx_msg void OnToolDrag();
	afx_msg void OnUpdateToolDrag(CCmdUI* pCmdUI);
	afx_msg void OnToolPart();
	afx_msg void OnUpdateToolPart(CCmdUI* pCmdUI);
	afx_msg void OnToolWire();
	afx_msg void OnUpdateToolWire(CCmdUI* pCmdUI);
	afx_msg void OnToolBus();
	afx_msg void OnUpdateToolBus(CCmdUI* pCmdUI);
	afx_msg void OnToolDash();
	afx_msg void OnUpdateToolDash(CCmdUI* pCmdUI);
	afx_msg void OnToolJunction();
	afx_msg void OnUpdateToolJunction(CCmdUI* pCmdUI);
	afx_msg void OnToolEntryWl();
	afx_msg void OnUpdateToolEntryWl(CCmdUI* pCmdUI);
	afx_msg void OnToolEntryBl();
	afx_msg void OnUpdateToolEntryBl(CCmdUI* pCmdUI);
	afx_msg void OnToolTag();
	afx_msg void OnUpdateToolTag(CCmdUI* pCmdUI);
	afx_msg void OnToolLabel();
	afx_msg void OnUpdateToolLabel(CCmdUI* pCmdUI);
	afx_msg void OnToolComment();
	afx_msg void OnUpdateToolComment(CCmdUI* pCmdUI);
	afx_msg void OnSetLibrary();
	afx_msg void OnSetCommentFont();
	afx_msg void OnUpdateZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnZoomIn();
	afx_msg void OnUpdateZoomOut(CCmdUI* pCmdUI);
	afx_msg void OnZoomOut();
	afx_msg void OnSheet640();
	afx_msg void OnSheet1000();
	afx_msg void OnSheet1600();
	afx_msg void OnSheet2000();
	afx_msg void OnSheet3000();
	afx_msg void OnUpdateDispGrid(CCmdUI* pCmdUI);
	afx_msg void OnDispGrid();
	afx_msg void OnUpdatePartFrame(CCmdUI* pCmdUI);
	afx_msg void OnPartFrame();
	afx_msg void OnUpdateSnapGrid(CCmdUI* pCmdUI);
	afx_msg void OnSnapGrid();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditDelete(CCmdUI* pCmdUI);
	afx_msg void OnEditDelete();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnTimer(UINT nIDEvent);
//	afx_msg void OnRefresh();
	afx_msg void OnUpdateRotate(CCmdUI* pCmdUI);
	afx_msg void OnRotate();
	afx_msg void OnUpdateMirror(CCmdUI* pCmdUI);
	afx_msg void OnMirror();
	afx_msg void OnUpdateEditAttribute(CCmdUI* pCmdUI);
	afx_msg void OnEditAttribute();
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnFileExpoBmp();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnFilePrintOption();
//	afx_msg void OnUsePreview();
//	afx_msg void OnUpdateUsePreview(CCmdUI* pCmdUI);
	afx_msg void OnEditSetuplayer();
	afx_msg void OnUpdateEditMovetolayer0(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditMovetolayer1(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditMovetolayer2(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditMovetolayer3(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditMovetolayer4(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditMovetolayer5(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditMovetolayer6(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditMovetolayer7(CCmdUI* pCmdUI);
	afx_msg void OnEditMovetolayer0();
	afx_msg void OnEditMovetolayer1();
	afx_msg void OnEditMovetolayer2();
	afx_msg void OnEditMovetolayer3();
	afx_msg void OnEditMovetolayer4();
	afx_msg void OnEditMovetolayer5();
	afx_msg void OnEditMovetolayer6();
	afx_msg void OnEditMovetolayer7();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSheetFree();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnUpdateMenuCrossCursor(CCmdUI *pCmdUI);
	afx_msg void OnMenuCrossCursor();
	afx_msg void OnFileExpoEmf();
	afx_msg void OnSetColor();
	afx_msg void OnEditFind();
	//afx_msg void OnEditFindnext();
	//afx_msg void OnUpdateEditFindnext(CCmdUI *pCmdUI);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnExtMenu_0();
	afx_msg void OnExtMenu_1();
	afx_msg void OnExtMenu_2();
	afx_msg void OnExtMenu_3();
	afx_msg void OnExtMenu_4();
	afx_msg void OnExtMenu_5();
	afx_msg void OnExtMenu_6();
	afx_msg void OnExtMenu_7();
	afx_msg void OnExtMenu_8();
	afx_msg void OnExtMenu_9();

	afx_msg void OnEditEmfCopy();
	afx_msg void OnUpdateEditEmfCopy(CCmdUI *pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	afx_msg void OnSetCoarseStepZoom();
//	afx_msg void OnUpdateSetCoarseStepZoom(CCmdUI *pCmdUI);
	afx_msg void OnDispNcmark();
	afx_msg void OnUpdateDispNcmark(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDisplayLayer0(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDisplayLayer1(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDisplayLayer2(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDisplayLayer3(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDisplayLayer4(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDisplayLayer5(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDisplayLayer6(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDisplayLayer7(CCmdUI *pCmdUI);
	afx_msg void OnClickDisplayLayer0();
	afx_msg void OnClickDisplayLayer1();
	afx_msg void OnClickDisplayLayer2();
	afx_msg void OnClickDisplayLayer3();
	afx_msg void OnClickDisplayLayer4();
	afx_msg void OnClickDisplayLayer5();
	afx_msg void OnClickDisplayLayer6();
	afx_msg void OnClickDisplayLayer7();
	afx_msg void OnUpdateEditLayer0(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditLayer1(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditLayer2(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditLayer3(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditLayer4(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditLayer5(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditLayer6(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditLayer7(CCmdUI *pCmdUI);
	afx_msg void OnClickEditLayer0();
	afx_msg void OnClickEditLayer1();
	afx_msg void OnClickEditLayer2();
	afx_msg void OnClickEditLayer3();
	afx_msg void OnClickEditLayer4();
	afx_msg void OnClickEditLayer5();
	afx_msg void OnClickEditLayer6();
	afx_msg void OnClickEditLayer7();
	afx_msg void OnUpdateSheetFree(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSheet640(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSheet1000(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSheet1600(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSheet2000(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSheet3000(CCmdUI *pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);
	afx_msg void OnToolMarker();
	afx_msg void OnUpdateToolMarker(CCmdUI *pCmdUI);
	afx_msg void OnDispHiddenpinnum();
	afx_msg void OnUpdateDispHiddenpinnum(CCmdUI *pCmdUI);
	afx_msg void OnSetupQuick();
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnSetupAllowappendselect();
	afx_msg void OnUpdateSetupAllowappendselect(CCmdUI *pCmdUI);
	afx_msg void OnConvertC2l();
	afx_msg void OnUpdateConvertC2l(CCmdUI *pCmdUI);
	afx_msg void OnConvertC2t();
	afx_msg void OnUpdateConvertC2t(CCmdUI *pCmdUI);
	afx_msg void OnConvertL2t();
	afx_msg void OnUpdateConvertL2t(CCmdUI *pCmdUI);
	afx_msg void OnConvertL2c();
	afx_msg void OnUpdateConvertL2c(CCmdUI *pCmdUI);
	afx_msg void OnConvertT2l();
	afx_msg void OnUpdateConvertT2l(CCmdUI *pCmdUI);
	afx_msg void OnConvertT2c();
	afx_msg void OnUpdateConvertT2c(CCmdUI *pCmdUI);
	afx_msg void OnBulkattributeTagframe();
	afx_msg void OnUpdateBulkattributeTagframe(CCmdUI *pCmdUI);
	afx_msg void OnBulkattributeMarkerline();
	afx_msg void OnUpdateBulkattributeMarkerline(CCmdUI *pCmdUI);
	afx_msg void OnBulkattributeCommentfont();
	afx_msg void OnUpdateBulkattributeCommentfont(CCmdUI *pCmdUI);
	afx_msg void OnBulkattributeReplacecomponentinfo();
	afx_msg void OnUpdateBulkattributeReplacecomponentinfo(CCmdUI *pCmdUI);
	afx_msg void OnInsertBitmap();
	afx_msg void OnUpdateBulkattributeHidenumbers(CCmdUI *pCmdUI);
	afx_msg void OnBulkattributeHidenumbers();
	afx_msg void OnToolUseobjectfilter();
	afx_msg void OnUpdateToolUseobjectfilter(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditPoints(CCmdUI *pCmdUI);
	afx_msg void OnEditPoints();
	afx_msg void OnSetupPlacelinebyclick();
	afx_msg void OnUpdateSetupPlacelinebyclick(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOrderBringtofront(CCmdUI *pCmdUI);
	afx_msg void OnOrderBringtofront();
	afx_msg void OnOrderBringtoback();
	afx_msg void OnUpdateReloadReload(CCmdUI *pCmdUI);
	afx_msg void OnReloadReload();
	afx_msg void OnUpdateEditFind(CCmdUI *pCmdUI);

	afx_msg LRESULT OnScrollToMsg(UINT wParam,LONG lParam);
	afx_msg void OnBulkattributeDecorationline();
	afx_msg void OnUpdateBulkattributeDecorationline(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFloatEditWithLcov(CCmdUI *pCmdUI);
	afx_msg void OnFloatEditWithLcov();
	afx_msg void OnSetupDispblock();
	afx_msg void OnToolRunLcov();
	afx_msg void OnToolRunPl3w();
	afx_msg void OnToolRunNut3w();
	afx_msg void OnToolRunNl3w();
	afx_msg void OnToolRunCe3search();
	afx_msg void OnFileExpoClipbord();
};

#ifndef _DEBUG  // BSchView.cpp ファイルがデバッグ環境の時使用されます。
inline CBSchDoc* CBSchView::GetDocument()
   { return (CBSchDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

#endif
