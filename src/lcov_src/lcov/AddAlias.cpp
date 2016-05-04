/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/


// AddAlias.cpp : 実装ファイル
//

#include "stdafx.h"
#include "LCoV.h"
#include "AddAlias.h"


// CAddAlias ダイアログ

IMPLEMENT_DYNAMIC(CAddAlias, CDialog)
CAddAlias::CAddAlias(CWnd* pParent /*=NULL*/)
	: CDialog(CAddAlias::IDD, pParent)
	, m_strName(_T(""))
	, m_strAlias(_T(""))
{
}

CAddAlias::~CAddAlias()
{
}

void CAddAlias::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_ALIAS, m_strAlias);
}


BEGIN_MESSAGE_MAP(CAddAlias, CDialog)
END_MESSAGE_MAP()


// CAddAlias メッセージ ハンドラ
