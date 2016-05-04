/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

// CompProperty.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "LCoV.h"
#include "CompProperty.h"
#include ".\compproperty.h"


// CCompProperty �_�C�A���O

IMPLEMENT_DYNAMIC(CCompProperty, CDialog)
CCompProperty::CCompProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CCompProperty::IDD, pParent)
	
{
	m_name = "NAME";
	m_num = "U";
	m_note = "";
	m_mfr = "";
	m_mfrpn = "";
	m_pkg = "";
	m_block = 1;
	m_x = 2;
	m_y = 2;
	m_usePtn = FALSE;
	m_noBitPtn = FALSE;
	m_useFixedBlockNum = FALSE;
	m_fixedBlockNum = 1;
	m_sharedBlock = FALSE;

}

CCompProperty::~CCompProperty()
{
}

void CCompProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NOTE, m_note);
	DDX_Text(pDX, IDC_EDIT_MFR, m_mfr);
	DDX_Text(pDX, IDC_EDIT_MFRPN, m_mfrpn);
	DDX_Text(pDX, IDC_EDIT_PKG, m_pkg);
	DDV_MaxChars(pDX, m_note, 1023);
}


BEGIN_MESSAGE_MAP(CCompProperty, CDialog)
//	ON_EN_CHANGE(IDC_EDIT_NOTE, OnEnChangeEditNote)
ON_BN_CLICKED(IDC_CHECK_USEPTN, OnBnClickedCheckUseptn)
ON_BN_CLICKED(IDC_CHECK_FIXEDBLOCKNUM, &CCompProperty::OnClickedCheckFixedblocknum)
ON_BN_CLICKED(IDC_CHECK_SHARED_BLOCK, &CCompProperty::OnClickedCheckSharedBlock)
END_MESSAGE_MAP()


// CCompProperty ���b�Z�[�W �n���h��

BOOL CCompProperty::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO :  �����ɏ�������ǉ����Ă�������
	if(!m_usePtn) m_noBitPtn = FALSE;
	SetDlgItemText(IDC_EDIT_COMP_NAME,m_name);
	SetDlgItemText(IDC_EDIT_COMP_REF,m_num);
	SetDlgItemInt(IDC_EDIT_COMP_SIZEX,m_x);
	SetDlgItemInt(IDC_EDIT_COMP_SIZEY,m_y);
	//SetDlgItemInt(IDC_EDIT_COMP_BLOCK,m_block);
	CButton* pButton;
	pButton = (CButton*)GetDlgItem(IDC_CHECK_USEPTN);
	pButton->SetCheck(m_usePtn);
	pButton = (CButton*)GetDlgItem(IDC_CHECK_NOBIT);
	pButton->SetCheck(m_noBitPtn);
	pButton->EnableWindow(m_usePtn);


	setBlockDlgItems();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

void CCompProperty::setBlockDlgItems()
{
	GetDlgItem(IDC_EDIT_COMP_BLOCK)->EnableWindow(		!m_sharedBlock  && !m_useFixedBlockNum);
	GetDlgItem(IDC_EDIT_COMP_FIXEDBLOCK)->EnableWindow( m_useFixedBlockNum );
				
	((CButton*)GetDlgItem(IDC_CHECK_SHARED_BLOCK))->SetCheck(m_sharedBlock ? BST_CHECKED:BST_UNCHECKED);
	((CButton*)GetDlgItem(IDC_CHECK_FIXEDBLOCKNUM))->SetCheck(!m_sharedBlock  && m_useFixedBlockNum? BST_CHECKED:BST_UNCHECKED);
	

	if(m_sharedBlock){
		SetDlgItemInt(IDC_EDIT_COMP_BLOCK,1);
		SetDlgItemInt(IDC_EDIT_COMP_FIXEDBLOCK,1);
	}else if(m_useFixedBlockNum){
		SetDlgItemInt(IDC_EDIT_COMP_BLOCK,1);
		SetDlgItemInt(IDC_EDIT_COMP_FIXEDBLOCK,m_fixedBlockNum);
	}else{
		SetDlgItemInt(IDC_EDIT_COMP_BLOCK,m_block);
		SetDlgItemInt(IDC_EDIT_COMP_FIXEDBLOCK,1);
	}
}

