/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// SetLib.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include <Shlwapi.h>
#include "BSch.h"
#include "SetLib.h"
#include "xbschglobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetLib �_�C�A���O


CSetLib::CSetLib(CWnd* pParent /*=NULL*/)
	: CDialog(CSetLib::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetLib)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT
}


void CSetLib::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetLib)
                // ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetLib, CDialog)
	//{{AFX_MSG_MAP(CSetLib)
	ON_LBN_SELCHANGE(IDC_LIB_LIST, OnSelchangeLibList)
	ON_BN_CLICKED(IDC_LIB_UP, OnLibUp)
	ON_BN_CLICKED(IDC_LIB_DOWN, OnLibDown)
	ON_BN_CLICKED(IDC_LIB_ADD, OnLibAdd)
	ON_BN_CLICKED(IDC_LIB_DEL, OnLibDel)
	//}}AFX_MSG_MAP
	ON_WM_COPYDATA()
	ON_BN_CLICKED(IDC_LIB_ABS, &CSetLib::OnClickedLibAbs)
	ON_BN_CLICKED(IDC_LIB_REL, &CSetLib::OnClickedLibRel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetLib �����o�[�֐��C���v�������g

//�_�C�A���O�\���{�^���̃C�l�[�u��
void CSetLib::EnableButton(UINT id,BOOL bEnable)
{
	CButton*  pBtn=(CButton*)GetDlgItem(id);
	// �f�B�Z�[�u�����悤�Ƃ���{�^���Ƀt�H�[�J�X������ꍇ��
	// �n�j�{�^���Ƀt�H�[�J�X���Z�b�g����
	if(!bEnable){
		CWnd* pWnd=GetFocus();
		if(pWnd==pBtn){
			CButton* pDefaultBtn=(CButton*)GetDlgItem(IDOK);
			pDefaultBtn->SetFocus();
		}
	}
	pBtn->EnableWindow(bEnable);
}

void CSetLib::SetListboxWidth()
{
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIB_LIST);
	CString rString;
	CDC* pDC;
	CSize textsize;
	int maxwidth=0;
	int i,n;
	pDC = pLB->GetDC();

	n = pLB->GetCount();
	for(i=0;i<n;i++){
		pLB->GetText(i,rString);
		textsize = pDC->GetTextExtent(rString);
		if(textsize.cx>maxwidth) maxwidth = textsize.cx;
	}
	pLB->SetHorizontalExtent(maxwidth);
	pLB->ReleaseDC(pDC);
}



/////////////////////////////////////////////////////////////////////////////
// CSetLib ���b�Z�[�W �n���h��

//WM_INITDIALOG ���b�Z�[�W�n���h��
BOOL CSetLib::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIB_LIST);
	CString rString;
//	CDC* pDC;
	CSize textsize;
	TCHAR str[32];
	int i;
//	int maxwidth=0;



//	pDC = pLB->GetDC();
	//���W�X�g���ɓo�^���ꂽ���C�u�����t�@�C�������X�g�{�b�N�X�֐ݒ�
	for(i=0;i<MAX_LIB;i++){
		wsprintf(str,_T("LIB%d"),i);			//�G���g��������̍쐬
		rString=AfxGetApp()->GetProfileString(_T("Library"),str);
		if(rString.IsEmpty()) break;		//������i���C�u�������j���l���ł��Ȃ�������break
		pLB->InsertString(-1,rString);		//���X�g�{�b�N�X�̍Ō�ɒǉ�
//		textsize = pDC->GetTextExtent(rString);
//		if(textsize.cx>maxwidth) maxwidth = textsize.cx;
	}
	SetListboxWidth();


	m_nList=pLB->GetCount();				//�o�^���̐ݒ�
	EnableButton(IDC_LIB_ADD,m_nList<MAX_LIB);	//�܂��o�^������łȂ���Βǉ��\�Ȃ悤�ɁuADD�v���C�l�[�u��
		
	if(m_nList!=0){					//�o�^�����O�łȂ����
		pLB->SetCurSel(m_nList-1);			//���X�g�{�b�N�X�̈�ԉ��̍s�ɃJ�[�\���ݒ�
	}
	EnableButtons(m_nList-1);


	//EnableButton(IDC_LIB_DOWN,FALSE);		//�u���ֈړ��v�{�^���̓f�B�Z�[�u��
	//if(m_nList!=0){					//�o�^�����O�łȂ����
	//	pLB->SetCurSel(m_nList-1);			//���X�g�{�b�N�X�̈�ԉ��̍s�ɃJ�[�\���ݒ�
	//}else{							//�o�^�����O�Ȃ��
	//	EnableButton(IDC_LIB_UP,  FALSE);	//�u��ֈړ��v���u�폜�v���f�B�Z�[�u��
	//	EnableButton(IDC_LIB_DEL, FALSE);
	//}




	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

