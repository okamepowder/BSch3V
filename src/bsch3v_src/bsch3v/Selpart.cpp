/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// SelPart.cpp : �C���v�������e�[�V���� �t�@�C��
//
//***����***
//���i���C�u�����̃��X�g�{�b�N�X����\�[�g�̑����ł��邱�Ƃ�O��ɂ��Ă���B

#include "stdafx.h"
#include "BSch.h"
#include <assert.h>
#include <string>
#include <list>
using namespace std;

#include "complib.h"
#include "Preview.h"
#include "SelPart.h"

#include "xbschglobal.h"
#include <io.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelPart �_�C�A���O


CSelPart::CSelPart(CWnd* pParent /*=NULL*/)
	: CDialog(CSelPart::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelPart)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT
	m_nLastSelectPart=0;
	m_nLastSelectLibrary=0;
	m_pwndPreview=NULL;
	m_bUsePreview=TRUE;
	m_bInhibitUpdatePartName = FALSE;
	m_bInhibitUpdateListbox = FALSE;
	m_pCompIndex = NULL;
}


void CSelPart::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelPart)
                // ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelPart, CDialog)
	//{{AFX_MSG_MAP(CSelPart)
	ON_LBN_SELCHANGE(IDC_LIST_LIBRARY, OnSelchangeListLibrary)
	ON_LBN_SELCHANGE(IDC_LIST_PARTNAME, OnSelchangeListPartname)
	ON_LBN_DBLCLK(IDC_LIST_PARTNAME, OnDblclkListPartname)
	ON_EN_CHANGE(IDC_EDIT_PARTNAME, OnChangeEditPartname)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//�w�肵�����C�u�����̕��i�����X�g�{�b�N�X�ɐݒ肷��
void CSelPart::SetPartName(int nLib)
{
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIST_PARTNAME);
	pLB->ShowWindow(SW_HIDE);
	//���e�̑S����
	int n=pLB->GetCount();			//�o�^�ς݂̐��𓾂�
	while(n) n=pLB->DeleteString(0);//�c�肪�������擪�̕����������
	m_nPartCount=0;
	SCompIndex* pPartIndex;
	const SCompLib* pCompLib = g_LibraryInfo(nLib);
	if(pCompLib){
		m_nPartCount=pCompLib->count();	//���C�u�����ɓo�^����Ă��镔�i��
		for(n=0;n<m_nPartCount;n++){
			pPartIndex=pCompLib->compIndex(n);
			pLB->AddString(pPartIndex->name());
		}
		//�ݒ肵�����i�����A�O��I���������i�̏����ԍ��𖞂����Ȃ��ꍇ��
		//�O��̑I�����N���A����B
		if(m_nPartCount-1< m_nLastSelectPart){
			m_nLastSelectPart=0;
		}
		pLB->SetCurSel(m_nLastSelectPart);	//�O��I���������̂ɃJ�[�\����u��
		OnSelchangeListPartname(); 
//		CString rStrPartName;
//		pLB->GetText(m_nLastSelectPart,rStrPartName);
//		SetEditPartName(rStrPartName);
		if(m_pwndPreview && !m_bInhibitUpdatePartName)  //�v���r���[�̍X�V
		{
			int nSel=m_nLastSelectPart;
			SCompIndex* pPartIndex=pCompLib->compIndex(nSel);
			m_pwndPreview->SetPartIndex(pPartIndex);
		}
	}else{//�w�肵�����C�u�����������������Ƃ��̏���
	//		m_nLastSelectPart=0;
	}
	pLB->ShowWindow(SW_SHOW);
}

void CSelPart::AutoUpdate()
{
	int n;
	int count=g_LibraryCount();
	for(n=0;n<count;n++){
		const SCompLib* pCompLib = g_LibraryInfo(n);
		_wfinddata64i32_t findData;
		if(_tfindfirst(pCompLib->m_lpszLibraryName,&findData)==-1) continue;
		int wr_time = findData.time_write;
		if(wr_time <= pCompLib->m_aux1){
			continue;
		}else{
			CString strPathname = pCompLib->m_lpszLibraryName;
			SCompLib* pCompLib=new SCompLib;
			if(!pCompLib->readLibraryFile(strPathname)){
				delete pCompLib;
			}else{
				if(!g_ReplaceLibrary(n,pCompLib)){
					delete pCompLib;
				}else{
					TRACE("Update library - %s\n",strPathname);
				}
			}
		}
	}
}


