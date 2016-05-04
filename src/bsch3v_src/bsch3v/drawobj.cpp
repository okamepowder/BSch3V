/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#include "stdafx.h"
#include <stdlib.h> //abs()のためにインクルード 
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <list>
#include <string>
using namespace std;

#include "BSch.h"

#include "SetLib.h"
#include "SPtnObj.h"
#include "SPtnObjArc.h"
#include "SPtnObjCircle.h"
#include "SPtnObjLine.h"
#include "SPtnObjPolygon.h"

//#include "BSchObj.h"
//#include "BSchCmnt.h"
//#include "BSchLine.h"
//#include "BSchBus.h"
//#include "BSchDash.h"
//#include "BSchWire.h"
//#include "BSchJunc.h"
//#include "BSchEntr.h"

#include "Pattern.h"

#include "xbschdocsch.h"
#include "BSchDoc.h"
#include "BSchView.h"
#include "drawobj.h"

#include "imagedata.h"

#include "complib.h"
#include "BSchFont.h"
#include "Global.h"

#include "coord2.h"
#include <atlimage.h>

#define M_PI       3.14159265358979323846

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//bool g_bIsWin9x;
bool g_bFillEndOfPtnLine;//倍率1においてパターン描画の水平、垂直描画直線の終端を補う。

static int PinDataNormal[4][4]={
		{	//L
			-10,		0,
			  0,		0
		},
		{	//T
			0,		-10,
			0,		0
		},
		{	//R
			10,		0,
			0,	0
		},
		{	//B
			0,		10,
			0,		0
		}
	};

#define CLK_DRAW_SIZE 4
static int PinDataClcok[4][6]={
		{	//L
			0,						CLK_DRAW_SIZE,
			CLK_DRAW_SIZE,			0,
			0,						-CLK_DRAW_SIZE
		},
		{	//T
			-CLK_DRAW_SIZE,			0,
			0,						CLK_DRAW_SIZE,
			CLK_DRAW_SIZE,			0
		},
		{	//R
			0,						CLK_DRAW_SIZE,
			-CLK_DRAW_SIZE,			0,
			0,						-CLK_DRAW_SIZE
		},
		{	//B
			-CLK_DRAW_SIZE,			0,
			0,						-CLK_DRAW_SIZE,
			CLK_DRAW_SIZE,			0
		}
	};

#define INVCIR_SIZE 3
static int PinDataInvert[4][8]={
		{	//L
			-10,					0,
			-INVCIR_SIZE*2,			0,
			-INVCIR_SIZE*2,			-INVCIR_SIZE,
			0,					INVCIR_SIZE,
		},
		{	//T
			0,					-10,
			0,					-INVCIR_SIZE*2,
			-INVCIR_SIZE,		-INVCIR_SIZE*2,
			INVCIR_SIZE,		0
		},
		{	//R
			10,					0,
			INVCIR_SIZE*2,		0,
			0,				-INVCIR_SIZE,
			INVCIR_SIZE*2,INVCIR_SIZE,
		},
		{	//B
			0,					10,
			0,					INVCIR_SIZE*2,
			-INVCIR_SIZE,		0,
			INVCIR_SIZE,		INVCIR_SIZE*2
		}
	};

//static int PinDataNormal[4][4]={
//		{	//L
//			0,		0,
//			10,		0
//		},
//		{	//T
//			0,		0,
//			0,		10
//		},
//		{	//R
//			0,		0,
//			-10,	0
//		},
//		{	//B
//			0,		0,
//			0,		-10
//		}
//	};
//
//#define CLK_DRAW_SIZE 4
//static int PinDataClcok[4][6]={
//		{	//L
//			10,						CLK_DRAW_SIZE,
//			10+CLK_DRAW_SIZE,		0,
//			10,						-CLK_DRAW_SIZE
//		},
//		{	//T
//			-CLK_DRAW_SIZE,			10,
//			0,						10+CLK_DRAW_SIZE,
//			CLK_DRAW_SIZE,			10
//		},
//		{	//R
//			-10,					CLK_DRAW_SIZE,
//			-(10+CLK_DRAW_SIZE),	0,
//			-10,					-CLK_DRAW_SIZE
//		},
//		{	//B
//			-CLK_DRAW_SIZE,			-10,
//			0,						-(10+CLK_DRAW_SIZE),
//			CLK_DRAW_SIZE,			-10
//		}
//	};
//
//#define INVCIR_SIZE 3
//static int PinDataInvert[4][8]={
//		{	//L
//			0,					0,
//			10-INVCIR_SIZE*2,	0,
//			10-INVCIR_SIZE*2,	-INVCIR_SIZE,
//			10,					INVCIR_SIZE,
//		},
//		{	//T
//			0,					0,
//			0,					10-INVCIR_SIZE*2,
//			-INVCIR_SIZE,		10-INVCIR_SIZE*2,
//			INVCIR_SIZE,		10
//		},
//		{	//R
//			0,					0,
//			-(10-INVCIR_SIZE*2),0,
//			-10,				-INVCIR_SIZE,
//			-(10-INVCIR_SIZE*2),INVCIR_SIZE,
//		},
//		{	//B
//			0,					0,
//			0,					-(10-INVCIR_SIZE*2),
//			-INVCIR_SIZE,		-10,
//			INVCIR_SIZE,		-(10-INVCIR_SIZE*2)
//		}
//	};


void  DotLine9x(CDC* pDC,int x0,int y0,int x1,int y1,int width,bool& rbMark,int& rPos)
{
	int x=x0;
	int y=y0;
	int n;
	int xd,yd;
	int movePitch;
	double incline;
	if(width<1)width=1;
	int pitch = width*3;
	if(rPos>pitch)rPos=0;

	if(x0==x1 && y0==y1)return;

	pDC->MoveTo(x0,y0);
	if(abs(x0-x1)>abs(y0-y1)){
		xd=abs(x0-x1);	//abs of deleta X
		yd=y1-y0;		//deleta Y
		incline=(double)yd/xd;
		movePitch =(int) sqrt(pitch*pitch/(1+incline*incline));
//TRACE("%d,%d,%f\n",pitch,movePitch,incline);

		if(rPos) n=movePitch*rPos/pitch;
		else n=movePitch;
		while(1){
			if(n>=xd){
				rPos=n-xd;
				x=x1;
				y=y1;
			}else{
				if(x0<x1){
					x=x0+n;
				}else{
					x=x0-n;
				}
				y=y0+yd*n/xd;
			}
			if(rbMark){
				pDC->LineTo(x,y);
			}else{
				pDC->MoveTo(x,y);
			}
			if(n>xd)break;
			rbMark=!rbMark;
			if(x==x1)break;
			n+=movePitch;
		}
	}else{
		yd=abs(y0-y1);	//abs of deleta Y
		xd=x1-x0;		//deleta X
		incline=(double)xd/yd;
		movePitch =(int) sqrt(pitch*pitch/(1+incline*incline));

		if(rPos) n=movePitch*rPos/pitch;
		else n=movePitch;

//TRACE("%d,%d,%d,%f\n",pitch,movePitch,n,incline);

		while(1){
			if(n>=yd){
				rPos=n-yd;
				x=x1;
				y=y1;
			}else{
				if(y0<y1){
					y=y0+n;
				}else{
					y=y0-n;
				}
				x=x0+xd*n/yd;
			}
			if(rbMark){
				pDC->LineTo(x,y);
			}else{
				pDC->MoveTo(x,y);
			}
			if(n>yd)break;
			rbMark=!rbMark;
			if(y==y1)break;
			n+=movePitch;
		}
	}
	rPos=pitch*rPos/movePitch;
//TRACE("     %d,%d\n",rbMark,rPos);
}

void DotPolygon9x(CDC* pDC,LPPOINT pPoint,int node,int width)
{
	if(node<2)return;
	bool bMark=true;
	int nPos=0;
	int i;
//TRACE("DotPolygon9x\n");
	for(i=0;i<node-1;i++){
		DotLine9x(pDC,	pPoint[i].x,   pPoint[i].y,
						pPoint[i+1].x, pPoint[i+1].y,
						width,bMark,nPos);
	}
	DotLine9x(pDC,	pPoint[node-1].x,   pPoint[node-1].y,
					pPoint[0].x, pPoint[0].y,
					width,bMark,nPos);
}



void DotArc9x(CDC* pDC,CRect* prc,CPoint ptBegin, CPoint ptEnd,int nWidth)
{
	double centerX,centerY;
	double rv;
	double hm;
	double dBegin,dEnd,a;
	int x0,y0,x1,y1;
	bool bMark=true;
	int nPos=0;
	double width=prc->Width();
	double height=prc->Height();

	centerX=(double)(prc->left+prc->right)/2;
	centerY=(double)(prc->top +prc->bottom)/2;
	rv=(double)(prc->bottom)-centerY;
	
	if(height==0)height=1;
	if(width==0)width=1;
	hm=width/height;
	if(ptBegin==ptEnd){
		dBegin=0;
		dEnd=2*M_PI;
	}else{
		dBegin = atan2(centerY-(double)ptBegin.y,((double)ptBegin.x-centerX)/hm);
		dEnd = atan2(centerY-(double)ptEnd.y,((double)ptEnd.x-centerX)/hm);
		if(dEnd<dBegin)dEnd+=2*M_PI;
	}
//TRACE("begin %f,end %f\n",dBegin,dEnd);
	a=dBegin;
	x0=(int)(centerX+rv*cos(a)*hm);
	y0=(int)(centerY-rv*sin(a));
	while(1){
		//TRACE("%d,%d,%f,%f,%f\n",x0,y0,a,cos(a),sin(a));
		a+=(2*M_PI)/32;
		if(a>=dEnd){
			a=dEnd;
		}
		x1=(int)(centerX+rv*cos(a)*hm);
		y1=(int)(centerY-rv*sin(a));
		//TRACE("%d,%d\n",bMark,nPos);
		DotLine9x(pDC,x0,y0,x1,y1,nWidth,bMark,nPos);
		if(a==dEnd)break;
		x0=x1;
		y0=y1;
	}
}



void convDirXY(int&rx,int&ry,int dir,int cx,int cy)
{
	int x,y,w;
	switch(dir&3){
		case 0:
			x= rx;
			y= ry;
			w= cx;
			break;
		case 1:
			x=-ry+cy;
			y= rx;
			w= cy;
			break;
		case 2:
			x=-rx+cx;
			y=-ry+cy;
			w= cx;
			break;
		case 3:
			x= ry;
			y=-rx+cx;
			w= cy;
			break;
	}
	if(dir&4) x=-x+w;

	rx= x;
	ry= y;
};

void g_DrawPtnText(
			CDC* pDC,				//描画
			SPtnObjText* pObj,		//描画オブジェクト
			COLORREF col,			//描画色
			int	 dir,
			int	 cx,				//部品のXサイズ
			int  cy,				//部品のYサイズ
			int	 x0,				//部品の左端座標
			int  y0,				//部品の上座標
			int  nVExt,				//表示倍率(View)
			int  nWExt)				//表示倍率(Window)
{

	int xp,yp;
	xp=pObj->m_p0.x();	yp=pObj->m_p0.y();
	convDirXY(xp,yp,dir,cx,cy);
	xp+=x0;  yp+=y0;
	//xp = (xp*nVExt)/nWExt;
	//yp = (yp*nVExt)/nWExt;
	bool vflag = !(pObj->m_bHoriz);
	int align = pObj->m_align;
	if((align&2)==0){
		if(vflag){
			if(dir&2){
				align ^= 1;
			}
		}else{
			if((dir&3)==1 || (dir&3)==2){
				align ^= 1;
			}
		}
	}
	if((align&8)==0){
		if(vflag){
			if(dir&2){
				align ^= 4;
			}
		}else{
			if((dir&3)==1 || (dir&3)==2){
				align ^= 4;
			}
		}
	}

	if(dir&1){
		vflag = !vflag;
	}
	if(dir&4){
		if(!vflag && (align&2)==0){
			align ^= 1;
		}
		if(vflag && (align&8)==0){
			align ^= 4;
		}

	}
	CSize size;
	if(pObj->m_fontName.length()==0){
		size=g_pFont->PutFont(pDC,xp,yp,
								pObj->m_strText.c_str(),-1,col,DRAW_ON,
								vflag,align,nVExt,nWExt,FALSE);
	}else{
		LOGFONT lf;
		char* p=(char*)&lf;	//LOGFONT構造体の０イニシャライズ
		for(int n=0;n<sizeof(LOGFONT);n++) *p++=0;
		lf.lfHeight=-(pObj->m_fontSize);
		lf.lfWeight=(pObj->m_fontBold ? FW_BOLD : FW_REGULAR);
		lf.lfItalic=pObj->m_fontItalic;
		lf.lfCharSet=DEFAULT_CHARSET;
		lf.lfPitchAndFamily=0;
		_tcsncpy(lf.lfFaceName,pObj->m_fontName.c_str(),LF_FACESIZE);
		lf.lfFaceName[LF_FACESIZE-1]='\0';

		size=g_pFont->PutWinFont(
				pDC,
				xp,yp,
				pObj->m_strText.c_str(),
				-1,
				&lf,
				col,DRAW_ON,
				vflag,align,
				nVExt,nWExt);


	}
	int w = size.cx * nWExt/ nVExt;
	int h = size.cy * nWExt/ nVExt;
	pObj->setDrawSize(SSize(w,h));

}


//パターンのLineの描画
void g_DrawPtnLine(
			CDC* pDC,				//描画
			SPtnObjLine* pPtnObj,	//描画オブジェクト
			COLORREF col,			//描画色
			int	 dir,
			int	 cx,
			int  cy,
			int	 x0,
			int  y0,
			int  nVExt,				//表示倍率(View)
			int  nWExt)				//表示倍率(Window)
{
	CPen newPen;
	CPen* pPenOld;
	int x1,y1;
	int xs,ys;
	bool b9xDot=false;

	int width = (pPtnObj->m_width * nVExt)/nWExt;
	int style = pPtnObj->m_style;

	if(style == 0){
		newPen.CreatePen(PS_SOLID,width,col);
	}else{	//DOT LINE
		if(width>1){
			//if(::g_bIsWin9x){
			//	newPen.CreatePen(PS_SOLID,width,col);
			//	b9xDot=true;
			//}else
			{
				DWORD adwPenStyle[2];
				adwPenStyle[0]=adwPenStyle[1]=width*2;
				LOGBRUSH logBrush;
				logBrush.lbStyle = BS_SOLID;
				logBrush.lbColor = col;
				newPen.CreatePen(PS_USERSTYLE|PS_GEOMETRIC|PS_ENDCAP_ROUND, width, &logBrush,2,adwPenStyle);
			}
		}else{
			newPen.CreatePen(PS_DOT,width,col);
		}
	}
	pPenOld = pDC->SelectObject(&newPen);

	xs=pPtnObj->GetP0().x();	ys=pPtnObj->GetP0().y();
	convDirXY(xs,ys,dir,cx,cy);
	xs+=x0;  ys+=y0;
	xs = (xs*nVExt)/nWExt;
	ys = (ys*nVExt)/nWExt;

	x1=pPtnObj->GetP1().x();	y1=pPtnObj->GetP1().y();
	convDirXY(x1,y1,dir,cx,cy);
	x1+=x0;  y1+=y0;
	x1 = (x1*nVExt)/nWExt;
	y1 = (y1*nVExt)/nWExt;

	if(g_bFillEndOfPtnLine){	// 2007/06/10
		if((nVExt==nWExt)&&(width<=1)){
			if(xs==x1){//垂直線
				if(ys<y1) y1++;
				else if(ys>y1) y1--;
			}else if(ys==y1){//水平線
				if(xs<x1) x1++;
				else if(xs>x1) x1--;
			}
		}
	}

	//if(b9xDot){
	//	bool bMark=true;
	//	int nPos=0;
	//	DotLine9x(pDC,xs,ys,x1,y1,width,bMark,nPos);
	//}else{
	if(!pPtnObj->getCurve()){
		int nOldBkMmode=pDC->SetBkMode(TRANSPARENT);
		pDC->MoveTo(xs,ys);
		pDC->LineTo(x1,y1);
		pDC->SetBkMode(nOldBkMmode);
	}else{
		CPoint pointArray[4];
		pointArray[0] = CPoint(xs,ys);
		pointArray[3] = CPoint(x1,y1);

		xs=pPtnObj->GetPC0().x();	ys=pPtnObj->GetPC0().y();
		convDirXY(xs,ys,dir,cx,cy);
		xs+=x0;  ys+=y0;
		xs = (xs*nVExt)/nWExt;
		ys = (ys*nVExt)/nWExt;
		pointArray[1] = CPoint(xs,ys);

		xs=pPtnObj->GetPC1().x();	ys=pPtnObj->GetPC1().y();
		convDirXY(xs,ys,dir,cx,cy);
		xs+=x0;  ys+=y0;
		xs = (xs*nVExt)/nWExt;
		ys = (ys*nVExt)/nWExt;
		pointArray[2] =  CPoint(xs,ys);


		pDC->PolyBezier(pointArray,4);
	}
	pDC->SelectObject(pPenOld);
}

