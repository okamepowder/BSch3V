/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// SelPart.cpp : インプリメンテーション ファイル
//
//***注意***
//部品ライブラリのリストボックスが非ソートの属性であることを前提にしている。

#include "stdafx.h"
#include "BSch.h"
#include <assert.h>
#include <string>
#include <list>
using namespace std;

#include "complib.h"
#include "Preview.h"
#include "SelPart.h"

#include "xbschglobal.h"
#include <io.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelPart ダイアログ


CSelPart::CSelPart(CWnd* pParent /*=NULL*/)
	: CDialog(CSelPart::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelPart)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
	m_nLastSelectPart=0;
	m_nLastSelectLibrary=0;
	m_pwndPreview=NULL;
	m_bUsePreview=TRUE;
	m_bInhibitUpdatePartName = FALSE;
	m_bInhibitUpdateListbox = FALSE;
	m_pCompIndex = NULL;
}


void CSelPart::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelPart)
                // メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelPart, CDialog)
	//{{AFX_MSG_MAP(CSelPart)
	ON_LBN_SELCHANGE(IDC_LIST_LIBRARY, OnSelchangeListLibrary)
	ON_LBN_SELCHANGE(IDC_LIST_PARTNAME, OnSelchangeListPartname)
	ON_LBN_DBLCLK(IDC_LIST_PARTNAME, OnDblclkListPartname)
	ON_EN_CHANGE(IDC_EDIT_PARTNAME, OnChangeEditPartname)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//指定したライブラリの部品をリストボックスに設定する
void CSelPart::SetPartName(int nLib)
{
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIST_PARTNAME);
	pLB->ShowWindow(SW_HIDE);
	//内容の全消去
	int n=pLB->GetCount();			//登録済みの数を得る
	while(n) n=pLB->DeleteString(0);//残りがある限り先頭の文字列を消去
	m_nPartCount=0;
	SCompIndex* pPartIndex;
	const SCompLib* pCompLib = g_LibraryInfo(nLib);
	if(pCompLib){
		m_nPartCount=pCompLib->count();	//ライブラリに登録されている部品数
		for(n=0;n<m_nPartCount;n++){
			pPartIndex=pCompLib->compIndex(n);
			pLB->AddString(pPartIndex->name());
		}
		//設定した部品数が、前回選択した部品の順序番号を満たさない場合は
		//前回の選択をクリアする。
		if(m_nPartCount-1< m_nLastSelectPart){
			m_nLastSelectPart=0;
		}
		pLB->SetCurSel(m_nLastSelectPart);	//前回選択したものにカーソルを置く
		OnSelchangeListPartname(); 
//		CString rStrPartName;
//		pLB->GetText(m_nLastSelectPart,rStrPartName);
//		SetEditPartName(rStrPartName);
		if(m_pwndPreview && !m_bInhibitUpdatePartName)  //プレビューの更新
		{
			int nSel=m_nLastSelectPart;
			SCompIndex* pPartIndex=pCompLib->compIndex(nSel);
			m_pwndPreview->SetPartIndex(pPartIndex);
		}
	}else{//指定したライブラリが無効だったときの処理
	//		m_nLastSelectPart=0;
	}
	pLB->ShowWindow(SW_SHOW);
}

void CSelPart::AutoUpdate()
{
	int n;
	int count=g_LibraryCount();
	for(n=0;n<count;n++){
		const SCompLib* pCompLib = g_LibraryInfo(n);
		_wfinddata64i32_t findData;
		if(_tfindfirst(pCompLib->m_lpszLibraryName,&findData)==-1) continue;
		int wr_time = findData.time_write;
		if(wr_time <= pCompLib->m_aux1){
			continue;
		}else{
			CString strPathname = pCompLib->m_lpszLibraryName;
			SCompLib* pCompLib=new SCompLib;
			if(!pCompLib->readLibraryFile(strPathname)){
				delete pCompLib;
			}else{
				if(!g_ReplaceLibrary(n,pCompLib)){
					delete pCompLib;
				}else{
					TRACE("Update library - %s\n",strPathname);
				}
			}
		}
	}
}


