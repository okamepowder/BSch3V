/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// BSch.cpp : アプリケーション用クラスの機能定義を行います。
//

#include "stdafx.h"
#include "cderr.h"
#include "BSch.h"
#include <afxdisp.h>
#include <assert.h>
#include <string>
#include <list>
#include <Shlobj.h>

#ifndef LC_ALL
#include <locale.h>
#endif

using namespace std;


#include "MainFrm.h"

//#include "BSchObj.h"

#include "BSchDoc.h"
#include "BSchView.h"

#include "complib.h"
#include "BSchFont.h"
#include "Global.h"

// Import from Qt-BSch project
#include "xbschglobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CBSchApp

BEGIN_MESSAGE_MAP(CBSchApp, CWinApp)
	//{{AFX_MSG_MAP(CBSchApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_PRINT_SETUP, OnFilePrintSetup)
	//}}AFX_MSG_MAP
	// 標準のファイル基本ドキュメント コマンド
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// 標準の印刷セットアップ コマンド
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBSchApp クラスの構築

CBSchApp::CBSchApp()
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
	//int i;
	//for(i=0;i<MAX_LIB;i++) g_pCompLib[i]=NULL;
	//g_bPrintPartNum=TRUE;	// 1997/01/26
	//g_bPrintPartName=TRUE;	// 1998/07/22

	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR pathModule[_MAX_PATH];

	m_szIniFileDir[0] = '\0';

	m_pszIniFileName = new TCHAR[_MAX_PATH];		//パス名格納エリアの確保
												//MFCがこれをdeleteするので、固定エリアを確保してはいけない。

	//モジュールのパスを調べる。
	::GetModuleFileName(NULL,m_moduleFileName,_MAX_PATH);

	//連携するLCoVのパスを設定する
	::_tsplitpath(m_moduleFileName,drive,dir,NULL,NULL);
	::_tmakepath(m_LCoVPath,drive,dir,_T("LCoV"),_T(".exe"));

	//INIファイルを探す
	::_tsplitpath(m_moduleFileName,pathModule,dir,NULL,NULL);
	_tcscat(pathModule,dir);	//pathModuleにモジュールのディレクトリ。末尾は\記号。

	//カレントディレクトリ
	::GetCurrentDirectory(_MAX_PATH,m_pszIniFileName);	//カレントディレクトリを得る
	_tcscat(m_pszIniFileName,_T("\\"));			//\記号を追加

	//モジュールのパスとカレントディレクトリが一致しないときは、
	//カレントディレクトリのBSCH3.INIの有無を確認する。
	//カレントディレクトリにBSCH3.INIが見つかったときはこれをINIファイルとして使う。
	if(_tcscmp(pathModule,m_pszIniFileName)!=0){
		_tcscat(m_pszIniFileName,_T("BSCH3.INI"));			//BSCH3.INIを追加
		//lstrcpy(m_pszIniFileName,"BSCH.INI");
		FILE *pf=_tfopen(m_pszIniFileName,_T("rt"));
		if(pf){
			fclose(pf);
			goto EndOfFunc;
			//m_pszProfileName=m_pszIniFileName;	//作成した文字列のポインタをm_pszProfileNameにコピー
			//return;
		}
	}

	//次にWindows 2000/XPにおいて
	//\Documents and Settings\username\Local Settings\Application Data\SuigyodoONLINE\BSCH3.INI を探す。
	//これが見つかったときは、これをINIファイルとして使う。
	OSVERSIONINFO versionInfo;
	versionInfo.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	if(GetVersionEx(&versionInfo)){
		if(versionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT){
			if(SHGetSpecialFolderPath(NULL,m_pszIniFileName,CSIDL_LOCAL_APPDATA,FALSE)){
				TRACE(_T("CBSchApp::CBSchApp() %s\n"),m_pszIniFileName);
				_tcscat(m_pszIniFileName,_T("\\SuigyodoONLINE\\BSCH3.INI"));
				FILE *pf=_tfopen(m_pszIniFileName,_T("rt"));
				if(pf){
					fclose(pf);
					goto EndOfFunc;
					//m_pszProfileName=m_pszIniFileName;	//作成した文字列のポインタをm_pszProfileNameにコピー
					//return;
				}
			}
		}
	}

	//どこにも見つからなかったときにはモジュールの格納フォルダの BSCH3.INIを使う
	//ここにもBSCH3.INIが見つからないときは、ここにBSCH3.INIが新規作成されるが、その場合、新規作成される前に0xFF,0xFEだけのファイルを作っておく
	::_tsplitpath(m_moduleFileName,drive,dir,NULL,NULL);

	TCHAR makeinipath[_MAX_PATH];
	::_tmakepath(makeinipath,drive,dir,_T("BSCH3"),_T(".INI"));
	::PathCanonicalize(m_pszIniFileName,makeinipath);	//途中に相対が入った状態を整理する。

	//::_tmakepath(makeinipath,drive,dir,_T("BSCH3"),_T(".INI"));
	{
		FILE *pf=_tfopen(m_pszIniFileName,_T("rt"));
		if(pf){
			fclose(pf);
		}else{
			static const byte BOMDATA[2]={0xff,0xfe};
			pf=_tfopen(m_pszIniFileName,_T("wb"));
			if(pf){
				fwrite(BOMDATA,1,2,pf);
				fclose(pf);
			}
		}
	}

