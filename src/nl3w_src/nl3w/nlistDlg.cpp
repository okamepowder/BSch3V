/****************************************************************************
    NL3W netlist utility for BSch3V
    Copyright (C) 2004-2014 H.Okada
*****************************************************************************/

#include "stdafx.h"
#include "nlist.h"
#include "nlistDlg.h"
#include "netlist.h"
#include ".\nlistdlg.h"
#include "SetAddIn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// アプリケーションのバージョン情報で使われている CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// メッセージ ハンドラがありません。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNlistDlg ダイアログ

CNlistDlg::CNlistDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNlistDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNlistDlg)
		// メモ: この位置に ClassWizard によってメンバの初期化が追加されます。
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_noMessageBox=FALSE;
}

void CNlistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNlistDlg)
	DDX_Control(pDX, IDC_LIST_SRC, m_listsrc);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNlistDlg, CDialog)
	//{{AFX_MSG_MAP(CNlistDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_REFSRC, OnRefsrc)
	ON_BN_CLICKED(IDC_CLR, OnClr)
	ON_LBN_SELCHANGE(IDC_LIST_SRC, OnSelchangeListSrc)
	ON_BN_CLICKED(IDC_REFDST, OnRefdst)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_REFRPT, OnBnClickedRefrpt)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_SETADDIN, &CNlistDlg::OnBnClickedButtonSetaddin)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNlistDlg メッセージ ハンドラ

BOOL CNlistDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "バージョン情報..." メニュー項目をシステム メニューへ追加します。

	// IDM_ABOUTBOX はコマンド メニューの範囲でなければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログ用のアイコンを設定します。フレームワークはアプリケーションのメイン
	// ウィンドウがダイアログでない時は自動的に設定しません。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
	HICON hIcon=AfxGetApp()->LoadIcon(IDI_SMALL);
	SetIcon(hIcon, FALSE);		// 小さいアイコンを設定
//	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定
	
	// TODO: 特別な初期化を行う時はこの場所に追加してください。

	CString title;
	GetWindowText(title);

	CString strVersion;
	strVersion.LoadString(IDS_VERSION);

	title = title + _T(" Ver. ") + strVersion;
	SetWindowText(title);



	CSrcLB* pLB=(CSrcLB*)GetDlgItem(IDC_LIST_SRC);
	const CStringList& strListSrcFiles=((CNlistApp*)AfxGetApp())->m_cmdInfo.m_strListSrcFiles;
	POSITION pos = strListSrcFiles.GetHeadPosition();
	while(pos){
		CString fileName = strListSrcFiles.GetNext(pos);
		pLB->AddString(fileName);
	}
	SetDstFileName();
	CString name;

	name= ((CNlistApp*)AfxGetApp())->m_cmdInfo.m_strNetList;
	if(!name.IsEmpty()){
		SetDlgItemText(IDC_EDIT_DST,name);
	}

	name= ((CNlistApp*)AfxGetApp())->m_cmdInfo.m_strReportFile;
	if(!name.IsEmpty()){
		SetDlgItemText(IDC_EDIT_RPT,name);
	}




	int listType = ((CNlistApp*)AfxGetApp())->m_cmdInfo.m_listType;
	if(listType == 0){
		listType = AfxGetApp()->GetProfileInt(_T("OPTION"),_T("ListFormat"),1);
	}

	//アドインの設定をINIファイルから読み込んでライブラリを設定する。
	g_addinFiles.update();


	SetListTypeComboBox(listType-1);
	//CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_NETTYPE);
	//pCombo->AddString("Telesis");
	//pCombo->AddString("Protel");
	

	//int listTypeCount = pCombo->GetCount();
	//int selIndex = listType-1;
	//if(selIndex<0)selIndex = 0;
	//else if(selIndex>=listTypeCount)selIndex=listTypeCount-1;

	//pCombo->SetCurSel(selIndex);
	

	//if(listType == 1){
	//	((CButton*)GetDlgItem(IDC_RADIO_TELESIS))->SetCheck(BST_CHECKED);
	//}else{
	//	((CButton*)GetDlgItem(IDC_RADIO_PROTEL))->SetCheck(BST_CHECKED);
	//}

	CButton* pButton;

	int check;
	int flag3sRemoveNOCONNECTION = ((CNlistApp*)AfxGetApp())->m_cmdInfo.m_flag3sRemoveNOCONNECTION;
	if(flag3sRemoveNOCONNECTION!=0){
		check = (flag3sRemoveNOCONNECTION>0 ? 1 : 0);
	}else{
		check = AfxGetApp()->GetProfileInt(_T("OPTION"),_T("ExcludeNoConnectionPin"),1);
	}
	pButton=(CButton*)GetDlgItem(IDC_CHECK_NC);
	pButton->SetCheck(check);


	int flag3sSetPinInfoToNoNameNet = ((CNlistApp*)AfxGetApp())->m_cmdInfo.m_flag3sSetPinInfoToNoNameNet;
	if( flag3sSetPinInfoToNoNameNet!=0){
		check = ( flag3sSetPinInfoToNoNameNet>0 ? 1 : 0);
	}else{
		check = AfxGetApp()->GetProfileInt(_T("OPTION"),_T("SetPinInfoToNoNameNet"),0);
	}
	pButton=(CButton*)GetDlgItem(IDC_CHECK_SIGNAME);
	pButton->SetCheck(check);


	
	if(((CNlistApp*)AfxGetApp())->m_cmdInfo.m_forceExecute){
		m_noMessageBox=TRUE;
		OnOK();
		//CDialog::OnOK();
		EndDialog(IDOK);
	}

	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}

