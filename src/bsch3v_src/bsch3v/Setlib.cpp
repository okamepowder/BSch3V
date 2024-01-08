/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// SetLib.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <Shlwapi.h>
#include "BSch.h"
#include "SetLib.h"
#include "xbschglobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetLib ダイアログ


CSetLib::CSetLib(CWnd* pParent /*=NULL*/)
	: CDialog(CSetLib::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetLib)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CSetLib::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetLib)
                // メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetLib, CDialog)
	//{{AFX_MSG_MAP(CSetLib)
	ON_LBN_SELCHANGE(IDC_LIB_LIST, OnSelchangeLibList)
	ON_BN_CLICKED(IDC_LIB_UP, OnLibUp)
	ON_BN_CLICKED(IDC_LIB_DOWN, OnLibDown)
	ON_BN_CLICKED(IDC_LIB_ADD, OnLibAdd)
	ON_BN_CLICKED(IDC_LIB_DEL, OnLibDel)
	//}}AFX_MSG_MAP
	ON_WM_COPYDATA()
	ON_BN_CLICKED(IDC_LIB_ABS, &CSetLib::OnClickedLibAbs)
	ON_BN_CLICKED(IDC_LIB_REL, &CSetLib::OnClickedLibRel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetLib メンバー関数インプリメント

//ダイアログ構成ボタンのイネーブル
void CSetLib::EnableButton(UINT id,BOOL bEnable)
{
	CButton*  pBtn=(CButton*)GetDlgItem(id);
	// ディセーブルしようとするボタンにフォーカスがある場合は
	// ＯＫボタンにフォーカスをセットする
	if(!bEnable){
		CWnd* pWnd=GetFocus();
		if(pWnd==pBtn){
			CButton* pDefaultBtn=(CButton*)GetDlgItem(IDOK);
			pDefaultBtn->SetFocus();
		}
	}
	pBtn->EnableWindow(bEnable);
}

void CSetLib::SetListboxWidth()
{
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIB_LIST);
	CString rString;
	CDC* pDC;
	CSize textsize;
	int maxwidth=0;
	int i,n;
	pDC = pLB->GetDC();

	n = pLB->GetCount();
	for(i=0;i<n;i++){
		pLB->GetText(i,rString);
		textsize = pDC->GetTextExtent(rString);
		if(textsize.cx>maxwidth) maxwidth = textsize.cx;
	}
	pLB->SetHorizontalExtent(maxwidth);
	pLB->ReleaseDC(pDC);
}



/////////////////////////////////////////////////////////////////////////////
// CSetLib メッセージ ハンドラ

//WM_INITDIALOG メッセージハンドラ
BOOL CSetLib::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIB_LIST);
	CString rString;
//	CDC* pDC;
	CSize textsize;
	TCHAR str[32];
	int i;
//	int maxwidth=0;



//	pDC = pLB->GetDC();
	//レジストリに登録されたライブラリファイルをリストボックスへ設定
	for(i=0;i<MAX_LIB;i++){
		wsprintf(str,_T("LIB%d"),i);			//エントリ文字列の作成
		rString=AfxGetApp()->GetProfileString(_T("Library"),str);
		if(rString.IsEmpty()) break;		//文字列（ライブラリ名）が獲得できなかったらbreak
		pLB->InsertString(-1,rString);		//リストボックスの最後に追加
//		textsize = pDC->GetTextExtent(rString);
//		if(textsize.cx>maxwidth) maxwidth = textsize.cx;
	}
	SetListboxWidth();


	m_nList=pLB->GetCount();				//登録数の設定
	EnableButton(IDC_LIB_ADD,m_nList<MAX_LIB);	//まだ登録数上限でなければ追加可能なように「ADD」をイネーブル
		
	if(m_nList!=0){					//登録数が０でなければ
		pLB->SetCurSel(m_nList-1);			//リストボックスの一番下の行にカーソル設定
	}
	EnableButtons(m_nList-1);


	//EnableButton(IDC_LIB_DOWN,FALSE);		//「下へ移動」ボタンはディセーブル
	//if(m_nList!=0){					//登録数が０でなければ
	//	pLB->SetCurSel(m_nList-1);			//リストボックスの一番下の行にカーソル設定
	//}else{							//登録数が０ならば
	//	EnableButton(IDC_LIB_UP,  FALSE);	//「上へ移動」も「削除」もディセーブル
	//	EnableButton(IDC_LIB_DEL, FALSE);
	//}




	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