EndOfFunc:
	m_pszProfileName=m_pszIniFileName;	//作成した文字列のポインタをm_pszProfileNameにコピー
	::_tsplitpath(m_pszIniFileName,m_szIniFileDir,dir,NULL,NULL);
	::_tcscat(m_szIniFileDir,dir);
}

/////////////////////////////////////////////////////////////////////////////
// CBSchApp クラスの消滅  96/10/20 追加

CBSchApp::~CBSchApp()
{



	g_FreeLibrary();	//ライブラリのメモリ解放
	delete g_pFont;
	delete g_pFontName;
}



/////////////////////////////////////////////////////////////////////////////
// 唯一の CBSchApp オブジェクト

CBSchApp theApp;


/////////////////////////////////////////////////////////////////////////////
// CBSchApp クラスの初期化

BOOL CBSchApp::InitInstance()
{
	_tsetlocale(LC_ALL, _T(""));

	// 標準的な初期化処理
	// もしこれらの機能を使用せず、実行ファイルのサイズを小さく
	// したければ以下の特定の初期化ルーチンの中から不必要なもの
	// を削除してください。

	//AfxMessageBox(m_lpCmdLine);


	CWinApp::InitInstance();


	// Initialize OLE libraries
    if (!AfxOleInit()) 
    {
        return FALSE;
    }

#ifdef _AFXDLL
	Enable3dControls();			// 共有 DLL の中で MFC を使用する場合にはここをコールしてください。 
#else
	Enable3dControlsStatic();	// MFC と静的にリンクしている場合にはここをコールしてください。
#endif

	LCID lcid = ::GetSystemDefaultLCID();
	g_bJapaneseLangEnv = (lcid == 1041);

	//言語リソースDLLを読み込む前のリソース中のバージョン情報とリソースハンドルの保存

	
	//ここから Version 0.69 20101016
	//言語DLLの読み込みパスを実行ファイルのディレクトリに制限
	TCHAR dllPath[_MAX_PATH];
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	::_tsplitpath(m_moduleFileName,drive,dir,NULL,NULL);
	::_tmakepath(dllPath,drive,dir,_T("bsch3v_lang"),_T("dll"));
	//ここまで Version 0.69 20101016
	
	CString strVersion;
	strVersion.LoadString(IDS_VERSION);
	HINSTANCE hInstRcOrg = AfxGetResourceHandle();
	//HINSTANCE hInstRc = LoadLibrary( dllPath );			//Version 0.69 20101016 言語DLLの読み込みパスを実行ファイルのディレクトリに制限
	//if ( hInstRc){				// 言語リソースDLLが見つかった.
	//	AfxSetResourceHandle(hInstRc);
	//	//言語リソース中のバージョン情報をチェック
	//	CString strVersionAddRc;
	//	strVersionAddRc.LoadString(IDS_VERSION);
	//	//バージョンが一致しなければ、元のリソースハンドルに戻す
	//	if(strVersion.Compare(strVersionAddRc)!=0){
	//		AfxSetResourceHandle(hInstRcOrg);
	//	}
	//}

	//::_tmakepath(g_logPath,drive,dir,_T("bsch3v_log"),_T("txt"));
	//g_log = _T("BSCH3V_LOG\n");


//	SetRegistryKey("Suigyodo");	//INIファイルの代わりにレジストリ"Suigyodo(水魚堂)"を使う
// 1998/05/19
// Version 0.40   レジストリのかわりにカレントディレクトリの BSCH.INI を使用するように変更
// CBSchApp() (コンストラクタ) で、m_pszProfileName にカレントディレクトリのBSCH.INIを指す
// ように設定
// TRACE("m_pszProfileName %s\n",m_pszProfileName);


	LoadStdProfileSettings(4);  // 標準の INI ファイルのオプションをローﾄﾞします (MRU を含む)


	// アプリケーション用のドキュメント テンプレートを登録します。ドキュメント テンプレート
	// はドキュメント、フレーム ウィンドウとビューを結合するために機能します。

	/////////////////////////////////////////////////////////////////////////////
	// 96/10/21
	//グローバル変数の初期化など
	g_ReadLibrary();	//部品ライブラリの読み込み
	LOGFONT lf;
	g_pFont=new CBSchFont;
	g_IniReadLabelFontInfo(&lf);
	g_pFont->setLogFont(lf);

	g_pFontName=new CBSchFont;
	g_IniReadNameFontInfo(&lf);
	g_pFontName->setLogFont(lf);
	g_pViewWindow = NULL;


	//g_RegisterClipboardFormat();//クリップボードフォーマットの登録
			
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CBSchDoc),
		RUNTIME_CLASS(CMainFrame),       // メイン SDI フレーム ウィンドウ
		RUNTIME_CLASS(CBSchView));
	AddDocTemplate(pDocTemplate);

	// DDE Execute open を使用可能にします。
	EnableShellOpen();
	RegisterShellFileTypes(FALSE);


	// DDE、file open など標準のシェル コマンドのコマンドラインを解析します。
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if(cmdInfo.m_nShellCommand == CCommandLineInfo::AppRegister){
		RegisterShellFileTypes(TRUE); 	//20090211
		AfxMessageBox(IDS_REG_FILETYPE,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}


	//ウィンドウサイズの復元
	int cmdShow;
	int mainframeCx;
	int mainframeCy;
	CMainFrame::IniReadWindowPosition(cmdShow,mainframeCx,mainframeCy);
	if(cmdShow==SW_SHOWMAXIMIZED){	//最大化するときだけ前回終了時の最大化情報を使う。それ以外はWindowsの指定に従う
		m_nCmdShow = SW_SHOWMAXIMIZED;
	}
	CMainFrame::SetInitialWindowSize(mainframeCx,mainframeCy);

	//20090211
	if(cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister){
		UnregisterShellFileTypes();
		AfxMessageBox(IDS_UNREG_FILETYPE,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}

	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// コマンドラインでディスパッチ コマンドを指定します。

	// ドラッグ/ドロップ オープンを許可します
	m_pMainWnd->DragAcceptFiles();

	// INIファイルによるグローバル変数の初期化
	::g_notSaveBitPtn = (AfxGetApp()->GetProfileInt(_T("Option"),_T("NotSaveBitPtn"),0)!=0);
	AfxGetApp()->WriteProfileInt(_T("Option"),_T("NotSaveBitPtn"),(g_notSaveBitPtn?1:0));

	m_pMainWnd->ShowWindow(m_nCmdShow);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// アプリケーションのバージョン情報で使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

  	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV のサポート
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// メッセージ ハンドラはありません。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSuigyodo();
	afx_msg void OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult);
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
	ON_BN_CLICKED(IDC_BUTTON_SUIGYODO, OnBnClickedButtonSuigyodo)
END_MESSAGE_MAP()

// ダイアログを実行するためのアプリケーション コマンド
void CBSchApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CBSchApp コマンド


CDocument* CBSchApp::OpenDocumentFile(LPCTSTR lpszFileName) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	TRACE(_T("OpenDocument(%s)\n"),lpszFileName);
	//ロングファイルネームを強制的に使用する BSch Version 0.64 1998/12/14
	TCHAR szLongFname[_MAX_PATH];
	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];
	WIN32_FIND_DATA ffd;
	HANDLE hFindFile = ::FindFirstFile(lpszFileName,&ffd);
	if(hFindFile != INVALID_HANDLE_VALUE){
		::_tsplitpath(lpszFileName,szDrive,szDir,NULL,NULL);
		::_tmakepath(szLongFname,szDrive,szDir,_T(""),_T(""));
		_tcscat(szLongFname,ffd.cFileName);
		::FindClose(hFindFile);
	}else{
		_tcscpy(szLongFname,lpszFileName);
	}

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if(pMainFrame){
		CDocument* pDoc=pMainFrame->GetActiveDocument();
		if(pDoc){
			//現在のドキュメントが空でなければ、新規ウィンドウを開く
			if(!((CBSchDoc*)pDoc)->IsEmpty()){
				if(ExecBSch(szLongFname)) return pDoc;
			}
		}
	}

	return CWinApp::OpenDocumentFile(szLongFname);
