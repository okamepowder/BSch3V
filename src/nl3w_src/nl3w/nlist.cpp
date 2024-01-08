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
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNlistApp �N���X�̍\�z

CNlistApp::CNlistApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B

	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
//	TCHAR pathModule[_MAX_PATH];
	TCHAR pathExe[_MAX_PATH];


	m_pszIniFileName = new TCHAR[_MAX_PATH];		//�p�X���i�[�G���A�̊m��
												//MFC�������delete����̂ŁA�Œ�G���A���m�ۂ��Ă͂����Ȃ��B

	//���W���[���̃p�X�𒲂ׂ�B
	::GetModuleFileName(NULL,pathExe,_MAX_PATH);

	//Windows 2000/XP�ɂ�����
	//\Documents and Settings\username\Local Settings\Application Data\SuigyodoONLINE\NL3W.INI ��T���B
	//���ꂪ���������Ƃ��́A�����INI�t�@�C���Ƃ��Ďg���B
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
					m_pszProfileName=m_pszIniFileName;	//�쐬����������̃|�C���^��m_pszProfileName�ɃR�s�[
					return;
				}
			}
		}
	}

	//�ǂ��ɂ�������Ȃ������Ƃ��ɂ̓��W���[���̊i�[�t�H���_�� INI�t�@�C�����g��
	//�����ɂ�INI�t�@�C����������Ȃ��Ƃ��́A������INI�t�@�C�����V�K�쐬����邪�A���̏ꍇ�A�V�K�쐬�����O��0xFF,0xFE�����̃t�@�C��������Ă���
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


	m_pszProfileName=m_pszIniFileName;	//�쐬����������̃|�C���^��m_pszProfileName�ɃR�s�[

}

/////////////////////////////////////////////////////////////////////////////
// �B��� CNlistApp �I�u�W�F�N�g

CNlistApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CNlistApp �N���X�̏�����


BOOL CNlistApp::InitInstance()
{

	ParseCommandLine(m_cmdInfo);

	//AfxEnableControlContainer();

	// �W���I�ȏ���������
	// ���������̋@�\���g�p�����A���s�t�@�C���̃T�C�Y��������������
	//  ��Έȉ��̓���̏��������[�`���̒�����s�K�v�Ȃ��̂��폜����
	//  ���������B

#ifdef _AFXDLL
	Enable3dControls();			// ���L DLL ���� MFC ���g���ꍇ�͂������R�[�����Ă��������B
#else
	Enable3dControlsStatic();	// MFC �ƐÓI�Ƀ����N����ꍇ�͂������R�[�����Ă��������B
#endif

	//�������� Version 0.69 20101017
	//����DLL�̓ǂݍ��݃p�X�����s�t�@�C���̃f�B���N�g���ɐ���
	TCHAR moduleFileName[_MAX_PATH];
	TCHAR dllPath[_MAX_PATH];
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	::GetModuleFileName(NULL,moduleFileName,_MAX_PATH);
	::_tsplitpath(moduleFileName,drive,dir,NULL,NULL);
	::_tmakepath(dllPath,drive,dir,_T("nl3w_lang"),_T("dll"));
	//�����܂� Version 0.69 20101017


//���ꃊ�\�[�XDLL��ǂݍ��ޑO�̃��\�[�X���̃o�[�W�������ƃ��\�[�X�n���h���̕ۑ�
	CString strVersion;
	strVersion.LoadString(IDS_VERSION);
	HINSTANCE hInstRcOrg = AfxGetResourceHandle();
	HINSTANCE hInstRc = LoadLibrary( dllPath );
	if ( hInstRc){				// ���ꃊ�\�[�XDLL����������.
		AfxSetResourceHandle(hInstRc);
		//���ꃊ�\�[�X���̃o�[�W���������`�F�b�N
		CString strVersionAddRc;
		strVersionAddRc.LoadString(IDS_VERSION);
		//�o�[�W��������v���Ȃ���΁A���̃��\�[�X�n���h���ɖ߂�
		if(strVersion.Compare(strVersionAddRc)!=0){
			AfxSetResourceHandle(hInstRcOrg);
		}
	}


	//HINSTANCE hInstRc = ::LoadLibrary( "nl3w_lang.dll");
	//if ( hInstRc){				// ���ꃊ�\�[�XDLL����������.
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
		// TODO: �_�C�A���O�� <OK> �ŏ����ꂽ���̃R�[�h��
		//       �L�q���Ă��������B
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �_�C�A���O�� <��ݾ�> �ŏ����ꂽ���̃R�[�h��
		//       �L�q���Ă��������B
	}

	// �_�C�A���O�������Ă���A�v���P�[�V�����̃��b�Z�[�W �|���v���J�n������́A
	// �A�v���P�[�V�������I�����邽�߂� FALSE ��Ԃ��Ă��������B
	return FALSE;
}
