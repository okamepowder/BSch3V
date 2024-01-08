// AtribImage.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "BSch.h"
#include "AtribImage.h"


// CAtribImage �_�C�A���O

IMPLEMENT_DYNAMIC(CAtribImage, CDialog)

CAtribImage::CAtribImage(CWnd* pParent /*=NULL*/)
	: CDialog(CAtribImage::IDD, pParent)
{
	m_magnification = 100;
}

CAtribImage::~CAtribImage()
{
}

void CAtribImage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAtribImage, CDialog)
END_MESSAGE_MAP()


// CAtribImage ���b�Z�[�W �n���h��

BOOL CAtribImage::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �����ɏ�������ǉ����Ă�������
	SetDlgItemInt(IDC_EDIT_MAG,m_magnification);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

void CAtribImage::OnOK()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	int n = GetDlgItemInt(IDC_EDIT_MAG);
	if(n<20 || n>200){
		AfxMessageBox(IDS_INVALID_IMAGE_MAGNIFICATION,MB_OK|MB_ICONEXCLAMATION);
	}else{
		m_magnification = n;
		CDialog::OnOK();
	}
}
