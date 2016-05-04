/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// Place.cpp : CBSchView �N���X�̂����}�ʗv�f�̔z�u�̒�`���s���܂��B

#include "stdafx.h"
#include <assert.h>
#include <string>
#include <list>
using namespace std;

#include "BSch.h"

#include "SetLib.h"
#include "TagAtrb.h"
#include "CmntAtrb.h"
#include "LblAtrb.h"
#include "MarkerLineAtrb.h"

#include "xbschglobal.h"


//#include "BSchObj.h"
//#include "BSchJunc.h"
//#include "BSchEntr.h"
//#include "BSchLine.h"
//#include "BSchWire.h"
//#include "BSchBus.h"
//#include "BSchDash.h"
//#include "BSchTag.h"
//#include "BSchLbl.h"
//#include "BSchCmnt.h"





#include "BSchDoc.h"
#include "BSchView.h"
#include "drawobj.h"

#include "complib.h"
#include "BSchFont.h"
#include "Global.h"

//#include "BSchPart.h"
#include "Preview.h"
#include "SelPart.h"

#include "coord2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//�W�����N�V�����̔z�u
//���W�� m_pointOcccasional
//BOOL CBSchView::PlaceJunction(CDC* pDC,CBSchDoc* pDoc)
BOOL CBSchView::PlaceJunction(CBSchDoc* pDoc)
{
	SXBSchJunc* pObj=new SXBSchJunc;
	if(!pObj) return FALSE;
	pObj->setP1(CPoint2SPoint(m_pointOccasional));		//���W�̐ݒ�
	pObj->setLayer(pDoc->editLayer());

	pDoc->AddData(pObj);				//��������ꎞ�f�[�^�ɂ�����
	//pDoc->FixSelection();				//�ꎞ�f�[�^���t�B�b�N�X
	FixSelection(SXBSchDoc::RESET_SELECT_NORMAL);				//�ꎞ�f�[�^���t�B�b�N�X
	if(g_bDisplayNcPinMark) pDoc->setNcPinFlag();//NC�s���̃t���O�̐ݒ�
//	g_DrawJunc(pDC,pObj,DRAW_ON,m_nVExt,m_nWExt);
	return TRUE;
}

//�G���g���[�̔z�u
//���W�� m_pointOcccasional
//��ނ� m_nCurrentTool
BOOL CBSchView::PlaceEntry(CBSchDoc* pDoc)
{
	SXBSchEntry* pObj = NULL;
	if(m_nCurrentTool==ID_TOOL_ENTRY_BL || m_nCurrentTool==ID_TOOL_ENTRY_BR){
		pObj = new SXBSchBusEntry; 
	}else if(m_nCurrentTool==ID_TOOL_ENTRY_WL || m_nCurrentTool==ID_TOOL_ENTRY_WR){
		pObj = new SXBSchEntry; 
	}
	if(!pObj) return FALSE;
	pObj->setP1(CPoint2SPoint(m_pointOccasional));		//���W�̐ݒ�
	if(m_nCurrentTool==ID_TOOL_ENTRY_BL || m_nCurrentTool==ID_TOOL_ENTRY_WL){
		pObj->setAspect(SXBSchEntry::LEFT_DOWN);
	}else{
		pObj->setAspect(SXBSchEntry::RIGHT_DOWN);
	}
	pObj->setLayer(pDoc->editLayer());

	pDoc->AddData(pObj);		//��������ꎞ�f�[�^�ɂ�����
	FixSelection(SXBSchDoc::RESET_SELECT_NORMAL);		//�ꎞ�f�[�^���t�B�b�N�X
//	pDoc->FixSelection();		//�ꎞ�f�[�^���t�B�b�N�X
//	g_DrawEntry(pDC,pObj,DRAW_ON,m_nVExt,m_nWExt);
	return TRUE;
}


