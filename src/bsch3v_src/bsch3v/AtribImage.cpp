// AtribImage.cpp : 実装ファイル
//

#include "stdafx.h"
#include "BSch.h"
#include "AtribImage.h"


// CAtribImage ダイアログ

IMPLEMENT_DYNAMIC(CAtribImage, CDialog)

CAtribImage::CAtribImage(CWnd* pParent /*=NULL*/)
	: CDialog(CAtribImage::IDD, pParent)
{
	m_magnification = 100;
}

CAtribImage::~CAtribImage()
{
}

void CAtribImage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAtribImage, CDialog)
END_MESSAGE_MAP()


// CAtribImage メッセージ ハンドラ

BOOL CAtribImage::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	SetDlgItemInt(IDC_EDIT_MAG,m_magnification);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CAtribImage::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	int n = GetDlgItemInt(IDC_EDIT_MAG);
	if(n<20 || n>200){
		AfxMessageBox(IDS_INVALID_IMAGE_MAGNIFICATION,MB_OK|MB_ICONEXCLAMATION);
	}else{
		m_magnification = n;
		CDialog::OnOK();
	}
}
