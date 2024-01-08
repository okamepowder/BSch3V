/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

/****************************************************************************
** XBSch図面要素ラベルクラスの実装
****************************************************************************/
#include "stdafx.h"
#include <stdio.h>
#include <assert.h>
#include <string>
using namespace std;
#include "ce3io.h"
#include "xbschobj.h"
#include "xbschlabel.h"


//コンストラクタ
SXBSchLabel::SXBSchLabel()
{
	m_drawSizeIsValid	= false;
	m_bHoriz = true;
	m_strText = _T("");
}

bool SXBSchLabel::readCe3(SReadCE3& rce3,const SPoint* pptOrigin,SXBSchDoc*)
{
	wstring str(_T(""));
	bool bInitX1 = false;
	bool bInitY1 = false;
	bool bInitDir = false;
	bool bInitText = false;

	while(1){
		if(rce3.ReadRecord(str)==WEOF) return false;
		if(str[0] == '-'){
			if( (str != _T("-LABEL")) || !bInitX1 || !bInitY1 || !bInitDir || !bInitText){
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
				if(var==_T("X")){
					if(pptOrigin) nParam += pptOrigin->x();		
					m_p1.setX(nParam);
					bInitX1 = true;
				}else
				if(var==_T("Y")){
					if(pptOrigin) nParam += pptOrigin->y();		
					m_p1.setY(nParam);
					bInitY1 = true;
				}else
				if(var==_T("D")){
					if(nParam==0)	m_bHoriz = false;
					else			m_bHoriz = true;
					bInitDir = true;
				}else
				if(var==_T("S")){
					m_strText=str.substr(n+1);
					//if(m_strText.length()>0){
						bInitText = true;
					//}
				}
			}
		}
	}

	return true;
}

bool SXBSchLabel::writeCe3(SWriteCE3& wce3,const SPoint* pptOrigin)
{
	int x = m_p1.x();
	int y = m_p1.y();
	int d = (m_bHoriz ? 1 : 0);
	if(pptOrigin){
		x -= pptOrigin->x();
		y -= pptOrigin->y();
	}

	wce3.WriteRecord(_T("+LABEL"));
	wce3.WriteRecordInt(_T("L"),m_Layer);
	wce3.WriteRecordInt(_T("X"),x);
	wce3.WriteRecordInt(_T("Y"),y);
	wce3.WriteRecordInt(_T("D"),d);
	wce3.WriteRecordString(_T("S"),m_strText);

	wce3.WriteRecord(_T("-LABEL"));
	wce3.WriteEOL();
	return true;
}


//複製を行う
SXBSchObj* SXBSchLabel::duplicate()
{
	SXBSchObj* newObj =new SXBSchLabel(*this);
	return newObj;
}

//点を指定して選択重要度を返す
int SXBSchLabel::qSelectWeight(const SPoint& p)
{
	int nResult = PointAndRectMag(p,area());
	if(nResult) return (nResult | ON_OBJ);
	else		return 0;
}

//範囲を指定して選択を行う
unsigned SXBSchLabel::testSelection(const SRect& rc)
{
	if( rc.intersect(area()) ){
		return SELECT_ALL;
	}else{
		return 0;
	}
}

//範囲を選択してドラッグ選択を行う
unsigned SXBSchLabel::testSelectionForDrag(const SRect& rc)
{
	if( rc.intersect(area()) ){
		return SELECT_ALL;
	}else{
		return 0;
	}
}

//描画が必要かどうかを返す
bool SXBSchLabel::qRedraw(const SRect& rc)
{
	if(!m_drawSizeIsValid){
		return true;
	}
	return rc.intersect(area());
}

//占有範囲を返す
SRect SXBSchLabel::area()
{

	if(!m_drawSizeIsValid){
		int length=m_strText.length()*8;
		m_drawSize=SSize(length,10);
	}
	int h = m_drawSize.h();
	int w = m_drawSize.w();

	if(m_bHoriz){
		return SRect(m_p1.x(),m_p1.y()-h,w,h);
	}else{
		return SRect(m_p1.x()-h,m_p1.y()-w,h,w);
	}
}

void SXBSchLabel::setDrawSize(const SSize& size)
{
	m_drawSize=size;
	m_drawSizeIsValid=true;
}


SRect SXBSchLabel::rotateRect()
{
	return SRect(m_p1,m_p1);
}

//XY中心座標を指定して、回転を行う
void SXBSchLabel::rotate(const SPoint& p)
{
	m_bHoriz = !m_bHoriz;	
	SXBSchObj::rotate(p);
}

//回転を行う
void SXBSchLabel::rotate()
{
	rotate(m_p1);
}

//テキストの設定
void SXBSchLabel::setText(const TCHAR* psz)
{
	if(psz == NULL){
		m_strText = _T("");
	}else{
		m_strText = psz;
	}
	m_drawSizeIsValid=false;
}