//IDOK(OKボタン)の処理
//レジストリにリストボックスの内容を書き込む
void CSetLib::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIB_LIST);
	CString rString;
	TCHAR str[32];
	int i,n;

	n=pLB->GetCount();							//リストボックスへの登録数の取得
	for(i=0;i<MAX_LIB;i++){
		if(i<n) pLB->GetText(i,rString);		//指定行のライブラリ名の取得
		else	rString.Empty();				//登録数を超える場合は空文字列にする
		wsprintf(str,_T("LIB%d"),i);				//エントリ文字列の作成
		AfxGetApp()->WriteProfileString(_T("Library"),str,rString);//レジストリへの書き込み
	}
	CDialog::OnOK();
}


void CSetLib::EnableButtons(int n)
{
	EnableButton(IDC_LIB_DOWN,n>=0 && n<m_nList-1);		//「下へ」ボタンの設定
	EnableButton(IDC_LIB_UP  ,n>0);						//「上へ」ボタンの設定
	
	if(n>=0){
		CString path;
		CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIB_LIST);
		pLB->GetText(n,path);
		BOOL pathRel = ::PathIsRelative(path);
		EnableButton(IDC_LIB_ABS,pathRel);
		EnableButton(IDC_LIB_REL,!pathRel);
	}else{
		EnableButton(IDC_LIB_ABS,FALSE);
		EnableButton(IDC_LIB_REL,FALSE);
	}
}


//ライブラリ名を保存するリストボックスで、カーソルが移動（選択されているもの）
//が変った場合の処理
//「上へ移動」「下へ移動」のボタンのイネーブル設定
void CSetLib::OnSelchangeLibList() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIB_LIST);
	int n=pLB->GetCurSel();								//選択されている位置の取得
 	EnableButtons(n);
}


//ライブラリ名を保存するリストボックスで、選択されているものを「上へ移動」する
//現在の位置からいったん削除して、一つ前に挿入する
void CSetLib::OnLibUp() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIB_LIST);
	CString rString;
	int n=pLB->GetCurSel();			//選択されている位置の取得
	if(n==LB_ERR || n==0) return;	//選択されているものがないか既に先頭の場合は何もしない
	pLB->GetText(n,rString);		//選択されている文字列を取得して…
	pLB->DeleteString(n);			//			リストボックスから削除
	n--;							//順序番号を上げて…
	pLB->InsertString(n,rString);	//			追加して…
	pLB->SetCurSel(n);				//			カーソルを設定して選択状態にする
	EnableButton(IDC_LIB_DOWN,n<m_nList-1);		//「下へ」ボタンの設定
	EnableButton(IDC_LIB_UP  ,n>0);				//「上へ」ボタンの設定
}

//ライブラリ名を保存するリストボックスで、選択されているものを「下へ移動」する
//現在の位置からいったん削除して、一つ前に挿入する
void CSetLib::OnLibDown() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIB_LIST);
	CString rString;
	int n=pLB->GetCurSel();			//選択されている位置の取得
	if(n==LB_ERR || n>=m_nList-1) return;	//選択されているものがないか既に最後尾の場合は何もしない
	pLB->GetText(n,rString);		//選択されている文字列を取得して…
	pLB->DeleteString(n);			//			リストボックスから削除
	n++;							//順序番号を上げて…
	pLB->InsertString(n,rString);	//			追加して…
	pLB->SetCurSel(n);				//			カーソルを設定して選択状態にする
	EnableButton(IDC_LIB_DOWN,n<m_nList-1);		//「下へ」ボタンの設定
	EnableButton(IDC_LIB_UP  ,n>0);				//「上へ」ボタンの設定
}

