/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// 2 of 2

#include "stdafx.h"
#include <stdlib.h> //abs()�̂��߂ɃC���N���[�h 
#include <assert.h>
#include <string>
#include <list>
using namespace std;
#include "BSch.h"

#include "SetLib.h"

#include "BSchDoc.h"
#include "BSchView.h"
#include "drawobj.h"

#include "complib.h"
#include "BSchFont.h"
#include "Global.h"
#include "coord2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//�O���b�h�̕`��
void CBSchView::DrawSheetGrid(CDC* pDC,const CRect& rcClip)
{
	if(m_nVExt<m_nWExt || !m_bDisplayGrid) return;

	CBSchDoc* pDoc = GetDocument();				//�h�L�������g�̃|�C���^�𓾂�
	CSize sz = pDoc->GetSheetSize();				//�}�ʃT�C�Y�𓾂�
	
	int nx,ny;
	int xlim =(sz.cx<rcClip.right ? sz.cx : rcClip.right);
	int ylim =(sz.cy<rcClip.bottom ? sz.cy : rcClip.bottom);

	ny=((rcClip.top+9)/10)*10;
	while(ny<ylim ){
		nx=((rcClip.left+9)/10)*10;
		while(nx<xlim){
			int x=(nx*m_nVExt)/m_nWExt;
			int y=(ny*m_nVExt)/m_nWExt;
			pDC->SetPixel(x,y,m_COL.colSHEET_GRID);
			nx+=10;
		}
		ny+=10;
	}
}


//�}�ʘg�̐������W�����̕`��
void CBSchView::DrawSheetFrameCharH(CDC* pDC,COLORREF col,int ny,int nWidth,UINT nMode,int nVExt,int nWExt)
{
	int x=100;
	TCHAR c='A';
	while(x<nWidth){
		g_pFont->PutFont(
				pDC,				//�o�͐�̃f�o�C�X�R���e�L�X�g
				x,ny,				//�o�͈ʒu
				&c,					//�o�͕�����
				1,					//������
				col,				//�`��F
				nMode,				//�`�惂�[�h
				FALSE,
				0,
				nVExt,
				nWExt);
		x+=200;
		c++;
	}
}

//�}�ʘg�̐������W�����̕`��
void CBSchView::DrawSheetFrameCharV(CDC* pDC,COLORREF col,int nx,int nWidth,UINT nMode,int nVExt,int nWExt)
{
	int y=100;
	TCHAR c='1';
	while(y<nWidth){
		g_pFont->PutFont(
				pDC,				//�o�͐�̃f�o�C�X�R���e�L�X�g
				nx,y,				//�o�͈ʒu
				&c,					//�o�͕�����
				1,					//������
				col,				//�`��F
				nMode,				//�`�惂�[�h
				FALSE,
				0,
				nVExt,
				nWExt);

		y+=200;
		if(c=='9') c='0';
		else c++;
	}
}


//������̔w�i�F�̕`��
//dwMode�� DRAW_ON�݂̂��L���i���̑��̂Ƃ��͉������Ȃ��j
void CBSchView::DrawPrintBg(CDC* pDC,DWORD dwMode,int nVExt,int nWExt,const CRect* prc)
{
	CSize size;				//�}�ʂ̃T�C�Y
	CRect rc;

	if((dwMode & DRAW_ON)==0) return;

	//���W�̐ݒ�
	if(prc == NULL){
		CBSchDoc* pDoc = GetDocument();				//�h�L�������g�̃|�C���^�𓾂�
		size = pDoc->GetSheetSize();				//�}�ʃT�C�Y�𓾂�
		rc = CRect(CPoint(0,0),size);
	}else{
		rc = *prc;
	}
	BSchPtoLP(&rc,nVExt,nWExt);
	pDC->FillSolidRect(&rc,m_COL.colBG);	//2009/01/11
}


