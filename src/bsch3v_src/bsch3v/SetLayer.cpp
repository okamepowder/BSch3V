/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// SetLayer.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "bsch.h"
#include "SetLayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetLayer ダイアログ


CSetLayer::CSetLayer(CWnd* pParent /*=NULL*/)
	: CDialog(CSetLayer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetLayer)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
	m_wShow=0xFFFF;
	m_nEdit=0;
	m_fEditHigh=FALSE;
}


void CSetLayer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetLayer)
                // メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetLayer, CDialog)
	//{{AFX_MSG_MAP(CSetLayer)
	ON_BN_CLICKED(IDC_D0, OnD0)
	ON_BN_CLICKED(IDC_D1, OnD1)
	ON_BN_CLICKED(IDC_D2, OnD2)
	ON_BN_CLICKED(IDC_D3, OnD3)
	ON_BN_CLICKED(IDC_D4, OnD4)
	ON_BN_CLICKED(IDC_D5, OnD5)
	ON_BN_CLICKED(IDC_D6, OnD6)
	ON_BN_CLICKED(IDC_D7, OnD7)
	ON_BN_CLICKED(IDC_E0, OnE0)
	ON_BN_CLICKED(IDC_E1, OnE1)
	ON_BN_CLICKED(IDC_E2, OnE2)
	ON_BN_CLICKED(IDC_E3, OnE3)
	ON_BN_CLICKED(IDC_E4, OnE4)
	ON_BN_CLICKED(IDC_E5, OnE5)
	ON_BN_CLICKED(IDC_E6, OnE6)
	ON_BN_CLICKED(IDC_E7, OnE7)
	ON_BN_CLICKED(IDC_CHECK_EDIT_HIGHLIGHT, OnCheckEditHighlight)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_DIPLAY_NAME_ON_BAR, &CSetLayer::OnClickedCheckDiplayNameOnBar)
END_MESSAGE_MAP()


WORD CSetLayer::GetBit(int nLayer)
{
	static WORD table[]={
		0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80
	};
	if(nLayer<0 || nLayer>7) return 1;
	return table[nLayer];
}



/////////////////////////////////////////////////////////////////////////////
// CSetLayer メッセージ ハンドラ

BOOL CSetLayer::OnInitDialog() 
{
	CDialog::OnInitDialog();


	// TODO: この位置に初期化の補足処理を追加してください
	WORD wBit;
	int  n;
	int  nID;

	if(m_wShow==0) m_wShow=0xFF;
	if(!(m_wShow & GetBit(m_nEdit))){
		wBit=0x01;
		n=0;
		while(!(wBit & m_wShow)){ wBit<<=1; n++; }
		m_nEdit=n;
	}
	//IDC_D0～IDC_D7は連続した値になるようにリソースエディタで設定してある
	//IDC_E0～IDC_E7は連続した値になるようにリソースエディタで設定してある
	//IDC_N0～IDC_N7は連続した値になるようにリソースエディタで設定してある
	CButton* pButton;
	nID=IDC_D0;
	wBit=1;
	for(n=0;n<8;n++){
		pButton=(CButton*)GetDlgItem(nID);
		pButton->SetCheck((wBit & m_wShow)!=0);
		nID++;
		wBit<<=1;
	}
	nID=IDC_E0;
	for(n=0;n<8;n++){
		pButton=(CButton*)GetDlgItem(nID);
		pButton->SetCheck(n==m_nEdit);
		nID++;
	}
	pButton=(CButton*)GetDlgItem(IDC_CHECK_EDIT_HIGHLIGHT);
	pButton->SetCheck(m_fEditHigh!=FALSE);

	pButton = (CButton*)GetDlgItem(IDC_CHECK_DIPLAY_NAME_ON_BAR);
	pButton->SetCheck(m_fDisplayNameOnDlgBar);


	CEdit* pEdit;
	nID = IDC_N0;
	for (int i = 0; i < 8; i++) {
		pEdit = (CEdit*)GetDlgItem(nID);
		pEdit->SetWindowText(m_strName[i]);
		nID++;
	}


	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CSetLayer::OnDCheck(int n)
{
	CButton* pButton=(CButton*)GetDlgItem(IDC_D0+n);
	if(pButton->GetCheck()){	//現在がチェック→チェックをOFFにする。現在の編集対象のレイヤーはOFFにできない。
		CButton* pEButton=(CButton*)GetDlgItem(IDC_E0+n);
		if(!pEButton->GetCheck())	pButton->SetCheck(0);
	}else{
		pButton->SetCheck(1);
	}
}

void CSetLayer::OnECheck(int n)
{
	CButton* pButton=(CButton*)GetDlgItem(IDC_D0+n);
	pButton->SetCheck(1);	//該当レイヤーは強制的に表示状態にする
	int i;
	for(i=0;i<8;i++){
		CButton* pEButton=(CButton*)GetDlgItem(IDC_E0+i);
		pEButton->SetCheck(i==n);
	}
}


void CSetLayer::OnD0() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	OnDCheck(0);
}

