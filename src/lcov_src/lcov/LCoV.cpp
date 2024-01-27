/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/


// LCoV.cpp : �A�v���P�[�V�����̃N���X������`���܂��B
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


// CLCoVApp �R���X�g���N�V����

CLCoVApp::CLCoVApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B

	m_bTempMode = false;
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR pathModule[_MAX_PATH];

	m_szIniFileDir[0] = '\0';

	m_pszIniFileName = new TCHAR[_MAX_PATH];		//�p�X���i�[�G���A�̊m��
												//MFC�������delete����̂ŁA�Œ�G���A���m�ۂ��Ă͂����Ȃ��B

	//���W���[���̃p�X�𒲂ׂ�B
	::GetModuleFileName(NULL,m_moduleFileName,_MAX_PATH);
	::_tsplitpath(m_moduleFileName,pathModule,dir,NULL,NULL);
	lstrcat(pathModule,dir);	//pathModule�Ƀ��W���[���̃f�B���N�g���B������\�L���B

	//�J�����g�f�B���N�g��
	::GetCurrentDirectory(_MAX_PATH,m_pszIniFileName);	//�J�����g�f�B���N�g���𓾂�
	_tcscat(m_pszIniFileName,_T("\\"));			//\�L����ǉ�

	//���W���[���̃p�X�ƃJ�����g�f�B���N�g������v���Ȃ��Ƃ��́A
	//�J�����g�f�B���N�g����LCOV.INI�̗L�����m�F����B
	//�J�����g�f�B���N�g����LCOV.INI�����������Ƃ��͂����INI�t�@�C���Ƃ��Ďg���B
	if(lstrcmp(pathModule,m_pszIniFileName)!=0){
		lstrcat(m_pszIniFileName,_T("LCOV.INI"));			//LCOV.INI��ǉ�
		FILE *pf=_tfopen(m_pszIniFileName,_T("rt, ccs=UTF-8"));
		if(pf){
			fclose(pf);
			goto EndOfFunc;
		}
	}

	// ���NT
	//����Windows 2000/XP�ɂ�����
	//\Documents and Settings\username\Local Settings\Application Data\SuigyodoONLINE\LCOV.INI ��T���B
	//���ꂪ���������Ƃ��́A�����INI�t�@�C���Ƃ��Ďg���B
	if(SHGetSpecialFolderPath(NULL,m_pszIniFileName,CSIDL_LOCAL_APPDATA,FALSE)){
		TRACE(_T("CBSchApp::CBSchApp() %s\n"),m_pszIniFileName);
		_tcscat(m_pszIniFileName,_T("\\SuigyodoONLINE\\LCOV.INI"));
		FILE *pf=_tfopen(m_pszIniFileName,_T("rt, ccs=UTF-8"));
		if(pf){
			fclose(pf);
			goto EndOfFunc;
		}
	}

	//�ǂ��ɂ�������Ȃ������Ƃ��ɂ̓��W���[���̊i�[�t�H���_�� LCOV.INI���g��
	//�����ɂ�LCOV.INI��������Ȃ��Ƃ��́A������LCOV.INI���V�K�쐬����邪�A���̏ꍇ�A�V�K�쐬�����O��0xFF,0xFE�����̃t�@�C��������Ă���
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
	m_pszProfileName=m_pszIniFileName;	//�쐬����������̃|�C���^��m_pszProfileName�ɃR�s�[
	::_tsplitpath(m_pszIniFileName,m_szIniFileDir,dir,NULL,NULL);
	::_tcscat(m_szIniFileDir,dir);
}


// �B��� CLCoVApp �I�u�W�F�N�g�ł��B

CLCoVApp theApp;

// CLCoVApp ������

