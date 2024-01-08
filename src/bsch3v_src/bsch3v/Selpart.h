/****************************************************************************
     BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// SelPart.h : ヘッダー ファイル
//
// 部品を選択するためのダイアログボックスのクラス
//   このダイアログは部品名を返す。プログラム（BSch）は、その
// 部品名から、部品インデックスへのポインタを得なければなら
// ない。

/////////////////////////////////////////////////////////////////////////////
// CSelPart ダイアログ

class CSelPart : public CDialog
{
	int m_nPartCount;
	int m_nLibraryCount;
	CRect m_rcDialog;
	CRect m_rcClient;

	CPreview* m_pwndPreview;

	BOOL m_bInhibitUpdatePartName;
	BOOL m_bInhibitUpdateListbox;

// コンストラクション
public:
	CString m_rStrPartName;	//ダイアログ終了後、部品名が入る
	int m_nLastSelectPart;
	int m_nLastSelectLibrary;
	const SCompIndex* m_pCompIndex;
	
	BOOL m_bUsePreview;

	CSelPart(CWnd* pParent = NULL);   // 標準のコンストラクタ

	~CSelPart()
	{
		delete m_pwndPreview;
	}

// ダイアログ データ
	//{{AFX_DATA(CSelPart)
	enum { IDD = IDD_SELECT_PARTS };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CSelPart)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSelPart)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListLibrary();
	afx_msg void OnSelchangeListPartname();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnDblclkListPartname();
	afx_msg void OnChangeEditPartname();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
	void SetPartName(int nLib);	//指定したライブラリの部品をリストボックスに設定する
	void SetLibraryName();		//ライブラリの名前をリストボックスに設定する
	void SetEditPartName(LPCTSTR str);//エディットコントロールにパーツ名を設定
	void AutoUpdate();
};
