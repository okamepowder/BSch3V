/****************************************************************************
    PL3W partslist utility for BSch3V.
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���A
//            �܂��͎Q�Ɖ񐔂������A�����܂�ύX����Ȃ�
//            �v���W�F�N�g��p�̃C���N���[�h �t�@�C�����L�q���܂��B
//
#pragma once

#define _BIND_TO_CURRENT_VCLIBS_VERSION 1

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Windows �w�b�_�[����g�p����Ă��Ȃ����������O���܂��B
#endif

#include "targetver.h"


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// �ꕔ�� CString �R���X�g���N�^�͖����I�ł��B

// ��ʓI�Ŗ������Ă����S�� MFC �̌x�����b�Z�[�W�̈ꕔ�̔�\�����������܂��B
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC �̃R�A����ѕW���R���|�[�l���g
#include <afxext.h>         // MFC �̊g������
#include <afxdisp.h>        // MFC �I�[�g���[�V���� �N���X

#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �R���� �R���g���[�� �T�|�[�g
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �R���� �R���g���[�� �T�|�[�g
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <Shlwapi.h>


//#define _BIND_TO_CURRENT_VCLIBS_VERSION 1

//#if !defined(AFX_STDAFX_H__5B347A42_030D_4720_AE83_E2F0709D5326__INCLUDED_)
//#define AFX_STDAFX_H__5B347A42_030D_4720_AE83_E2F0709D5326__INCLUDED_
//
//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
//
//#define VC_EXTRALEAN		// Windows �w�b�_�[����w�ǎg�p����Ȃ��X�^�b�t�����O���܂��B
//
//#include <afxwin.h>         // MFC �̃R�A����ѕW���R���|�[�l���g
//#include <afxext.h>         // MFC �̊g������
//#include <afxdisp.h>        // MFC �̃I�[�g���[�V���� �N���X
//#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �R���� �R���g���[�� �T�|�[�g
//#ifndef _AFX_NO_AFXCMN_SUPPORT
//#include <afxcmn.h>			// MFC �� Windows �R���� �R���g���[�� �T�|�[�g
//#endif // _AFX_NO_AFXCMN_SUPPORT
//
//
////{{AFX_INSERT_LOCATION}}
//// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B
//
//#endif // !defined(AFX_STDAFX_H__5B347A42_030D_4720_AE83_E2F0709D5326__INCLUDED_)




//
//
//#if !defined(AFX_STDAFX_H__5B347A42_030D_4720_AE83_E2F0709D5326__INCLUDED_)
//#define AFX_STDAFX_H__5B347A42_030D_4720_AE83_E2F0709D5326__INCLUDED_
//
//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
//
//#define VC_EXTRALEAN		// Windows �w�b�_�[����w�ǎg�p����Ȃ��X�^�b�t�����O���܂��B
//
//
//
//#include <afxwin.h>         // MFC �̃R�A����ѕW���R���|�[�l���g
//#include <afxext.h>         // MFC �̊g������
//#include <afxdisp.h>        // MFC �̃I�[�g���[�V���� �N���X
//#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �R���� �R���g���[�� �T�|�[�g
//#ifndef _AFX_NO_AFXCMN_SUPPORT
//#include <afxcmn.h>			// MFC �� Windows �R���� �R���g���[�� �T�|�[�g
//#endif // _AFX_NO_AFXCMN_SUPPORT
//
//
////{{AFX_INSERT_LOCATION}}
//// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B
//
//#endif // !defined(AFX_STDAFX_H__5B347A42_030D_4720_AE83_E2F0709D5326__INCLUDED_)
