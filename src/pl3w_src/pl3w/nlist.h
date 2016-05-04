/****************************************************************************
    PL3W partslist utility for BSch3V.
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/


// nlist.h : NLIST �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#if !defined(AFX_NLIST_H__4D8E6AF1_916A_40CF_856D_3E12A75CF47D__INCLUDED_)
#define AFX_NLIST_H__4D8E6AF1_916A_40CF_856D_3E12A75CF47D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��

#include "cmdlineparam.h"

/////////////////////////////////////////////////////////////////////////////
// CNlistApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� nlist.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CNlistApp : public CWinApp
{
public:
	CNlistApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CNlistApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����

	//{{AFX_MSG(CNlistApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	SCmdLineParam m_cmdInfo;
private:
	TCHAR *m_pszIniFileName;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_NLIST_H__4D8E6AF1_916A_40CF_856D_3E12A75CF47D__INCLUDED_)