void CNlistDlg::SetListTypeComboBox(int selIndex)
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_NETTYPE);

	if(selIndex<0){
		selIndex = pCombo->GetCurSel();
	}

	pCombo->ResetContent();
	pCombo->AddString(_T("Telesis"));
	pCombo->AddString(_T("Protel"));


	int addinCount = g_addinFiles.m_nAddinCount;
	for(int i=0;i<addinCount;i++){
		void( __stdcall *typeName)(TCHAR*,int);
		typeName = (void(__stdcall*)(TCHAR*,int)) ::GetProcAddress( g_addinFiles.m_hAddin[i],"typeName"); 
		TCHAR buff[64];
		typeName(buff,64);
		pCombo->AddString(buff);
	}

	int listTypeCount = pCombo->GetCount();
	if(selIndex<0)selIndex = 0;
	else if(selIndex>=listTypeCount)selIndex=listTypeCount-1;

	pCombo->SetCurSel(selIndex);
}


void CNlistDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
// コードを以下に記述する必要があります。MFC アプリケーションは document/view
// モデルを使っているので、この処理はフレームワークにより自動的に処理されます。

void CNlistDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画用のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// クライアントの矩形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンを描画します。
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// システムは、ユーザーが最小化ウィンドウをドラッグしている間、
// カーソルを表示するためにここを呼び出します。
HCURSOR CNlistDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



void CNlistDlg::OnRefsrc() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int nResult;
	//TCHAR szFileNameBuffer[FILE_NAME_BUFFER_SIZE];
	_tcscpy(m_szFileNameBuffer,_T("*.CE3"));

	CFileDialog dlg(TRUE,				//オープンダイアログ
					NULL,				//拡張子が付けられなかった場合のデフォルト
					NULL,			//初期ファイル名
					OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_ALLOWMULTISELECT,
					_T("BSch3 data(*.ce3)|*.CE3||"),	//フィルタ文字列
					this);				//親ウインドウ
	
	dlg.m_ofn.lpstrFile = m_szFileNameBuffer;	//ファイル名を格納するバッファを設定する。
	dlg.m_ofn.nMaxFile = FILE_NAME_BUFFER_SIZE;		//ファイル名を格納するバッファのサイズを設定する。
	
	nResult=dlg.DoModal();			//ファイルダイアログの実行
	if(nResult==IDOK){		//正常終了なら…
		CSrcLB* pLB=(CSrcLB*)GetDlgItem(IDC_LIST_SRC);	//リストボックスを取得して…
		//pLB->ResetContent();
		CString strPath;
		POSITION pos = dlg.GetStartPosition();
		while(pos){
			strPath = dlg.GetNextPathName(pos);				
			pLB->AddString(strPath);	//リストボックスへのセット
		}
		pLB->SetListboxWidth();
		SetDstFileName();
	}

}