//ライブラリへの追加
//ファイルダイアログを起動して(*.LIB)をピックアップして選択する
void CSetLib::OnLibAdd() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int nResult;
	if(m_nList>=MAX_LIB) return;	//すでに登録上限ならなにもしない

	TCHAR szFileNameBuffer[_MAX_PATH *MAX_LIB+256];
	_tcscpy(szFileNameBuffer,_T("*.LB3"));

	CFileDialog dlg(TRUE,				//オープンダイアログ
					NULL,				//拡張子が付けられなかった場合のデフォルト
					NULL,			//初期ファイル名
					OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT,
					_T("BSch Library(*.lb3)|*.LB3|BSch3 schematic(*.ce3)|*.CE3||"),	//フィルタ文字列
					this);				//親ウインドウ
	
	CString rCaption;
	rCaption.LoadString(IDS_LIB_ADD_NEW);//ダイアログボックスのキャプションをリソースからロード
	dlg.m_ofn.lpstrTitle=rCaption;		//ファイルダイアログクラスにキャプションの設定
	dlg.m_ofn.lpstrFile = szFileNameBuffer;	//ファイル名を格納するバッファを設定する。
	dlg.m_ofn.nMaxFile = _MAX_PATH *MAX_LIB+256;		//ファイル名を格納するバッファのサイズを設定する。
	
	nResult=dlg.DoModal();			//ファイルダイアログの実行
	if(nResult==IDOK){		//正常終了なら…
//		CString rString=CString(dlg.m_ofn.lpstrFile);		//ファイルダイアログからファイル名を取得して
		CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIB_LIST);	//リストボックスを取得して…
		int n;
		CString strPath;
		POSITION pos = dlg.GetStartPosition();
		while(pos){
			strPath = dlg.GetNextPathName(pos);				
			n=pLB->GetCurSel();				//現在選択されているところに挿入するために現在値取得
			if(n==LB_ERR) n=0;				//現在選択されているものがなければ先頭に入れる
			pLB->InsertString(n,strPath);	//リストボックスへのセット
			pLB->SetCurSel(n);				//選択行の再設定
			m_nList=pLB->GetCount();		//メンバー変数への登録数の設定
			if(m_nList>=MAX_LIB) break;
		}
		SetListboxWidth();
		EnableButton(IDC_LIB_ADD ,m_nList<MAX_LIB);//「ADD」ボタンの設定
		EnableButton(IDC_LIB_DOWN,n<m_nList-1);		//「下へ」ボタンの設定
		EnableButton(IDC_LIB_UP  ,n>0);				//「上へ」ボタンの設定
		EnableButton(IDC_LIB_DEL ,TRUE);			//「DEL」ボタンの設定
	}
}

//ライブラリの削除
void CSetLib::OnLibDel() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int n;
	if(m_nList==0) return;		//登録数が０なら何もしない
    CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIB_LIST);
	n=pLB->GetCurSel();			//選択されているものの番号を取得
	if(n==LB_ERR) return;		//選択されているものがなければ何もしない
	pLB->DeleteString(n); 		//選択されているものを削除
    m_nList=pLB->GetCount(); 	//登録数の設定
	if(n>=m_nList) n=m_nList-1;	//削除したのが最下行のものなら次に選択するのは一つ上
	if(n>=0)pLB->SetCurSel(n);
	SetListboxWidth();
	EnableButton(IDC_LIB_ADD, TRUE);
   	EnableButton(IDC_LIB_DOWN,n<m_nList-1 && m_nList>0);
   	EnableButton(IDC_LIB_UP,  n>0);
	EnableButton(IDC_LIB_DEL, m_nList>0);
	
}




void CSetLib::OnClickedLibAbs()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CString path;
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIB_LIST);
	int n=pLB->GetCurSel();
	pLB->GetText(n,path);
	BOOL pathRel = ::PathIsRelative(path);
	if(!pathRel) return;

	CString iniDir = ((CBSchApp*)AfxGetApp())->m_szIniFileDir;

	TCHAR fullPath[_MAX_PATH*2];	//ややこしい相対パスもあるだろうし。
	TCHAR clnFullPath[_MAX_PATH];
	::PathCombine(fullPath,iniDir,path);
	::PathCanonicalize(clnFullPath,fullPath);	//途中に相対が入った状態を整理する。

	pLB->DeleteString(n);				//			リストボックスから削除
	pLB->InsertString(n,clnFullPath);	//			追加して…
	pLB->SetCurSel(n);					//			カーソルを設定して選択状態にする

	EnableButtons(n);
}


void CSetLib::OnClickedLibRel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CString path;
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIB_LIST);
	int n=pLB->GetCurSel();
	pLB->GetText(n,path);
	BOOL pathisRel = ::PathIsRelative(path);
	if(pathisRel) return;

	TCHAR relPath[_MAX_PATH];

	::PathRelativePathTo(relPath, ((CBSchApp*)AfxGetApp())->m_szIniFileDir ,FILE_ATTRIBUTE_DIRECTORY,path,FILE_ATTRIBUTE_NORMAL);

	//TCHAR iniDir[_MAX_PATH];
	//::PathCanonicalize(iniDir,((CBSchApp*)AfxGetApp())->m_szIniFileDir); 
	//::PathRelativePathTo(relPath, iniDir ,FILE_ATTRIBUTE_DIRECTORY,path,FILE_ATTRIBUTE_NORMAL);

	pLB->DeleteString(n);				//			リストボックスから削除
	pLB->InsertString(n,relPath);		//			追加して…
	pLB->SetCurSel(n);					//			カーソルを設定して選択状態にする

	EnableButtons(n);

}
