// LinePropertyDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "LCoV.h"
#include "LinePropertyDlg.h"
#include "afxdialogex.h"


// CLinePropertyDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CLinePropertyDlg, CDialog)

CLinePropertyDlg::CLinePropertyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLinePropertyDlg::IDD, pParent)
	, m_curveline(FALSE)
	, m_dash(FALSE)
{
	m_width = 1;
}

CLinePropertyDlg::~CLinePropertyDlg()
{
}

void CLinePropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_CURVELINE, m_curveline);
	DDX_Check(pDX, IDC_CHECK_DASHSTYLE, m_dash);
}


BEGIN_MESSAGE_MAP(CLinePropertyDlg, CDialog)
END_MESSAGE_MAP()


// CLinePropertyDlg ���b�Z�[�W �n���h���[


BOOL CLinePropertyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �����ɏ�������ǉ����Ă�������

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

	

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


void CLinePropertyDlg::OnOK()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	CComboBox* pCombo =(CComboBox*)GetDlgItem(IDC_COMBO_WIDTH);
	m_width = pCombo->GetCurSel();
	if(m_width>5){
		m_width = 5;
	}else if(m_width<0){
		m_width = 0;
	}

	CDialog::OnOK();
}
