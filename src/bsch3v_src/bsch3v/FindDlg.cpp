/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// FindDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "BSch.h"
#include "FindDlg.h"
#include ".\finddlg.h"


// CFindDlg ダイアログ

IMPLEMENT_DYNAMIC(CFindDlg, CDialog)
CFindDlg::CFindDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindDlg::IDD, pParent)
	, m_bFindComment(FALSE)
	, m_bFindLabel(FALSE)
	, m_bFindName(FALSE)
	, m_bFindNameLib(FALSE)
	, m_bFindManufacture(FALSE)
	, m_bFindManufacturePartNumber(FALSE)
	, m_bFindPackage(FALSE)
	, m_bFindNote(FALSE)
	, m_bFindNum(FALSE)
	, m_bFindTag(FALSE)
	, m_bMatchWhole(FALSE)
	, m_bCaseSensitive(FALSE)
	, m_strFind(_T(""))
{
}

CFindDlg::~CFindDlg()
{
}

void CFindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_FIND_COMMENT, m_bFindComment);
	DDX_Check(pDX, IDC_FIND_LABEL, m_bFindLabel);
	DDX_Check(pDX, IDC_FIND_NAME, m_bFindName);
	DDX_Check(pDX, IDC_FIND_NAME_LIB, m_bFindNameLib);
	DDX_Check(pDX, IDC_FIND_MANUFACTURE, m_bFindManufacture);
	DDX_Check(pDX, IDC_FIND_MANUFACTUREPN, m_bFindManufacturePartNumber);
	DDX_Check(pDX, IDC_FIND_PACKAGE, m_bFindPackage);
	DDX_Check(pDX, IDC_FIND_NOTE, m_bFindNote);
	DDX_Check(pDX, IDC_FIND_NUM, m_bFindNum);
	DDX_Check(pDX, IDC_FIND_TAG, m_bFindTag);
	DDX_Check(pDX, IDC_FIND_MATCHWHOLE, m_bMatchWhole);
	DDX_Check(pDX, IDC_FIND_CASE_SENSITIVE, m_bCaseSensitive);
	DDX_Text(pDX, IDC_FINDSTRING, m_strFind);
}


BEGIN_MESSAGE_MAP(CFindDlg, CDialog)

END_MESSAGE_MAP()


// CFindDlg メッセージ ハンドラ