void g_DrawPtnCircle(
			CDC* pDC,				//描画
			SPtnObjCircle* pPtnObj,	//描画オブジェクト
			COLORREF col,			//描画色
			int	 dir,
			int	 cx,
			int  cy,
			int	 x0,
			int  y0,
			int  nVExt,				//表示倍率(View)
			int  nWExt)				//表示倍率(Window)
{
	CPen newPen;
	CPen* pPenOld;
	CBrush newBrush;
	CBrush* pBrushOld;

	int x1,y1,x2,y2;
	//bool b9xDot=false;
	int fill = pPtnObj->m_nFill;


	int width = (pPtnObj->m_width * nVExt)/nWExt;
	int style = pPtnObj->m_style;

//	int width = (buff[1]*nVExt)/nWExt;
//	int style = buff[2];

	if(style == 0){
		newPen.CreatePen(PS_SOLID,width,col);
	}else /*if(style == 1)*/{
		if(width>1){
			//if(::g_bIsWin9x){
			//	if(fill == -1){	//フィルなしのときだけまじめに点線描画
			//		newPen.CreatePen(PS_SOLID,width,col);
			//		b9xDot=true;
			//	}else{
			//		newPen.CreatePen(PS_DOT,1,col);
			//	}
			//}else
			{
				DWORD adwPenStyle[2];
				adwPenStyle[0]=adwPenStyle[1]=width*2;
				LOGBRUSH logBrush;
				logBrush.lbStyle = BS_SOLID;
				logBrush.lbColor = col;
				newPen.CreatePen(PS_USERSTYLE|PS_GEOMETRIC|PS_ENDCAP_ROUND, width, &logBrush,2,adwPenStyle);
			}
		}else{
			newPen.CreatePen(PS_DOT,width,col);
		}
	}

	pPenOld = pDC->SelectObject(&newPen);




	//int fill = buff[3];

	if(fill==-1){
		pBrushOld=(CBrush*)pDC->SelectStockObject(NULL_BRUSH);
	}else{
		if(fill==1){
			newBrush.CreateSolidBrush(col);
			pBrushOld=(CBrush*)pDC->SelectObject(&newBrush);
		}else{
			if(col==RGB(0,0,0)){
				pBrushOld=(CBrush*)pDC->SelectStockObject(WHITE_BRUSH);
			}else{
				pBrushOld=(CBrush*)pDC->SelectStockObject(BLACK_BRUSH);
			}
		}
	}

	x1=pPtnObj->m_p0.x();	y1=pPtnObj->m_p0.y();
	convDirXY(x1,y1,dir,cx,cy);
	x1+=x0;  y1+=y0;
	x1 = (x1*nVExt)/nWExt;
	y1 = (y1*nVExt)/nWExt;

	x2=pPtnObj->m_p1.x();	y2=pPtnObj->m_p1.y();
	convDirXY(x2,y2,dir,cx,cy);
	x2+=x0;  y2+=y0;
	x2 = (x2*nVExt)/nWExt;
	y2 = (y2*nVExt)/nWExt;
	
	CRect rc(x1,y1,x2,y2);
	rc.NormalizeRect();
	if(width<=1){
		rc.right++;
		rc.bottom++;
	}

	//if(b9xDot){
	//	CPoint pt(1,1);
	//	DotArc9x(pDC,&rc,pt,pt,width);
	//}else
	{
		int nOldBkMmode=pDC->SetBkMode(TRANSPARENT);
		pDC->Ellipse(&rc);
		pDC->SetBkMode(nOldBkMmode);
	}
	pDC->SelectObject(pBrushOld);
	pDC->SelectObject(pPenOld);
}

void g_DrawPtnPolygon(
			CDC* pDC,				//描画
			SPtnObjPolygon* pPtnObj,	//描画オブジェクト
			COLORREF col,			//描画色
			int	 dir,
			int	 cx,
			int  cy,
			int	 x0,
			int  y0,
			int  nVExt,				//表示倍率(View)
			int  nWExt)				//表示倍率(Window)
{
	CPen newPen;
	CPen* pPenOld;
	CBrush newBrush;
	CBrush* pBrushOld;
	static POINT aPoint[VECTPLOYGON_MAXNODE]; 
	//bool b9xDot=false;
	int fill = pPtnObj->m_nFill;
	

	int node=pPtnObj->m_nCount;
	if(node<2||node>VECTPLOYGON_MAXNODE) return;

	int x1,y1;

	int width = (pPtnObj->m_width * nVExt)/nWExt;
	int style = pPtnObj->m_style;

//	int width = (buff[1]*nVExt)/nWExt;
//	int style = buff[2];

	if(style == 0){
		newPen.CreatePen(PS_SOLID,width,col);
	}else /*if(style == 1)*/{
		if(width>1){
			//if(::g_bIsWin9x){
			//	if(fill == -1){	//フィルなしのときだけまじめに点線描画
			//		newPen.CreatePen(PS_SOLID,width,col);
			//		b9xDot=true;
			//	}else{
			//		newPen.CreatePen(PS_DOT,1,col);
			//	}
			//}else
			{
				DWORD adwPenStyle[2];
				adwPenStyle[0]=adwPenStyle[1]=width*2;
				LOGBRUSH logBrush;
				logBrush.lbStyle = BS_SOLID;
				logBrush.lbColor = col;
				newPen.CreatePen(PS_USERSTYLE|PS_GEOMETRIC|PS_ENDCAP_ROUND, width, &logBrush,2,adwPenStyle);
			}
		}else{
			newPen.CreatePen(PS_DOT,width,col);
		}
	}

	pPenOld = pDC->SelectObject(&newPen);


//	int fill = buff[3];
	if(fill==-1){
		pBrushOld=(CBrush*)pDC->SelectStockObject(NULL_BRUSH);
	}else{
		if(fill==1){
			newBrush.CreateSolidBrush(col);
			pBrushOld=(CBrush*)pDC->SelectObject(&newBrush);
		}else{
			if(col==RGB(0,0,0)){
				pBrushOld=(CBrush*)pDC->SelectStockObject(WHITE_BRUSH);
			}else{
				pBrushOld=(CBrush*)pDC->SelectStockObject(BLACK_BRUSH);
			}
		}
	}


	int i;
	for(i=0;i<node;i++){
		x1=pPtnObj->m_pPt[i].x();
		y1=pPtnObj->m_pPt[i].y();
//		x1=buff[j++];	y1=buff[j++];
		convDirXY(x1,y1,dir,cx,cy);
		x1+=x0;  y1+=y0;
		x1 = (x1*nVExt)/nWExt;
		y1 = (y1*nVExt)/nWExt;
		aPoint[i].x=x1;
		aPoint[i].y=y1;
	}
	//if(b9xDot){
	//	DotPolygon9x(pDC,aPoint,node,width);
	//}else
	{
		pDC->SetPolyFillMode(WINDING);
		int nOldBkMmode=pDC->SetBkMode(TRANSPARENT);
		pDC->Polygon(aPoint,node);
		pDC->SetBkMode(nOldBkMmode);
	}
	pDC->SelectObject(pBrushOld);
	pDC->SelectObject(pPenOld);

}

void g_DrawPtnArc(
			CDC* pDC,				//描画
			SPtnObjArc* pPtnObj,	//描画オブジェクト
			COLORREF col,			//描画色
			int	 dir,
			int	 cx,
			int  cy,
			int	 x0,
			int  y0,
			int  nVExt,				//表示倍率(View)
			int  nWExt)				//表示倍率(Window)
{
	CPen newPen;
	CPen* pPenOld;
	//bool b9xDot=false;

	int x,y,r,x1,y1,x2,y2,x3,y3,x4,y4;

	int width = (pPtnObj->m_width * nVExt)/nWExt;
	int style = pPtnObj->m_style;
//	int width = (buff[1]*nVExt)/nWExt;
//	int style = buff[2];

	if(style == 0){
		newPen.CreatePen(PS_SOLID,width,col);
	}else /*if(style == 1)*/{
		if(width>1){
			//if(::g_bIsWin9x){
			//	newPen.CreatePen(PS_SOLID,width,col);
			//	b9xDot=true;
			//}else
			{
				DWORD adwPenStyle[2];
				adwPenStyle[0]=adwPenStyle[1]=width*2;
				LOGBRUSH logBrush;
				logBrush.lbStyle = BS_SOLID;
				logBrush.lbColor = col;
				newPen.CreatePen(PS_USERSTYLE|PS_GEOMETRIC|PS_ENDCAP_ROUND, width, &logBrush,2,adwPenStyle);
			}
		}else{
			newPen.CreatePen(PS_DOT,width,col);
		}
	}

	pPenOld = pDC->SelectObject(&newPen);

	x=pPtnObj->m_ptC.x();
	y=pPtnObj->m_ptC.y();
	r=pPtnObj->m_nR;
//	x=buff[3];	y=buff[4];  r=buff[5];

	x1=x-r;  y1=y-r;

	convDirXY(x1,y1,dir,cx,cy);
	x1+=x0;  y1+=y0;
	x1 = (x1*nVExt)/nWExt;
	y1 = (y1*nVExt)/nWExt;

	x2=x+r;  y2=y+r;
	convDirXY(x2,y2,dir,cx,cy);
	x2+=x0;  y2+=y0;
	x2 = (x2*nVExt)/nWExt;
	y2 = (y2*nVExt)/nWExt;

	x3=pPtnObj->m_ptBegin.x();
	y3=pPtnObj->m_ptBegin.y();
//	x3=buff[8];	y3=buff[9];
	convDirXY(x3,y3,dir,cx,cy);
	x3+=x0;  y3+=y0;
	x3 = (x3*nVExt)/nWExt;
	y3 = (y3*nVExt)/nWExt;

	x4=pPtnObj->m_ptEnd.x();
	y4=pPtnObj->m_ptEnd.y();
//	x4=buff[10]; y4=buff[11];
	convDirXY(x4,y4,dir,cx,cy);
	x4+=x0;  y4+=y0;
	x4 = (x4*nVExt)/nWExt;
	y4 = (y4*nVExt)/nWExt;


	CRect rc(x1,y1,x2,y2);
	rc.NormalizeRect();

	rc.right++;
	rc.bottom++;
	//if(width<=1){
	//	rc.right++;
	//	rc.bottom++;
	//}else if((width&1)==0){
	//	TRACE("On drawing ARC trimed rectangle size.\n");
	//	//rc.left--;
	//	//rc.top--;
	//	rc.right++;
	//	rc.bottom++;
	//}
	CPoint ptBegin;
	CPoint ptEnd;

	if(dir&4){
		ptBegin=CPoint(x4,y4);
		ptEnd=CPoint(x3,y3);
	}else{
		ptBegin=CPoint(x3,y3);
		ptEnd=CPoint(x4,y4);
	}

	//if(b9xDot){
	//	DotArc9x(pDC,&rc,ptBegin,ptEnd,width);
	//}else
	{
		int nOldBkMmode=pDC->SetBkMode(TRANSPARENT);
		pDC->Arc(&rc,ptBegin,ptEnd);
		pDC->SetBkMode(nOldBkMmode);
	}
	pDC->SelectObject(pPenOld);
}



//バス線の描画
void g_DrawBus(
			CDC* pDC,				//描画
			SXBSchObj* pObj,			//描画オブジェクト
			DWORD dwMode,			//描画モード
			int  nVExt,				//表示倍率(View)
			int  nWExt,				//表示倍率(Window)
			LPCRECT lprcClip)		//クリップ情報
{
	if(pObj->id()!=SXBSchObj::ID_BUS) return;
	int x0,y0,x1,y1;
	SXBSchBus* pBus=(SXBSchBus*)pObj;
	//描画座標の設定
	int nWidth=(3*nVExt)/nWExt;
	x0=(pBus->p1().x() * nVExt)/nWExt;
	y0=(pBus->p1().y() * nVExt)/nWExt;
	x1=(pBus->p2().x() * nVExt)/nWExt;
	y1=(pBus->p2().y() * nVExt)/nWExt;
	//線幅が１以下のときには
	//水平・垂直の場合に限り終点を補う
	if(nWidth<=1){
		if(pBus->p1().y()==pBus->p2().y()){//水平線なら...
			if(x1>x0)	x1+=1;
			else		x1-=1;		
		}else if(pBus->p1().x()==pBus->p2().x()){//垂直線なら...
			if(y1>y0)	y1+=1;
			else		y1-=1;
		}
	}

	int nOldDrawMode,nNewDrawMode;
	CPen penNew;
	CPen* ppenOld;
	//描画モードによってペンの種類・色とCOPY or XORを設定する
	if(dwMode & DRAW_DARK){
		penNew.CreatePen(PS_SOLID,nWidth,CBSchView::m_COL.colDARK);
		nNewDrawMode=R2_COPYPEN;
	}else if(dwMode & DRAW_ON){
		penNew.CreatePen(PS_SOLID,nWidth,CBSchView::m_COL.colBUS);
		nNewDrawMode=R2_COPYPEN;
	}else if(dwMode & DRAW_OFF){
		penNew.CreatePen(PS_SOLID,nWidth,RGB(0,0,0));
		nNewDrawMode=R2_COPYPEN;
	}else if(dwMode & DRAW_TEMP){
		penNew.CreatePen(PS_SOLID,nWidth,CBSchView::m_COL.colXOR);
		nNewDrawMode=R2_COPYPEN;
	}else{
		TRACE("\n不正な描画モード(CBSchBus::DrawBody())");
		return;
	}
	ppenOld=pDC->SelectObject(&penNew);			//作成したペンの選択
	nOldDrawMode=pDC->SetROP2(nNewDrawMode);	//描画モードの設定
	pDC->MoveTo(x0,y0);							//始点へ移動
	pDC->LineTo(x1,y1);							//描線
	pDC->SetROP2(nOldDrawMode);					//描画モードの復帰
	pDC->SelectObject(ppenOld);					//ペンの復帰
}


