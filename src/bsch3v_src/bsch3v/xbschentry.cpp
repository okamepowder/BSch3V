/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

/****************************************************************************
** XBSch図面要素エントリークラスの実装
****************************************************************************/
#include "stdafx.h"
#include <stdio.h>
#include <assert.h>
#include <string>
using namespace std;
#include "ce3io.h"
#include "xbschentry.h"
#include "xbschline.h"
#include "xbschentry.h"

//RIGHT_DOWN
//m_p2
//■□□□□□□□□□□
//□■□□□□□□□□□
//□□■□□□□□□□□
//□□□■□□□□□□□
//□□□□■□□□□□□
//□□□□□■□□□□□
//□□□□□□■□□□□
//□□□□□□□■□□□
//□□□□□□□□■□□
//□□□□□□□□□■□
//□□□□□□□□□□■ m_p1

//LEFT_DOWN
//□□□□□□□□□□■ m_p2
//□□□□□□□□□■□
//□□□□□□□□■□□
//□□□□□□□■□□□
//□□□□□□■□□□□
//□□□□□■□□□□□
//□□□□■□□□□□□
//□□□■□□□□□□□
//□□■□□□□□□□□
//□■□□□□□□□□□
//■□□□□□□□□□□
//m_p1


//コンストラクタ
SXBSchEntry::SXBSchEntry()
{
	//デフォルトで左下がりになる
	m_p1=SPoint(0,10);
	m_p2=SPoint(10,0);
}

//m_p1 が m_p2の下になるように設定する。
void SXBSchEntry::normalize()
{
	if(m_p1.y() < m_p2.y()){
		SPoint point = m_p1;
		m_p1 = m_p2;
		m_p2 = point;
	}
}


//エントリーの右下がり、左下がりを取得する
unsigned SXBSchEntry::aspect()
{
	normalize();
	if(m_p1.x() < m_p2.x()) return LEFT_DOWN;
	else	return RIGHT_DOWN;
}


//エントリーの右下がり、左下がりを設定する
void SXBSchEntry::setAspect(int aspect)
{
	m_p2.setY( m_p1.y() - 10);
	if(aspect == RIGHT_DOWN){
		m_p2.setX( m_p1.x() - 10);
	}else{
		m_p2.setX( m_p1.x() + 10);
	}
}
		
/*
//ストリームからの読み込み ～-ENTRY
bool SXBSchEntry::readStream(SCsvIStream* pStrm,const SPoint* pptOrigin)
{
	if(!readStreamEx(pStrm,pptOrigin,"ENTRY")) return false;
	//Entryのm_p2を再設定する。
	setAspect(aspect());
	return true;
}
*/


bool SXBSchEntry::readCe3(SReadCE3& rce3,const SPoint* pptOrigin,SXBSchDoc*)
{
	if(!readCe3Ex(rce3,pptOrigin,_T("ENTRY"))) return false;
	setAspect(aspect());
	return true;
}

bool SXBSchEntry::writeCe3(SWriteCE3& wce3,const SPoint* pptOrigin)
{
	normalize();
	return writeCe3Ex(wce3,pptOrigin,_T("ENTRY"));
}


/*
//ストリームへの書き込み　+ENTRY ～ -ENTRY
bool SXBSchEntry::writeStream(SCsvOStream* pStrm,const SPoint* pptOrigin)
{
	normalize();
	return writeStreamEx(pStrm,pptOrigin,"ENTRY");
}
*/

//複製を行う
SXBSchObj* SXBSchEntry::duplicate()
{
	SXBSchObj* newObj =new SXBSchEntry(*this);
	return newObj;
}

//点を指定して選択重要度を返す
//int SXBSchEntry::qSelectWeight(const SPoint& p)
//{
//	int nResult = PointAndRectMag(p,area());
//	if(nResult) return (nResult | ON_OBJ);
//	else		return 0;
//}

//範囲を指定して選択を行う
//unsigned SXBSchEntry::testSelection(const SRect& rc)
//{
//	if( rc.intersects(area()) ){
//		return SELECT_ALL;
//	}else{
//		return 0;
//	}
//}

//座標の設定  
//m_p1への設定を行うとm_p2への設定を完了する。
void SXBSchEntry::setX1(int x)
{
	if(aspect()==RIGHT_DOWN){
		m_p2.setX(x-10);
	}else{
		m_p2.setX(x+10);
	}
	m_p1.setX(x);
}

void SXBSchEntry::setY1(int y)
{
	m_p1.setY(y);
	m_p2.setY(y-10);
}

void SXBSchEntry::setP1(const SPoint& p1)
{
	setX1(p1.x());
	setY1(p1.y());
}


bool SXBSchBusEntry::readCe3(SReadCE3& rce3,const SPoint* pptOrigin,SXBSchDoc*)
{
	if(!readCe3Ex(rce3,pptOrigin,_T("BENTRY"))) return false;
	setAspect(aspect());
	return true;
}

bool SXBSchBusEntry::writeCe3(SWriteCE3& wce3,const SPoint* pptOrigin)
{
	normalize();
	return writeCe3Ex(wce3,pptOrigin,_T("BENTRY"));
}


//複製を行う
SXBSchObj* SXBSchBusEntry::duplicate()
{
	SXBSchObj* newObj =new SXBSchBusEntry(*this);
	return newObj;
}
