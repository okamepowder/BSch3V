/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// CmntAtrb.h : ヘッダー ファイル
//

#ifndef CMNTATRB_070318_H
#define CMNTATRB_070318_H

#include "EditML.h"

/////////////////////////////////////////////////////////////////////////////
// CCmntAtrb ダイアログ

class CCmntAtrb : public CDialog
{
// コンストラクション
public:
	CCmntAtrb(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CCmntAtrb)
	enum { IDD = IDD_ATRIB_COMMENT };
	CEditML m_editml;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CCmntAtrb)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

public:
	CString	m_rText;
	LOGFONT m_logfont;
	bool m_enableWidthSetting;
	int m_width;
	bool m_enableTag;

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CCmntAtrb)
	afx_msg void OnButtonFont();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnEnChangeEditComment();
	afx_msg void OnBnClickedCheckEnableWidth();
	virtual BOOL OnInitDialog();
protected:

	CSize m_sizeMin;
	CSize m_sizePrev;

	bool initFlag;

	void OffsetCtrl(int dx, int dy, int id);

	virtual void OnOK();
public:
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	virtual void OnCancel();
};

#endif