//�}�ʘg�̕`��
//dwMode�� DRAW_ON��DRAW_OFF�݂̂��L���i���̑��͖����j
void CBSchView::DrawSheetFrame(CDC* pDC,DWORD dwMode,int nVExt,int nWExt,const CRect& rcClip)
{
	CSize sizeOrg,size;				//�}�ʂ̃T�C�Y
	COLORREF col;					//�g�̕`��F
	CRect rcOutside, rcInside;		//�g�̍��W
	int nFrameWidth=10;				//�g�̕�

	//���W�̐ݒ�
	nFrameWidth=nFrameWidth*nVExt/nWExt;	//�Y�[����ɑΉ�����g���ɕϊ�
	CBSchDoc* pDoc = GetDocument();				//�h�L�������g�̃|�C���^�𓾂�
	sizeOrg = pDoc->GetSheetSize();				//�}�ʃT�C�Y�𓾂�
	size=sizeOrg;
	BSchPtoLP(&size,nVExt,nWExt);
	rcOutside=CRect(0,0,size.cx-1,size.cy-1);	//�O�g�̍��W�̐ݒ�
	rcInside =CRect(nFrameWidth,nFrameWidth,	//���g�̍��W�̐ݒ�
		size.cx-nFrameWidth,size.cy-nFrameWidth);
	//�`��F�̐ݒ�	
	if(dwMode & DRAW_ON) col=m_COL.colSHEET_FRAME;	//DRAW_ON�ł�BSch.H�Œ�`�����F
	else				 col=RGB(0,0,0);		//DRAW_OFF�ł͍��ł̕`��

	int width = 1*nVExt/nWExt;

	//�g�̕`��
	CPen* pPen=new CPen(PS_SOLID,width,col);
	pPen=pDC->SelectObject(pPen);	//�쐬�����y���̑I��
	if(rcClip.left<=10){				//�����g���̕`��
		pDC->MoveTo(rcInside.left,rcInside.top);
		pDC->LineTo(rcInside.left,rcInside.bottom);
		if(nVExt>=nWExt)	//���{�`��̂Ƃ��ɂ͍��W�������`��
			DrawSheetFrameCharV(pDC,col,2,sizeOrg.cy,dwMode,nVExt,nWExt);
		if(rcClip.left<=0){			//���O�g���̕`��
			pDC->MoveTo(rcOutside.left,rcOutside.top);
			pDC->LineTo(rcOutside.left,rcOutside.bottom);
		}
	}
	if(rcClip.top<=10){				//����g���̕`��
		pDC->MoveTo(rcInside.left,rcInside.top);
		pDC->LineTo(rcInside.right,rcInside.top);
		if(nVExt>=nWExt)	//���{�`��̂Ƃ��ɂ͍��W�������`��
			DrawSheetFrameCharH(pDC,col,9,sizeOrg.cx,dwMode,nVExt,nWExt);
		if(rcClip.top<=0){			//��O�g���̕`��
			pDC->MoveTo(rcOutside.left,rcOutside.top);
			pDC->LineTo(rcOutside.right,rcOutside.top);
		}
	}
	if(rcClip.right>=sizeOrg.cx-10){		//�E���g���̕`��
		int a=0;
		if(width<=1) a=1;
		pDC->MoveTo(rcInside.right,rcInside.top);
		pDC->LineTo(rcInside.right,rcInside.bottom+a);		//�I�_��₤
		if(nVExt>=nWExt)	//���{�`��̂Ƃ��ɂ͍��W�������`��
			DrawSheetFrameCharV(pDC,col,sizeOrg.cx-8,sizeOrg.cy,dwMode,nVExt,nWExt);
		if(rcClip.right>=sizeOrg.cx-1){	//�E�O�g���̕`��
			pDC->MoveTo(rcOutside.right,rcOutside.top);
			pDC->LineTo(rcOutside.right,rcOutside.bottom+a);//�I�_��₤
		}
	}
	if(rcClip.bottom>=sizeOrg.cy-10){		//�����g���̕`��
		pDC->MoveTo(rcInside.left,rcInside.bottom);
		pDC->LineTo(rcInside.right,rcInside.bottom);
		if(nVExt>=nWExt)	//���{�`��̂Ƃ��ɂ͍��W�������`��
			DrawSheetFrameCharH(pDC,col,sizeOrg.cy-2,sizeOrg.cx,dwMode,nVExt,nWExt);
		if(rcClip.bottom>=sizeOrg.cy-1){
			pDC->MoveTo(rcOutside.left,rcOutside.bottom);
			pDC->LineTo(rcOutside.right,rcOutside.bottom);
		}
	}
	pPen=pDC->SelectObject(pPen);	//�y���̕��A
	delete pPen;					//�쐬�����y���̍폜
}

void  CBSchView::DrawObj(					//�}�ʗv�f��`�悷��
		CDC* pDC,							//�`���
		SXBSchObj* pObj,						//�`��I�u�W�F�N�g
		DWORD dwMode,						//�`�惂�[�h
		int  nVExt,						//�\���{��(View)
		int  nWExt,						//�\���{��(Window)
		LPCRECT lprcClip)				//�N���b�v���
{
	switch(pObj->id()){
	case SXBSchObj::ID_BUS:
		g_DrawBus(pDC,pObj,dwMode,nVExt,nWExt,lprcClip);
		break;
	case SXBSchObj::ID_WIRE:
		g_DrawWire(pDC,pObj,dwMode,nVExt,nWExt,lprcClip);
		break;
	case SXBSchObj::ID_DASH:
		g_DrawDash(pDC,pObj,dwMode,nVExt,nWExt,lprcClip);
		break;
	case SXBSchObj::ID_MARKER:
		g_DrawMarkerLine(pDC,pObj,dwMode,nVExt,nWExt,lprcClip);
		break;
	case SXBSchObj::ID_ENTRY:
	case SXBSchObj::ID_BENTRY:
		g_DrawEntry(pDC,pObj,dwMode,nVExt,nWExt,lprcClip);
		break;
	case SXBSchObj::ID_JUNCTION:
		g_DrawJunc(pDC,pObj,dwMode,nVExt,nWExt,lprcClip);
		break;
	case SXBSchObj::ID_COMMENT:
		g_DrawComment(pDC,pObj,dwMode,nVExt,nWExt,lprcClip);
		break;
	case SXBSchObj::ID_LABEL:
		g_DrawLabel(pDC,pObj,dwMode,nVExt,nWExt,lprcClip);
		break;
	case SXBSchObj::ID_TAG:
		g_DrawTag(pDC,pObj,dwMode,nVExt,nWExt,lprcClip);
		break;
	case SXBSchObj::ID_COMPONENT:
		g_DrawComponent(pDC,pObj,dwMode,nVExt,nWExt,lprcClip,m_dispBN);
		break;
	case SXBSchObj::ID_IMAGEOBJECT:
		g_DrawImageObj(pDC,pObj,dwMode,nVExt,nWExt,lprcClip);
		break;
	default:
		;
	}
}



