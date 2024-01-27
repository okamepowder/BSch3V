/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

/****************************************************************************
** XBSch図面要素線クラスの実装
****************************************************************************/
#include "stdafx.h"
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <math.h>
#include <string>
using namespace std;
#include "ce3io.h"
#include "xbschobj.h"
#include "xbschline.h"
//#include <SPoint.h>
//#include <SRect.h>

//コンストラクタ
SXBSchLine::SXBSchLine()
{
}

//選択状態のときのスナップポイントを取得する
SPoint SXBSchLine::snapPoint()
{
	if(m_Selection & SELECT_P1) 		return m_p1;
	else if(m_Selection & SELECT_P2) 	return m_p2;
	else 								return m_p1;
}

//点を指定して選択重要度を返す
//線オブジェクトの重要度は常に1
int  SXBSchLine::qSelectWeight(const SPoint& pt)
{
	SRect rcSpot(pt.x()-2,pt.y()-2,5,5);
	if(testSelection(rcSpot)){
		return (1 | ON_OBJ); 
	}else{
		return 0;
	}
}

//範囲を指定して選択を行う
unsigned SXBSchLine::testSelection(const SRect& rc)
{
	unsigned outcodeP1,outcodeP2;

	if(rc.intersect(m_p1,m_p2,outcodeP1,outcodeP2)){
		return SELECT_ALL;
	}else{
		return 0;
	}
}



//	unsigned nOutcode1 = g_setOutCode(m_p1,rc);
//	unsigned nOutcode2 = g_setOutCode(m_p2,rc);
//	if(nOutcode1 & nOutcode2) return 0;
//	if((nOutcode1 == 0 )||(nOutcode2 == 0 )) return SELECT_ALL;
//	unsigned nOutcode = nOutcode1 | nOutcode2;
//	if(nOutcode == (OUTCODE_LEFT  | OUTCODE_RIGHT)) return SELECT_ALL;
//	if(nOutcode == (OUTCODE_UPPER | OUTCODE_LOWER)) return SELECT_ALL;
//
//	int x,y;
//	int x1 = m_p1.x();
//	int y1 = m_p1.y();
//	int x2 = m_p2.x();
//	int y2 = m_p2.y();
//
//	if(nOutcode & OUTCODE_LEFT){	//片方が左に出ている
//		x=rc.l();
//		y=y1+((x-x1)*(y2-y1))/(x2-x1);
//		if(y >= rc.t() && y <= rc.b()) return SELECT_ALL;
//	}
//	if(nOutcode & OUTCODE_RIGHT){	//片方が右に出ている
//		x=rc.r();
//		y=y1+((x-x1)*(y2-y1))/(x2-x1);
//		if(y >= rc.t() && y <= rc.b()) return SELECT_ALL;
//	}
//	if(nOutcode & OUTCODE_UPPER){	//片方が上に出ている
//		y=rc.t();
//		x=x1+((y-y1)*(x2-x1))/(y2-y1);
//		if(x >= rc.l() && x <= rc.r()) return SELECT_ALL;
//	}
//	if(nOutcode & OUTCODE_LOWER){	//片方が下に出ている
//		y=rc.b();
//		x=x1+((y-y1)*(x2-x1))/(y2-y1);
//		if(x >= rc.l() && x <= rc.r()) return SELECT_ALL;
//	}
//	return 0;
//}

//範囲を選択してドラッグ選択を行う
unsigned SXBSchLine::testSelectionForDrag(const SRect& rc)
{
//	unsigned nResult = 0;

	//TRACE("SXBSchLine::testSelectionForDrag() W:%d H:%d\n",rc.w(),rc.h());

	bool pointSel = (rc.w()<5 && rc.h()<5); 


	unsigned nOutcode1 = g_setOutCode(m_p1,rc);
	unsigned nOutcode2 = g_setOutCode(m_p2,rc);
	if(nOutcode1 & nOutcode2) return 0;

	if(pointSel){
		if(nOutcode1 == 0 ) return SELECT_P1;
		if(nOutcode2 == 0 ) return SELECT_P2;
		return 0;
	}else{
		unsigned nResult = 0;
		if(nOutcode1 == 0 ) nResult |= SELECT_P1;
		if(nOutcode2 == 0 ) nResult |= SELECT_P2;
		return nResult;
	}
}

