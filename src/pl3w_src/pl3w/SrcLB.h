/****************************************************************************
    PL3W partslist utility for BSch3V.
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/


#if !defined(AFX_SRCLB_H__CE0BE66B_125E_4EC2_90E4_A056F69D5BF8__INCLUDED_)
#define AFX_SRCLB_H__CE0BE66B_125E_4EC2_90E4_A056F69D5BF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SrcLB.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CSrcLB ウィンドウ

class CSrcLB : public CListBox
{
// コンストラクション
public:
	CSrcLB();

// アトリビュート
public:

// オペレーション
public:
	void SetListboxWidth();
// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSrcLB)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CSrcLB();

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CSrcLB)
	afx_msg void OnDropFiles(HDROP hDropInfo);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SRCLB_H__CE0BE66B_125E_4EC2_90E4_A056F69D5BF8__INCLUDED_)
