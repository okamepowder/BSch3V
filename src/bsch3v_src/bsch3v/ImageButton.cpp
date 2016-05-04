// ImageButton.cpp : 実装ファイル
//

#include "stdafx.h"
#include "resource.h"
#include "ImageButton.h"



// CImageButton

IMPLEMENT_DYNAMIC(CImageButton, CButton)

CImageButton::CImageButton()
{
	TRACE("CImageButton::CImageButton()\n");
	m_nCheck = 0;
	m_pImageList = NULL;
	m_nImageIndex = -1;
}

CImageButton::~CImageButton()
{
}


BEGIN_MESSAGE_MAP(CImageButton, CButton)
END_MESSAGE_MAP()



// CImageButton メッセージ ハンドラ



void CImageButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CDC* pDC;
    CRect rc;
    UINT nState;
    CString strText;

    pDC = CDC::FromHandle(lpDrawItemStruct->hDC);	
    rc = lpDrawItemStruct->rcItem;              
    nState = lpDrawItemStruct->itemState;         
	
	int cxEdge = GetSystemMetrics(SM_CXEDGE);
	int cyEdge = GetSystemMetrics(SM_CYEDGE);


	if((nState & ODS_SELECTED) != 0){
        pDC->DrawFrameControl(rc, DFC_BUTTON, DFCS_BUTTONPUSH | DFCS_PUSHED | DFCS_FLAT);	//3Dボタンの方が良いときは、| DFCS_FLAT を取り除く
        rc.DeflateRect(cxEdge,cyEdge);
        pDC->FillSolidRect(rc, RGB(0xC0,0xC0,0xC0));
    }
    else{
        pDC -> DrawFrameControl(rc, DFC_BUTTON, DFCS_BUTTONPUSH | DFCS_FLAT); //3Dボタンの方が良いときは、| DFCS_FLAT を取り除く
        rc.DeflateRect(cxEdge,cyEdge);
        pDC -> FillSolidRect(rc, RGB(0xD0,0xD0,0xD0));
    }

	int x = rc.top+(rc.Width()-16)/2;
	int y = rc.left+(rc.Height()-16)/2;

	if(m_pImageList!=NULL){
		m_pImageList->Draw(pDC,m_nImageIndex,CPoint(x,y),ILD_NORMAL);
	}

	if(m_nCheck != 0){
		LOGBRUSH logBrush;
		logBrush.lbStyle = BS_SOLID;
		logBrush.lbColor =	RGB(255,0,0);

		CPen pen;
		pen.CreatePen(PS_GEOMETRIC|PS_ENDCAP_SQUARE,2,&logBrush);
		CPen* pPenOld = pDC->SelectObject(&pen);
		int mx = x+10;
		int my = y+10;

		pDC->MoveTo(mx,my);
		pDC->LineTo(mx+6,my+6);
		pDC->MoveTo(mx,my+6);
		pDC->LineTo(mx+6,my);
		pDC->SelectObject(pPenOld);
	}
}


void CImageButton::SetCheck(int nCheck)
{
	m_nCheck = nCheck;
	if(GetSafeHwnd()!=0){
		Invalidate();
	}
}

void CImageButton::SetImage(CImageList* pImageList,int index)
{
	m_pImageList = pImageList;
	m_nImageIndex = index;
}