void arrowMarkEnd(SPoint pE,SPoint pC,int markSize,POINT* pEndArray,int  nVExt,int  nWExt)
{
	int x0 = (pE.x() * nVExt)/nWExt;
	int y0 = (pE.y() * nVExt)/nWExt;

	markSize = (markSize * nVExt)/nWExt;
	int markX = markSize;
	int markY = (markSize+3)/4;
	int x1 = markX;
	int y1 = markY;
	int x2 = markX;
	int y2 = -markY;

	double dx = pC.x()-pE.x();
	double dy = pC.y()-pE.y();
	dx = (dx * nVExt)/nWExt;
	dy = (dy * nVExt)/nWExt;

	double len=sqrt(dx*dx+dy*dy);
	dx /= len;	//dxは回転におけるcos値
	dy /= len;	//dyは回転におけるsin値
	
	int newX1 = (int)(x1*dx-y1*dy);
	int newY1 = (int)(x1*dy+y1*dx);

	int newX2 = (int)(x2*dx-y2*dy);
	int newY2 = (int)(x2*dy+y2*dx);

	pEndArray[0].x = x0;
	pEndArray[0].y = y0;

	pEndArray[1].x = x0+newX1;
	pEndArray[1].y = y0+newY1;

	pEndArray[2].x = x0+newX2;
	pEndArray[2].y = y0+newY2;
}



//破線の描画
void g_DrawDash(
			CDC* pDC,						//描画先
			SXBSchObj* pObj,					//描画オブジェクト
			DWORD dwMode,					//描画モード
			int  nVExt /* =1 */,			//表示倍率(View)
			int  nWExt /* =1 */,			//表示倍率(Window)
			LPCRECT lprcClip /*=NULL*/)		//クリップ情報
{
	if(pObj->id()!=SXBSchObj::ID_DASH) return;
	SXBSchDash* pDash=(SXBSchDash*)pObj;
	//描画座標の設定
	//int nWidth=(1*nVExt)/nWExt;
	int nWidth=(pDash->lineWidth()*nVExt)/nWExt;

	//int x0,y0,x1,y1;
	//x0=(pDash->p1().x() * nVExt)/nWExt;
	//y0=(pDash->p1().y() * nVExt)/nWExt;
	//x1=(pDash->p2().x() * nVExt)/nWExt;
	//y1=(pDash->p2().y() * nVExt)/nWExt;
	////線幅が1以下のときには
	////直線で、水平・垂直の場合に限り終点を補う
	//if(nWidth<=1 && !pDash->curve()){
	//	if(pDash->p1().y()==pDash->p2().y()){//水平線なら...
	//		if(x1>x0)	x1+=1;
	//		else		x1-=1;
	//	}else if(pDash->p1().x()==pDash->p2().x()){//垂直線なら...
	//		if(y1>y0)	y1+=1;
	//		else		y1-=1;
	//	}
	//}
	
	int nOldDrawMode;
	int nOldBkMmode;
	CPen penNew;
	CPen* ppenOld;
	COLORREF col;
	//描画モードによってペンの種類・色とCOPY or XORを設定する
	if(dwMode & DRAW_DARK){
		col = CBSchView::m_COL.colDARK;
	}else if(dwMode & DRAW_ON){
		col = CBSchView::m_COL.colDASH;
	}else if(dwMode & DRAW_OFF){
		col = RGB(0,0,0);
	}else if(dwMode & DRAW_TEMP){
		col = CBSchView::m_COL.colXOR;
	}else{
		TRACE("\n不正な描画モード(CBSchDash::DrawBody())");
		return;
	}


	if(pDash->lineStyle()==SXBSchDash::LINESTYLE_NORMAL){
		penNew.CreatePen(PS_SOLID,nWidth,col);
	}else{
		DWORD adwPenStyle[6];
		int pitchBase = nWidth;
		if(pitchBase<1) pitchBase = 1;
		LOGBRUSH logBrush;
		logBrush.lbStyle = BS_SOLID;
		logBrush.lbColor = col;

		if(pDash->lineStyle()==SXBSchDash::LINESTYLE_LINEDOT){
			adwPenStyle[0]=pitchBase*12;
			adwPenStyle[1]=adwPenStyle[2]=adwPenStyle[3]=pitchBase*3;
			penNew.CreatePen(PS_USERSTYLE|PS_GEOMETRIC|PS_ENDCAP_ROUND, nWidth, &logBrush,4,adwPenStyle);
		}else if(pDash->lineStyle()==SXBSchDash::LINESTYLE_LINEDOTDOT){
			adwPenStyle[0]=pitchBase*12;
			adwPenStyle[1]=adwPenStyle[2]=adwPenStyle[3]=adwPenStyle[4]=adwPenStyle[5]=pitchBase*3;
			penNew.CreatePen(PS_USERSTYLE|PS_GEOMETRIC|PS_ENDCAP_ROUND, nWidth, &logBrush,6,adwPenStyle);
		}else{	//pDash->lineStyle()==SXBSchDash::LINESTYLE_DASH
			adwPenStyle[0]=adwPenStyle[1]=pitchBase*4;
			penNew.CreatePen(PS_USERSTYLE|PS_GEOMETRIC|PS_ENDCAP_ROUND, nWidth, &logBrush,2,adwPenStyle);
		}
	}
	//bool b9xDot=false;
	//if(nWidth>1){
		//if(::g_bIsWin9x){
		//	penNew.CreatePen(PS_SOLID,nWidth,col);
		//	b9xDot=true;
		//}else
		//{
		//	int pitchBase = nWidth;
		//	if(pitchBase<1) pitchBase = 1;
		//	DWORD adwPenStyle[2];
		//	adwPenStyle[0]=adwPenStyle[1]=pitchBase*4;
		//	LOGBRUSH logBrush;
		//	logBrush.lbStyle = BS_SOLID;
		//	logBrush.lbColor = col;
		//	penNew.CreatePen(PS_USERSTYLE|PS_GEOMETRIC|PS_ENDCAP_ROUND, nWidth, &logBrush,2,adwPenStyle);
		//}
	//}else{
	//	penNew.CreatePen(PS_DOT,nWidth,col);
	//}
	nOldBkMmode=pDC->SetBkMode(TRANSPARENT);
	ppenOld=pDC->SelectObject(&penNew);			//作成したペンの選択
	nOldDrawMode=pDC->SetROP2(R2_COPYPEN);		//描画モードの設定
	//if(b9xDot){
	//	bool bMark=true;
	//	int nPos=0;
	//	DotLine9x(pDC,x0,y0,x1,y1,nWidth,bMark,nPos);
	//}else
	if(!pDash->curve()){
		int x0,y0,x1,y1;
		x0=(pDash->p1().x() * nVExt)/nWExt;
		y0=(pDash->p1().y() * nVExt)/nWExt;
		x1=(pDash->p2().x() * nVExt)/nWExt;
		y1=(pDash->p2().y() * nVExt)/nWExt;
		//線幅が1以下のときには
		//直線で、水平・垂直の場合に限り終点を補う
		//if(nWidth<=1 && !pDash->curve()){
		if(nWidth<=1){
			if(pDash->p1().y()==pDash->p2().y()){//水平線なら...
				if(x1>x0)	x1+=1;
				else		x1-=1;
			}else if(pDash->p1().x()==pDash->p2().x()){//垂直線なら...
				if(y1>y0)	y1+=1;
				else		y1-=1;
			}
		}

		pDC->MoveTo(x0,y0);							//始点へ移動
		pDC->LineTo(x1,y1);							//描線
	}else{
		CPoint pointArray[4];
		pointArray[0] = CPoint((pDash->p1().x() * nVExt)/nWExt,(pDash->p1().y() * nVExt)/nWExt);
		pointArray[1] = CPoint((pDash->Ct1().x() * nVExt)/nWExt,(pDash->Ct1().y() * nVExt)/nWExt);
		pointArray[2] = CPoint((pDash->Ct2().x() * nVExt)/nWExt,(pDash->Ct2().y() * nVExt)/nWExt);
		pointArray[3] = CPoint((pDash->p2().x() * nVExt)/nWExt,(pDash->p2().y() * nVExt)/nWExt);

		pDC->PolyBezier(pointArray,4);
	}


	pDC->SelectObject(ppenOld);					//ペンの復帰
	penNew.DeleteObject();

	if(pDash->startPointStyle() != SXBSchDash::ENDPOINTSTYLE_NORMAL || pDash->endPointStyle() != SXBSchDash::ENDPOINTSTYLE_NORMAL){
		CBrush brushNew;
		CBrush* pbrushOld;
		brushNew.CreateSolidBrush(col);
		pbrushOld = (CBrush*)pDC->SelectObject(&brushNew);
		penNew.CreatePen(PS_SOLID,nWidth,col);
		ppenOld=pDC->SelectObject(&penNew);
		pDC->SetPolyFillMode(WINDING);

		POINT ptArray[3];
		if(pDash->startPointStyle() == SXBSchDash::ENDPOINTSTYLE_ARROW){
			SPoint pC;
			if(pDash->curve()){
				pC = pDash->Ct1();
			}else{
				pC = pDash->p2();
			}
			arrowMarkEnd(pDash->p1(),pC,pDash->endMarkSize(),ptArray,nVExt,nWExt);
			pDC->Polygon(ptArray,3);
		}else if(pDash->startPointStyle() == SXBSchDash::ENDPOINTSTYLE_CIRCLE){
			int rad = (pDash->endMarkSize()*nVExt)/(2*nWExt);
			int x0=(pDash->p1().x() * nVExt)/nWExt;
			int y0=(pDash->p1().y() * nVExt)/nWExt;
			int x1= x0-rad;
			int y1= y0-rad;
			x0+=rad;
			y0+=rad;
			pDC->Ellipse(x0,y0,x1,y1);
		}
		
		if(pDash->endPointStyle() == SXBSchDash::ENDPOINTSTYLE_ARROW){
			SPoint pC;
			if(pDash->curve()){
				pC = pDash->Ct2();
			}else{
				pC = pDash->p1();
			}
			arrowMarkEnd(pDash->p2(),pC,pDash->endMarkSize(),ptArray,nVExt,nWExt);
			pDC->Polygon(ptArray,3);
		}else if(pDash->endPointStyle() == SXBSchDash::ENDPOINTSTYLE_CIRCLE){
			int rad = (pDash->endMarkSize()*nVExt)/(2*nWExt);
			int x0=(pDash->p2().x() * nVExt)/nWExt;
			int y0=(pDash->p2().y() * nVExt)/nWExt;
			int x1= x0-rad;
			int y1= y0-rad;
			x0+=rad;
			y0+=rad;
			pDC->Ellipse(x0,y0,x1,y1);
		}
		


		pDC->SelectObject(ppenOld);
		pDC->SelectObject(pbrushOld);
	}



	pDC->SetROP2(nOldDrawMode);					//描画モードの復帰
	pDC->SetBkMode(nOldBkMmode);				//透過モードからの復帰

}

//ワイヤーの描画
void g_DrawWire(
			CDC* pDC,						//描画先
			SXBSchObj* pObj,				//描画オブジェクト
			DWORD dwMode,					//描画モード
			int  nVExt /* =1 */,			//表示倍率(View)
			int  nWExt /* =1 */,			//表示倍率(Window)
			LPCRECT lprcClip /*=NULL*/)		//クリップ情報
{
	if(pObj->id()!=SXBSchObj::ID_WIRE) return;
	int x0,y0,x1,y1;
	SXBSchWire* pWire=(SXBSchWire*)pObj;
	//描画座標の設定
	x0=(pWire->p1().x() * nVExt)/nWExt;
	y0=(pWire->p1().y() * nVExt)/nWExt;
	x1=(pWire->p2().x() * nVExt)/nWExt;
	y1=(pWire->p2().y() * nVExt)/nWExt;
	//線幅が１以下のときには
	//水平・垂直の場合に限り終点を補う
	int width = (1*nVExt)/nWExt;
	if(width<=1){
		if(pWire->p1().y()==pWire->p2().y()){//水平線なら...
			if(x1>x0)	x1+=1;
			else		x1-=1;
		}else if(pWire->p1().x()==pWire->p2().x()){//垂直線なら...
			if(y1>y0)	y1+=1;
			else		y1-=1;
		}
	}
//	
//	
//	x0=(pWire->GetPoint().x  * nVExt)/nWExt;
//	y0=(pWire->GetPoint().y  * nVExt)/nWExt;
//	x1=(pWire->GetPoint1().x * nVExt)/nWExt;
//	y1=(pWire->GetPoint1().y * nVExt)/nWExt;
//	//水平・垂直の場合に限り終点を補う
//	if(pWire->GetPoint().y==pWire->GetPoint1().y){//水平線なら...
//		x1+=1;
//	}else if(pWire->GetPoint().x==pWire->GetPoint1().x){//垂直線なら...
//		y1+=1;
//	}

	int nOldDrawMode;
	COLORREF col;
	CPen penNew;
	CPen* ppenOld;
	//描画モードによってペンの種類・色とCOPY or XORを設定する
	if(dwMode & DRAW_DARK){
		col = CBSchView::m_COL.colDARK;
	}else if(dwMode & DRAW_ON){
		col = CBSchView::m_COL.colWIRE;
	}else if(dwMode & DRAW_OFF){
		col = RGB(0,0,0);
	}else if(dwMode & DRAW_TEMP){
		col = CBSchView::m_COL.colXOR;
	}else{
		TRACE("\n不正な描画モード(CBSchWire::DrawBody())");
		return;
	}

	penNew.CreatePen(PS_SOLID,width,col);
	ppenOld=pDC->SelectObject(&penNew);			//作成したペンの選択
	nOldDrawMode=pDC->SetROP2(R2_COPYPEN);		//描画モードの設定
	pDC->MoveTo(x0,y0);							//始点へ移動
	pDC->LineTo(x1,y1);							//描線
	pDC->SetROP2(nOldDrawMode);					//描画モードの復帰
	pDC->SelectObject(ppenOld);					//ペンの復帰
}


//マーカーラインの描画
void g_DrawMarkerLine(
			CDC* pDC,						//描画先
			SXBSchObj* pObj,				//描画オブジェクト
			DWORD dwMode,					//描画モード
			int  nVExt /* =1 */,			//表示倍率(View)
			int  nWExt /* =1 */,			//表示倍率(Window)
			LPCRECT lprcClip /*=NULL*/)		//クリップ情報
{
	if(pObj->id()!=SXBSchObj::ID_MARKER) return;
	int x0,y0,x1,y1;
	SXBSchMarkerLine* pMarker=(SXBSchMarkerLine*)pObj;
	//描画座標の設定
	x0=(pMarker->p1().x() * nVExt)/nWExt;
	y0=(pMarker->p1().y() * nVExt)/nWExt;
	x1=(pMarker->p2().x() * nVExt)/nWExt;
	y1=(pMarker->p2().y() * nVExt)/nWExt;
	int width = (pMarker->m_width * nVExt)/nWExt;

	int nOldDrawMode;
	COLORREF col;
	CPen penNew;
	CPen* ppenOld;
	int drawMode = R2_COPYPEN;
	//描画モードによってペンの種類・色とCOPY or XORを設定する
	if(dwMode & DRAW_DARK){
		col = CBSchView::m_COL.colDARK;
	}else if((dwMode & DRAW_ON)||(dwMode & DRAW_OFF)){
		col = (COLORREF)(pMarker->m_nColor);
//	}else if(dwMode & DRAW_OFF){
//		col = RGB(0,0,0);
	}else if(dwMode & DRAW_TEMP){
		col = (DWORD)(CBSchView::m_COL.colXOR)^(DWORD)(CBSchView::m_COL.colBG);
		drawMode = R2_XORPEN;
	}else{
		TRACE("\n不正な描画モード(CBSchWire::DrawBody())");
		return;
	}

	LOGBRUSH logBrush;
	logBrush.lbStyle = BS_SOLID;
	logBrush.lbColor = col;

	penNew.CreatePen(PS_GEOMETRIC|PS_ENDCAP_SQUARE, width,&logBrush);
	ppenOld=pDC->SelectObject(&penNew);			//作成したペンの選択
	nOldDrawMode=pDC->SetROP2(drawMode);		//描画モードの設定
	pDC->MoveTo(x0,y0);							//始点へ移動
	pDC->LineTo(x1,y1);							//描線
	pDC->SetROP2(nOldDrawMode);					//描画モードの復帰
	pDC->SelectObject(ppenOld);					//ペンの復帰
}



