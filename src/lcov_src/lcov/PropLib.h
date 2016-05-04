/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

#pragma once


// CPropLib ダイアログ

class CPropLib : public CDialog
{
	DECLARE_DYNAMIC(CPropLib)

public:
	CPropLib(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CPropLib();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_PROP_LIB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CString m_propString;
};
