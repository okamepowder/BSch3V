/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/
// CmntAtrb.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "BSch.h"
#include "CmntAtrb.h"
#include ".\cmntatrb.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCmntAtrb ダイアログ


CCmntAtrb::CCmntAtrb(CWnd* pParent /*=NULL*/)
	: CDialog(CCmntAtrb::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmntAtrb)
	//}}AFX_DATA_INIT
	m_rText = _T("");
	m_enableWidthSetting = false;
	m_width = 10000;
	m_enableTag = false;
	m_sizeMin = CSize(100,100);
	initFlag = false;
}


void CCmntAtrb::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmntAtrb)
	//DDX_Text(pDX, IDC_EDIT_COMMENT, m_rText);
	DDX_Control(pDX, IDC_EDIT_COMMENT, m_editml);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCmntAtrb, CDialog)
	//{{AFX_MSG_MAP(CCmntAtrb)
	ON_BN_CLICKED(IDC_BUTTON_FONT, OnButtonFont)
	//}}AFX_MSG_MAP
//	ON_EN_CHANGE(IDC_EDIT_COMMENT, OnEnChangeEditComment)
ON_BN_CLICKED(IDC_CHECK_ENABLE_WIDTH, OnBnClickedCheckEnableWidth)
ON_WM_GETMINMAXINFO()
ON_WM_SIZE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmntAtrb メッセージ ハンドラ

void CCmntAtrb::OnButtonFont() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	LOGFONT logfont = m_logfont;
	//logfont.lfHeight = -logfont.lfHeight;
	//CFontDialog dlg(&logfont,CF_SCREENFONTS);
	CFontDialog dlg(&logfont,CF_TTONLY | CF_SCREENFONTS);
	
	if(dlg.DoModal()==IDOK){
		m_logfont = logfont;
	//	m_logfont.lfHeight = dlg.GetSize()/10;
	}
}


void CCmntAtrb::OnBnClickedCheckEnableWidth()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	m_enableWidthSetting = ((CButton*)GetDlgItem(IDC_CHECK_ENABLE_WIDTH))->GetCheck();
	GetDlgItem(IDC_EDIT_WIDTH)->EnableWindow(m_enableWidthSetting);
	if(!m_enableWidthSetting){
		m_width = GetDlgItemInt(IDC_EDIT_WIDTH);
		SetDlgItemText(IDC_EDIT_WIDTH,_T(""));
	}else{
		SetDlgItemInt(IDC_EDIT_WIDTH,m_width);
	}
}

BOOL CCmntAtrb::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO :  ここに初期化を追加してください
	((CEdit*)GetDlgItem(IDC_EDIT_COMMENT))->SetLimitText(8000);
	SetDlgItemText(IDC_EDIT_COMMENT,m_rText);

	((CButton*)GetDlgItem(IDC_CHECK_ENABLE_WIDTH))->SetCheck(m_enableWidthSetting);
	GetDlgItem(IDC_EDIT_WIDTH)->EnableWindow(m_enableWidthSetting);
	((CButton*)GetDlgItem(IDC_CHECK_ENABLE_TAG))->SetCheck(m_enableTag);
	if(m_width<50)m_width = 50;
	if(!m_enableWidthSetting){
		SetDlgItemText(IDC_EDIT_WIDTH,_T(""));
	}else{
		SetDlgItemInt(IDC_EDIT_WIDTH,m_width);
	}

	CRect rc;
	GetWindowRect(&rc);
	m_sizeMin = rc.Size();
	GetClientRect(&rc);
	m_sizePrev = rc.Size();
	


	TRACE("CCmntAtrb::OnInitDialog()  cx:%d  cy:%d\n",m_sizePrev.cx,m_sizePrev.cy);

	
	initFlag = true;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CCmntAtrb::OnOK()
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	GetDlgItemText(IDC_EDIT_COMMENT,m_rText);
	m_rText.TrimRight();
	m_enableWidthSetting = ((CButton*)GetDlgItem(IDC_CHECK_ENABLE_WIDTH))->GetCheck();
	m_enableTag = ((CButton*)GetDlgItem(IDC_CHECK_ENABLE_TAG))->GetCheck();
	m_width = GetDlgItemInt(IDC_EDIT_WIDTH);
	CDialog::OnOK();
}


void CCmntAtrb::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	lpMMI->ptMinTrackSize = CPoint(m_sizeMin);
	CDialog::OnGetMinMaxInfo(lpMMI);
}



void CCmntAtrb::OffsetCtrl(int dx, int dy,int id)
{
	CWnd* pwnd;
	CRect rc;

	pwnd = GetDlgItem(id);
	pwnd->GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.OffsetRect(dx,dy);
	pwnd->MoveWindow(&rc,0);
}

void CCmntAtrb::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	TRACE("CCmntAtrb::OnSize(UINT nType, int cx, int cy)  cx:%d  cy:%d\n",cx,cy);
	if(initFlag){
		// TODO : ここにメッセージ ハンドラ コードを追加します。
		if(cx != m_sizePrev.cx || cy != m_sizePrev.cy){
			int dx = cx - m_sizePrev.cx;
			int dy = cy - m_sizePrev.cy;
			
			OffsetCtrl(dx, dy,IDC_CHECK_ENABLE_WIDTH);
			OffsetCtrl(dx, dy,IDC_CHECK_ENABLE_TAG);
			OffsetCtrl(dx, dy,IDC_STATIC_WIDTH_TITLE);
			OffsetCtrl(dx, dy,IDC_EDIT_WIDTH);
			OffsetCtrl(dx, dy,IDC_BUTTON_FONT);
			OffsetCtrl(dx, dy,IDOK);
			OffsetCtrl(dx, dy,IDCANCEL);

			CWnd* pwnd;
			CRect rc;
			pwnd = GetDlgItem(IDC_EDIT_COMMENT);
			pwnd->GetWindowRect(&rc);
			ScreenToClient(&rc);
			rc.right = rc.right+dx;
			rc.bottom = rc.bottom+dy;
			pwnd->MoveWindow(&rc,0);

			m_sizePrev = CSize(cx,cy);
			Invalidate();
			UpdateWindow();

		}
	}

}

void CCmntAtrb::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT_COMMENT);
	if(pEdit->GetModify()){
		if(IDNO==AfxMessageBox(IDS_DISCARD_CHANGES_COMMENT,MB_YESNO|MB_ICONEXCLAMATION|MB_DEFBUTTON2)){
			return;
		}
	}
	CDialog::OnCancel();
}