//エントリーの描画
void g_DrawEntry(
			CDC* pDC,							//描画先
			SXBSchObj* pObj,					//描画オブジェクト
			DWORD dwMode,						//描画モード
			int  nVExt,							//表示倍率(View)
			int  nWExt,							//表示倍率(Window)
			LPCRECT lprcClip)					//クリップ情報
{
	int id=pObj->id();
	if((id!=SXBSchObj::ID_ENTRY) && (id!=SXBSchObj::ID_BENTRY)) return;
	int x0,y0,x1,y1;
	SXBSchEntry* pEntry=(SXBSchEntry*)pObj;

	x0=(pEntry->p1().x() * nVExt)/nWExt;
	y0=(pEntry->p1().y() * nVExt)/nWExt;
	x1=(pEntry->p2().x() * nVExt)/nWExt;
	y1=(pEntry->p2().y() * nVExt)/nWExt;

//	x1=(pEntry->GetPoint().x*nVExt)/nWExt;
//	y1=(pEntry->GetPoint().y*nVExt)/nWExt;
//	y0=((pEntry->GetPoint().y-10)*nVExt)/nWExt;
//	DWORD dwType = pEntry->GetType();
//	if(dwType==ENTRY_BL || dwType==ENTRY_WL) x0=pEntry->GetPoint().x+10;
//	else										 x0=pEntry->GetPoint().x-10;
//	x0=(x0*nVExt)/nWExt;
	//描画線幅の設定
	int nWidth;
	if(id==SXBSchObj::ID_BENTRY){
		nWidth=3;
	}else{
		nWidth=1;
	}

	nWidth=(nWidth*nVExt)/nWExt;

	int nOldDrawMode;
	CPen penNew;
	CPen* ppenOld;
	//描画モードによってペンの種類・色とCOPY or XORを設定する
	if(dwMode & DRAW_DARK){
		penNew.CreatePen(PS_SOLID,nWidth,CBSchView::m_COL.colDARK);
	}else if(dwMode & DRAW_ON){
		COLORREF col;
		if(id==SXBSchObj::ID_BENTRY) col=CBSchView::m_COL.colBUS;
		else                         col=CBSchView::m_COL.colWIRE;
		penNew.CreatePen(PS_SOLID,nWidth,col);
	}else if(dwMode & DRAW_OFF){
		penNew.CreatePen(PS_SOLID,nWidth,RGB(0,0,0));
	}else if(dwMode & DRAW_TEMP){
		penNew.CreatePen(PS_SOLID,nWidth,CBSchView::m_COL.colXOR);
	}else{
		TRACE("\n不正な描画モード(CBSchEntry::Draw())");
		return;
	}
	ppenOld=pDC->SelectObject(&penNew);			//作成したペンの選択
	nOldDrawMode=pDC->SetROP2(R2_COPYPEN);		//描画モードの設定
	pDC->MoveTo(x0,y0);							//始点へ移動
	pDC->LineTo(x1,y1);							//描線
	pDC->SetROP2(nOldDrawMode);					//描画モードの復帰
	pDC->SelectObject(ppenOld);					//ペンの復帰
}



//ジャンクションの描画
void g_DrawJunc(
			CDC* pDC,						//描画先
			SXBSchObj* pObj,				//描画オブジェクト
			DWORD dwMode,					//描画モード
			int  nVExt /* =1 */,			//表示倍率(View)
			int  nWExt /* =1 */,			//表示倍率(Window)
			LPCRECT lprcClip /*=NULL*/)		//クリップ情報
{
	if(pObj->id()!=SXBSchObj::ID_JUNCTION) return;
	SXBSchJunc* pJunc=(SXBSchJunc*)pObj;

    //DWORD dwRop;
	//COLORREF col,colBk;
	COLORREF col;
	int	noff,nh;	//描画サイズ
	int x,y;		//描画位置
	int nOldDrawMode,nNewDrawMode;

	CPen penNew;
	CPen* ppenOld;
	CBrush brushNew;
	CBrush* pbrushOld;

	//描画オペレーションと描画色の設定
	if(dwMode & DRAW_DARK){
		nNewDrawMode=R2_COPYPEN;
		col=CBSchView::m_COL.colDARK;
	}else if(dwMode & DRAW_ON){
		nNewDrawMode=R2_COPYPEN;
		col=CBSchView::m_COL.colJUNC;
	}else if(dwMode & DRAW_OFF){
		nNewDrawMode=R2_COPYPEN;
		col=RGB(0,0,0);
	}else if(dwMode & DRAW_TEMP){
		nNewDrawMode=R2_COPYPEN;
		col=CBSchView::m_COL.colXOR;
	}else{
		TRACE("Invalid draw mode (CBSchJunc::Draw())\n");
	}
	
	//描画サイズと描画位置の設定
	noff=(2*nVExt)/nWExt;			//PatBlt()の左上へのオフセット
	nh=noff*2+1;					//PatBlt()の正方形の一辺のサイズ
	x=(pJunc->p1().x()*nVExt)/nWExt-noff;	//描画座標
	y=(pJunc->p1().y()*nVExt)/nWExt-noff;	//描画座標

	penNew.CreatePen(PS_SOLID,0,col);
	brushNew.CreateSolidBrush(col);
	ppenOld=(CPen*)pDC->SelectObject(&penNew);
	pbrushOld=(CBrush*)pDC->SelectObject(&brushNew);

//	colBk = pDC->GetBkColor();
	nOldDrawMode = pDC->SetROP2(nNewDrawMode);
	pDC->Ellipse(x,y,x+nh,y+nh);
//	pDC->FillSolidRect(x,y,nh,nh,col);
	pDC->SetROP2(nOldDrawMode);
//	pDC->SetBkColor(colBk);

	pDC->SelectObject(ppenOld);
	pDC->SelectObject(pbrushOld);
}


void g_DrawLabel(
	CDC* pDC,						//描画先
	SXBSchObj* pObj,				//描画オブジェクト
	DWORD dwMode,					//描画モード
	int nVExt/*=1*/,				//表示倍率(View)
	int nWExt/*=1*/,				//表示倍率(Window)
	LPCRECT lprcClip /*=NULL*/)		//クリップ情報
{
	if(pObj->id()!=SXBSchObj::ID_LABEL) return;
	SXBSchLabel* pLabel = (SXBSchLabel*)pObj;
	const TCHAR* pcsz = pLabel->text();
	if(!pcsz) return;
	//文字数を得る
	int nStrLen;		//文字数
	nStrLen=_tcslen(pcsz);
	if(!nStrLen) return;
	//描画色の決定
	COLORREF col;
	if(dwMode & DRAW_DARK)		col=CBSchView::m_COL.colDARK; 
	else if(dwMode & DRAW_TEMP)	col=CBSchView::m_COL.colXOR; 
	else if(dwMode & DRAW_OFF)	col=RGB(0,0,0);
	else						col=CBSchView::m_COL.colLABEL;
	//座標の設定
	int x,y;
	CSize size;
	if(pLabel->horizontal()){
		x=pLabel->p1().x();
		y=pLabel->p1().y()-2;
		size = g_pFont->PutFont(pDC,x,y,pcsz,nStrLen,col,dwMode,FALSE,0,nVExt,nWExt,FALSE);
	}else{
		x=pLabel->p1().x()-2;
		y=pLabel->p1().y();
		size = g_pFont->PutFont(pDC,x,y,pcsz,nStrLen,col,dwMode,TRUE,0,nVExt,nWExt,FALSE);
	}
//TRACE("XY(%d,%d)\n",size.cx,size.cy);
	int w = size.cx * nWExt/ nVExt;
	int h = size.cy * nWExt/ nVExt;
	pLabel->setDrawSize(SSize(w,h));
}


#define SUB_SUP_SIZE 6

//タグ修飾したコメントの折り返しフラグの設定
static void setRichCommentBreakFlags(
	CDC* pDC,						//描画先
	SXBSchComment* pComment)		//描画オブジェクト
{
	int width = pComment->width();
	if(width<0) width=100000;
	CSize size;

	LOGFONT lf;
	char* p=(char*)&lf;	//LOGFONT構造体の０イニシャライズ
	for(int n=0;n<sizeof(LOGFONT);n++) *p++=0;
	//lf.lfHeight=-pComment->fontSize();
	lf.lfWeight=FW_REGULAR;						
	//lf.lfItalic=false;
	lf.lfCharSet=DEFAULT_CHARSET;
	lf.lfPitchAndFamily=0;

	CFont pFont;

	MarkupTextLineIterator ite;
	int widthRemain =width;
	ite = pComment->m_pMarkupTextLineList->begin();
	while(ite != pComment->m_pMarkupTextLineList->end()){
		SMarkupTextLine* pMTL = *ite;
		if(pMTL->m_fontName.length()>0){
			_tcsncpy(lf.lfFaceName,pMTL->m_fontName.c_str(),LF_FACESIZE);
		}else{
			_tcsncpy(lf.lfFaceName,pComment->fontName(),LF_FACESIZE);
		}
		lf.lfFaceName[LF_FACESIZE-1]='\0';		
		lf.lfHeight = ((pMTL->m_textSyle & ( (unsigned int)SMarkupTextLine::SUB | (unsigned int)SMarkupTextLine::SUP)) ? -pComment->fontSize()*SUB_SUP_SIZE/10 : -pComment->fontSize()); 
		lf.lfItalic = ( pMTL->m_textSyle & (unsigned int)SMarkupTextLine::ITALIC) != 0;
		lf.lfWeight = ((pMTL->m_textSyle & (unsigned int)SMarkupTextLine::BOLD) ? FW_BOLD : FW_REGULAR);
		CFont*	pFont=new CFont;
		pFont->CreateFontIndirect(&lf);	//フォントクラスの初期化
		pFont=pDC->SelectObject(pFont);					//フォントの選択

		int length = _tcslen(pMTL->m_strText.c_str());		

		if(length==0 && (pMTL->m_textSyle & (unsigned int)SMarkupTextLine::CR)){	//改行だけのMTL
			pMTL->m_charFlags[ 0 ] |= SMarkupTextLine::BREAKLINE;
			widthRemain =width;			//残り幅値を初期化
		}

		int curLineTopOffset  = 0;
		int tryLength = length;
		int newTryLength;
		const TCHAR* pcsz = pMTL->m_strText.c_str();

		//CSize sizeText = pDC->GetTextExtent(pMTL->m_strText.c_str(),length);	//文字列全体の描画幅を取得する。


		while(curLineTopOffset<length){
		
			CSize sizeText = pDC->GetTextExtent(pcsz + curLineTopOffset,tryLength);
			if(widthRemain >= sizeText.cx){		//行の残り幅に文字列全体が収まる場合
				if(pMTL->m_textSyle & (unsigned int)SMarkupTextLine::CR){	//文字列で改行する場合
					pMTL->m_charFlags[ length ] |= SMarkupTextLine::BREAKLINE;
					widthRemain =width;			//残り幅値を初期化
				}else{
					widthRemain -=  sizeText.cx;	//残り幅から描画幅を引く
				}
				break;
			}else{
				int loopLimit = 0;
				int prevTryLength = tryLength;
				while(loopLimit<20){
					newTryLength = tryLength * widthRemain / (sizeText.cx+1);
					

					//改行可能な位置を探す
					while((curLineTopOffset + newTryLength)<length && pMTL->m_charFlags[ curLineTopOffset + newTryLength ]==0){
						newTryLength++;
					}

					if( (curLineTopOffset + newTryLength) >= length){
						newTryLength = length - curLineTopOffset;
						break;
					}
					if(newTryLength == tryLength || newTryLength == prevTryLength){
						break;
					}
					prevTryLength = tryLength;
					tryLength = newTryLength;
					sizeText = pDC->GetTextExtent(pcsz+curLineTopOffset,tryLength);
					if(loopLimit>10){
						TRACE("loopLimit > 10\n");
					}
					loopLimit++;
				}
				TRACE("setRichCommentBreakFlags() loopLimit : %d\n",loopLimit);

				pMTL->m_charFlags[ curLineTopOffset + newTryLength ] |= SMarkupTextLine::BREAKLINE;
				widthRemain = width;
				curLineTopOffset += newTryLength;
				tryLength = length - curLineTopOffset;
			}
		}
		//フォントの削除
		pFont=pDC->SelectObject(pFont);					
		delete pFont;

#ifdef _DEBUG
		//TRACE(_T("%s STYLE:%04X FONT:%s\n"),(*ite)->m_strText.c_str(),(*ite)->m_textSyle,(*ite)->m_fontName.c_str());
		//for(int dn = 0;dn<length+1;dn++){
		//	TRACE("%d",((*ite)->m_charFlags)[dn]);
		//}
		//TRACE("\n");
#endif
		ite++;
	}

}







//コメントの描画
void g_DrawRichComment(
	CDC* pDC,						//描画先
	SXBSchComment* pComment,		//描画オブジェクト
	DWORD dwMode,					//描画モード
	int  nVExt,						//表示倍率(View)
	int  nWExt,						//表示倍率(Window)
	LPCRECT lprcClip)				//クリップ情報			
{
	bool printerMode = ((dwMode & DRAW_FOR_PRINTER)!=0);
	if(printerMode){
		pComment->deleteMarkupLineList();
	}
	if(pComment->m_pMarkupTextLineList == NULL){
		pComment->setupMarkupLineList(printerMode);
		setRichCommentBreakFlags(pDC,pComment);
	}

	COLORREF col;
	CSize size;
	//描画色の決定
	if(dwMode & DRAW_DARK)		col=CBSchView::m_COL.colDARK;
	else if(dwMode & DRAW_TEMP)	col=CBSchView::m_COL.colXOR; 
	else						col=CBSchView::m_COL.colCOMMENT;

	LOGFONT lf;
	char* p=(char*)&lf;	//LOGFONT構造体の０イニシャライズ
	for(int n=0;n<sizeof(LOGFONT);n++) *p++=0;
	lf.lfCharSet=DEFAULT_CHARSET;



	//Changed Y position of the RichComment On Version 0.69.02
	//older version: align top
	//0.69.02      : align baseline

	int baseX = pComment->p1().x() * nVExt/nWExt;
	
	//Y-positon is adjusted without deep thinking to keep compatibility with the older version.
	int baseY = (pComment->p1().y()-COMMENT_Y_OFFSET) * nVExt/nWExt;
	int lineHeight = pComment->fontSize()*12 * nVExt/nWExt/10;
	baseY -= (pComment->fontSize()* 2 * nVExt/nWExt/10);

	//int nSUBoffset = pComment->fontSize() *(10-SUB_SUP_SIZE)*nVExt/nWExt/10;

	int currentX = baseX;
	int currentY = baseY;

	int maxX = baseX;


	CFont* pFont;
	int fontHeight;
	TEXTMETRIC textMetric;

	/////////////////////////////////////////////////////////
	//Version 0.69.02
	//Set top ypos-offset for <SUP>
	_tcsncpy(lf.lfFaceName,pComment->fontName(),LF_FACESIZE);
	lf.lfFaceName[LF_FACESIZE-1]='\0';		
	lf.lfHeight = -pComment->fontSize() * nVExt/nWExt;
	if(lf.lfHeight==0) lf.lfHeight = -1;//2014/04/13
	lf.lfItalic = 0;
	lf.lfWeight = FW_REGULAR;
	pFont=new CFont;
	pFont->CreateFontIndirect(&lf);
	pFont=pDC->SelectObject(pFont);		//Select created font
	
	pDC->GetTextMetrics(&textMetric);
	
	int yPosSUP = textMetric.tmAscent;
	
	//	*** To get y-position for <SUP>, following way does not work as expected when fontface is Mieryo. ***
	//	int yPosSUP = textMetric.tmHeight-textMetric.tmInternalLeading-textMetric.tmDescent;	

	pFont=pDC->SelectObject(pFont);		//restore font
	delete pFont;						//delete created font
	pFont = NULL;
	/////////////////////////////////////////////////////////

	bool lastlineUsed = false;
	int lineCount = 1;

	MarkupTextLineIterator ite;
	SMarkupTextLine* pMTL;
	ite = pComment->m_pMarkupTextLineList->begin();
	while(ite != pComment->m_pMarkupTextLineList->end()){
		pMTL = *ite;

		//TRACE("%s\n",pMTL->m_strText.c_str());

		if(pMTL->m_fontName.length()>0){
			_tcsncpy(lf.lfFaceName,pMTL->m_fontName.c_str(),LF_FACESIZE);
		}else{
			_tcsncpy(lf.lfFaceName,pComment->fontName(),LF_FACESIZE);
		}

		fontHeight = ((pMTL->m_textSyle & ( (unsigned int)SMarkupTextLine::SUB | (unsigned int)SMarkupTextLine::SUP)) ? -pComment->fontSize()*SUB_SUP_SIZE/10 : -pComment->fontSize()); 
		lf.lfFaceName[LF_FACESIZE-1]='\0';		
		lf.lfHeight = fontHeight * nVExt/nWExt;
		if(lf.lfHeight==0)lf.lfHeight = -1;		//2014/04/13
		lf.lfItalic = ( pMTL->m_textSyle & (unsigned int)SMarkupTextLine::ITALIC) != 0;
		lf.lfWeight = ((pMTL->m_textSyle & (unsigned int)SMarkupTextLine::BOLD) ? FW_BOLD : FW_REGULAR);
		pFont=new CFont;
		pFont->CreateFontIndirect(&lf);		//フォントクラスの初期化
		pFont=pDC->SelectObject(pFont);		//フォントの選択
		

		int length = _tcslen(pMTL->m_strText.c_str());		
		int curLineTopOffset  = 0;
		int drawLength;
		int endOffset;
		const TCHAR* pcsz = pMTL->m_strText.c_str();
		
		int yoffset=0;
		//if(pMTL->m_textSyle & (unsigned int)SMarkupTextLine::SUB) yoffset = nSUBoffset;

		if((pMTL->m_charFlags[0]&(unsigned int)SMarkupTextLine::BREAKLINE)!=0){
			lineCount++;
			currentY += lineHeight;
			currentX = baseX;
			lastlineUsed = false;
		}


		while(curLineTopOffset<length){
			drawLength = 0;
			endOffset = curLineTopOffset;
			while(endOffset<length){
				endOffset++;
				drawLength++;
				if((pMTL->m_charFlags[endOffset]&(unsigned int)SMarkupTextLine::BREAKLINE)!=0)break;
			}
			bool lineBreak = ((pMTL->m_charFlags[endOffset]&(unsigned int)SMarkupTextLine::BREAKLINE)!=0);
			//bool lineEnd = (endOffset==length || (pMTL->m_charFlags[endOffset]&(unsigned int)SMarkupTextLine::BREAKLINE)!=0);

			CSize sizeText = pDC->GetTextExtent(pcsz + curLineTopOffset,drawLength);

			COLORREF colOldText;
			int      nOldBkMode;

			//Set text aligin mode (and save old mode) 2011/05/04 0.69.02
			UINT newAlignMode;
			if(pMTL->m_textSyle &  (unsigned int)SMarkupTextLine::SUP){
				newAlignMode = TA_LEFT|TA_TOP|TA_NOUPDATECP;
				yoffset = -yPosSUP;
			}else{
				newAlignMode = TA_LEFT|TA_BASELINE|TA_NOUPDATECP;
				if(pMTL->m_textSyle &  (unsigned int)SMarkupTextLine::SUB){
					yoffset = (1 * nVExt/nWExt)+1;
				}else{
					yoffset = 0;
				}
			}

			UINT oldAlignMode = pDC->SetTextAlign(newAlignMode);

			if(dwMode&DRAW_ON || dwMode&DRAW_TEMP){
				colOldText	= pDC->SetTextColor(col);			//TextColorの設定
				nOldBkMode  = pDC->SetBkMode(TRANSPARENT);		//描画は透過モード
				pDC->TextOut(currentX,currentY+yoffset,pcsz + curLineTopOffset,drawLength);				//文字描画
				pDC->SetBkMode(nOldBkMode);						//背景描画モードの復帰
				pDC->SetTextColor(colOldText);					//TextColorの復帰
			}else if(dwMode&DRAW_OFF){
				colOldText	= pDC->SetTextColor(0);				//TextColorの設定（黒で描画）
				nOldBkMode  = pDC->SetBkMode(TRANSPARENT);		//描画は透過モード
				pDC->TextOut(currentX,currentY+yoffset,pcsz + curLineTopOffset,drawLength);				//文字描画
				pDC->SetBkMode(nOldBkMode);						//背景描画モードの復帰
				pDC->SetTextColor(colOldText);					//TextColorの復帰
			}else{
				TRACE("不正な描画モード\n");
			}

			//0.69.02
			//restore the old text align mode
			pDC->SetTextAlign(oldAlignMode);

			
			
			/////////////////////////


			if(drawLength>0 &&(
				( pMTL->m_textSyle & (unsigned int)SMarkupTextLine::UNDERLINE) != 0 || ( pMTL->m_textSyle & (unsigned int)SMarkupTextLine::OVERBAR) != 0)){
				CPen penNew;
				CPen* ppenOld;
				int penWidth = nVExt/nWExt;
				//描画モードによってペンの種類・色を設定する
				penNew.CreatePen(PS_SOLID,penWidth,col);
				ppenOld=pDC->SelectObject(&penNew);			//作成したペンの選択
				int lineY = currentY;
				if(( pMTL->m_textSyle & (unsigned int)SMarkupTextLine::UNDERLINE) != 0){
					lineY += 3 * nVExt/nWExt; 
				}else{
					lineY -= yPosSUP;
				}

				pDC->MoveTo(currentX,lineY);
				pDC->LineTo(currentX+sizeText.cx,lineY);
				pDC->SelectObject(ppenOld);
			}

			/////////////////////////
			
			if(sizeText.cx>0){
				lastlineUsed = true;
			}

			currentX += sizeText.cx;
			
			if(maxX < currentX){
				maxX = currentX;
			}

			if(lineBreak){
				lineCount++;
				currentY += lineHeight;
				currentX = baseX;
				lastlineUsed = false;
			}
			curLineTopOffset += drawLength;
		}

		pFont=pDC->SelectObject(pFont);					//フォントの選択
		delete pFont;
		pFont = NULL;
		ite++;
	}


	if(!lastlineUsed) lineCount--;
	if(lineCount<=0) lineCount = 1;

	int rw = maxX - baseX;
	int rh = lineCount * lineHeight;

	int w = rw * nWExt/ nVExt;
	int h = rh * nWExt/ nVExt;

	
	int w1l = w;
	int h1l = lineHeight * nWExt/ nVExt;


//		x=x*nVExt/nWExt;
//	y=y*nVExt/nWExt;
//		pComment->setDrawSize(COMMENT_Y_OFFSET+size.cy,CSize2SSize(size));
//		pComment->setDrawSize(COMMENT_Y_OFFSET+h,SSize(w,h));
		pComment->setDrawSize(SSize(w,h),SSize(w1l,h1l));
//		pComment->setDrawRealSize(SSize(rw,rh));
//	}


}



//void g_DrawComment(
//	CDC* pDC,						//描画先
//	SXBSchObj* pObj,				//描画オブジェクト
//	DWORD dwMode,					//描画モード
//	int  nVExt,						//表示倍率(View)
//	int  nWExt,						//表示倍率(Window)
//	LPCRECT lprcClip)				//クリップ情報			
//{
//	if(pObj->id()!=SXBSchObj::ID_COMMENT) return;
//	SXBSchComment* pComment =(SXBSchComment*)pObj;
//
//
//	double tilt = pComment->tiltVal();
//	int anchorVal =pComment->anchor();
//	tilt = fmod(tilt,M_PI*2);
//	if(tilt<0){
//		tilt+=M_PI*2;
//	}
//	
//
//	if(pComment->inhUpsidedown() &&
//	   anchorVal>=0 &&
//	   (tilt>(M_PI/2) && tilt<(M_PI/2)*3)){
//
//			if((anchorVal & SXBSchComment::ANCHOR_CENTER) ==0){
//				anchorVal ^= SXBSchComment::ANCHOR_RIGHT;
//			}
//			if((anchorVal & SXBSchComment::ANCHOR_MIDDLE) ==0){
//				anchorVal ^= SXBSchComment::ANCHOR_TOP;
//			}
//			tilt+=M_PI;
//	}
//
//
//
//	XFORM xform;
//	xform.eM11 = cos(-tilt);
//	xform.eM12 = sin(-tilt);
//	xform.eM21 = -xform.eM12;
//	xform.eM22 = xform.eM11;
//
//	if(!pComment->drawSizeIsValid()){
//		xform.eDx = 0;
//		xform.eDy = 0;
//		int dcInfo = pDC->SaveDC();
//		CDC dcMem; //メモリデバイスコンテキストオブジェクトの作成
//		dcMem.CreateCompatibleDC(pDC);	//クライアントエリアと互換性のあるメモリデバイスコンテキスト
//		int graphicsMode = dcMem.SetGraphicsMode(GM_ADVANCED);
//		dcMem.SetWorldTransform(&xform);
//		g_DrawComment_base(
//			&dcMem,
//			pComment,
//			1,
//			1,
//			1,
//			true,
//			NULL);
//		dcMem.RestoreDC(dcInfo);
//		dcMem.SetGraphicsMode(graphicsMode);
//	}
//
//	SPointF p1 = pComment->pf1();
//	SPoint tempP1 = SPoint(0,0);
//	pComment->setP1(tempP1);
//
//	int xp = p1.x();
//	int yp = p1.y();
//
//	int w = pComment->width();
//	if(w<0){
//		w = pComment->drawSize().w();
//	}
//
//	//int anchorVal =pComment->anchor();
//	if(anchorVal<0){
//	}else if(anchorVal & SXBSchComment::ANCHOR_CENTER){
//		xp -= w/2;
//	}else if(anchorVal & SXBSchComment::ANCHOR_RIGHT){
//		xp -= w;
//	}else{
//	}
//
//	int lineHeight = pComment->lineDrawSize().h();
//	int drawHeight = pComment->drawSize().h();
//
//	//TRACE("lineHeight  %d, drawHeight  %d\n",lineHeight,drawHeight);
//
//	if(anchorVal<0){
//	}else if(anchorVal & SXBSchComment::ANCHOR_TOP){
//		yp+= lineHeight;
//	}else if(anchorVal & SXBSchComment::ANCHOR_MIDDLE){
//		yp-=(drawHeight/2-lineHeight);
//	}else{
//		yp-=(drawHeight-lineHeight);
//	}
//
//
//	SPointF pdraw = SPointF(xp,yp);
//	pdraw.rotate(p1,tilt);
//
//
//	int xc = (int)Round((pdraw.x()*nVExt)/nWExt);
//	int yc = (int)Round((pdraw.y()*nVExt)/nWExt);
//
//
//
//	int graphicsMode = pDC->SetGraphicsMode(GM_ADVANCED);
//	//
//	//XFORM xform;
//	//xform.eM11 = cos(-tilt);
//	//xform.eM12 = sin(-tilt);
//	//xform.eM21 = -xform.eM12;
//	//xform.eM22 = xform.eM11;
//	xform.eDx = xc;
//	xform.eDy = yc;
//
//	int dcInfo = pDC->SaveDC();
//	pDC->SetWorldTransform(&xform);
//	g_DrawComment_base(
//		pDC,
//		pComment,
//		dwMode,
//		nVExt,
//		nWExt,
//		false,
//		NULL);
//	pComment->setPf1(p1);
//	pDC->RestoreDC(dcInfo);
//
//	pDC->SetGraphicsMode(graphicsMode);
//}




//コメントの描画
void g_DrawComment_base(
	CDC* pDC,						//描画先
	SXBSchObj* pObj,				//描画オブジェクト
	DWORD dwMode,					//描画モード
	int  nVExt,						//表示倍率(View)
	int  nWExt,						//表示倍率(Window)
	LPCRECT lprcClip)				//クリップ情報			
{
	if(pObj->id()!=SXBSchObj::ID_COMMENT) return;
	SXBSchComment* pComment =(SXBSchComment*)pObj;

	if(pComment->enableTag()){
		return g_DrawRichComment(
					pDC,					//描画先
					pComment,			//描画オブジェクト
					dwMode,				//描画モード
					nVExt,				//表示倍率(View)
					nWExt,				//表示倍率(Window)
					lprcClip);			//クリップ情報		
	}


	COLORREF col;
	CSize size;
	//描画色の決定
	if(dwMode & DRAW_DARK)		col=CBSchView::m_COL.colDARK;
	else if(dwMode & DRAW_TEMP)	col=CBSchView::m_COL.colXOR; 
	else						col=CBSchView::m_COL.colCOMMENT;

	LOGFONT lf;
	char* p=(char*)&lf;	//LOGFONT構造体の０イニシャライズ
	for(int n=0;n<sizeof(LOGFONT);n++) *p++=0;
	lf.lfHeight=-pComment->fontSize();
	lf.lfWeight=(pComment->fontBold() ? FW_BOLD : FW_REGULAR);
	lf.lfItalic=pComment->fontItalic();
	lf.lfCharSet=DEFAULT_CHARSET;
	lf.lfPitchAndFamily=0;
	_tcsncpy(lf.lfFaceName,pComment->fontName(),LF_FACESIZE);
	lf.lfFaceName[LF_FACESIZE-1]='\0';

	int width = pComment->width();
	if(width<0) width=100000;
	CSize size1Line;
	size=CBSchFont::PutWinFontML(
				pDC,
				pComment->p1().x(),
				pComment->p1().y()-COMMENT_Y_OFFSET,
				pComment->text(),
				-1,
				&lf,
				width,
				size1Line,
				col,dwMode,//false,0,
				nVExt,nWExt);

	//size=CBSchFont::PutWinFont(
	//			pDC,
	//			pComment->p1().x(),
	//			pComment->p1().y()-COMMENT_Y_OFFSET,
	//			pComment->text(),
	//			-1,
	//			&lf,
	//			col,dwMode,false,0,
	//			nVExt,nWExt);
	
	
//	if(nVExt==nWExt){
		int w = size.cx * nWExt/ nVExt;
		int h = size.cy * nWExt/ nVExt;

		int w1l = size1Line.cx * nWExt/ nVExt;
		int h1l = size1Line.cy * nWExt/ nVExt;


//		x=x*nVExt/nWExt;
//	y=y*nVExt/nWExt;
//		pComment->setDrawSize(COMMENT_Y_OFFSET+size.cy,CSize2SSize(size));
//		pComment->setDrawSize(COMMENT_Y_OFFSET+h,SSize(w,h));
		pComment->setDrawSize(SSize(w,h),SSize(w1l,h1l));
//		pComment->setDrawRealSize(SSize(size.cx,size.cy));
//	}
}

