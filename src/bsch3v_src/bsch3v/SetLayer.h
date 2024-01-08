/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// SetLayer.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CSetLayer ダイアログ

class CSetLayer : public CDialog
{
// コンストラクション
public:
	CSetLayer(CWnd* pParent = NULL);   // 標準のコンストラクタ

	WORD	m_wShow;
	int		m_nEdit;
	BOOL	m_fEditHigh;
	BOOL	m_fDisplayNameOnDlgBar;
	CString m_strName[8];
// ダイアログ データ
	//{{AFX_DATA(CSetLayer)
	enum { IDD = IDD_SETLAYER };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CSetLayer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSetLayer)
	virtual BOOL OnInitDialog();
	afx_msg void OnD0();
	afx_msg void OnD1();
	afx_msg void OnD2();
	afx_msg void OnD3();
	afx_msg void OnD4();
	afx_msg void OnD5();
	afx_msg void OnD6();
	afx_msg void OnD7();
	afx_msg void OnE0();
	afx_msg void OnE1();
	afx_msg void OnE2();
	afx_msg void OnE3();
	afx_msg void OnE4();
	afx_msg void OnE5();
	afx_msg void OnE6();
	afx_msg void OnE7();
	virtual void OnOK();
	afx_msg void OnCheckEditHighlight();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	WORD GetBit(int nLayer);
	void OnDCheck(int n);
	void OnECheck(int n);
public:
	afx_msg void OnClickedCheckDiplayNameOnBar();
};
