/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// PartAtrb.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include <assert.h>
#include <string>
#include <list>
using namespace std;
#include "BSch.h"

//#include "PartLib.h"
#include "PartAtrb.h"
#include "complib.h"
#include "Preview.h"
#include "SelPart.h"

#include "xbschglobal.h"

#include <stdlib.h>  //_itoa()�̂��߂ɃC���N���[�h
#include ".\partatrb.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPartAtrb �_�C�A���O


CPartAtrb::CPartAtrb(CWnd* pParent /*=NULL*/)
	: CDialog(CPartAtrb::IDD, pParent)
	, m_bHideNum(FALSE)
	, m_bHideVal(FALSE)
{
	//{{AFX_DATA_INIT(CPartAtrb)
	m_rPartName = _T("");
	m_rPartNum = _T("");
	m_rNote = _T("");
	//}}AFX_DATA_INIT
	m_nBlock   =1;
	m_nBlockMin=1;
	m_nBlockMax=1;
//	m_pPartIndexReplace = NULL;
	m_pPartIndex = NULL;
	m_enableEditComp = FALSE;
	m_modifiedCompInfo =FALSE;
	m_bNameVertical=FALSE;
	m_bNumVertical=FALSE;
	m_mfr = _T("");
	m_mfrpnum = _T("");
	m_package = _T("");
	m_nFocusedTo = FOCUSED_TO_BODY;
}


void CPartAtrb::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_EDIT_PARTBLOCK, m_nBlock);
	//DDV_MinMaxUInt(pDX, m_nBlock, 0, 100);
	//{{AFX_DATA_MAP(CPartAtrb)
	DDX_Text(pDX, IDC_EDIT_PARTNAME, m_rPartName);
	DDX_Text(pDX, IDC_EDIT_PARTNUM, m_rPartNum);
	DDX_Text(pDX, IDC_EDIT_NOTE, m_rNote);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK_NAMEVERT, m_bNameVertical);
	DDX_Check(pDX, IDC_CHECK_NUMVERT, m_bNumVertical);
	DDX_Text(pDX, IDC_EDIT_MFR, m_mfr);
	DDX_Text(pDX, IDC_EDIT_MFR_PNUM, m_mfrpnum);
	DDX_Text(pDX, IDC_EDIT_PKG, m_package);
	DDX_Check(pDX, IDC_CHECK_HIDENUM, m_bHideNum);
	DDX_Check(pDX, IDC_CHECK_HIDEVAL, m_bHideVal);
	DDV_MaxChars(pDX, m_rNote, 1023);
}