//コメントの描画
void g_DrawComment(
	CDC* pDC,						//描画先
	SXBSchObj* pObj,				//描画オブジェクト
	DWORD dwMode,					//描画モード
	int  nVExt,						//表示倍率(View)
	int  nWExt,						//表示倍率(Window)
	LPCRECT lprcClip)				//クリップ情報			
{
	if(pObj->id()!=SXBSchObj::ID_COMMENT) return;
	SXBSchComment* pComment =(SXBSchComment*)pObj;

	double rot;
//	double rot = (pComment->horizontal()? 0 : -M_PI/2);
	switch(pComment->dir()){
		case SXBSchComment::BSCHCOMMENT_DIR::COMMENT_DIR_0:
			rot = 0.0; break;
		case SXBSchComment::BSCHCOMMENT_DIR::COMMENT_DIR_1:
			rot = M_PI/2; break;
		case SXBSchComment::BSCHCOMMENT_DIR::COMMENT_DIR_2:
			rot = M_PI; break;
		//case SXBSchComment::BSCHCOMMENT_DIR::COMMENT_DIR_3:
		default:
			rot = -M_PI/2;
	}





	int xp = pComment->p1().x();
	int yp = pComment->p1().y();


	XFORM xform;
	xform.eM11 = cos(rot);
	xform.eM12 = sin(rot);
	xform.eM21 = -xform.eM12;
	xform.eM22 = xform.eM11;
	xform.eDx = (xp * nVExt)/nWExt;
	xform.eDy = (yp * nVExt)/nWExt;
	int dcInfo = pDC->SaveDC();
	int graphicsMode = pDC->SetGraphicsMode(GM_ADVANCED);

	pDC->SetWorldTransform(&xform);


	pComment->setP1(SPoint(0,0));


	g_DrawComment_base(
		pDC,						//描画先
		pObj,				//描画オブジェクト
		dwMode,					//描画モード
		nVExt,						//表示倍率(View)
		nWExt,						//表示倍率(Window)
		lprcClip);

	pComment->setP1(SPoint(xp,yp));

	pDC->RestoreDC(dcInfo);
	pDC->SetGraphicsMode(graphicsMode);

}




void g_DrawTag(
	CDC* pDC,						//描画先
	SXBSchObj* pObj,					//描画オブジェクト
	DWORD dwMode,					//描画モード
	int nVExt/*=1*/,				//表示倍率(View)
	int nWExt/*=1*/,				//表示倍率(Window)
	LPCRECT lprcClip /*=NULL*/)		//クリップ情報
{
	if(pObj->id()!=SXBSchObj::ID_TAG) return;
	SXBSchTag* pTag =(SXBSchTag*)pObj; 

	int nWidth;			//タグの幅,図面座標での値。
	int nStrLen;		//文字数
	CPoint ptary[6];	//頂点座標
	int nPoint;			//頂点数
	int n;
//	int x = pTag->GetPoint().x;
//	int y = pTag->GetPoint().y;

	const TCHAR* pcsz = pTag->text();
	//文字数を得る
	if(pcsz) nStrLen=lstrlen(pcsz);
	else     nStrLen=0;
	//幅を得る
	//nWidth=pTag->width();

	int framex,framey,frameh,framew;
	int textx,texty,textw;

	framex = (pTag->x1()*nVExt)/nWExt;
	framey = (pTag->y1()*nVExt)/nWExt;
	//hとwは水平のときの値。垂直なら入れ替わる。
	frameh = (10*nVExt)/nWExt/2;
	framew = (pTag->width()*nVExt)/nWExt;
	nWidth = pTag->width();;
	int linewidth = 1*nVExt/nWExt;

	textw = _tcslen(pTag->text())*8;
	

	if(pTag->horizontal()){
		switch(pTag->tagType()){
		case SXBSchTag::TAG_SQ:
			nPoint=4;
			ptary[0]=CPoint(framex,			framey-frameh);
			ptary[1]=CPoint(framex + framew,	framey-frameh);
			ptary[2]=CPoint(framex + framew,	framey+frameh);
			ptary[3]=CPoint(framex,			framey+frameh);
			break;
		case SXBSchTag::TAG_BI:
			nPoint=6;
			ptary[0]=CPoint(framex,					framey		 );
			ptary[1]=CPoint(framex + frameh,			framey-frameh);
			ptary[2]=CPoint(framex + framew - frameh,framey-frameh);
			ptary[3]=CPoint(framex + framew,			framey		 );
			ptary[4]=CPoint(framex + framew - frameh,framey+frameh);
			ptary[5]=CPoint(framex + frameh,			framey+frameh);
			break;
		case SXBSchTag::TAG_LU:
			nPoint=5;
			ptary[0]=CPoint(framex,					framey		 );
			ptary[1]=CPoint(framex + frameh,			framey-frameh);
			ptary[2]=CPoint(framex + framew,			framey-frameh);
			ptary[3]=CPoint(framex + framew,			framey+frameh);
			ptary[4]=CPoint(framex + frameh,			framey+frameh);
			break;
		case SXBSchTag::TAG_RD:
			nPoint=5;
			ptary[0]=CPoint(framex,					framey-frameh);
			ptary[1]=CPoint(framex + framew - frameh,framey-frameh);
			ptary[2]=CPoint(framex + framew,			framey		 );
			ptary[3]=CPoint(framex + framew - frameh,framey+frameh);
			ptary[4]=CPoint(framex,					framey+frameh);
			break;
		}
	}else{
		switch(pTag->tagType()){
		case SXBSchTag::TAG_SQ:
			nPoint=4;
			ptary[0]=CPoint(framex - frameh,	framey		 );
			ptary[1]=CPoint(framex - frameh,	framey-framew);
			ptary[2]=CPoint(framex + frameh,	framey-framew);
			ptary[3]=CPoint(framex + frameh,	framey		 );
			break;
		case SXBSchTag::TAG_BI:
			nPoint=6;
			ptary[0]=CPoint(framex,			framey					);
			ptary[1]=CPoint(framex - frameh,	framey -frameh			);
			ptary[2]=CPoint(framex - frameh,	framey -framew + frameh	);
			ptary[3]=CPoint(framex,			framey -framew			);
			ptary[4]=CPoint(framex + frameh,	framey -framew + frameh	);
			ptary[5]=CPoint(framex + frameh,	framey -frameh			);
			break;
		case SXBSchTag::TAG_LU:
			nPoint=5;
			ptary[0]=CPoint(framex - frameh,	framey					);
			ptary[1]=CPoint(framex - frameh,	framey -framew + frameh	);
			ptary[2]=CPoint(framex,			framey -framew			);
			ptary[3]=CPoint(framex + frameh,	framey -framew + frameh	);
			ptary[4]=CPoint(framex + frameh,	framey					);
			break;
		case SXBSchTag::TAG_RD:
			nPoint=5;
			ptary[0]=CPoint(framex,			framey			);
			ptary[1]=CPoint(framex - frameh,	framey -frameh	);
			ptary[2]=CPoint(framex - frameh,	framey-framew	);
			ptary[3]=CPoint(framex + frameh,	framey-framew	);
			ptary[4]=CPoint(framex + frameh,	framey -frameh	);
			break;
		}
	}






//	//枠の頂点の座標を得る
//	DWORD dwType = pTag->GetType();
//	if(nVExt!=nWExt || dwType==TAG_SQ){ //等倍以外では四角形で表示
//		nPoint=4;
//		ptary[0].x=x;        ptary[0].y=y-5; 
//		ptary[1].x=x+nWidth; ptary[1].y=y-5; 
//		ptary[2].x=x+nWidth; ptary[2].y=y+5;
//		ptary[3].x=x;        ptary[3].y=y+5;
//	}else if(dwType==TAG_R){
//		nPoint=5;
//		ptary[0].x=x;          ptary[0].y=y-5; 
//		ptary[1].x=x+nWidth-5; ptary[1].y=y-5; 
//		ptary[2].x=x+nWidth;   ptary[2].y=y;
//		ptary[3].x=x+nWidth-5; ptary[3].y=y+5; 
//		ptary[4].x=x;          ptary[4].y=y+5;
//	}else if(dwType==TAG_L){
//		nPoint=5;
//		ptary[0].x=x+nWidth;   ptary[0].y=y-5; 
//		ptary[1].x=x+5;        ptary[1].y=y-5; 
//		ptary[2].x=x;          ptary[2].y=y;
//		ptary[3].x=x+5;        ptary[3].y=y+5; 
//		ptary[4].x=x+nWidth;   ptary[4].y=y+5;
//	}else{
//		nPoint=6;
//		ptary[0].x=x+5;        ptary[0].y=y-5; 
//		ptary[1].x=x;          ptary[1].y=y;
//		ptary[2].x=x+5;        ptary[2].y=y+5; 
//		ptary[3].x=x+nWidth-5; ptary[3].y=y+5; 
//		ptary[4].x=x+nWidth;   ptary[4].y=y;
//		ptary[5].x=x+nWidth-5; ptary[5].y=y-5; 
//	}

//	//座標を論理座標に変換
//	for(n=0;n<nPoint;n++){
//		ptary[n].x=(ptary[n].x * nVExt)/nWExt;
//		ptary[n].y=(ptary[n].y * nVExt)/nWExt;
//	}
	int nOldDrawMode;
	CPen penNew;
	CPen* ppenOld;
	COLORREF col;
	//描画モードによってペンの種類・色とCOPY or XORを設定する
	if(dwMode & DRAW_DARK){
		col = CBSchView::m_COL.colDARK;
//		penNew.CreatePen(PS_SOLID,0,COL_DARK);
//		nNewDrawMode=R2_COPYPEN;
	}else if(dwMode & DRAW_ON){
		col =CBSchView::m_COL.colTAG;
//		penNew.CreatePen(PS_SOLID,0,COL_TAG);
//		nNewDrawMode=R2_COPYPEN;
	}else if(dwMode & DRAW_OFF){
		col =RGB(0,0,0);
//		penNew.CreatePen(PS_SOLID,0,RGB(0,0,0));
//		nNewDrawMode=R2_COPYPEN;
	}else if(dwMode & DRAW_TEMP){
		col =CBSchView::m_COL.colXOR;
//		penNew.CreatePen(PS_SOLID,0,COL_XOR);
//		nNewDrawMode=R2_COPYPEN;
		//nNewDrawMode=R2_XORPEN;
	}else{
		TRACE("\n不正な描画モード(CBSchTag::Draw())");
		return;
	}

	penNew.CreatePen(PS_SOLID,linewidth,col);

	ppenOld=pDC->SelectObject(&penNew);			//作成したペンの選択
	nOldDrawMode=pDC->SetROP2(R2_COPYPEN);		//描画モードの設定
	pDC->MoveTo(ptary[0]);						//始点へ移動
	for(n=1;n<nPoint;n++) pDC->LineTo(ptary[n]);//描線
	pDC->LineTo(ptary[0]);									
	pDC->SetROP2(nOldDrawMode);					//描画モードの復帰
	pDC->SelectObject(ppenOld);					//ペンの復帰

	//文字の描画


	//タグ文字列の描画
	if(!(dwMode & DRAW_FAST)){
		COLORREF col;
		//int x0,y0;
		//描画色の決定
		if(dwMode & DRAW_DARK)			col=CBSchView::m_COL.colDARK;
		else if(dwMode & DRAW_TEMP)		col=CBSchView::m_COL.colXOR;
		else if(dwMode & DRAW_OFF)		col=RGB(0,0,0);
		else							col=CBSchView::m_COL.colTAG_NAME;
		//座標の設定
		if(pTag->horizontal()){
			if(nVExt>=nWExt){
				//TRACE("PTAG %d,%d\n",pTag->x1(),nWidth);
				textx = pTag->x1()+nWidth/2;
				texty = pTag->y1()+3;
				//g_SmallFont.DrawText(p,textx,texty,pTag->text(),colText,nMode,false);
				g_pFont->PutFont(pDC,textx,texty,pcsz,nStrLen,col,dwMode,FALSE,2,nVExt,nWExt,FALSE);
			}
		}else{
			if(nVExt>=nWExt){
				textx = pTag->x1()+3;
				texty = pTag->y1()-nWidth/2;
				//g_SmallFont.DrawText(p,textx,texty,pTag->text(),colText,nMode,true);
				g_pFont->PutFont(pDC,textx,texty,pcsz,nStrLen,col,dwMode,TRUE,2,nVExt,nWExt,FALSE);
			}
		}
	}
}

void g_DrawImageWithGdiPlus(	
	CDC* pDC,//描画先
	int x,
	int y,
	int w,
	int h,
	byte* imageBuff,
	int imageBuffSize)
{
	CImage image;

	CComPtr<IStream> pStream;
	pStream = NULL;

	HGLOBAL hMem = ::GlobalAlloc(GHND, imageBuffSize); 
	LPVOID pGBuff = ::GlobalLock(hMem);
	memcpy(pGBuff, imageBuff, imageBuffSize);
	::GlobalUnlock(hMem);

	CreateStreamOnHGlobal(hMem, TRUE, &pStream);

	HRESULT res = image.Load(pStream);
	if(!FAILED(res)){
		image.StretchBlt(pDC->GetSafeHdc(),x,y,w,h);
	}
	image.Destroy();
	::GlobalFree(hMem);
}


