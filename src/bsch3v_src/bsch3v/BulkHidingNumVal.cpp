// BulkHidingNumVal.cpp : 実装ファイル
//

#include "stdafx.h"
#include "BSch.h"
#include "BulkHidingNumVal.h"


// CBulkHidingNumVal ダイアログ

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

	// TODO:  ここに初期化を追加してください
	CButton* pCheck;
	pCheck = (CButton*)GetDlgItem(IDC_CHECK_HIDENUM);
	pCheck->SetCheck(BST_INDETERMINATE);
	pCheck = (CButton*)GetDlgItem(IDC_CHECK_HIDEVAL);
	pCheck->SetCheck(BST_INDETERMINATE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CBulkHidingNumVal::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	CButton* pCheck;
	pCheck = (CButton*)GetDlgItem(IDC_CHECK_HIDENUM);
	m_bHideNumCheckState =  pCheck->GetCheck();
	pCheck = (CButton*)GetDlgItem(IDC_CHECK_HIDEVAL);
	m_bHideValCheckState =  pCheck->GetCheck();
	CDialog::OnOK();
}