void CSetLayer::OnD1() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	OnDCheck(1);
}

void CSetLayer::OnD2() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	OnDCheck(2);
}

void CSetLayer::OnD3() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	OnDCheck(3);
}

void CSetLayer::OnD4() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	OnDCheck(4);
}

void CSetLayer::OnD5() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	OnDCheck(5);
}

void CSetLayer::OnD6() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	OnDCheck(6);
}

void CSetLayer::OnD7() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	OnDCheck(7);
}

void CSetLayer::OnE0() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	OnECheck(0);
}

void CSetLayer::OnE1() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	OnECheck(1);
}

void CSetLayer::OnE2() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	OnECheck(2);
}

void CSetLayer::OnE3() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	OnECheck(3);
}

void CSetLayer::OnE4() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	OnECheck(4);
}

void CSetLayer::OnE5() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	OnECheck(5);
}

void CSetLayer::OnE6() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	OnECheck(6);
}

void CSetLayer::OnE7() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	OnECheck(7);
}

void CSetLayer::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください

	//IDC_D0～IDC_D7は連続した値になるようにリソースエディタで設定してある
	//IDC_E0～IDC_E7は連続した値になるようにリソースエディタで設定してある
	//IDC_N0～IDC_N7は連続した値になるようにリソースエディタで設定してある
	int n;
	CButton* pButton;
	WORD wBit=1;
	m_wShow=0;
	for(n=0;n<8;n++){
		pButton=(CButton*)GetDlgItem(IDC_D0+n);
		if(pButton->GetCheck()) m_wShow |= wBit;
		wBit<<=1;
	}
	for(n=0;n<8;n++){
		pButton=(CButton*)GetDlgItem(IDC_E0+n);
		if(pButton->GetCheck()){
			m_nEdit=n;
			break;
		}
	}
	pButton=(CButton*)GetDlgItem(IDC_CHECK_EDIT_HIGHLIGHT);
	m_fEditHigh=pButton->GetCheck();

	pButton = (CButton*)GetDlgItem(IDC_CHECK_DIPLAY_NAME_ON_BAR);
	m_fDisplayNameOnDlgBar = pButton->GetCheck();


	CEdit* pEdit;
	int nID = IDC_N0;
	for (int i = 0; i < 8; i++) {
		pEdit = (CEdit*)GetDlgItem(nID);
		pEdit->GetWindowText(m_strName[i]);
		nID++;
	}



	CDialog::OnOK();
}

void CSetLayer::OnCheckEditHighlight() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CButton* pButton=(CButton*)GetDlgItem(IDC_CHECK_EDIT_HIGHLIGHT);
	pButton->SetCheck(pButton->GetCheck()==FALSE);
}


void CSetLayer::OnClickedCheckDiplayNameOnBar()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_DIPLAY_NAME_ON_BAR);
	pButton->SetCheck(pButton->GetCheck() == FALSE);
}