void CNlistDlg::OnClr() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CSrcLB* pLB=(CSrcLB*)GetDlgItem(IDC_LIST_SRC);
	pLB->ResetContent();
	SetDstFileName();

}

void CNlistDlg::OnSelchangeListSrc() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	TRACE("CNlistDlg::OnSelchangeListSrc()\n");
	SetDstFileName();
}

void CNlistDlg::SetDstFileName()
{
//	CEdit* pEdit=(CEdit*)GetDlgItem(IDC_EDIT_DST);
	CButton* pBtn=(CButton*)GetDlgItem(IDOK);
	CSrcLB* pLB=(CSrcLB*)GetDlgItem(IDC_LIST_SRC);
	int n = pLB->GetCount();
	if(n>0){
		CString strSrc;
		int index = pLB->GetCurSel();
		if(index == LB_ERR){
			index = 0;
		}
		pLB->GetText(index,strSrc);
		TCHAR szPath[_MAX_PATH];
		TCHAR szDrive[_MAX_DRIVE];
		TCHAR szDir[_MAX_DIR];
		TCHAR szName[_MAX_FNAME];
		_tsplitpath(strSrc,szDrive,szDir,szName,NULL);
		_tmakepath_s(szPath,_MAX_PATH,szDrive,szDir,szName,_T(".net"));
		SetDlgItemText(IDC_EDIT_DST,szPath);
		_tmakepath_s(szPath,_MAX_PATH,szDrive,szDir,szName,_T(".rpt"));
		SetDlgItemText(IDC_EDIT_RPT,szPath);
		//pEdit->SetWindowText(szPath);
		pBtn->EnableWindow(TRUE);
	}else{
		SetDlgItemText(IDC_EDIT_DST,_T(""));
		SetDlgItemText(IDC_EDIT_RPT,_T(""));
		//pEdit->SetWindowText("");
		pBtn->EnableWindow(FALSE);
	}
}
 
void CNlistDlg::OnRefdst() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int nResult;
//	TCHAR szFileNameBuffer[_MAX_PATH];
//	strcpy(szFileNameBuffer,"*.CE3");

	CFileDialog dlg(FALSE,				//オープンダイアログ
					NULL,				//拡張子が付けられなかった場合のデフォルト
					NULL,			//初期ファイル名
					OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
					_T("Netlist(*.net)|*.NET||"),	//フィルタ文字列
					this);				//親ウインドウ
	
	
	nResult=dlg.DoModal();			//ファイルダイアログの実行
	if(nResult==IDOK){				//正常終了なら…
		CEdit* pEdit=(CEdit*)GetDlgItem(IDC_EDIT_DST);
		CString strPath = dlg.GetPathName();
		pEdit->SetWindowText(strPath);
	}	
}

void CNlistDlg::OnBnClickedRefrpt()
{
	int nResult;
	CFileDialog dlg(FALSE,				
					NULL,				//拡張子が付けられなかった場合のデフォルト
					NULL,			//初期ファイル名
					OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
					_T("Report(*.rpt)|*.RPT||"),	//フィルタ文字列
					this);				//親ウインドウ
	
	
	nResult=dlg.DoModal();			//ファイルダイアログの実行
	if(nResult==IDOK){				//正常終了なら…
		CEdit* pEdit=(CEdit*)GetDlgItem(IDC_EDIT_RPT);
		CString strPath = dlg.GetPathName();
		pEdit->SetWindowText(strPath);
	}	
}

