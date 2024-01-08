/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// LblAtrb.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CLblAtrb ダイアログ

#ifndef LBLATRB_H
#define LBLATRB_H


class CLblAtrb : public CDialog
{
// コンストラクション
public:
	CLblAtrb(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CLblAtrb)
	enum { IDD = IDD_ATRIB_LABEL };
	CString	m_rText;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CLblAtrb)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CLblAtrb)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