//描画が必要かどうかを返す
bool SXBSchLine::qRedraw(const SRect& rc)
{
	SRect rcDraw(rc.l()-1,rc.t()-1,rc.r()+1,rc.b()+1);
	if(testSelection(rcDraw)) 	return true;
	else 						return false;
}

SRect SXBSchLine::area()
{
	SRect rcResult = SRect(SPoint(INT_MAX,INT_MAX),SPoint(INT_MIN,INT_MIN));
	if(m_Selection & (SELECT_P1 | SELECT_P2)){
		if(m_Selection & SELECT_P1) rcResult |= SRect(m_p1.x()-2,m_p1.y()-2,4,4);
		if(m_Selection & SELECT_P2) rcResult |= SRect(m_p2.x()-2,m_p2.y()-2,4,4);
	}else{
		rcResult |= SRect(m_p1.x()-2,m_p1.y()-2,4,4);
		rcResult |= SRect(m_p2.x()-2,m_p2.y()-2,4,4);
	}
	return rcResult;
}

SRect SXBSchLine::rotateRect(){
	SRect rc;
	switch(m_Selection & SELECT_ALL){
	case NOT_SELECTED:
	case SELECT_ALL:
		rc=SRect(m_p1,m_p2);
		rc.normalize();
		break;
	case SELECT_P1:
		rc=SRect(m_p1,m_p1);
		break;
	default:
//	case SELECT_P2:
		rc=SRect(m_p2,m_p2);
		break;
	}
	return rc;
}

//X座標を指定して左右反転を行う
void SXBSchLine::mirrorH(int x)
{
	switch(m_Selection & SELECT_ALL){
	case NOT_SELECTED:
	case SELECT_ALL:
		m_p1.setX(x*2 -m_p1.x());
		m_p2.setX(x*2 -m_p2.x());
		break;
	case SELECT_P1:
		m_p1.setX(x*2 -m_p1.x());
		break;
	default:
//	case SELECT_P2:
		m_p2.setX(x*2 -m_p2.x());
		break;
	}
}

//左右反転を行う
void SXBSchLine::mirrorH()
{
	switch(m_Selection & SELECT_ALL){
	case NOT_SELECTED:
	case SELECT_ALL:
		int x= (m_p1.x() + m_p2.x())/2;
		mirrorH(x);
		break;
	}
}
	
//XY中心座標を指定して、回転を行う
void SXBSchLine::rotate(const SPoint& p)
{
	int newX,newY;

	bool rot1 = false;
	bool rot2 = false;
	switch(m_Selection & SELECT_ALL){
	case NOT_SELECTED:
	case SELECT_ALL:
		rot1 = true;
		rot2 = true;
		break;
	case SELECT_P1:
		rot1 =true;
		break;
	default:
//	case SELECT_P2:
		rot2=true;
		break;
	}


	if(rot1){
		newY = p.y() + (m_p1.x() - p.x()) ;
		newX = p.x() - (m_p1.y() - p.y()) ;
		m_p1.setY( newY );
		m_p1.setX( newX );
	}
	if(rot2){
		newY = p.y() + (m_p2.x() - p.x()) ;
		newX = p.x() - (m_p2.y() - p.y()) ;
		m_p2.setY( newY );
		m_p2.setX( newX );
	}
}

//回転を行う
void SXBSchLine::rotate()
{
	switch(m_Selection & SELECT_ALL){
	case NOT_SELECTED:
	case SELECT_ALL:
		SPoint p;
		p.setX((m_p1.x()+m_p2.x())/2);
		p.setY((m_p1.y()+m_p2.y())/2);
		rotate(p);
		break;
	}
}






