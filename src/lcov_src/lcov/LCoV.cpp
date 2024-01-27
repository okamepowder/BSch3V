/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/


// LCoV.cpp : アプリケーションのクラス動作を定義します。
//

#include "stdafx.h"
#include "LCoV.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLCoVApp

BEGIN_MESSAGE_MAP(CLCoVApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
END_MESSAGE_MAP()


// CLCoVApp コンストラクション

CLCoVApp::CLCoVApp()
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。

	m_bTempMode = false;
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR pathModule[_MAX_PATH];

	m_szIniFileDir[0] = '\0';

	m_pszIniFileName = new TCHAR[_MAX_PATH];		//パス名格納エリアの確保
												//MFCがこれをdeleteするので、固定エリアを確保してはいけない。

	//モジュールのパスを調べる。
	::GetModuleFileName(NULL,m_moduleFileName,_MAX_PATH);
	::_tsplitpath(m_moduleFileName,pathModule,dir,NULL,NULL);
	lstrcat(pathModule,dir);	//pathModuleにモジュールのディレクトリ。末尾は\記号。

	//カレントディレクトリ
	::GetCurrentDirectory(_MAX_PATH,m_pszIniFileName);	//カレントディレクトリを得る
	_tcscat(m_pszIniFileName,_T("\\"));			//\記号を追加

	//モジュールのパスとカレントディレクトリが一致しないときは、
	//カレントディレクトリのLCOV.INIの有無を確認する。
	//カレントディレクトリにLCOV.INIが見つかったときはこれをINIファイルとして使う。
	if(lstrcmp(pathModule,m_pszIniFileName)!=0){
		lstrcat(m_pszIniFileName,_T("LCOV.INI"));			//LCOV.INIを追加
		FILE *pf=_tfopen(m_pszIniFileName,_T("rt, ccs=UTF-8"));
		if(pf){
			fclose(pf);
			goto EndOfFunc;
		}
	}

	// 常にNT
	//次にWindows 2000/XPにおいて
	//\Documents and Settings\username\Local Settings\Application Data\SuigyodoONLINE\LCOV.INI を探す。
	//これが見つかったときは、これをINIファイルとして使う。
	if(SHGetSpecialFolderPath(NULL,m_pszIniFileName,CSIDL_LOCAL_APPDATA,FALSE)){
		TRACE(_T("CBSchApp::CBSchApp() %s\n"),m_pszIniFileName);
		_tcscat(m_pszIniFileName,_T("\\SuigyodoONLINE\\LCOV.INI"));
		FILE *pf=_tfopen(m_pszIniFileName,_T("rt, ccs=UTF-8"));
		if(pf){
			fclose(pf);
			goto EndOfFunc;
		}
	}

	//どこにも見つからなかったときにはモジュールの格納フォルダの LCOV.INIを使う
	//ここにもLCOV.INIが見つからないときは、ここにLCOV.INIが新規作成されるが、その場合、新規作成される前に0xFF,0xFEだけのファイルを作っておく
	::_tsplitpath(m_moduleFileName,drive,dir,NULL,NULL);
	::_tmakepath(m_pszIniFileName,drive,dir,_T("LCOV"),_T(".INI"));
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


// 唯一の CLCoVApp オブジェクトです。

CLCoVApp theApp;

// CLCoVApp 初期化

BOOL CLCoVApp::InitInstance()
{
	// アプリケーション　マニフェストが　visual スタイルを有効にするために、
	// ComCtl32.dll バージョン 6　以降の使用を指定する場合は、
	// Windows XP に　InitCommonControls() が必要です。さもなければ、ウィンドウ作成はすべて失敗します。

	//ここから Version 0.69 20101017
	//言語DLLの読み込みパスを実行ファイルのディレクトリに制限
	TCHAR moduleFileName[_MAX_PATH];
	TCHAR dllPath[_MAX_PATH];
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	::GetModuleFileName(NULL,moduleFileName,_MAX_PATH);
	::_tsplitpath(moduleFileName,drive,dir,NULL,NULL);
	::_tmakepath(dllPath,drive,dir,_T("lcov_lang"),_T("dll"));
	//ここまで Version 0.69 20101017



	CString strVersion;
	strVersion.LoadString(IDS_VERSION);
	HINSTANCE hInstRcOrg = AfxGetResourceHandle();

	// 廃止： 言語リソース


	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	//ウィンドウサイズの復元
	int cmdShow;
	int mainframeCx;
	int mainframeCy;
	CMainFrame::IniReadWindowPosition(cmdShow,mainframeCx,mainframeCy);
	if(cmdShow==SW_SHOWMAXIMIZED){	//最大化するときだけ前回終了時の最大化情報を使う。それ以外はWindowsの指定に従う
		m_nCmdShow = SW_SHOWMAXIMIZED;
	}
	CMainFrame::SetInitialWindowSize(mainframeCx,mainframeCy);




	InitCommonControls();

	CWinApp::InitInstance();

	// OLE ライブラリを初期化します。
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 標準初期化
	// これらの機能を使わずに、最終的な実行可能ファイルのサイズを縮小したい場合は、
	// 以下から、不要な初期化ルーチンを
	// 削除してください。
	// 設定が格納されているレジストリ キーを変更します。
	// TODO: この文字列を、会社名または組織名などの、
	// 適切な文字列に変更してください。
	//SetRegistryKey(_T("アプリケーション ウィザードで生成されたローカル アプリケーション"));
	// メイン ウインドウを作成するとき、このコードは新しいフレーム ウインドウ オブジェクトを作成し、
	// それをアプリケーションのメイン ウインドウにセットします
	LoadStdProfileSettings();  // 標準の INI ファイルのオプションをローﾄﾞします (MRU を含む)

	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// フレームをリソースからロードして作成します
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);
	// メイン ウィンドウが初期化されたので、表示と更新を行います。
	m_pMainWnd->ShowWindow(m_nCmdShow);
	//pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	if(!cmdInfo.m_strFileName.IsEmpty()){
		pFrame->DoOpen(cmdInfo.m_strFileName);
		TCHAR szExt[_MAX_EXT];
		_tsplitpath(cmdInfo.m_strFileName,NULL,NULL,NULL,szExt);
		if(_tcsicmp(szExt,_T(".tmp"))==0){
			m_bTempMode = true;
		}
	}
	// 接尾辞が存在する場合にのみ DragAcceptFiles を呼び出してください。
	//  SDI アプリケーションでは、ProcessShellCommand の直後にこの呼び出しが発生しなければなりません。
	return TRUE;
}


// CLCoVApp メッセージ ハンドラ



// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// ダイアログを実行するためのアプリケーション コマンド
void CLCoVApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CLCoVApp メッセージ ハンドラ


BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	CString msg;
	CString strVersion;
	strVersion.LoadString(IDS_VERSION);
	msg = "LCoV Version ";
	msg += strVersion;
	SetDlgItemText(IDC_STATIC_VERSION,msg);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
