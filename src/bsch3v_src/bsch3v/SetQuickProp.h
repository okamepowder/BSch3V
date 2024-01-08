#pragma once
//#include "afxwin.h"


// CSetQuickProp ダイアログ

class CSetQuickProp : public CDialog
{
	DECLARE_DYNAMIC(CSetQuickProp)

public:
	CSetQuickProp(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CSetQuickProp();

// ダイアログ データ
	enum { IDD = IDD_SETQUICKPROP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	
	BOOL m_checkPackage;
	BOOL m_checkMFR;
	BOOL m_checkMFRPN;
	BOOL m_checkNote;

	BOOL m_checkNameinlib;
	BOOL m_checkUseQuickProp;
	BOOL m_checkVal;
	BOOL m_checkNum;
	BOOL m_checkOnMouseHover;
};
