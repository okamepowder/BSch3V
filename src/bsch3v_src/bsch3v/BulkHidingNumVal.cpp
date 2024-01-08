// BulkHidingNumVal.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "BSch.h"
#include "BulkHidingNumVal.h"


// CBulkHidingNumVal �_�C�A���O

IMPLEMENT_DYNAMIC(CBulkHidingNumVal, CDialog)

CBulkHidingNumVal::CBulkHidingNumVal(CWnd* pParent /*=NULL*/)
	: CDialog(CBulkHidingNumVal::IDD, pParent)
{
	m_bHideNumCheckState = BST_INDETERMINATE;
	m_bHideValCheckState = BST_INDETERMINATE;
}

CBulkHidingNumVal::~CBulkHidingNumVal()
{
}

void CBulkHidingNumVal::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBulkHidingNumVal, CDialog)
//	ON_BN_CLICKED(IDC_CHECK_HIDENUM, &CBulkHidingNumVal::OnBnClickedCheckHidenum)
END_MESSAGE_MAP()



BOOL CBulkHidingNumVal::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �����ɏ�������ǉ����Ă�������
	CButton* pCheck;
	pCheck = (CButton*)GetDlgItem(IDC_CHECK_HIDENUM);
	pCheck->SetCheck(BST_INDETERMINATE);
	pCheck = (CButton*)GetDlgItem(IDC_CHECK_HIDEVAL);
	pCheck->SetCheck(BST_INDETERMINATE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

void CBulkHidingNumVal::OnOK()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	CButton* pCheck;
	pCheck = (CButton*)GetDlgItem(IDC_CHECK_HIDENUM);
	m_bHideNumCheckState =  pCheck->GetCheck();
	pCheck = (CButton*)GetDlgItem(IDC_CHECK_HIDEVAL);
	m_bHideValCheckState =  pCheck->GetCheck();
	CDialog::OnOK();
}
