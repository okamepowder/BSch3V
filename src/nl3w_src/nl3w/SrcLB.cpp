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
// CSrcLB ���b�Z�[�W �n���h��

void CSrcLB::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	
	//CListBox::OnDropFiles(hDropInfo);
	int nFiles = DragQueryFile(hDropInfo,-1,NULL,0);
	if(nFiles>0){
		TCHAR szPath[_MAX_PATH];
		TCHAR szExt[_MAX_EXT];
		//CListBox* pLB=(CListBox*)GetDlgItem(IDC_FILELIST);	//���X�g�{�b�N�X���擾���āc
		//ResetContent();
		int n;
		for(n=0;n<nFiles;n++){
			::DragQueryFile(hDropInfo,n,szPath,_MAX_PATH);
			_tsplitpath(szPath,NULL,NULL,NULL,szExt);
			if(_tcsicmp(szExt,_T(".ce3"))==0){
				AddString(szPath);	//���X�g�{�b�N�X�ւ̃Z�b�g
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