//IDOK(OK�{�^��)�̏���
//���W�X�g���Ƀ��X�g�{�b�N�X�̓��e����������
void CSetLib::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIB_LIST);
	CString rString;
	TCHAR str[32];
	int i,n;

	n=pLB->GetCount();							//���X�g�{�b�N�X�ւ̓o�^���̎擾
	for(i=0;i<MAX_LIB;i++){
		if(i<n) pLB->GetText(i,rString);		//�w��s�̃��C�u�������̎擾
		else	rString.Empty();				//�o�^���𒴂���ꍇ�͋󕶎���ɂ���
		wsprintf(str,_T("LIB%d"),i);				//�G���g��������̍쐬
		AfxGetApp()->WriteProfileString(_T("Library"),str,rString);//���W�X�g���ւ̏�������
	}
	CDialog::OnOK();
}


void CSetLib::EnableButtons(int n)
{
	EnableButton(IDC_LIB_DOWN,n>=0 && n<m_nList-1);		//�u���ցv�{�^���̐ݒ�
	EnableButton(IDC_LIB_UP  ,n>0);						//�u��ցv�{�^���̐ݒ�
	
	if(n>=0){
		CString path;
		CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIB_LIST);
		pLB->GetText(n,path);
		BOOL pathRel = ::PathIsRelative(path);
		EnableButton(IDC_LIB_ABS,pathRel);
		EnableButton(IDC_LIB_REL,!pathRel);
	}else{
		EnableButton(IDC_LIB_ABS,FALSE);
		EnableButton(IDC_LIB_REL,FALSE);
	}
}


//���C�u��������ۑ����郊�X�g�{�b�N�X�ŁA�J�[�\�����ړ��i�I������Ă�����́j
//���ς����ꍇ�̏���
//�u��ֈړ��v�u���ֈړ��v�̃{�^���̃C�l�[�u���ݒ�
void CSetLib::OnSelchangeLibList() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIB_LIST);
	int n=pLB->GetCurSel();								//�I������Ă���ʒu�̎擾
 	EnableButtons(n);
}


//���C�u��������ۑ����郊�X�g�{�b�N�X�ŁA�I������Ă�����̂��u��ֈړ��v����
//���݂̈ʒu���炢������폜���āA��O�ɑ}������
void CSetLib::OnLibUp() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIB_LIST);
	CString rString;
	int n=pLB->GetCurSel();			//�I������Ă���ʒu�̎擾
	if(n==LB_ERR || n==0) return;	//�I������Ă�����̂��Ȃ������ɐ擪�̏ꍇ�͉������Ȃ�
	pLB->GetText(n,rString);		//�I������Ă��镶������擾���āc
	pLB->DeleteString(n);			//			���X�g�{�b�N�X����폜
	n--;							//�����ԍ����グ�āc
	pLB->InsertString(n,rString);	//			�ǉ����āc
	pLB->SetCurSel(n);				//			�J�[�\����ݒ肵�đI����Ԃɂ���
	EnableButton(IDC_LIB_DOWN,n<m_nList-1);		//�u���ցv�{�^���̐ݒ�
	EnableButton(IDC_LIB_UP  ,n>0);				//�u��ցv�{�^���̐ݒ�
}

