/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// SetLayer.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "bsch.h"
#include "SetLayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetLayer �_�C�A���O


CSetLayer::CSetLayer(CWnd* pParent /*=NULL*/)
	: CDialog(CSetLayer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetLayer)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT
	m_wShow=0xFFFF;
	m_nEdit=0;
	m_fEditHigh=FALSE;
}


void CSetLayer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetLayer)
                // ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetLayer, CDialog)
	//{{AFX_MSG_MAP(CSetLayer)
	ON_BN_CLICKED(IDC_D0, OnD0)
	ON_BN_CLICKED(IDC_D1, OnD1)
	ON_BN_CLICKED(IDC_D2, OnD2)
	ON_BN_CLICKED(IDC_D3, OnD3)
	ON_BN_CLICKED(IDC_D4, OnD4)
	ON_BN_CLICKED(IDC_D5, OnD5)
	ON_BN_CLICKED(IDC_D6, OnD6)
	ON_BN_CLICKED(IDC_D7, OnD7)
	ON_BN_CLICKED(IDC_E0, OnE0)
	ON_BN_CLICKED(IDC_E1, OnE1)
	ON_BN_CLICKED(IDC_E2, OnE2)
	ON_BN_CLICKED(IDC_E3, OnE3)
	ON_BN_CLICKED(IDC_E4, OnE4)
	ON_BN_CLICKED(IDC_E5, OnE5)
	ON_BN_CLICKED(IDC_E6, OnE6)
	ON_BN_CLICKED(IDC_E7, OnE7)
	ON_BN_CLICKED(IDC_CHECK_EDIT_HIGHLIGHT, OnCheckEditHighlight)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_DIPLAY_NAME_ON_BAR, &CSetLayer::OnClickedCheckDiplayNameOnBar)
END_MESSAGE_MAP()


WORD CSetLayer::GetBit(int nLayer)
{
	static WORD table[]={
		0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80
	};
	if(nLayer<0 || nLayer>7) return 1;
	return table[nLayer];
}



/////////////////////////////////////////////////////////////////////////////
// CSetLayer ���b�Z�[�W �n���h��

BOOL CSetLayer::OnInitDialog() 
{
	CDialog::OnInitDialog();


	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	WORD wBit;
	int  n;
	int  nID;

	if(m_wShow==0) m_wShow=0xFF;
	if(!(m_wShow & GetBit(m_nEdit))){
		wBit=0x01;
		n=0;
		while(!(wBit & m_wShow)){ wBit<<=1; n++; }
		m_nEdit=n;
	}
	//IDC_D0�`IDC_D7�͘A�������l�ɂȂ�悤�Ƀ��\�[�X�G�f�B�^�Őݒ肵�Ă���
	//IDC_E0�`IDC_E7�͘A�������l�ɂȂ�悤�Ƀ��\�[�X�G�f�B�^�Őݒ肵�Ă���
	//IDC_N0�`IDC_N7�͘A�������l�ɂȂ�悤�Ƀ��\�[�X�G�f�B�^�Őݒ肵�Ă���
	CButton* pButton;
	nID=IDC_D0;
	wBit=1;
	for(n=0;n<8;n++){
		pButton=(CButton*)GetDlgItem(nID);
		pButton->SetCheck((wBit & m_wShow)!=0);
		nID++;
		wBit<<=1;
	}
	nID=IDC_E0;
	for(n=0;n<8;n++){
		pButton=(CButton*)GetDlgItem(nID);
		pButton->SetCheck(n==m_nEdit);
		nID++;
	}
	pButton=(CButton*)GetDlgItem(IDC_CHECK_EDIT_HIGHLIGHT);
	pButton->SetCheck(m_fEditHigh!=FALSE);

	pButton = (CButton*)GetDlgItem(IDC_CHECK_DIPLAY_NAME_ON_BAR);
	pButton->SetCheck(m_fDisplayNameOnDlgBar);


	CEdit* pEdit;
	nID = IDC_N0;
	for (int i = 0; i < 8; i++) {
		pEdit = (CEdit*)GetDlgItem(nID);
		pEdit->SetWindowText(m_strName[i]);
		nID++;
	}


	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CSetLayer::OnDCheck(int n)
{
	CButton* pButton=(CButton*)GetDlgItem(IDC_D0+n);
	if(pButton->GetCheck()){	//���݂��`�F�b�N���`�F�b�N��OFF�ɂ���B���݂̕ҏW�Ώۂ̃��C���[��OFF�ɂł��Ȃ��B
		CButton* pEButton=(CButton*)GetDlgItem(IDC_E0+n);
		if(!pEButton->GetCheck())	pButton->SetCheck(0);
	}else{
		pButton->SetCheck(1);
	}
}

void CSetLayer::OnECheck(int n)
{
	CButton* pButton=(CButton*)GetDlgItem(IDC_D0+n);
	pButton->SetCheck(1);	//�Y�����C���[�͋����I�ɕ\����Ԃɂ���
	int i;
	for(i=0;i<8;i++){
		CButton* pEButton=(CButton*)GetDlgItem(IDC_E0+i);
		pEButton->SetCheck(i==n);
	}
}


void CSetLayer::OnD0() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	OnDCheck(0);
}