//�^�O�̔z�u
//�P�D�A�g���r���[�g�̃_�C�A���O�ŕ�����Ƙg�����߂�
//�Q�D���o�^
//
//�L�����Z���{�^�����������ꍇ�̓���
//      �u�P�Ƀ_�C�A���O�����v����u�Z���N�^�c�[���ɖ߂�v�ɕύX 1997/01/06
//BOOL CBSchView::PlaceTag(CDC* pDC,CBSchDoc* pDoc,CPoint* ppoint)
BOOL CBSchView::PlaceTag(CBSchDoc* pDoc,CPoint* ppoint)
{
	CTagAtrb dlg;
	dlg.m_rName.Empty();
	dlg.m_nType=0;
	if(dlg.DoModal()==IDCANCEL){
		PostMessage(WM_COMMAND,ID_TOOL_SELECTOR,0); //�Z���N�^�c�[���Ɉڍs���郁�b�Z�[�W
		return TRUE;
	}

	SXBSchTag* pObj=new SXBSchTag;
	CPoint point=*ppoint;
	SnapToGrid(&point);			//�O���b�h�ւ̃X�i�b�v
	pObj->setP1(CPoint2SPoint(point));		//�����W�̐ݒ�

	pObj->setTagType(dlg.m_nType);
	pObj->setText(dlg.m_rName);
	pObj->setLayer(pDoc->editLayer());

	pDoc->AddData(pObj);					//�ꎞ�f�[�^�ɒǉ�
//	pDoc->SetDirty();
	return TRUE;
}

//���x���̔z�u
//�P�D�A�g���r���[�g�̃_�C�A���O�ŕ���������߂�
//�Q�D���o�^
//
//�L�����Z���{�^�����������ꍇ�̓���
//      �u�P�Ƀ_�C�A���O�����v����u�Z���N�^�c�[���ɖ߂�v�ɕύX 1997/01/06
//BOOL CBSchView::PlaceLabel(CDC* pDC,CBSchDoc* pDoc,CPoint* ppoint)
BOOL CBSchView::PlaceLabel(CBSchDoc* pDoc,CPoint* ppoint)
{
	CLblAtrb dlg;
	dlg.m_rText.Empty();
	if(dlg.DoModal()==IDCANCEL){
		PostMessage(WM_COMMAND,ID_TOOL_SELECTOR,0); //�Z���N�^�c�[���Ɉڍs���郁�b�Z�[�W
		return TRUE;
	}
	if(dlg.m_rText.IsEmpty()) return TRUE;
	SXBSchLabel* pObj=new SXBSchLabel;
	pObj->setText(dlg.m_rText);
	pObj->setLayer(pDoc->editLayer());
	CPoint point=*ppoint;
	SnapToGrid(&point);					//�O���b�h�ւ̃X�i�b�v
	pObj->setP1(CPoint2SPoint(point));	//�����W�̐ݒ�
	pDoc->AddData(pObj);				//�ꎞ�f�[�^�ɒǉ�
//	pDoc->SetDirty();
	return TRUE;
}



//�R�����g�̔z�u
//�P�D�A�g���r���[�g�̃_�C�A���O�ŕ�����ƕ����̎�ނ����߂�
//�Q�D���o�^
//
//�L�����Z���{�^�����������ꍇ�̓���
//      �u�P�Ƀ_�C�A���O�����v����u�Z���N�^�c�[���ɖ߂�v�ɕύX 1997/01/06
//BOOL CBSchView::PlaceComment(CDC* pDC,CBSchDoc* pDoc,CPoint* ppoint)
BOOL CBSchView::PlaceComment(CBSchDoc* pDoc,CPoint* ppoint)
{
//	int n;
	CCmntAtrb dlg;
	dlg.m_rText.Empty();
	g_IniReadFontInfo(&dlg.m_logfont);

	if(dlg.DoModal()==IDCANCEL){
		PostMessage(WM_COMMAND,ID_TOOL_SELECTOR,0); //�Z���N�^�c�[���Ɉڍs���郁�b�Z�[�W
		return TRUE;
	}

	if(dlg.m_rText.IsEmpty()) return TRUE;
	SXBSchComment* pObj=new SXBSchComment;

	CPoint point=*ppoint;
	SnapToGrid(&point);			//�O���b�h�ւ̃X�i�b�v
	pObj->setP1(CPoint2SPoint(point));		//�����W�̐ݒ�

	pObj->setText(dlg.m_rText);
	pObj->setFontSize(abs(dlg.m_logfont.lfHeight));
	pObj->setFontBold(dlg.m_logfont.lfWeight>=FW_BOLD);
	pObj->setFontItalic(dlg.m_logfont.lfItalic!=0);
	pObj->setFontName(dlg.m_logfont.lfFaceName);
	pObj->setEnableTag(dlg.m_enableTag);
	if(dlg.m_enableWidthSetting){
		pObj->setWidth(dlg.m_width);
	}else{
		pObj->setWidth(-1);
	}


	
	pObj->setLayer(pDoc->editLayer());
//	m_bDrawTmpData=TRUE;		//�ꎞ�f�[�^��`�悵�����Ƃɂ���
	pDoc->AddData(pObj);		//�ꎞ�f�[�^�ɒǉ�
//	pDoc->SetDirty();		//����������Ă����Ȃ��ƏI�����ɕۑ��𕷂��ė��Ȃ����Ƃ�����
	return TRUE;
}



