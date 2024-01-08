/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// MainFrm.h : CMainFrame クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#ifndef MAINFRM_H
#define MAINFRM_H

#include "extensionmenu.h"



class CMainFrame : public CFrameWnd
{
protected: // シリアライズ機能のみから作成します。
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)



// アトリビュート
public:
	

// オペレーション
public:
	void SetStatusBarXY(const POINT* ppoint);	//ppointの値をステータスバーに表示
	void SetMessageLine(CString& rStr); //ステータスバーのペイン0にメッセージを出力する

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // コントロール バー用メンバ
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
public:
	CDialogBar  m_dbarLayer;

protected:
	void addExtensionMenu();
	int CreateDBarLayer(UINT idd);

public:
	int m_extensionMenuCount;				//2005/10/15
	CExtensionMenu m_aryExtensionMenu[10];	//2005/10/15

	CWnd* CheckOpened(const TCHAR* pcszPathName);

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateViewStatusBar(CCmdUI* pCmdUI);
	afx_msg void OnViewStatusBar();
	afx_msg void OnViewToolbar();
	afx_msg void OnUpdateViewToolbar(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnScrollToMsg(UINT wParam,LONG lParam);

public:
	afx_msg void OnDestroy();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
protected:
	//static BOOL CALLBACK CMainFrame::EnumWindowsProcCheckOpened(HWND hwnd, LPARAM lParam);
	static BOOL CALLBACK EnumWindowsProcCheckOpened(HWND hwnd, LPARAM lParam);
public:
	afx_msg void OnViewLayerbox();
	afx_msg void OnUpdateViewLayerbox(CCmdUI *pCmdUI);

protected:
	static CSize m_sizeInitialWindowSize;

public:
	static void SetInitialWindowSize(int cx,int cy);

public:
	static void IniWriteWindowPosition(int cmdShow,int cx,int cy);
	static void IniReadWindowPosition(int& cmdShow,int& cx,int& cy);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

/////////////////////////////////////////////////////////////////////////////

#endif
