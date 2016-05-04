// SetQuickProp.cpp : 実装ファイル
//

#include "stdafx.h"
#include "BSch.h"
#include "SetQuickProp.h"


// CSetQuickProp ダイアログ

IMPLEMENT_DYNAMIC(CSetQuickProp, CDialog)

CSetQuickProp::CSetQuickProp(CWnd* pParent /*=NULL*/)
	: CDialog(CSetQuickProp::IDD, pParent)
	, m_checkMFRPN(FALSE)
	, m_checkNote(FALSE)
	, m_checkPackage(FALSE)
	, m_checkMFR(FALSE)
	, m_checkNameinlib(FALSE)
	, m_checkUseQuickProp(FALSE)
	, m_checkVal(FALSE)
	, m_checkNum(FALSE)
	, m_checkOnMouseHover(FALSE)
{

}

CSetQuickProp::~CSetQuickProp()
{
}

void CSetQuickProp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHECK_PKG, m_checkPackage);
	DDX_Check(pDX, IDC_CHECK_MFR, m_checkMFR);
	DDX_Check(pDX, IDC_CHECK_MFRPN, m_checkMFRPN);
	DDX_Check(pDX, IDC_CHECK_NOTE, m_checkNote);

	DDX_Check(pDX, IDC_CHECK_NAMEINLIB, m_checkNameinlib);
	DDX_Check(pDX, IDC_CHECK_USE_QUICKPROP, m_checkUseQuickProp);
	DDX_Check(pDX, IDC_CHECK_VAL, m_checkVal);
	DDX_Check(pDX, IDC_CHECK_NUM, m_checkNum);
	DDX_Check(pDX, IDC_CHECK_ON_MOUSEHOVER, m_checkOnMouseHover);
}


BEGIN_MESSAGE_MAP(CSetQuickProp, CDialog)
END_MESSAGE_MAP()


// CSetQuickProp メッセージ ハンドラ