void CCompProperty::OnOK()
{
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	GetDlgItemText(IDC_EDIT_COMP_NAME,m_name);
	GetDlgItemText(IDC_EDIT_COMP_REF,m_num);
	m_x = GetDlgItemInt(IDC_EDIT_COMP_SIZEX);
	m_y = GetDlgItemInt(IDC_EDIT_COMP_SIZEY);
	m_block = GetDlgItemInt(IDC_EDIT_COMP_BLOCK);
	CButton* pButton;
	pButton = (CButton*)GetDlgItem(IDC_CHECK_USEPTN);
	m_usePtn = pButton->GetCheck();
	pButton = (CButton*)GetDlgItem(IDC_CHECK_NOBIT);
	m_noBitPtn = pButton->GetCheck();

	m_fixedBlockNum =  GetDlgItemInt(IDC_EDIT_COMP_FIXEDBLOCK);


	if(!isValidPartName(m_name)){
		AfxMessageBox(IDS_PARTNAME_INVALID);
		return;
	}

	if(m_x<1 || m_x>255 || m_y<1 || m_y>255) {
		AfxMessageBox(IDS_PARTSIZE_RANGE);
		return;
	}
	if(m_block<1 || m_block>255){
		AfxMessageBox(IDS_PART_BLOCKRANGE);
		return;
	}

	if(m_fixedBlockNum<1 || m_fixedBlockNum>255){
		AfxMessageBox(IDS_PART_FIXEDBLOCKRANGE);
		return;
	}


	if(!storeCheckSharedBlock()){
		storeFixedblocknumState();
	}

	CDialog::OnOK();
}


BOOL CCompProperty::isValidPartName(const TCHAR*  psz)
{
	int n=lstrlen(psz);
	if(n==0 || n>32) return FALSE;
	while(*psz){
		if(  *psz<0x21
		   ||*psz>0x7E
   		   ||*psz=='{'
		   ||*psz=='}') return FALSE;
		psz++;
	}
	return TRUE;
}

//void CCompProperty::OnEnChangeEditNote()
//{
//	// TODO :  ���ꂪ RICHEDIT �R���g���[���̏ꍇ�A�܂��ACDialog::OnInitDialog() �֐����I�[�o�[���C�h���āA
//	// OR ��Ԃ� ENM_CORRECTTEXT �t���O���}�X�N�ɓ���āA
//	// CRichEditCtrl().SetEventMask() ���Ăяo���Ȃ�����A
//	// �R���g���[���́A���̒ʒm�𑗐M���܂���B
//
//	// TODO :  �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����Ă��������B
//}

void CCompProperty::OnBnClickedCheckUseptn()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	CButton* pButton;
	pButton = (CButton*)GetDlgItem(IDC_CHECK_USEPTN);
	m_usePtn = pButton->GetCheck();
	pButton = (CButton*)GetDlgItem(IDC_CHECK_NOBIT);
	pButton->EnableWindow(m_usePtn);
	pButton->SetCheck(m_usePtn);	//0.81.01
}


void CCompProperty::OnClickedCheckFixedblocknum()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	storeFixedblocknumState();
	
	setBlockDlgItems();
}

BOOL CCompProperty::storeFixedblocknumState()
{
	if(((CButton*)GetDlgItem(IDC_CHECK_FIXEDBLOCKNUM))->GetCheck()==BST_CHECKED){
		m_sharedBlock = FALSE;
		m_useFixedBlockNum = TRUE;
	}else{
		m_sharedBlock = FALSE;
		m_useFixedBlockNum = FALSE;
	}
	return m_useFixedBlockNum;
}



void CCompProperty::OnClickedCheckSharedBlock()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	storeCheckSharedBlock();

	setBlockDlgItems();

}

BOOL CCompProperty::storeCheckSharedBlock()
{
	if(((CButton*)GetDlgItem(IDC_CHECK_SHARED_BLOCK))->GetCheck()==BST_CHECKED){
		m_sharedBlock = TRUE;
		m_useFixedBlockNum = FALSE;
	}else{
		m_sharedBlock = FALSE;
		m_useFixedBlockNum = FALSE;
	}
	return m_sharedBlock;
}