//	return CWinApp::OpenDocumentFile(lpszFileName);
}


BOOL CBSchApp::PreTranslateMessage(MSG* pMsg)
{
	return CWinApp::PreTranslateMessage(pMsg);
}

BOOL CBSchApp::ExecBSch(const TCHAR* pathname)
{
	CString strCmdline;
	if(!pathname || *pathname == 0){
		strCmdline.Format(_T("\"%s\""),m_moduleFileName);
	}else{
		strCmdline.Format(_T("\"%s\" \"%s\""),m_moduleFileName,pathname);
	}

	TCHAR* cmdline = new TCHAR[_tcslen(strCmdline)+1];
	_tcscpy(cmdline,strCmdline);


	PROCESS_INFORMATION pi;
	STARTUPINFO si;

    ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    BOOL resProc = CreateProcess(NULL, cmdline, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

	CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

	delete[]cmdline;

	return resProc;

}


void CBSchApp::OnFileNew() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if(pMainFrame){
		CDocument* pDoc=pMainFrame->GetActiveDocument();
		if(!((CBSchDoc*)pDoc)->IsEmpty()){
			if(ExecBSch(_T(""))) return;
		}
	}
	CWinApp::OnFileNew();
}

/////////////////////////////////////////////////////////////
// プリンタ設定関連
static const TCHAR* lpszPrinterSetupInfo = _T("PrinterSetupInfo");

