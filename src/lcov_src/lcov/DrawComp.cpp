/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/


#include "stdafx.h"
#include <stdlib.h> //abs()のためにインクルード 
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <list>
#include <string>
using namespace std;
#include "complib.h"
#include "draw.h"
#include "bschfont.h"
#define M_PI       3.14159265358979323846

bool g_bIsWin9x;

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
		movePitch = sqrt(pitch*pitch/(1+incline*incline));
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
		movePitch = sqrt(pitch*pitch/(1+incline*incline));

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
	x0=centerX+rv*cos(a)*hm;
	y0=centerY-rv*sin(a);
	while(1){
		//TRACE("%d,%d,%f,%f,%f\n",x0,y0,a,cos(a),sin(a));
		a+=(2*M_PI)/32;
		if(a>=dEnd){
			a=dEnd;
		}
		x1=centerX+rv*cos(a)*hm;
		y1=centerY-rv*sin(a);
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
			int	 cx,
			int  cy,
			int	 x0,
			int  y0,
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
			SPtnObjLine* pObj,		//描画オブジェクト
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
	//bool b9xDot=false;

	int width = (pObj->m_width * nVExt)/nWExt;
	int style = pObj->m_style;

	if(style == 0){
		newPen.CreatePen(PS_SOLID,width,col);
	}else /*if(style == 1)*/{
		if(width>1){
			//if(::g_bIsWin9x){
			//	newPen.CreatePen(PS_SOLID,width,col);
			//	b9xDot=true;
			//}else{
				DWORD adwPenStyle[2];
				adwPenStyle[0]=adwPenStyle[1]=width*2;
				LOGBRUSH logBrush;
				logBrush.lbStyle = BS_SOLID;
				logBrush.lbColor = col;
				newPen.CreatePen(PS_USERSTYLE|PS_GEOMETRIC|PS_ENDCAP_ROUND, width, &logBrush,2,adwPenStyle);
			//}
		}else{
			newPen.CreatePen(PS_DOT,width,col);
		}
	}
	pPenOld = pDC->SelectObject(&newPen);

	xs=pObj->GetP0().x();	ys=pObj->GetP0().y();
	convDirXY(xs,ys,dir,cx,cy);
	xs+=x0;  ys+=y0;
	xs = (xs*nVExt)/nWExt;
	ys = (ys*nVExt)/nWExt;

	x1=pObj->GetP1().x();	y1=pObj->GetP1().y();
	convDirXY(x1,y1,dir,cx,cy);
	x1+=x0;  y1+=y0;
	x1 = (x1*nVExt)/nWExt;
	y1 = (y1*nVExt)/nWExt;
	//if(b9xDot){
	//	bool bMark=true;
	//	int nPos=0;
	//	DotLine9x(pDC,xs,ys,x1,y1,width,bMark,nPos);
	//}else{
	if(!pObj->getCurve()){
		int nOldBkMmode=pDC->SetBkMode(TRANSPARENT);
		pDC->MoveTo(xs,ys);
		pDC->LineTo(x1,y1);
		pDC->SetBkMode(nOldBkMmode);
	}else{
		CPoint pointArray[4];
		pointArray[0] = CPoint(xs,ys);
		pointArray[3] = CPoint(x1,y1);

		xs=pObj->GetPC0().x();	ys=pObj->GetPC0().y();
		convDirXY(xs,ys,dir,cx,cy);
		xs+=x0;  ys+=y0;
		xs = (xs*nVExt)/nWExt;
		ys = (ys*nVExt)/nWExt;
		pointArray[1] = CPoint(xs,ys);

		xs=pObj->GetPC1().x();	ys=pObj->GetPC1().y();
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
			SPtnObjCircle* pObj,	//描画オブジェクト
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
	bool b9xDot=false;
	int fill = pObj->m_nFill;


	int width = (pObj->m_width * nVExt)/nWExt;

	int style = pObj->m_style;

	if(style == 0){
		newPen.CreatePen(PS_SOLID,width,col);
	}else /*if(style == 1)*/{
		if(width>1){
			if(::g_bIsWin9x){
				if(fill == -1){	//フィルなしのときだけまじめに点線描画
					newPen.CreatePen(PS_SOLID,width,col);
					b9xDot=true;
				}else{
					newPen.CreatePen(PS_DOT,1,col);
				}
			}else{
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

	x1=pObj->m_p0.x();	y1=pObj->m_p0.y();
	//x1=buff[4];	y1=buff[5];
	convDirXY(x1,y1,dir,cx,cy);
	x1+=x0;  y1+=y0;
	x1 = (x1*nVExt)/nWExt;
	y1 = (y1*nVExt)/nWExt;

	x2=pObj->m_p1.x();	y2=pObj->m_p1.y();
	//x2=buff[6];	y2=buff[7];
	convDirXY(x2,y2,dir,cx,cy);
	x2+=x0;  y2+=y0;
	x2 = (x2*nVExt)/nWExt;
	y2 = (y2*nVExt)/nWExt;
	
	CRect rc(x1,y1,x2,y2);
	rc.NormalizeRect();
	//if(width<=1){
		rc.right++;
		rc.bottom++;
	//}

//	int nOldBkMmode=pDC->SetBkMode(TRANSPARENT);
//	pDC->Ellipse(&rc);
//	pDC->SetBkMode(nOldBkMmode);


	if(b9xDot){
		CPoint pt(1,1);
		DotArc9x(pDC,&rc,pt,pt,width);
	}else{
		int nOldBkMmode=pDC->SetBkMode(TRANSPARENT);
		pDC->Ellipse(&rc);
		pDC->SetBkMode(nOldBkMmode);
	}


	pDC->SelectObject(pBrushOld);
	pDC->SelectObject(pPenOld);

}

void g_DrawPtnPolygon(
			CDC* pDC,				//描画
			SPtnObjPolygon* pObj,	//描画オブジェクト
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
	bool b9xDot=false;
	int fill = pObj->m_nFill;
	

	int node=pObj->m_nCount;
	if(node<2||node>VECTPLOYGON_MAXNODE) return;

	int x1,y1;
	int width = (pObj->m_width*nVExt)/nWExt;

	int style = pObj->m_style;

	if(style == 0){
		newPen.CreatePen(PS_SOLID,width,col);
	}else /*if(style == 1)*/{
		if(width>1){
			if(::g_bIsWin9x){
				if(fill == -1){	//フィルなしのときだけまじめに点線描画
					newPen.CreatePen(PS_SOLID,width,col);
					b9xDot=true;
				}else{
					newPen.CreatePen(PS_DOT,1,col);
				}
			}else{
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
		x1 = pObj->m_pPt[i].x();
		y1 = pObj->m_pPt[i].y();
		//x1=buff[j++];	y1=buff[j++];
		convDirXY(x1,y1,dir,cx,cy);
		x1+=x0;  y1+=y0;
		x1 = (x1*nVExt)/nWExt;
		y1 = (y1*nVExt)/nWExt;
		aPoint[i].x=x1;
		aPoint[i].y=y1;
	}
	if(b9xDot){
		DotPolygon9x(pDC,aPoint,node,width);
	}else{
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
			SPtnObjArc* pObj,		//描画オブジェクト
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

	int x,y,r,x1,y1,x2,y2,x3,y3,x4,y4;
	int width = (pObj->m_width * nVExt)/nWExt;
	int style = pObj->m_style;
	bool b9xDot=false;

	if(style == 0){
		newPen.CreatePen(PS_SOLID,width,col);
	}else /*if(style == 1)*/{
		if(width>1){
			if(::g_bIsWin9x){
				newPen.CreatePen(PS_SOLID,width,col);
				b9xDot=true;
			}else{
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

	x=pObj->m_ptC.x();	y=pObj->m_ptC.y();  r=pObj->m_nR;

	double rad;
	rad = pObj->m_nBegin;
	rad = M_PI*rad/2880;
	x3 =((int)(500*cos(rad)))+x;
	y3 =(-(int)(500*sin(rad)))+y;

	rad = pObj->m_nEnd;
	rad = M_PI*rad/2880;
	x4 =((int)(500*cos(rad)))+x;
	y4 =(-(int)(500*sin(rad)))+y;
//	TRACE("DRAW ARC %d:%d,%d  %d:%d,%d\n",pObj->m_nBegin,x3,y3,pObj->m_nEnd,x4,y4);

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

	convDirXY(x3,y3,dir,cx,cy);
	x3+=x0;  y3+=y0;
	x3 = (x3*nVExt)/nWExt;
	y3 = (y3*nVExt)/nWExt;

	convDirXY(x4,y4,dir,cx,cy);
	x4+=x0;  y4+=y0;
	x4 = (x4*nVExt)/nWExt;
	y4 = (y4*nVExt)/nWExt;


	CRect rc(x1,y1,x2,y2);
	rc.NormalizeRect();
	//if(width<=1){
		rc.right++;
		rc.bottom++;
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
	if(b9xDot){
		DotArc9x(pDC,&rc,ptBegin,ptEnd,width);
	}else{
		int nOldBkMmode=pDC->SetBkMode(TRANSPARENT);
		pDC->Arc(&rc,ptBegin,ptEnd);
		pDC->SetBkMode(nOldBkMmode);
	}
	pDC->SelectObject(pPenOld);
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
	case PIN_LOC_L:	l=pt.x()-nOffset-len;		t=pt.y()-10;				w=len;	h=8;	break;
	case PIN_LOC_T:	l=pt.x()-10;				t=pt.y()-nOffset-len;		w=8;	h=len;	break;
	case PIN_LOC_R:	l=pt.x()+nOffset;			t=pt.y()-10;				w=len;	h=8;	break;
	default:		l=pt.x()-10;				t=pt.y()+nOffset;			w=8;	h=len;	break;
	}
	return CRect(l,t,l+w,t+h);
}


void g_drawComponentPinNum(	CDC* pDC,
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

	g_pFont->PutFont(pDC,x,y,name,-1,col,dwMode,nLtrb&1,align,nVExt,nWExt);
}

void g_drawComponentPinName(	CDC* pDC,
								const CRect& rc,
								int nLtrb,
								DWORD dwMode,
								COLORREF col,
								COLORREF col_hide,
								const TCHAR* name,
								unsigned long pinNameDispFlag,
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

	g_pFont->PutFontPinName(pDC,x,y,name,pinNameDispFlag,-1,col,col_hide,dwMode,nLtrb&1,align,nVExt,nWExt);
}




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

	int width = 1*nVExt/nWExt;

	//描画モードによってペンの種類・色とCOPY or XORを設定する
	if(dwMode & DRAW_DARK){
		col = COL_DARK;
	}else if(dwMode & DRAW_ON){
		col = COL_PART;
	}else if(dwMode & DRAW_OFF){
		col = RGB(0,0,0);
	}else if(dwMode & DRAW_TEMP){
		col = COL_XOR;
	}else{
		return;
	}

	penNew.CreatePen(PS_SOLID,width,col);

	ppenOld=pDC->SelectObject(&penNew);			//作成したペンの選択
	nOldDrawMode=pDC->SetROP2(R2_COPYPEN);		//描画モードの設定
	if(nType & PIN_TYPE_ZLENG){
		CBrush* pBrush=(CBrush*)pDC->SelectStockObject(NULL_BRUSH);
        x1=((x0-2)*nVExt)/nWExt;
        y1=((y0-2)*nVExt)/nWExt;
        x2=((x0+2)*nVExt)/nWExt+1;
        y2=((y0+2)*nVExt)/nWExt+1;
		pDC->Rectangle(x1,y1,x2,y2);
		pDC->SelectObject(pBrush);
	}else if((nType & PIN_TYPE_NEGATIVE)==0){
        x1=((PinDataNormal[nLtrb][0]+x0)*nVExt)/nWExt;
        y1=((PinDataNormal[nLtrb][1]+y0)*nVExt)/nWExt;
		pDC->MoveTo(x1,y1);
        x1=((PinDataNormal[nLtrb][2]+x0)*nVExt)/nWExt;
        y1=((PinDataNormal[nLtrb][3]+y0)*nVExt)/nWExt;
		pDC->LineTo(x1,y1);
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


void g_drawComponentPin(
	CDC* pDC,						//描画先
	const SPoint& pinPos,
	int	pinType,
	int nLtrb,
	LPCTSTR pszPinName,				//ピン名
	unsigned long pinNameDispFlag,
	LPCTSTR pszPinNum,				//ピン番号
	DWORD dwMode,					//描画モード
	int nVExt,						//表示倍率(View)
	int nWExt,						//表示倍率(Window)
	LPCRECT lprcClip)				//クリップ情報

{
	COLORREF col;
	COLORREF col_hide;

	BOOL  bBodyOnly=(dwMode & DRAW_FAST)||(nVExt<nWExt);//これがTRUEのときピン名、ピン番号を描画しない
	CRect rc;
	CRect rcIntersect;
	
	//ピン本体の描画
	//  ・ビットマップピンを使う場合
	//  ・線描のピンを使う場合

	rc = g_drawComponentPinArea(pinPos,nLtrb);

	if( !lprcClip || rcIntersect.IntersectRect(&rc,lprcClip)){
		g_drawComponentPinBody(
					pDC,
					pinPos,
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

	COLORREF namecol = COL_PART_PINNAME;
	COLORREF numcol = COL_PART_PINNUM;


	//ピン名とピン番号の描画位置を入れ替える
	if((dwMode & DRAW_SWAP_PINNAME_PINNUM)!=0){
		LPCTSTR psz = pszPinName;
		pszPinName = pszPinNum;
		pszPinNum = psz;

		namecol = COL_PART_PINNUM;
		numcol = COL_PART_PINNAME;

	}

	//ピン名の描画
	if(pszPinName!=NULL){
		CRect rc=g_drawComponentPinNameArea(pinPos,nLtrb,pszPinName);
		if(!lprcClip || rcIntersect.IntersectRect(&rc,lprcClip)){
			if(dwMode & DRAW_DARK){
				col=COL_DARK;
				col_hide = COL_DARK;
			}else if(dwMode & DRAW_TEMP){
				col=COL_XOR;
				col_hide = COL_XOR_DARK;
			}else if(dwMode & DRAW_OFF){
				col=RGB(0,0,0);
				col_hide = RGB(0,0,0);
			}else{
				col=namecol;
				col_hide = COL_DARK;
			}
			if((dwMode & DRAW_SWAP_PINNAME_PINNUM)==0){
				g_drawComponentPinName(pDC,rc,nLtrb,dwMode,col,col_hide,pszPinName,pinNameDispFlag,nVExt,nWExt);
			}else{
				g_drawComponentPinNum(pDC,rc,nLtrb,dwMode,col,pszPinName,nVExt,nWExt);
			}
		}
	}

	//ピン番号の描画
//	if((pszPinNum!=NULL)&&((pinType & PIN_TYPE_NUMHIDE) ==0)){
	if(pszPinNum!=NULL){
		//描画色の決定
		CRect rc=g_drawComponentPinNumArea(pinPos,nLtrb,pszPinNum,pinType);
		if(!lprcClip || rcIntersect.IntersectRect(&rc,lprcClip)){
			if(dwMode & DRAW_DARK || pinType & PIN_TYPE_NUMHIDE)		col=COL_DARK;
			else if(dwMode & DRAW_TEMP)	col=COL_XOR; 
			else if(dwMode & DRAW_OFF)	col=RGB(0,0,0);
			else						col=numcol;
			if((dwMode & DRAW_SWAP_PINNAME_PINNUM)==0){
				g_drawComponentPinNum(pDC,rc,nLtrb^2,dwMode,col,pszPinNum,nVExt,nWExt);
			}else{
				g_drawComponentPinName(pDC,rc,nLtrb^2,dwMode,col,col_hide,pszPinNum,pinNameDispFlag,nVExt,nWExt);
			}
		}
	}
}