BEGIN_MESSAGE_MAP(CPartAtrb, CDialog)
	//{{AFX_MSG_MAP(CPartAtrb)
	ON_EN_CHANGE(IDC_EDIT_PARTBLOCK, OnChangeEditPartblock)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_REPLACE_COMPINDEX, OnBnClickedReplaceCompindex)
	ON_BN_CLICKED(IDC_EDIT_COMPINDEX, OnBnClickedEditCompindex)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPartAtrb ���b�Z�[�W �n���h��

BOOL CPartAtrb::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	SetDlgItemText(IDC_ORGNAME,m_rOrgName);
	if(m_pPartIndex->m_pCompInfo){
		SetDlgItemText(IDC_EDIT_LIB_NOTE,m_pPartIndex->m_pCompInfo->m_note.c_str());
	}

	m_nBlockMax=m_pPartIndex->block();

	//-------2014/05/17--------------------------
	CString blockStr;
	switch(m_pPartIndex->m_pCompInfo->m_ciType){
		case SCompInfo::CITYPE::FIXED_BLOCK:
			m_nBlock = 1;
			blockStr.LoadString(IDS_FIXED_BLOCK);
			SetDlgItemInt(IDC_EDIT_PARTBLOCK,m_pPartIndex->m_pCompInfo->m_nFixedBlock+1);			
			break;
		case SCompInfo::CITYPE::SHARED_BLOCK:
			m_nBlock = 1;
			blockStr.LoadString(IDS_SHARED_BLOCK);
			SetDlgItemInt(IDC_EDIT_PARTBLOCK,1);			
		    break;
		default:
			blockStr.LoadString(IDS_BLOCK);
	}
	SetDlgItemText(IDC_STATIC_BLOCK_STATUS,blockStr);
	if(m_pPartIndex->m_pCompInfo->m_ciType == SCompInfo::CITYPE::GENERAL){
		GetDlgItem(IDC_EDIT_PARTBLOCK)->EnableWindow(TRUE);
		CSpinButtonCtrl* pSpin=(CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_PARTBLOCK);
		pSpin->ShowWindow(SW_SHOW);
		pSpin->SetRange(m_nBlockMin,m_nBlockMax);
		if(m_nBlock>m_nBlockMax){
			m_nBlock=m_nBlockMax;
		}
		pSpin->SetPos(m_nBlock);
	}else{
		GetDlgItem(IDC_SPIN_PARTBLOCK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_PARTBLOCK)->EnableWindow(FALSE);
	}


	//-------------------------------------------
	


	SetBlockPins(m_nBlock);
	CEdit* pEdit=(CEdit*)GetDlgItem(IDC_EDIT_PARTBLOCK);
	pEdit->LimitText(2);

	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];

	::GetModuleFileName(NULL,m_szLcoPath,_MAX_PATH);
	::_tsplitpath(m_szLcoPath,drive,dir,NULL,NULL);
	::_tmakepath(m_szLcoPath,drive,dir,_T("LCoV"),_T(".exe"));

	FILE* fp;

	fp = _tfopen(m_szLcoPath,_T("r"));
	m_enableEditComp = (fp != NULL);
	if(fp != NULL) fclose(fp);
	CButton* pButton = (CButton*)GetDlgItem(IDC_EDIT_COMPINDEX);
	pButton->EnableWindow(m_enableEditComp);

	CEdit* pEditFocused = NULL;
	if(m_nFocusedTo == FOCUSED_TO_VAL){
		pEditFocused = (CEdit*)GetDlgItem(IDC_EDIT_PARTNAME);
	}else{
		pEditFocused = (CEdit*)GetDlgItem(IDC_EDIT_PARTNUM);
	}
	if(pEditFocused){
		pEditFocused->SetFocus();
		pEditFocused->SetSel(0,-1);
		return FALSE;
	}else{
		return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
	}
}

void CPartAtrb::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	CString rStr;
	CEdit* pEdit=(CEdit*)GetDlgItem(IDC_EDIT_PARTBLOCK);
	pEdit->GetWindowText(rStr);
	m_nBlock=_tstoi(rStr);
	if(m_nBlock<m_nBlockMin) m_nBlock=m_nBlockMin;
	else if(m_nBlock>m_nBlockMax) m_nBlock=m_nBlockMax;

	SCompInfo* pInfo = m_pPartIndex->compInfo();
	CString str;
	GetDlgItemText(IDC_EDIT_MFR,str);
	pInfo->m_mfr = str;
	GetDlgItemText(IDC_EDIT_MFR_PNUM,str);
	pInfo->m_mfrpn = str;
	GetDlgItemText(IDC_EDIT_PKG,str);
	pInfo->m_pkg = str;

	CDialog::OnOK();
}

//�g�p�u���b�N��ύX�����Ƃ��A�s���ԍ���\������悤�ɕύX 1997/01/21
//���̋@�\�� OnVScroll��������G�f�B�b�g�R���g���[���ύX�����ɕύX 1997/01/22
//void CPartAtrb::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
//{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	// ���̃_�C�A���O�ɂ� WM_VSCROLL �𔭐�����A�C�e����������Ȃ��̂�
	// �T�{���̃R�[�h�ɂȂ��Ă���BWM_VSCROLL �𔭐�����A�C�e����ǉ�������
	// �����Ə���
//	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);

//	if(nSBCode==SB_THUMBPOSITION){
//		SetBlockPins(nPos);
//	}
//}