//���C�u�����̖��O�����X�g�{�b�N�X�ɐݒ肷��
//�\�������C�u�����t�@�C���̃p�X���疼�O�����ɕύX  1997/01/21
void CSelPart::SetLibraryName()
{
	TCHAR szLibPath[_MAX_PATH];
	TCHAR szLibName[_MAX_FNAME];
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIST_LIBRARY);
	int n=pLB->GetCount();			//�o�^�ς݂̐��𓾂�
	while(n) n=pLB->DeleteString(0);//�c�肪�������擪�̕����������
	for(n=0;n<MAX_LIB;n++){
		const SCompLib* pCompLib = g_LibraryInfo(n);
		if(pCompLib==NULL) break;
		_tcscpy(szLibPath,pCompLib->m_lpszLibraryName);
		_tsplitpath(szLibPath,NULL,NULL,szLibName,NULL);
		pLB->AddString(szLibName);
	}
	m_nLibraryCount=n;
	//�ݒ肵�����C�u���������A�O��I���������C�u�����ԍ��𖞂����Ȃ��ꍇ��
	if(m_nLibraryCount-1<m_nLastSelectLibrary){
		m_nLastSelectLibrary=0;
		m_nLastSelectPart=0;
	}
	pLB->SetCurSel(m_nLastSelectLibrary);
}

//�G�f�B�b�g�R���g���[���Ƀp�[�c����ݒ�
void CSelPart::SetEditPartName(LPCTSTR psz)
{
	m_bInhibitUpdateListbox = TRUE;
	if(!m_bInhibitUpdatePartName){
		CEdit* pED=(CEdit*)GetDlgItem(IDC_EDIT_PARTNAME);
		pED->SetWindowText(psz);
	}

	m_rStrPartName = psz;
	m_rStrPartName.MakeUpper();
	m_bInhibitUpdateListbox = FALSE;

}

/////////////////////////////////////////////////////////////////////////////
// CSelPart ���b�Z�[�W �n���h��

BOOL CSelPart::OnInitDialog() 
{
	CDialog::OnInitDialog();

	AutoUpdate();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	//�v���r���[���g���Ƃ��ɂ́A�_�C�A���O�̕����L���āA������CPreview�̃E�C���h�E��
	//CreateEx����B
	if(m_bUsePreview){
		m_pwndPreview=new CPreview;
		GetWindowRect(&m_rcDialog);
		GetClientRect(&m_rcClient);
		int nHeight=m_rcClient.bottom;
		MoveWindow( m_rcDialog.left,m_rcDialog.top,
					m_rcDialog.right-m_rcDialog.left+nHeight,
					m_rcDialog.bottom-m_rcDialog.top);
		CString strClassName=AfxRegisterWndClass(
			CS_VREDRAW|CS_HREDRAW,
			AfxGetApp()->LoadStandardCursor(IDC_ARROW), 
			NULL);

//			(HBRUSH)GetStockObject(WHITE_BRUSH));
		m_pwndPreview->CreateEx(
			WS_EX_CLIENTEDGE,
			strClassName,
			_T("Preview"),			//�v��Ȃ�
			WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL,
			m_rcClient.right,
			6,
			nHeight-6,
			nHeight-12,
			this->m_hWnd,
			(HMENU)IDC_PREVIEW);
	}
	//���i���G�f�B�b�g�{�b�N�X�̕���������
	CEdit* pED=(CEdit*)GetDlgItem(IDC_EDIT_PARTNAME);
	pED->SetLimitText(32);
	SetLibraryName();
	SetPartName(m_nLastSelectLibrary);
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

//���C�u�����̑I����Ԃ��ς�����Ƃ��̃n���h��
void CSelPart::OnSelchangeListLibrary() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIST_LIBRARY);
	m_nLastSelectLibrary=pLB->GetCurSel();
	m_nLastSelectPart=0;	//���C�u�����̑I�����ς��ΑO��I���p�[�c���O�N���A
	SetPartName(m_nLastSelectLibrary);
}

