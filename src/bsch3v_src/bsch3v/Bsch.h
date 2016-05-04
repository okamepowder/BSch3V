/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// BSch.h : BSCH �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//
#ifndef BSCH_H
#define BSCH_H

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

//#include "resource.h"       // ���C�� �V���{��

#include "SheetSizeInfo.h"

class SCompInfo;
class SCompLib;

/////////////////////////////////////////////////////////////////////////////
// BSch �̂��߂̃O���[�o���Ȑ��l��`

//#define MAX_LIB 64	// ���C�u�����̍ő吔

#define MAX_SHEETSIZE 5 //�V�[�g�T�C�Y��0�`(MAX_SHEETSIZE-1)


//���i�P�ƑI���̂Ƃ��̃L�[���̓t�H�[�J�X
enum{
	FOCUSED_TO_BODY = 0,
	FOCUSED_TO_NUM = 1,
	FOCUSED_TO_VAL = 2
};

enum DISP_BLOCKNUM_TYPE{
	NOTDISPLAY_BLOCKNUM	=0,
	DISPLAY_BLOCKNUM_NUM,
	DISPLAY_BLOCKNUM_ALPHA
};



///////////////////////////////////////////////////////////////
// �F
#define COL_BG				(RGB(255,255,255))
#define COL_JUNC			(RGB(  0,128,  0))
#define COL_WIRE			(RGB(  0,128,  0))
#define COL_BUS				(RGB(  0,128,192))
#define COL_DASH			(RGB(  0,  0,192))
#define COL_TAG				(RGB(192,  0,  0))
#define COL_TAG_NAME		(RGB(  0,  0,  0))
#define COL_COMMENT			(RGB(  0,  0,  0))
#define COL_LABEL			(RGB(  0,  0,  0))
#define COL_XOR				(RGB(255,  0,  0))
#define COL_PART			(RGB(192,  0,  0))
#define COL_PART_PIN		(RGB(192,  0,  0))
#define COL_PART_PINNAME	(RGB(  0,  0,  0))	
#define COL_PART_PINNUM		(RGB(  0,  0,  0))
#define COL_PART_NAME		(RGB(  0,  0,  0))	
#define COL_PART_NUM		(RGB(  0,  0,  0))
#define COL_PART_FRAME		(RGB(128,128,255))
#define COL_SHEET_FRAME		(RGB(128, 32, 32))
#define COL_SHEET_GRID		(RGB(255, 64, 64))
#define COL_DARK			(RGB(255,192,192))
#define COL_ZLPIN			(RGB(255,192,192))
#define COL_NC_MARK			(RGB(255, 32, 32))
#define COL_QUICKPROPVIEW	(RGB(240,230,200))
#define COL_TEMPFRAME_FOCUSED		(RGB(128,128,128))
#define COL_TEMPFRAME_NOT_FOCUSED	(RGB(160,220,220))
#define COL_FIND_MARK	(RGB(64,64,255))


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
#define DRAW_NC_MARK 128
#define DRAW_INH_PARTNAME 	256	//���i���̕`������Ȃ�
#define DRAW_INH_PARTNUM  	512	//���i�ԍ��̕`������Ȃ�
#define DRAW_INH_PARTBODY  1024	//���i�{�f�B�̕`������Ȃ�
#define DRAW_FOR_PRINT	   2048 //����p�̕`��
#define DRAW_FOR_PRINTER	4096 //�v�����^�[����p�̕`��


//#define COMMENT_Y_OFFSET 2


#define SCROLL_TO_MSG (WM_APP+100)



/////////////////////////////////////////////////////////////////////////////
// CBSchApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� BSch.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CBSchApp : public CWinApp
{
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CBSchApp();

	~CBSchApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CBSchApp)
	public:
	virtual BOOL InitInstance();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����

	//{{AFX_MSG(CBSchApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnFilePrintSetup();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	TCHAR m_szIniFileDir[_MAX_PATH];
private:
	TCHAR *m_pszIniFileName;
	TCHAR m_moduleFileName[_MAX_PATH];
	TCHAR m_LCoVPath[_MAX_PATH];

public:
	void UpdatePrinterSelection(BOOL bForceDefaults);
	void IniWritePrinterSetup();
	BOOL ExecBSch(const TCHAR* pathname);
	LPCTSTR LCoVPath(){return m_LCoVPath;}
	virtual BOOL OnIdle(LONG lCount);

public:
	static void  editWidthLCoV(LPCTSTR name, SCompInfo* pInfo, SCompLib& tempLib);
};


/////////////////////////////////////////////////////////////////////////////

#endif
