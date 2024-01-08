// SetAddIn.cpp : 実装ファイル
//

#include "stdafx.h"
#include "nlist.h"
#include "SetAddIn.h"
#include "SAddInFiles.h"

// CSetAddIn ダイアログ

IMPLEMENT_DYNAMIC(CSetAddIn, CDialog)

CSetAddIn::CSetAddIn(CWnd* pParent /*=NULL*/)
	: CDialog(CSetAddIn::IDD, pParent)
{
	m_nAddinCount = 0;
}

CSetAddIn::~CSetAddIn()
{
}

void CSetAddIn::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CSetAddIn::SetListboxWidth()
{
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIST_FILES);
	CString str;
	CDC* pDC;
	CSize textsize;
	int maxwidth=0;
	int i,n;
	pDC = pLB->GetDC();

	n = pLB->GetCount();
	for(i=0;i<n;i++){
		pLB->GetText(i,str);
		textsize = pDC->GetTextExtent(str);
		if(textsize.cx>maxwidth) maxwidth = textsize.cx;
	}
	pLB->SetHorizontalExtent(maxwidth);
	pLB->ReleaseDC(pDC);
}

//ダイアログ構成ボタンのイネーブル
void CSetAddIn::EnableButton(UINT id,BOOL bEnable)
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


BEGIN_MESSAGE_MAP(CSetAddIn, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CSetAddIn::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, &CSetAddIn::OnBnClickedButtonRemove)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, &CSetAddIn::OnBnClickedButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_UP, &CSetAddIn::OnBnClickedButtonUp)
	ON_LBN_SELCHANGE(IDC_LIST_FILES, &CSetAddIn::OnLbnSelchangeListFiles)
END_MESSAGE_MAP()


// CSetAddIn メッセージ ハンドラ

BOOL CSetAddIn::OnInitDialog()
{
	CDialog::OnInitDialog();

	
	// TODO: この位置に初期化の補足処理を追加してください
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIST_FILES);
	CString str;
	CSize textsize;
	TCHAR szEntry[32];
	int i;


	//レジストリに登録されたライブラリファイルをリストボックスへ設定
	for(i=0;i<MAX_ADDIN;i++){
		wsprintf(szEntry,_T("ADDIN%d"),i);			//エントリ文字列の作成
		str=AfxGetApp()->GetProfileString(_T("ADDINLIB"),szEntry);
		if(str.IsEmpty()) break;		//文字列（アドインファイル名）が獲得できなかったらbreak
		pLB->InsertString(-1,str);		//リストボックスの最後に追加
	}
	SetListboxWidth();


	m_nAddinCount = pLB->GetCount();			//登録数の設定
	EnableButton(IDC_BUTTON_ADD,m_nAddinCount<MAX_ADDIN);	//まだ登録数上限でなければ追加可能なように「ADD」をイネーブル
		
	EnableButton(IDC_BUTTON_DOWN,FALSE);		//「下へ移動」ボタンはディセーブル
	if(m_nAddinCount!=0){						//登録数が０でなければ
		pLB->SetCurSel(m_nAddinCount-1);		//リストボックスの一番下の行にカーソル設定
	}else{										//登録数が０ならば
		EnableButton(IDC_BUTTON_UP,  FALSE);	//「上へ移動」も「削除」もディセーブル
		EnableButton(IDC_BUTTON_REMOVE, FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CSetAddIn::OnBnClickedButtonAdd()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
		int nResult;
		if(m_nAddinCount>=MAX_ADDIN) return;	//すでに登録上限ならなにもしない

	TCHAR szFileNameBuffer[_MAX_PATH *MAX_ADDIN+256];
	_tcscpy(szFileNameBuffer,_T("*.DLL"));

	CFileDialog dlg(TRUE,				//オープンダイアログ
					_T("dll"),				//拡張子が付けられなかった場合のデフォルト
					NULL,			//初期ファイル名
					OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT,
					_T("Add in (*.dll)|*.DLL||"),	//フィルタ文字列
					this);				//親ウインドウ
	
	CString strCaption;
	strCaption.LoadString(IDS_ADD_ADDIN);//ダイアログボックスのキャプションをリソースからロード
	dlg.m_ofn.lpstrTitle=strCaption;		//ファイルダイアログクラスにキャプションの設定
	dlg.m_ofn.lpstrFile = szFileNameBuffer;	//ファイル名を格納するバッファを設定する。
	dlg.m_ofn.nMaxFile = _MAX_PATH *MAX_ADDIN+256;		//ファイル名を格納するバッファのサイズを設定する。
	
	nResult=dlg.DoModal();			//ファイルダイアログの実行
	if(nResult==IDOK){		//正常終了なら…
//		CString rString=CString(dlg.m_ofn.lpstrFile);		//ファイルダイアログからファイル名を取得して
		CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIST_FILES);	//リストボックスを取得して…
		int n;
		CString strPath;
		POSITION pos = dlg.GetStartPosition();
		while(pos){
			strPath = dlg.GetNextPathName(pos);				
			n=pLB->GetCurSel();				//現在選択されているところに挿入するために現在値取得
			if(n==LB_ERR) n=0;				//現在選択されているものがなければ先頭に入れる
			pLB->InsertString(n,strPath);	//リストボックスへのセット
			pLB->SetCurSel(n);				//選択行の再設定
			m_nAddinCount=pLB->GetCount();		//メンバー変数への登録数の設定
			if(m_nAddinCount>=MAX_ADDIN) break;
		}
		SetListboxWidth();
		EnableButton(IDC_BUTTON_ADD ,m_nAddinCount<MAX_ADDIN);//「ADD」ボタンの設定
		EnableButton(IDC_BUTTON_DOWN,n<m_nAddinCount-1);		//「下へ」ボタンの設定
		EnableButton(IDC_BUTTON_UP  ,n>0);				//「上へ」ボタンの設定
		EnableButton(IDC_BUTTON_REMOVE ,TRUE);			//「DEL」ボタンの設定
	}
}

void CSetAddIn::OnBnClickedButtonRemove()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int n;
	if(m_nAddinCount==0) return;		//登録数が０なら何もしない
    CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIST_FILES);
	n=pLB->GetCurSel();			//選択されているものの番号を取得
	if(n==LB_ERR) return;		//選択されているものがなければ何もしない
	pLB->DeleteString(n); 		//選択されているものを削除
    m_nAddinCount=pLB->GetCount(); 	//登録数の設定
	if(n>=m_nAddinCount) n=m_nAddinCount-1;	//削除したのが最下行のものなら次に選択するのは一つ上
	if(n>=0)pLB->SetCurSel(n);
	SetListboxWidth();
	EnableButton(IDC_BUTTON_ADD, TRUE);
   	EnableButton(IDC_BUTTON_DOWN,n<m_nAddinCount-1 && m_nAddinCount>0);
   	EnableButton(IDC_BUTTON_UP,  n>0);
	EnableButton(IDC_BUTTON_REMOVE, m_nAddinCount>0);
}

void CSetAddIn::OnBnClickedButtonDown()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIST_FILES);
	CString str;
	int n=pLB->GetCurSel();						//選択されている位置の取得
	if(n==LB_ERR || n>=m_nAddinCount-1) return;	//選択されているものがないか既に最後尾の場合は何もしない
	pLB->GetText(n,str);						//選択されている文字列を取得して…
	pLB->DeleteString(n);						//			リストボックスから削除
	n++;										//順序番号を下げてげて…
	pLB->InsertString(n,str);					//			追加して…
	pLB->SetCurSel(n);							//			カーソルを設定して選択状態にする
	EnableButton(IDC_BUTTON_DOWN,n<m_nAddinCount-1);		//「下へ」ボタンの設定
	EnableButton(IDC_BUTTON_UP  ,n>0);				//「上へ」ボタンの設定
}

void CSetAddIn::OnBnClickedButtonUp()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIST_FILES);
	CString str;
	int n=pLB->GetCurSel();			//選択されている位置の取得
	if(n==LB_ERR || n==0) return;	//選択されているものがないか既に先頭の場合は何もしない
	pLB->GetText(n,str);		//選択されている文字列を取得して…
	pLB->DeleteString(n);			//			リストボックスから削除
	n--;							//順序番号を上げて…
	pLB->InsertString(n,str);	//			追加して…
	pLB->SetCurSel(n);				//			カーソルを設定して選択状態にする
	EnableButton(IDC_BUTTON_DOWN,n<m_nAddinCount-1);		//「下へ」ボタンの設定
	EnableButton(IDC_BUTTON_UP  ,n>0);				//「上へ」ボタンの設定
}

void CSetAddIn::OnLbnSelchangeListFiles()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIST_FILES);
	int n=pLB->GetCurSel();									//選択されている位置の取得
	EnableButton(IDC_BUTTON_DOWN,n<m_nAddinCount-1);		//「下へ」ボタンの設定
	EnableButton(IDC_BUTTON_UP  ,n>0);						//「上へ」ボタンの設定
}

void CSetAddIn::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	CListBox* pLB=(CListBox*)GetDlgItem(IDC_LIST_FILES);	//リストボックスを取得して…
	int count = pLB->GetCount();
	for(int index = 0;index<MAX_ADDIN;index++){
		CString addinPath;
		TCHAR szEntry[32];
		wsprintf(szEntry,_T("ADDIN%d"),index);			//エントリ文字列の作成

		if(index<count){
			pLB->GetText(index,addinPath);
		}else{
			addinPath.Empty();
		}
		AfxGetApp()->WriteProfileString(_T("ADDINLIB"),szEntry,addinPath);
	}

	CDialog::OnOK();
}

void CSetAddIn::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	CDialog::OnCancel();
}
