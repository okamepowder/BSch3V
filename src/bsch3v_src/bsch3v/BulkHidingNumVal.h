#pragma once


// CBulkHidingNumVal ダイアログ

class CBulkHidingNumVal : public CDialog
{
	DECLARE_DYNAMIC(CBulkHidingNumVal)

public:
	CBulkHidingNumVal(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CBulkHidingNumVal();

	int m_bHideNumCheckState;
	int m_bHideValCheckState;

// ダイアログ データ
	enum { IDD = IDD_SET_HIDING_NUM_VAL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnBnClickedCheckHidenum();
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