//���i�̔z�u
//�P�D���i�I���̃_�C�A���O�ŕ��i�̖��O�𓾂�
//�Q�D���i�̖��O���畔�i�C���f�b�N�X�ւ̃|�C���^�𓾂�
//�R�D���i�N���X���m�ۂ���
//�S�D���o�^
//
//�L�����Z���{�^�����������ꍇ�̓���
//      �u�P�Ƀ_�C�A���O�����v����u�Z���N�^�c�[���ɖ߂�v�ɕύX 1997/01/06
//BOOL CBSchView::PlacePart(CDC* pDC,CBSchDoc* pDoc,CPoint* ppoint)
BOOL CBSchView::PlacePart(CBSchDoc* pDoc,CPoint* ppoint)
{
	CSelPart dlg;
	dlg.m_bUsePreview=TRUE;		//�v���r���[���g�����ǂ����̐ݒ�
//	dlg.m_bUsePreview=m_bUsePreview;	//�v���r���[���g�����ǂ����̐ݒ�
//	dlg.m_nLastSelectPart=m_nLastSelectPart;
//
//  CPartLib.m_nLastSelectLibrary�V�݂ł��̂����荬���C���B�������K�v����
//					1997/01/28
	if(g_LibraryInfo(m_nLastSelectLibrary)){//NULL�łȂ����...
//		dlg.m_nLastSelectPart=g_pPartLib[m_nLastSelectLibrary]->m_nLastSelectIndex;
		dlg.m_nLastSelectLibrary=m_nLastSelectLibrary;
		dlg.m_nLastSelectPart=m_nLastSelectPart;
	}else{
//		dlg.m_nLastSelectPart=0;
		dlg.m_nLastSelectLibrary=0;
		dlg.m_nLastSelectPart=0;
	}
	if(dlg.DoModal()==IDCANCEL){
		PostMessage(WM_COMMAND,ID_TOOL_SELECTOR,0); //�Z���N�^�c�[���Ɉڍs���郁�b�Z�[�W
		return TRUE;
	}

	const SCompIndex* pPartIndex = dlg.m_pCompIndex;
	if(pPartIndex==NULL){
		CString rString;
		//���b�Z�[�W�{�b�N�X�p�̃��b�Z�[�W�̍쐬
		AfxFormatString1(rString,IDS_NOT_FOUND_PART,dlg.m_rStrPartName); 
		AfxMessageBox(rString,MB_OK|MB_ICONEXCLAMATION);
		return TRUE;
	}
	m_nLastSelectPart=    dlg.m_nLastSelectPart;
	m_nLastSelectLibrary= dlg.m_nLastSelectLibrary;

	
//	int nLib;
//	int nPart;
//
//
//	const SCompIndex* pPartIndex = g_SearchComponentIndex(dlg.m_rStrPartName,&nLib,&nPart,NULL);
//	if(pPartIndex==NULL){
//		CString rString;
//		//���b�Z�[�W�{�b�N�X�p�̃��b�Z�[�W�̍쐬
//		AfxFormatString1(rString,IDS_NOT_FOUND_PART,dlg.m_rStrPartName); 
//		AfxMessageBox(rString,MB_OK|MB_ICONEXCLAMATION);
//		return TRUE;
//	}
//	m_nLastSelectPart=nPart;
//	m_nLastSelectLibrary=nLib;
//	g_pPartLib[nLib]->m_nLastSelectIndex=nPart;
	

	SXBSchComponent* pObj=new SXBSchComponent;
	if(!pObj) return FALSE;
	pObj->setCompInfoIndex(pPartIndex);
	pObj->setName(pPartIndex->name());
	pObj->setRefnum(pPartIndex->number());
	pObj->setMfr(pPartIndex->Mfr());
	pObj->setMFrPNum(pPartIndex->MfrPn());
	pObj->setPackageName(pPartIndex->Pkg());

//	pObj->SetPartIndex(pPartIndex);
	//���W�̌���
	//�P�D(0,0)�ɉ��z�u���Ă��� GetPointLT()�ō�����W�𓾂�
	//�Q�D�z�u�������Ԃ�E���ɂ��炷
	CPoint point=CPoint(0,0);	//��������(0,0)�ɉ��z�u
	pObj->setLayer(pDoc->editLayer());

//	pObj->SetPoint(&point);		//�����W�̐ݒ�
//	point=pObj->GetPointLT();	//������W�𓾂�
	SnapToGrid(ppoint);			//�O���b�h�ւ̃X�i�b�v
//	point.x=ppoint->x - point.x;//�E���ւ��炷
//	point.y=ppoint->y - point.y;
	pObj->setP1(CPoint2SPoint(*ppoint));		//���W�̐ݒ�
//	m_bDrawTmpData=TRUE;		//�ꎞ�f�[�^��`�悵�����Ƃɂ���
	m_nFocusedTo = FOCUSED_TO_BODY;
	pDoc->AddData(pObj);		//�ꎞ�f�[�^�ɒǉ�
//	pDoc->SetDirty();		//����������Ă����Ȃ��ƏI�����ɕۑ��𕷂��ė��Ȃ����Ƃ�����
	return TRUE;
}



