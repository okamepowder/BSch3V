/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

// PropLib.cpp : 実装ファイル
//

#include "stdafx.h"
#include "LCoV.h"
#include "PropLib.h"


// CPropLib ダイアログ

IMPLEMENT_DYNAMIC(CPropLib, CDialog)
CPropLib::CPropLib(CWnd* pParent /*=NULL*/)
	: CDialog(CPropLib::IDD, pParent)
	, m_propString(_T(""))
{
}

CPropLib::~CPropLib()
{
}

void CPropLib::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_STRING, m_propString);
	DDV_MaxChars(pDX, m_propString, 255);
}


BEGIN_MESSAGE_MAP(CPropLib, CDialog)
END_MESSAGE_MAP()


// CPropLib メッセージ ハンドラ
