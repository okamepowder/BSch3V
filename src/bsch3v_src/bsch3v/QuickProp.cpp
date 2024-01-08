// QuickProp.cpp : 実装ファイル
//

#include "stdafx.h"
#include "QuickProp.h"


// CQuickProp

IMPLEMENT_DYNAMIC(CQuickProp, CWnd)

CQuickProp::CQuickProp()
{
	m_pView = NULL;

}

CQuickProp::~CQuickProp()
{
}


BEGIN_MESSAGE_MAP(CQuickProp, CWnd)
ON_WM_PAINT()
ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



// CQuickProp メッセージ ハンドラ



void CQuickProp::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ここにメッセージ ハンドラ コードを追加します。
	// 描画メッセージで CWnd::OnPaint() を呼び出さないでください。


	CRect rc;
	this->GetClientRect(&rc);

	dc.FillSolidRect(0,0,rc.right-1,rc.bottom-1,m_COL_BG);

	CPen pen;

	pen.CreatePen(PS_SOLID,1,RGB(0,0,0));
	CPen *pOldPen= dc.SelectObject(&pen);
	dc.MoveTo(0,0);
	dc.LineTo(0,rc.bottom-2);
	dc.LineTo(rc.right-2,rc.bottom-2);
	dc.LineTo(rc.right-2,0);
	dc.LineTo(0,0);

	dc.MoveTo(1,rc.bottom-1);
	dc.LineTo(rc.right-1,rc.bottom-1);
	dc.LineTo(rc.right-1,1);

	dc.SelectObject(pOldPen);
	pen.DeleteObject();




	if(m_str.GetLength()==0)return;


	CFont*	pFont=new CFont;
	if(m_lf.lfHeight>0) m_lf.lfHeight = -m_lf.lfHeight;
	m_lf.lfCharSet=DEFAULT_CHARSET;
	
	pFont->CreateFontIndirect(&m_lf);	//フォントクラスの初期化
	CFont* pOldFont=dc.SelectObject(pFont);					//フォントの選択

	rc = CRect(5,5,100000,100000);
	UINT nFormat =DT_NOPREFIX|DT_TOP|DT_LEFT|DT_WORDBREAK;
	dc.DrawText(m_str,m_str.GetLength(),&rc,nFormat);

	dc.SelectObject(pOldFont);
	delete pFont;

}



void CQuickProp::OnMouseMove(UINT nFlags, CPoint point)
{
	this->ClientToScreen(&point);
	m_pView->ScreenToClient(&point);
	m_pView->SendMessage(WM_MOUSEMOVE,nFlags,MAKELPARAM(point.x,point.y));
}



CSize CQuickProp::GetDrawSize()
{
	CDC *pDC = this->GetDC();

	CFont*	pFont=new CFont;
	if(m_lf.lfHeight>0) m_lf.lfHeight = -m_lf.lfHeight;
	m_lf.lfCharSet=DEFAULT_CHARSET;
	pFont->CreateFontIndirect(&m_lf);	//フォントクラスの初期化
	CFont* pOldFont=pDC->SelectObject(pFont);					//フォントの選択

	CRect rc(0,0,100000,100000);
	UINT nFormat =DT_NOPREFIX|DT_TOP|DT_LEFT|DT_WORDBREAK|DT_CALCRECT;
	pDC->DrawText(m_str,m_str.GetLength(),&rc,nFormat);	//描画しない。

	pDC->SelectObject(pOldFont);
	delete pFont;

	ReleaseDC(pDC);
	return CSize(rc.right+10,rc.bottom+10);
}