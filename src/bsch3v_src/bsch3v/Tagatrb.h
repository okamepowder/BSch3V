/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// TagAtrb.h : ヘッダー ファイル
//

#ifndef TAGATRB_H
#define TAGATRB_H

/////////////////////////////////////////////////////////////////////////////
// CTagAtrb ダイアログ

class CTagAtrb : public CDialog
{
// コンストラクション
public:
	CTagAtrb(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CTagAtrb)
	enum { IDD = IDD_ATRIB_TAG };
	CString	m_rName;
	int		m_nType;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CTagAtrb)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CTagAtrb)
		// メモ: ClassWizard はこの位置にメンバ関数を追加します。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif