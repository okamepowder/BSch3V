/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// Place.cpp : CBSchView クラスのうち図面要素の配置の定義を行います。

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


//ジャンクションの配置
//座標は m_pointOcccasional
//BOOL CBSchView::PlaceJunction(CDC* pDC,CBSchDoc* pDoc)
BOOL CBSchView::PlaceJunction(CBSchDoc* pDoc)
{
	SXBSchJunc* pObj=new SXBSchJunc;
	if(!pObj) return FALSE;
	pObj->setP1(CPoint2SPoint(m_pointOccasional));		//座標の設定
	pObj->setLayer(pDoc->editLayer());

	pDoc->AddData(pObj);				//いったん一時データにおいて
	//pDoc->FixSelection();				//一時データをフィックス
	FixSelection(SXBSchDoc::RESET_SELECT_NORMAL);				//一時データをフィックス
	if(g_bDisplayNcPinMark) pDoc->setNcPinFlag();//NCピンのフラグの設定
//	g_DrawJunc(pDC,pObj,DRAW_ON,m_nVExt,m_nWExt);
	return TRUE;
}

//エントリーの配置
//座標は m_pointOcccasional
//種類は m_nCurrentTool
BOOL CBSchView::PlaceEntry(CBSchDoc* pDoc)
{
	SXBSchEntry* pObj = NULL;
	if(m_nCurrentTool==ID_TOOL_ENTRY_BL || m_nCurrentTool==ID_TOOL_ENTRY_BR){
		pObj = new SXBSchBusEntry; 
	}else if(m_nCurrentTool==ID_TOOL_ENTRY_WL || m_nCurrentTool==ID_TOOL_ENTRY_WR){
		pObj = new SXBSchEntry; 
	}
	if(!pObj) return FALSE;
	pObj->setP1(CPoint2SPoint(m_pointOccasional));		//座標の設定
	if(m_nCurrentTool==ID_TOOL_ENTRY_BL || m_nCurrentTool==ID_TOOL_ENTRY_WL){
		pObj->setAspect(SXBSchEntry::LEFT_DOWN);
	}else{
		pObj->setAspect(SXBSchEntry::RIGHT_DOWN);
	}
	pObj->setLayer(pDoc->editLayer());

	pDoc->AddData(pObj);		//いったん一時データにおいて
	FixSelection(SXBSchDoc::RESET_SELECT_NORMAL);		//一時データをフィックス
//	pDoc->FixSelection();		//一時データをフィックス
//	g_DrawEntry(pDC,pObj,DRAW_ON,m_nVExt,m_nWExt);
	return TRUE;
}


//タグの配置
//１．アトリビュートのダイアログで文字列と枠を決める
//２．仮登録
//
//キャンセルボタンを押した場合の動作
//      「単にダイアログを閉じる」から「セレクタツールに戻る」に変更 1997/01/06
//BOOL CBSchView::PlaceTag(CDC* pDC,CBSchDoc* pDoc,CPoint* ppoint)
BOOL CBSchView::PlaceTag(CBSchDoc* pDoc,CPoint* ppoint)
{
	CTagAtrb dlg;
	dlg.m_rName.Empty();
	dlg.m_nType=0;
	if(dlg.DoModal()==IDCANCEL){
		PostMessage(WM_COMMAND,ID_TOOL_SELECTOR,0); //セレクタツールに移行するメッセージ
		return TRUE;
	}

	SXBSchTag* pObj=new SXBSchTag;
	CPoint point=*ppoint;
	SnapToGrid(&point);			//グリッドへのスナップ
	pObj->setP1(CPoint2SPoint(point));		//仮座標の設定

	pObj->setTagType(dlg.m_nType);
	pObj->setText(dlg.m_rName);
	pObj->setLayer(pDoc->editLayer());

	pDoc->AddData(pObj);					//一時データに追加
//	pDoc->SetDirty();
	return TRUE;
}

//ラベルの配置
//１．アトリビュートのダイアログで文字列を決める
//２．仮登録
//
//キャンセルボタンを押した場合の動作
//      「単にダイアログを閉じる」から「セレクタツールに戻る」に変更 1997/01/06
//BOOL CBSchView::PlaceLabel(CDC* pDC,CBSchDoc* pDoc,CPoint* ppoint)
BOOL CBSchView::PlaceLabel(CBSchDoc* pDoc,CPoint* ppoint)
{
	CLblAtrb dlg;
	dlg.m_rText.Empty();
	if(dlg.DoModal()==IDCANCEL){
		PostMessage(WM_COMMAND,ID_TOOL_SELECTOR,0); //セレクタツールに移行するメッセージ
		return TRUE;
	}
	if(dlg.m_rText.IsEmpty()) return TRUE;
	SXBSchLabel* pObj=new SXBSchLabel;
	pObj->setText(dlg.m_rText);
	pObj->setLayer(pDoc->editLayer());
	CPoint point=*ppoint;
	SnapToGrid(&point);					//グリッドへのスナップ
	pObj->setP1(CPoint2SPoint(point));	//仮座標の設定
	pDoc->AddData(pObj);				//一時データに追加
//	pDoc->SetDirty();
	return TRUE;
}



//コメントの配置
//１．アトリビュートのダイアログで文字列と文字の種類を決める
//２．仮登録
//
//キャンセルボタンを押した場合の動作
//      「単にダイアログを閉じる」から「セレクタツールに戻る」に変更 1997/01/06
//BOOL CBSchView::PlaceComment(CDC* pDC,CBSchDoc* pDoc,CPoint* ppoint)
BOOL CBSchView::PlaceComment(CBSchDoc* pDoc,CPoint* ppoint)
{
//	int n;
	CCmntAtrb dlg;
	dlg.m_rText.Empty();
	g_IniReadFontInfo(&dlg.m_logfont);

	if(dlg.DoModal()==IDCANCEL){
		PostMessage(WM_COMMAND,ID_TOOL_SELECTOR,0); //セレクタツールに移行するメッセージ
		return TRUE;
	}

	if(dlg.m_rText.IsEmpty()) return TRUE;
	SXBSchComment* pObj=new SXBSchComment;

	CPoint point=*ppoint;
	SnapToGrid(&point);			//グリッドへのスナップ
	pObj->setP1(CPoint2SPoint(point));		//仮座標の設定

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
//	m_bDrawTmpData=TRUE;		//一時データを描画したことにする
	pDoc->AddData(pObj);		//一時データに追加
//	pDoc->SetDirty();		//これを書いておかないと終了時に保存を聞いて来ないことがある
	return TRUE;
}



