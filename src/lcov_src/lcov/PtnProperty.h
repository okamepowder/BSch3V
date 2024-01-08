/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

#pragma once
#include "afxwin.h"


// CPtnProperty ダイアログ

class CPtnProperty : public CDialog
{
	DECLARE_DYNAMIC(CPtnProperty)

public:
	CPtnProperty(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CPtnProperty();

	virtual void OnFinalRelease();

// ダイアログ データ
	enum { IDD = IDD_PTN_PROP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	BOOL m_fill;
	BOOL m_dash;
	int m_width;
	BOOL m_enableFill;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
