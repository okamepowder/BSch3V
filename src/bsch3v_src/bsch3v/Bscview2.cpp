/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// 2 of 2

#include "stdafx.h"
#include <stdlib.h> //abs()のためにインクルード 
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


//グリッドの描画
void CBSchView::DrawSheetGrid(CDC* pDC,const CRect& rcClip)
{
	if(m_nVExt<m_nWExt || !m_bDisplayGrid) return;

	CBSchDoc* pDoc = GetDocument();				//ドキュメントのポインタを得て
	CSize sz = pDoc->GetSheetSize();				//図面サイズを得る
	
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


//図面枠の水平座標文字の描画
void CBSchView::DrawSheetFrameCharH(CDC* pDC,COLORREF col,int ny,int nWidth,UINT nMode,int nVExt,int nWExt)
{
	int x=100;
	TCHAR c='A';
	while(x<nWidth){
		g_pFont->PutFont(
				pDC,				//出力先のデバイスコンテキスト
				x,ny,				//出力位置
				&c,					//出力文字列
				1,					//文字列長
				col,				//描画色
				nMode,				//描画モード
				FALSE,
				0,
				nVExt,
				nWExt);
		x+=200;
		c++;
	}
}

//図面枠の垂直座標文字の描画
void CBSchView::DrawSheetFrameCharV(CDC* pDC,COLORREF col,int nx,int nWidth,UINT nMode,int nVExt,int nWExt)
{
	int y=100;
	TCHAR c='1';
	while(y<nWidth){
		g_pFont->PutFont(
				pDC,				//出力先のデバイスコンテキスト
				nx,y,				//出力位置
				&c,					//出力文字列
				1,					//文字列長
				col,				//描画色
				nMode,				//描画モード
				FALSE,
				0,
				nVExt,
				nWExt);

		y+=200;
		if(c=='9') c='0';
		else c++;
	}
}


//印刷時の背景色の描画
//dwModeは DRAW_ONのみが有効（その他のときは何もしない）
void CBSchView::DrawPrintBg(CDC* pDC,DWORD dwMode,int nVExt,int nWExt,const CRect* prc)
{
	CSize size;				//図面のサイズ
	CRect rc;

	if((dwMode & DRAW_ON)==0) return;

	//座標の設定
	if(prc == NULL){
		CBSchDoc* pDoc = GetDocument();				//ドキュメントのポインタを得て
		size = pDoc->GetSheetSize();				//図面サイズを得る
		rc = CRect(CPoint(0,0),size);
	}else{
		rc = *prc;
	}
	BSchPtoLP(&rc,nVExt,nWExt);
	pDC->FillSolidRect(&rc,m_COL.colBG);	//2009/01/11
}


//図面枠の描画
//dwModeは DRAW_ONとDRAW_OFFのみが有効（その他は無視）
void CBSchView::DrawSheetFrame(CDC* pDC,DWORD dwMode,int nVExt,int nWExt,const CRect& rcClip)
{
	CSize sizeOrg,size;				//図面のサイズ
	COLORREF col;					//枠の描画色
	CRect rcOutside, rcInside;		//枠の座標
	int nFrameWidth=10;				//枠の幅

	//座標の設定
	nFrameWidth=nFrameWidth*nVExt/nWExt;	//ズーム比に対応する枠幅に変換
	CBSchDoc* pDoc = GetDocument();				//ドキュメントのポインタを得て
	sizeOrg = pDoc->GetSheetSize();				//図面サイズを得る
	size=sizeOrg;
	BSchPtoLP(&size,nVExt,nWExt);
	rcOutside=CRect(0,0,size.cx-1,size.cy-1);	//外枠の座標の設定
	rcInside =CRect(nFrameWidth,nFrameWidth,	//内枠の座標の設定
		size.cx-nFrameWidth,size.cy-nFrameWidth);
	//描画色の設定	
	if(dwMode & DRAW_ON) col=m_COL.colSHEET_FRAME;	//DRAW_ONではBSch.Hで定義した色
	else				 col=RGB(0,0,0);		//DRAW_OFFでは黒での描画

	int width = 1*nVExt/nWExt;

	//枠の描画
	CPen* pPen=new CPen(PS_SOLID,width,col);
	pPen=pDC->SelectObject(pPen);	//作成したペンの選択
	if(rcClip.left<=10){				//左内枠線の描画
		pDC->MoveTo(rcInside.left,rcInside.top);
		pDC->LineTo(rcInside.left,rcInside.bottom);
		if(nVExt>=nWExt)	//等倍描画のときには座標文字も描画
			DrawSheetFrameCharV(pDC,col,2,sizeOrg.cy,dwMode,nVExt,nWExt);
		if(rcClip.left<=0){			//左外枠線の描画
			pDC->MoveTo(rcOutside.left,rcOutside.top);
			pDC->LineTo(rcOutside.left,rcOutside.bottom);
		}
	}
	if(rcClip.top<=10){				//上内枠線の描画
		pDC->MoveTo(rcInside.left,rcInside.top);
		pDC->LineTo(rcInside.right,rcInside.top);
		if(nVExt>=nWExt)	//等倍描画のときには座標文字も描画
			DrawSheetFrameCharH(pDC,col,9,sizeOrg.cx,dwMode,nVExt,nWExt);
		if(rcClip.top<=0){			//上外枠線の描画
			pDC->MoveTo(rcOutside.left,rcOutside.top);
			pDC->LineTo(rcOutside.right,rcOutside.top);
		}
	}
	if(rcClip.right>=sizeOrg.cx-10){		//右内枠線の描画
		int a=0;
		if(width<=1) a=1;
		pDC->MoveTo(rcInside.right,rcInside.top);
		pDC->LineTo(rcInside.right,rcInside.bottom+a);		//終点を補う
		if(nVExt>=nWExt)	//等倍描画のときには座標文字も描画
			DrawSheetFrameCharV(pDC,col,sizeOrg.cx-8,sizeOrg.cy,dwMode,nVExt,nWExt);
		if(rcClip.right>=sizeOrg.cx-1){	//右外枠線の描画
			pDC->MoveTo(rcOutside.right,rcOutside.top);
			pDC->LineTo(rcOutside.right,rcOutside.bottom+a);//終点を補う
		}
	}
	if(rcClip.bottom>=sizeOrg.cy-10){		//下内枠線の描画
		pDC->MoveTo(rcInside.left,rcInside.bottom);
		pDC->LineTo(rcInside.right,rcInside.bottom);
		if(nVExt>=nWExt)	//等倍描画のときには座標文字も描画
			DrawSheetFrameCharH(pDC,col,sizeOrg.cy-2,sizeOrg.cx,dwMode,nVExt,nWExt);
		if(rcClip.bottom>=sizeOrg.cy-1){
			pDC->MoveTo(rcOutside.left,rcOutside.bottom);
			pDC->LineTo(rcOutside.right,rcOutside.bottom);
		}
	}
	pPen=pDC->SelectObject(pPen);	//ペンの復帰
	delete pPen;					//作成したペンの削除
}

void  CBSchView::DrawObj(					//図面要素を描画する
		CDC* pDC,							//描画先
		SXBSchObj* pObj,						//描画オブジェクト
		DWORD dwMode,						//描画モード
		int  nVExt,						//表示倍率(View)
		int  nWExt,						//表示倍率(Window)
		LPCRECT lprcClip)				//クリップ情報
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



//指定リストデータの描画
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


	//装飾系オブジェクトの描画
	XBSchObjListIterator pObjIte = pList->begin();
	while(pObjIte != pList->end()){
		SXBSchObj* pObj = *pObjIte;
		pObjIte++;
		if(pObj->deleteNum()<0 && pObj->keepOrder()){ //()!=SXBSchObj::ID_JUNCTION && pObj->id()!=SXBSchObj::ID_MARKER && pObj->id()!=SXBSchObj::ID_IMAGEOBJECT){
			if((dwOption & DRAW_TEMP_DATA)||(pObj->refObj() == NULL)){
				if(!(dwOption & DRAW_ALL_LAYER)){
					if(!(layerMask & SLayer::wLayerTable[pObj->layer()]) ){
						continue;//描画レイヤーでなければ描画しない
					}
				}

				if(pObj->qRedraw(rc)){
					DrawObj(pDC,pObj,dwMode,nVExt,nWExt,&rcClip);
				}
			}
		}
	}









	////イメージの描画
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
	//					continue;//描画レイヤーでなければ描画しない
	//				}
	//				if(pObj->qRedraw(rc)){
	//					DrawObj(pDC,pObj,dwMode,nVExt,nWExt,&rcClip);
	//				}
	//			}
	//		}
	//	}
	//}



	////マーカーラインの描画
	//XBSchObjListIterator pObjIte = pList->begin();
	//while(pObjIte != pList->end()){
	//	SXBSchObj* pObj = *pObjIte;
	//	pObjIte++;
	//	if(pObj->deleteNum()<0 && pObj->id()==SXBSchObj::ID_MARKER){
	//		if((dwOption & DRAW_TEMP_DATA)||(pObj->refObj() == NULL)){
	//			if(!(dwOption & DRAW_ALL_LAYER)){
	//				if(!(layerMask & wLayerTable[pObj->layer()]) ){
	//					continue;//描画レイヤーでなければ描画しない
	//				}
	//			}
	//			if(pObj->qRedraw(rc)){
	//				DrawObj(pDC,pObj,dwMode,nVExt,nWExt,&rcClip);
	//			}
	//		}
	//	}
	//}


	//NCマークの描画
	if(dwMode & DRAW_NC_MARK){
		XBSchObjListIterator pObjIte = pList->begin();
		while(pObjIte != pList->end()){
			SXBSchObj* pObj = *pObjIte;
			pObjIte++;
			//NCマークは一時データのときは描画しない
			if((pObj->deleteNum()>=0)||(dwOption & DRAW_TEMP_DATA)||(pObj->refObj() != NULL)){
				continue;
			}

			if(!(dwOption & DRAW_ALL_LAYER)){
				if(!(layerMask & SLayer::wLayerTable[pObj->layer()]) ){
					continue;//描画レイヤーでなければ描画しない
				}
			}

			if(pObj->id()==SXBSchObj::ID_COMPONENT){
				if(!pObj->qRedraw(rc)){
					//SXBSchComponentのqRedraw()はVer0.48現在常にtrueを返すので実は意味がない。
					continue;
				}
				SXBSchComponent* pComp = (SXBSchComponent*)pObj;
				int pincount = pComp->pinCount();
				int i;
				for(i=0;i<pincount;i++){
					if(pComp->pinNC(i)){
						SPoint pt = pComp->pinPosition(i);
						g_drawNcMark(	pDC,
										pt,						//位置
										dwMode,					//描画モード
										nVExt,					//表示倍率(View)
										nWExt,					//表示倍率(Window)
										&rcClip);				//クリップ情報
					}
				}
			}
		}
	}

	//部品の描画
	pObjIte = pList->begin();
	while(pObjIte != pList->end()){
		SXBSchObj* pObj = *pObjIte;
		pObjIte++;
		if(pObj->deleteNum()<0 && pObj->id()==SXBSchObj::ID_COMPONENT){
			if((dwOption & DRAW_TEMP_DATA)||(pObj->refObj() == NULL)){
				if(!(dwOption & DRAW_ALL_LAYER)){
					if(!(layerMask & SLayer::wLayerTable[pObj->layer()]) ){
						continue;//描画レイヤーでなければ描画しない
					}
				}

				if(pObj->qRedraw(rc)){
					DrawObj(pDC,pObj,dwMode,nVExt,nWExt,&rcClip);
				}
			}
		}
	}

	//バス、バスエントリーの描画
	pObjIte = pList->begin();
	while(pObjIte != pList->end()){
		SXBSchObj* pObj = *pObjIte;
		pObjIte++;
		if(pObj->deleteNum()<0 && (pObj->id()==SXBSchObj::ID_BUS||pObj->id()==SXBSchObj::ID_BENTRY)){
			if((dwOption & DRAW_TEMP_DATA)||(pObj->refObj() == NULL)){
				if(!(dwOption & DRAW_ALL_LAYER)){
					if(!(layerMask & SLayer::wLayerTable[pObj->layer()]) ){
						continue;//描画レイヤーでなければ描画しない
					}
				}

				if(pObj->qRedraw(rc)){
					DrawObj(pDC,pObj,dwMode,nVExt,nWExt,&rcClip);
				}
			}
		}
	}


	//ワイヤー、ワイヤーエントリー、ジャンクションの描画
	pObjIte = pList->begin();
	while(pObjIte != pList->end()){
		SXBSchObj* pObj = *pObjIte;
		pObjIte++;
		if(pObj->deleteNum()<0 && (pObj->id()==SXBSchObj::ID_WIRE||pObj->id()==SXBSchObj::ID_ENTRY||pObj->id()==SXBSchObj::ID_JUNCTION)){
			if((dwOption & DRAW_TEMP_DATA)||(pObj->refObj() == NULL)){
				if(!(dwOption & DRAW_ALL_LAYER)){
					if(!(layerMask & SLayer::wLayerTable[pObj->layer()]) ){
						continue;//描画レイヤーでなければ描画しない
					}
				}

				if(pObj->qRedraw(rc)){
					DrawObj(pDC,pObj,dwMode,nVExt,nWExt,&rcClip);
				}
			}
		}
	}

	//ラベル、タグの描画
	pObjIte = pList->begin();
	while(pObjIte != pList->end()){
		SXBSchObj* pObj = *pObjIte;
		pObjIte++;
		if(pObj->deleteNum()<0 && (pObj->id()==SXBSchObj::ID_LABEL||pObj->id()==SXBSchObj::ID_TAG)){
			if((dwOption & DRAW_TEMP_DATA)||(pObj->refObj() == NULL)){
				if(!(dwOption & DRAW_ALL_LAYER)){
					if(!(layerMask & SLayer::wLayerTable[pObj->layer()]) ){
						continue;//描画レイヤーでなければ描画しない
					}
				}

				if(pObj->qRedraw(rc)){
					DrawObj(pDC,pObj,dwMode,nVExt,nWExt,&rcClip);
				}
			}
		}
	}

}



