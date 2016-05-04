/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

//

#include "stdafx.h"
#include "BSch.h"
#include "TagAtrb.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTagAtrb ダイアログ


CTagAtrb::CTagAtrb(CWnd* pParent /*=NULL*/)
	: CDialog(CTagAtrb::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTagAtrb)
	m_rName = _T("");
	m_nType = -1;
	//}}AFX_DATA_INIT
}


void CTagAtrb::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTagAtrb)
	DDX_Text(pDX, IDC_EDIT_TAGNAME, m_rName);
	DDX_Radio(pDX, IDC_TAG_SQ, m_nType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTagAtrb, CDialog)
	//{{AFX_MSG_MAP(CTagAtrb)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTagAtrb メッセージ ハンドラ
