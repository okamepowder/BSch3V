/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// PrintOpt.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "BSch.h"
#include "PrintOpt.h"
#include ".\printopt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrintOpt ダイアログ


CPrintOpt::CPrintOpt(CWnd* pParent /*=NULL*/)
	: CDialog(CPrintOpt::IDD, pParent)

{
	//{{AFX_DATA_INIT(CPrintOpt)
	m_bFrame = TRUE;
	m_bPartNum = TRUE;
	m_bPartName = FALSE;
	m_bPrintBmp = FALSE;
	m_bPrintColor = FALSE;
	m_bFooter = FALSE;
	m_strFooter = "";
	//}}AFX_DATA_INIT

	m_bPageFit = FALSE;

	m_nPrintRes = 0;
	m_nImageMag = 100;		//2016/05/03
	m_nLeftMargin	=0;
	m_nRightMargin	=0;
	m_nUpperMargin	=0;
	m_nLowerMargin	=0;

	//m_nDefaultRes = 200;
}


void CPrintOpt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrintOpt)
	DDX_Check(pDX, IDC_CHECK_FRAME, m_bFrame);
	DDX_Check(pDX, IDC_CHECK_PARTNUM, m_bPartNum);
	DDX_Check(pDX, IDC_CHECK_PARTNAME, m_bPartName);
	DDX_Check(pDX, IDC_CHECK_PRINTBMP,m_bPrintBmp);
	DDX_Check(pDX, IDC_CHECK_FOOTER,m_bFooter);
	DDX_Text(pDX, IDC_EDIT_FOOTER, m_strFooter);

	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK_PRINT_COLOR, m_bPrintColor);
}


BEGIN_MESSAGE_MAP(CPrintOpt, CDialog)
	//{{AFX_MSG_MAP(CPrintOpt)
//	ON_BN_CLICKED(IDC_CHECK_RES_AUTO, OnCheckResAuto)
	//}}AFX_MSG_MAP
//	ON_BN_CLICKED(IDC_CHECK_PRINT_COLOR, OnBnClickedCheckPrintColor)
ON_BN_CLICKED(IDC_CHECK_PRINTBMP, OnBnClickedCheckPrintbmp)
ON_BN_CLICKED(IDC_CHECK_PAGEFIT, OnBnClickedCheckPagefit)
ON_BN_CLICKED(IDC_CHECK_FOOTER, &CPrintOpt::OnBnClickedCheckFooter)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintOpt メッセージ ハンドラ

BOOL CPrintOpt::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	CButton* pCheck=(CButton*)GetDlgItem(IDC_CHECK_PAGEFIT);
	CEdit*   pEdit =(CEdit*)GetDlgItem(IDC_EDIT_RES);
	pEdit->SetLimitText(3);
	if(m_bPageFit){
		pCheck->SetCheck(1);
		pEdit->EnableWindow(FALSE);
		SetDlgItemText(IDC_EDIT_RES,_T(""));
	}else{
		pCheck->SetCheck(0);
		pEdit->EnableWindow(TRUE);
		SetDlgItemInt(IDC_EDIT_RES,m_nPrintRes,FALSE);
	}

	pEdit =(CEdit*)GetDlgItem(IDC_EDIT_IMAGE_MAG);
	pEdit->SetLimitText(3);
	SetDlgItemInt(IDC_EDIT_IMAGE_MAG,m_nImageMag,FALSE);




	SetDlgItemInt(IDC_EDIT_LEFT_M,m_nLeftMargin,FALSE);
	SetDlgItemInt(IDC_EDIT_RIGHT_M,m_nRightMargin,FALSE);
	SetDlgItemInt(IDC_EDIT_UPPER_M,m_nUpperMargin,FALSE);
	SetDlgItemInt(IDC_EDIT_LOWER_M,m_nLowerMargin,FALSE);

	pCheck=(CButton*)GetDlgItem(IDC_CHECK_PRINTBMP);
	if(pCheck->GetCheck()){
		pCheck=(CButton*)GetDlgItem(IDC_CHECK_PRINT_COLOR);
		pCheck->SetCheck(0);
		pCheck->EnableWindow(FALSE);
	}

	pEdit =(CEdit*)GetDlgItem(IDC_EDIT_FOOTER);
	pCheck=(CButton*)GetDlgItem(IDC_CHECK_FOOTER);
	pEdit->EnableWindow(pCheck->GetCheck());
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

