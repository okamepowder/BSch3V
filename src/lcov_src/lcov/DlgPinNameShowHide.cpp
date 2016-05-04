// DlgPinNameShowHide.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "LCoV.h"
#include "DlgPinNameShowHide.h"
#include "afxdialogex.h"


// CDlgPinNameShowHide �_�C�A���O

IMPLEMENT_DYNAMIC(CDlgPinNameShowHide, CDialog)

CDlgPinNameShowHide::CDlgPinNameShowHide(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPinNameShowHide::IDD, pParent)
{
	m_displayFlag = 0xFFFFFFFF;
	m_strName = "";
}

CDlgPinNameShowHide::~CDlgPinNameShowHide()
{
}

void CDlgPinNameShowHide::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_NAMES, m_listNames);
}


BEGIN_MESSAGE_MAP(CDlgPinNameShowHide, CDialog)
//	ON_BN_CLICKED(IDC_BUTTONDISP_ALL, &CDlgPinNameShowHide::OnBnClickedButtondispAll)
	ON_BN_CLICKED(IDC_BUTTONSHOW_ALL, &CDlgPinNameShowHide::OnBnClickedButtonshowAll)
	ON_BN_CLICKED(IDC_BUTTONHIDE_ALL, &CDlgPinNameShowHide::OnBnClickedButtonhideAll)
END_MESSAGE_MAP()

int CDlgPinNameShowHide::parseName(CStringList &r_nameList)
{
	r_nameList.RemoveAll();

	int len = _tcslen(m_strName);
	CString str("");
	bool backSlashFlag = false;
	for(int i = 0;i<len;i++){
		if(!backSlashFlag){
			if(m_strName[i] == '\\'){
				backSlashFlag = TRUE;
			}else if(m_strName[i] == '/'){
				r_nameList.AddTail(str);
				str = "";
				continue;
			}
		}else{
			backSlashFlag = FALSE;
		}
		str += m_strName[i];
	}
	r_nameList.AddTail(str);
	return r_nameList.GetCount();
}



// CDlgPinNameShowHide ���b�Z�[�W �n���h���[


BOOL CDlgPinNameShowHide::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �����ɏ�������ǉ����Ă�������
	CStringList nameList;
	int count = parseName(nameList);
	POSITION pos = nameList.GetHeadPosition();
	for(int i=0;i<count;i++){
		if(pos == NULL) break;
		CString str = nameList.GetNext(pos);
		m_listNames.InsertString(-1, str);
	}
	count = m_listNames.GetCount();
	unsigned long displayFlagBit = 0x00000001;
	for(int i=0;i<count;i++){
		if(i<32){
			m_listNames.SetCheck(i,(displayFlagBit&m_displayFlag)!=0);
			displayFlagBit<<=1;
		}else{
			m_listNames.SetCheck(i,TRUE);
			m_listNames.Enable(i,FALSE);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}


//void CDlgPinNameShowHide::OnBnClickedButtondispAll()
//{
//	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
//}


void CDlgPinNameShowHide::OnBnClickedButtonshowAll()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	int count = m_listNames.GetCount();
	for(int index = 0;index<count;index++){
		m_listNames.SetCheck(index, TRUE);
	}
}


void CDlgPinNameShowHide::OnBnClickedButtonhideAll()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	int count = m_listNames.GetCount();
	for(int index = 0;index<count;index++){
		m_listNames.SetCheck(index, FALSE);
	}
}


void CDlgPinNameShowHide::OnOK()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	m_displayFlag = 0xFFFFFFFF;
	unsigned long displayFlagBit = 0x00000001;
	int count = m_listNames.GetCount();
	if(count>32){
		count = 32;
	}
	for(int index = 0;index<count;index++){
		if(m_listNames.GetCheck(index)!=BST_CHECKED){
			m_displayFlag ^= displayFlagBit;
		}
		displayFlagBit<<=1;
	}
	CDialog::OnOK();
}
