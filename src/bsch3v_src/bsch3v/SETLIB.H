/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/
#pragma once
// SetLib.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CSetLib ダイアログ


class CSetLib : public CDialog
{
protected:
	int m_nList;	// リストボックスへの登録数

// コンストラクション
public:
	CSetLib(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSetLib)
	enum { IDD = IDD_SETLIB_DLG };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CSetLib)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSetLib)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeLibList();
	afx_msg void OnLibUp();
	afx_msg void OnLibDown();
	afx_msg void OnLibAdd();
	afx_msg void OnLibDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	afx_msg void OnClickedLibAbs();
	afx_msg void OnClickedLibRel();

	void EnableButton(UINT id,BOOL bEnable);//ダイアログ構成ボタンのイネーブル
	void SetListboxWidth();

	void EnableButtons(int sel);



public:
};
