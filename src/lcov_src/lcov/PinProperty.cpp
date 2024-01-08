/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

// PinProperty.cpp : 実装ファイル
//

#include "stdafx.h"
#include "LCoV.h"
#include "PinProperty.h"
#include ".\pinproperty.h"
#include <list>
#include <string>
using namespace std;
#include "complib.h"
#include "DlgPinNameShowHide.h"


// PinProperty ダイアログ

IMPLEMENT_DYNAMIC(CPinProperty, CDialog)
CPinProperty::CPinProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CPinProperty::IDD, pParent)
{
	m_arrayNum = NULL;
	//m_pPin = NULL;
	m_currentBlock = 0;
	//m_bChanged = false;
	m_hideNum = false;
	m_pinType = 0;
	m_pinNameDispFlag = 0xFFFFFFFF;

}

CPinProperty::~CPinProperty()
{
	delete[]m_arrayNum;
	//delete m_pPin;
}

void CPinProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CPinProperty::setBlock(int block)
{
	delete[]m_arrayNum;
	m_arrayNum = NULL;
	if(block>0){
		m_arrayNum = new CString[block];
	}
	m_block = block;
}

void CPinProperty::setName(const TCHAR* name)
{
	if(!name) return;
	m_name = "";
	while(*name){
		TCHAR cc = *name;
		cc &= 0x7f;
		if(*name & 0x80){
			m_name += '\\';
		}
		m_name += cc;
		name++;
	}
}




BEGIN_MESSAGE_MAP(CPinProperty, CDialog)
//	ON_BN_CLICKED(IDC_RADIO_NORM, OnBnClickedRadioNorm)
ON_EN_CHANGE(IDC_EDIT_BLOCK, OnEnChangeEditBlock)
ON_BN_CLICKED(IDC_BUTTON_SHOWHIDE_NAME, &CPinProperty::OnBnClickedButtonShowhideName)
ON_EN_CHANGE(IDC_EDIT_PIN_NAME, &CPinProperty::OnEnChangeEditPinName)
END_MESSAGE_MAP()


// PinProperty メッセージ ハンドラ

BOOL CPinProperty::OnInitDialog()
{
	CDialog::OnInitDialog();

	int nPinType = 0;

	// TODO :  ここに初期化を追加してください
	CEdit* pEdit;

	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PIN_NAME);
	pEdit->SetLimitText(250);					//0.81.01
	pEdit->SetWindowText(m_name);

	m_name_old = m_name;

	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PIN_NUM);
	pEdit->SetLimitText(64);					//0.81.01
	if(m_block>0){
		pEdit->SetWindowText(m_arrayNum[0]);
	}

	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_BLOCK);
	pEdit->SetWindowText(_T("1"));
		
	CSpinButtonCtrl* pSpin=(CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_BLOCK);
	pSpin->SetRange(1,m_block);
	if(m_block<=1) pSpin->EnableWindow(FALSE);
	else	 pSpin->EnableWindow(TRUE);
		
	CButton* pButton;
	pButton = (CButton*)GetDlgItem(IDC_CHECK_HIDE_NUM);
	pButton->SetCheck(m_hideNum!=false);

	if(_tcslen(m_name)==0){
		pButton = (CButton*)GetDlgItem(IDC_BUTTON_SHOWHIDE_NAME);
		pButton->EnableWindow(FALSE);
	}



	CBitmap bm;
	pButton = (CButton*)GetDlgItem(IDC_RADIO_NORM);
	pButton->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDB_BMP_PIN_NORM)));
	pButton->SetCheck(m_pinType == PIN_TYPE_NOMAL);

	pButton = (CButton*)GetDlgItem(IDC_RADIO_CK);
	pButton->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDB_BMP_PIN_CK)));
	pButton->SetCheck(m_pinType == PIN_TYPE_CLOCK);

	pButton = (CButton*)GetDlgItem(IDC_RADIO_NEGA);
	pButton->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDB_BMP_PIN_NEGA)));
	pButton->SetCheck(m_pinType == PIN_TYPE_NEGATIVE);

	pButton = (CButton*)GetDlgItem(IDC_RADIO_NEGACK);
	pButton->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDB_BMP_PIN_NEGACK)));
	pButton->SetCheck(m_pinType == (PIN_TYPE_NEGATIVE | PIN_TYPE_CLOCK));

	pButton = (CButton*)GetDlgItem(IDC_RADIO_SMALL);
	pButton->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDB_BMP_PIN_SMALL)));
	pButton->SetCheck(m_pinType == PIN_TYPE_SMALL);

	pButton = (CButton*)GetDlgItem(IDC_RADIO_ZLENG);
	pButton->SetBitmap(::LoadBitmap(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDB_BMP_PIN_ZLENG)));
	pButton->SetCheck(m_pinType == PIN_TYPE_ZLENG);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CPinProperty::OnEnChangeEditBlock()
{
	// TODO :  これが RICHEDIT コントロールの場合、まず、CDialog::OnInitDialog() 関数をオーバーライドして、
	// OR 状態の ENM_CORRECTTEXT フラグをマスクに入れて、
	// CRichEditCtrl().SetEventMask() を呼び出さない限り、
	// コントロールは、この通知を送信しません。

	// TODO :  ここにコントロール通知ハンドラ コードを追加してください。
	TRACE("void CPinProperty::OnEnChangeEditBlock()\n");

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_BLOCK);
	CString strBlock;
	pEdit->GetWindowText(strBlock);
	int newBlock = _tstoi(strBlock) -1;
	if(newBlock<0) newBlock = 0;
	else if(newBlock >= m_block) newBlock = m_block -1;
	if(m_currentBlock == newBlock) return;

	CString strNum;
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PIN_NUM);
	pEdit->GetWindowText(strNum);
	if(strNum != m_arrayNum[m_currentBlock]){
		m_arrayNum[m_currentBlock] = strNum;
	}
	pEdit->SetWindowText(m_arrayNum[newBlock]);
	m_currentBlock = newBlock;
}