//ストリームからの読み込み
bool SXBSchLine::readCe3Ex(SReadCE3& rce3,const SPoint* pptOrigin,const TCHAR* pTag)
{
	assert(pTag != NULL);
	wstring str(_T(""));
	bool bInitX1 = false;
	bool bInitY1 = false;
	bool bInitX2 = false;
	bool bInitY2 = false;
	while(1){
		if(rce3.ReadRecord(str)==WEOF) return false;
		if(str[0] == '-'){
			if( (str.substr(1) != pTag) || !bInitX1 || !bInitY1 || !bInitX2 || !bInitY2){
				return false;
			}else{
				break;	//break while(1){ }
			}
		}else if(str[0] == '+'){
			str[0]='-';
			if(rce3.SkipTo(str)==WEOF) return false;
		}else{
			int n;
			int l=str.length();
			for(n=0;n<l;n++){
				if(str[n]==':') break;
			}
			if(0<n && n<(l-1)){	// : で分けられたレコードである
				wstring var = str.substr(0,n);				//先頭から:の手前まで
				int nParam = _tstoi(str.substr(n+1).c_str()); //:から終端までを数値に変換

//#ifdef _MFCBSCH	//Qt-BSch doesn't support layer.
				if(var==_T("L")){
					if(isValidLayer(nParam)){
						m_Layer	= nParam;			
					}
				}else
//#endif
				
				if(var == _T("X1")){
					if(pptOrigin) nParam += pptOrigin->x();		
					m_p1.setX(nParam);
					bInitX1 = true;
				}else
				if(var == _T("Y1")){
					if(pptOrigin) nParam += pptOrigin->y();		
					m_p1.setY(nParam);
					bInitY1 = true;
				}else
				if(var == _T("X2")){
					if(pptOrigin) nParam += pptOrigin->x();		
					m_p2.setX(nParam);
					bInitX2 = true;
				}else
				if(var == _T("Y2")){
					if(pptOrigin) nParam += pptOrigin->y();		
					m_p2.setY(nParam);
					bInitY2 = true;
				}
			}

		}
	}

	return true;
}

//ストリームへの書き込み
bool SXBSchLine::writeCe3Ex(SWriteCE3& wce3,const SPoint* pptOrigin,const TCHAR* pTag)
{
	assert(pTag != NULL);

	TCHAR sz[32];
	int x1 = m_p1.x();
	int y1 = m_p1.y();
	int x2 = m_p2.x();
	int y2 = m_p2.y();
	if(pptOrigin){
		x1 -= pptOrigin->x();
		y1 -= pptOrigin->y();
		x2 -= pptOrigin->x();
		y2 -= pptOrigin->y();
	}

	_stprintf_s(sz, sizeof(sz) / sizeof(sz[0]), _T("+%s"), pTag);
	wce3.WriteRecord(sz);
	wce3.WriteRecordInt(_T("L"),m_Layer);
	wce3.WriteRecordInt(_T("X1"),x1);
	wce3.WriteRecordInt(_T("Y1"),y1);
	wce3.WriteRecordInt(_T("X2"),x2);
	wce3.WriteRecordInt(_T("Y2"),y2);

//	sprintf(sz,"X1:%d",x1);
//	wce3.WriteRecord(sz);
//	sprintf(sz,"Y1:%d",y1);
//	wce3.WriteRecord(sz);
//	sprintf(sz,"X2:%d",x2);
//	wce3.WriteRecord(sz);
//	sprintf(sz,"Y2:%d",y2);
//	wce3.WriteRecord(sz);

	_stprintf_s(sz, sizeof(sz) / sizeof(sz[0]), _T("-%s"), pTag);
	wce3.WriteRecord(sz);
	wce3.WriteEOL();
	return true;
}

//ワイヤーデータクラス
//ストリームへの書き込み　+WIRE ～ -WIRE
bool SXBSchWire::readCe3(SReadCE3& rce3,const SPoint* pptOrigin,SXBSchDoc*)
{
	return readCe3Ex(rce3,pptOrigin,_T("WIRE"));
}

