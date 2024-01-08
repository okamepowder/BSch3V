/****************************************************************************
    BSch3V and utility
	Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#include "stdafx.h"
#include "sptnobjtext.h"

SPtnObjText::SPtnObjText(void)
{
	m_strText = _T("");
	m_align	= 0;
	m_bHoriz = true;
	m_drawSizeIsValid = false;

	m_fontName		= _T("");
	m_fontSize		= 8;
	m_fontBold		= false;
	m_fontItalic	= false;
	m_fontUnderline	= false;
	m_fontStrikeOut	= false;
}

SPtnObjText::~SPtnObjText(void)
{
}

SPtnObjText::SPtnObjText(const SPtnObjText& obj)	//コピーコンストラクタ
{
	*this = obj;
}

SPtnObjText& SPtnObjText::operator=(const SPtnObjText& obj)		//代入演算子
{
	if(this != &obj){
		(SPtnObj&)*this = obj;	//基本クラスのメンバーのコピー

		m_p0		= obj.m_p0;
		m_strText	= obj.m_strText;
		m_align		= obj.m_align;
		m_bHoriz	= obj.m_bHoriz;
		m_drawSizeIsValid = obj.m_drawSizeIsValid;
		m_drawSize	= obj.m_drawSize;
		m_fontName		= obj.m_fontName;
		m_fontSize		= obj.m_fontSize;
		m_fontBold		= obj.m_fontBold;
		m_fontItalic	= obj.m_fontItalic;
		m_fontUnderline	= obj.m_fontUnderline;
		m_fontStrikeOut	= obj.m_fontStrikeOut;
	}
	return *this;
}

SPtnObj* SPtnObjText::duplicate()	//複製を作る
{
	return new SPtnObjText(*this);
}

bool SPtnObjText::qValid()	//有効なデータかどうかを返す。
{
	return (m_strText.length()>0);
}

//グリップの位置を動かす
void SPtnObjText::setGripPos(int grip,int x,int y)
{
	m_p0 = SPoint(x,y);
}

void SPtnObjText::move(int offsetx,int offsety) //位置を動かす
{
	SPoint ptOffset(offsetx,offsety);
	m_p0 += ptOffset;
}

void SPtnObjText::mirrorH(int x) //X座標を指定して左右反転を行う
{
	m_p0 = mirrorPoint(m_p0,x);
	if(m_bHoriz){
		if((m_align&2) == 0){ //中央位置決めでない場合は左右ビットを反転する
			m_align ^= 1;
		}
	}else{
		if((m_align&8) == 0){ //中央位置決めでない場合は上下ビットを反転する
			m_align ^= 4;
		}
	}
}

void SPtnObjText::mirrorH() //左右反転を行う
{
	SRect rc = area();
	int x = rc.center().x();
	mirrorH(x);
}

void SPtnObjText::rotate(const SPoint& p)	//XY中心座標を指定して、回転を行う
{
	m_p0 = rotatePoint(m_p0,p);
	if(m_bHoriz){	//水平から垂直に変化するときは揃え位置の前後が入れ替わる
		if((m_align&2) == 0){ //中央位置決めでない場合は最下位ビットを反転する
			m_align ^= 1;
		}
		if((m_align&8) == 0){ //中央位置決めでない場合は上下ビットを反転する
			m_align ^= 4;
		}
	}
	m_bHoriz = !m_bHoriz;	//水平垂直を反転する
}
void SPtnObjText::rotate()	//回転を行う
{
	SRect rc = area();
	SPoint p = rc.center();
	rotate(p);
}

bool SPtnObjText::testIntersect(const SRect& rc) //範囲を指定して選択チェックを行う
{
	SRect rc1 = area();
	if(!rc.intersect( SPoint(rc1.l(),rc1.t()) ) ) return false;
	if(!rc.intersect( SPoint(rc1.r(),rc1.b()) ) ) return false;
	return true;
}
bool SPtnObjText::testIntersect(const SPoint& pt)	//点を指定して選択チェックを行う
{
	SRect rc = area();
	if(!rc.intersect( pt )) return false;
	return true;
}

SRect SPtnObjText::area()	//占有範囲を返す
{
	SRect rc;
	SSize size;
	int offset;
	if(m_drawSizeIsValid) size = m_drawSize;
	else size=SSize(m_strText.length()*8,8);

	if(m_fontName.length()==0){
		if((m_align&0x0C)==0){
			offset = size.h()/4;
		}else if((m_align&0x0C)==4){
			offset = size.h();
		}else{
			offset = size.h()*5/8;
		}
	}else{
		if((m_align&0x0C)==0){
			offset = 0;
		}else if((m_align&0x0C)==4){
			offset = size.h();
		}else{
			offset = size.h()/2;
		}
	}



	if(m_bHoriz){
		rc.setB(m_p0.y()+offset);
		rc.setT(m_p0.y()+offset-size.h());
		if((m_align&3) == 2){
			rc.setL(m_p0.x()-size.w()/2);
			rc.setR(m_p0.x()+(size.w()-size.w()/2));
		}else if((m_align&3) == 1){
			rc.setL(m_p0.x()-size.w());
			rc.setR(m_p0.x());
		}else{
			rc.setL(m_p0.x());
			rc.setR(m_p0.x()+size.w());
		}
	}else{
		rc.setR(m_p0.x()+offset);
		rc.setL(m_p0.x()+offset-size.h());
		if((m_align&3) == 2){
			rc.setB(m_p0.y()+size.w()/2);
			rc.setT(m_p0.y()-(size.w()-size.w()/2));
		}else if((m_align&3) == 1){
			rc.setB(m_p0.y()+size.w());
			rc.setT(m_p0.y());
		}else{
			rc.setB(m_p0.y());
			rc.setT(m_p0.y()-size.w());
		}
	}
	//TRACE("SPtnObjText::area() %d,%d,%d,%d\n",rc.l(),rc.t(),rc.r(),rc.b());
	return rc;
}

void SPtnObjText::setDrawSize(SSize size)
{
	m_drawSize = size;
	m_drawSizeIsValid = true;
}
void SPtnObjText::setText(const TCHAR* psz)
{
	if(psz == NULL){
		m_strText = _T("");
	}else{
		m_strText = psz;
	}
	m_drawSizeIsValid = false;
}


bool SPtnObjText::readCe3(SReadCE3& rce3)
{
	wstring str(_T(""));
	bool bInitX = false;
	bool bInitY = false;
	bool bInitText = false;

	while(1){
		if(rce3.ReadRecord(str)==EOF) return false;
		if(str[0] == '-'){
			if( (str != _T("-TX")) || !bInitX || !bInitY || !bInitText){
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
			if(0<n && n<(l-1)){	// : で分けられたレコードである
				wstring var = str.substr(0,n);				//先頭から:の手前まで
				int nParam = _tstoi(str.substr(n+1).c_str()); //:から終端までを数値に変換
				if(var==_T("X")){
					m_p0.setX(nParam);
					bInitX = true;
				}else
				if(var==_T("Y")){
					m_p0.setY(nParam);
					bInitY = true;
				}else
				if(var==_T("D")){
					m_bHoriz = (nParam!=0);
				}else
				if(var==_T("A")){
					m_align = nParam;
				}else
				if(var==_T("S")){
					m_strText=str.substr(n+1);
					bInitText = true;
				}else
				if(var==_T("FN")){	//フォント名
					m_fontName=str.substr(n+1);
				}else
				if(var==_T("FS")){	//フォントサイズ
					setFontSize(nParam);
				}else
				if(var==_T("FF")){	//フォントフラグ
					wstring val=str.substr(n+1);
					int n=val.length();
					for(int i=0;i<n;i++){
						switch(val[i]){
						case 'B': m_fontBold		= true; break;
						case 'I': m_fontItalic		= true; break;
						case 'U': m_fontUnderline	= true; break;
						case 'S': m_fontStrikeOut	= true; break;
						}
					}
				}
			}
		}
	}
	return true;
}

void SPtnObjText::setFontName(const TCHAR* psz)
{
	if(psz == NULL){
		m_fontName = _T("");
	}else{
		m_fontName = psz;
	}
	m_drawSizeIsValid = false;
}

void SPtnObjText::setFontSize(int fontSize)
{
	if(fontSize<4) fontSize=4;
	else if(fontSize>80) fontSize=80;
	m_fontSize = fontSize;
	m_drawSizeIsValid = false;
}
//ボールド設定
void SPtnObjText::setFontBold(bool istrue)
{
	m_fontBold=istrue;
	m_drawSizeIsValid = false;
}

//イタリック設定
void SPtnObjText::setFontItalic(bool istrue)
{
	m_fontItalic=istrue;
	m_drawSizeIsValid = false;
}

//アンダーライン設定
void SPtnObjText::setFontUnderline(bool istrue)
{
	m_fontUnderline=istrue;
	m_drawSizeIsValid = false;
}

//取り消し線設定
void SPtnObjText::setFontStrikeOut(bool istrue)
{
	m_fontStrikeOut=istrue;
	m_drawSizeIsValid = false;
}

bool SPtnObjText::writeCe3(SWriteCE3& wce3,SPoint* pOrigin)
{
	int x = m_p0.x();
	int y = m_p0.y();
	if(pOrigin){
		x -= pOrigin->x();
		y -= pOrigin->y();
	}
	int d = (m_bHoriz ? 1 : 0);
	int a = m_align;

	wce3.WriteRecord(_T("+TX"));
	wce3.WriteRecordInt(_T("X"),x);
	wce3.WriteRecordInt(_T("Y"),y);
	wce3.WriteRecordInt(_T("A"),a);
	wce3.WriteRecordInt(_T("D"),d);
	wce3.WriteRecordString(_T("S"),m_strText);
	wce3.WriteRecordString(_T("FN"),m_fontName);
	wce3.WriteRecordInt(_T("FS"),m_fontSize);
	wstring str = _T("FF:");
	if(m_fontBold)		str = str + _T("B");
	if(m_fontItalic)	str = str + _T("I");
	if(m_fontUnderline)	str = str + _T("U");
	if(m_fontStrikeOut) str = str + _T("S");
	wce3.WriteRecord(str);

	wce3.WriteRecord(_T("-TX"));
	wce3.WriteEOL();
	return true;
}
