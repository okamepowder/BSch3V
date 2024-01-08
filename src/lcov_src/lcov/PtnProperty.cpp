/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

// PtnProperty.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "LCoV.h"
#include "PtnProperty.h"
#include ".\ptnproperty.h"


// CPtnProperty �_�C�A���O

IMPLEMENT_DYNAMIC(CPtnProperty, CDialog)
CPtnProperty::CPtnProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CPtnProperty::IDD, pParent)
	, m_fill(FALSE)
	, m_dash(FALSE)
{
	EnableAutomation();
	m_width = 1;
	m_enableFill = FALSE;

}

CPtnProperty::~CPtnProperty()
{
}

void CPtnProperty::OnFinalRelease()
{
	// �I�[�g���[�V���� �I�u�W�F�N�g�ɑ΂���Ō�̎Q�Ƃ���������Ƃ���
	// OnFinalRelease ���Ăяo����܂��B��{�N���X�͎����I�ɃI�u�W�F�N
	// �g���폜���܂��B��{�N���X���Ăяo���O�ɁA�I�u�W�F�N�g�ŕK�v�ȓ�
	// �ʂȌ㏈����ǉ����Ă��������B

	CDialog::OnFinalRelease();
}

void CPtnProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_FILL, m_fill);
	DDX_Check(pDX, IDC_CHECK_DASHSTYLE, m_dash);
}


BEGIN_MESSAGE_MAP(CPtnProperty, CDialog)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CPtnProperty, CDialog)
END_DISPATCH_MAP()

// ���� : VBA ����^�C�v �Z�[�t�ȘA�����T�|�[�g���邽�߂ɁAIID_ILCoV �̃T�|�[�g��ǉ����܂��B
//  ���� IID �́A.IDL �t�@�C���̃f�B�X�p�b�` �C���^�[�t�F�C�X�փA�^�b�`����� 
//  GUID �ƈ�v���Ȃ���΂Ȃ�܂���B

// {A9544089-6C40-4280-A9FA-E1AFA8616A23}
static const IID IID_IPtnProperty =
{ 0xA9544089, 0x6C40, 0x4280, { 0xA9, 0xFA, 0xE1, 0xAF, 0xA8, 0x61, 0x6A, 0x23 } };

BEGIN_INTERFACE_MAP(CPtnProperty, CDialog)
	INTERFACE_PART(CPtnProperty, IID_IPtnProperty, Dispatch)
END_INTERFACE_MAP()


// CPtnProperty ���b�Z�[�W �n���h��

BOOL CPtnProperty::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO :  �����ɏ�������ǉ����Ă�������
	CComboBox* pCombo =(CComboBox*)GetDlgItem(IDC_COMBO_WIDTH);
	int i;
	TCHAR buff[16];
	for(i=0;i<=5;i++){
		_stprintf(buff,_T("%d"),i);
		pCombo->AddString(buff);
	}
	if(m_width>5){
		m_width = 5;
	}else if(m_width<0){
		m_width = 0;
	}
	pCombo->SetCurSel(m_width);

	CButton* pButton =(CButton*)GetDlgItem(IDC_CHECK_FILL);
	pButton->EnableWindow(m_enableFill);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

void CPtnProperty::OnOK()
{
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	CComboBox* pCombo =(CComboBox*)GetDlgItem(IDC_COMBO_WIDTH);
	m_width = pCombo->GetCurSel();
	if(m_width>5){
		m_width = 5;
	}else if(m_width<0){
		m_width = 0;
	}

	CDialog::OnOK();
}
