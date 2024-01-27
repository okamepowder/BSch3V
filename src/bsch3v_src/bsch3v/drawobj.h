/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#ifndef DRAWOBJ_H
#define DRAWOBJ_H

extern bool g_bFillEndOfPtnLine;

//�o�X���̕`��
void g_DrawBus(
			CDC* pDC,						//�`��
			SXBSchObj* pObj,					//�`��I�u�W�F�N�g
			DWORD dwMode,					//�`�惂�[�h
			int  nVExt=1,					//�\���{��(View)
			int  nWExt=1,					//�\���{��(Window)
			LPCRECT lprcClip=NULL);			//�N���b�v���

//�j���̕`��
void g_DrawDash(
			CDC* pDC,						//�`���
			SXBSchObj* pObj,					//�`��I�u�W�F�N�g
			DWORD dwMode,					//�`�惂�[�h
			int  nVExt=1,					//�\���{��(View)
			int  nWExt=1,					//�\���{��(Window)
			LPCRECT lprcClip=NULL);			//�N���b�v���

//���C���[�̕`��
void g_DrawWire(
			CDC* pDC,						//�`���
			SXBSchObj* pObj,					//�`��I�u�W�F�N�g
			DWORD dwMode,					//�`�惂�[�h
			int  nVExt=1,					//�\���{��(View)
			int  nWExt=1,					//�\���{��(Window)
			LPCRECT lprcClip=NULL);			//�N���b�v���

//�G���g���[�̕`��
void g_DrawEntry(
			CDC* pDC,						//�`���
			SXBSchObj* pObj,					//�`��I�u�W�F�N�g
			DWORD dwMode,					//�`�惂�[�h
			int  nVExt=1,					//�\���{��(View)
			int  nWExt=1,					//�\���{��(Window)
			LPCRECT lprcClip=NULL);			//�N���b�v���

//�W�����N�V�����̕`��
void g_DrawJunc(
			CDC* pDC,						//�`���
			SXBSchObj* pObj,					//�`��I�u�W�F�N�g
			DWORD dwMode,					//�`�惂�[�h
			int  nVExt=1,					//�\���{��(View)
			int  nWExt=1,					//�\���{��(Window)
			LPCRECT lprcClip=NULL);			//�N���b�v���

//�R�����g�̕`��
void g_DrawComment(
			CDC* pDC,						//�`���
			SXBSchObj* pObj,					//�`��I�u�W�F�N�g
			DWORD dwMode,					//�`�惂�[�h
			int  nVExt=1,					//�\���{��(View)
			int  nWExt=1,					//�\���{��(Window)
			LPCRECT lprcClip=NULL);			//�N���b�v���
			
//�C���[�W�̕`��
void g_DrawImageObj(
	CDC* pDC,					//�`���
	SXBSchObj* pObj,
	DWORD dwMode,				//�`�惂�[�h
	int nVExt,					//�\���{��(View)
	int nWExt,
	LPCRECT lprcClip=NULL);			//�N���b�v���


//���x���̕`��
void g_DrawLabel(
			CDC* pDC,						//�`���
			SXBSchObj* pObj,					//�`��I�u�W�F�N�g
			DWORD dwMode,					//�`�惂�[�h
			int  nVExt=1,					//�\���{��(View)
			int  nWExt=1,					//�\���{��(Window)
			LPCRECT lprcClip=NULL);			//�N���b�v���

//�^�O�̕`��
void g_DrawTag(
			CDC* pDC,						//�`���
			SXBSchObj* pObj,					//�`��I�u�W�F�N�g
			DWORD dwMode,					//�`�惂�[�h
			int  nVExt=1,					//�\���{��(View)
			int  nWExt=1,					//�\���{��(Window)
			LPCRECT lprcClip=NULL);			//�N���b�v���

//�}�[�J�[���C���̕`��
void g_DrawMarkerLine(
			CDC* pDC,						//�`���
			SXBSchObj* pObj,				//�`��I�u�W�F�N�g
			DWORD dwMode,					//�`�惂�[�h
			int  nVExt /* =1 */,			//�\���{��(View)
			int  nWExt /* =1 */,			//�\���{��(Window)
			LPCRECT lprcClip /*=NULL*/);		//�N���b�v���

//���i����XOR�`��
//void g_drawComponentNameXOR(CDC* pDC,SXBSchComponent* pObj,int nVExt,int nWExt);

//���i����XOR�`��
//void g_drawComponentNumXOR(CDC* pDC,SXBSchComponent* pObj,int nVExt,int nWExt);

//NC�}�[�N�̕`��
void	g_drawNcMark(	
			CDC* pDC,
			const SPoint& pt,						//�ʒu
			DWORD dwMode,					//�`�惂�[�h
			int nVExt,					//�\���{��(View)
			int nWExt,					//�\���{��(Window)
			LPCRECT);					//�N���b�v���


//���i�̕`��
void g_DrawComponent(
			CDC* pDC,						//�`���
			SXBSchObj* pObj,
			DWORD dwMode,					//�`�惂�[�h
			int nVExt=1,					//�\���{��(View)
			int nWExt=1,					//�\���{��(Window)
			LPCRECT lprcClip =NULL,		//�N���b�v���
			DISP_BLOCKNUM_TYPE dispBN = DISP_BLOCKNUM_TYPE::NOTDISPLAY_BLOCKNUM);	//�u���b�N�ԍ��̕\��

#endif