/****************************************************************************
    NL3W netlist utility for BSch3V
   Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#include "stdafx.h"
#include "nlist.h"
#include "nlistDlg.h"
#include "cmdlineparam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNlistApp

BEGIN_MESSAGE_MAP(CNlistApp, CWinApp)
	//{{AFX_MSG_MAP(CNlistApp)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNlistApp クラスの構築

CNlistApp::CNlistApp()
{
	// TODO: この位置に構築用のコードを追加してください。
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
	//\Documents and Settings\username\Local Settings\Application Data\SuigyodoONLINE\NL3W.INI を探す。
	//これが見つかったときは、これをINIファイルとして使う。
	OSVERSIONINFO versionInfo;
	versionInfo.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	if(GetVersionEx(&versionInfo)){
		if(versionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT){
			if(SHGetSpecialFolderPath(NULL,m_pszIniFileName,CSIDL_LOCAL_APPDATA,FALSE)){
				TRACE("CBSchApp::CBSchApp() %s\n",m_pszIniFileName);
				lstrcat(m_pszIniFileName,_T("\\SuigyodoONLINE\\NL3W.INI"));
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
	::_tmakepath_s(m_pszIniFileName,_MAX_PATH,drive,dir,_T("NL3W"),_T(".INI"));
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

}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CNlistApp オブジェクト

CNlistApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CNlistApp クラスの初期化


BOOL CNlistApp::InitInstance()
{

	ParseCommandLine(m_cmdInfo);

	//AfxEnableControlContainer();

	// 標準的な初期化処理
	// もしこれらの機能を使用せず、実行ファイルのサイズを小さくしたけ
	//  れば以下の特定の初期化ルーチンの中から不必要なものを削除して
	//  ください。

#ifdef _AFXDLL
	Enable3dControls();			// 共有 DLL 内で MFC を使う場合はここをコールしてください。
#else
	Enable3dControlsStatic();	// MFC と静的にリンクする場合はここをコールしてください。
#endif

	//ここから Version 0.69 20101017
	//言語DLLの読み込みパスを実行ファイルのディレクトリに制限
	TCHAR moduleFileName[_MAX_PATH];
	TCHAR dllPath[_MAX_PATH];
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	::GetModuleFileName(NULL,moduleFileName,_MAX_PATH);
	::_tsplitpath(moduleFileName,drive,dir,NULL,NULL);
	::_tmakepath(dllPath,drive,dir,_T("nl3w_lang"),_T("dll"));
	//ここまで Version 0.69 20101017


//言語リソースDLLを読み込む前のリソース中のバージョン情報とリソースハンドルの保存
	CString strVersion;
	strVersion.LoadString(IDS_VERSION);
	HINSTANCE hInstRcOrg = AfxGetResourceHandle();
	HINSTANCE hInstRc = LoadLibrary( dllPath );
	if ( hInstRc){				// 言語リソースDLLが見つかった.
		AfxSetResourceHandle(hInstRc);
		//言語リソース中のバージョン情報をチェック
		CString strVersionAddRc;
		strVersionAddRc.LoadString(IDS_VERSION);
		//バージョンが一致しなければ、元のリソースハンドルに戻す
		if(strVersion.Compare(strVersionAddRc)!=0){
			AfxSetResourceHandle(hInstRcOrg);
		}
	}


	//HINSTANCE hInstRc = ::LoadLibrary( "nl3w_lang.dll");
	//if ( hInstRc){				// 言語リソースDLLが見つかった.
	//	AfxSetResourceHandle(hInstRc);
	//}



	//HINSTANCE hDllUserDefNet = LoadLibrary("nl3wpads.dll");
	//int ( __stdcall *DllFunc)(void);

	//if(hDllUserDefNet){
	//	DllFunc = (int(__stdcall*)(void))  ::GetProcAddress( hDllUserDefNet,"fnnl3wpads");
	//	if(DllFunc){
	//		TRACE("DLL function value %d\n",DllFunc());
	//	}
	//}

	//::FreeLibrary(hDllUserDefNet);
	
	//HINSTANCE hDllUserDefNet = LoadLibrary("nlusrfmt.dll");
	//int ( __stdcall *DllFunc)(int,int);
	//void( __stdcall *typeName)(TCHAR*,int);

	//if(hDllUserDefNet){
	//	DllFunc = (int(__stdcall*)(int,int))  ::GetProcAddress( hDllUserDefNet,"testfunc");
	//	if(DllFunc){
	//		TRACE("DLL function value %d\n",DllFunc(10,3));
	//	}

	//	typeName = (void(__stdcall*)(TCHAR*,int)) ::GetProcAddress( hDllUserDefNet,"typeName");
	//	if(typeName){
	//		TCHAR buff[20];
	//		typeName(buff,20);
	//		TRACE("typeName %s\n",buff);
	//	}
	//}

	//::FreeLibrary(hDllUserDefNet);
	


	CNlistDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: ダイアログが <OK> で消された時のコードを
		//       記述してください。
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: ダイアログが <ｷｬﾝｾﾙ> で消された時のコードを
		//       記述してください。
	}

	// ダイアログが閉じられてからアプリケーションのメッセージ ポンプを開始するよりは、
	// アプリケーションを終了するために FALSE を返してください。
	return FALSE;
}
