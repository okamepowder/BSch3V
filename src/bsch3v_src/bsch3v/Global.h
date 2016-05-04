/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// Global.h �O���[�o���ϐ��̊O���Q�Ƃ̐錾������
//
/////////////////////////////////////////////////////////////////////////////

#include <string>
using namespace std;


//////////////////////////////////////////////////////////////////////
//��ʕ\���֘A
extern BOOL g_bDisplayPartFrame;
extern BOOL g_bDisplayNcPinMark;
extern BOOL g_bDisplayHiddenPinNum;	//0.63.01

extern CWnd* g_pViewWindow;

//////////////////////////////////////////////////////////////////////
//���֘A
extern BOOL g_bJapaneseLangEnv;	
extern BOOL g_bAvailableGDIplus;

//////////////////////////////////////////////////////////////////////
//�A�N�e�B�u�ȃh�L�������g�̃p�X���̃R�s�[

extern wstring g_docPath;
extern wstring g_docTimeStamp;
extern wstring g_docDate;
extern wstring g_printTime;




//////////////////////////////////////////////////////////////////////
//���O�֘A

//extern wstring g_log;
//extern TCHAR g_logPath[];

//////////////////////////////////////////////////////////////////////
//����֘A
//extern int g_bPrintPartNum;		//������̕��i�ԍ��o��    1997/01/26
//extern int g_bPrintPartName;	//������̕��i���o��    1998/07/22


/*     PartLib.h�Ɉړ� 1997/01/04
//////////////////////////////////////////////////////////////////////
//���C�u�����֘A
extern CPartLib* g_pPartLib[MAX_LIB];	//���i���C�u����
BOOL g_ReadLibrary();	//���W�X�g�����烉�C�u�����̏��𓾂ēǂݍ���
void g_FreeLibrary();	//���C�u�����̃������̉�����s��
CPartIndex* g_SearchPartIndex(LPCSTR pszName);	//���C�u�������畔�i�C���f�b�N�X�𓾂�
*/

/*     BSchFont.h�Ɉړ� 1997/01/01
//////////////////////////////////////////////////////////////////////
//�t�H���g�֘A
extern CBSchFont* g_pFont;
BOOL g_IniReadFontInfo(LPLOGFONT lplf);	//���W�X�g������t�H���g���𓾂�
BOOL g_IniWriteFontInfo(LPLOGFONT lplf);//���W�X�g���Ƀt�H���g������������
*/
//////////////////////////////////////////////////////////////////////
//���W�֘A
//void g_MakeRECT(LPPOINT p1,LPPOINT p2,LPRECT prc);//�Q�_�̍��W�����`�͈͂�ݒ肷��
//void g_CorrectRECT(LPRECT prc);		//��`���W�̏㉺���E������ւ���ėv��ꍇ�ɐ���������
//BOOL g_IsRect2insideRect1(LPRECT prc1,LPRECT prc2);
//��`2����`1�̓����i�܂��͓����j�ɂ��邩�ǂ������m���߂�


//////////////////////////////////////////////////////////////////////
//�N���b�v�{�[�h�֌W

//UINT g_GetClipboardFormat();		//�N���b�v�{�[�h�̃t�H�[�}�b�g�̒l�𓾂�
//UINT g_RegisterClipboardFormat();	//�N���b�v�{�[�h�̃t�H�[�}�b�g�̓o�^������
