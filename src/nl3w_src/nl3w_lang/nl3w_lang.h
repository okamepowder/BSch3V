// nl3w_lang.h : nl3w_lang.DLL �̃��C�� �w�b�_�[ �t�@�C��
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"		// ���C�� �V���{��


// Cnl3w_langApp
// ���̃N���X�̎����Ɋւ��Ă� nl3w_lang.cpp ���Q�Ƃ��Ă��������B
//

class Cnl3w_langApp : public CWinApp
{
public:
	Cnl3w_langApp();

// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