//�w�胊�X�g�f�[�^�̕`��
void CBSchView::DrawListData(CDC* pDC,XBSchObjList* pList,DWORD dwMode,
								int  nVExt,int  nWExt,
								const CRect& rcClip,DWORD dwOption,DWORD layerMask)
{


	//static WORD wLayerTable[]={	0x0001,0x0002,0x0004,0x0008,
	//								0x0010,0x0020,0x0040,0x0080,
	//								0x0100,0x0200,0x0400,0x0800,
	//								0x1000,0x2000,0x4000,0x8000};
//	CBSchDoc* pDoc = GetDocument();
//	WORD wShowLayer=pDoc->visibleLayer();
//	int  nEditLayer=pDoc->editLayer();

	SRect rc=CRect2SRect(rcClip);


	//�����n�I�u�W�F�N�g�̕`��
	XBSchObjListIterator pObjIte = pList->begin();
	while(pObjIte != pList->end()){
		SXBSchObj* pObj = *pObjIte;
		pObjIte++;
		if(pObj->deleteNum()<0 && pObj->keepOrder()){ //()!=SXBSchObj::ID_JUNCTION && pObj->id()!=SXBSchObj::ID_MARKER && pObj->id()!=SXBSchObj::ID_IMAGEOBJECT){
			if((dwOption & DRAW_TEMP_DATA)||(pObj->refObj() == NULL)){
				if(!(dwOption & DRAW_ALL_LAYER)){
					if(!(layerMask & SLayer::wLayerTable[pObj->layer()]) ){
						continue;//�`�惌�C���[�łȂ���Ε`�悵�Ȃ�
					}
				}

				if(pObj->qRedraw(rc)){
					DrawObj(pDC,pObj,dwMode,nVExt,nWExt,&rcClip);
				}
			}
		}
	}









	////�C���[�W�̕`��
	//for(int imgLayer=7;imgLayer>=0;imgLayer--){
	//	if((!(dwOption & DRAW_ALL_LAYER)) && (!(layerMask & wLayerTable[imgLayer])) ){
	//		continue;
	//	}
	//	XBSchObjListIterator pObjIte = pList->begin();
	//	while(pObjIte != pList->end()){
	//		SXBSchObj* pObj = *pObjIte;
	//		pObjIte++;
	//		if(pObj->deleteNum()<0 && pObj->id()==SXBSchObj::ID_IMAGEOBJECT){
	//			if((dwOption & DRAW_TEMP_DATA)||(pObj->refObj() == NULL)){
	//				if((!(dwOption & DRAW_ALL_LAYER))&&(imgLayer != pObj->layer())){
	//					continue;//�`�惌�C���[�łȂ���Ε`�悵�Ȃ�
	//				}
	//				if(pObj->qRedraw(rc)){
	//					DrawObj(pDC,pObj,dwMode,nVExt,nWExt,&rcClip);
	//				}
	//			}
	//		}
	//	}
	//}



	////�}�[�J�[���C���̕`��
	//XBSchObjListIterator pObjIte = pList->begin();
	//while(pObjIte != pList->end()){
	//	SXBSchObj* pObj = *pObjIte;
	//	pObjIte++;
	//	if(pObj->deleteNum()<0 && pObj->id()==SXBSchObj::ID_MARKER){
	//		if((dwOption & DRAW_TEMP_DATA)||(pObj->refObj() == NULL)){
	//			if(!(dwOption & DRAW_ALL_LAYER)){
	//				if(!(layerMask & wLayerTable[pObj->layer()]) ){
	//					continue;//�`�惌�C���[�łȂ���Ε`�悵�Ȃ�
	//				}
	//			}
	//			if(pObj->qRedraw(rc)){
	//				DrawObj(pDC,pObj,dwMode,nVExt,nWExt,&rcClip);
	//			}
	//		}
	//	}
	//}


	//NC�}�[�N�̕`��
	if(dwMode & DRAW_NC_MARK){
		XBSchObjListIterator pObjIte = pList->begin();
		while(pObjIte != pList->end()){
			SXBSchObj* pObj = *pObjIte;
			pObjIte++;
			//NC�}�[�N�͈ꎞ�f�[�^�̂Ƃ��͕`�悵�Ȃ�
			if((pObj->deleteNum()>=0)||(dwOption & DRAW_TEMP_DATA)||(pObj->refObj() != NULL)){
				continue;
			}

			if(!(dwOption & DRAW_ALL_LAYER)){
				if(!(layerMask & SLayer::wLayerTable[pObj->layer()]) ){
					continue;//�`�惌�C���[�łȂ���Ε`�悵�Ȃ�
				}
			}

			if(pObj->id()==SXBSchObj::ID_COMPONENT){
				if(!pObj->qRedraw(rc)){
					//SXBSchComponent��qRedraw()��Ver0.48���ݏ��true��Ԃ��̂Ŏ��͈Ӗ����Ȃ��B
					continue;
				}
				SXBSchComponent* pComp = (SXBSchComponent*)pObj;
				int pincount = pComp->pinCount();
				int i;
				for(i=0;i<pincount;i++){
					if(pComp->pinNC(i)){
						SPoint pt = pComp->pinPosition(i);
						g_drawNcMark(	pDC,
										pt,						//�ʒu
										dwMode,					//�`�惂�[�h
										nVExt,					//�\���{��(View)
										nWExt,					//�\���{��(Window)
										&rcClip);				//�N���b�v���
					}
				}
			}
		}
	}

	//���i�̕`��
	pObjIte = pList->begin();
	while(pObjIte != pList->end()){
		SXBSchObj* pObj = *pObjIte;
		pObjIte++;
		if(pObj->deleteNum()<0 && pObj->id()==SXBSchObj::ID_COMPONENT){
			if((dwOption & DRAW_TEMP_DATA)||(pObj->refObj() == NULL)){
				if(!(dwOption & DRAW_ALL_LAYER)){
					if(!(layerMask & SLayer::wLayerTable[pObj->layer()]) ){
						continue;//�`�惌�C���[�łȂ���Ε`�悵�Ȃ�
					}
				}

				if(pObj->qRedraw(rc)){
					DrawObj(pDC,pObj,dwMode,nVExt,nWExt,&rcClip);
				}
			}
		}
	}

	//�o�X�A�o�X�G���g���[�̕`��
	pObjIte = pList->begin();
	while(pObjIte != pList->end()){
		SXBSchObj* pObj = *pObjIte;
		pObjIte++;
		if(pObj->deleteNum()<0 && (pObj->id()==SXBSchObj::ID_BUS||pObj->id()==SXBSchObj::ID_BENTRY)){
			if((dwOption & DRAW_TEMP_DATA)||(pObj->refObj() == NULL)){
				if(!(dwOption & DRAW_ALL_LAYER)){
					if(!(layerMask & SLayer::wLayerTable[pObj->layer()]) ){
						continue;//�`�惌�C���[�łȂ���Ε`�悵�Ȃ�
					}
				}

				if(pObj->qRedraw(rc)){
					DrawObj(pDC,pObj,dwMode,nVExt,nWExt,&rcClip);
				}
			}
		}
	}


	//���C���[�A���C���[�G���g���[�A�W�����N�V�����̕`��
	pObjIte = pList->begin();
	while(pObjIte != pList->end()){
		SXBSchObj* pObj = *pObjIte;
		pObjIte++;
		if(pObj->deleteNum()<0 && (pObj->id()==SXBSchObj::ID_WIRE||pObj->id()==SXBSchObj::ID_ENTRY||pObj->id()==SXBSchObj::ID_JUNCTION)){
			if((dwOption & DRAW_TEMP_DATA)||(pObj->refObj() == NULL)){
				if(!(dwOption & DRAW_ALL_LAYER)){
					if(!(layerMask & SLayer::wLayerTable[pObj->layer()]) ){
						continue;//�`�惌�C���[�łȂ���Ε`�悵�Ȃ�
					}
				}

				if(pObj->qRedraw(rc)){
					DrawObj(pDC,pObj,dwMode,nVExt,nWExt,&rcClip);
				}
			}
		}
	}

	//���x���A�^�O�̕`��
	pObjIte = pList->begin();
	while(pObjIte != pList->end()){
		SXBSchObj* pObj = *pObjIte;
		pObjIte++;
		if(pObj->deleteNum()<0 && (pObj->id()==SXBSchObj::ID_LABEL||pObj->id()==SXBSchObj::ID_TAG)){
			if((dwOption & DRAW_TEMP_DATA)||(pObj->refObj() == NULL)){
				if(!(dwOption & DRAW_ALL_LAYER)){
					if(!(layerMask & SLayer::wLayerTable[pObj->layer()]) ){
						continue;//�`�惌�C���[�łȂ���Ε`�悵�Ȃ�
					}
				}

				if(pObj->qRedraw(rc)){
					DrawObj(pDC,pObj,dwMode,nVExt,nWExt,&rcClip);
				}
			}
		}
	}

}



