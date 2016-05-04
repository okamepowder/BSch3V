/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// Preview.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <assert.h>
#include <string>
#include <list>
using namespace std;
#include "bsch.h"
#include "complib.h"
//#include "BSchObj.h"
//#include "BSchPart.h"

#include "xbschcomponent.h"
#include "BSchDoc.h"
#include "BSchView.h"
#include "Preview.h"

#include "drawobj.h"
#include "coord2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreview

CPreview::CPreview()
{
	m_pPartIndex=NULL;
}

CPreview::~CPreview()
{
}


BEGIN_MESSAGE_MAP(CPreview, CWnd)
	//{{AFX_MSG_MAP(CPreview)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////
//プレビューする部品のライブラリ情報のインデックスを設定
//    部品のサイズに応じたスクロールバーの設定をしてからInvalidate()して再描画
void CPreview::SetPartIndex(const SCompIndex* pPartIndex)
{
	if(!pPartIndex)
	{
		m_pPartIndex=NULL;
		return;
	}
	m_pPartIndex=pPartIndex;
	m_sizePart=SSize2CSize(m_pPartIndex->size());
	TRACE("%d,%d\n",m_sizePart.cx,m_sizePart.cy);
	CRect rcClient;
	GetClientRect(&rcClient);
	SendMessage(WM_SIZE,0,MAKELONG(rcClient.right,rcClient.bottom));
/*
	//垂直スクロールバーの設定
	int nMax;
	nMax=(m_sizePart.cy*10+80)-rcClient.bottom;
	if(nMax<0) nMax=0;
	SetScrollPos(SB_VERT,0,FALSE);
	SetScrollRange(SB_VERT,0,nMax);
	//水平スクロールバー
	nMax=(m_sizePart.cx*10+80)-rcClient.right;
	if(nMax<0) nMax=0;
	SetScrollPos(SB_HORZ,0,FALSE);
	SetScrollRange(SB_HORZ,0,nMax);


	Invalidate();
*/
}


/////////////////////////////////////////////////////////////////////////////
// CPreview メッセージ ハンドラ

void CPreview::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	TRACE("OnPaint\n");

	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	if(!m_pPartIndex) return;
	CRect rcArea;
	GetClientRect(&rcArea);

	dc.FillSolidRect(&rcArea,CBSchView::m_COL.colBG);

	int nXoffset,nYoffset;
	nXoffset=GetScrollPos(SB_HORZ);
	nYoffset=GetScrollPos(SB_VERT);
	SXBSchComponent part;
	SPoint	        point;
	part.setCompInfoIndex(m_pPartIndex);
	part.setName(m_pPartIndex->name());
	part.setRefnum(m_pPartIndex->number());
	point.setX(40+m_sizePart.cx*10-nXoffset);
	point.setY(40+m_sizePart.cy*10-nYoffset);
	part.setP1(point);
	g_DrawComponent(&dc,&part,DRAW_ON,1);
	// 描画用メッセージとして CWnd::OnPaint() を呼び出してはいけません
}

void CPreview::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
		int nCurrentPos,nNewPos;
	int nMin,nMax;
	BOOL bChange=FALSE;
	nCurrentPos=GetScrollPos(SB_HORZ);
	GetScrollRange(SB_HORZ,&nMin,&nMax);
	switch(nSBCode){
	case SB_THUMBPOSITION:
		SetScrollPos(SB_HORZ,nPos);
		bChange=TRUE;
		break;
	case SB_LINEUP:
		nNewPos=nCurrentPos-10;
		if(nNewPos<nMin) nNewPos=nMin;
		SetScrollPos(SB_HORZ,nNewPos);
		if(nNewPos!=nCurrentPos) bChange=TRUE;
		break;
	case SB_LINEDOWN:
		nNewPos=nCurrentPos+10;
		if(nNewPos>nMax) nNewPos=nMax;
		SetScrollPos(SB_HORZ,nNewPos);
		if(nNewPos!=nCurrentPos) bChange=TRUE;
		break;
	case SB_PAGEUP:
		nNewPos=nCurrentPos-50;
		if(nNewPos<nMin) nNewPos=nMin;
		SetScrollPos(SB_HORZ,nNewPos);
		if(nNewPos!=nCurrentPos) bChange=TRUE;
		break;
	case SB_PAGEDOWN:
		nNewPos=nCurrentPos+50;
		if(nNewPos>nMax) nNewPos=nMax;
		SetScrollPos(SB_HORZ,nNewPos);
		if(nNewPos!=nCurrentPos) bChange=TRUE;
		break;
	}
	if(bChange)Invalidate();

	//CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPreview::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	int nCurrentPos,nNewPos;
	int nMin,nMax;
	BOOL bChange=FALSE;
	nCurrentPos=GetScrollPos(SB_VERT);
	GetScrollRange(SB_VERT,&nMin,&nMax);
	switch(nSBCode){
	case SB_THUMBPOSITION:
		SetScrollPos(SB_VERT,nPos);
		bChange=TRUE;
		break;
	case SB_LINEUP:
		nNewPos=nCurrentPos-10;
		if(nNewPos<nMin) nNewPos=nMin;
		SetScrollPos(SB_VERT,nNewPos);
		if(nNewPos!=nCurrentPos) bChange=TRUE;
		break;
	case SB_LINEDOWN:
		nNewPos=nCurrentPos+10;
		if(nNewPos>nMax) nNewPos=nMax;
		SetScrollPos(SB_VERT,nNewPos);
		if(nNewPos!=nCurrentPos) bChange=TRUE;
		break;
	case SB_PAGEUP:
		nNewPos=nCurrentPos-50;
		if(nNewPos<nMin) nNewPos=nMin;
		SetScrollPos(SB_VERT,nNewPos);
		if(nNewPos!=nCurrentPos) bChange=TRUE;
		break;
	case SB_PAGEDOWN:
		nNewPos=nCurrentPos+50;
		if(nNewPos>nMax) nNewPos=nMax;
		SetScrollPos(SB_VERT,nNewPos);
		if(nNewPos!=nCurrentPos) bChange=TRUE;
		break;
	}
	if(bChange)Invalidate();
//	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

//////////////////////////////////////////////////////////////////
//OnSize()でスクロールバーの設定を行う
//スクロールバーのON/OFFでウインドウサイズは変わる
void CPreview::OnSize(UINT nType, int cx, int cy) 
{
	static int ncx=cx;
	static int ncy=cy;
	//CWnd::OnSize(nType, cx, cy);

	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	if(!m_pPartIndex) return; //部品未設定
	//垂直スクロールバーの設定
	int nMax;
	nMax=(m_sizePart.cy*10+80)-ncy;
	if(nMax<0) nMax=0;
	SetScrollPos(SB_VERT,0,FALSE);
	SetScrollRange(SB_VERT,0,nMax);

	//水平スクロールバー
	nMax=(m_sizePart.cx*10+80)-ncx;
	if(nMax<0) nMax=0;
	SetScrollPos(SB_HORZ,0,FALSE);
	SetScrollRange(SB_HORZ,0,nMax);
	Invalidate();
}	