bool SXBSchWire::writeCe3(SWriteCE3& wce3,const SPoint* pptOrigin)
{
	return writeCe3Ex(wce3,pptOrigin,_T("WIRE"));
}



//複製を行う
SXBSchObj* SXBSchWire::duplicate()
{
	SXBSchObj* newObj =new SXBSchWire(*this);
	return newObj;
}




//バスデータクラス
//ストリームからの読み込み
bool SXBSchBus::readCe3(SReadCE3& rce3,const SPoint* pptOrigin,SXBSchDoc*)
{
	return readCe3Ex(rce3,pptOrigin,_T("BUS"));
}

//ストリームへの書き込み　+BUS ～ -BUS
bool SXBSchBus::writeCe3(SWriteCE3& wce3,const SPoint* pptOrigin)
{
	return writeCe3Ex(wce3,pptOrigin,_T("BUS"));
}


//複製を行う
SXBSchObj* SXBSchBus::duplicate()
{
	SXBSchObj* newObj =new SXBSchBus(*this);
	return newObj;
}


//マーカーラインクラス

//ストリームからの読み込み   
bool SXBSchMarkerLine::readCe3(SReadCE3& rce3,const SPoint* pptOrigin,SXBSchDoc*)
{
	wstring str(_T(""));
	bool bInitX1 = false;
	bool bInitY1 = false;
	bool bInitX2 = false;
	bool bInitY2 = false;
	while(1){
		if(rce3.ReadRecord(str)==WEOF) return false;
		if(str[0] == '-'){
			if( (str.substr(1) != _T("ALINE")) || !bInitX1 || !bInitY1 || !bInitX2 || !bInitY2){
				return false;
			}else{
				break;	//break while(1){ }
			}
		}else if(str[0] == '+'){
			str[0]='-';
			if(rce3.SkipTo(str)==WEOF) return false;
		}else{
			int n;
			int l=str.length();
			for(n=0;n<l;n++){
				if(str[n]==':') break;
			}
			if(0<n && n<(l-1)){	// : で分けられたレコードである
				wstring var = str.substr(0,n);				//先頭から:の手前まで
				int nParam = _tstoi(str.substr(n+1).c_str()); //:から終端までを数値に変換

				if(var==_T("L")){
					if(isValidLayer(nParam)){
						m_Layer	= nParam;			
					}
				}else
				
				if(var == _T("X1")){
					if(pptOrigin) nParam += pptOrigin->x();		
					m_p1.setX(nParam);
					bInitX1 = true;
				}else
				if(var == _T("Y1")){
					if(pptOrigin) nParam += pptOrigin->y();		
					m_p1.setY(nParam);
					bInitY1 = true;
				}else
				if(var == _T("X2")){
					if(pptOrigin) nParam += pptOrigin->x();		
					m_p2.setX(nParam);
					bInitX2 = true;
				}else
				if(var == _T("Y2")){
					if(pptOrigin) nParam += pptOrigin->y();		
					m_p2.setY(nParam);
					bInitY2 = true;
				}else

				if(var == _T("WDT")){
					m_width = (nParam);
					if(m_width < MIN_MARKER_WIDTH) m_width =  MIN_MARKER_WIDTH;
					else if(m_width > MAX_MARKER_WIDTH) m_width = MAX_MARKER_WIDTH;
				}else

				if(var == _T("CLR")){
					m_nColor = nParam;
				}

			}

		}
	}

	return true;
}