//���C���f�[�^�̕`��
void CBSchView::DrawMainData(CDC* pDC,CBSchDoc* pDoc,DWORD dwMode,
							 int  nVExt,int  nWExt,
							 const CRect& rcClip)
{
//	int n;
	XBSchObjList* pMainData;
	if(!pDC || !pDoc) return;
	DWORD layerMask;
	DWORD editLayerMask = 0x01 << pDoc->editLayer();
	DWORD wShowLayerOrg	= pDoc->visibleLayer();

	if(m_fEditHighLight && dwMode & DRAW_ON){
//		WORD wShowLayerOrg	= pDoc->visibleLayer();
//		editLayerMask = 0x01 << pDoc->editLayer();

		//�ŏ��ɕҏW���C���[�ȊO�̃��C���[��`�悷��
		layerMask = wShowLayerOrg & ~editLayerMask;
		if((pMainData=pDoc->GetMainData(0))!=NULL){
			DrawListData(pDC,pMainData,dwMode | DRAW_DARK,nVExt,nWExt,rcClip,0,layerMask);
		}
		//�ォ��ҏW���C���[��`�悷��
		//pDoc->m_wShowLayer = wEditLayer;
		if((pMainData=pDoc->GetMainData(0))!=NULL){
			DrawListData(pDC,pMainData,dwMode,nVExt,nWExt,rcClip,0,editLayerMask);
		}
	}else{
		if((pMainData=pDoc->GetMainData(0))!=NULL){
			DrawListData(pDC,pMainData,dwMode,nVExt,nWExt,rcClip,0,editLayerMask|wShowLayerOrg);
		}
	}
}