//ライブラリの名前をリストボックスに設定する
//表示をライブラリファイルのパスから名前だけに変更  1997/01/21
void CSelPart::SetLibraryName()
{
	TCHAR szLibPath[_MAX_PATH];
	TCHAR szLibName[_MAX_FNAME];
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIST_LIBRARY);
	int n=pLB->GetCount();			//登録済みの数を得る
	while(n) n=pLB->DeleteString(0);//残りがある限り先頭の文字列を消去
	for(n=0;n<MAX_LIB;n++){
		const SCompLib* pCompLib = g_LibraryInfo(n);
		if(pCompLib==NULL) break;
		_tcscpy(szLibPath,pCompLib->m_lpszLibraryName);
		_tsplitpath(szLibPath,NULL,NULL,szLibName,NULL);
		pLB->AddString(szLibName);
	}
	m_nLibraryCount=n;
	//設定したライブラリ数が、前回選択したライブラリ番号を満たさない場合は
	if(m_nLibraryCount-1<m_nLastSelectLibrary){
		m_nLastSelectLibrary=0;
		m_nLastSelectPart=0;
	}
	pLB->SetCurSel(m_nLastSelectLibrary);
}

//エディットコントロールにパーツ名を設定
void CSelPart::SetEditPartName(LPCTSTR psz)
{
	m_bInhibitUpdateListbox = TRUE;
	if(!m_bInhibitUpdatePartName){
		CEdit* pED=(CEdit*)GetDlgItem(IDC_EDIT_PARTNAME);
		pED->SetWindowText(psz);
	}

	m_rStrPartName = psz;
	m_rStrPartName.MakeUpper();
	m_bInhibitUpdateListbox = FALSE;

}

/////////////////////////////////////////////////////////////////////////////
// CSelPart メッセージ ハンドラ

BOOL CSelPart::OnInitDialog() 
{
	CDialog::OnInitDialog();

	AutoUpdate();
	
	// TODO: この位置に初期化の補足処理を追加してください
	//プレビューを使うときには、ダイアログの幅を広げて、そこにCPreviewのウインドウを
	//CreateExする。
	if(m_bUsePreview){
		m_pwndPreview=new CPreview;
		GetWindowRect(&m_rcDialog);
		GetClientRect(&m_rcClient);
		int nHeight=m_rcClient.bottom;
		MoveWindow( m_rcDialog.left,m_rcDialog.top,
					m_rcDialog.right-m_rcDialog.left+nHeight,
					m_rcDialog.bottom-m_rcDialog.top);
		CString strClassName=AfxRegisterWndClass(
			CS_VREDRAW|CS_HREDRAW,
			AfxGetApp()->LoadStandardCursor(IDC_ARROW), 
			NULL);

//			(HBRUSH)GetStockObject(WHITE_BRUSH));
		m_pwndPreview->CreateEx(
			WS_EX_CLIENTEDGE,
			strClassName,
			_T("Preview"),			//要らない
			WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL,
			m_rcClient.right,
			6,
			nHeight-6,
			nHeight-12,
			this->m_hWnd,
			(HMENU)IDC_PREVIEW);
	}
	//部品名エディットボックスの文字数制限
	CEdit* pED=(CEdit*)GetDlgItem(IDC_EDIT_PARTNAME);
	pED->SetLimitText(32);
	SetLibraryName();
	SetPartName(m_nLastSelectLibrary);
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

//ライブラリの選択状態が変わったときのハンドラ
void CSelPart::OnSelchangeListLibrary() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIST_LIBRARY);
	m_nLastSelectLibrary=pLB->GetCurSel();
	m_nLastSelectPart=0;	//ライブラリの選択が変われば前回選択パーツも０クリア
	SetPartName(m_nLastSelectLibrary);
}