void g_DrawImageObj(
	CDC* pDC,//描画先
	SXBSchObj* pObj,
	
	DWORD dwMode,				//描画モード
	int nVExt,					//表示倍率(View)
	int nWExt,					//表示倍率(Window)
	LPCRECT lprcClip)			//クリップ情報
{
	if(!pObj) return;
	int x,y,w,h,dw,dh,mag;

	SXBSchImageObj* pImageObj = (SXBSchImageObj*)pObj;
	
	w=pImageObj->ImageWidth();
	h=pImageObj->ImageHeight();
	if(w==0||h==0) return;
	mag = pImageObj->DrawMagnification();
	dw = ((w*mag)/100)*nVExt/nWExt;
	dh = ((h*mag)/100)*nVExt/nWExt;
	
	const SImageDIB* pImageDIB = (SImageDIB*)(pImageObj->ImageData());

	x=(pImageObj->p1().x() * nVExt)/nWExt;
	y=(pImageObj->p1().y() * nVExt)/nWExt;

	if(dwMode & DRAW_DARK){
		CRect rc = CRect(CPoint(x,y),CSize(dw,dh));
		CBitmap bmp;
		bmp.LoadBitmap(IDB_IMAGE_PTN);
		CPen pen(PS_SOLID,1,CBSchView::m_COL.colDARK);
		CPen* pPenOld = pDC->SelectObject(&pen);
		CBrush brush(&bmp);
		CBrush* pOldBrush = pDC->SelectObject(&brush);
		COLORREF oldTextColor = pDC->SetTextColor(CBSchView::m_COL.colDARK);
		pDC->Rectangle(&rc);
		pDC->SetTextColor(oldTextColor);
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pPenOld);
	}else{
		int oldStretchMode = ::SetStretchBltMode(pDC->GetSafeHdc(),HALFTONE);
		::SetBrushOrgEx(pDC->GetSafeHdc(),x, y,NULL);


		if(pImageDIB->m_pBmpIH->biCompression == BI_JPEG || pImageDIB->m_pBmpIH->biCompression == BI_PNG){
			if(g_bAvailableGDIplus){
				g_DrawImageWithGdiPlus(pDC,x,y,dw,dh,pImageDIB->m_pbuff+54,pImageDIB->m_pBmpIH->biSizeImage);
			}else{
				CRect rc = CRect(CPoint(x,y),CSize(dw,dh));
				CPen pen(PS_SOLID,1,RGB(192,192,192));
				CPen* pPenOld = pDC->SelectObject(&pen);
				CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
				pDC->Rectangle(&rc);
				pDC->SelectObject(pOldBrush);
				pDC->SelectObject(pPenOld);
			}
		}else{
			::StretchDIBits(pDC->GetSafeHdc() ,x, y, dw, dh,0, 0, w, h, pImageDIB->m_pBitData,pImageDIB->m_pBmpInfo,DIB_RGB_COLORS, SRCCOPY);
		}

		SetStretchBltMode(pDC->GetSafeHdc(),oldStretchMode);


		if(dwMode & DRAW_TEMP){
			CBitmap bmp;
			bmp.LoadBitmap(IDB_IMAGE_SEL_PTN);
			CBrush brush(&bmp);
			CBrush* pOldBrush = pDC->SelectObject(&brush);
			COLORREF oldTextColor = pDC->SetTextColor(CBSchView::m_COL.colXOR);
			//pDC->FillRect(&rc,&brush);
			//pDC->Rectangle(&rc);

				// [P]   1   1   1   1   0   0   0   0
				// [S]   1   1   0   0   1   1   0   0
				// [D]   1   0   1   0   1   0   1   0
				// -------------------------------------
				//       1   0   1   0   0   0   0   0  ----> 0xA0  00A000C9

			pDC->BitBlt(x,y,dw,dh,NULL,0,0,0x00A000C9);
			pDC->SetTextColor(oldTextColor);
			pDC->SelectObject(pOldBrush);
		}

	}
}



//部品胴部の描画
void g_DrawComponentBody(
	CDC* pDC,						//描画先
	SXBSchComponent* pObj,
	DWORD dwMode,					//描画モード
	int nVExt,				//表示倍率(View)
	int nWExt)				//表示倍率(Window)
{
	if(!pObj) return;

	SSize size=pObj->size();

	int x,y,l,t,r,b,w,h;

	w = pObj->size().w();
	h = pObj->size().h();
	x=pObj->p1().x()-w;
	y=pObj->p1().y()-h;
	//胴部の上下左右の座標
	l=(x*nVExt)/nWExt;							//描画左座標
	t=(y*nVExt)/nWExt;							//描画上座標
	r=(pObj->p1().x()*nVExt)/nWExt;			//描画右座標
	b=(pObj->p1().y()*nVExt)/nWExt;			//描画下座標

	int nOldDrawMode;
	COLORREF col;
	const SCompIndex* compInfo=pObj->compInfoIndex();
	const SPtn* pPtn = NULL;
	if(compInfo) pPtn =compInfo->pattern();
	
	//胴部矩形の描画
	//パターンが存在しない場合か、（パターンが存在しても）枠描画指定があるときは矩形を描画
	if(pPtn==NULL || dwMode & DRAW_PARTFRAME){	//g_bDisplayPartFrame!=FALSE){
		CPen penNew;
		CPen* ppenOld;

		int width;
		if(pPtn==NULL)	width=1;
		else			width=0;
		width = width*nVExt/nWExt;

		if(dwMode & DRAW_DARK){
			col=CBSchView::m_COL.colDARK;
//			penNew.CreatePen(PS_SOLID,0,COL_DARK);
//			nNewDrawMode=R2_COPYPEN;
		}else if(dwMode & DRAW_ON){
			if(pPtn==NULL) col=CBSchView::m_COL.colPART; else col=CBSchView::m_COL.colPART_FRAME;
//			penNew.CreatePen(PS_SOLID,0,COL_PART_FRAME);
//			nNewDrawMode=R2_COPYPEN;
		}else if(dwMode & DRAW_OFF){
			col=RGB(0,0,0);
//			penNew.CreatePen(PS_SOLID,0,RGB(0,0,0));
//			nNewDrawMode=R2_COPYPEN;
		}else if(dwMode & DRAW_TEMP){
			col=CBSchView::m_COL.colXOR;
//			penNew.CreatePen(PS_SOLID,0,COL_XOR);
//			//nNewDrawMode=R2_XORPEN;
//			nNewDrawMode=R2_COPYPEN;
		}else{
			TRACE("\n不正な描画モード(CBSchPart::DrawBody())");
			return;
		}
		penNew.CreatePen(PS_SOLID,width,col);
		ppenOld=pDC->SelectObject(&penNew);			//作成したペンの選択
		nOldDrawMode=pDC->SetROP2(R2_COPYPEN);		//描画モードの設定
		pDC->MoveTo(l,t);
		pDC->LineTo(r,t); pDC->LineTo(r,b);			//４辺の描画
		pDC->LineTo(l,b); pDC->LineTo(l,t);
		pDC->SetROP2(nOldDrawMode);					//描画モードの復帰
		pDC->SelectObject(ppenOld);					//ペンの復帰
	}
	if(pPtn!=NULL){	//パターンが存在
		if(dwMode & DRAW_DARK){
			col=CBSchView::m_COL.colDARK;
		}else if(dwMode & DRAW_ON){
			col=CBSchView::m_COL.colPART;
		}else if(dwMode & DRAW_OFF){
			col=RGB(0,0,0);
		}else if(dwMode & DRAW_TEMP){
			col=CBSchView::m_COL.colXOR;
		}else{
			TRACE("\n不正な描画モード(CBSchPart::DrawBody())");
			return;
		}

		if(pPtn->m_vPtnCount>0 && pPtn->m_ppvPtn!=NULL){
			SSize ssize = compInfo->size();
			int cx = ssize.w()*10;
			int cy = ssize.h()*10;
			int i=0;
			int retv;
			while(i<pPtn->m_vPtnCount){
				SPtnObj* pPtnObj=pPtn->m_ppvPtn[i];
				retv=1;
				switch(pPtnObj->id()){
					case VECTPTN_L:
						g_DrawPtnLine(pDC,(SPtnObjLine*)pPtnObj,col,pObj->dir(),cx,cy,x,y,nVExt,nWExt);
						break;
					case VECTPTN_C:
						g_DrawPtnCircle(pDC,(SPtnObjCircle*)pPtnObj,col,pObj->dir(),cx,cy,x,y,nVExt,nWExt);
						break;
					case VECTPTN_PG:
						g_DrawPtnPolygon(pDC,(SPtnObjPolygon*)pPtnObj,col,pObj->dir(),cx,cy,x,y,nVExt,nWExt);
						break;
					case VECTPTN_AR:
						g_DrawPtnArc(pDC,(SPtnObjArc*)pPtnObj,col,pObj->dir(),cx,cy,x,y,nVExt,nWExt);
						break;
					case VECTPTN_TXT:
						g_DrawPtnText(pDC,(SPtnObjText*)pPtnObj,col,pObj->dir(),cx,cy,x,y,nVExt,nWExt);
						break;
					default:
						retv=0;
				}
				if(retv==0) break;
				i++;
			}
		}else if(pPtn->m_pBuff){
			//if(pPtn->m_pBuff!=NULL){ //Version 0.52  コメントアウト
				g_Pattern.PutPtn(pDC,x,y,
					pPtn->m_pBuff,
					pPtn->m_size.w(),
					pPtn->m_size.h(),
					col,dwMode,
					pObj->dir()&0x03,	//Rotate
					pObj->dir()&0x04,	//Mirror
					nVExt,nWExt);
			//}	 //Version 0.52 コメントアウト
		}
	}
}


CRect g_drawComponentPinArea(const SPoint& pt,int nLtrb)
{
	int l,t,w,h;
	switch(nLtrb){
	//case PIN_LOC_L:	l=pt.x();	t=pt.y()-4;		w=16;		h=9;		break;
	//case PIN_LOC_T:	l=pt.x()-4;	t=pt.y();		w=9;		h=16;		break;
	//case PIN_LOC_R:	l=pt.x()-16;t=pt.y()-4;		w=16;		h=9;		break;
	//default:		l=pt.x()-4;	t=pt.y()-16;	w=9;		h=16;		break;
	case PIN_LOC_L:	l=pt.x()-10;	t=pt.y()-4;			w=16;		h=9;		break;
	case PIN_LOC_T:	l=pt.x()-4;		t=pt.y()-10;		w=9;		h=16;		break;
	case PIN_LOC_R:	l=pt.x()-6;		t=pt.y()-4;			w=16;		h=9;		break;
	default:		l=pt.x()-4;		t=pt.y()-6;			w=9;		h=16;		break;
	}
	return CRect(l,t,l+w,t+h);
}
	
CRect g_drawComponentPinNameArea(const SPoint& pt,int nLtrb,const TCHAR* name)
{
	if(name == NULL || *name == '\0') return CRect();
	int len = _tcslen(name)*8;
	int l,t,w,h;
	switch(nLtrb){
	//case PIN_LOC_L:	l=pt.x()+15;		t=pt.y()-7;			w=len;	h=10;	break;
	//case PIN_LOC_T:	l=pt.x()-7;			t=pt.y()+15;		w=10;	h=len;	break;
	//case PIN_LOC_R:	l=pt.x()-15-len;	t=pt.y()-7;			w=len;	h=10;	break;
	//default:		l=pt.x()-7;			t=pt.y()-15-len;	w=10;	h=len;	break;
	case PIN_LOC_L:	l=pt.x()+5;			t=pt.y()-7;			w=len;	h=10;	break;
	case PIN_LOC_T:	l=pt.x()-7;			t=pt.y()+5;			w=10;	h=len;	break;
	case PIN_LOC_R:	l=pt.x()-5-len;		t=pt.y()-7;			w=len;	h=10;	break;
	default:		l=pt.x()-7;			t=pt.y()-5-len;		w=10;	h=len;	break;
	}
	return CRect(l,t,l+w,t+h);
}


CRect g_drawComponentPinNumArea(const SPoint& pt,int nLtrb,const TCHAR* name,int pintype)
{
	if(name == NULL || *name == '\0') return CRect();
	int nOffset;
	int len = _tcslen(name)*8;
	int l,t,w,h;

	if(pintype & PIN_TYPE_SMALL || pintype & PIN_TYPE_ZLENG){
		nOffset = 2;
	}else{
		nOffset = 9;
	}
	
	switch(nLtrb){
	//case PIN_LOC_L:	l=pt.x()+10-nOffset-len;	t=pt.y()-10;				w=len;	h=8;	break;
	//case PIN_LOC_T:	l=pt.x()-10;				t=pt.y()+10-nOffset-len;	w=8;	h=len;	break;
	//case PIN_LOC_R:	l=pt.x()-10+nOffset;		t=pt.y()-10;				w=len;	h=8;	break;
	//default:		l=pt.x()-10;				t=pt.y()-10+nOffset;		w=8;	h=len;	break;
	case PIN_LOC_L:	l=pt.x()-nOffset-len;		t=pt.y()-10;				w=len;	h=8;	break;
	case PIN_LOC_T:	l=pt.x()-10;				t=pt.y()-nOffset-len;		w=8;	h=len;	break;
	case PIN_LOC_R:	l=pt.x()+nOffset;			t=pt.y()-10;				w=len;	h=8;	break;
	default:		l=pt.x()-10;				t=pt.y()+nOffset;			w=8;	h=len;	break;
	}
	return CRect(l,t,l+w,t+h);
}

void g_drawComponentPinNumName(	CDC* pDC,
								const CRect& rc,
								int nLtrb,
								DWORD dwMode,
								COLORREF col,
								const TCHAR* name,
								int nVExt,						//表示倍率(View)
								int nWExt)						//表示倍率(Window)
{
	int x;
	int y;
	int align;
	if(nLtrb==0){			//left
		x=rc.left; 
		y=rc.bottom;
		align = 0;
	}else if(nLtrb==1){		//top
		x=rc.right;
		y=rc.top;
		align = 1;
	}else if(nLtrb==2){		//right
		x=rc.right;
		y=rc.bottom;
		align = 1;
	}else{					//bottom
		x=rc.right;
		y=rc.bottom;
		align = 0;
	}

//	if(nLtrb==0||nLtrb==2){
//		x=rc.left; y=rc.bottom;
//	}else{
//		x=rc.right; y=rc.bottom;
//	}
////	g_SmallFont.DrawText(p,x,y,name,colText,nMode,nLtrb&1);
//
//	if(nLtrb==1||nLtrb==2){
//		align = 1;
//	}else{
//		align = 0;
//	}
	g_pFont->PutFont(pDC,x,y,name,-1,col,dwMode,nLtrb&1,align,nVExt,nWExt);
}



//	PIN_TYPE_NOMAL		=0x00,
//	PIN_TYPE_SMALL		=0x01,
//	PIN_TYPE_CLOCK		=0x04,
//	PIN_TYPE_NEGATIVE	=0x08,
//	PIN_TYPE_NUMHIDE	=0x10