//���C���[�̔z�u
//�z�u���W�́Am_bMoveFrom,m_bMoveTo,m_bWireHorizon1st ����v�Z
//BOOL CBSchView::PlaceWire(CDC* pDC,CBSchDoc* pDoc)
BOOL CBSchView::PlaceWire(CBSchDoc* pDoc)
{
	CPoint point;
	if(m_bWireHorizon1st){		//��ɐ�����
		point.x=m_ptMoveTo.x;
		point.y=m_ptMoveFrom.y;
	}else{
		point.x=m_ptMoveFrom.x;
		point.y=m_ptMoveTo.y;
	}
	SXBSchWire* pObj;
	bool bPlace = false;
	//�P�{�ڂ̃��C���[�I�u�W�F�N�g�̍쐬�Ɠo�^
	if(m_ptMoveFrom!=point){
		pObj=new SXBSchWire;
		if(!pObj) return FALSE;
		pObj->setP1(CPoint2SPoint(m_ptMoveFrom));
		pObj->setP2(CPoint2SPoint(point));
		pObj->setLayer(pDoc->editLayer());
		pDoc->AddData(pObj);
		//pDoc->SetDirty();
		//pDoc->FixSelection();	//�ꎞ�f�[�^���t�B�b�N�X
		FixSelection(SXBSchDoc::RESET_SELECT_NORMAL);	//�ꎞ�f�[�^���t�B�b�N�X
		//g_DrawWire(pDC,pObj,DRAW_ON,m_nVExt,m_nWExt);
		bPlace = true;
	}
	//�Q�{�ڂ̃��C���[�I�u�W�F�N�g�̍쐬�Ɠo�^
	if(point!=m_ptMoveTo){
		pObj=new SXBSchWire;
		if(!pObj) return FALSE;
		pObj->setP1(CPoint2SPoint(point));
		pObj->setP2(CPoint2SPoint(m_ptMoveTo));
		pObj->setLayer(pDoc->editLayer());
		pDoc->AddData(pObj);
		//pDoc->SetDirty();
		//pDoc->FixSelection();	//�ꎞ�f�[�^���t�B�b�N�X
		FixSelection(SXBSchDoc::RESET_SELECT_NORMAL);	//�ꎞ�f�[�^���t�B�b�N�X
		bPlace = true;
		//g_DrawWire(pDC,pObj,DRAW_ON,m_nVExt,m_nWExt);
	}
	if(bPlace){
		if(g_bDisplayNcPinMark) pDoc->setNcPinFlag();//NC�s���̃t���O�̐ݒ�
	}
	return TRUE;
}	

//�o�X�̔z�u
//�z�u���W�́Am_bMoveFrom,m_bMoveTo,m_bWireHorizon1st ����v�Z
//BOOL CBSchView::PlaceBus(CDC* pDC,CBSchDoc* pDoc)
BOOL CBSchView::PlaceBus(CBSchDoc* pDoc)
{
	CPoint point;
	if(m_bWireHorizon1st){		//��ɐ�����
		point.x=m_ptMoveTo.x;
		point.y=m_ptMoveFrom.y;
	}else{
		point.x=m_ptMoveFrom.x;
		point.y=m_ptMoveTo.y;
	}
	SXBSchBus* pObj;
	//�P�{�ڂ̃o�X�I�u�W�F�N�g�̍쐬�Ɠo�^
	if(m_ptMoveFrom!=point){
		pObj=new SXBSchBus;
		if(!pObj) return FALSE;
		pObj->setP1(CPoint2SPoint(m_ptMoveFrom));
		pObj->setP2(CPoint2SPoint(point));
		pObj->setLayer(pDoc->editLayer());
		pDoc->AddData(pObj);
		//pDoc->SetDirty();
		//pDoc->FixSelection();	//�ꎞ�f�[�^���t�B�b�N�X
		FixSelection(SXBSchDoc::RESET_SELECT_NORMAL);	//�ꎞ�f�[�^���t�B�b�N�X
		//g_DrawBus(pDC,pObj,DRAW_ON,m_nVExt,m_nWExt);
	}
	//�Q�{�ڂ̃o�X�I�u�W�F�N�g�̍쐬�Ɠo�^
	if(point!=m_ptMoveTo){
		pObj=new SXBSchBus;
		if(!pObj) return FALSE;
		pObj->setP1(CPoint2SPoint(point));
		pObj->setP2(CPoint2SPoint(m_ptMoveTo));
		pObj->setLayer(pDoc->editLayer());

		pDoc->AddData(pObj);
		//pDoc->SetDirty();
		//pDoc->FixSelection();	//�ꎞ�f�[�^���t�B�b�N�X
		FixSelection(SXBSchDoc::RESET_SELECT_NORMAL);	//�ꎞ�f�[�^���t�B�b�N�X
		//g_DrawBus(pDC,pObj,DRAW_ON,m_nVExt,m_nWExt);
	}
	return TRUE;
}	