//�ꎞ�f�[�^�̕`��
void CBSchView::DrawTmpData(CDC* pDC,CBSchDoc* pDoc,DWORD dwMode,
							int  nVExt,int  nWExt,
							const CRect& rcClip,BOOL bDraw/*=TRUE*/)
{
	XBSchObjList* pTmpData;
	if(!pDC || !pDoc){ 
	//	TRACE("\nBSChView::DrawTmpData() #3");
		return;
	}
	UINT nDrawAllBit=DRAW_TEMP_DATA;
	if(!(dwMode & DRAW_ON)){
		nDrawAllBit|=DRAW_ALL_LAYER;
	}

	if((pTmpData=pDoc->GetTmpList())==NULL){		//�ꎞ�f�[�^�̃��X�g�𓾂�
//		TRACE("\nBSChView::DrawTmpData() #2");
		return;
	}
	if(!pTmpData->size()){
		m_bDrawTmpData=FALSE;
		return;
	}
	DrawListData(pDC,pTmpData,dwMode,nVExt,nWExt,rcClip,nDrawAllBit);	//�ꎞ�f�[�^�̃��X�g�̕`��
	if(dwMode & DRAW_TEMP)m_bDrawTmpData=bDraw;
	else m_bDrawTmpData=FALSE;
}


//�ꎞ�f�[�^�̘g�̕`��
//���̏ꍇ�͘g��`�悵�Ȃ�
//   �E�ꎞ�f�[�^�����݂��Ȃ��ꍇ
//bDraw��TRUE�̂Ƃ��`��
//       FALSE�̂Ƃ�����
void CBSchView::DrawTmpDataFrame(CDC* pDC,CBSchDoc* pDoc,BOOL bDraw/*=TRUE*/)
{
	RECT rc;
	if(bDraw==FALSE && m_bDrawTmpDataFrame==FALSE) return;
	if(!pDC || !pDoc) return;
	int nTmpCount=pDoc->GetTmpDataCount();

	if(nTmpCount<1){	//�i�����P�j
		m_bDrawTmpDataFrame=FALSE;//�`��t���O���N���A���Ă���
		return;
	}
	//rc=pDoc->GetTmpRect();		//�ꎞ�f�[�^�̊O���𓾂�
	//BSchPtoLP(&rc,m_nVExt,m_nWExt);				//�}�ʍ��W��_�����W�ɕϊ�
	//rc.left-=1; rc.top-=1; rc.right+=2; rc.bottom+=2;



	int nOldDrawMode;
	CPen penNew;
	CPen* ppenOld;
	CBrush brushNew;
	CBrush* pbrushOld;
	COLORREF col;

	//bool bSelCompSingle = ((nTmpCount == 1)&&(!pDoc->isSelectedForDtag()));
	bool bSelectSingle = (nTmpCount == 1);


	
	pbrushOld = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
	nOldDrawMode=pDC->SetROP2(R2_COPYPEN);	//�`�惂�[�h�̐ݒ�

	bool bInhibitDrawOutline = false;	//�I��͈͊O�g�`��̋֎~�t���O �������C����EditPoints���[�h�̂Ƃ��֎~

	if(bSelectSingle){
		XBSchObjList* pTmpData = pDoc->GetTmpList();
		XBSchObjListIterator pObjIte = pTmpData->begin();
		SXBSchObj* pObj = *pObjIte;


		if(pObj){
			if(pObj->id() == SXBSchObj::ID_COMPONENT){
				SRect src;

				if(!((SXBSchComponent*)(pObj))->GetHideNum()){
					col = ((m_nFocusedTo == FOCUSED_TO_NUM)?m_COL.colTEMPFRAME_FOCUSED:m_COL.colTEMPFRAME_NOT_FOCUSED);
					penNew.CreatePen(PS_SOLID,0,col);
					ppenOld=pDC->SelectObject(&penNew);			//�쐬�����y���̑I��

					src = ((SXBSchComponent*)(pObj))->refnumArea();
					rc.left = src.l();
					rc.top = src.t();
					rc.right = src.r();
					rc.bottom = src.b();
					BSchPtoLP(&rc,m_nVExt,m_nWExt);				//�}�ʍ��W��_�����W�ɕϊ�
					rc.right+=1; rc.bottom+=1;
					pDC->Rectangle(&rc);

					pDC->SelectObject(ppenOld);
					penNew.DeleteObject();
				}


				if(!((SXBSchComponent*)(pObj))->GetHideVal()){
					col = ((m_nFocusedTo == FOCUSED_TO_VAL)?m_COL.colTEMPFRAME_FOCUSED:m_COL.colTEMPFRAME_NOT_FOCUSED);
					penNew.CreatePen(PS_SOLID,0,col);
					ppenOld=pDC->SelectObject(&penNew);			//�쐬�����y���̑I��


					src = ((SXBSchComponent*)(pObj))->nameArea();
					rc.left = src.l();
					rc.top = src.t();
					rc.right = src.r();
					rc.bottom = src.b();
					BSchPtoLP(&rc,m_nVExt,m_nWExt);				//�}�ʍ��W��_�����W�ɕϊ�
					rc.right+=1; rc.bottom+=1;
					pDC->Rectangle(&rc);

					pDC->SelectObject(ppenOld);
					penNew.DeleteObject();
				}
			}else if(pObj->id() == SXBSchObj::ID_DASH){
				SXBSchDash* pDash = (SXBSchDash*)pObj;
				if(m_bEditPointsMode){
					bInhibitDrawOutline = true;
					unsigned selectFlag = pDash->selection();
					col = m_COL.colTEMPFRAME_NOT_FOCUSED;
					penNew.CreatePen(PS_SOLID,0,col);
					ppenOld=pDC->SelectObject(&penNew);			//�쐬�����y���̑I��
					if(selectFlag & SXBSchObj::SELECT_P1){
						int x = pDash->p1().x();
						int y = pDash->p1().y();
						CRect rc=CRect(x-2,y-2,x+2,y+2);
						BSchPtoLP(&rc,m_nVExt,m_nWExt);				//�}�ʍ��W��_�����W�ɕϊ�
						rc.left-=1; rc.top-=1; rc.right+=2; rc.bottom+=2;
						pDC->Rectangle(&rc);
					}
					if(selectFlag & SXBSchObj::SELECT_P2){
						int x = pDash->p2().x();
						int y = pDash->p2().y();
						CRect rc=CRect(x-2,y-2,x+2,y+2);
						BSchPtoLP(&rc,m_nVExt,m_nWExt);				//�}�ʍ��W��_�����W�ɕϊ�
						rc.left-=1; rc.top-=1; rc.right+=2; rc.bottom+=2;
						pDC->Rectangle(&rc);
					}

					if(pDash->curve()){
						if(selectFlag & SXBSchObj::SELECT_P1){
							int x = pDash->Ct1().x();
							int y = pDash->Ct1().y();
							CRect rc=CRect(x-2,y-2,x+2,y+2);
							BSchPtoLP(&rc,m_nVExt,m_nWExt);				//�}�ʍ��W��_�����W�ɕϊ�
							rc.left-=1; rc.top-=1; rc.right+=2; rc.bottom+=2;
							pDC->Rectangle(&rc);
							CPoint pt=CPoint(x,y);
							BSchPtoLP(&pt,m_nVExt,m_nWExt);		
							pDC->MoveTo(pt);
							pt = SPoint2CPoint(pDash->p1());
							BSchPtoLP(&pt,m_nVExt,m_nWExt);		
							pDC->LineTo(pt);
						}
						if(selectFlag & SXBSchObj::SELECT_P2){
							int x = pDash->Ct2().x();
							int y = pDash->Ct2().y();
							CRect rc=CRect(x-2,y-2,x+2,y+2);
							BSchPtoLP(&rc,m_nVExt,m_nWExt);				//�}�ʍ��W��_�����W�ɕϊ�
							rc.left-=1; rc.top-=1; rc.right+=2; rc.bottom+=2;
							pDC->Rectangle(&rc);
							CPoint pt=CPoint(x,y);
							BSchPtoLP(&pt,m_nVExt,m_nWExt);		
							pDC->MoveTo(pt);
							pt = SPoint2CPoint(pDash->p2());
							BSchPtoLP(&pt,m_nVExt,m_nWExt);		
							pDC->LineTo(pt);
						}
					}
					pDC->SelectObject(ppenOld);
					penNew.DeleteObject();
				}
			}
		}
	}

	if(!bInhibitDrawOutline){

		//�I���I�u�W�F�N�g�{�̂̑I���t���[���̕`��
		col = ((!bSelectSingle || m_nFocusedTo == FOCUSED_TO_BODY)?m_COL.colTEMPFRAME_FOCUSED:m_COL.colTEMPFRAME_NOT_FOCUSED);
		penNew.CreatePen(PS_SOLID,0,col);
		ppenOld=pDC->SelectObject(&penNew);			//�쐬�����y���̑I��

		rc=pDoc->GetTmpRect();		//�ꎞ�f�[�^�̊O���𓾂�
		BSchPtoLP(&rc,m_nVExt,m_nWExt);				//�}�ʍ��W��_�����W�ɕϊ�
		rc.left-=1; rc.top-=1; rc.right+=2; rc.bottom+=2;
		pDC->Rectangle(&rc);

		pDC->SelectObject(ppenOld);
		penNew.DeleteObject();
	}

	pDC->SetROP2(nOldDrawMode);		//�`�惂�[�h�̕��A
	pDC->SelectObject(pbrushOld);
	m_bDrawTmpDataFrame=bDraw;	
}
	

