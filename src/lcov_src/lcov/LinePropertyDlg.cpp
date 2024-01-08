// LinePropertyDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "LCoV.h"
#include "LinePropertyDlg.h"
#include "afxdialogex.h"


// CLinePropertyDlg ダイアログ

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


// CLinePropertyDlg メッセージ ハンドラー


BOOL CLinePropertyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください

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
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CLinePropertyDlg::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	CComboBox* pCombo =(CComboBox*)GetDlgItem(IDC_COMBO_WIDTH);
	m_width = pCombo->GetCurSel();
	if(m_width>5){
		m_width = 5;
	}else if(m_width<0){
		m_width = 0;
	}

	CDialog::OnOK();
}