void CNlistDlg::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	CButton* pButton;
	pButton=(CButton*)GetDlgItem(IDC_CHECK_NC);
	int nExcludeNC=pButton->GetCheck();
	AfxGetApp()->WriteProfileInt(_T("OPTION"),_T("ExcludeNoConnectionPin"),(nExcludeNC!=0 ? 1 : 0));

	pButton=(CButton*)GetDlgItem(IDC_CHECK_SIGNAME);
	int nSigNamePinInfo=pButton->GetCheck();
	AfxGetApp()->WriteProfileInt(_T("OPTION"),_T("SetPinInfoToNoNameNet"),(nSigNamePinInfo!=0 ? 1 : 0));


	CSrcLB* pLB=(CSrcLB*)GetDlgItem(IDC_LIST_SRC);
	int nCount = pLB->GetCount();
	if(nCount==0){
		return;
	}else{
		if(nCount>64)nCount=64;
	}

	
	const TCHAR* filelist[64];

	TCHAR* top = m_szFileNameBuffer;
	for(int n=0;n<nCount;n++){
		int charCount = pLB->GetText(n,top);
		if(charCount == LB_ERR || charCount == 0){
			nCount = n;
			break;
		}
		filelist[n]=top;
		top+=(charCount+1);
	}

	CString strDstNetPath;
	GetDlgItemText(IDC_EDIT_DST,strDstNetPath);
	//FILE* fp =fopen(strDstPath,"wt");
	//if(!fp){
	//	if(!m_noMessageBox)	AfxMessageBox(IDS_DSTOPENERR);
	//	return;
	//}

	CString strDstRptPath;
	GetDlgItemText(IDC_EDIT_RPT,strDstRptPath);
	FILE* fprpt =_wfopen(strDstRptPath,_T("wb"));


	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_NETTYPE);
	int listFormat = pCombo->GetCurSel()+1;
	if(listFormat<1)listFormat=1;


	//int listFormat = ( ((CButton*)GetDlgItem(IDC_RADIO_TELESIS))->GetCheck()==BST_CHECKED ? 1 : 2);
	AfxGetApp()->WriteProfileInt(_T("OPTION"),_T("ListFormat"),listFormat);


	//ネットリストを作成する。
	SNetlistOption option;
	option.pszDstPath = strDstNetPath;
	//option.fpDst=fp;
	option.fpRpt=fprpt;
	option.bPlistMode=false;
	option.listType=listFormat;
	option.bExcludeNC = (nExcludeNC!=0);
	option.bSigNamePinInfo = (nSigNamePinInfo!=0);

	int res = netlist(nCount,filelist,option);
	//fclose(fp);
	if(fprpt) fclose(fprpt);

	
	switch(res){
	case -1:
		if(!m_noMessageBox)	AfxMessageBox(IDS_DSTOPENERR);
		break;
	case -2:
		if(!m_noMessageBox)	AfxMessageBox(IDS_NO_NETINFO);
		break;
	case -3:
		if(!m_noMessageBox)	AfxMessageBox(IDS_SRCOPENERR);
		break;
	case -4:
		if(!m_noMessageBox)	AfxMessageBox(IDS_ADDIN_INDEX_ERROR);
		break;
	case 0:
		if(!m_noMessageBox)	AfxMessageBox(IDS_FINISHED);
		break;
	default:
		if(!m_noMessageBox){
			CString strMsg;
			strMsg.Format(_T("Add-in error: %d"),res);
			AfxMessageBox(strMsg);
		}		
	}

	//CDialog::OnOK();
}



void CNlistDlg::OnBnClickedCancel()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	OnCancel();
}

void CNlistDlg::OnBnClickedButtonSetaddin()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	CSetAddIn dlg;
	if(dlg.DoModal() == IDOK){
		g_addinFiles.update();
		SetListTypeComboBox(-1);
	}
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	CString msg;
	CString strVersion;
	strVersion.LoadString(IDS_VERSION);
	msg = "NL3W Version ";
	msg += strVersion;
	SetDlgItemText(IDC_STATIC_VERSION,msg);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