void CBSchApp::IniWritePrinterSetup()
{
	if(m_hDevNames != NULL && m_hDevMode != NULL){
		DEVNAMES* lpDevNames = (DEVNAMES*)::GlobalLock(m_hDevNames);
		DEVMODE* lpDevMode = (DEVMODE*)::GlobalLock(m_hDevMode);
		AfxGetApp()->WriteProfileString(lpszPrinterSetupInfo,_T("Driver"),(LPCTSTR)lpDevNames+lpDevNames->wDriverOffset);
		AfxGetApp()->WriteProfileString(lpszPrinterSetupInfo,_T("Device"),(LPCTSTR)lpDevNames+lpDevNames->wDeviceOffset);
		AfxGetApp()->WriteProfileString(lpszPrinterSetupInfo,_T("Output"),(LPCTSTR)lpDevNames+lpDevNames->wOutputOffset);
		AfxGetApp()->WriteProfileInt(lpszPrinterSetupInfo,_T("Orientation"),lpDevMode->dmOrientation);
		AfxGetApp()->WriteProfileInt(lpszPrinterSetupInfo,_T("PaperSize"),lpDevMode->dmPaperSize);
		GlobalUnlock(m_hDevNames);
		GlobalUnlock(m_hDevMode);
	}
}


void g_GlobalFree(HGLOBAL hGlobal)
{
	if (hGlobal == NULL)return;
	UINT nCount = GlobalFlags(hGlobal) & GMEM_LOCKCOUNT;
	while (nCount--)
		GlobalUnlock(hGlobal);
	GlobalFree(hGlobal);
}