//ストリームへの書き込み　+ALINE ～ -ALINE
bool SXBSchMarkerLine::writeCe3(SWriteCE3& wce3,const SPoint* pptOrigin)
{
	int x1 = m_p1.x();
	int y1 = m_p1.y();
	int x2 = m_p2.x();
	int y2 = m_p2.y();
	if(pptOrigin){
		x1 -= pptOrigin->x();
		y1 -= pptOrigin->y();
		x2 -= pptOrigin->x();
		y2 -= pptOrigin->y();
	}

	wce3.WriteRecord(_T("+ALINE"));
	wce3.WriteRecordInt(_T("L"),m_Layer);
	wce3.WriteRecordInt(_T("X1"),x1);
	wce3.WriteRecordInt(_T("Y1"),y1);
	wce3.WriteRecordInt(_T("X2"),x2);
	wce3.WriteRecordInt(_T("Y2"),y2);

	wce3.WriteRecordInt(_T("STL"),0);
	wce3.WriteRecordInt(_T("WDT"),m_width);
	wce3.WriteRecordInt(_T("CLR"),m_nColor);

	wce3.WriteRecord(_T("-ALINE"));
	wce3.WriteEOL();
	return true;
}


//複製を行う
SXBSchObj* SXBSchMarkerLine::duplicate()
{
	SXBSchObj* newObj =new SXBSchMarkerLine(*this);
	((SXBSchMarkerLine*)newObj)->m_width = this->m_width;
	((SXBSchMarkerLine*)newObj)->m_nColor = this->m_nColor;
	return newObj;
}

//マーカーラインの外周を形成する4つのポイントの座標を得る
void SXBSchMarkerLine::outlinePoint(SPoint* pp)
{
	int halfWidth = m_width/2;
	if(x1()==x2()){	//垂直
		int mi,ma;
		if(y1()<y2()){
			mi = y1();
			ma = y2();
		}else{
			mi = y2();
			ma = y1();
		}
		
		pp[0] = SPoint(x1() + halfWidth , mi - halfWidth);
		pp[1] = SPoint(x1() + halfWidth , ma + halfWidth);
		pp[2] = SPoint(x1() - halfWidth , ma + halfWidth);
		pp[3] = SPoint(x1() - halfWidth , mi - halfWidth);
	}else if(y1()==y2()){
		int mi,ma;
		if(x1()<x2()){
			mi = x1();
			ma = x2();
		}else{
			mi = x2();
			ma = x1();
		}
		
		pp[0] = SPoint(mi - halfWidth , y1() + halfWidth);
		pp[1] = SPoint(ma + halfWidth , y1() + halfWidth);
		pp[2] = SPoint(ma + halfWidth , y1() - halfWidth);
		pp[3] = SPoint(mi - halfWidth , y1() - halfWidth);
	}else{
		double a=((double)(y2()-y1()))/(x2()-x1());
		double b=y1()-a*x1();
		double ext1 = halfWidth*sqrt(1/(1+a*a));
		double ext2 = halfWidth*sqrt(a*a/(1+a*a));

		SPoint _p1,_p2;
		if(x1()<x2()){
			_p1 = m_p1;
			_p2 = m_p2;
		}else{
			_p2 = m_p1;
			_p1 = m_p2;
		}

		double incFlag = (a>0 ? 1 : -1);

		pp[0] = SPoint((int)(double)(_p1.x() - ext1 - incFlag * ext2), (int)(double)(_p1.y() + ext1 - incFlag * ext2));
		pp[1] = SPoint((int)(double)(_p2.x() + ext1 - incFlag * ext2), (int)(double)(_p2.y() + ext1 + incFlag * ext2));
		pp[2] = SPoint((int)(double)(_p2.x() + ext1 + incFlag * ext2), (int)(double)(_p2.y() - ext1 + incFlag * ext2));
		pp[3] = SPoint((int)(double)(_p1.x() - ext1 + incFlag * ext2), (int)(double)(_p1.y() - ext1 - incFlag * ext2));

	}
}

SRect SXBSchMarkerLine::area()
{
	//選択されていないか、両端が選択されている場合
	if(((m_Selection & (SELECT_P1 | SELECT_P2)) == (SELECT_P1 | SELECT_P2))||((m_Selection & (SELECT_P1 | SELECT_P2))==0)){
		SRect rcResult = SRect(SPoint(INT_MAX,INT_MAX),SPoint(INT_MIN,INT_MIN));
		SPoint ap[4];
		outlinePoint(ap);
		for(int i=0;i<4;i++){
			rcResult |= ap[i];
		}
		return rcResult;
	}

	int endSq = m_width*7/10;
	if(endSq<5) endSq=5;
	if(m_Selection & SELECT_P1){
		return SRect(m_p1.x()-endSq/2,m_p1.y()-endSq/2,endSq,endSq);
	}else{
		return SRect(m_p2.x()-endSq/2,m_p2.y()-endSq/2,endSq,endSq);
	}
}


