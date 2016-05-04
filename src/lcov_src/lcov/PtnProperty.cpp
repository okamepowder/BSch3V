/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

// PtnProperty.cpp : 実装ファイル
//

#include "stdafx.h"
#include "LCoV.h"
#include "PtnProperty.h"
#include ".\ptnproperty.h"


// CPtnProperty ダイアログ

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
	// オートメーション オブジェクトに対する最後の参照が解放されるときに
	// OnFinalRelease が呼び出されます。基本クラスは自動的にオブジェク
	// トを削除します。基本クラスを呼び出す前に、オブジェクトで必要な特
	// 別な後処理を追加してください。

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

// メモ : VBA からタイプ セーフな連結をサポートするために、IID_ILCoV のサポートを追加します。
//  この IID は、.IDL ファイルのディスパッチ インターフェイスへアタッチされる 
//  GUID と一致しなければなりません。

// {A9544089-6C40-4280-A9FA-E1AFA8616A23}
static const IID IID_IPtnProperty =
{ 0xA9544089, 0x6C40, 0x4280, { 0xA9, 0xFA, 0xE1, 0xAF, 0xA8, 0x61, 0x6A, 0x23 } };

BEGIN_INTERFACE_MAP(CPtnProperty, CDialog)
	INTERFACE_PART(CPtnProperty, IID_IPtnProperty, Dispatch)
END_INTERFACE_MAP()


// CPtnProperty メッセージ ハンドラ

BOOL CPtnProperty::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO :  ここに初期化を追加してください
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
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CPtnProperty::OnOK()
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	CComboBox* pCombo =(CComboBox*)GetDlgItem(IDC_COMBO_WIDTH);
	m_width = pCombo->GetCurSel();
	if(m_width>5){
		m_width = 5;
	}else if(m_width<0){
		m_width = 0;
	}

	CDialog::OnOK();
}
