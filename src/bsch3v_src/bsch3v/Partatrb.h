/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// PartAtrb.h : ヘッダー ファイル
//
#ifndef PARTATRB_H
#define PARTATRB_H

#include "complib.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CPartAtrb ダイアログ

class CPartAtrb : public CDialog
{
// コンストラクション
public:
	CPartAtrb(CWnd* pParent = NULL);   // 標準のコンストラクタ
// ダイアログ データ
	//{{AFX_DATA(CPartAtrb)
	enum { IDD = IDD_ATRIB_PART };
	CString	m_rPartName;
	CString	m_rPartNum;
	CString m_rNote;
	//}}AFX_DATA
	int		m_nBlock;
	int		m_nBlockMin;       //コンストラクタで１に初期化
	int		m_nBlockMax;
	CString m_rOrgName;
	const SCompIndex* m_pPartIndex;
	int m_nLastSelectPart;
	int m_nLastSelectLibrary;
	BOOL m_modifiedCompInfo;	   //コンストラクタでFALSEに初期化

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CPartAtrb)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CPartAtrb)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChangeEditPartblock();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//IDC_PIN_NUM スタティックコントロールにピン番号をセット
	void SetBlockPins(int nBlock);

	TCHAR m_szLcoPath[_MAX_PATH];
	BOOL m_enableEditComp;
	//SCompIndex* m_pTempPartIndex;
	SCompLib m_tempLib;		//LCoVで編集するときに使う。このダイアログクラスが消滅するまで実体を失わないようにクラス変数として宣言するよ。

	UINT m_timerID;

public:
	afx_msg void OnBnClickedReplaceCompindex();
	afx_msg void OnBnClickedEditCompindex();

	BOOL m_bNameVertical;
	BOOL m_bNumVertical;
	
	CString m_mfr;
	CString m_mfrpnum;
	CString m_package;

	int m_nFocusedTo;

	

	BOOL m_bHideNum;
	BOOL m_bHideVal;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#endif