BOOL CLCoVApp::InitInstance()
{
	// �A�v���P�[�V�����@�}�j�t�F�X�g���@visual �X�^�C����L���ɂ��邽�߂ɁA
	// ComCtl32.dll �o�[�W���� 6�@�ȍ~�̎g�p���w�肷��ꍇ�́A
	// Windows XP �Ɂ@InitCommonControls() ���K�v�ł��B�����Ȃ���΁A�E�B���h�E�쐬�͂��ׂĎ��s���܂��B

	//�������� Version 0.69 20101017
	//����DLL�̓ǂݍ��݃p�X�����s�t�@�C���̃f�B���N�g���ɐ���
	TCHAR moduleFileName[_MAX_PATH];
	TCHAR dllPath[_MAX_PATH];
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	::GetModuleFileName(NULL,moduleFileName,_MAX_PATH);
	::_tsplitpath(moduleFileName,drive,dir,NULL,NULL);
	::_tmakepath(dllPath,drive,dir,_T("lcov_lang"),_T("dll"));
	//�����܂� Version 0.69 20101017



	CString strVersion;
	strVersion.LoadString(IDS_VERSION);
	HINSTANCE hInstRcOrg = AfxGetResourceHandle();

	// �p�~�F ���ꃊ�\�[�X


	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	//�E�B���h�E�T�C�Y�̕���
	int cmdShow;
	int mainframeCx;
	int mainframeCy;
	CMainFrame::IniReadWindowPosition(cmdShow,mainframeCx,mainframeCy);
	if(cmdShow==SW_SHOWMAXIMIZED){	//�ő剻����Ƃ������O��I�����̍ő剻�����g���B����ȊO��Windows�̎w��ɏ]��
		m_nCmdShow = SW_SHOWMAXIMIZED;
	}
	CMainFrame::SetInitialWindowSize(mainframeCx,mainframeCy);




	InitCommonControls();

	CWinApp::InitInstance();

	// OLE ���C�u���������������܂��B
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// �W��������
	// �����̋@�\���g�킸�ɁA�ŏI�I�Ȏ��s�\�t�@�C���̃T�C�Y���k���������ꍇ�́A
	// �ȉ�����A�s�v�ȏ��������[�`����
	// �폜���Ă��������B
	// �ݒ肪�i�[����Ă��郌�W�X�g�� �L�[��ύX���܂��B
	// TODO: ���̕�������A��Ж��܂��͑g�D���Ȃǂ́A
	// �K�؂ȕ�����ɕύX���Ă��������B
	//SetRegistryKey(_T("�A�v���P�[�V���� �E�B�U�[�h�Ő������ꂽ���[�J�� �A�v���P�[�V����"));
	// ���C�� �E�C���h�E���쐬����Ƃ��A���̃R�[�h�͐V�����t���[�� �E�C���h�E �I�u�W�F�N�g���쐬���A
	// ������A�v���P�[�V�����̃��C�� �E�C���h�E�ɃZ�b�g���܂�
	LoadStdProfileSettings();  // �W���� INI �t�@�C���̃I�v�V���������[�ނ��܂� (MRU ���܂�)

	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// �t���[�������\�[�X���烍�[�h���č쐬���܂�
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);
	// ���C�� �E�B���h�E�����������ꂽ�̂ŁA�\���ƍX�V���s���܂��B
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
	// �ڔ��������݂���ꍇ�ɂ̂� DragAcceptFiles ���Ăяo���Ă��������B
	//  SDI �A�v���P�[�V�����ł́AProcessShellCommand �̒���ɂ��̌Ăяo�����������Ȃ���΂Ȃ�܂���B
	return TRUE;
}


// CLCoVApp ���b�Z�[�W �n���h��



// �A�v���P�[�V�����̃o�[�W�������Ɏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ����
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

// �_�C�A���O�����s���邽�߂̃A�v���P�[�V���� �R�}���h
void CLCoVApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CLCoVApp ���b�Z�[�W �n���h��


BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �����ɏ�������ǉ����Ă�������
	CString msg;
	CString strVersion;
	strVersion.LoadString(IDS_VERSION);
	msg = "LCoV Version ";
	msg += strVersion;
	SetDlgItemText(IDC_STATIC_VERSION,msg);


	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}