//�I�𒆂̓_���g�̕`��
//bDraw��TRUE�̂Ƃ��`��
//       FALSE�̂Ƃ�����
void CBSchView::DrawSelectFrame(CDC* pDC,BOOL bDraw/*=TRUE*/)
{
	CRect rc;
	if(bDraw==FALSE && m_bDrawSelectFrame==FALSE) return;//�����w��ł��łɏ����ς݂Ȃ牽�����Ȃ�
	rc=CRect(m_ptMoveFrom,m_ptMoveTo);
	rc.NormalizeRect();
	//g_MakeRECT(&m_ptMoveFrom,&m_ptMoveTo,&rc);	//�_���g�̐}�ʏ�̍��W�𓾂�
	BSchPtoLP(&rc,m_nVExt,m_nWExt);				//�}�ʍ��W��_�����W�ɕϊ�

	int nOldDrawMode;
	CPen penNew;
	CPen* ppenOld;
	CBrush brushNew;
	CBrush* pbrushOld;

	penNew.CreatePen(PS_SOLID,0,RGB(128,128,128));
	ppenOld=pDC->SelectObject(&penNew);			//�쐬�����y���̑I��
	pbrushOld = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
	nOldDrawMode=pDC->SetROP2(R2_COPYPEN);	//�`�惂�[�h�̐ݒ�
	pDC->Rectangle(&rc);
	pDC->SetROP2(nOldDrawMode);		//�`�惂�[�h�̕��A
	pDC->SelectObject(pbrushOld);
	pDC->SelectObject(ppenOld);

	//	pDC->DrawFocusRect(&rc);	//�_���g�̕`��

	m_bDrawSelectFrame=bDraw;	//�g�O���łȂ��_�ɒ��� 1996/12/01
}

