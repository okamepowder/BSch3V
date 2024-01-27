// DispBlockNum.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DispBlockNum.h"
#include "afxdialogex.h"


// CDispBlockNum ダイアログ

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


// CDispBlockNum メッセージ ハンドラー


BOOL CDispBlockNum::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	CButton* pBtn;
	
	pBtn=(CButton*)GetDlgItem(IDC_RADIO_NOTDISPLAY);
	pBtn->SetCheck(m_dispBN == DISP_BLOCKNUM_TYPE::NOTDISPLAY_BLOCKNUM ? BST_CHECKED : BST_UNCHECKED);

	pBtn=(CButton*)GetDlgItem(IDC_RADIO_NUM);
	pBtn->SetCheck(m_dispBN == DISP_BLOCKNUM_TYPE::DISPLAY_BLOCKNUM_NUM ? BST_CHECKED : BST_UNCHECKED);

	pBtn=(CButton*)GetDlgItem(IDC_RADIO_ALP);
	pBtn->SetCheck(m_dispBN == DISP_BLOCKNUM_TYPE::DISPLAY_BLOCKNUM_ALPHA ? BST_CHECKED : BST_UNCHECKED);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CDispBlockNum::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
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
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	CDialog::OnCancel();
}