void CPinProperty::OnOK()
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	CEdit* pEdit;

	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PIN_NAME);
	pEdit->GetWindowText(m_name);

	if(m_name_old != m_name){
		m_pinNameDispFlag = 0xFFFFFFFF;
	}

	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_BLOCK);
	CString strBlock;
	pEdit->GetWindowText(strBlock);
	int block = _tstoi(strBlock) -1;
	if(block<0) block = 0;
	else if(block >= m_block) block = m_block -1;

	CString strNum;
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PIN_NUM);
	pEdit->GetWindowText(strNum);
	if(strNum != m_arrayNum[block]){
		m_arrayNum[block] = strNum;
	}

	CButton* pButton;
	pButton = (CButton*)GetDlgItem(IDC_CHECK_HIDE_NUM);
	m_hideNum = (pButton->GetCheck()!=0);

	if(((CButton*)GetDlgItem(IDC_RADIO_NORM))->GetCheck()){
		m_pinType = PIN_TYPE_NOMAL;
	}else if(((CButton*)GetDlgItem(IDC_RADIO_CK))->GetCheck()){
		m_pinType = PIN_TYPE_CLOCK;
	}else if(((CButton*)GetDlgItem(IDC_RADIO_NEGA))->GetCheck()){
		m_pinType = PIN_TYPE_NEGATIVE;
	}else if(((CButton*)GetDlgItem(IDC_RADIO_NEGACK))->GetCheck()){
		m_pinType = (PIN_TYPE_NEGATIVE | PIN_TYPE_CLOCK);
	}else if(((CButton*)GetDlgItem(IDC_RADIO_SMALL))->GetCheck()){
		m_pinType = PIN_TYPE_SMALL;
	}else{	//if(((CButton*)GetDlgItem(IDC_RADIO_ZLENG))->GetCheck()){
		m_pinType = PIN_TYPE_ZLENG;
	}
	CDialog::OnOK();
}


void CPinProperty::OnBnClickedButtonShowhideName()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if(m_name_old != m_name){
		m_pinNameDispFlag = 0xFFFFFFFF;
		m_name_old = m_name;
	}
	CDlgPinNameShowHide dlg;
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PIN_NAME);
	pEdit->GetWindowText(m_name);
	dlg.m_strName = m_name;
	dlg.m_displayFlag = m_pinNameDispFlag;
	if(dlg.DoModal()==IDOK){
		m_pinNameDispFlag = dlg.m_displayFlag;
	}

}


void CPinProperty::OnEnChangeEditPinName()
{
	// TODO:  これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、CDialog::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO:  ここにコントロール通知ハンドラー コードを追加してください。



	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PIN_NAME);
	pEdit->GetWindowText(m_name);
	CButton* pButton = (CButton*)GetDlgItem(IDC_BUTTON_SHOWHIDE_NAME);
	pButton->EnableWindow(_tcslen(m_name)==0? FALSE:TRUE);
}
