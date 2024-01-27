
// CE3Search.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CE3Search.h"
#include "CE3SearchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCE3SearchApp

//BEGIN_MESSAGE_MAP(CCE3SearchApp, CWinAppEx)
BEGIN_MESSAGE_MAP(CCE3SearchApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCE3SearchApp construction

CCE3SearchApp::CCE3SearchApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
//	TCHAR pathModule[_MAX_PATH];
	TCHAR pathExe[_MAX_PATH];


	m_pszIniFileName = new TCHAR[_MAX_PATH];		//パス名格納エリアの確保
												//MFCがこれをdeleteするので、固定エリアを確保してはいけない。

	//モジュールのパスを調べる。
	::GetModuleFileName(NULL,pathExe,_MAX_PATH);

	// 常にNT
	//Windows 2000/XPにおいて
	//\Documents and Settings\username\Local Settings\Application Data\SuigyodoONLINE\NL3W.INI を探す。
	//これが見つかったときは、これをINIファイルとして使う。
	if(SHGetSpecialFolderPath(NULL,m_pszIniFileName,CSIDL_LOCAL_APPDATA,FALSE)){
		TRACE("CBSchApp::CBSchApp() %s\n",m_pszIniFileName);
		_tcscat(m_pszIniFileName,_T("\\SuigyodoONLINE\\CE3Search.INI"));
		FILE *pf=_tfopen(m_pszIniFileName,_T("rt, ccs=UTF-8"));
		if(pf){
			fclose(pf);
			m_pszProfileName=m_pszIniFileName;	//作成した文字列のポインタをm_pszProfileNameにコピー
			return;
		}
	}

	//上記フォルダに見つからなかったときにはモジュールの格納フォルダの CE3Search.INIを使う
	//ここにもCE3Search.INIが見つからないときは、ここにCE3Search.INIが新規作成されるが、その場合、新規作成される前に0xFF,0xFEだけのファイルを作っておく
	::_tsplitpath(pathExe,drive,dir,NULL,NULL);
	::_tmakepath_s(m_pszIniFileName,_MAX_PATH,drive,dir,_T("CE3Search"),_T(".INI"));
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


// The one and only CCE3SearchApp object

CCE3SearchApp theApp;


// CCE3SearchApp initialization

BOOL CCE3SearchApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	//CWinAppEx::InitInstance();
	CWinApp::InitInstance();

	ParseCommandLine(m_cmdInfo);

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	//SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CCE3SearchDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