bool SXBSchMarkerLine::pointIsInside(const SPoint& pt, const SPoint* pa)
{
	int l,t,r,b;
	l=pa[0].x();
	t=pa[0].y();
	r=l;
	b=t;
	for(int i=1;i<4;i++){
		int x = pa[i].x();
		int y = pa[i].y();
		if(x<l) l=x;
		if(x>r) r=x;
		if(y<t) t=y;
		if(y>b) b=y;
	}

	if(pt.x()<l || pt.x()>r || pt.y()<t || pt.y()>b) return false;

	int dx = x1()-x2();
	int dy = y1()-y2();

	int mag =0 ;
	if(dx!=0){
		mag = 100000/dx;
	}else if(dy!=0){
		mag = 100000/dy;
	}else{
		return false;
	}

	if(mag==0) mag = 1;

	//SPoint p01=SPoint(pt.x(),100000);
	SPoint p01=SPoint(pt.x()+mag*dx,pt.y()+mag*dy);


	int xcount = 0;
	if(g_linecross(pt,p01,pa[0],pa[1])) xcount++;
	if(g_linecross(pt,p01,pa[1],pa[2])) xcount++;
	if(g_linecross(pt,p01,pa[2],pa[3])) xcount++;
	if(g_linecross(pt,p01,pa[3],pa[0])) xcount++;
	return (xcount & 1);
}




//範囲を指定して選択を行う
unsigned SXBSchMarkerLine::testSelection(const SRect& rc)
{	
	SPoint ap[4];
	outlinePoint(ap);


	SPoint pt;
	pt = SPoint(rc.r(),rc.t());
	if(pointIsInside(pt, ap)) return SELECT_ALL;
	pt = SPoint(rc.l(),rc.t());
	if(pointIsInside(pt, ap)) return SELECT_ALL;
	pt = SPoint(rc.l(),rc.b());
	if(pointIsInside(pt, ap)) return SELECT_ALL;
	pt = SPoint(rc.r(),rc.b());
	if(pointIsInside(pt, ap)) return SELECT_ALL;

	unsigned ocp1,ocp2;//ダミー

	if(rc.intersect(ap[0],ap[1],ocp1,ocp2))return SELECT_ALL;
	if(rc.intersect(ap[1],ap[2],ocp1,ocp2))return SELECT_ALL;
	if(rc.intersect(ap[2],ap[3],ocp1,ocp2))return SELECT_ALL;
	if(rc.intersect(ap[3],ap[0],ocp1,ocp2))return SELECT_ALL;

	return 0;
}

//範囲を選択してドラッグ選択を行う
unsigned SXBSchMarkerLine::testSelectionForDrag(const SRect& rcSel)
{
	//TRACE("SXBSchLine::testSelectionForDrag() W:%d H:%d\n",rcSel.w(),rcSel.h());
	bool pointSel = (rcSel.w()<5 && rcSel.h()<5); 

	SRect rcEnd;
	int endSq = m_width*7/10;
	if(endSq<5) endSq=5;

	unsigned retVal = 0;

	rcEnd = SRect(m_p1.x()-endSq/2,m_p1.y()-endSq/2,endSq,endSq);
	if(rcSel.intersect(rcEnd)){
		retVal = SELECT_P1;
		if(pointSel)return retVal;
	}

	rcEnd = SRect(m_p2.x()-endSq/2,m_p2.y()-endSq/2,endSq,endSq);
	if(rcSel.intersect(rcEnd)){
		retVal |= SELECT_P2;
	}

	return retVal;
}