//ピンの描画
void g_drawComponentPinBody(	CDC* pDC,
				   const SPoint& sptPin,
				   int nLtrb,
				   int nType,
				   DWORD dwMode,
				   int nVExt,						//表示倍率(View)
				   int nWExt)						//表示倍率(Window)
{
	COLORREF col;
	CPen penNew;
	CPen* ppenOld;
	int nOldDrawMode;
	int x0 = sptPin.x();
	int y0 = sptPin.y();
	int x1,y1;
	int x2,y2;

	int width;
	if(nType & PIN_TYPE_ZLENG){
		if(nVExt<nWExt) return;
		width = 0;
	}else{
		width = 1*nVExt/nWExt;
	}
	//描画モードによってペンの種類・色とCOPY or XORを設定する
	if(dwMode & DRAW_DARK){
		col = CBSchView::m_COL.colDARK;
//		penNew.CreatePen(PS_SOLID,0,COL_DARK);
	}else if(dwMode & DRAW_ON){
		if(nType & PIN_TYPE_ZLENG){
			col = CBSchView::m_COL.colZLPIN;
		}else{
			col = CBSchView::m_COL.colPART_PIN;
		}
//		penNew.CreatePen(PS_SOLID,0,COL_PART);
	}else if(dwMode & DRAW_OFF){
		col = RGB(0,0,0);
//		penNew.CreatePen(PS_SOLID,0,RGB(0,0,0));
	}else if(dwMode & DRAW_TEMP){
		col = CBSchView::m_COL.colXOR;
//		penNew.CreatePen(PS_SOLID,0,COL_XOR);
	}else{
		TRACE("\n不正な描画モード(CBSchPart::DrawPin())");
		return;
	}

	penNew.CreatePen(PS_SOLID,width,col);

	ppenOld=pDC->SelectObject(&penNew);			//作成したペンの選択
	nOldDrawMode=pDC->SetROP2(R2_COPYPEN);		//描画モードの設定

	if(nType & PIN_TYPE_ZLENG){
		if((dwMode & DRAW_FOR_PRINT)==0){
			CBrush* pBrush=(CBrush*)pDC->SelectStockObject(NULL_BRUSH);
			x1=((x0-2)*nVExt)/nWExt;
			y1=((y0-2)*nVExt)/nWExt;
			x2=((x0+2)*nVExt)/nWExt+1;
			y2=((y0+2)*nVExt)/nWExt+1;
			pDC->Rectangle(x1,y1,x2,y2);
			pDC->SelectObject(pBrush);
		}
	}else if((nType & PIN_TYPE_NEGATIVE)==0){
        x1=((PinDataNormal[nLtrb][0]+x0)*nVExt)/nWExt;
        y1=((PinDataNormal[nLtrb][1]+y0)*nVExt)/nWExt;
		pDC->MoveTo(x1,y1);
        x2=((PinDataNormal[nLtrb][2]+x0)*nVExt)/nWExt;
        y2=((PinDataNormal[nLtrb][3]+y0)*nVExt)/nWExt;
		if(g_bFillEndOfPtnLine){	// 2007/06/10
			if(nVExt==nWExt){
				if(x1==x2){//垂直線
					if(y2<y2) y2++;
					else if(y1>y2) y2--;
				}else if(y1==y2){//水平線
					if(x1<x2) x2++;
					else if(x1>x2) x2--;
				}
			}
		}
		pDC->LineTo(x2,y2);
	}else{
		CBrush* pBrush=(CBrush*)pDC->SelectStockObject(NULL_BRUSH);
        x1=((PinDataInvert[nLtrb][0]+x0)*nVExt)/nWExt;
        y1=((PinDataInvert[nLtrb][1]+y0)*nVExt)/nWExt;
		pDC->MoveTo(x1,y1);
        x1=((PinDataInvert[nLtrb][2]+x0)*nVExt)/nWExt;
        y1=((PinDataInvert[nLtrb][3]+y0)*nVExt)/nWExt;
		pDC->LineTo(x1,y1);
        x1=((PinDataInvert[nLtrb][4]+x0)*nVExt)/nWExt;
        y1=((PinDataInvert[nLtrb][5]+y0)*nVExt)/nWExt;
        x2=((PinDataInvert[nLtrb][6]+x0)*nVExt)/nWExt+1;
        y2=((PinDataInvert[nLtrb][7]+y0)*nVExt)/nWExt+1;
		pDC->Ellipse(x1,y1,x2,y2);
		pDC->SelectObject(pBrush);
	}
	if(nType & PIN_TYPE_CLOCK){
        x1=((PinDataClcok[nLtrb][0]+x0)*nVExt)/nWExt;
        y1=((PinDataClcok[nLtrb][1]+y0)*nVExt)/nWExt;
		pDC->MoveTo(x1,y1);
        x1=((PinDataClcok[nLtrb][2]+x0)*nVExt)/nWExt;
        y1=((PinDataClcok[nLtrb][3]+y0)*nVExt)/nWExt;
		pDC->LineTo(x1,y1);
        x1=((PinDataClcok[nLtrb][4]+x0)*nVExt)/nWExt;
        y1=((PinDataClcok[nLtrb][5]+y0)*nVExt)/nWExt;
		pDC->LineTo(x1,y1);
	}
	pDC->SetROP2(nOldDrawMode);					//描画モードの復帰
	pDC->SelectObject(ppenOld);					//ペンの復帰
}

//NCマークの描画
void	g_drawNcMark(	
	CDC* pDC,
	const SPoint& pt,						//位置
	DWORD dwMode,					//描画モード
	int nVExt,					//表示倍率(View)
	int nWExt,					//表示倍率(Window)
	LPCRECT)					//クリップ情報
{
	COLORREF col;
	if(dwMode & DRAW_OFF) col=RGB(0,0,0);
	else if(dwMode & DRAW_DARK) col=CBSchView::m_COL.colDARK;
	else col=CBSchView::m_COL.colNC_MARK;

	int width = 1*nVExt/nWExt;

	int nPadding = (width<=1? 1:0);

	int x0 = pt.x();
	int y0 = pt.y();
	int x1,y1;
	int x2,y2;
	CPen pen;
	CPen* pPen;
	int nOldDrawMode;

	pen.CreatePen(PS_SOLID,width,col);
	pPen=pDC->SelectObject(&pen);			//作成したペンの選択
	nOldDrawMode=pDC->SetROP2(R2_COPYPEN);		//描画モードの設定

	x1=((x0-4)*nVExt)/nWExt;
	y1=((y0-4)*nVExt)/nWExt;
	x2=((x0+4)*nVExt)/nWExt;
	y2=((y0+4)*nVExt)/nWExt;

	pDC->MoveTo(x1,y1);
	pDC->LineTo(x2+nPadding,y2+nPadding);
	pDC->MoveTo(x1,y2);
	pDC->LineTo(x2+nPadding,y1-nPadding);

	pDC->SetROP2(nOldDrawMode);		//描画モードの設定
	pDC->SelectObject(pPen);		//作成したペンの選択

}

void g_drawComponentPin(
	CDC* pDC,						//描画先
	const SPoint& pinLoc,
	int	pinType,
	int nLtrb,
	LPCTSTR pszPinName,				//ピン名
	LPCTSTR pszPinNum,				//ピン番号
	DWORD dwMode,					//描画モード
	int nVExt,						//表示倍率(View)
	int nWExt,						//表示倍率(Window)
	LPCRECT lprcClip)				//クリップ情報

{
	COLORREF col;
	BOOL  bBodyOnly=(dwMode & DRAW_FAST)||(nVExt<nWExt);//これがTRUEのときピン名、ピン番号を描画しない
	CRect rc;
	CRect rcIntersect;
	//bool fDrawBody=false;
	
	//ピン本体の描画
	//  ・ビットマップピンを使う場合
	//  ・線描のピンを使う場合

	rc = g_drawComponentPinArea(pinLoc,nLtrb);

	if( !lprcClip || rcIntersect.IntersectRect(&rc,lprcClip)){
		g_drawComponentPinBody(
					pDC,
					pinLoc,
					nLtrb,
					pinType,
					dwMode,
					nVExt,						//表示倍率(View)
					nWExt);						//表示倍率(Window)

	}

	///////////////////////////////////////////////////////////////////////////
	// DRAW_FASTモードや縮小描画モードではピン本体の描画だけなのでここで終了 // 
	///////////////////////////////////////////////////////////////////////////
	if(bBodyOnly) return;	//戻り

//	BOOL bPointFront;	//指定座標が文字列の前のときTRUE
//	BOOL bVert;			//垂直のときTRUE;
	//ピン名の描画
	if(pszPinName!=NULL){
		CRect rc=g_drawComponentPinNameArea(pinLoc,nLtrb,pszPinName);
		if(!lprcClip || rcIntersect.IntersectRect(&rc,lprcClip)){
			if(dwMode & DRAW_DARK) col=CBSchView::m_COL.colDARK;
			else if(dwMode & DRAW_TEMP) col=CBSchView::m_COL.colXOR;
			else if(dwMode & DRAW_OFF)	col=RGB(0,0,0);
			else col=CBSchView::m_COL.colPART_PINNAME;
			g_drawComponentPinNumName(pDC,rc,nLtrb,dwMode,col,pszPinName,nVExt,nWExt);
		}
	}

	//ピン番号の描画
//	int nPinOffset;
	if(((pinType & PIN_TYPE_NUMHIDE) ==0)||g_bDisplayHiddenPinNum){
		if(pszPinNum!=NULL){
			//描画色の決定
			CRect rc=g_drawComponentPinNumArea(pinLoc,nLtrb,pszPinNum,pinType);
			if(!lprcClip || rcIntersect.IntersectRect(&rc,lprcClip)){
				if(dwMode & DRAW_DARK)		col=CBSchView::m_COL.colDARK;
				else if(dwMode & DRAW_TEMP)	col=CBSchView::m_COL.colXOR; 
				else if(dwMode & DRAW_OFF)	col=RGB(0,0,0);
				else						col=CBSchView::m_COL.colPART_PINNUM;
				g_drawComponentPinNumName(pDC,rc,nLtrb^2,dwMode,col,pszPinNum,nVExt,nWExt);
			}
		}
	}
}

CSize g_drawComponentNumName(CDC* pDC,
							const SRect& rc,
							DWORD  dwMode,
							BOOL bVert,
							COLORREF col,
							const TCHAR* name,
							int nVExt,						//表示倍率(View)
							int nWExt,						//表示倍率(Window)
							BOOL bUseUpperLine)
{
	int x = (bVert ? rc.r() : rc.l());
	return g_pFontName->PutFont(pDC,x,rc.b()-1,name,-1,col,dwMode,bVert,0,nVExt,nWExt,bUseUpperLine);
}

//部品の描画
void g_DrawComponent(
	CDC* pDC,						//描画先
	SXBSchObj* pObj,
	DWORD dwMode,					//描画モード
	int nVExt/*=1*/,				//表示倍率(View)
	int nWExt/*=1*/,				//表示倍率(Window)
	LPCRECT lprcClip ,		//クリップ情報
	DISP_BLOCKNUM_TYPE dispBN )	//ブロック番号の表示
{
	int n,nPin;

	if(pObj->id()!=SXBSchObj::ID_COMPONENT) return;
	//int x0,y0,x1,y1;
	SXBSchComponent* pPart=(SXBSchComponent*)pObj;
	const SCompIndex* pIndex = pPart->compInfoIndex();

	if(!(dwMode & DRAW_INH_PARTBODY)){
//		//部品本体の描画
//		g_DrawComponentBody(pDC,pPart,dwMode,nVExt,nWExt);//胴部の描画
	
		if(pIndex){
			int nBlock=pPart->block();
			nPin=pPart->pinCount();//ピン数を得て
			for(n=0;n<nPin;n++){	//ピンの数だけピンの描画を繰り返す
				SPoint pinLoc;
				int nLtrb;
				const SPin* pPin = pPart->pinLoc(n,nLtrb,pinLoc);
				const TCHAR* pPinName;
				if(g_bDisplayHiddenPinNum){
					pPinName = pPin->pinName();
				}else{
					pPinName = pPin->pinNameDraw();
				}
					
				g_drawComponentPin(	pDC,
									pinLoc,
									pPin->type(),
									nLtrb,
									pPinName,				//ピン名
									pPin->pinNum(nBlock),	//ピン番号
									dwMode,					//描画モード
									nVExt,						//表示倍率(View)
									nWExt,						//表示倍率(Window)
									lprcClip);				//クリップ情報

				//if((dwMode & DRAW_NC_MARK)&&(!(dwMode & DRAW_TEMP))&&pPin->nc()){
				//	SPoint pt = pPart->pinPosition(n);
				//	g_drawNcMark(	pDC,
				//					pt,						//位置
				//					dwMode,					//描画モード
				//					nVExt,					//表示倍率(View)
				//					nWExt,					//表示倍率(Window)
				//					lprcClip);				//クリップ情報
				//}

			
			}
		}
		//部品本体の描画
		g_DrawComponentBody(pDC,pPart,dwMode,nVExt,nWExt);//胴部の描画
	}
	/////////////////////////////////////////////////////////////////
	// DRAW_FASTでは部品名、部品番号を描画しないのでここまでで終了 //
	/////////////////////////////////////////////////////////////////
	if(dwMode & DRAW_FAST) return;	//戻り
	COLORREF col;
	SRect rc;
	CRect crc;
	CRect rcIntersect;

	CSize sizeDraw;
	int w,h;


	//部品名の描画
	if(!(dwMode & DRAW_INH_PARTNAME) && !(pPart->GetHideVal() && !g_bDisplayHiddenPinNum)){
		if(dwMode & DRAW_DARK)		col=CBSchView::m_COL.colDARK;
		else if(dwMode & DRAW_TEMP)	col=CBSchView::m_COL.colXOR; 
		else if(dwMode & DRAW_OFF)	col=RGB(0,0,0);
		else col=CBSchView::m_COL.colPART_NAME;

		rc = pPart->nameArea();
		crc = SRect2CRect(rc);
		if(!lprcClip || rcIntersect.IntersectRect(&crc,lprcClip)){
			sizeDraw = g_drawComponentNumName(pDC,rc,dwMode,!pPart->nameHorizontal(),col,pPart->name(),nVExt,nWExt,FALSE);
			w = sizeDraw.cx * nWExt/ nVExt;
			h = sizeDraw.cy * nWExt/ nVExt;
			pPart->setNameDrawSize(SSize(w,h));
		}
	}	
	


	//部品番号の描画
	if(!(dwMode & DRAW_INH_PARTNUM) && !(pPart->GetHideNum() && !g_bDisplayHiddenPinNum)){
		if(dwMode & DRAW_DARK)		col=CBSchView::m_COL.colDARK;
		else if(dwMode & DRAW_TEMP)	col=CBSchView::m_COL.colXOR; 
		else if(dwMode & DRAW_OFF)	col=RGB(0,0,0);
		else						col=CBSchView::m_COL.colPART_NUM;

		CString str = pPart->refnum();
		CString bn;
		//if(pPart->block()
		if(dispBN!=DISP_BLOCKNUM_TYPE::NOTDISPLAY_BLOCKNUM && 
			(
				(pPart->compInfoIndex()->block()>1) || (pPart->compInfoIndex()->m_pCompInfo->m_ciType==SCompInfo::CITYPE::FIXED_BLOCK)
			)
			
		){ 

			
			//2014/04/29
			int nBn=0;
			if(pPart->compInfoIndex()->m_pCompInfo->m_ciType==SCompInfo::CITYPE::FIXED_BLOCK){
				nBn = pPart->compInfoIndex()->m_pCompInfo->m_nFixedBlock;
			}else{
				nBn= pPart->block();
			}
			if(nBn<0)nBn=0;
			else if(nBn>254) nBn = 254;

			if(dispBN==DISP_BLOCKNUM_TYPE::DISPLAY_BLOCKNUM_ALPHA){
				if(nBn<26){
					bn.Format(_T("%c"),_T('A')+nBn);
				}else{
					bn.Format(_T("%c%c"),_T('A')+nBn/26,_T('A')+nBn%26);
				}
			}else if(dispBN==DISP_BLOCKNUM_TYPE::DISPLAY_BLOCKNUM_NUM){
				bn.Format(_T("-%d"),nBn+1);
			}
			str.Append(bn);
		}

		rc = pPart->refnumArea();
		crc = SRect2CRect(rc);
		if(!lprcClip || rcIntersect.IntersectRect(&crc,lprcClip)){
			sizeDraw = g_drawComponentNumName(pDC,rc,dwMode,!pPart->refnumHorizontal(),col,str,nVExt,nWExt,FALSE);
			w = sizeDraw.cx * nWExt/ nVExt;
			h = sizeDraw.cy * nWExt/ nVExt;
			pPart->setRefnumDrawSize(SSize(w,h));
		}
	}
}
