// pl3w_lang.h : pl3w_lang.DLL �̃��C�� �w�b�_�[ �t�@�C��
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"		// ���C�� �V���{��


// Cpl3w_langApp
// ���̃N���X�̎����Ɋւ��Ă� pl3w_lang.cpp ���Q�Ƃ��Ă��������B
//

class Cpl3w_langApp : public CWinApp
{
public:
	Cpl3w_langApp();

// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