//IDC_PIN_NUM �X�^�e�B�b�N�R���g���[���Ƀs���ԍ����Z�b�g
void CPartAtrb::SetBlockPins(int nBlock)
{
	CString rString;		//��̕�����
	//char szPinNum[32];			//�����ɏ����s���ԍ����i�[���� rString �ɒǉ�
//	char* pc;				//�ꎞ�I�Ɏg�������|�C���^
//	DWORD dwPinNumInfo;		//m_pPartIndex���瓾���鐶�̃s���ԍ����
	int n,nMaxPin;
	//WORD wNumPfix;
	const SPin* ppininfo;
	
	if(nBlock>=m_nBlockMin && nBlock<=m_nBlockMax){
		nMaxPin=m_pPartIndex->pinCount();
		if(nMaxPin>10) nMaxPin=10;	//��������\�����Ă��d�����Ȃ��A�Ǝv��
		for(n=0;n<nMaxPin;n++){
			ppininfo = m_pPartIndex->pin(n);
			//dwPinNumInfo=m_pPartIndex->GetPinNum(n,nBlock-1);//�unBlock-1�v�����ۂ̃u���b�N�ԍ�
			//wNumPfix=HIWORD(dwPinNumInfo);					 //�s���ԍ��̏�ʃ��[�h
			//pc=szPinNum;
			if(n>0) rString+=",";	//�Q�߈ȍ~�͐擪��','������
			rString+=ppininfo->pinNum(nBlock-1);
		}
	}
	CStatic* pST=(CStatic*)GetDlgItem(IDC_PIN_NUM);
	if(pST!=NULL)	pST->SetWindowText(rString);
}


//�g�p�u���b�N��ύX�����Ƃ��A�s���ԍ���\������ 1997/01/22
//    �i���̋@�\�� OnVScroll�������炱��ɕύX�����j
//�E�X�s���R���g���[���𑀍삵���ꍇ
//   (1)�X�s���R���g���[���������Ńo�f�B��IDC_EDIT_PARTBLOCK��ύX
//   (2)EN_CHANGE�𔭐�
//   (3)SetBlockPins()�����o�[�֐���IDC_PIN_NUM�Ƀs���ԍ����Z�b�g
//�E���� IDC_EDIT_PARTBLOCK ��ύX�����ꍇ
//   (1)���[�U�[��IDC_EDIT_PARTBLOCK��ύX
//   (2)EN_CHANGE�𔭐�
//   (3)SetBlockPins()�����o�[�֐���IDC_PIN_NUM�Ƀs���ԍ����Z�b�g
void CPartAtrb::OnChangeEditPartblock() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CString rStr;
	CEdit* pEdit=(CEdit*)GetDlgItem(IDC_EDIT_PARTBLOCK);
	pEdit->GetWindowText(rStr);
	int nBlock=_tstoi(rStr);
	SetBlockPins(nBlock);
}

