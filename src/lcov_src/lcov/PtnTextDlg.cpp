// PtnTextDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "LCoV.h"
#include "PtnTextDlg.h"
#include ".\ptntextdlg.h"


// CPtnTextDlg �_�C�A���O

IMPLEMENT_DYNAMIC(CPtnTextDlg, CDialog)
CPtnTextDlg::CPtnTextDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPtnTextDlg::IDD, pParent)
{
	m_strText="";
	m_align=0;

	char* p=(char*)(&m_logfont);	//LOGFONT�\���̂̂O�C�j�V�����C�Y
	for(int n=0;n<sizeof(LOGFONT);n++) *p++=0;
	m_logfont.lfHeight=10;
}

CPtnTextDlg::~CPtnTextDlg()
{
}

void CPtnTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPtnTextDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK_COMFONG, OnBnClickedCheckComfong)
	ON_BN_CLICKED(IDC_BUTTON_FONT, OnBnClickedButtonFont)
END_MESSAGE_MAP()


// CPtnTextDlg ���b�Z�[�W �n���h��

BOOL CPtnTextDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TEXT);
	pEdit->SetLimitText(250);					//0.82.01


	((CButton*)GetDlgItem(IDC_CHECK_COMFONT))->SetCheck(m_logfont.lfFaceName[0]==0);
	OnBnClickedCheckComfong();

	// TODO :  �����ɏ�������ǉ����Ă�������
	SetDlgItemText(IDC_EDIT_TEXT,m_strText);
	switch(m_align&3){
		case 0:
			((CButton*)GetDlgItem(IDC_RADIO_HL))->SetCheck(1);
			break;
		case 1:
			((CButton*)GetDlgItem(IDC_RADIO_HR))->SetCheck(1);
			break;
		default:
			((CButton*)GetDlgItem(IDC_RADIO_HC))->SetCheck(1);
	}

	switch((m_align&0xC)>>2){
		case 0:
			((CButton*)GetDlgItem(IDC_RADIO_VB))->SetCheck(1);
			break;
		case 1:
			((CButton*)GetDlgItem(IDC_RADIO_VT))->SetCheck(1);
			break;
		default:
			((CButton*)GetDlgItem(IDC_RADIO_VC))->SetCheck(1);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

void CPtnTextDlg::OnOK()
{
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	GetDlgItemText(IDC_EDIT_TEXT,m_strText);
	if(((CButton*)GetDlgItem(IDC_RADIO_HL))->GetCheck()){
		m_align = 0;
	}else
	if(((CButton*)GetDlgItem(IDC_RADIO_HR))->GetCheck()){
		m_align = 1;
	}else
	{
		m_align = 2;
	}

	if(((CButton*)GetDlgItem(IDC_RADIO_VT))->GetCheck()){
		m_align |= 0x4;
	}else
	if(((CButton*)GetDlgItem(IDC_RADIO_VC))->GetCheck()){
		m_align |= 0x8;
	}

	CDialog::OnOK();
}

void CPtnTextDlg::OnBnClickedCheckComfong()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	BOOL checked = ((CButton*)GetDlgItem(IDC_CHECK_COMFONT))->GetCheck();
	((CButton*)GetDlgItem(IDC_BUTTON_FONT))->EnableWindow(!checked);
	if(checked){
		m_logfont.lfFaceName[0] = 0;
		m_logfont.lfHeight = 10;
	}
}

void CPtnTextDlg::OnBnClickedButtonFont()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	LOGFONT logfont = m_logfont;
	CFontDialog dlg(&logfont,CF_TTONLY | CF_SCREENFONTS);
	if(dlg.DoModal()==IDOK){
		m_logfont = logfont;
	}
}