//���C�u��������ۑ����郊�X�g�{�b�N�X�ŁA�I������Ă�����̂��u���ֈړ��v����
//���݂̈ʒu���炢������폜���āA��O�ɑ}������
void CSetLib::OnLibDown() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIB_LIST);
	CString rString;
	int n=pLB->GetCurSel();			//�I������Ă���ʒu�̎擾
	if(n==LB_ERR || n>=m_nList-1) return;	//�I������Ă�����̂��Ȃ������ɍŌ���̏ꍇ�͉������Ȃ�
	pLB->GetText(n,rString);		//�I������Ă��镶������擾���āc
	pLB->DeleteString(n);			//			���X�g�{�b�N�X����폜
	n++;							//�����ԍ����グ�āc
	pLB->InsertString(n,rString);	//			�ǉ����āc
	pLB->SetCurSel(n);				//			�J�[�\����ݒ肵�đI����Ԃɂ���
	EnableButton(IDC_LIB_DOWN,n<m_nList-1);		//�u���ցv�{�^���̐ݒ�
	EnableButton(IDC_LIB_UP  ,n>0);				//�u��ցv�{�^���̐ݒ�
}

//���C�u�����ւ̒ǉ�
//�t�@�C���_�C�A���O���N������(*.LIB)���s�b�N�A�b�v���đI������
void CSetLib::OnLibAdd() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	int nResult;
	if(m_nList>=MAX_LIB) return;	//���łɓo�^����Ȃ�Ȃɂ����Ȃ�

	TCHAR szFileNameBuffer[_MAX_PATH *MAX_LIB+256];
	_tcscpy(szFileNameBuffer,_T("*.LB3"));

	CFileDialog dlg(TRUE,				//�I�[�v���_�C�A���O
					NULL,				//�g���q���t�����Ȃ������ꍇ�̃f�t�H���g
					NULL,			//�����t�@�C����
					OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT,
					_T("BSch Library(*.lb3)|*.LB3|BSch3 schematic(*.ce3)|*.CE3||"),	//�t�B���^������
					this);				//�e�E�C���h�E
	
	CString rCaption;
	rCaption.LoadString(IDS_LIB_ADD_NEW);//�_�C�A���O�{�b�N�X�̃L���v�V���������\�[�X���烍�[�h
	dlg.m_ofn.lpstrTitle=rCaption;		//�t�@�C���_�C�A���O�N���X�ɃL���v�V�����̐ݒ�
	dlg.m_ofn.lpstrFile = szFileNameBuffer;	//�t�@�C�������i�[����o�b�t�@��ݒ肷��B
	dlg.m_ofn.nMaxFile = _MAX_PATH *MAX_LIB+256;		//�t�@�C�������i�[����o�b�t�@�̃T�C�Y��ݒ肷��B
	
	nResult=dlg.DoModal();			//�t�@�C���_�C�A���O�̎��s
	if(nResult==IDOK){		//����I���Ȃ�c
//		CString rString=CString(dlg.m_ofn.lpstrFile);		//�t�@�C���_�C�A���O����t�@�C�������擾����
		CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIB_LIST);	//���X�g�{�b�N�X���擾���āc
		int n;
		CString strPath;
		POSITION pos = dlg.GetStartPosition();
		while(pos){
			strPath = dlg.GetNextPathName(pos);				
			n=pLB->GetCurSel();				//���ݑI������Ă���Ƃ���ɑ}�����邽�߂Ɍ��ݒl�擾
			if(n==LB_ERR) n=0;				//���ݑI������Ă�����̂��Ȃ���ΐ擪�ɓ����
			pLB->InsertString(n,strPath);	//���X�g�{�b�N�X�ւ̃Z�b�g
			pLB->SetCurSel(n);				//�I���s�̍Đݒ�
			m_nList=pLB->GetCount();		//�����o�[�ϐ��ւ̓o�^���̐ݒ�
			if(m_nList>=MAX_LIB) break;
		}
		SetListboxWidth();
		EnableButton(IDC_LIB_ADD ,m_nList<MAX_LIB);//�uADD�v�{�^���̐ݒ�
		EnableButton(IDC_LIB_DOWN,n<m_nList-1);		//�u���ցv�{�^���̐ݒ�
		EnableButton(IDC_LIB_UP  ,n>0);				//�u��ցv�{�^���̐ݒ�
		EnableButton(IDC_LIB_DEL ,TRUE);			//�uDEL�v�{�^���̐ݒ�
	}
}

