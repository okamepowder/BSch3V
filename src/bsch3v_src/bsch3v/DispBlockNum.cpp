// DispBlockNum.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "DispBlockNum.h"
#include "afxdialogex.h"


// CDispBlockNum �_�C�A���O

IMPLEMENT_DYNAMIC(CDispBlockNum, CDialog)

CDispBlockNum::CDispBlockNum(CWnd* pParent /*=NULL*/)
	: CDialog(CDispBlockNum::IDD, pParent)
{
	m_dispBN = DISP_BLOCKNUM_TYPE::NOTDISPLAY_BLOCKNUM;
}

CDispBlockNum::~CDispBlockNum()
{
}

void CDispBlockNum::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDispBlockNum, CDialog)
END_MESSAGE_MAP()


// CDispBlockNum ���b�Z�[�W �n���h���[


BOOL CDispBlockNum::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �����ɏ�������ǉ����Ă�������

	CButton* pBtn;
	
	pBtn=(CButton*)GetDlgItem(IDC_RADIO_NOTDISPLAY);
	pBtn->SetCheck(m_dispBN == DISP_BLOCKNUM_TYPE::NOTDISPLAY_BLOCKNUM ? BST_CHECKED : BST_UNCHECKED);

	pBtn=(CButton*)GetDlgItem(IDC_RADIO_NUM);
	pBtn->SetCheck(m_dispBN == DISP_BLOCKNUM_TYPE::DISPLAY_BLOCKNUM_NUM ? BST_CHECKED : BST_UNCHECKED);

	pBtn=(CButton*)GetDlgItem(IDC_RADIO_ALP);
	pBtn->SetCheck(m_dispBN == DISP_BLOCKNUM_TYPE::DISPLAY_BLOCKNUM_ALPHA ? BST_CHECKED : BST_UNCHECKED);


	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


void CDispBlockNum::OnOK()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	//CButton* pBtn;
	
	if(((CButton*)GetDlgItem(IDC_RADIO_NOTDISPLAY))->GetCheck() ==  BST_CHECKED){
		m_dispBN = DISP_BLOCKNUM_TYPE::NOTDISPLAY_BLOCKNUM;
	}else if(((CButton*)GetDlgItem(IDC_RADIO_NUM))->GetCheck() ==  BST_CHECKED){
		m_dispBN = DISP_BLOCKNUM_TYPE::DISPLAY_BLOCKNUM_NUM;
	}else if(((CButton*)GetDlgItem(IDC_RADIO_ALP))->GetCheck() ==  BST_CHECKED){
		m_dispBN = DISP_BLOCKNUM_TYPE::DISPLAY_BLOCKNUM_ALPHA;
	}
	
	CDialog::OnOK();
}


void CDispBlockNum::OnCancel()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

	CDialog::OnCancel();
}
