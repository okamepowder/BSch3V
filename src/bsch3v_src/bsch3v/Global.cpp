/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// Global.cpp : �O���[�o���ϐ��̒�`���s��
//

#include "stdafx.h"
#include "BSch.h"

#include <string>
using namespace std;

//#include "PartLib.h"
//#include "BSchFont.h"

//////////////////////////////////////////////////////////////////////
//��ʕ\���֘A
BOOL g_bDisplayPartFrame;	//���i�g�̕\��   1997/01/01 CBSchView����ړ�
BOOL g_bDisplayNcPinMark;	//NC�s���̃}�[�N�̕\��
BOOL g_bDisplayHiddenPinNum;	//��\���s���ԍ��̕\�� 0.63.01


CWnd* g_pViewWindow;


//////////////////////////////////////////////////////////////////////
//���֘A
BOOL g_bJapaneseLangEnv;

//////////////////////////////////////////////////////////////////////
//�A�N�e�B�u�ȃh�L�������g�̃p�X���̃R�s�[

wstring g_docPath;
wstring g_docTimeStamp;
wstring g_docDate;
wstring g_printTime;



//////////////////////////////////////////////////////////////////////
//���O�֘A

//wstring g_log;
//TCHAR g_logPath[_MAX_PATH];



//////////////////////////////////////////////////////////////////////
//���W�֘A
//�Q�_�̍��W�����`�͈͂�ݒ肷��
//void g_MakeRECT(LPPOINT p1,LPPOINT p2,LPRECT prc)
//{
//	if(p1->x < p2->x){
//		prc->left =p1->x;
//		prc->right=p2->x;
//	}else{
//		prc->left =p2->x;
//		prc->right=p1->x;
//	}
//	if(p1->y < p2->y){
//		prc->top   =p1->y;
//		prc->bottom=p2->y;
//	}else{
//		prc->top   =p2->y;
//		prc->bottom=p1->y;
//	}
//}

////��`���W�̏㉺���E������ւ���Ă���ꍇ�ɐ���������
//void g_CorrectRECT(LPRECT prc)
//{
//	int n;
//	if(prc->top > prc->bottom){
//		n=prc->top;
//		prc->top=prc->bottom;
//		prc->bottom=n;
//	}
//	if(prc->left > prc->right){
//		n=prc->left;
//		prc->left=prc->right;
//		prc->right=n;
//	}
//}

//��`2����`1�̓����i�܂��͓����j�ɂ��邩�ǂ������m���߂�
//BOOL g_IsRect2insideRect1(LPRECT prc1,LPRECT prc2)
//{
//	if(  prc2->left   >= prc1->left
//	  && prc2->left   <= prc1->right
//	  && prc2->right  >= prc1->left
//	  && prc2->right  <= prc1->right
//	  && prc2->top    >= prc1->top
//	  && prc2->top    <= prc1->bottom
//	  && prc2->bottom >= prc1->top
//	  && prc2->bottom <= prc1->bottom
//	)return TRUE;
//	else return FALSE;
//}

//////////////////////////////////////////////////////////////////////
//�N���b�v�{�[�h�֌W
//const char szClipboardFormatName[]="BSch CE2 format data";
//UINT nClipboardFormat;

////�N���b�v�{�[�h�̃t�H�[�}�b�g�̒l�𓾂�
//UINT g_GetClipboardFormat()
//{
//	return nClipboardFormat;
//}

////�N���b�v�{�[�h�̃t�H�[�}�b�g�̓o�^������
//UINT g_RegisterClipboardFormat()
//{
//	nClipboardFormat=::RegisterClipboardFormat(szClipboardFormatName);
//	return nClipboardFormat;
//}