void CBSchView::DrawFindMark(CDC* pDC)
{
	if(!isFindMarkActive())return;

	int nOldDrawMode;
	CPen penNew;
	CPen* ppenOld;
	penNew.CreatePen(PS_SOLID,0,m_COL.colFIND_MARK);
	ppenOld=pDC->SelectObject(&penNew);			//�쐬�����y���̑I��
	nOldDrawMode=pDC->SetROP2(R2_COPYPEN);	//�`�惂�[�h�̐ݒ�

	CPoint ptBegin;
	CPoint ptEnd;

	ptBegin = CPoint(-10,m_ptFindMark.y);
	ptEnd   = CPoint(6000,m_ptFindMark.y);
	BSchPtoLP(&ptBegin,m_nVExt,m_nWExt);				//�}�ʍ��W��_�����W�ɕϊ�
	BSchPtoLP(&ptEnd,m_nVExt,m_nWExt);				//�}�ʍ��W��_�����W�ɕϊ�
	pDC->MoveTo(ptBegin);
	pDC->LineTo(ptEnd);

	ptBegin = CPoint(m_ptFindMark.x,-10);
	ptEnd   = CPoint(m_ptFindMark.x,5000);
	BSchPtoLP(&ptBegin,m_nVExt,m_nWExt);				//�}�ʍ��W��_�����W�ɕϊ�
	BSchPtoLP(&ptEnd,m_nVExt,m_nWExt);				//�}�ʍ��W��_�����W�ɕϊ�
	pDC->MoveTo(ptBegin);
	pDC->LineTo(ptEnd);


	pDC->SetROP2(nOldDrawMode);		//�`�惂�[�h�̕��A
	pDC->SelectObject(ppenOld);
}


void CBSchView::DrawCrossCursor(CDC* pDC)
{
	//TRACE("DrawCrossCursor %d\n",bDraw);

	if(!m_bUseCrossCursor){
		return;
	}

	int nOldDrawMode;
	CPen penNew;
	CPen* ppenOld;
	penNew.CreatePen(PS_SOLID,0,RGB(192,192,192));
	ppenOld=pDC->SelectObject(&penNew);			//�쐬�����y���̑I��
	nOldDrawMode=pDC->SetROP2(R2_XORPEN);	//�`�惂�[�h�̐ݒ�

	CPoint ptBegin;
	CPoint ptEnd;

	ptBegin = CPoint(-10,m_ptMouse.y);
	ptEnd   = CPoint(6000,m_ptMouse.y);
	BSchPtoLP(&ptBegin,m_nVExt,m_nWExt);				//�}�ʍ��W��_�����W�ɕϊ�
	BSchPtoLP(&ptEnd,m_nVExt,m_nWExt);				//�}�ʍ��W��_�����W�ɕϊ�
	pDC->MoveTo(ptBegin);
	pDC->LineTo(ptEnd);

	ptBegin = CPoint(m_ptMouse.x,-10);
	ptEnd   = CPoint(m_ptMouse.x,5000);
	BSchPtoLP(&ptBegin,m_nVExt,m_nWExt);				//�}�ʍ��W��_�����W�ɕϊ�
	BSchPtoLP(&ptEnd,m_nVExt,m_nWExt);				//�}�ʍ��W��_�����W�ɕϊ�
	pDC->MoveTo(ptBegin);
	pDC->LineTo(ptEnd);


	pDC->SetROP2(nOldDrawMode);		//�`�惂�[�h�̕��A
	pDC->SelectObject(ppenOld);


//	RECT rc;
//	rc=CRect(CPoint(-10,-10),m_ptMouse);
//	rc=CRect(-10,m_ptMouse.y,5000,4000);

//	BSchPtoLP(&rc);				//�}�ʍ��W��_�����W�ɕϊ�
//	pDC->DrawFocusRect(&rc);	//�_���g�̕`��
//	rc=CRect(m_ptMouse,CPoint(5000,3000));
//	rc=CRect(m_ptMouse.x,-10,5000,4000);

//	BSchPtoLP(&rc);				//�}�ʍ��W��_�����W�ɕϊ�
//	pDC->DrawFocusRect(&rc);	//�_���g�̕`��
//	m_bDrawCrossCursor=bDraw;
}



