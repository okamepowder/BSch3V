/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// BSch.h : BSCH アプリケーションのメイン ヘッダー ファイル
//
#ifndef BSCH_H
#define BSCH_H

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

//#include "resource.h"       // メイン シンボル

#include "SheetSizeInfo.h"

class SCompInfo;
class SCompLib;

/////////////////////////////////////////////////////////////////////////////
// BSch のためのグローバルな数値定義

//#define MAX_LIB 64	// ライブラリの最大数

#define MAX_SHEETSIZE 5 //シートサイズは0～(MAX_SHEETSIZE-1)


//部品単独選択のときのキー入力フォーカス
enum{
	FOCUSED_TO_BODY = 0,
	FOCUSED_TO_NUM = 1,
	FOCUSED_TO_VAL = 2
};

enum DISP_BLOCKNUM_TYPE{
	NOTDISPLAY_BLOCKNUM	=0,
	DISPLAY_BLOCKNUM_NUM,
	DISPLAY_BLOCKNUM_ALPHA
};



///////////////////////////////////////////////////////////////
// 色
#define COL_BG				(RGB(255,255,255))
#define COL_JUNC			(RGB(  0,128,  0))
#define COL_WIRE			(RGB(  0,128,  0))
#define COL_BUS				(RGB(  0,128,192))
#define COL_DASH			(RGB(  0,  0,192))
#define COL_TAG				(RGB(192,  0,  0))
#define COL_TAG_NAME		(RGB(  0,  0,  0))
#define COL_COMMENT			(RGB(  0,  0,  0))
#define COL_LABEL			(RGB(  0,  0,  0))
#define COL_XOR				(RGB(255,  0,  0))
#define COL_PART			(RGB(192,  0,  0))
#define COL_PART_PIN		(RGB(192,  0,  0))
#define COL_PART_PINNAME	(RGB(  0,  0,  0))	
#define COL_PART_PINNUM		(RGB(  0,  0,  0))
#define COL_PART_NAME		(RGB(  0,  0,  0))	
#define COL_PART_NUM		(RGB(  0,  0,  0))
#define COL_PART_FRAME		(RGB(128,128,255))
#define COL_SHEET_FRAME		(RGB(128, 32, 32))
#define COL_SHEET_GRID		(RGB(255, 64, 64))
#define COL_DARK			(RGB(255,192,192))
#define COL_ZLPIN			(RGB(255,192,192))
#define COL_NC_MARK			(RGB(255, 32, 32))
#define COL_QUICKPROPVIEW	(RGB(240,230,200))
#define COL_TEMPFRAME_FOCUSED		(RGB(128,128,128))
#define COL_TEMPFRAME_NOT_FOCUSED	(RGB(160,220,220))
#define COL_FIND_MARK	(RGB(64,64,255))


//描画モード （以下の値の論理和をとって使う）
#define DRAW_ON		1	// 通常の描画
#define DRAW_OFF	2	// 消去。おそらくは背景色での描画を指す
#define DRAW_TEMP	4	// XORモードでの描画
#define DRAW_FAST	8	// 高速モードでの描画。おそらくは次のようなもの）
						//		・部品ならピン番号、ピン名、部品番号、部品名を省略
						//		・タグなら文字列を省略
						//	 その他の多くの図面要素ではこのビットは意味を持たない
#define DRAW_INVERT	16	// BSchの8ドット文字列のXORだけで有効。部品番号、部品名の描画で使用
#define DRAW_PARTFRAME 		32	//部品枠の描画
#define DRAW_DARK	64	// 暗い色での描画
#define DRAW_NC_MARK 128
#define DRAW_INH_PARTNAME 	256	//部品名の描画をしない
#define DRAW_INH_PARTNUM  	512	//部品番号の描画をしない
#define DRAW_INH_PARTBODY  1024	//部品ボディの描画をしない
#define DRAW_FOR_PRINT	   2048 //印刷用の描画
#define DRAW_FOR_PRINTER	4096 //プリンター印刷用の描画


//#define COMMENT_Y_OFFSET 2


#define SCROLL_TO_MSG (WM_APP+100)



/////////////////////////////////////////////////////////////////////////////
// CBSchApp:
// このクラスの動作の定義に関しては BSch.cpp ファイルを参照してください。
//

class CBSchApp : public CWinApp
{
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CBSchApp();

	~CBSchApp();

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CBSchApp)
	public:
	virtual BOOL InitInstance();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CBSchApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnFilePrintSetup();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	TCHAR m_szIniFileDir[_MAX_PATH];
private:
	TCHAR *m_pszIniFileName;
	TCHAR m_moduleFileName[_MAX_PATH];
	TCHAR m_LCoVPath[_MAX_PATH];

public:
	void UpdatePrinterSelection(BOOL bForceDefaults);
	void IniWritePrinterSetup();
	BOOL ExecBSch(const TCHAR* pathname);
	LPCTSTR LCoVPath(){return m_LCoVPath;}
	virtual BOOL OnIdle(LONG lCount);

public:
	static void  editWidthLCoV(LPCTSTR name, SCompInfo* pInfo, SCompLib& tempLib);
};


/////////////////////////////////////////////////////////////////////////////

#endif
