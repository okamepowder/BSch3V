/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// LblAtrb.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "BSch.h"
#include "LblAtrb.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLblAtrb ダイアログ


CLblAtrb::CLblAtrb(CWnd* pParent /*=NULL*/)
	: CDialog(CLblAtrb::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLblAtrb)
	m_rText = _T("");
	//}}AFX_DATA_INIT
}


void CLblAtrb::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLblAtrb)
	DDX_Text(pDX, IDC_EDIT_LABELNAME, m_rText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLblAtrb, CDialog)
	//{{AFX_MSG_MAP(CLblAtrb)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLblAtrb メッセージ ハンドラ