//���C�u�����̍폜
void CSetLib::OnLibDel() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	int n;
	if(m_nList==0) return;		//�o�^�����O�Ȃ牽�����Ȃ�
    CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIB_LIST);
	n=pLB->GetCurSel();			//�I������Ă�����̂̔ԍ����擾
	if(n==LB_ERR) return;		//�I������Ă�����̂��Ȃ���Ή������Ȃ�
	pLB->DeleteString(n); 		//�I������Ă�����̂��폜
    m_nList=pLB->GetCount(); 	//�o�^���̐ݒ�
	if(n>=m_nList) n=m_nList-1;	//�폜�����̂��ŉ��s�̂��̂Ȃ玟�ɑI������͈̂��
	if(n>=0)pLB->SetCurSel(n);
	SetListboxWidth();
	EnableButton(IDC_LIB_ADD, TRUE);
   	EnableButton(IDC_LIB_DOWN,n<m_nList-1 && m_nList>0);
   	EnableButton(IDC_LIB_UP,  n>0);
	EnableButton(IDC_LIB_DEL, m_nList>0);
	
}




void CSetLib::OnClickedLibAbs()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	CString path;
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIB_LIST);
	int n=pLB->GetCurSel();
	pLB->GetText(n,path);
	BOOL pathRel = ::PathIsRelative(path);
	if(!pathRel) return;

	CString iniDir = ((CBSchApp*)AfxGetApp())->m_szIniFileDir;

	TCHAR fullPath[_MAX_PATH*2];	//��₱�������΃p�X�����邾�낤���B
	TCHAR clnFullPath[_MAX_PATH];
	::PathCombine(fullPath,iniDir,path);
	::PathCanonicalize(clnFullPath,fullPath);	//�r���ɑ��΂���������Ԃ𐮗�����B

	pLB->DeleteString(n);				//			���X�g�{�b�N�X����폜
	pLB->InsertString(n,clnFullPath);	//			�ǉ����āc
	pLB->SetCurSel(n);					//			�J�[�\����ݒ肵�đI����Ԃɂ���

	EnableButtons(n);
}


void CSetLib::OnClickedLibRel()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	CString path;
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIB_LIST);
	int n=pLB->GetCurSel();
	pLB->GetText(n,path);
	BOOL pathisRel = ::PathIsRelative(path);
	if(pathisRel) return;

	TCHAR relPath[_MAX_PATH];

	::PathRelativePathTo(relPath, ((CBSchApp*)AfxGetApp())->m_szIniFileDir ,FILE_ATTRIBUTE_DIRECTORY,path,FILE_ATTRIBUTE_NORMAL);

	//TCHAR iniDir[_MAX_PATH];
	//::PathCanonicalize(iniDir,((CBSchApp*)AfxGetApp())->m_szIniFileDir); 
	//::PathRelativePathTo(relPath, iniDir ,FILE_ATTRIBUTE_DIRECTORY,path,FILE_ATTRIBUTE_NORMAL);

	pLB->DeleteString(n);				//			���X�g�{�b�N�X����폜
	pLB->InsertString(n,relPath);		//			�ǉ����āc
	pLB->SetCurSel(n);					//			�J�[�\����ݒ肵�đI����Ԃɂ���

	EnableButtons(n);

}
