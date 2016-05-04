/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

// LCoV.h : LCoV �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//
#pragma once

#ifndef __AFXWIN_H__
	#error ���̃t�@�C���� PCH �Ɋ܂߂�O�ɁA'stdafx.h' ���܂߂Ă��������B
#endif




// CLCoVApp:
// ���̃N���X�̎����ɂ��ẮALCoV.cpp ���Q�Ƃ��Ă��������B
//

class CLCoVApp : public CWinApp
{
public:
	CLCoVApp();


// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();
	TCHAR m_szIniFileDir[_MAX_PATH];

private:
	TCHAR *m_pszIniFileName;
	TCHAR m_moduleFileName[_MAX_PATH];


// ����

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:
	bool m_bTempMode;
};

extern CLCoVApp theApp;
