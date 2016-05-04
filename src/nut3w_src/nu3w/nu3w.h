/****************************************************************************
    NUT3W Numbering software for BSch3V
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// nu3w.h : PROJECT_NAME �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#pragma once

#ifndef __AFXWIN_H__
	#error ���̃t�@�C���� PCH �Ɋ܂߂�O�ɁA'stdafx.h' ���܂߂Ă��������B
#endif

#include "resource.h"		// ���C�� �V���{��

#include "cmdlineparam.h"
// Cnu3wApp:
// ���̃N���X�̎����ɂ��ẮAnu3w.cpp ���Q�Ƃ��Ă��������B
//

class Cnu3wApp : public CWinApp
{
public:
	Cnu3wApp();

// �I�[�o�[���C�h
	public:
	virtual BOOL InitInstance();
public:
	SCmdLineParam m_cmdInfo;

// ����

	DECLARE_MESSAGE_MAP()

private:
	TCHAR *m_pszIniFileName;
};

extern Cnu3wApp theApp;
