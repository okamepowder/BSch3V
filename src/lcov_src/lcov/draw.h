/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

#pragma once

#include "coord.h"

//�`�惂�[�h �i�ȉ��̒l�̘_���a���Ƃ��Ďg���j
#define DRAW_ON		1	// �ʏ�̕`��
#define DRAW_OFF	2	// �����B�����炭�͔w�i�F�ł̕`����w��
#define DRAW_TEMP	4	// XOR���[�h�ł̕`��
#define DRAW_FAST	8	// �������[�h�ł̕`��B�����炭�͎��̂悤�Ȃ��́j
						//		�E���i�Ȃ�s���ԍ��A�s�����A���i�ԍ��A���i�����ȗ�
						//		�E�^�O�Ȃ當������ȗ�
						//	 ���̑��̑����̐}�ʗv�f�ł͂��̃r�b�g�͈Ӗ��������Ȃ�
#define DRAW_INVERT	16	// BSch��8�h�b�g�������XOR�����ŗL���B���i�ԍ��A���i���̕`��Ŏg�p
#define DRAW_PARTFRAME 		32	//���i�g�̕`��
#define DRAW_DARK	64	// �Â��F�ł̕`��
#define DRAW_INH_PARTNAME 	256	//���i���̕`������Ȃ�
#define DRAW_INH_PARTNUM  	512	//���i�ԍ��̕`������Ȃ�
#define DRAW_INH_PARTBODY  1024	//���i�{�f�B�̕`������Ȃ�
#define DRAW_SWAP_PINNAME_PINNUM 2048

///////////////////////////////////////////////////////////////
// �F
#define COL_JUNC			(RGB(  0,255,  0))
#define COL_WIRE			(RGB(  0,255,  0))
#define COL_BUS				(RGB(  0,255,255))
#define COL_DASH			(RGB(255,255,  0))
#define COL_TAG				(RGB(255,255,  0))
#define COL_TAG_NAME		(RGB(  0,255,255))
#define COL_COMMENT			(RGB(  0,255,255))
#define COL_LABEL			(RGB(  0,255,  0))
#define COL_XOR				(RGB(255,  0,  0))
#define COL_XOR_DARK		(RGB(255,128,128))
#define COL_PART			(RGB(  0,128,  0))
#define COL_PART_PINNAME	(RGB(  0,  0,255))	
#define COL_PART_PINNUM		(RGB(128,  0,128))
#define COL_PART_NAME		(RGB(255,255,255))	
#define COL_PART_NUM		(RGB(255,255,255))
#define COL_PART_FRAME		(RGB(  0,  0,255))
#define COL_SHEET_FRAME		(RGB(255,255,  0))
#define COL_SHEET_GRID		(RGB(255,  0,  0))
#define COL_DARK			(RGB(220,220,220))
#define COL_EDITPTN			(RGB(  0,128,255))
#define COL_OFF				(RGB(  0,  0,  0))

class SPtnObjLine;
class SPtnObjCircle;
class SPtnObjPolygon;
class SPtnObjArc;
class SPtnObjText;

extern bool g_bIsWin9x;

//�p�^�[����Line�̕`��
void g_DrawPtnLine(
			CDC* pDC,				//�`��
			SPtnObjLine* pObj,		//�`��I�u�W�F�N�g
			COLORREF col,			//�`��F
			int	 dir,
			int	 cx,
			int  cy,
			int	 x0,
			int  y0,
			int  nVExt,				//�\���{��(View)
			int  nWExt)				//�\���{��(Window)
;

//�p�^�[����Circle�̕`��
void g_DrawPtnCircle(
			CDC* pDC,				//�`��
			SPtnObjCircle* pObj,	//�`��I�u�W�F�N�g
			COLORREF col,			//�`��F
			int	 dir,
			int	 cx,
			int  cy,
			int	 x0,
			int  y0,
			int  nVExt,				//�\���{��(View)
			int  nWExt)				//�\���{��(Window)
;

void g_DrawPtnPolygon(
			CDC* pDC,				//�`��
			SPtnObjPolygon* pObj,	//�`��I�u�W�F�N�g
			COLORREF col,			//�`��F
			int	 dir,
			int	 cx,
			int  cy,
			int	 x0,
			int  y0,
			int  nVExt,				//�\���{��(View)
			int  nWExt)				//�\���{��(Window)
;

void g_DrawPtnArc(
			CDC* pDC,				//�`��
			SPtnObjArc* pObj,		//�`��I�u�W�F�N�g
			COLORREF col,			//�`��F
			int	 dir,
			int	 cx,
			int  cy,
			int	 x0,
			int  y0,
			int  nVExt,				//�\���{��(View)
			int  nWExt)				//�\���{��(Window)
;

void g_DrawPtnText(
			CDC* pDC,				//�`��
			SPtnObjText* pObj,		//�`��I�u�W�F�N�g
			COLORREF col,			//�`��F
			int	 dir,
			int	 cx,
			int  cy,
			int	 x0,
			int  y0,
			int  nVExt,				//�\���{��(View)
			int  nWExt)				//�\���{��(Window)
;

void g_drawComponentPin(
	CDC* pDC,						//�`���
	const SPoint& pinPos,
	int	pinType,
	int nLtrb,
	LPCTSTR pszPinName,				//�s����
	unsigned long pinNameDispFlag,
	LPCTSTR pszPinNum,				//�s���ԍ�
	DWORD dwMode,					//�`�惂�[�h
	int nVExt,						//�\���{��(View)
	int nWExt,						//�\���{��(Window)
	LPCRECT lprcClip)				//�N���b�v���
;