//�Վ��f�[�^�̕`��
//dwMode��DRAW_TEMP�̂Ƃ��AbDraw���L���B
//      bDraw��TRUE�̂Ƃ��`��
//             FALSE�̂Ƃ�����
//dwMode��DRAW_OFF�̂Ƃ��Am_bDrawTmpData���N���A(FALSE��)����B
void CBSchView::DrawOccasional(CDC* pDC,BOOL bDraw/*=TRUE*/)
{
	//XOR�`��ŁA�����w��ł��łɏ����ς݂Ȃ牽�����Ȃ�
//	if(bDraw==FALSE && m_bDrawOccasional==FALSE) return;
	if(!pDC) return;
	CPoint point;
	CPen* ppenNew;
	CPen* ppenOld;
	int nOldDrawMode;
	
	//�����E�������̕`��
	if(m_nCurrentTool==ID_TOOL_WIRE || m_nCurrentTool==ID_TOOL_BUS){
		nOldDrawMode=pDC->SetROP2(R2_COPYPEN);	//�`�惂�[�h�̐ݒ�
		int nWidth=1;
		if(m_nCurrentTool==ID_TOOL_BUS) nWidth=3;
		nWidth=(nWidth*m_nVExt)/m_nWExt;
		//�����y���̏���
		ppenNew=new CPen;
		ppenNew->CreatePen(PS_SOLID,nWidth,m_COL.colXOR);
		ppenOld=pDC->SelectObject(ppenNew);
		//�n�_�ւ�MoveTo;
		point=m_ptMoveFrom;
		BSchPtoLP(&point,m_nVExt,m_nWExt);
		pDC->MoveTo(point);
		//���Ԃ̓_���W��ݒ肵�Đ��`��
		if(m_bWireHorizon1st){		//��ɐ�����
			point.x=m_ptMoveTo.x;
			point.y=m_ptMoveFrom.y;
		}else{
			point.x=m_ptMoveFrom.x;
			point.y=m_ptMoveTo.y;
		}
		BSchPtoLP(&point,m_nVExt,m_nWExt);
		pDC->LineTo(point);
		//�I�_�ւ̐��`��;
		point=m_ptMoveTo;
		BSchPtoLP(&point,m_nVExt,m_nWExt);
		pDC->LineTo(point);
		pDC->SelectObject(ppenOld);
		delete ppenNew;	//�����y���̏���
		pDC->SetROP2(nOldDrawMode);	//�`�惂�[�h�̕��A
//�j���̕`��
	}else if(m_nCurrentTool==ID_TOOL_DASH){
		//2010/05/22 �������̈ꎞ�`������ۂ̃A�g���r���[�g�ɏ]�������̂ɏC��
		SXBSchDash decoLine;
		decoLine.setP1(CPoint2SPoint(m_ptMoveFrom));
		decoLine.setP2(CPoint2SPoint(m_ptMoveTo));
		//decoLine.setLayer(pDoc->editLayer());

		decoLine.setLineStyle(	m_tempDecoLine.lineStyle());
		decoLine.setLineWidth( m_tempDecoLine.lineWidth());
		decoLine.setStartPointStyle( m_tempDecoLine.startPointStyle());
		decoLine.setEndPointStyle( m_tempDecoLine.endPointStyle());
		decoLine.setEndMarkSize( m_tempDecoLine.endMarkSize());
		//decoLine.setCurve( m_tempDecoLine.curve());


		g_DrawDash(pDC,&decoLine,DRAW_TEMP,m_nVExt,m_nWExt,NULL);


		//nOldDrawMode=pDC->SetROP2(R2_COPYPEN);	//�`�惂�[�h�̐ݒ�
		////���߃��[�h�̐ݒ�
		//int nOldBkMode=pDC->SetBkMode(TRANSPARENT);
		////�j���y���̏���
		//ppenNew=new CPen;
		//ppenNew->CreatePen(PS_DOT,1,m_COL.colXOR);
		//ppenOld=pDC->SelectObject(ppenNew);
		////�n�_�ւ�MoveTo;
		//point=m_ptMoveFrom;
		//BSchPtoLP(&point,m_nVExt,m_nWExt);
		//pDC->MoveTo(point);
		////�I�_�ւ̐��`��;
		//point=m_ptMoveTo;
		//BSchPtoLP(&point,m_nVExt,m_nWExt);
		//pDC->LineTo(point);
		//pDC->SelectObject(ppenOld);
		//delete ppenNew;	//�j���y���̏���
		//pDC->SetBkMode(nOldBkMode);//���߃��[�h����̕��A
		//pDC->SetROP2(nOldDrawMode);	//�`�惂�[�h�̕��A
//�}�[�J�[���C���̕`��
	}else if(m_nCurrentTool==ID_TOOL_MARKER){
		m_tempMarkerLine.setX1(m_ptMoveFrom.x);
		m_tempMarkerLine.setY1(m_ptMoveFrom.y);
		m_tempMarkerLine.setX2(m_ptMoveTo.x);
		m_tempMarkerLine.setY2(m_ptMoveTo.y);

		g_DrawMarkerLine(
			pDC,							//�`���
			&m_tempMarkerLine,				//�`��I�u�W�F�N�g
			DRAW_TEMP,					//�`�惂�[�h
			m_nVExt,			//�\���{��(View)
			m_nWExt,			//�\���{��(Window)
			NULL);		//�N���b�v���


	//�G���g���[�̕`��
	}else if(  m_nCurrentTool==ID_TOOL_ENTRY_BL
			|| m_nCurrentTool==ID_TOOL_ENTRY_BR){
		SXBSchBusEntry entry;
		entry.setP1(CPoint2SPoint(m_pointOccasional));
		if(m_nCurrentTool==ID_TOOL_ENTRY_BL){
			entry.setAspect(SXBSchEntry::LEFT_DOWN);
		}else{
			entry.setAspect(SXBSchEntry::RIGHT_DOWN);
		}
		g_DrawEntry(pDC,&entry,DRAW_TEMP,m_nVExt,m_nWExt);

	}else if(  m_nCurrentTool==ID_TOOL_ENTRY_WL
			|| m_nCurrentTool==ID_TOOL_ENTRY_WR){
		SXBSchEntry entry;
		entry.setP1(CPoint2SPoint(m_pointOccasional));
		if(m_nCurrentTool==ID_TOOL_ENTRY_WL){
			entry.setAspect(SXBSchEntry::LEFT_DOWN);
		}else{
			entry.setAspect(SXBSchEntry::RIGHT_DOWN);
		}
		g_DrawEntry(pDC,&entry,DRAW_TEMP,m_nVExt,m_nWExt);
	//�ڍ��_�̕`��
	}else if(m_nCurrentTool==ID_TOOL_JUNCTION){
		SXBSchJunc junction;
		junction.setP1(CPoint2SPoint(m_pointOccasional));
		g_DrawJunc(pDC,&junction,DRAW_TEMP,m_nVExt,m_nWExt);
	}
	m_bDrawOccasional=bDraw;
}