//部品の選択状態が変わったときのハンドラ
void CSelPart::OnSelchangeListPartname() 
{
	int nSel;
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIST_PARTNAME);
	nSel=pLB->GetCurSel();
	//g_pPartLib[m_nLastSelectLibrary]->m_nLastSelectIndex=nSel;
	m_nLastSelectPart=nSel;
	CString rStrPartName;
	pLB->GetText(nSel,rStrPartName);
	SetEditPartName(rStrPartName);
	if(m_pwndPreview) //プレビューの更新
	{
		//SCompIndex* pPartIndex=g_LibraryInfo(n)
		const SCompLib* pCompLib = g_LibraryInfo(m_nLastSelectLibrary);
		m_pCompIndex=pCompLib->compIndex(nSel);
		//CPartIndex* pPartIndex=g_pPartLib[m_nLastSelectLibrary]->GetPartIndex(nSel);
		m_pwndPreview->SetPartIndex(m_pCompIndex);
	}
	if(m_pCompIndex->m_pCompInfo){
		SetDlgItemText(IDC_EDIT_MFR2,m_pCompIndex->Mfr());
		SetDlgItemText(IDC_EDIT_MFR_PNUM2,m_pCompIndex->MfrPn());
		SetDlgItemText(IDC_EDIT_PKG2,m_pCompIndex->Pkg());
		CString note;
		switch(m_pCompIndex->m_pCompInfo->m_ciType){
			case SCompInfo::CITYPE::FIXED_BLOCK:
				note.LoadString(IDS_FIXED_BLOCK);
				break;
			case SCompInfo::CITYPE::SHARED_BLOCK:
				note.LoadString(IDS_SHARED_BLOCK);
				break;
		}
		note.Append(m_pCompIndex->m_pCompInfo->m_note.c_str());
		SetDlgItemText(IDC_EDIT_NOTE,note);
	}
}

void CSelPart::OnCancel() 
{
	// TODO: この位置に特別な後処理を追加してください。
	
	CDialog::OnCancel();
}

void CSelPart::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
//	CEdit* pED=(CEdit*)GetDlgItem(IDC_EDIT_PARTNAME);
//	pED->GetWindowText(m_rStrPartName);
//	m_rStrPartName.MakeUpper();	//大文字に変換 1997/01/25
	CDialog::OnOK();
}

//部品選択リストボックスのダブルクリックで選択終了 1997/01/25
void CSelPart::OnDblclkListPartname() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	OnOK();
}

void CSelPart::OnChangeEditPartname() 
{
	// TODO: これが RICHEDIT コントロールの場合、コントロールは、 lParam マスク
	// 内での論理和の ENM_CHANGE フラグ付きで CRichEditCrtl().SetEventMask()
	// メッセージをコントロールへ送るために CDialog::OnInitDialog() 関数をオーバー
	// ライドしない限りこの通知を送りません。
	
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	//static int debug_val = 0;
	//debug_val++;
	//TRACE("CSelPart::OnChangeEditPartname()  %d\n",debug_val);
	//return;

	if(m_bInhibitUpdateListbox)return;

	CEdit* pED=(CEdit*)GetDlgItem(IDC_EDIT_PARTNAME);
	pED->GetWindowText(m_rStrPartName);
	m_rStrPartName.MakeUpper();

	int nLib;
	int nIndex;
	
	m_pCompIndex = g_SearchComponentIndex(m_rStrPartName,&nLib,&nIndex,NULL);
	if(m_pCompIndex){
		m_bInhibitUpdatePartName = TRUE;

		m_nLastSelectLibrary = nLib;
		//OnSelchangeListPartname();
		m_nLastSelectPart  = nIndex;

		CListBox* pLBLib=(CListBox*)GetDlgItem(IDC_LIST_LIBRARY);
		pLBLib->SetCurSel(m_nLastSelectLibrary);
		SetPartName(m_nLastSelectLibrary);
		CListBox* pLBPart=(CListBox*)GetDlgItem(IDC_LIST_PARTNAME);
		pLBPart->SetCurSel(m_nLastSelectPart);

		if(m_pwndPreview){ //プレビューの更新
			m_pwndPreview->SetPartIndex(m_pCompIndex);
		}
		if(m_pCompIndex->m_pCompInfo){
			SetDlgItemText(IDC_EDIT_MFR2,m_pCompIndex->Mfr());
			SetDlgItemText(IDC_EDIT_MFR_PNUM2,m_pCompIndex->MfrPn());
			SetDlgItemText(IDC_EDIT_PKG2,m_pCompIndex->Pkg());
			CString note;
			switch(m_pCompIndex->m_pCompInfo->m_ciType){
				case SCompInfo::CITYPE::FIXED_BLOCK:
					note.LoadString(IDS_FIXED_BLOCK);
					break;
				case SCompInfo::CITYPE::SHARED_BLOCK:
					note.LoadString(IDS_SHARED_BLOCK);
					break;
			}
			note.Append(m_pCompIndex->m_pCompInfo->m_note.c_str());
			SetDlgItemText(IDC_EDIT_NOTE,note);
		}
		m_bInhibitUpdatePartName = FALSE;
	}


}
