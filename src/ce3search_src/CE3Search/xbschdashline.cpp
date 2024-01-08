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
#include "xbschdashline.h"

//破線データクラス
SXBSchDash::SXBSchDash()
{	
	m_nodeCacheValid = false;

	m_bCurve = false;
	//m_ptCt1;
	//m_ptCt2;
	m_lineStyle = SXBSchDash::LINESTYLE_DASH;
	m_lineWidth = 1;
	m_startPointStyle = SXBSchDash::ENDPOINTSTYLE_NORMAL;
	m_endPointStyle = SXBSchDash::ENDPOINTSTYLE_NORMAL;
	m_endMarkSize = 8;
	//m_nodeCount = 0;
	m_arrayNode = NULL;
}

SXBSchDash::~SXBSchDash()
{
	delete[]m_arrayNode;
}


//範囲を指定して選択を行う
unsigned SXBSchDash::testSelection(const SRect& rc)
{
	SRect rcDraw=rc;
	rcDraw.increase((m_lineWidth+1)/2);
	if(!m_bCurve){
		return SXBSchLine::testSelection(rcDraw);
	}else{
		if(!m_nodeCacheValid){
			createNodes();
		}
		if(!m_rcCurve.intersect(rcDraw)){
			return 0;
		}
		unsigned oc1,oc2;
		for(int i=0;i<SBezier::m_nBezierNodes;i++){
			if(rcDraw.intersect(m_arrayNode[i],m_arrayNode[i+1],oc1,oc2))return SELECT_ALL;
		}
	}
	return 0;
}



//描画が必要かどうかを返す
bool SXBSchDash::qRedraw(const SRect& rc)
{
	SRect rcDraw=rc;
	rcDraw.increase((m_lineWidth+1)/2);
	if(!m_bCurve){
		return (SXBSchLine::testSelection(rcDraw)!=0);
	}else{
		if(!m_nodeCacheValid){
			createNodes();
		}
		return m_rcCurve.intersect(rcDraw);
	}
}

SRect SXBSchDash::area()
{
	SRect rcArea;
	if(!m_bCurve){
		rcArea =  SXBSchLine::area();
	}else{
		if(!m_nodeCacheValid){
			createNodes();
		}
		unsigned int selflag = m_Selection & (SELECT_P1 | SELECT_P2);
		if(selflag == 0 || selflag == (SELECT_P1 | SELECT_P2)){
			rcArea = m_rcCurve;
		}else{
			rcArea =  SXBSchLine::area();
		}
	}
	rcArea.increase(m_lineWidth/2+1);
	return rcArea;
}

SRect SXBSchDash::rotateRect()
{
	if(!m_bCurve){
		return SXBSchLine::rotateRect();
	}else{
		if(!m_nodeCacheValid){
			createNodes();
		}
		unsigned int selflag = m_Selection & (SELECT_P1 | SELECT_P2);
		if(selflag == (SELECT_P1 | SELECT_P2)){
			return m_rcCurve;
		}else{
			return SXBSchLine::rotateRect();
		}
	}
}


//ストリームからの読み込み
bool SXBSchDash::readCe3(SReadCE3& rce3,const SPoint* pptOrigin,SXBSchDoc*)
{
	//return readCe3Ex(rce3,pptOrigin,"DASH");
	m_nodeCacheValid = false;
	//m_nodeCount = 0;
	delete[]m_arrayNode;
	m_arrayNode = NULL;


	wstring str(_T(""));
	bool bInitX1 = false;
	bool bInitY1 = false;
	bool bInitX2 = false;
	bool bInitY2 = false;

	bool bInitCtX1 = false;
	bool bInitCtY1 = false;
	bool bInitCtX2 = false;
	bool bInitCtY2 = false;


	while(1){
		if(rce3.ReadRecord(str)==EOF) return false;
		if(str[0] == '-'){
			if(str.substr(1) != _T("DASH") || !bInitX1 || !bInitY1 || !bInitX2 || !bInitY2){
				return false;
			}else{
				if(!bInitCtX1 || !bInitCtY1 || !bInitCtX2 || !bInitCtY2 ){	//制御点が指定されなかったらカーブではない。
					m_bCurve = false;
				}
				break;	//break while(1){ }
			}
		}else if(str[0] == '+'){
			str[0]='-';
			if(rce3.SkipTo(str)==EOF) return false;
		}else{
			int n;
			int l=str.length();
			for(n=0;n<l;n++){
				if(str[n]==':') break;
			}
			if(0<n && n<(l-1)){	// : で分けられたレコードである
				wstring var = str.substr(0,n);				//先頭から:の手前まで
				wstring strParam = str.substr(n+1);			//:から終端までのパラメータ文字列
				if(var==_T("LS")){
					if(strParam == _T("NORM")){
						m_lineStyle = SXBSchDash::LINESTYLE_NORMAL;
					}else if(strParam == _T("LDT")){
						m_lineStyle = SXBSchDash::LINESTYLE_LINEDOT;
					}else if(strParam == _T("LDDT")){
						m_lineStyle = SXBSchDash::LINESTYLE_LINEDOTDOT;
					}else{	//デフォルトはDASH
						m_lineStyle = SXBSchDash::LINESTYLE_DASH;
					}
				}else if(var==_T("SSTL")){
					if(strParam == _T("ARRW")){
						m_startPointStyle = SXBSchDash::ENDPOINTSTYLE_ARROW;
					}else if(strParam == _T("CRCL")){
						m_startPointStyle = SXBSchDash::ENDPOINTSTYLE_CIRCLE;
					}else{	//デフォルトは端末装飾なし
						m_startPointStyle = SXBSchDash::ENDPOINTSTYLE_NORMAL;
					}
				}else if(var==_T("ESTL")){
					if(strParam == _T("ARRW")){
						m_endPointStyle = SXBSchDash::ENDPOINTSTYLE_ARROW;
					}else if(strParam == _T("CRCL")){
						m_endPointStyle = SXBSchDash::ENDPOINTSTYLE_CIRCLE;
					}else{	//デフォルトは端末装飾なし
						m_endPointStyle = SXBSchDash::ENDPOINTSTYLE_NORMAL;
					}
				}else{


					//int nParam = atoi(str.substr(n+1).c_str()); //:から終端までを数値に変換
					int nParam = _tstoi(strParam.c_str()); //:から終端までを数値に変換

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

					if(var == _T("CTX1")){
						if(pptOrigin) nParam += pptOrigin->x();		
						m_ptCt1.setX(nParam);
						bInitCtX1 = true;
					}else
					if(var == _T("CTY1")){
						if(pptOrigin) nParam += pptOrigin->y();		
						m_ptCt1.setY(nParam);
						bInitCtY1 = true;
					}else
					if(var == _T("CTX2")){
						if(pptOrigin) nParam += pptOrigin->x();		
						m_ptCt2.setX(nParam);
						bInitCtX2 = true;
					}else
					if(var == _T("CTY2")){
						if(pptOrigin) nParam += pptOrigin->y();		
						m_ptCt2.setY(nParam);
						bInitCtY2 = true;
					}else

					if(var == _T("CURV")){
						m_bCurve = (nParam!=0);
					}else

					if(var == _T("WDT")){
						m_lineWidth = (nParam);
						if(m_lineWidth < MIN_WIDTH) m_lineWidth =  MIN_WIDTH;
						else if(m_lineWidth > MAX_WIDTH) m_lineWidth = MAX_WIDTH;
					}else

					if(var == _T("EMS")){
						m_endMarkSize = nParam;
						if(m_endMarkSize < MIN_ENDMARKSIZE) m_endMarkSize = MIN_ENDMARKSIZE;
						else if(m_endMarkSize > MAX_ENDMARKSIZE) m_endMarkSize = MAX_ENDMARKSIZE;
					}
				}
			}

		}
	}

	return true;
}

//ストリームへの書き込み　+DASH ～ -DASH
bool SXBSchDash::writeCe3(SWriteCE3& wce3,const SPoint* pptOrigin)
{
	//return writeCe3Ex(wce3,pptOrigin,"DASH");
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
	int ctx1 = m_ptCt1.x();
	int cty1 = m_ptCt1.y();
	int ctx2 = m_ptCt2.x();
	int cty2 = m_ptCt2.y();
	if(m_bCurve && pptOrigin){
		ctx1 -= pptOrigin->x();
		cty1 -= pptOrigin->y();
		ctx2 -= pptOrigin->x();
		cty2 -= pptOrigin->y();
	}
	
	wce3.WriteRecord(_T("+DASH"));
	wce3.WriteRecordInt(_T("L"),m_Layer);
	wce3.WriteRecordInt(_T("X1"),x1);
	wce3.WriteRecordInt(_T("Y1"),y1);
	wce3.WriteRecordInt(_T("X2"),x2);
	wce3.WriteRecordInt(_T("Y2"),y2);
	wce3.WriteRecordInt(_T("CURV"),(m_bCurve?1:0));
	wce3.WriteRecordInt(_T("CTX1"),ctx1);
	wce3.WriteRecordInt(_T("CTY1"),cty1);
	wce3.WriteRecordInt(_T("CTX2"),ctx2);
	wce3.WriteRecordInt(_T("CTY2"),cty2);

	wce3.WriteRecordInt(_T("WDT"),m_lineWidth);

	if(m_lineStyle==SXBSchDash::LINESTYLE_NORMAL){
		wce3.WriteRecordString(_T("LS"),_T("NORM"));
	}else if(m_lineStyle==SXBSchDash::LINESTYLE_LINEDOT){
		wce3.WriteRecordString(_T("LS"),_T("LDT"));
	}else if(m_lineStyle==SXBSchDash::LINESTYLE_LINEDOTDOT){
		wce3.WriteRecordString(_T("LS"),_T("LDDT"));
	}else{
		wce3.WriteRecordString(_T("LS"),_T("DASH"));
	}


	if(m_startPointStyle==SXBSchDash::ENDPOINTSTYLE_ARROW){
		wce3.WriteRecordString(_T("SSTL"),_T("ARRW"));
	}else if(m_startPointStyle==SXBSchDash::ENDPOINTSTYLE_CIRCLE){
		wce3.WriteRecordString(_T("SSTL"),_T("CRCL"));
	}else{
		wce3.WriteRecordString(_T("SSTL"),_T("NORM"));
	}

	if(m_endPointStyle==SXBSchDash::ENDPOINTSTYLE_ARROW){
		wce3.WriteRecordString(_T("ESTL"),_T("ARRW"));
	}else if(m_endPointStyle==SXBSchDash::ENDPOINTSTYLE_CIRCLE){
		wce3.WriteRecordString(_T("ESTL"),_T("CRCL"));
	}else{
		wce3.WriteRecordString(_T("ESTL"),_T("NORM"));
	}

	wce3.WriteRecordInt(_T("EMS"),m_endMarkSize);

	wce3.WriteRecord(_T("-DASH"));
	wce3.WriteEOL();
	return true;
}


//複製を行う
SXBSchObj* SXBSchDash::duplicate()
{
	SXBSchObj* newObj =new SXBSchDash(*this);
	((SXBSchDash*)newObj)->m_bCurve = this->m_bCurve;
	((SXBSchDash*)newObj)->m_ptCt1 = this->m_ptCt1;
	((SXBSchDash*)newObj)->m_ptCt2 = this->m_ptCt2;
	((SXBSchDash*)newObj)->m_lineStyle = this->m_lineStyle;
	((SXBSchDash*)newObj)->m_lineWidth = this->m_lineWidth;
	((SXBSchDash*)newObj)->m_startPointStyle = this->m_startPointStyle;
	((SXBSchDash*)newObj)->m_endPointStyle = this->m_endPointStyle;
	((SXBSchDash*)newObj)->m_endMarkSize = this->m_endMarkSize;
	((SXBSchDash*)newObj)->m_arrayNode = NULL;
	((SXBSchDash*)newObj)->m_nodeCacheValid = false;
	return newObj;
}

//指定点が操作可能なものの上にあるかどうか
int SXBSchDash::isEditablePoint(const SPoint& pt,int& info)
{
	SRect rc = SRect(pt.x()-2,pt.y()-2,4,4);

	if(m_Selection & SXBSchObj::SELECT_P2){
		if(rc.intersect(m_p2)){
			info = SXBSchLine::EDITPOINT_P2;
			return ON_NODE;
		}
	}
	if(m_Selection & SXBSchObj::SELECT_P1){
		if(rc.intersect(m_p1)){
			info = SXBSchLine::EDITPOINT_P1;
			return ON_NODE;
		}
	}

	if(m_bCurve){
		if(m_Selection & SXBSchObj::SELECT_P2){
			if(rc.intersect(m_ptCt2)){
				info = SXBSchLine::EDITPOINT_C2;
				return ON_CTRLPOINT;
			}
		}
		if(m_Selection & SXBSchObj::SELECT_P1){
			if(rc.intersect(m_ptCt1)){
				info = SXBSchLine::EDITPOINT_C1;
				return ON_CTRLPOINT;
			}
		}
	}
	info = 0;
	return 0;
}


//インデックスを使った座標の設定
//端点の移動は制御点の移動もともなう
void SXBSchDash::setPointIndex(int index,SPoint pt){
	m_nodeCacheValid=false;
	if(index == EDITPOINT_P1 || index == EDITPOINT_P2){
		if(m_bCurve){
			int curx,cury;
			if(index == EDITPOINT_P1){
				curx=m_p1.x();
				cury=m_p1.y();
				m_p1 = pt;
			}else{
				curx=m_p2.x();
				cury=m_p2.y();
				m_p2 = pt;
			}
			SPoint ptOffset = SPoint(pt.x()-curx,pt.y()-cury);
			if(index == EDITPOINT_P1){
				m_ptCt1+=ptOffset;
			}else{
				m_ptCt2+=ptOffset;
			}
		}else{
			if(index == EDITPOINT_P1){
				m_p1 = pt;
			}else{
				m_p2 = pt;
			}
		}
	}else{
		if(index == EDITPOINT_C1){
			m_ptCt1 = pt;
		}else{
			m_ptCt2 = pt;
		}
	}
}


//有効かどうか。両端点が一致していても曲線で制御点が生きていれば有効。
bool  SXBSchDash::qValid()
{
	if(m_p1 != m_p2) return true;
	if(!m_bCurve){
		return false;
	}else{
		if(m_p1 != m_ptCt1 || m_p2 != m_ptCt2) return true;
		else return false;
	}
}


//位置を動かす
void  SXBSchDash::move(int offsetx,int offsety)
{
	TRACE("DASH MOVE\n");
	m_nodeCacheValid = false;
	SPoint p(offsetx,offsety);
	switch(m_Selection & SELECT_ALL){
	case NOT_SELECTED:
	case SELECT_ALL:
		m_ptCt1 += p;
		m_ptCt2 += p;
		break;
	case SELECT_P1:
		m_ptCt1 += p;
		break;
	case SELECT_P2:
		m_ptCt2 += p;
		break;
	}
	SXBSchObj::move(offsetx,offsety);
}	



//X座標を指定して左右反転を行う
void  SXBSchDash::mirrorH(int x)
{
	m_nodeCacheValid = false;
	switch(m_Selection & SELECT_ALL){
	case NOT_SELECTED:
	case SELECT_ALL:
		m_ptCt1.setX(x*2 -m_ptCt1.x());
		m_ptCt2.setX(x*2 -m_ptCt2.x());
		break;
	case SELECT_P1:
		m_ptCt1.setX(x*2 -m_ptCt1.x());
		break;
	default:
//	case SELECT_P2:
		m_ptCt2.setX(x*2 -m_ptCt2.x());
		break;
	}
	SXBSchLine::mirrorH(x);
}

//左右反転を行う
void  SXBSchDash::mirrorH()
{
	m_nodeCacheValid = false;
	switch(m_Selection & SELECT_ALL){
	case NOT_SELECTED:
	case SELECT_ALL:
		int x= (m_p1.x() + m_p2.x())/2;
		mirrorH(x);
		break;
	}
}

//XY中心座標を指定して、回転を行う
void  SXBSchDash::rotate(const SPoint& p)
{
	m_nodeCacheValid = false;
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
		newY = p.y() + (m_ptCt1.x() - p.x()) ;
		newX = p.x() - (m_ptCt1.y() - p.y()) ;
		m_ptCt1.setY( newY );
		m_ptCt1.setX( newX );
	}
	if(rot2){
		newY = p.y() + (m_ptCt2.x() - p.x()) ;
		newX = p.x() - (m_ptCt2.y() - p.y()) ;
		m_ptCt2.setY( newY );
		m_ptCt2.setX( newX );
	}
	SXBSchLine::rotate(p);
}

//回転を行う
void  SXBSchDash::rotate()
{
	m_nodeCacheValid = false;
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


//曲線フラグの入出力
void SXBSchDash::setCurve(bool curve){
	m_nodeCacheValid=false; 

	if(!m_bCurve && curve){
		setDefaultCtrlPos();
	}

	m_bCurve = curve;
	if(!curve){
		//m_nodeCount = 0;
		delete[]m_arrayNode;
		m_arrayNode = NULL;
	}
}


//曲線の制御点をデフォルト位置に設定する。
void SXBSchDash::setDefaultCtrlPos()
{
	if(m_p1==m_p2){
		m_ptCt1 = SPoint(m_p1.x()+10,m_p1.y());
		m_ptCt2 = SPoint(m_p1.x()-10,m_p1.y());
		return;
	}
	int dx = m_p2.x()-m_p1.x();
	int dy = m_p2.y()-m_p1.y();
	if(abs(dx)>80 || abs(dy)>80){	//元の線に十分な長さがあるときは、制御点を両端点から内側1/4の位置にする
		m_ptCt1 = SPoint(m_p1.x()+dx/4, m_p1.y()+dy/4);
		m_ptCt2 = SPoint(m_p2.x()-dx/4, m_p2.y()-dy/4);
	}else{ //元の線が短いときは、反対側の端点を超えた位置にする
		m_ptCt1 = SPoint(m_p1.x()+dx*3/2, m_p1.y()+dy*3/2);
		m_ptCt2 = SPoint(m_p2.x()-dx*3/2, m_p2.y()-dy*3/2);
	}
}


//曲線の選択テスト用のノードを生成する
void SXBSchDash::createNodes()
{
	if(!m_bCurve){
		return;	//曲線でないときは何もしない。
	}

	if(m_arrayNode==NULL){
		m_arrayNode = new SPoint[SBezier::m_nBezierNodes+1];
		//m_nodeCount =m_nBezierNodes;
	}

	SBezier::CreateNodes(m_p1,m_p2,m_ptCt1,m_ptCt2,m_arrayNode);
	m_rcCurve = SRect(m_p1,m_p2);
	m_rcCurve.normalize();
	for(int i=1;i<SBezier::m_nBezierNodes;i++){
		m_rcCurve |= m_arrayNode[i];
	}

	m_nodeCacheValid = true;
}