//�j���̔z�u
//�z�u���W�́Am_bMoveFrom,m_bMoveTo
//BOOL CBSchView::PlaceDash(CDC* pDC,CBSchDoc* pDoc)
BOOL CBSchView::PlaceDash(CBSchDoc* pDoc)
{
	SXBSchDash* pObj;
	//�j���I�u�W�F�N�g�̍쐬�Ɠo�^
	if(m_ptMoveFrom!=m_ptMoveTo){
		pObj=new SXBSchDash;
		if(!pObj) return FALSE;
		pObj->setP1(CPoint2SPoint(m_ptMoveFrom));
		pObj->setP2(CPoint2SPoint(m_ptMoveTo));
		pObj->setLayer(pDoc->editLayer());

		pObj->setLineStyle(	m_tempDecoLine.lineStyle());
		pObj->setLineWidth( m_tempDecoLine.lineWidth());
		pObj->setStartPointStyle( m_tempDecoLine.startPointStyle());
		pObj->setEndPointStyle( m_tempDecoLine.endPointStyle());
		pObj->setEndMarkSize( m_tempDecoLine.endMarkSize());
		pObj->setCurve( m_tempDecoLine.curve());

		pDoc->AddData(pObj);
		//pDoc->SetDirty();
		//pDoc->FixSelection();	//�ꎞ�f�[�^���t�B�b�N�X
		FixSelection(SXBSchDoc::RESET_SELECT_NORMAL);	//�ꎞ�f�[�^���t�B�b�N�X
		//g_DrawDash(pDC,pObj,DRAW_ON,m_nVExt,m_nWExt);
	}
	return TRUE;
}

//�}�[�J�[���C���̔z�u
//�z�u���W�́Am_bMoveFrom,m_bMoveTo
BOOL CBSchView::PlaceMarkerLine(CBSchDoc* pDoc)
{
	SXBSchObj* pObj;
	//�}�[�J�[���C���I�u�W�F�N�g�̍쐬�Ɠo�^
	if(m_ptMoveFrom!=m_ptMoveTo){
		m_tempMarkerLine.setX1(m_ptMoveFrom.x);
		m_tempMarkerLine.setY1(m_ptMoveFrom.y);
		m_tempMarkerLine.setX2(m_ptMoveTo.x);
		m_tempMarkerLine.setY2(m_ptMoveTo.y);

		pObj=m_tempMarkerLine.duplicate();
		if(!pObj) return FALSE;
		pObj->setLayer(pDoc->editLayer());
		pDoc->AddData(pObj);
		//pDoc->FixSelection();	//�ꎞ�f�[�^���t�B�b�N�X
		FixSelection(SXBSchDoc::RESET_SELECT_NORMAL);	//�ꎞ�f�[�^���t�B�b�N�X
	}else{
		//if(DoAttributeDialogMarkerLine(&m_tempMarkerLine)){
		//	IniWriteMarkerLineInitValue();
		//}
		//CMarkerLineAtrb dlg;
		//dlg.m_nColor = m_tempMarkerLine.m_nColor;
		//dlg.m_nWidth = m_tempMarkerLine.m_width;
		//if(dlg.DoModal()==IDOK){
		//	m_tempMarkerLine.m_nColor = dlg.m_nColor;
		//	m_tempMarkerLine.m_width = dlg.m_nWidth;
		//	IniWriteMarkerLineInitValue();
		//}
	}
	return TRUE;
}

