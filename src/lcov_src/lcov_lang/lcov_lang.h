// lcov_lang.h : lcov_lang.DLL �̃��C�� �w�b�_�[ �t�@�C��
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"		// ���C�� �V���{��


// Clcov_langApp
// ���̃N���X�̎����Ɋւ��Ă� lcov_lang.cpp ���Q�Ƃ��Ă��������B
//

class Clcov_langApp : public CWinApp
{
public:
	Clcov_langApp();

// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