void CPartAtrb::OnBnClickedReplaceCompindex()
{
	CSelPart dlg;
	dlg.m_bUsePreview=TRUE;		//�v���r���[���g�����ǂ����̐ݒ�
	if(g_LibraryInfo(m_nLastSelectLibrary)){//NULL�łȂ����...
		dlg.m_nLastSelectLibrary=m_nLastSelectLibrary;
		dlg.m_nLastSelectPart=m_nLastSelectPart;
	}else{
		dlg.m_nLastSelectLibrary=0;
		dlg.m_nLastSelectPart=0;
	}
	if(dlg.DoModal()==IDCANCEL){
		return;
	}

	const SCompIndex* pPartIndex = dlg.m_pCompIndex;
	if(pPartIndex==NULL){
		CString rString;
		//���b�Z�[�W�{�b�N�X�p�̃��b�Z�[�W�̍쐬
		AfxFormatString1(rString,IDS_NOT_FOUND_PART,dlg.m_rStrPartName); 
		AfxMessageBox(rString,MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	m_nLastSelectPart=    dlg.m_nLastSelectPart;
	m_nLastSelectLibrary= dlg.m_nLastSelectLibrary;
	m_pPartIndex = pPartIndex;
	m_rOrgName = pPartIndex->name();
	m_nBlockMax=m_pPartIndex->block();

	//-------2014/05/17--------------------------
	CString blockStr;
	switch(m_pPartIndex->m_pCompInfo->m_ciType){
		case SCompInfo::CITYPE::FIXED_BLOCK:
			m_nBlock = 1;
			blockStr.LoadString(IDS_FIXED_BLOCK);
			SetDlgItemInt(IDC_EDIT_PARTBLOCK,m_pPartIndex->m_pCompInfo->m_nFixedBlock+1);			
			break;
		case SCompInfo::CITYPE::SHARED_BLOCK:
			m_nBlock = 1;
			blockStr.LoadString(IDS_SHARED_BLOCK);
			SetDlgItemInt(IDC_EDIT_PARTBLOCK,1);			
		    break;
		default:
			blockStr.LoadString(IDS_BLOCK);
	}
	SetDlgItemText(IDC_STATIC_BLOCK_STATUS,blockStr);
	if(m_pPartIndex->m_pCompInfo->m_ciType == SCompInfo::CITYPE::GENERAL){
		GetDlgItem(IDC_EDIT_PARTBLOCK)->EnableWindow(TRUE);
		CSpinButtonCtrl* pSpin=(CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_PARTBLOCK);
		pSpin->ShowWindow(SW_SHOW);
		pSpin->SetRange(m_nBlockMin,m_nBlockMax);
		if(m_nBlock>m_nBlockMax){
			m_nBlock=m_nBlockMax;
		}
		pSpin->SetPos(m_nBlock);
	}else{
		GetDlgItem(IDC_SPIN_PARTBLOCK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_PARTBLOCK)->EnableWindow(FALSE);
	}

//-------------------------------------------


	SetBlockPins(m_nBlock);

//	CStatic* pST=(CStatic*)GetDlgItem(IDC_ORGNAME);
//	pST->SetWindowText(m_rOrgName);
	SetDlgItemText(IDC_ORGNAME,m_rOrgName);
	if(m_pPartIndex->m_pCompInfo){
		SetDlgItemText(IDC_EDIT_LIB_NOTE,m_pPartIndex->m_pCompInfo->m_note.c_str());
		const TCHAR* pc;

		pc = m_pPartIndex->Mfr();
		if(pc!=NULL && *pc != 0){	SetDlgItemText(IDC_EDIT_MFR,pc);	}

		pc = m_pPartIndex->MfrPn();
		if(pc!=NULL && *pc != 0){	SetDlgItemText(IDC_EDIT_MFR_PNUM,pc);	}

		pc = m_pPartIndex->Pkg();
		if(pc!=NULL && *pc != 0){	SetDlgItemText(IDC_EDIT_PKG,pc);	}
	}
}





void CPartAtrb::OnBnClickedEditCompindex()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B

	if(!m_enableEditComp)return;

	SCompInfo* pInfo = m_pPartIndex->compInfo();

	CString str;
	GetDlgItemText(IDC_EDIT_MFR,str);
	pInfo->m_mfr = str;
	GetDlgItemText(IDC_EDIT_MFR_PNUM,str);
	pInfo->m_mfrpn = str;
	GetDlgItemText(IDC_EDIT_PKG,str);
	pInfo->m_pkg = str;

	CBSchApp::editWidthLCoV(m_rOrgName, pInfo, m_tempLib);

	//TCHAR dir[_MAX_PATH];
	//TCHAR path[_MAX_PATH];
	//TCHAR cmdline[_MAX_PATH*2+10];

	//::GetTempPath(_MAX_PATH,dir);
	//if(::GetTempFileName(dir,_T("b3v"),0,path)==0) return;

	//FILE* fp;
	//if((fp = _tfopen(path,_T("wt, ccs=UTF-8")))==NULL) return;
	//fseek( fp, 0, SEEK_SET ) ; //BAD KNOWHOW for removing BOM CODE.

	//SCompInfo* pInfo = m_pPartIndex->compInfo();

	//CString str;
	//GetDlgItemText(IDC_EDIT_MFR,str);
	//pInfo->m_mfr = str;
	//GetDlgItemText(IDC_EDIT_MFR_PNUM,str);
	//pInfo->m_mfrpn = str;
	//GetDlgItemText(IDC_EDIT_PKG,str);
	//pInfo->m_pkg = str;


	//SWriteCE3 wce3;
	//wce3.Attach(fp);
	//wce3.WriteRecord(_T("+BSCH3_LIB_V.1.0"));	wce3.WriteEOL();
	//wce3.WriteRecordInt(_T("VER"),SCompLib::m_nConstDataVersion);	wce3.WriteEOL();

	////SCompInfo* pInfo = m_pPartIndex->compInfo();
	//pInfo->writeCe3(wce3,m_rOrgName);
	//wce3.WriteRecord(_T("-BSCH3_LIB_V.1.0"));
	//wce3.WriteEOL();
	//fclose(fp);

	//_stprintf(cmdline,_T("%s \"%s\""),m_szLcoPath,path);

	//PROCESS_INFORMATION pi;
	//STARTUPINFO si;

 //   ZeroMemory(&pi, sizeof(pi));
	//ZeroMemory(&si, sizeof(si));
 //   si.cb = sizeof(si);

 //   BOOL resProc = CreateProcess(NULL, cmdline, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	//if(!resProc) return;
	//
	//while(WAIT_TIMEOUT==WaitForSingleObject(pi.hProcess,1)){
	//	MSG msg;
	//	if(GetMessage(&msg,NULL,0,0)){
	//		if(msg.message == WM_PAINT){				//	���b�Z�[�W�� WM_PAINT �Ȃ�
	//			TranslateMessage(&msg);
	//			DispatchMessage(&msg);
	//		}
	//	}
	//}

	//TRACE("End LCoV\n");
	//CloseHandle(pi.hProcess);
 //   CloseHandle(pi.hThread);

	//tempLib.deleteContent();
	//tempLib.readLibraryFile(path);
	//DeleteFile(path);	
	
	if(m_tempLib.count()<1) return;

	SCompIndex* pPartIndex = m_tempLib.compIndex(0);
	if(pPartIndex==NULL) return;
	
	m_pPartIndex = pPartIndex;



	m_rOrgName = pPartIndex->name();

	m_nBlockMax=m_pPartIndex->block();


//-------2014/05/17--------------------------
	CString blockStr;
	switch(m_pPartIndex->m_pCompInfo->m_ciType){
		case SCompInfo::CITYPE::FIXED_BLOCK:
			m_nBlock = 1;
			blockStr.LoadString(IDS_FIXED_BLOCK);
			SetDlgItemInt(IDC_EDIT_PARTBLOCK,m_pPartIndex->m_pCompInfo->m_nFixedBlock+1);			
			break;
		case SCompInfo::CITYPE::SHARED_BLOCK:
			m_nBlock = 1;
			blockStr.LoadString(IDS_SHARED_BLOCK);
			SetDlgItemInt(IDC_EDIT_PARTBLOCK,1);			
		    break;
		default:
			blockStr.LoadString(IDS_BLOCK);
	}
	SetDlgItemText(IDC_STATIC_BLOCK_STATUS,blockStr);
	if(m_pPartIndex->m_pCompInfo->m_ciType == SCompInfo::CITYPE::GENERAL){
		GetDlgItem(IDC_EDIT_PARTBLOCK)->EnableWindow(TRUE);
		CSpinButtonCtrl* pSpin=(CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_PARTBLOCK);
		pSpin->ShowWindow(SW_SHOW);
		pSpin->SetRange(m_nBlockMin,m_nBlockMax);
		if(m_nBlock>m_nBlockMax){
			m_nBlock=m_nBlockMax;
		}
		pSpin->SetPos(m_nBlock);
	}else{
		GetDlgItem(IDC_SPIN_PARTBLOCK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_PARTBLOCK)->EnableWindow(FALSE);
	}

//-------------------------------------------

	SetBlockPins(m_nBlock);

//	CStatic* pST=(CStatic*)GetDlgItem(IDC_ORGNAME);
//	pST->SetWindowText(m_rOrgName);
	SetDlgItemText(IDC_ORGNAME,m_rOrgName);
	if(m_pPartIndex->m_pCompInfo){
		SetDlgItemText(IDC_EDIT_LIB_NOTE,m_pPartIndex->m_pCompInfo->m_note.c_str());

		SetDlgItemText(IDC_EDIT_MFR,m_pPartIndex->Mfr());
		SetDlgItemText(IDC_EDIT_MFR_PNUM,m_pPartIndex->MfrPn());
		SetDlgItemText(IDC_EDIT_PKG,m_pPartIndex->Pkg());
	}

	m_modifiedCompInfo = TRUE;
	
}