//部品の配置
//１．部品選択のダイアログで部品の名前を得る
//２．部品の名前から部品インデックスへのポインタを得る
//３．部品クラスを確保して
//４．仮登録
//
//キャンセルボタンを押した場合の動作
//      「単にダイアログを閉じる」から「セレクタツールに戻る」に変更 1997/01/06
//BOOL CBSchView::PlacePart(CDC* pDC,CBSchDoc* pDoc,CPoint* ppoint)
BOOL CBSchView::PlacePart(CBSchDoc* pDoc,CPoint* ppoint)
{
	CSelPart dlg;
	dlg.m_bUsePreview=TRUE;		//プレビューを使うかどうかの設定
//	dlg.m_bUsePreview=m_bUsePreview;	//プレビューを使うかどうかの設定
//	dlg.m_nLastSelectPart=m_nLastSelectPart;
//
//  CPartLib.m_nLastSelectLibrary新設でこのあたり混乱気味。整理が必要かも
//					1997/01/28
	if(g_LibraryInfo(m_nLastSelectLibrary)){//NULLでなければ...
//		dlg.m_nLastSelectPart=g_pPartLib[m_nLastSelectLibrary]->m_nLastSelectIndex;
		dlg.m_nLastSelectLibrary=m_nLastSelectLibrary;
		dlg.m_nLastSelectPart=m_nLastSelectPart;
	}else{
//		dlg.m_nLastSelectPart=0;
		dlg.m_nLastSelectLibrary=0;
		dlg.m_nLastSelectPart=0;
	}
	if(dlg.DoModal()==IDCANCEL){
		PostMessage(WM_COMMAND,ID_TOOL_SELECTOR,0); //セレクタツールに移行するメッセージ
		return TRUE;
	}

	const SCompIndex* pPartIndex = dlg.m_pCompIndex;
	if(pPartIndex==NULL){
		CString rString;
		//メッセージボックス用のメッセージの作成
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
//		//メッセージボックス用のメッセージの作成
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
	//座標の決定
	//１．(0,0)に仮配置してから GetPointLT()で左上座標を得る
	//２．配置先を左上ぶん右下にずらす
	CPoint point=CPoint(0,0);	//いったん(0,0)に仮配置
	pObj->setLayer(pDoc->editLayer());

//	pObj->SetPoint(&point);		//仮座標の設定
//	point=pObj->GetPointLT();	//左上座標を得る
	SnapToGrid(ppoint);			//グリッドへのスナップ
//	point.x=ppoint->x - point.x;//右下へずらす
//	point.y=ppoint->y - point.y;
	pObj->setP1(CPoint2SPoint(*ppoint));		//座標の設定
//	m_bDrawTmpData=TRUE;		//一時データを描画したことにする
	m_nFocusedTo = FOCUSED_TO_BODY;
	pDoc->AddData(pObj);		//一時データに追加
//	pDoc->SetDirty();		//これを書いておかないと終了時に保存を聞いて来ないことがある
	return TRUE;
}



//ワイヤーの配置
//配置座標は、m_bMoveFrom,m_bMoveTo,m_bWireHorizon1st から計算
//BOOL CBSchView::PlaceWire(CDC* pDC,CBSchDoc* pDoc)
BOOL CBSchView::PlaceWire(CBSchDoc* pDoc)
{
	CPoint point;
	if(m_bWireHorizon1st){		//先に水平線
		point.x=m_ptMoveTo.x;
		point.y=m_ptMoveFrom.y;
	}else{
		point.x=m_ptMoveFrom.x;
		point.y=m_ptMoveTo.y;
	}
	SXBSchWire* pObj;
	bool bPlace = false;
	//１本目のワイヤーオブジェクトの作成と登録
	if(m_ptMoveFrom!=point){
		pObj=new SXBSchWire;
		if(!pObj) return FALSE;
		pObj->setP1(CPoint2SPoint(m_ptMoveFrom));
		pObj->setP2(CPoint2SPoint(point));
		pObj->setLayer(pDoc->editLayer());
		pDoc->AddData(pObj);
		//pDoc->SetDirty();
		//pDoc->FixSelection();	//一時データをフィックス
		FixSelection(SXBSchDoc::RESET_SELECT_NORMAL);	//一時データをフィックス
		//g_DrawWire(pDC,pObj,DRAW_ON,m_nVExt,m_nWExt);
		bPlace = true;
	}
	//２本目のワイヤーオブジェクトの作成と登録
	if(point!=m_ptMoveTo){
		pObj=new SXBSchWire;
		if(!pObj) return FALSE;
		pObj->setP1(CPoint2SPoint(point));
		pObj->setP2(CPoint2SPoint(m_ptMoveTo));
		pObj->setLayer(pDoc->editLayer());
		pDoc->AddData(pObj);
		//pDoc->SetDirty();
		//pDoc->FixSelection();	//一時データをフィックス
		FixSelection(SXBSchDoc::RESET_SELECT_NORMAL);	//一時データをフィックス
		bPlace = true;
		//g_DrawWire(pDC,pObj,DRAW_ON,m_nVExt,m_nWExt);
	}
	if(bPlace){
		if(g_bDisplayNcPinMark) pDoc->setNcPinFlag();//NCピンのフラグの設定
	}
	return TRUE;
}	

//バスの配置
//配置座標は、m_bMoveFrom,m_bMoveTo,m_bWireHorizon1st から計算
//BOOL CBSchView::PlaceBus(CDC* pDC,CBSchDoc* pDoc)
BOOL CBSchView::PlaceBus(CBSchDoc* pDoc)
{
	CPoint point;
	if(m_bWireHorizon1st){		//先に水平線
		point.x=m_ptMoveTo.x;
		point.y=m_ptMoveFrom.y;
	}else{
		point.x=m_ptMoveFrom.x;
		point.y=m_ptMoveTo.y;
	}
	SXBSchBus* pObj;
	//１本目のバスオブジェクトの作成と登録
	if(m_ptMoveFrom!=point){
		pObj=new SXBSchBus;
		if(!pObj) return FALSE;
		pObj->setP1(CPoint2SPoint(m_ptMoveFrom));
		pObj->setP2(CPoint2SPoint(point));
		pObj->setLayer(pDoc->editLayer());
		pDoc->AddData(pObj);
		//pDoc->SetDirty();
		//pDoc->FixSelection();	//一時データをフィックス
		FixSelection(SXBSchDoc::RESET_SELECT_NORMAL);	//一時データをフィックス
		//g_DrawBus(pDC,pObj,DRAW_ON,m_nVExt,m_nWExt);
	}
	//２本目のバスオブジェクトの作成と登録
	if(point!=m_ptMoveTo){
		pObj=new SXBSchBus;
		if(!pObj) return FALSE;
		pObj->setP1(CPoint2SPoint(point));
		pObj->setP2(CPoint2SPoint(m_ptMoveTo));
		pObj->setLayer(pDoc->editLayer());

		pDoc->AddData(pObj);
		//pDoc->SetDirty();
		//pDoc->FixSelection();	//一時データをフィックス
		FixSelection(SXBSchDoc::RESET_SELECT_NORMAL);	//一時データをフィックス
		//g_DrawBus(pDC,pObj,DRAW_ON,m_nVExt,m_nWExt);
	}
	return TRUE;
}	

//破線の配置
//配置座標は、m_bMoveFrom,m_bMoveTo
//BOOL CBSchView::PlaceDash(CDC* pDC,CBSchDoc* pDoc)
BOOL CBSchView::PlaceDash(CBSchDoc* pDoc)
{
	SXBSchDash* pObj;
	//破線オブジェクトの作成と登録
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
		//pDoc->FixSelection();	//一時データをフィックス
		FixSelection(SXBSchDoc::RESET_SELECT_NORMAL);	//一時データをフィックス
		//g_DrawDash(pDC,pObj,DRAW_ON,m_nVExt,m_nWExt);
	}
	return TRUE;
}

//マーカーラインの配置
//配置座標は、m_bMoveFrom,m_bMoveTo
BOOL CBSchView::PlaceMarkerLine(CBSchDoc* pDoc)
{
	SXBSchObj* pObj;
	//マーカーラインオブジェクトの作成と登録
	if(m_ptMoveFrom!=m_ptMoveTo){
		m_tempMarkerLine.setX1(m_ptMoveFrom.x);
		m_tempMarkerLine.setY1(m_ptMoveFrom.y);
		m_tempMarkerLine.setX2(m_ptMoveTo.x);
		m_tempMarkerLine.setY2(m_ptMoveTo.y);

		pObj=m_tempMarkerLine.duplicate();
		if(!pObj) return FALSE;
		pObj->setLayer(pDoc->editLayer());
		pDoc->AddData(pObj);
		//pDoc->FixSelection();	//一時データをフィックス
		FixSelection(SXBSchDoc::RESET_SELECT_NORMAL);	//一時データをフィックス
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

