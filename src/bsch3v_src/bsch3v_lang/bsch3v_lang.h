// bsch3v_lang.h : bsch3v_lang.DLL �̃��C�� �w�b�_�[ �t�@�C��
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"		// ���C�� �V���{��


// Cbsch3v_langApp
// ���̃N���X�̎����Ɋւ��Ă� bsch3v_lang.cpp ���Q�Ƃ��Ă��������B
//

class Cbsch3v_langApp : public CWinApp
{
public:
	Cbsch3v_langApp();

// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