void CBSchApp::OnFilePrintSetup() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	//CWinApp::OnFilePrintSetup();

		CPrintDialog pd(TRUE);
	//	DoPrintDialog(&pd);

	UpdatePrinterSelection(FALSE);

	pd.m_pd.hDevMode = m_hDevMode;
	pd.m_pd.hDevNames = m_hDevNames;
	int nResponse = pd.DoModal();

	// if OK or Cancel is selected we need to update cached devMode/Names
	while (nResponse != IDOK && nResponse != IDCANCEL)
	{
		switch (::CommDlgExtendedError())
		{
		// CommDlg cannot give these errors after NULLing these handles
		case PDERR_PRINTERNOTFOUND:
		case PDERR_DNDMMISMATCH:
		case PDERR_DEFAULTDIFFERENT:
			if (pd.m_pd.hDevNames != NULL)
			{
				ASSERT(m_hDevNames == pd.m_pd.hDevNames);
				g_GlobalFree(pd.m_pd.hDevNames);
				pd.m_pd.hDevNames = NULL;
				m_hDevNames = NULL;
			}

			if (pd.m_pd.hDevMode)
			{
				ASSERT(m_hDevMode == pd.m_pd.hDevMode);
				g_GlobalFree(pd.m_pd.hDevMode);
				pd.m_pd.hDevMode = NULL;
				m_hDevMode = NULL;
			}
			break;

		default:
			return;
		}
		nResponse = pd.DoModal();
	}
	// refresh current CWinApp cache of printer device information
	m_hDevMode = pd.m_pd.hDevMode;
	m_hDevNames = pd.m_pd.hDevNames;
	IniWritePrinterSetup();

}

void CBSchApp::UpdatePrinterSelection(BOOL bForceDefaults)
{
	BOOL fSet1st = (m_hDevNames == NULL);
	
	
	CWinApp::UpdatePrinterSelection(bForceDefaults);

	//2013/10/07
	if(fSet1st && !bForceDefaults && m_hDevNames != NULL && m_hDevMode != NULL){

		CString strDriver	= AfxGetApp()->GetProfileString(lpszPrinterSetupInfo,_T("Driver"),_T(""));
		CString strDevice	= AfxGetApp()->GetProfileString(lpszPrinterSetupInfo,_T("Device"),_T(""));
		CString strOutput	= AfxGetApp()->GetProfileString(lpszPrinterSetupInfo,_T("Output"),_T(""));
		int nOrientation	= AfxGetApp()->GetProfileInt(lpszPrinterSetupInfo,_T("Orientation"),0);
		int nPaperSize		= AfxGetApp()->GetProfileInt(lpszPrinterSetupInfo,_T("PaperSize"),0);


		DEVNAMES* lpDevNames = (DEVNAMES*)::GlobalLock(m_hDevNames);
		bool sizeSettable = false;
	
		//設定済みの用紙が、現在のプリンタで使用可能かどうかチェックする。
		//設定済み用紙が、ユーザー定義でない場合の処理
		if((nPaperSize & DMPAPER_USER) == 0 && nPaperSize!=0){
			CString activePrinterName = (LPCTSTR)lpDevNames+lpDevNames->wDeviceOffset;
			int count = ::DeviceCapabilities(strDevice,NULL,DC_PAPERS,NULL,NULL);
			if(count>0){
				WORD *papersizeArray = new WORD[count];
				::DeviceCapabilities(strDevice,NULL,DC_PAPERS,(LPWSTR)papersizeArray,NULL);
				for(int i=0;i<count;i++){
					TRACE(_T("%d "),papersizeArray[i]);
					if(papersizeArray[i] == nPaperSize){
						sizeSettable = true;
					}
				}
				TRACE(_T("\n"));
				delete[]papersizeArray;
			}
		}

		DEVMODE* lpDevMode = (DEVMODE*)::GlobalLock(m_hDevMode);
		if(nOrientation!=0) lpDevMode->dmOrientation = nOrientation;
		if(nPaperSize!=0 && sizeSettable)	lpDevMode->dmPaperSize   = nPaperSize;
		GlobalUnlock(m_hDevMode);


		GlobalUnlock(m_hDevNames);
	}
}


