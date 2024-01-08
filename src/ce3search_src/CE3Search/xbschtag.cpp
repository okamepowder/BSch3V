/****************************************************************************
    BSch3V schematic capture
	Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

/****************************************************************************
** XBSch図面要素タグクラスの実装
****************************************************************************/
#include "stdafx.h"
#include <stdio.h>
#include <assert.h>
#include <string>
using namespace std;
#include "ce3io.h"
#include "xbschobj.h"
#include "xbschtag.h"


//コンストラクタ
SXBSchTag::SXBSchTag()
{
	m_bHoriz = true;
	m_nType = TAG_SQ;
	m_strText =_T("");
}

bool SXBSchTag::readCe3(SReadCE3& rce3,const SPoint* pptOrigin,SXBSchDoc*)
{
	wstring str(_T(""));
	bool bInitX1 = false;
	bool bInitY1 = false;
	bool bInitDir = false;
	bool bInitText = false;
	bool bInitType = false;

	while(1){
		if(rce3.ReadRecord(str)==EOF) return false;
		if(str[0] == '-'){
			if( (str != _T("-TAG")) || !bInitX1 || !bInitY1 || !bInitDir || !bInitText || !bInitType){
				return false;
			}else{
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
			if(0<n && n<l){	// : で分けられたレコードである
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
				if(var==_T("T")){
					if(nParam>0 && nParam<=3)	m_nType = nParam;
					else						m_nType = 0;
					bInitType = true;
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
	setp2();
	return true;
}

bool SXBSchTag::writeCe3(SWriteCE3& wce3,const SPoint* pptOrigin)
{
	//char sz[32];

	int x = m_p1.x();
	int y = m_p1.y();
	int d = (m_bHoriz ? 1 : 0);
	//string str("S:");

	if(pptOrigin){
		x -= pptOrigin->x();
		y -= pptOrigin->y();
	}

	wce3.WriteRecord(_T("+TAG"));
	wce3.WriteRecordInt(_T("L"),m_Layer);
	wce3.WriteRecordInt(_T("X"),x);
	wce3.WriteRecordInt(_T("Y"),y);
	wce3.WriteRecordInt(_T("D"),d);
	wce3.WriteRecordInt(_T("T"),m_nType);
	wce3.WriteRecordString(_T("S"),m_strText);
	wce3.WriteRecord(_T("-TAG"));
	wce3.WriteEOL();
	return true;
}



//複製を行う
SXBSchObj* SXBSchTag::duplicate()
{
	SXBSchObj* newObj =new SXBSchTag(*this);
	return newObj;
}

//文字数に対応したタグの幅を求める
//文字数が12以下の場合はBSchと互換性のあるテーブルを使う
int SXBSchTag::width()
{
	static int narrayTagWidth[]={20,20,30,40,50,60,70,80,80,90,100,110,120};
	unsigned int l=m_strText.length();
	if(l<=12) return narrayTagWidth[l];
	else return ((l*8+29)/10)*10;
}


//点を指定して選択重要度を返す
int SXBSchTag::qSelectWeight(const SPoint& p)
{
	int nResult = PointAndRectMag(p,area());
	if(nResult) return (nResult | ON_OBJ);
	else		return 0;
}

//範囲を指定して選択を行う
unsigned SXBSchTag::testSelection(const SRect& rc)
{
	if( rc.intersect(area()) ){
		return SELECT_ALL;
	}else{
		return 0;
	}
}

//範囲を選択してドラッグ選択を行う
unsigned SXBSchTag::testSelectionForDrag(const SRect& rc)
{
	if( rc.intersect(area()) ){
		return SELECT_ALL;
	}else{
		return 0;
	}
}

//描画が必要かどうかを返す
bool SXBSchTag::qRedraw(const SRect& rc)
{
	return rc.intersect(area());
}

//占有範囲を返す
SRect SXBSchTag::area()
{
	if(m_bHoriz){
		return SRect(m_p1.x(),m_p1.y()-5,m_p2.x()-m_p1.x()+1,11);
	}else{
		return SRect(m_p2.x()-5,m_p2.y(),11,m_p1.y()-m_p2.y()+1);
	}
}


// m_p1が左または下になるように調整する
void SXBSchTag::normalize()
{
	int n;
	if(m_bHoriz){
		if(m_p1.x() > m_p2.x()){
			n = m_p1.x();
			m_p1.setX(m_p2.x());
			m_p2.setX(n);
		}
	}else{
		if(m_p1.y() < m_p2.y()){
			n = m_p1.y();
			m_p1.setY(m_p2.y());
			m_p2.setY(n);
		}
	}
}


SRect SXBSchTag::rotateRect()
{
	return SRect(m_p1,m_p2);
}

//XY中心座標を指定して、回転を行う
void SXBSchTag::rotate(const SPoint& p)
{
	if(m_bHoriz){
		m_bHoriz = false;
	}else{
		m_bHoriz = true;
		switch(m_nType){
		case TAG_LU:
			m_nType = TAG_RD;
			break;
		case TAG_RD:
			m_nType = TAG_LU;
			break;
		}
	}
	SXBSchObj::rotate(p);
	normalize();
}

//回転を行う
void SXBSchTag::rotate()
{
	rotate(area().center());
}

//X座標を指定して左右反転を行う
void SXBSchTag::mirrorH(int x)
{
	if(m_bHoriz){
		switch(m_nType){
		case TAG_LU:
			m_nType = TAG_RD;
			break;
		case TAG_RD:
			m_nType = TAG_LU;
			break;
		}
	}
	SXBSchObj::mirrorH(x);
	normalize();
}

SPoint SXBSchTag::pinPosition(int n) const
{
	if(n==0) return m_p1;
	else	 return m_p2;
}

void SXBSchTag::setText(const TCHAR* psz)
{
	if(psz == NULL){
		m_strText = _T("");
	}else{
		m_strText = psz;
	}
	setp2();
}

void SXBSchTag::setp2()
{
	int w = width();
	if(m_bHoriz){
		m_p2.setY(m_p1.y());
		m_p2.setX(m_p1.x()+w);
	}else{
		m_p2.setX(m_p1.x());
		m_p2.setY(m_p1.y()-w);
	}
}