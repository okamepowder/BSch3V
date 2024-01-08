/****************************************************************************
    PL3W partslist utility for BSch3V.
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/


// nlistDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "nlist.h"
#include "nlistDlg.h"
#include "plist.h"

#include "atlpath.h"

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

	m_noMessageBox = FALSE;
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
	ON_BN_CLICKED(IDC_RADIO_CSVOUT, &CNlistDlg::OnBnClickedRadioCsvout)
	ON_BN_CLICKED(IDC_RADIO_SYLKOUT, &CNlistDlg::OnBnClickedRadioSylkout)
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






	int check;

	//オプションの設定
	//コマンドラインの設定を確認して、0ならコマンドラインの指定がないのでINIファイル設定を使う
	//checkが1ならチェック、0または-1ならオフ

	//check = ((CNlistApp*)AfxGetApp())->m_cmdInfo.m_flag3sAvoidZeroSupOnExcel;
	//if(check == 0){
	//	check = AfxGetApp()->GetProfileInt("OPTION","AvoidZeroSupOnExcel",0);
	//}
	//((CButton*)GetDlgItem(IDC_CHECK_AVOID_ZERO_SUPPRESS))->SetCheck((check==1 ? BST_CHECKED : BST_UNCHECKED));


	//出力ファイル形式の初期設定
	//コマンドラインで設定されていたら、INIファイル設定より優先する。
	//出力ファイル名の拡張子がCSVまたはSLKならそれでオーバーライドする。

	int dstType = ((CNlistApp*)AfxGetApp())->m_cmdInfo.m_dstType;
	if(dstType == 0){
		dstType = AfxGetApp()->GetProfileInt(_T("OPTION"),_T("DstType"),0);
	}

	CString name;
	name= ((CNlistApp*)AfxGetApp())->m_cmdInfo.m_strList;
	if(!name.IsEmpty()){
		SetDlgItemText(IDC_EDIT_DST,name);

		TCHAR szExt[_MAX_EXT];
		_tsplitpath(name,NULL,NULL,NULL,szExt);
		if(_tcsicmp(szExt,_T(".xml"))==0){
			dstType = PListParam::PL_FILETYPE_XML;
		}else if(_tcsicmp(szExt,_T(".csv"))==0){
			dstType = PListParam::PL_FILETYPE_CSV;
		}
	}

	((CButton*)GetDlgItem(IDC_RADIO_CSVOUT))->SetCheck((dstType<=1 ? BST_CHECKED : BST_UNCHECKED));
	((CButton*)GetDlgItem(IDC_RADIO_SYLKOUT))->SetCheck((dstType==2 ? BST_CHECKED : BST_UNCHECKED));

	if(name.IsEmpty()){
		SetDstFileName();	//この関数はラジオボタンの状態をチェックしているので、ラジオボタンの設定後に実行する必要がある
	}

	
	check = ((CNlistApp*)AfxGetApp())->m_cmdInfo.m_flag3sMfr;
	if(check == 0){
		check = AfxGetApp()->GetProfileInt(_T("OPTION"),_T("Mfr"),0);
	}
	((CButton*)GetDlgItem(IDC_CHECK_MFR))->SetCheck((check==1 ? BST_CHECKED : BST_UNCHECKED));


	check = ((CNlistApp*)AfxGetApp())->m_cmdInfo.m_flag3sMfrPn;
	if(check == 0){
		check = AfxGetApp()->GetProfileInt(_T("OPTION"),_T("MfrPn"),0);
	}
	((CButton*)GetDlgItem(IDC_CHECK_MFRPN))->SetCheck((check==1 ? BST_CHECKED : BST_UNCHECKED));
	
	check = ((CNlistApp*)AfxGetApp())->m_cmdInfo.m_flag3sPackage;
	if(check == 0){
		check = AfxGetApp()->GetProfileInt(_T("OPTION"),_T("Package"),0);
	}
	((CButton*)GetDlgItem(IDC_CHECK_PKG))->SetCheck((check==1 ? BST_CHECKED : BST_UNCHECKED));
	
	check = ((CNlistApp*)AfxGetApp())->m_cmdInfo.m_flag3sNote;
	if(check == 0){
		check = AfxGetApp()->GetProfileInt(_T("OPTION"),_T("Note"),1);	//ノートは互換性のために1をデフォルトとする
	}
	((CButton*)GetDlgItem(IDC_CHECK_NOTE))->SetCheck((check==1 ? BST_CHECKED : BST_UNCHECKED));

	check = ((CNlistApp*)AfxGetApp())->m_cmdInfo.m_flag3sInfo;
	if(check == 0){
		check = AfxGetApp()->GetProfileInt(_T("OPTION"),_T("Info"),0);	//ノートは互換性のために1をデフォルトとする
	}
	((CButton*)GetDlgItem(IDC_CHECK_COMPINFO))->SetCheck((check==1 ? BST_CHECKED : BST_UNCHECKED));



	if(((CNlistApp*)AfxGetApp())->m_cmdInfo.m_forceExecute){
		m_noMessageBox=TRUE;
		OnOK();
		EndDialog(IDOK);
	}

	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
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
	//TRACE("CNlistDlg::OnSelchangeListSrc()\n");
	SetDstFileName();
}


int CNlistDlg::DstFileSetting()
{
	if(((CButton*)GetDlgItem(IDC_RADIO_CSVOUT))->GetCheck() == BST_CHECKED){
		return PListParam::PL_FILETYPE_CSV;	//CSV
	}else{
		return PListParam::PL_FILETYPE_XML;	//XML
	}
}


void CNlistDlg::SetDstFileName()
{
	int fileType = DstFileSetting();

	CEdit* pEdit=(CEdit*)GetDlgItem(IDC_EDIT_DST);
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
		if(fileType == 0){
			_tmakepath(szPath,szDrive,szDir,szName,_T(".csv"));
		}else{
			_tmakepath(szPath,szDrive,szDir,szName,_T(".xml"));
		}
		pEdit->SetWindowText(szPath);
		pBtn->EnableWindow(TRUE);
	}else{
		pEdit->SetWindowText(_T(""));
		pBtn->EnableWindow(FALSE);
	}
}

void CNlistDlg::OnRefdst() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int nResult;
//	TCHAR szFileNameBuffer[_MAX_PATH];
//	strcpy(szFileNameBuffer,"*.CE3");

	static const TCHAR* CSV_FILTER = _T("CSV file(*.csv)|*.CSV||");
	static const TCHAR* XML_FILTER = _T("Excel 2007 XML file(*.xml)|*.xml||");


	const TCHAR* pcszFilter = (DstFileSetting()<=1 ? CSV_FILTER : XML_FILTER);

	CFileDialog dlg(TRUE,				//オープンダイアログ
					NULL,				//拡張子が付けられなかった場合のデフォルト
					NULL,			//初期ファイル名
					OFN_HIDEREADONLY,
					pcszFilter,	//フィルタ文字列
					this);				//親ウインドウ
	
	
	nResult=dlg.DoModal();			//ファイルダイアログの実行
	if(nResult==IDOK){				//正常終了なら…
		CString strPath = dlg.GetPathName();
		//CPath strPath = dlg.GetPathName();

		//CString strExt = dlg.GetFileExt();
		//strExt.MakeLower();
		//if(strExt!="csv" && strExt!="slk"){
		//	int extIndex = dlg.m_ofn.nFilterIndex;
		//	TRACE("%d\n",extIndex);
		//	if(extIndex == 2){
		//		strPath.RenameExtension(".slk");
		//	}else{
		//		strPath.RenameExtension(".csv");
		//	}

		//}

		CEdit* pEdit=(CEdit*)GetDlgItem(IDC_EDIT_DST);
		pEdit->SetWindowText(strPath);
	}	
}

void CNlistDlg::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	SPlist plist;

	PListParam param;
	
	bool setflag;
	
	//setflag = ( (((CButton*)GetDlgItem(IDC_CHECK_AVOID_ZERO_SUPPRESS))->GetCheck() == BST_CHECKED) ? true : false );
	//param.plAvoidZeroSuppressingOnMsExcel = setflag;
	//AfxGetApp()->WriteProfileInt("OPTION","AvoidZeroSupOnExcel",(setflag? 1: 0));

	//param.plAvoidZeroSuppressingOnMsExcel = (AfxGetApp()->GetProfileInt("OPTION","AvoidZeroSupOnExcelCSV",0)!=0);

	int dstType = DstFileSetting();
	plist.m_param.listtype = dstType;	//CSVOUTでなければ、SYLK
	AfxGetApp()->WriteProfileInt(_T("OPTION"),_T("DstType"),dstType);

	setflag = ( (((CButton*)GetDlgItem(IDC_CHECK_MFR))->GetCheck() == BST_CHECKED) ? true : false );
	plist.m_param.plFlagMfr = setflag;
	AfxGetApp()->WriteProfileInt(_T("OPTION"),_T("Mfr"),(setflag? 1: 0));

	setflag = ( (((CButton*)GetDlgItem(IDC_CHECK_MFRPN))->GetCheck() == BST_CHECKED) ? true : false );
	plist.m_param.plFlagMfrPn = setflag;
	AfxGetApp()->WriteProfileInt(_T("OPTION"),_T("MfrPn"),(setflag? 1: 0));

	setflag = ( (((CButton*)GetDlgItem(IDC_CHECK_PKG))->GetCheck() == BST_CHECKED) ? true : false );
	plist.m_param.plFlagPackage = setflag;
	AfxGetApp()->WriteProfileInt(_T("OPTION"),_T("Package"),(setflag? 1: 0));

	setflag = ( (((CButton*)GetDlgItem(IDC_CHECK_NOTE))->GetCheck() == BST_CHECKED) ? true : false );
	plist.m_param.plFlagNote = setflag;
	AfxGetApp()->WriteProfileInt(_T("OPTION"),_T("Note"),(setflag? 1: 0));


	setflag = ( (((CButton*)GetDlgItem(IDC_CHECK_COMPINFO))->GetCheck() == BST_CHECKED) ? true : false );
	plist.m_param.plFlagInfo = setflag;
	AfxGetApp()->WriteProfileInt(_T("OPTION"),_T("Info"),(setflag? 1: 0));


	CSrcLB* pLB=(CSrcLB*)GetDlgItem(IDC_LIST_SRC);
	int nCount = pLB->GetCount();
	if(nCount==0){
		return;
	}


	CString str;
	int readCount=0;
	for(int n=0;n<nCount;n++){
		pLB->GetText(n,str);
		int retval=0;
		retval=plist.readFile(str);
		switch(retval){
			case 1://すでに同じファイルを読み込みずみ
				break;
			case 2://読み込みエラー
				break;
			case 3:
				break;
			default:
				break;
		}
	}

	if(plist.fileCount()<nCount){
		if(!m_noMessageBox)	AfxMessageBox(IDS_SRCOPENERR);
		return;
	}


	CEdit* pEdit=(CEdit*)GetDlgItem(IDC_EDIT_DST);
	CString strDstPath;
	pEdit->GetWindowText(strDstPath);


//	FILE* fp =fopen(strDstPath,"wt");
//	FILE* fp =_tfopen(strDstPath,_T("wb"));

	//部品表を作成する。
	int result = plist.writePartsList(strDstPath);

	if(result == -2){
		if(!m_noMessageBox)	AfxMessageBox(IDS_DSTOPENERR);
		return;
	//}else if(result == -1){

	}else{
		if(!m_noMessageBox)	AfxMessageBox(IDS_FINISHED, MB_OK | MB_ICONINFORMATION);
	}

	//CDialog::OnOK();
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
		CString msg;
	CString strVersion;
	strVersion.LoadString(IDS_VERSION);
	msg = _T("PL3W Version ");
	msg += strVersion;
	SetDlgItemText(IDC_STATIC_VERSION,msg);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CNlistDlg::OnBnClickedRadioCsvout()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	TRACE("CNlistDlg::OnBnClickedRadioCsvout()\n");
	SetDstFileName();
}

void CNlistDlg::OnBnClickedRadioSylkout()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	TRACE("CNlistDlg::OnBnClickedRadioSylkout()\n");
	SetDstFileName();
}
