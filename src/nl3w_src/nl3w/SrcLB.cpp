/****************************************************************************
    NL3W netlist utility for BSch3V
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#include "stdafx.h"
#include "nlist.h"
#include "SrcLB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSrcLB

CSrcLB::CSrcLB()
{
}

CSrcLB::~CSrcLB()
{
}


BEGIN_MESSAGE_MAP(CSrcLB, CListBox)
	//{{AFX_MSG_MAP(CSrcLB)
	ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSrcLB メッセージ ハンドラ

void CSrcLB::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	
	//CListBox::OnDropFiles(hDropInfo);
	int nFiles = DragQueryFile(hDropInfo,-1,NULL,0);
	if(nFiles>0){
		TCHAR szPath[_MAX_PATH];
		TCHAR szExt[_MAX_EXT];
		//CListBox* pLB=(CListBox*)GetDlgItem(IDC_FILELIST);	//リストボックスを取得して…
		//ResetContent();
		int n;
		for(n=0;n<nFiles;n++){
			::DragQueryFile(hDropInfo,n,szPath,_MAX_PATH);
			_tsplitpath(szPath,NULL,NULL,NULL,szExt);
			if(_tcsicmp(szExt,_T(".ce3"))==0){
				AddString(szPath);	//リストボックスへのセット
			}
		}
		::DragFinish(hDropInfo);
		SetListboxWidth();
		GetParent()->SendMessage(WM_COMMAND,MAKELONG(GetDlgCtrlID(),LBN_SELCHANGE),(long)m_hWnd);
	}

}

void CSrcLB::SetListboxWidth()
{
	CString str;
	CDC* pDC;
	CFont* pOldFont;
	CFont* pFont = GetFont();
	CSize textsize;
	int maxwidth=0;
	int i,n;
	pDC = GetDC();

	pOldFont=pDC->SelectObject(pFont);
	n = GetCount();
	for(i=0;i<n;i++){
		GetText(i,str);
		textsize = pDC->GetTextExtent(str);
		if(textsize.cx>maxwidth) maxwidth = textsize.cx;
	}
	SetHorizontalExtent(maxwidth+20);
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);
}