//メインデータの描画
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

		//最初に編集レイヤー以外のレイヤーを描画する
		layerMask = wShowLayerOrg & ~editLayerMask;
		if((pMainData=pDoc->GetMainData(0))!=NULL){
			DrawListData(pDC,pMainData,dwMode | DRAW_DARK,nVExt,nWExt,rcClip,0,layerMask);
		}
		//後から編集レイヤーを描画する
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

//一時データの描画
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

	if((pTmpData=pDoc->GetTmpList())==NULL){		//一時データのリストを得る
//		TRACE("\nBSChView::DrawTmpData() #2");
		return;
	}
	if(!pTmpData->size()){
		m_bDrawTmpData=FALSE;
		return;
	}
	DrawListData(pDC,pTmpData,dwMode,nVExt,nWExt,rcClip,nDrawAllBit);	//一時データのリストの描画
	if(dwMode & DRAW_TEMP)m_bDrawTmpData=bDraw;
	else m_bDrawTmpData=FALSE;
}


//一時データの枠の描画
//次の場合は枠を描画しない
//   ・一時データが存在しない場合
//bDrawがTRUEのとき描画
//       FALSEのとき消去
void CBSchView::DrawTmpDataFrame(CDC* pDC,CBSchDoc* pDoc,BOOL bDraw/*=TRUE*/)
{
	RECT rc;
	if(bDraw==FALSE && m_bDrawTmpDataFrame==FALSE) return;
	if(!pDC || !pDoc) return;
	int nTmpCount=pDoc->GetTmpDataCount();

	if(nTmpCount<1){	//（条件１）
		m_bDrawTmpDataFrame=FALSE;//描画フラグもクリアしておく
		return;
	}
	//rc=pDoc->GetTmpRect();		//一時データの外周を得る
	//BSchPtoLP(&rc,m_nVExt,m_nWExt);				//図面座標を論理座標に変換
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
	nOldDrawMode=pDC->SetROP2(R2_COPYPEN);	//描画モードの設定

	bool bInhibitDrawOutline = false;	//選択範囲外枠描画の禁止フラグ 装飾ラインのEditPointsモードのとき禁止

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
					ppenOld=pDC->SelectObject(&penNew);			//作成したペンの選択

					src = ((SXBSchComponent*)(pObj))->refnumArea();
					rc.left = src.l();
					rc.top = src.t();
					rc.right = src.r();
					rc.bottom = src.b();
					BSchPtoLP(&rc,m_nVExt,m_nWExt);				//図面座標を論理座標に変換
					rc.right+=1; rc.bottom+=1;
					pDC->Rectangle(&rc);

					pDC->SelectObject(ppenOld);
					penNew.DeleteObject();
				}


				if(!((SXBSchComponent*)(pObj))->GetHideVal()){
					col = ((m_nFocusedTo == FOCUSED_TO_VAL)?m_COL.colTEMPFRAME_FOCUSED:m_COL.colTEMPFRAME_NOT_FOCUSED);
					penNew.CreatePen(PS_SOLID,0,col);
					ppenOld=pDC->SelectObject(&penNew);			//作成したペンの選択


					src = ((SXBSchComponent*)(pObj))->nameArea();
					rc.left = src.l();
					rc.top = src.t();
					rc.right = src.r();
					rc.bottom = src.b();
					BSchPtoLP(&rc,m_nVExt,m_nWExt);				//図面座標を論理座標に変換
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
					ppenOld=pDC->SelectObject(&penNew);			//作成したペンの選択
					if(selectFlag & SXBSchObj::SELECT_P1){
						int x = pDash->p1().x();
						int y = pDash->p1().y();
						CRect rc=CRect(x-2,y-2,x+2,y+2);
						BSchPtoLP(&rc,m_nVExt,m_nWExt);				//図面座標を論理座標に変換
						rc.left-=1; rc.top-=1; rc.right+=2; rc.bottom+=2;
						pDC->Rectangle(&rc);
					}
					if(selectFlag & SXBSchObj::SELECT_P2){
						int x = pDash->p2().x();
						int y = pDash->p2().y();
						CRect rc=CRect(x-2,y-2,x+2,y+2);
						BSchPtoLP(&rc,m_nVExt,m_nWExt);				//図面座標を論理座標に変換
						rc.left-=1; rc.top-=1; rc.right+=2; rc.bottom+=2;
						pDC->Rectangle(&rc);
					}

					if(pDash->curve()){
						if(selectFlag & SXBSchObj::SELECT_P1){
							int x = pDash->Ct1().x();
							int y = pDash->Ct1().y();
							CRect rc=CRect(x-2,y-2,x+2,y+2);
							BSchPtoLP(&rc,m_nVExt,m_nWExt);				//図面座標を論理座標に変換
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
							BSchPtoLP(&rc,m_nVExt,m_nWExt);				//図面座標を論理座標に変換
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

		//選択オブジェクト本体の選択フレームの描画
		col = ((!bSelectSingle || m_nFocusedTo == FOCUSED_TO_BODY)?m_COL.colTEMPFRAME_FOCUSED:m_COL.colTEMPFRAME_NOT_FOCUSED);
		penNew.CreatePen(PS_SOLID,0,col);
		ppenOld=pDC->SelectObject(&penNew);			//作成したペンの選択

		rc=pDoc->GetTmpRect();		//一時データの外周を得る
		BSchPtoLP(&rc,m_nVExt,m_nWExt);				//図面座標を論理座標に変換
		rc.left-=1; rc.top-=1; rc.right+=2; rc.bottom+=2;
		pDC->Rectangle(&rc);

		pDC->SelectObject(ppenOld);
		penNew.DeleteObject();
	}

	pDC->SetROP2(nOldDrawMode);		//描画モードの復帰
	pDC->SelectObject(pbrushOld);
	m_bDrawTmpDataFrame=bDraw;	
}
	

//選択中の点線枠の描画
//bDrawがTRUEのとき描画
//       FALSEのとき消去
void CBSchView::DrawSelectFrame(CDC* pDC,BOOL bDraw/*=TRUE*/)
{
	CRect rc;
	if(bDraw==FALSE && m_bDrawSelectFrame==FALSE) return;//消去指定ですでに消去済みなら何もしない
	rc=CRect(m_ptMoveFrom,m_ptMoveTo);
	rc.NormalizeRect();
	//g_MakeRECT(&m_ptMoveFrom,&m_ptMoveTo,&rc);	//点線枠の図面上の座標を得る
	BSchPtoLP(&rc,m_nVExt,m_nWExt);				//図面座標を論理座標に変換

	int nOldDrawMode;
	CPen penNew;
	CPen* ppenOld;
	CBrush brushNew;
	CBrush* pbrushOld;

	penNew.CreatePen(PS_SOLID,0,RGB(128,128,128));
	ppenOld=pDC->SelectObject(&penNew);			//作成したペンの選択
	pbrushOld = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
	nOldDrawMode=pDC->SetROP2(R2_COPYPEN);	//描画モードの設定
	pDC->Rectangle(&rc);
	pDC->SetROP2(nOldDrawMode);		//描画モードの復帰
	pDC->SelectObject(pbrushOld);
	pDC->SelectObject(ppenOld);

	//	pDC->DrawFocusRect(&rc);	//点線枠の描画

	m_bDrawSelectFrame=bDraw;	//トグルでない点に注意 1996/12/01
}

void CBSchView::DrawFindMark(CDC* pDC)
{
	if(!isFindMarkActive())return;

	int nOldDrawMode;
	CPen penNew;
	CPen* ppenOld;
	penNew.CreatePen(PS_SOLID,0,m_COL.colFIND_MARK);
	ppenOld=pDC->SelectObject(&penNew);			//作成したペンの選択
	nOldDrawMode=pDC->SetROP2(R2_COPYPEN);	//描画モードの設定

	CPoint ptBegin;
	CPoint ptEnd;

	ptBegin = CPoint(-10,m_ptFindMark.y);
	ptEnd   = CPoint(6000,m_ptFindMark.y);
	BSchPtoLP(&ptBegin,m_nVExt,m_nWExt);				//図面座標を論理座標に変換
	BSchPtoLP(&ptEnd,m_nVExt,m_nWExt);				//図面座標を論理座標に変換
	pDC->MoveTo(ptBegin);
	pDC->LineTo(ptEnd);

	ptBegin = CPoint(m_ptFindMark.x,-10);
	ptEnd   = CPoint(m_ptFindMark.x,5000);
	BSchPtoLP(&ptBegin,m_nVExt,m_nWExt);				//図面座標を論理座標に変換
	BSchPtoLP(&ptEnd,m_nVExt,m_nWExt);				//図面座標を論理座標に変換
	pDC->MoveTo(ptBegin);
	pDC->LineTo(ptEnd);


	pDC->SetROP2(nOldDrawMode);		//描画モードの復帰
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
	ppenOld=pDC->SelectObject(&penNew);			//作成したペンの選択
	nOldDrawMode=pDC->SetROP2(R2_XORPEN);	//描画モードの設定

	CPoint ptBegin;
	CPoint ptEnd;

	ptBegin = CPoint(-10,m_ptMouse.y);
	ptEnd   = CPoint(6000,m_ptMouse.y);
	BSchPtoLP(&ptBegin,m_nVExt,m_nWExt);				//図面座標を論理座標に変換
	BSchPtoLP(&ptEnd,m_nVExt,m_nWExt);				//図面座標を論理座標に変換
	pDC->MoveTo(ptBegin);
	pDC->LineTo(ptEnd);

	ptBegin = CPoint(m_ptMouse.x,-10);
	ptEnd   = CPoint(m_ptMouse.x,5000);
	BSchPtoLP(&ptBegin,m_nVExt,m_nWExt);				//図面座標を論理座標に変換
	BSchPtoLP(&ptEnd,m_nVExt,m_nWExt);				//図面座標を論理座標に変換
	pDC->MoveTo(ptBegin);
	pDC->LineTo(ptEnd);


	pDC->SetROP2(nOldDrawMode);		//描画モードの復帰
	pDC->SelectObject(ppenOld);


//	RECT rc;
//	rc=CRect(CPoint(-10,-10),m_ptMouse);
//	rc=CRect(-10,m_ptMouse.y,5000,4000);

//	BSchPtoLP(&rc);				//図面座標を論理座標に変換
//	pDC->DrawFocusRect(&rc);	//点線枠の描画
//	rc=CRect(m_ptMouse,CPoint(5000,3000));
//	rc=CRect(m_ptMouse.x,-10,5000,4000);

//	BSchPtoLP(&rc);				//図面座標を論理座標に変換
//	pDC->DrawFocusRect(&rc);	//点線枠の描画
//	m_bDrawCrossCursor=bDraw;
}



//臨時データの描画
//dwModeがDRAW_TEMPのとき、bDrawが有効。
//      bDrawがTRUEのとき描画
//             FALSEのとき消去
//dwModeがDRAW_OFFのとき、m_bDrawTmpDataをクリア(FALSEに)する。
void CBSchView::DrawOccasional(CDC* pDC,BOOL bDraw/*=TRUE*/)
{
	//XOR描画で、消去指定ですでに消去済みなら何もしない
//	if(bDraw==FALSE && m_bDrawOccasional==FALSE) return;
	if(!pDC) return;
	CPoint point;
	CPen* ppenNew;
	CPen* ppenOld;
	int nOldDrawMode;
	
	//水平・垂直線の描画
	if(m_nCurrentTool==ID_TOOL_WIRE || m_nCurrentTool==ID_TOOL_BUS){
		nOldDrawMode=pDC->SetROP2(R2_COPYPEN);	//描画モードの設定
		int nWidth=1;
		if(m_nCurrentTool==ID_TOOL_BUS) nWidth=3;
		nWidth=(nWidth*m_nVExt)/m_nWExt;
		//実線ペンの準備
		ppenNew=new CPen;
		ppenNew->CreatePen(PS_SOLID,nWidth,m_COL.colXOR);
		ppenOld=pDC->SelectObject(ppenNew);
		//始点へのMoveTo;
		point=m_ptMoveFrom;
		BSchPtoLP(&point,m_nVExt,m_nWExt);
		pDC->MoveTo(point);
		//中間の点座標を設定して線描画
		if(m_bWireHorizon1st){		//先に水平線
			point.x=m_ptMoveTo.x;
			point.y=m_ptMoveFrom.y;
		}else{
			point.x=m_ptMoveFrom.x;
			point.y=m_ptMoveTo.y;
		}
		BSchPtoLP(&point,m_nVExt,m_nWExt);
		pDC->LineTo(point);
		//終点への線描画;
		point=m_ptMoveTo;
		BSchPtoLP(&point,m_nVExt,m_nWExt);
		pDC->LineTo(point);
		pDC->SelectObject(ppenOld);
		delete ppenNew;	//実線ペンの消去
		pDC->SetROP2(nOldDrawMode);	//描画モードの復帰
//破線の描画
	}else if(m_nCurrentTool==ID_TOOL_DASH){
		//2010/05/22 装飾線の一時描画を実際のアトリビュートに従ったものに修正
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


		//nOldDrawMode=pDC->SetROP2(R2_COPYPEN);	//描画モードの設定
		////透過モードの設定
		//int nOldBkMode=pDC->SetBkMode(TRANSPARENT);
		////破線ペンの準備
		//ppenNew=new CPen;
		//ppenNew->CreatePen(PS_DOT,1,m_COL.colXOR);
		//ppenOld=pDC->SelectObject(ppenNew);
		////始点へのMoveTo;
		//point=m_ptMoveFrom;
		//BSchPtoLP(&point,m_nVExt,m_nWExt);
		//pDC->MoveTo(point);
		////終点への線描画;
		//point=m_ptMoveTo;
		//BSchPtoLP(&point,m_nVExt,m_nWExt);
		//pDC->LineTo(point);
		//pDC->SelectObject(ppenOld);
		//delete ppenNew;	//破線ペンの消去
		//pDC->SetBkMode(nOldBkMode);//透過モードからの復帰
		//pDC->SetROP2(nOldDrawMode);	//描画モードの復帰
//マーカーラインの描画
	}else if(m_nCurrentTool==ID_TOOL_MARKER){
		m_tempMarkerLine.setX1(m_ptMoveFrom.x);
		m_tempMarkerLine.setY1(m_ptMoveFrom.y);
		m_tempMarkerLine.setX2(m_ptMoveTo.x);
		m_tempMarkerLine.setY2(m_ptMoveTo.y);

		g_DrawMarkerLine(
			pDC,							//描画先
			&m_tempMarkerLine,				//描画オブジェクト
			DRAW_TEMP,					//描画モード
			m_nVExt,			//表示倍率(View)
			m_nWExt,			//表示倍率(Window)
			NULL);		//クリップ情報


	//エントリーの描画
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
	//接合点の描画
	}else if(m_nCurrentTool==ID_TOOL_JUNCTION){
		SXBSchJunc junction;
		junction.setP1(CPoint2SPoint(m_pointOccasional));
		g_DrawJunc(pDC,&junction,DRAW_TEMP,m_nVExt,m_nWExt);
	}
	m_bDrawOccasional=bDraw;
}
