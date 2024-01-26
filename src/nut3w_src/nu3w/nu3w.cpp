/****************************************************************************
    NUT3W Numbering software for BSch3V
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// nu3w.cpp : アプリケーションのクラス動作を定義します。
//

#include "stdafx.h"
#include "nu3w.h"
#include "nu3wDlg.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cnu3wApp

BEGIN_MESSAGE_MAP(Cnu3wApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// Cnu3wApp コンストラクション

Cnu3wApp::Cnu3wApp()
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。


	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
//	TCHAR pathModule[_MAX_PATH];
	TCHAR pathExe[_MAX_PATH];


	m_pszIniFileName = new TCHAR[_MAX_PATH];		//パス名格納エリアの確保
												//MFCがこれをdeleteするので、固定エリアを確保してはいけない。

	//モジュールのパスを調べる。
	::GetModuleFileName(NULL,pathExe,_MAX_PATH);

	//Windows 2000/XPにおいて
	//\Documents and Settings\username\Local Settings\Application Data\SuigyodoONLINE\NUT3W.INI を探す。
	//これが見つかったときは、これをINIファイルとして使う。
	OSVERSIONINFO versionInfo;
	versionInfo.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	if(GetVersionEx(&versionInfo)){
		if(versionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT){
			if(SHGetSpecialFolderPath(NULL,m_pszIniFileName,CSIDL_LOCAL_APPDATA,FALSE)){
				TRACE("CBSchApp::CBSchApp() %s\n",m_pszIniFileName);
				lstrcat(m_pszIniFileName,_T("\\SuigyodoONLINE\\NUT3W.INI"));
				FILE *pf=_wfopen(m_pszIniFileName,_T("rb"));
				if(pf){
					fclose(pf);
					m_pszProfileName=m_pszIniFileName;	//作成した文字列のポインタをm_pszProfileNameにコピー
					return;
				}
			}
		}
	}

	//どこにも見つからなかったときにはモジュールの格納フォルダの INIファイルを使う
	//ここにもINIファイルが見つからないときは、ここにINIファイルが新規作成されるが、その場合、新規作成される前に0xFF,0xFEだけのファイルを作っておく
	::_tsplitpath(pathExe,drive,dir,NULL,NULL);
	::_tmakepath_s(m_pszIniFileName,_MAX_PATH,drive,dir,_T("NUT3W"),_T(".INI"));
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

	m_pszProfileName=m_pszIniFileName;	//作成した文字列のポインタをm_pszProfileNameにコピー


	g_maxInitValue =  AfxGetApp()->GetProfileInt(_T("OPTION"),_T("MaxInitValue"),1000);
	AfxGetApp()->WriteProfileInt(_T("OPTION"),_T("MaxInitValue"),g_maxInitValue);
	g_maxStepValue =  AfxGetApp()->GetProfileInt(_T("OPTION"),_T("MaxStepValue"),20);
	AfxGetApp()->WriteProfileInt(_T("OPTION"),_T("MaxStepValue"),g_maxStepValue);

	if(g_maxInitValue<100) g_maxInitValue=100;
	else if(g_maxInitValue>100000) g_maxInitValue=100000;

	if(g_maxStepValue<2) g_maxStepValue=2;
	else if(g_maxStepValue>100) g_maxStepValue=100;


}


// 唯一の Cnu3wApp オブジェクトです。

Cnu3wApp theApp;


// Cnu3wApp 初期化

BOOL Cnu3wApp::InitInstance()
{
	ParseCommandLine(m_cmdInfo);


	//ここから Version 0.69 20101016
	//言語DLLの読み込みパスを実行ファイルのディレクトリに制限
	TCHAR moduleFileName[_MAX_PATH];
	TCHAR dllPath[_MAX_PATH];
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	::GetModuleFileName(NULL,moduleFileName,_MAX_PATH);
	::_tsplitpath(moduleFileName,drive,dir,NULL,NULL);
	::_tmakepath(dllPath,drive,dir,_T("nu3w_lang"),_T("dll"));
	//ここまで Version 0.69 20101016



	//言語リソースDLLを読み込む前のリソース中のバージョン情報とリソースハンドルの保存
	CString strVersion;
	strVersion.LoadString(IDS_VERSION);
	HINSTANCE hInstRcOrg = AfxGetResourceHandle();

	// 廃止: 言語リソース



	//HINSTANCE hInstRc = LoadLibrary( "nu3w_lang.dll");
	//if ( hInstRc){				// 言語リソースDLLが見つかった.
	//	AfxSetResourceHandle(hInstRc);
	//}

	// アプリケーション　マニフェストが　visual スタイルを有効にするために、
	// ComCtl32.dll バージョン 6　以降の使用を指定する場合は、
	// Windows XP に　InitCommonControls() が必要です。さもなければ、ウィンドウ作成はすべて失敗します。
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// 標準初期化
	// これらの機能を使わずに、最終的な実行可能ファイルのサイズを縮小したい場合は、
	// 以下から、不要な初期化ルーチンを
	// 削除してください。
	// 設定が格納されているレジストリ キーを変更します。
	// TODO: この文字列を、会社名または組織名などの、
	// 適切な文字列に変更してください。
	// SetRegistryKey(_T("アプリケーション ウィザードで生成されたローカル アプリケーション"));

	Cnu3wDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: ダイアログが <OK> で消された時のコードを
		//       記述してください。
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: ダイアログが <キャンセル> で消された時のコードを
		//       記述してください。
	}

	// ダイアログは閉じられました。アプリケーションのメッセージ ポンプを開始しないで
	// アプリケーションを終了するために FALSE を返してください。
	return FALSE;
}
