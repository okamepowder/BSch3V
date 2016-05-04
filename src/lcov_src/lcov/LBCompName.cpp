/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/


// LBCompName.cpp : 実装ファイル
//

#include "stdafx.h"
#include "LCoV.h"
#include "LBCompName.h"
#include <list>
#include <string>
using namespace std;
#include "complib.h"
#include ".\lbcompname.h"
#include "MainFrm.h"


// SLBCompName

IMPLEMENT_DYNAMIC(SLBCompName, CListBox)
SLBCompName::SLBCompName()
{
	char* p=(char*)&m_lf;	//LOGFONT構造体の０イニシャライズ
	for(int n=0;n<sizeof(LOGFONT);n++) *p++=0;
	m_lf.lfHeight=-13;
	m_lf.lfWeight=FW_REGULAR;
	m_lf.lfCharSet=DEFAULT_CHARSET;
	m_lf.lfPitchAndFamily=0;
	_tcscpy(m_lf.lfFaceName,_T("MS UI Gothic"));
	m_drawCall = 0;
}

SLBCompName::~SLBCompName()
{
}


BEGIN_MESSAGE_MAP(SLBCompName, CListBox)
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



// SLBCompName メッセージ ハンドラ


void SLBCompName::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO :  指定された項目のサイズを決定するコードを追加してください。
	lpMeasureItemStruct->itemHeight = 15;
}

void SLBCompName::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO :  指定された項目を描画するためのコードを追加してください。
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	CRect rcItem = lpDrawItemStruct->rcItem;
	int index = lpDrawItemStruct->itemID;
	int selectItem = GetCurSel();

//	TRACE("SLBCompName::DrawItem (%d,%d,%d)\n", m_drawCall++,index,selectItem);
//	if(index == 2){
//		TRACE("****\n");
//	}

	
	COLORREF col;
	


	if(lpDrawItemStruct->itemAction & ODA_SELECT){
		this->Invalidate(FALSE);
//		return;
	}

	if((lpDrawItemStruct->itemState & ODS_FOCUS) && (lpDrawItemStruct->itemState & ODS_SELECTED)){
		col = ::GetSysColor(COLOR_HIGHLIGHT);
		//colText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	}else{
		col = ::GetSysColor(COLOR_WINDOW);
		//colText = ::GetSysColor(COLOR_WINDOWTEXT);
	}
	{
		CBrush brush;
		brush.CreateSolidBrush(col);
		dc.FillRect(&rcItem,&brush);
	}
	if(lpDrawItemStruct->itemState & ODS_SELECTED){
		dc.DrawFocusRect(&rcItem);
	}
	if(index != -1){
//		SCompIndex* pCompIndex =(SCompIndex*)lpDrawItemStruct->itemData;
		CRect rcDraw = CRect(rcItem);

		SCompIndex* pCompIndex =(SCompIndex*)GetItemData(index);
		//if(lpDrawItemStruct->itemState & ODS_FOCUS){
		if((lpDrawItemStruct->itemState & ODS_FOCUS) && (lpDrawItemStruct->itemState & ODS_SELECTED)){
			col = ::GetSysColor(COLOR_HIGHLIGHTTEXT);	
		}else if(lpDrawItemStruct->itemState & ODS_SELECTED){
			col = ::GetSysColor(COLOR_WINDOWTEXT);
		}else{
			SCompIndex* pCompIndexSelected =(SCompIndex*)GetItemData(selectItem); 
			if((selectItem != -1) && (pCompIndexSelected->compInfo()==pCompIndex->compInfo())){
				col = RGB(255,0,0);
				rcDraw.left += 10;
			}
			else{
				col = ::GetSysColor(COLOR_WINDOWTEXT);
			}
		}
		CFont font;
		font.CreateFontIndirect(&m_lf);
		CFont* pOldFont = dc.SelectObject(&font);
		COLORREF colOldText	= dc.SetTextColor(col);			//TextColorの設定
		int nOldBkMode  = dc.SetBkMode(TRANSPARENT);			//描画は透過モード
		dc.DrawText(pCompIndex->name(),&rcDraw,DT_LEFT|DT_NOPREFIX);
		dc.SetTextColor(colOldText);
		dc.SelectObject(pOldFont);
		dc.SetBkMode(nOldBkMode);
	}
	dc.Detach();
}

BOOL SLBCompName::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if(! CListBox::PreCreateWindow(cs)) return FALSE;
	cs.style &= ~(LBS_OWNERDRAWVARIABLE | LBS_SORT);
	cs.style |= LBS_OWNERDRAWFIXED;
	return TRUE;
}

void SLBCompName::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO : ここにメッセージ ハンドラ コードを追加します。
	TRACE("SLBCompName::OnContextMenu()\n");
	CMenu menu;
	menu.LoadMenu(IDR_COMPNAMECONTEXT);
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,
		                               point.x,point.y,AfxGetMainWnd());
}

void SLBCompName::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	CListBox::OnLButtonDblClk(nFlags, point);
	((CMainFrame*)AfxGetMainWnd())->OnEditPropertyComp();
}