void CAboutDlg::OnBnClickedButtonSuigyodo()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	CString url;
	url.LoadString(IDS_URL);
	::ShellExecute(
		NULL,              // 親ウィンドウのハンドル
		_T("open"),         // 操作
		url,         // 操作対象のファイル
		NULL,   // 操作のパラメータ
		NULL,    // 既定のディレクトリ
		SW_SHOWDEFAULT            // 表示状態
	);
}




BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	CString msg;
	CString strVersion;
	strVersion.LoadString(IDS_VERSION);
	msg = "BSch3V Version ";
	msg += strVersion;
	SetDlgItemText(IDC_STATIC_VERSION,msg);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

BOOL CBSchApp::OnIdle(LONG lCount)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if(g_pViewWindow){
		if(g_pViewWindow->GetSafeHwnd()){
			((CBSchView*)g_pViewWindow)->DisplayMessageLine();
		}
	}
	//↓これでは駄目。プレビュー中はプレビューウィンドウがアクティブビューになる。
	//((CBSchView*)(((CMainFrame*)(AfxGetApp()->GetMainWnd()))->GetActiveView()))->DisplayMessageLine();
	return CWinApp::OnIdle(lCount);
}

void CBSchApp::editWidthLCoV(LPCTSTR name, SCompInfo* pInfo, SCompLib& tempLib)
{
	TCHAR dir[_MAX_PATH];
	TCHAR path[_MAX_PATH];
	TCHAR cmdline[_MAX_PATH*2+10];

	::GetTempPath(_MAX_PATH,dir);
	if(::GetTempFileName(dir,_T("b3v"),0,path)==0) return;

	FILE* fp;
	if((fp = _tfopen(path,_T("wt, ccs=UTF-8")))==NULL) return;
	fseek( fp, 0, SEEK_SET ) ; //BAD KNOWHOW for removing BOM CODE.

	SWriteCE3 wce3;
	wce3.Attach(fp);
	wce3.WriteRecord(_T("+BSCH3_LIB_V.1.0"));	wce3.WriteEOL();
	wce3.WriteRecordInt(_T("VER"),SCompLib::m_nConstDataVersion);	wce3.WriteEOL();

	//SCompInfo* pInfo = m_pPartIndex->compInfo();
	//pInfo->writeCe3(wce3,m_rOrgName);
	pInfo->writeCe3(wce3,name);
	wce3.WriteRecord(_T("-BSCH3_LIB_V.1.0"));
	wce3.WriteEOL();
	fclose(fp);

	LPCTSTR pLCoVPath =  ((CBSchApp*)AfxGetApp())->LCoVPath();

	_stprintf(cmdline,_T("\"%s\" \"%s\""),pLCoVPath,path);
	//TRACE(_T("%s\n"),cmdline);
	//g_log.append(cmdline);
	//g_log.append(_T("\n"));


	PROCESS_INFORMATION pi;
	STARTUPINFO si;

    ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    BOOL resProc = CreateProcess(NULL, cmdline, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(!resProc) return;
	
	while(WAIT_TIMEOUT==WaitForSingleObject(pi.hProcess,1)){
		MSG msg;
		if(GetMessage(&msg,NULL,0,0)){
			if(msg.message == WM_PAINT){				//	メッセージが WM_PAINT なら
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	TRACE("End LCoV\n");
	CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

	tempLib.deleteContent();
	tempLib.readLibraryFile(path);
	DeleteFile(path);	
}