void CSetLayer::OnD1() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	OnDCheck(1);
}

void CSetLayer::OnD2() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	OnDCheck(2);
}

void CSetLayer::OnD3() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	OnDCheck(3);
}

void CSetLayer::OnD4() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	OnDCheck(4);
}

void CSetLayer::OnD5() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	OnDCheck(5);
}

void CSetLayer::OnD6() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	OnDCheck(6);
}

void CSetLayer::OnD7() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	OnDCheck(7);
}

void CSetLayer::OnE0() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	OnECheck(0);
}

void CSetLayer::OnE1() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	OnECheck(1);
}

void CSetLayer::OnE2() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	OnECheck(2);
}

void CSetLayer::OnE3() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	OnECheck(3);
}

void CSetLayer::OnE4() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	OnECheck(4);
}

void CSetLayer::OnE5() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	OnECheck(5);
}

void CSetLayer::OnE6() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	OnECheck(6);
}

void CSetLayer::OnE7() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	OnECheck(7);
}

void CSetLayer::OnOK() 
{
	// TODO: ���̈ʒu�ɂ��̑��̌��ؗp�̃R�[�h��ǉ����Ă�������

	//IDC_D0�`IDC_D7�͘A�������l�ɂȂ�悤�Ƀ��\�[�X�G�f�B�^�Őݒ肵�Ă���
	//IDC_E0�`IDC_E7�͘A�������l�ɂȂ�悤�Ƀ��\�[�X�G�f�B�^�Őݒ肵�Ă���
	//IDC_N0�`IDC_N7�͘A�������l�ɂȂ�悤�Ƀ��\�[�X�G�f�B�^�Őݒ肵�Ă���
	int n;
	CButton* pButton;
	WORD wBit=1;
	m_wShow=0;
	for(n=0;n<8;n++){
		pButton=(CButton*)GetDlgItem(IDC_D0+n);
		if(pButton->GetCheck()) m_wShow |= wBit;
		wBit<<=1;
	}
	for(n=0;n<8;n++){
		pButton=(CButton*)GetDlgItem(IDC_E0+n);
		if(pButton->GetCheck()){
			m_nEdit=n;
			break;
		}
	}
	pButton=(CButton*)GetDlgItem(IDC_CHECK_EDIT_HIGHLIGHT);
	m_fEditHigh=pButton->GetCheck();

	pButton = (CButton*)GetDlgItem(IDC_CHECK_DIPLAY_NAME_ON_BAR);
	m_fDisplayNameOnDlgBar = pButton->GetCheck();


	CEdit* pEdit;
	int nID = IDC_N0;
	for (int i = 0; i < 8; i++) {
		pEdit = (CEdit*)GetDlgItem(nID);
		pEdit->GetWindowText(m_strName[i]);
		nID++;
	}



	CDialog::OnOK();
}

void CSetLayer::OnCheckEditHighlight() 
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CButton* pButton=(CButton*)GetDlgItem(IDC_CHECK_EDIT_HIGHLIGHT);
	pButton->SetCheck(pButton->GetCheck()==FALSE);
}


void CSetLayer::OnClickedCheckDiplayNameOnBar()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_DIPLAY_NAME_ON_BAR);
	pButton->SetCheck(pButton->GetCheck() == FALSE);
}
