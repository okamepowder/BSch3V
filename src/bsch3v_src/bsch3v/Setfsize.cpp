/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// SetFSize.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "bsch.h"
#include "SetFSize.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetFreeSize ダイアログ


CSetFreeSize::CSetFreeSize(CWnd* pParent /*=NULL*/)
	: CDialog(CSetFreeSize::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetFreeSize)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CSetFreeSize::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetFreeSize)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetFreeSize, CDialog)
	//{{AFX_MSG_MAP(CSetFreeSize)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_LBN_SELCHANGE(IDC_MYSTDSHEETSIZE, OnSelchangeMystdsheetsize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetFreeSize メッセージ ハンドラ

void CSetFreeSize::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	int nX,nY;
	if(!GetXY(&nX,&nY)){
		AfxMessageBox(IDS_INVALID_SHEETSIZE,MB_ICONEXCLAMATION|MB_OK);
		return;
	}
	m_nX = nX;
	m_nY = nY;
	CDialog::OnOK();
}

BOOL CSetFreeSize::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	CEdit* pEdit;

	//入力可能な文字数制限と値の設定
	pEdit=(CEdit*)GetDlgItem(IDC_EDIT_X);
	pEdit->SetLimitText(4);
	pEdit=(CEdit*)GetDlgItem(IDC_EDIT_Y);
	pEdit->SetLimitText(4);
	
	SetXY(m_nX,m_nY);

	CListBox* plistbox =(CListBox*)GetDlgItem(IDC_MYSTDSHEETSIZE);
	CString str;
	TCHAR szEntry[32];
	int n,x,y;
	for(n = 0; n < MAX_STDSIZE; n++){
		_stprintf(szEntry,_T("Size%d"),n);
		str = AfxGetApp()->GetProfileString(_T("MyStdSheetSize"),szEntry,_T(""));
		if(!str.IsEmpty()){
			if(SizeStr2Size(str,&x,&y)){
				plistbox->AddString(str);
			}
		}
	}
	
	SetAddDelButtonState();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

BOOL CSetFreeSize::SizeStr2Size(LPCTSTR strSize,int* x,int* y)
{
	TCHAR szSize[64];
	_tcsncpy(szSize,strSize,63);
	int n = 0;
	while(szSize[n]){
		if(szSize[n] == 'x'){
			szSize[n]=0;
			n++;
			break;
		}
		n++;
	}
	if(szSize[n] == 0) return FALSE;
	*x = _tstoi(&(szSize[0]));
	*y = _tstoi(&(szSize[n]));
	if(*x < XSIZE_MIN || *x > XSIZE_MAX || *y < YSIZE_MIN || *y > YSIZE_MAX){
		return FALSE;
	}else{
		return TRUE;
	}
}

void CSetFreeSize::SetAddDelButtonState()
{
	CListBox* plistbox =(CListBox*)GetDlgItem(IDC_MYSTDSHEETSIZE);
	int nCount = plistbox->GetCount();
	
	CButton* pButton;
	pButton =(CButton*)GetDlgItem(IDC_ADD);
	pButton->EnableWindow(nCount<MAX_STDSIZE);

	pButton =(CButton*)GetDlgItem(IDC_DEL);
	pButton->EnableWindow(nCount>0 && plistbox->GetCurSel()!=LB_ERR);
}

void CSetFreeSize::SaveMyStdSheetInfo()
{
	CListBox* plistbox =(CListBox*)GetDlgItem(IDC_MYSTDSHEETSIZE);

	CString str;
	TCHAR szEntry[32];
	int nCount = plistbox->GetCount();
	if(nCount > MAX_STDSIZE) nCount = MAX_STDSIZE;
	int n;
	for(n = 0; n < MAX_STDSIZE; n++){
		if(n < nCount) plistbox->GetText(n,str);
		else str=_T("");
		_stprintf(szEntry,_T("Size%d"),n);
		AfxGetApp()->WriteProfileString(_T("MyStdSheetSize"),szEntry,str);
	}
}

BOOL CSetFreeSize::SetXY(int x,int y)
{
	CEdit* pEdit;
	TCHAR sz[32];
	pEdit=(CEdit*)GetDlgItem(IDC_EDIT_X);
	wsprintf(sz,_T("%d"),x);
	pEdit->SetWindowText(sz);

	pEdit=(CEdit*)GetDlgItem(IDC_EDIT_Y);
	wsprintf(sz,_T("%d"),y);
	pEdit->SetWindowText(sz);
	return TRUE;
}


BOOL CSetFreeSize::GetXY(int* x,int* y)
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CEdit* pEdit;
	CString str;
	//エディットボックスのXの値の取得
	pEdit=(CEdit*)GetDlgItem(IDC_EDIT_X);
	pEdit->GetWindowText(str);
	*x=_tstoi(str);
	//エディットボックスのYの値の取得
	pEdit=(CEdit*)GetDlgItem(IDC_EDIT_Y);
	pEdit->GetWindowText(str);
	*y=_tstoi(str);
	//範囲内チェック
	if(*x<XSIZE_MIN || *x>XSIZE_MAX || *y<YSIZE_MIN || *y>YSIZE_MAX){
		return FALSE;
	}else{
		return TRUE;
	}
}

void CSetFreeSize::OnAdd() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CListBox* plistbox =(CListBox*)GetDlgItem(IDC_MYSTDSHEETSIZE);
	int nCount = plistbox->GetCount();
	if(nCount >= MAX_STDSIZE) return;
	
	int nX,nY;
	if(!GetXY(&nX,&nY)){
		AfxMessageBox(IDS_INVALID_SHEETSIZE,MB_ICONEXCLAMATION|MB_OK);
		return;
	}
	TCHAR sz[64];
	_stprintf(sz,_T("%dx%d"),nX,nY);

	//同じものがあったら登録しない。
	int n;
	for(n = 0; n < nCount; n++){
		CString str;
		plistbox->GetText(n,str);
		if(_tcscmp(sz,str) == 0) return;
	}

	n = plistbox->AddString(sz);
	plistbox->SetCurSel(n);
	SetAddDelButtonState();

	SaveMyStdSheetInfo();
}

void CSetFreeSize::OnDel() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CListBox* plistbox =(CListBox*)GetDlgItem(IDC_MYSTDSHEETSIZE);
	int nIndex = plistbox->GetCurSel();
	if(LB_ERR == nIndex) return;
	else plistbox->DeleteString(nIndex);
	SetAddDelButtonState();

	SaveMyStdSheetInfo();
}

void CSetFreeSize::OnSelchangeMystdsheetsize() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CListBox* plistbox =(CListBox*)GetDlgItem(IDC_MYSTDSHEETSIZE);
	int nIndex = plistbox->GetCurSel();
	CString str;
	plistbox->GetText(nIndex,str);
	int x,y;
	if(SizeStr2Size(str,&x,&y)){
		SetXY(x,y);
	}
	SetAddDelButtonState();
}
