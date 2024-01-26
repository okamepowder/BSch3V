/****************************************************************************
    NUT3W Numbering software for BSch3V
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// nu3w.cpp : �A�v���P�[�V�����̃N���X������`���܂��B
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


// Cnu3wApp �R���X�g���N�V����

Cnu3wApp::Cnu3wApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�R�[�h��ǉ����Ă��������B
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
	//\Documents and Settings\username\Local Settings\Application Data\SuigyodoONLINE\NUT3W.INI ��T���B
	//���ꂪ���������Ƃ��́A�����INI�t�@�C���Ƃ��Ďg���B
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
					m_pszProfileName=m_pszIniFileName;	//�쐬����������̃|�C���^��m_pszProfileName�ɃR�s�[
					return;
				}
			}
		}
	}

	//�ǂ��ɂ�������Ȃ������Ƃ��ɂ̓��W���[���̊i�[�t�H���_�� INI�t�@�C�����g��
	//�����ɂ�INI�t�@�C����������Ȃ��Ƃ��́A������INI�t�@�C�����V�K�쐬����邪�A���̏ꍇ�A�V�K�쐬�����O��0xFF,0xFE�����̃t�@�C��������Ă���
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

	m_pszProfileName=m_pszIniFileName;	//�쐬����������̃|�C���^��m_pszProfileName�ɃR�s�[


	g_maxInitValue =  AfxGetApp()->GetProfileInt(_T("OPTION"),_T("MaxInitValue"),1000);
	AfxGetApp()->WriteProfileInt(_T("OPTION"),_T("MaxInitValue"),g_maxInitValue);
	g_maxStepValue =  AfxGetApp()->GetProfileInt(_T("OPTION"),_T("MaxStepValue"),20);
	AfxGetApp()->WriteProfileInt(_T("OPTION"),_T("MaxStepValue"),g_maxStepValue);

	if(g_maxInitValue<100) g_maxInitValue=100;
	else if(g_maxInitValue>100000) g_maxInitValue=100000;

	if(g_maxStepValue<2) g_maxStepValue=2;
	else if(g_maxStepValue>100) g_maxStepValue=100;


}


// �B��� Cnu3wApp �I�u�W�F�N�g�ł��B

Cnu3wApp theApp;


// Cnu3wApp ������

BOOL Cnu3wApp::InitInstance()
{
	ParseCommandLine(m_cmdInfo);


	//�������� Version 0.69 20101016
	//����DLL�̓ǂݍ��݃p�X�����s�t�@�C���̃f�B���N�g���ɐ���
	TCHAR moduleFileName[_MAX_PATH];
	TCHAR dllPath[_MAX_PATH];
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	::GetModuleFileName(NULL,moduleFileName,_MAX_PATH);
	::_tsplitpath(moduleFileName,drive,dir,NULL,NULL);
	::_tmakepath(dllPath,drive,dir,_T("nu3w_lang"),_T("dll"));
	//�����܂� Version 0.69 20101016



	//���ꃊ�\�[�XDLL��ǂݍ��ޑO�̃��\�[�X���̃o�[�W�������ƃ��\�[�X�n���h���̕ۑ�
	CString strVersion;
	strVersion.LoadString(IDS_VERSION);
	HINSTANCE hInstRcOrg = AfxGetResourceHandle();

	// �p�~: ���ꃊ�\�[�X



	//HINSTANCE hInstRc = LoadLibrary( "nu3w_lang.dll");
	//if ( hInstRc){				// ���ꃊ�\�[�XDLL����������.
	//	AfxSetResourceHandle(hInstRc);
	//}

	// �A�v���P�[�V�����@�}�j�t�F�X�g���@visual �X�^�C����L���ɂ��邽�߂ɁA
	// ComCtl32.dll �o�[�W���� 6�@�ȍ~�̎g�p���w�肷��ꍇ�́A
	// Windows XP �Ɂ@InitCommonControls() ���K�v�ł��B�����Ȃ���΁A�E�B���h�E�쐬�͂��ׂĎ��s���܂��B
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// �W��������
	// �����̋@�\���g�킸�ɁA�ŏI�I�Ȏ��s�\�t�@�C���̃T�C�Y���k���������ꍇ�́A
	// �ȉ�����A�s�v�ȏ��������[�`����
	// �폜���Ă��������B
	// �ݒ肪�i�[����Ă��郌�W�X�g�� �L�[��ύX���܂��B
	// TODO: ���̕�������A��Ж��܂��͑g�D���Ȃǂ́A
	// �K�؂ȕ�����ɕύX���Ă��������B
	// SetRegistryKey(_T("�A�v���P�[�V���� �E�B�U�[�h�Ő������ꂽ���[�J�� �A�v���P�[�V����"));

	Cnu3wDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �_�C�A���O�� <OK> �ŏ����ꂽ���̃R�[�h��
		//       �L�q���Ă��������B
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �_�C�A���O�� <�L�����Z��> �ŏ����ꂽ���̃R�[�h��
		//       �L�q���Ă��������B
	}

	// �_�C�A���O�͕����܂����B�A�v���P�[�V�����̃��b�Z�[�W �|���v���J�n���Ȃ���
	// �A�v���P�[�V�������I�����邽�߂� FALSE ��Ԃ��Ă��������B
	return FALSE;
}
