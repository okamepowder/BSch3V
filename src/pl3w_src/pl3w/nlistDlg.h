/****************************************************************************
    PL3W partslist utility for BSch3V.
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/


// nlistDlg.h : ヘッダー ファイル
//

#if !defined(AFX_NLISTDLG_H__B8E59C02_9A8D_4DE7_A228_7DB454FAE122__INCLUDED_)
#define AFX_NLISTDLG_H__B8E59C02_9A8D_4DE7_A228_7DB454FAE122__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SrcLB.h"


/////////////////////////////////////////////////////////////////////////////
// CNlistDlg ダイアログ

#define FILE_NAME_BUFFER_SIZE (_MAX_PATH * 64  + 1)

class CNlistDlg : public CDialog
{
// 構築
public:
	CNlistDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ
protected:
	TCHAR m_szFileNameBuffer[FILE_NAME_BUFFER_SIZE];

	BOOL m_noMessageBox;

// ダイアログ データ
	//{{AFX_DATA(CNlistDlg)
	enum { IDD = IDD_NLIST_DIALOG };
	CSrcLB	m_listsrc;
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CNlistDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	void SetDstFileName();

	int DstFileSetting();

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CNlistDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRefsrc();
	afx_msg void OnClr();
	afx_msg void OnSelchangeListSrc();
	afx_msg void OnRefdst();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckMfr();
//	afx_msg void OnCbnSelchangeComboDstType();
	afx_msg void OnBnClickedRadioCsvout();
	afx_msg void OnBnClickedRadioSylkout();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_NLISTDLG_H__B8E59C02_9A8D_4DE7_A228_7DB454FAE122__INCLUDED_)
