/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

#pragma once


// CAddAlias ダイアログ

class CAddAlias : public CDialog
{
	DECLARE_DYNAMIC(CAddAlias)

public:
	CAddAlias(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CAddAlias();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_ALIAS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CString m_strName;
	CString m_strAlias;
};