//void CPrintOpt::OnCheckResAuto() 
//{
//	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
//	CButton* pCheck=(CButton*)GetDlgItem(IDC_CHECK_RES_AUTO);
//	CEdit*   pEdit =(CEdit*)GetDlgItem(IDC_EDIT_RES);
//	if(pCheck->GetCheck()){
//		pCheck->SetCheck(0);
//		pEdit->EnableWindow(TRUE);
//	}else{
//		pCheck->SetCheck(1);
//		SetDlgItemInt(IDC_EDIT_RES,m_nDefaultRes,FALSE);
//		pEdit->EnableWindow(FALSE);
//	}
//}

void CPrintOpt::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	CButton* pCheck=(CButton*)GetDlgItem(IDC_CHECK_PAGEFIT);
	CEdit*   pEdit =(CEdit*)GetDlgItem(IDC_EDIT_RES);

	m_bPageFit=pCheck->GetCheck();
	m_nPrintRes=GetDlgItemInt(IDC_EDIT_RES,NULL,FALSE);

	if(m_bPageFit){
		m_nPrintRes = 100;
	}else{
		if(m_nPrintRes<30 || m_nPrintRes>300){
			AfxMessageBox(IDS_INVALID_RESOLUTION,MB_ICONEXCLAMATION|MB_OK);
			return;
		}
	}

	m_nImageMag=GetDlgItemInt(IDC_EDIT_IMAGE_MAG,NULL,FALSE);
	if(m_nImageMag<100 || m_nImageMag>200){
		AfxMessageBox(IDS_INVALID_EXP_IMAGE_MAG,MB_ICONEXCLAMATION|MB_OK);
		return;
	}




	BOOL bTrans=TRUE;
	BOOL bTransResult;
	m_nLeftMargin=GetDlgItemInt(IDC_EDIT_LEFT_M,&bTransResult,FALSE);
	bTrans &= bTransResult;
	m_nRightMargin=GetDlgItemInt(IDC_EDIT_RIGHT_M,&bTransResult,FALSE);
	bTrans &= bTransResult;
	m_nUpperMargin=GetDlgItemInt(IDC_EDIT_UPPER_M,&bTransResult,FALSE);
	bTrans &= bTransResult;
	m_nLowerMargin=GetDlgItemInt(IDC_EDIT_LOWER_M,&bTransResult,FALSE);
	bTrans &= bTransResult;
	if(		!bTrans
		||	m_nLeftMargin>100
		||	m_nRightMargin>100
		||	m_nUpperMargin>100
		||	m_nLowerMargin>100){
		AfxMessageBox(IDS_INVALID_MARGIN,MB_ICONEXCLAMATION|MB_OK);
		return;
	}
		

	CDialog::OnOK();
}

void CPrintOpt::OnBnClickedCheckPrintbmp()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	CButton* pCheckBmp=(CButton*)GetDlgItem(IDC_CHECK_PRINTBMP);
	CButton* pCheckCol=(CButton*)GetDlgItem(IDC_CHECK_PRINT_COLOR);
	if(pCheckBmp->GetCheck()){
		pCheckCol->SetCheck(0);
		pCheckCol->EnableWindow(FALSE);
	}else{
		pCheckCol->EnableWindow(TRUE);
	}
}

void CPrintOpt::OnBnClickedCheckPagefit()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	CButton* pCheck=(CButton*)GetDlgItem(IDC_CHECK_PAGEFIT);
	CEdit*   pEdit =(CEdit*)GetDlgItem(IDC_EDIT_RES);

	if(pCheck->GetCheck()){
		m_nPrintRes=GetDlgItemInt(IDC_EDIT_RES,NULL,FALSE);
		SetDlgItemText(IDC_EDIT_RES,_T(""));
		pEdit->EnableWindow(FALSE);
	}else{
		pEdit->EnableWindow(TRUE);
		SetDlgItemInt(IDC_EDIT_RES,m_nPrintRes);
	}
}


void CPrintOpt::OnBnClickedCheckFooter()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CButton* pCheck=(CButton*)GetDlgItem(IDC_CHECK_FOOTER);
	CEdit*   pEdit =(CEdit*)GetDlgItem(IDC_EDIT_FOOTER);
	pEdit->EnableWindow(pCheck->GetCheck());

}