//���i�̑I����Ԃ��ς�����Ƃ��̃n���h��
void CSelPart::OnSelchangeListPartname() 
{
	int nSel;
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIST_PARTNAME);
	nSel=pLB->GetCurSel();
	//g_pPartLib[m_nLastSelectLibrary]->m_nLastSelectIndex=nSel;
	m_nLastSelectPart=nSel;
	CString rStrPartName;
	pLB->GetText(nSel,rStrPartName);
	SetEditPartName(rStrPartName);
	if(m_pwndPreview) //�v���r���[�̍X�V
	{
		//SCompIndex* pPartIndex=g_LibraryInfo(n)
		const SCompLib* pCompLib = g_LibraryInfo(m_nLastSelectLibrary);
		m_pCompIndex=pCompLib->compIndex(nSel);
		//CPartIndex* pPartIndex=g_pPartLib[m_nLastSelectLibrary]->GetPartIndex(nSel);
		m_pwndPreview->SetPartIndex(m_pCompIndex);
	}
	if(m_pCompIndex->m_pCompInfo){
		SetDlgItemText(IDC_EDIT_MFR2,m_pCompIndex->Mfr());
		SetDlgItemText(IDC_EDIT_MFR_PNUM2,m_pCompIndex->MfrPn());
		SetDlgItemText(IDC_EDIT_PKG2,m_pCompIndex->Pkg());
		CString note;
		switch(m_pCompIndex->m_pCompInfo->m_ciType){
			case SCompInfo::CITYPE::FIXED_BLOCK:
				note.LoadString(IDS_FIXED_BLOCK);
				break;
			case SCompInfo::CITYPE::SHARED_BLOCK:
				note.LoadString(IDS_SHARED_BLOCK);
				break;
		}
		note.Append(m_pCompIndex->m_pCompInfo->m_note.c_str());
		SetDlgItemText(IDC_EDIT_NOTE,note);
	}
}

void CSelPart::OnCancel() 
{
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B
	
	CDialog::OnCancel();
}

void CSelPart::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
//	CEdit* pED=(CEdit*)GetDlgItem(IDC_EDIT_PARTNAME);
//	pED->GetWindowText(m_rStrPartName);
//	m_rStrPartName.MakeUpper();	//�啶���ɕϊ� 1997/01/25
	CDialog::OnOK();
}

//���i�I�����X�g�{�b�N�X�̃_�u���N���b�N�őI���I�� 1997/01/25
void CSelPart::OnDblclkListPartname() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	OnOK();
}

void CSelPart::OnChangeEditPartname() 
{
	// TODO: ���ꂪ RICHEDIT �R���g���[���̏ꍇ�A�R���g���[���́A lParam �}�X�N
	// ���ł̘_���a�� ENM_CHANGE �t���O�t���� CRichEditCrtl().SetEventMask()
	// ���b�Z�[�W���R���g���[���֑��邽�߂� CDialog::OnInitDialog() �֐����I�[�o�[
	// ���C�h���Ȃ����肱�̒ʒm�𑗂�܂���B
	
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	//static int debug_val = 0;
	//debug_val++;
	//TRACE("CSelPart::OnChangeEditPartname()  %d\n",debug_val);
	//return;

	if(m_bInhibitUpdateListbox)return;

	CEdit* pED=(CEdit*)GetDlgItem(IDC_EDIT_PARTNAME);
	pED->GetWindowText(m_rStrPartName);
	m_rStrPartName.MakeUpper();

	int nLib;
	int nIndex;
	
	m_pCompIndex = g_SearchComponentIndex(m_rStrPartName,&nLib,&nIndex,NULL);
	if(m_pCompIndex){
		m_bInhibitUpdatePartName = TRUE;

		m_nLastSelectLibrary = nLib;
		//OnSelchangeListPartname();
		m_nLastSelectPart  = nIndex;

		CListBox* pLBLib=(CListBox*)GetDlgItem(IDC_LIST_LIBRARY);
		pLBLib->SetCurSel(m_nLastSelectLibrary);
		SetPartName(m_nLastSelectLibrary);
		CListBox* pLBPart=(CListBox*)GetDlgItem(IDC_LIST_PARTNAME);
		pLBPart->SetCurSel(m_nLastSelectPart);

		if(m_pwndPreview){ //�v���r���[�̍X�V
			m_pwndPreview->SetPartIndex(m_pCompIndex);
		}
		if(m_pCompIndex->m_pCompInfo){
			SetDlgItemText(IDC_EDIT_MFR2,m_pCompIndex->Mfr());
			SetDlgItemText(IDC_EDIT_MFR_PNUM2,m_pCompIndex->MfrPn());
			SetDlgItemText(IDC_EDIT_PKG2,m_pCompIndex->Pkg());
			CString note;
			switch(m_pCompIndex->m_pCompInfo->m_ciType){
				case SCompInfo::CITYPE::FIXED_BLOCK:
					note.LoadString(IDS_FIXED_BLOCK);
					break;
				case SCompInfo::CITYPE::SHARED_BLOCK:
					note.LoadString(IDS_SHARED_BLOCK);
					break;
			}
			note.Append(m_pCompIndex->m_pCompInfo->m_note.c_str());
			SetDlgItemText(IDC_EDIT_NOTE,note);
		}
		m_bInhibitUpdatePartName = FALSE;
	}


}
