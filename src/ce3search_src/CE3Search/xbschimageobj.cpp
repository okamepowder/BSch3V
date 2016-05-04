/****************************************************************************
    BSch3V schematic capture
	Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

/****************************************************************************
** XBSch図面要素イメージオブジェクト
****************************************************************************/
#include "stdafx.h"
#include <stdio.h>
//#include <qnamespace.h>
#include <string>
using namespace std;
#include "ce3io.h"
#include "xbschobj.h"
#include "imagedata.h"
#include "xbschimageobj.h"

SXBSchImageObj::SXBSchImageObj(void)
{
	m_pImageData = NULL;
	m_drawMagnification		= 100;
}

SXBSchImageObj::~SXBSchImageObj(void)
{
	DeleteImageData();
}

void SXBSchImageObj::SetDrawMagnification(int n)
{
	if(n<10)n=10;
	else if(n>200) n= 200;
	m_drawMagnification=n;
}

bool SXBSchImageObj::LoadDIB(const TCHAR* fname)
{
	DeleteImageData();
	m_pImageData = new SImageDIB;
	if(!m_pImageData->Load(fname)){
		delete m_pImageData;
		m_pImageData = NULL;
		return false;
	}
	m_pImageData->m_refCount++;
	return true;
}




int	SXBSchImageObj::ImageWidth()
{
	if(m_pImageData==NULL){
		return 0;
	}else{
		return m_pImageData->ImageWidth();
	}
}


int	SXBSchImageObj::ImageHeight()
{
	if(m_pImageData==NULL){
		return 0;
	}else{
		return m_pImageData->ImageHeight();
	}
}


void SXBSchImageObj::DeleteImageData()
{
	if(m_pImageData!=NULL){
		TRACE("SXBSchImageObj::DeleteImageData() %d\n",m_pImageData->m_refCount);
		if(m_pImageData->m_refCount>1){
			m_pImageData->m_refCount--;
		}else{
			delete m_pImageData;
		}
	}
	m_pImageData = NULL;
}


bool SXBSchImageObj::writeCe3(SWriteCE3& wce3,const SPoint* pptOrigin)
{
	int x = m_p1.x();
	int y = m_p1.y();
	if(pptOrigin){
		x -= pptOrigin->x();
		y -= pptOrigin->y();
	}

	wce3.WriteRecord(_T("+IMAGEOBJECT"));
	wce3.WriteRecordInt(_T("L"),m_Layer);
	wce3.WriteRecordInt(_T("X"),x);
	wce3.WriteRecordInt(_T("Y"),y);
	wce3.WriteRecordInt(_T("MAG"),m_drawMagnification);
	m_pImageData->Save(wce3);

	wce3.WriteRecord(_T("-IMAGEOBJECT"));
	wce3.WriteEOL();
	return true;
}


bool SXBSchImageObj::readCe3(SReadCE3& rce3,const SPoint* pptOrigin,SXBSchDoc*)
{
	DeleteImageData();
	m_drawMagnification		= 100;
	
	wstring str(_T(""));
	bool bInitX1 = false;
	bool bInitY1 = false;
	bool bSetImage = false;
	while(1){
		if(rce3.ReadRecord(str)==EOF) return false;
		if(str[0] == '-'){
			if( (str != _T("-IMAGEOBJECT")) || !bInitX1 || !bInitY1 || !bSetImage){
				return false;
			}else{
				break;	//break while(1){ }
			}
		}else if(str[0] == '+'){
			if(!bSetImage){
				if(str == _T("+IMAGE_DIB")){
					m_pImageData = new SImageDIB;
					if(!m_pImageData->Load(rce3)){
						delete m_pImageData;
						m_pImageData = NULL;
					}else{
						bSetImage = true;
						m_pImageData->m_refCount = 1;
					}
				}else{
					str[0]='-';
					if(rce3.SkipTo(str)==EOF) return false;
				}
			}else{
				str[0]='-';
				if(rce3.SkipTo(str)==EOF) return false;
			}
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

				if(var==_T("MAG")){
					if(nParam>=IMAGEOBJ_MIN_MAGNIFICATION && nParam<=IMAGEOBJ_MAX_MAGNIFICATION){
						SetDrawMagnification(nParam);
					}
				}else 
				
				if(var==_T("X")){
					if(pptOrigin) nParam += pptOrigin->x();		
					m_p1.setX(nParam);
					bInitX1 = true;
				}else 
				if(var==_T("Y")){
					if(pptOrigin) nParam += pptOrigin->y();		
					m_p1.setY(nParam);
					bInitY1 = true;
				}
			}
		}
	}



	return true;
}


SXBSchObj* SXBSchImageObj::duplicate()
{
	ASSERT(m_pImageData!=NULL);
	SXBSchImageObj* pObj = new SXBSchImageObj(*this);
	m_pImageData->m_refCount++;
	return (SXBSchObj*)pObj;
}

//占有範囲を返す
SRect SXBSchImageObj::area()
{
	int w=0;
	int h=0;

	if(m_pImageData!=NULL){
		w=m_pImageData->ImageWidth();
		h=m_pImageData->ImageHeight();
	}
	if(w==0 || h==0){
		w=20;
		h=10;
	}else{
		w = (w*m_drawMagnification)/100;
		h = (h*m_drawMagnification)/100;
	}
	return SRect(m_p1.x(),m_p1.y(),w,h);
}

SRect SXBSchImageObj::rotateRect()
{
	return area();
}


//点を指定して選択重要度を返す
int SXBSchImageObj::qSelectWeight(const SPoint& p)
{
	int nResult = PointAndRectMag(p,area());
	if(nResult) return (nResult | ON_OBJ);
	else		return 0;
}

//範囲を指定して選択を行う
unsigned SXBSchImageObj::testSelection(const SRect& rc)
{
	if( rc.intersect(area()) ){
		return SELECT_ALL;
	}else{
		return 0;
	}
}

//範囲を選択してドラッグ選択を行う
unsigned SXBSchImageObj::testSelectionForDrag(const SRect& rc)
{
	if( rc.intersect(area()) ){
		return SELECT_ALL;
	}else{
		return 0;
	}
}

//描画が必要かどうかを返す
//描画サイズが無効のときは常にtrueを返す
bool SXBSchImageObj::qRedraw(const SRect& rc)
{
	return rc.intersect(area());
}
