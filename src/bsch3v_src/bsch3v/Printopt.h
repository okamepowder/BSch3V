/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#ifndef _PRINTOPT_H
#define _PRINTOPT_H
#include "afxwin.h"

// PrintOpt.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CPrintOpt ダイアログ

class CPrintOpt : public CDialog
{
// コンストラクション
public:
	CPrintOpt(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CPrintOpt)
	enum { IDD = IDD_SET_PRINT_OPTION };
	BOOL	m_bFrame;
	BOOL	m_bPartNum;
	BOOL	m_bPartName;
	BOOL	m_bPrintBmp;
	BOOL	m_bFooter;
	CString m_strFooter;
	//}}AFX_DATA

	int m_nPrintRes;
	int m_nImageMag;		//2016/05/03 イメージファイルの拡大指定追加
	int m_nLeftMargin;
	int m_nRightMargin;
	int m_nUpperMargin;
	int m_nLowerMargin;

	//int m_nDefaultRes;

	BOOL m_bPageFit;



// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CPrintOpt)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CPrintOpt)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bPrintColor;
//	afx_msg void OnBnClickedCheckPrintColor();
	afx_msg void OnBnClickedCheckPrintbmp();
	afx_msg void OnBnClickedCheckPagefit();
	afx_msg void OnBnClickedCheckFooter();
};

#endif

