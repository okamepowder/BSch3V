#pragma once


// CSetAddIn ダイアログ

class CSetAddIn : public CDialog
{
	DECLARE_DYNAMIC(CSetAddIn)

public:
	CSetAddIn(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CSetAddIn();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_SETADDIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	void SetListboxWidth();

	//ダイアログ構成ボタンのイネーブル
	void EnableButton(UINT id,BOOL bEnable);

	int m_nAddinCount;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonRemove();
	afx_msg void OnBnClickedButtonDown();
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnLbnSelchangeListFiles();
protected:
	virtual void OnOK();
	virtual void OnCancel();
};
