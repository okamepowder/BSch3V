/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// SetFSize.cpp : �C���v�������e�[�V���� �t�@�C��
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
// CSetFreeSize �_�C�A���O


CSetFreeSize::CSetFreeSize(CWnd* pParent /*=NULL*/)
	: CDialog(CSetFreeSize::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetFreeSize)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT
}


void CSetFreeSize::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetFreeSize)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
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
// CSetFreeSize ���b�Z�[�W �n���h��

void CSetFreeSize::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
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
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	CEdit* pEdit;

	//���͉\�ȕ����������ƒl�̐ݒ�
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

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
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
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CEdit* pEdit;
	CString str;
	//�G�f�B�b�g�{�b�N�X��X�̒l�̎擾
	pEdit=(CEdit*)GetDlgItem(IDC_EDIT_X);
	pEdit->GetWindowText(str);
	*x=_tstoi(str);
	//�G�f�B�b�g�{�b�N�X��Y�̒l�̎擾
	pEdit=(CEdit*)GetDlgItem(IDC_EDIT_Y);
	pEdit->GetWindowText(str);
	*y=_tstoi(str);
	//�͈͓��`�F�b�N
	if(*x<XSIZE_MIN || *x>XSIZE_MAX || *y<YSIZE_MIN || *y>YSIZE_MAX){
		return FALSE;
	}else{
		return TRUE;
	}
}

void CSetFreeSize::OnAdd() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
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

	//�������̂���������o�^���Ȃ��B
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
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CListBox* plistbox =(CListBox*)GetDlgItem(IDC_MYSTDSHEETSIZE);
	int nIndex = plistbox->GetCurSel();
	if(LB_ERR == nIndex) return;
	else plistbox->DeleteString(nIndex);
	SetAddDelButtonState();

	SaveMyStdSheetInfo();
}

void CSetFreeSize::OnSelchangeMystdsheetsize() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
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
