/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

#pragma once


// PinProperty ダイアログ

//class SPin;

class CPinProperty : public CDialog
{
	DECLARE_DYNAMIC(CPinProperty)

public:
	CPinProperty(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CPinProperty();

	//SPin* m_pPin;
	//bool m_bChanged;

	CString m_name;
	CString m_name_old;	//show/hideをイニシャライズするかどうかを判定するために使う。
	CString* m_arrayNum;
	unsigned long m_pinNameDispFlag;
	int m_block;
	bool m_hideNum;
	int m_pinType;


protected:
	int m_currentBlock;

// ダイアログ データ
	enum { IDD = IDD_DIALOG_PIN_PROP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEditBlock();
public:
	void setBlock(int block);
	void setName(const TCHAR* name);
protected:
	virtual void OnOK();
public:
	afx_msg void OnBnClickedButtonShowhideName();
	afx_msg void OnEnChangeEditPinName();
};
