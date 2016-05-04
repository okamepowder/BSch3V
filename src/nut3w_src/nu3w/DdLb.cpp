/****************************************************************************
    NUT3W Numbering software for BSch3V
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// DdLb.cpp : 実装ファイル
//

#include "stdafx.h"
#include "nu3w.h"
#include "DdLb.h"
#include ".\ddlb.h"


// CDdLb

IMPLEMENT_DYNAMIC(CDdLb, CListBox)
CDdLb::CDdLb()
{
}

CDdLb::~CDdLb()
{
}


BEGIN_MESSAGE_MAP(CDdLb, CListBox)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()



// CDdLb メッセージ ハンドラ


void CDdLb::OnDropFiles(HDROP hDropInfo)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

//	CListBox::OnDropFiles(hDropInfo);
	int nFiles = DragQueryFile(hDropInfo,-1,NULL,0);
	if(nFiles>0){
		TCHAR szPath[_MAX_PATH];
		TCHAR szExt[_MAX_EXT];
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



void CDdLb::SetListboxWidth()
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