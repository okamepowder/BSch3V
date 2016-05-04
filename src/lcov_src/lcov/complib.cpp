/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

//部品ライブラリに関係する３つのクラスのインプリメント
#include "stdafx.h"
#include <string.h>
#include <assert.h>
#include <math.h>
#include <list>
#include <string>
using namespace std;

//#include <qstring.h>
#include <ctype.h>
#include "coord.h"
#include "ce3io.h"
#include "complib.h"
//#include "xbsch.h"


typedef TCHAR* PCSTRING;
#define M_PI       3.14159265358979323846

//#define NOT_BSCH3V		//BSch3V以外では、これを定義しておくと、読み込みが速くなる。

////////////////////////////////////////////////
// パターンクラス

SPtn::~SPtn(){				//デストラクタ
		delete[]m_pszName;
		delete[]m_pBuff;
		//delete[]m_pvBuff;
		for(int n=0;n<m_vPtnCount;n++){
			SPtnObj* pObj = m_ppvPtn[n];
			delete pObj;
		}
		delete[]m_ppvPtn;
	}


//コピーコンストラクタ
SPtn::SPtn(const SPtn& ptn)
{
	m_pszName=NULL;
	m_nBuffsize=0;
	m_pBuff=NULL;
	m_size=SSize(0,0);
	m_vPtnCount=0;
	m_ppvPtn=NULL;

	*this = ptn;
}

//代入演算子	
SPtn& SPtn::operator=(const SPtn& ptn)
{
	if(this != &ptn){
		delete[]m_pszName;
		delete[]m_pBuff;
		if(ptn.m_pszName != NULL){
			m_pszName = new TCHAR[_tcslen(ptn.m_pszName)+1];
			_tcscpy(m_pszName,(ptn.m_pszName));
		}else{
			m_pszName=NULL;
		}

		m_size = ptn.m_size;
		if(ptn.m_pBuff != NULL){
			m_pBuff = (unsigned char*)new char[m_nBuffsize = ptn.m_nBuffsize];
			for(int i=0;i<m_nBuffsize;i++) m_pBuff[i]=ptn.m_pBuff[i];
		}else{
			m_nBuffsize=0;
			m_pBuff=NULL;
		}


		for(int n=0;n<m_vPtnCount;n++){
			SPtnObj* pObj = m_ppvPtn[n];
			delete pObj;
		}
		delete[]m_ppvPtn;
		m_ppvPtn = NULL;
		m_vPtnCount = ptn.m_vPtnCount;
		if(m_vPtnCount>0){
			m_ppvPtn = new PSPtnObj[m_vPtnCount];
			for(int n=0;n<m_vPtnCount;n++){
				m_ppvPtn[n] =  ptn.m_ppvPtn[n]->duplicate();
			}
		}
	}
	return *this;
}

bool SPtn::writeCe3(SWriteCE3& wce3,const TCHAR* pszName)
{
	int x = m_size.w();
	int y = m_size.h();
	wce3.WriteRecord(_T("+PTN"));
	if(pszName != NULL){
		wce3.WriteRecordString(_T("N"),pszName);
	}else{
		wce3.WriteRecordString(_T("N"),m_pszName);
	}
	wce3.WriteRecordInt(_T("X"),x);
	wce3.WriteRecordInt(_T("Y"),y);
	wce3.WriteEOL();
	if(m_pBuff){
		wce3.WriteRecord(_T("+BMP"));	wce3.WriteEOL();

		TCHAR linebuff[128];
		int buffXByte = ((x+15)/16)*2;	//バッファの水平方向バイト数
		int xByte = (x+7)/8;			//書き込みの水平方向バイト数。2文字で1バイトなので読み込み文字はこれの倍
		int buffOffset;
		int i,j,k;
		for(i=0;i<y;i++){
			buffOffset = buffXByte*i;
			for(j=0,k=0;j<xByte;j++){
				if(buffOffset>=m_nBuffsize) break;
				linebuff[k++]=((m_pBuff[buffOffset]&0xf0)>>4)+'@';
				linebuff[k++]=(m_pBuff[buffOffset]&0xf)      +'@';
				buffOffset++;
			}
			linebuff[k]='\0';
			wce3.WriteRecord(linebuff); wce3.WriteEOL();
		}
		wce3.WriteRecord(_T("-BMP"));	wce3.WriteEOL();
	}

	for(int i=0;i<m_vPtnCount;i++){
		m_ppvPtn[i]->writeCe3(wce3);
	}
	wce3.WriteRecord(_T("-PTN"));	wce3.WriteEOL();
	return true;
}



//#define VECTPTNBUFF_SIZE 32768

bool SPtn::readCe3(SReadCE3& rce3)
{
	SPtnObjList listPtn;	//ベクトルパターンのリスト
	bool retv = true;
	

	wstring name(_T(""));
	int x=0;
	int y=0;

	m_nBuffsize=0;
	delete[]m_pBuff;
	m_pBuff=NULL;

	for(int n=0;n<m_vPtnCount;n++){
		SPtnObj* pObj = m_ppvPtn[n];
		delete pObj;
	}
	delete[]m_ppvPtn;

	m_vPtnCount = 0;  //Version 0.52
	m_ppvPtn = NULL;  //Version 0.52



	m_pBuff = NULL;
	int vBuffCount=0;
	int n,i;

//	unsigned char* ptnBuff=NULL;
//	SPtn* pPtn=NULL;

	wstring str(_T(""));
	while(rce3.ReadRecord(str)!=WEOF){
		if(str[0] == '-'){
			if(str != _T("-PTN")){//Version 0.52
//			if( (str != "-PTN") || (m_pBuff==NULL && listPtn.size()==0)){
				retv = false;
			}
			break;
		}else if(str[0] == '+'){
			SPtnObj* pPtnObj=NULL;
			if(str==_T("+BMP") && m_size.w()>0 && m_size.h()>0 && name!=_T("") && m_pBuff==NULL){
				m_nBuffsize=((m_size.w()+15)/16)*2*m_size.h();
				if(NULL==(m_pBuff=readBmp(rce3))) return false;
			}else if(str==_T("+L")){
				pPtnObj = new SPtnObjLine;
			}else if(str==_T("+C")){
				pPtnObj = new SPtnObjCircle;
			}else if(str==_T("+PG")){
				pPtnObj = new SPtnObjPolygon;
			}else if(str==_T("+AR")){
				pPtnObj = new SPtnObjArc;
			}else if(str==_T("+TX")){
				pPtnObj = new SPtnObjText;
			}else{
				str[0]='-';
				if(rce3.SkipTo(str) == WEOF){
					retv= false;
					break;
				}
			}
			if(pPtnObj!=NULL){
				if(pPtnObj->readCe3(rce3)){
					listPtn.push_back(pPtnObj);
				}else{
					delete pPtnObj;
				}
			}
		}else{
			int n;
			int l=str.length();
			for(n=0;n<l;n++){
				if(str[n]==':') break;
			}
			if(0<n && n<(l-1)){	// : で分けられたレコードである
				wstring var = str.substr(0,n);				//先頭から:の手前まで
				//int nParam = _tstoi(str.substr(n+1).c_str()); //:から終端までを数値に変換
				if(var==_T("X")){
					x = _tstoi(str.substr(n+1).c_str()); //:から終端までを数値に変換
					m_size.setW(x);
				}else
				if(var==_T("Y")){
					y = _tstoi(str.substr(n+1).c_str()); //:から終端までを数値に変換
					m_size.setH(y);
				}else
				if(var==_T("N")){
					name = str.substr(n+1).c_str();
					m_pszName = new TCHAR[name.length()+1];
					_tcscpy(m_pszName,name.c_str());
				}
			}
		}
	}
	if(listPtn.size()>0){
		m_vPtnCount = listPtn.size();
		m_ppvPtn = new PSPtnObj[m_vPtnCount];
		SPtnObjListIterator ite = listPtn.begin();
		int i=0;
		while(ite != listPtn.end()){
			m_ppvPtn[i] = (*ite);
			i++;
			ite++;
		}
	}
	return retv;
//RdErr:
//	delete[]ptnBuff;
//	return false;
}




void SPtn::setSize(const SSize& sizePixel,bool nobit)
{
	int i,j;
	int newX=sizePixel.w();
	int newY=sizePixel.h();
	static unsigned char rightMaskTable[]={0xFF,0x80,0xC0,0xE0,0xF0,0xF8,0xFC,0xFE};

	if(nobit){
		delete[]m_pBuff;
		m_pBuff=NULL;
		m_nBuffsize = 0;
	}else{
		//新しいバッファの準備
		assert(newX>0 && newY>0);
		if(newX>501) newX=501;					//xもｙも50グリッドぶんを上限とする。
		if(newY>501) newY=501;					//
		int newBuffXByte = ((newX+15)/16)*2;	//バッファの水平方向バイト数
		int newBuffSize = newBuffXByte*newY;
		unsigned char* newBuff= (unsigned char*)new char[newBuffSize];
		for(i=0;i<newBuffSize;i++) newBuff[i]=0;
		
		if(m_pBuff!=NULL){
			//旧バッファ関連の変数設定
			int x=m_size.w();
			int y=m_size.h();
			int buffXByte = ((x+15)/16)*2;	//バッファの水平方向バイト数

			//コピーするバイト数と右端のマスク値の設定
			int xCopyPixel =(newX < x ? newX : x);
			int xCopyByte  =(xCopyPixel+7)/8;
			unsigned char rightMask = rightMaskTable[xCopyPixel%8];
			int yCopyLine  =(newY < y ? newY : y);
		
			unsigned char* srcLeft  = m_pBuff;
			unsigned char* destLeft = newBuff;
			for(i=0;i<yCopyLine;i++){
				for(j=0;j<xCopyByte;j++) destLeft[j]=srcLeft[j];
				destLeft[xCopyByte-1] &= rightMask;
				srcLeft  += buffXByte;
				destLeft += newBuffXByte;
			}
			delete[]m_pBuff;
			m_pBuff=NULL;
		}
		m_pBuff = newBuff;
		m_nBuffsize = newBuffSize;
	}
	m_size.setW(newX);
	m_size.setH(newY);
}

////////////////////////////////////////////////
// ピンクラス

//コンストラクタ
SPin::SPin()
{
	m_nBlock	= 0;			//m_nBlockの数だけピン番号文字列配列を管理している
	m_ppszNum	= NULL;			//ピン番号文字列配列へのポインタ
	m_pszName	= NULL;			//ピン名へのポインタ
	m_nType		= 0;			//ピンのタイプ
	m_nLTRB		= PIN_LOC_L;	//位置 LTRB
	m_nOffset	= 0;			//位置 オフセット
	m_pinNameDisplayFlag = 0xFFFFFFFF;
}

//デストラクタ
SPin::~SPin()
{
	deleteBuff();
}

//バッファのクリア
void SPin::deleteBuff()
{
	delete[]m_pszName;
	m_pszName=NULL;
	int n;
	for(n=0;n<m_nBlock;n++){
		TCHAR* pc = m_ppszNum[n];
		delete[]pc;
	}
	delete[]m_ppszNum;
	m_ppszNum=NULL;
}



//コピーコンストラクタ
SPin::SPin(const SPin& pin)
{
	m_nBlock	= pin.m_nBlock;
	m_nType		= pin.m_nType;
	m_nLTRB		= pin.m_nLTRB;
	m_nOffset	= pin.m_nOffset;
	m_pinNameDisplayFlag = pin.m_pinNameDisplayFlag;
	if(m_nBlock>0){
		m_ppszNum = new PCSTRING[m_nBlock];
		for(int i=0;i<m_nBlock;i++){
			if(pin.m_ppszNum[i]!=NULL){
				m_ppszNum[i]=new TCHAR[_tcslen(pin.m_ppszNum[i])+1];
				_tcscpy(m_ppszNum[i],pin.m_ppszNum[i]);
			}else{
				m_ppszNum[i]=NULL;
			}
		}
	}else{
		m_ppszNum = NULL;
	}
	if(pin.m_pszName!=NULL){
		m_pszName = new TCHAR[_tcslen(pin.m_pszName)+1];
		_tcscpy(m_pszName,pin.m_pszName);
	}else{
		m_pszName = NULL;
	}
}

bool SPin::readCe3(SReadCE3& rce3,int block)
{
	deleteBuff();

	setBlock (block);
	int nPinSetCount=0;

	m_pinNameDisplayFlag = 0xFFFFFFFF;

	wstring str(_T(""));
	while(rce3.ReadRecord(str)!=WEOF){
		if(str[0] == '-'){
			if(str != _T("-PIN")){
				return false;
			}else{
				break;
			}
		}else if(str[0] == '+'){
			str[0]='-';
			if(rce3.SkipTo(str) == WEOF) return false;
		}else{
			int n;
			int l=str.length();
			for(n=0;n<l;n++){
				if(str[n]==':') break;
			}
			if(0<n && n<l){	// : で分けられたレコードである
				wstring var = str.substr(0,n);				//先頭から:の手前まで
				n++;
				if(var==_T("N")){
					setPinName(str.substr(n).c_str());
				}else if(var==_T("DF")){
					TCHAR *pc;
					m_pinNameDisplayFlag = _tcstoul(str.substr(n).c_str(),&pc,16);
				}else if(var==_T("L") && n<l){			//0.63.01
					switch ((str.substr(n))[0]){
					case 'L': m_nLTRB = PIN_LOC_L; break;
					case 'T': m_nLTRB = PIN_LOC_T; break;
					case 'R': m_nLTRB = PIN_LOC_R; break;
					case 'B': m_nLTRB = PIN_LOC_B; break;
					}
					int nOffset = _tstoi(str.substr(n+1).c_str());
					if(nOffset>=0 && nOffset<=255){
						m_nOffset = nOffset;  
					}
				}else if(var==_T("T")){
					int nType=0;
					int i=0;
					int len=str.substr(n).length();
					for(i=0;i<len;i++){
						switch ((str.substr(n))[i]){
						case 'Z': nType |= PIN_TYPE_ZLENG;		break;
						case 'S': nType |= PIN_TYPE_SMALL;		break;
						case 'N': nType |= PIN_TYPE_NEGATIVE;	break;
						case 'C': nType |= PIN_TYPE_CLOCK;		break;
						case 'm': nType |= PIN_TYPE_NUMHIDE;	break;
						}
					}
					m_nType = nType;
				}else if(var==_T("M")){
					if(nPinSetCount<block){
						setPinNum(nPinSetCount,str.substr(n).c_str());
						nPinSetCount++;
					}
				}
			}	
		}
	}
	return true;
}

wstring SPin::pinNameToString(const TCHAR* pinName)
{
	wstring name=_T("");
	while(*pinName){
		if(*pinName & 0x80){
			name += '\\';
		}
		name += *pinName & 0x7F;
		pinName++;
	}
	return name;
}


//ピンの書き込み
//+PIN,N:\P\R,L:T3,T:N,M:4,M:10,-PIN
bool SPin::writeCe3(SWriteCE3& wce3)
{

	//wstring name="";
	//const char* rawpinname = pinName();
	//while(*rawpinname){
	//	if(*rawpinname & 0x80){
	//		name += '\\';
	//	}
	//	name += *rawpinname & 0x7F;
	//	rawpinname++;
	//}

	wstring name = pinNameToString(pinName());

	TCHAR szLocation[32];
	switch (m_nLTRB){
	case PIN_LOC_L: szLocation[0] = 'L'; break;
	case PIN_LOC_T: szLocation[0] = 'T'; break;
	case PIN_LOC_R: szLocation[0] = 'R'; break;
	case PIN_LOC_B: 
	default:		szLocation[0] = 'B'; break;
	}
	_stprintf(szLocation+1,_T("%d"),m_nOffset);

	TCHAR szType[32];
	TCHAR* pc=szType;
	if(m_nType & PIN_TYPE_ZLENG)	*pc++ = 'Z';
	if(m_nType & PIN_TYPE_SMALL)	*pc++ = 'S';		//	case 'S': nType |= PIN_TYPE_SMALL;		break;
	if(m_nType & PIN_TYPE_NEGATIVE)	*pc++ = 'N';		//	case 'N': nType |= PIN_TYPE_NEGATIVE;	break;
	if(m_nType & PIN_TYPE_CLOCK)	*pc++ = 'C';		//	case 'C': nType |= PIN_TYPE_CLOCK;		break;
	if(m_nType & PIN_TYPE_NUMHIDE)	*pc++ = 'm';		//	case 'm': nType |= PIN_TYPE_NUMHIDE;	break;
	*pc='\0';


	


	wce3.WriteRecord(_T("+PIN"));
	wce3.WriteRecordString(_T("N"),name);
	wce3.WriteRecordLongFlag(_T("DF"),m_pinNameDisplayFlag);

	
	wce3.WriteRecordString(_T("L"),szLocation);
	wce3.WriteRecordString(_T("T"),szType);
	for(int i=0;i<m_nBlock;i++){
		const TCHAR* cp = pinNum(i);
		if(cp==NULL){
			wce3.WriteRecordString(_T("M"),_T(""));
		}else{
			wce3.WriteRecordString(_T("M"),cp);
		}
	}
	wce3.WriteRecord(_T("-PIN"));
	return true;
}


//代入演算子	
SPin& SPin::operator=(const SPin& pin)
{
	if(this != &pin){
		deleteBuff();
		m_nBlock	= pin.m_nBlock;
		m_nType		= pin.m_nType;
		m_nLTRB		= pin.m_nLTRB;
		m_nOffset	= pin.m_nOffset;
		if(m_nBlock>0){
			m_ppszNum = new PCSTRING[m_nBlock];
			for(int i=0;i<m_nBlock;i++){
				if(pin.m_ppszNum[i]!=NULL){
					m_ppszNum[i]=new TCHAR[_tcslen(pin.m_ppszNum[i])+1];
					_tcscpy(m_ppszNum[i],pin.m_ppszNum[i]);
				}else{
					m_ppszNum[i]=NULL;
				}
			}
		}
		if(pin.m_pszName!=NULL){
			m_pszName = new TCHAR[_tcslen(pin.m_pszName)+1];
			_tcscpy(m_pszName,pin.m_pszName);
		}
	}
	return *this;
}


//ブロック数をセットする。
void SPin::setBlock(int nBlock)
{
	int n;
	TCHAR**	ppszNum = new PCSTRING[nBlock];
	for(n=0;n<nBlock;n++) ppszNum[n]=NULL;
	int m = (nBlock < m_nBlock ? nBlock : m_nBlock);
	for(n=0;n<m;n++) ppszNum[n]=m_ppszNum[n];
	for(n=nBlock;n<m_nBlock;n++){	//旧ブロックサイズの方が大きい場合は余った文字列を解放
		TCHAR* pc = m_ppszNum[n];
		delete[]pc;
	}
	delete[]m_ppszNum;
	m_ppszNum = ppszNum;
	m_nBlock = nBlock;

}

//ピン名をセットする。
void SPin::setPinName(const TCHAR* name)
{
	delete[]m_pszName;
	if(name== NULL){
		m_pszName=NULL;
		return;
	}
	TCHAR buff[256];
	int n=0;
	int m=0;

	int len = _tcslen(name);
	
	while(n<len && m<255){
		if(name[n]=='\\'){
			if(name[n+1]){
				buff[m]=name[n+1] | 0x80;
				n++;
			}
		}else{
			buff[m]=name[n];
		}
		n++;m++;
	}
	buff[m]='\0';
	m_pszName = new TCHAR[m+1];
	_tcscpy(m_pszName,buff);
}

//ピン番号をセットする。
void SPin::setPinNum(int nBlock,const TCHAR* num)
{
	if(m_nBlock==0) return;
	if(nBlock<0 || nBlock>=m_nBlock ) return;
	TCHAR* pc =m_ppszNum[nBlock];
	delete[]pc;
	if(num== NULL){
		m_ppszNum[nBlock]=NULL;
		return;
	}
	int len = _tcslen(num);
	m_ppszNum[nBlock] = new TCHAR[len+1];
	_tcscpy(m_ppszNum[nBlock],num);
}

const TCHAR* SPin::pinNum(int nBlock) const
{
	if(nBlock<0 || nBlock>=m_nBlock) return NULL;
	return m_ppszNum[nBlock];
}

////////////////////////////////////////////////////////////////////////////
//SCompInfo のインプリメント

//コピーコンストラクタ
SCompInfo::SCompInfo(const SCompInfo& info)
{
	if(info.m_pszRef != NULL){
		m_pszRef = new TCHAR[_tcslen(info.m_pszRef)+1];	//デフォルト部品番号
		_tcscpy(m_pszRef,info.m_pszRef);
	}else{
		m_pszRef = NULL;
	}
	m_size = info.m_size;						//部品のサイズ
	m_nBlock = info.m_nBlock;					//パッケージあたりのブロック数
	if(info.m_pPtn != NULL){
		m_pPtn = new SPtn(*(info.m_pPtn));		//LB3から読み込んだパターンへのポインタ
	}else{
		m_pPtn =NULL;
	}
	m_nPinCount = info.m_nPinCount;			//ピン数
	if(m_nPinCount>0){
		m_ppPin = new PSPin[m_nPinCount];	//LB3から読み込んだピンクラスの配列へのポインタ
		for(int i=0;i<m_nPinCount;i++){
			m_ppPin[i]=new SPin(*(info.m_ppPin[i]));
		}
	}else{
		m_ppPin = NULL;
	}
	m_note = info.m_note;
	m_mfr	= info.m_mfr;
	m_mfrpn = info.m_mfrpn;
	m_pkg	= info.m_pkg;
	m_ciType = info.m_ciType;
	if(m_nBlock>1){
		m_nFixedBlock = CITYPE::GENERAL;
	}else{
		m_nFixedBlock = info.m_nFixedBlock;
	}


}

//代入演算子
SCompInfo& SCompInfo::operator=(const SCompInfo& info)
{
	if(this != &info){
		deleteBuff();
		if(info.m_pszRef != NULL){
			m_pszRef = new TCHAR[_tcslen(info.m_pszRef)+1];	//デフォルト部品番号
			_tcscpy(m_pszRef,info.m_pszRef);
		}
		m_size = info.m_size;						//部品のサイズ
		m_nBlock = info.m_nBlock;					//パッケージあたりのブロック数
		if(info.m_pPtn != NULL){
			m_pPtn = new SPtn(*(info.m_pPtn));		//LB3から読み込んだパターンへのポインタ
		}
		m_nPinCount = info.m_nPinCount;			//ピン数
		if(m_nPinCount>0){
			m_ppPin = new PSPin[m_nPinCount];	//LB3から読み込んだピンクラスの配列へのポインタ
			for(int i=0;i<m_nPinCount;i++){
				m_ppPin[i]=new SPin(*(info.m_ppPin[i]));
			}
		}
		m_note = info.m_note;
		m_mfr	= info.m_mfr;
		m_mfrpn = info.m_mfrpn;
		m_pkg	= info.m_pkg;
	}
	m_ciType = info.m_ciType;
	if(m_nBlock>1){
		m_nFixedBlock = CITYPE::GENERAL;
	}else{
		m_nFixedBlock = info.m_nFixedBlock;
	}

	return *this;
}


void SCompInfo::deleteBuff()
{
	delete[]m_pszRef;
	m_pszRef = NULL;
	delete m_pPtn;
	m_pPtn = NULL;
	if(m_ppPin){
		int n;
		for(n=0;n<m_nPinCount;n++){
			SPin* pPin = m_ppPin[n];
			delete pPin;
		}
		delete[]m_ppPin;
	}
}

//デストラクタ
SCompInfo::~SCompInfo()
{
	deleteBuff();
}


static const TCHAR* TYPE_WD[3] = {_T("GEN"),_T("SHB"),_T("FXB")};

//ファイルからの読み込み
bool SCompInfo::readCe3(SReadCE3& rce3,wstring& name,wstring& ptn)
{
	deleteBuff();


	wstring ref(_T("U"));
	wstring strval;
	name = _T("");
	ptn = _T("");
	int x=0;
	int y=0;
	int b=0;
	int fb=0;

	//SCompInfo* pComp = new SCompInfo;
	//SPin* pPin;
	//SPtn* pPtn;
	//SCompIndex* pIndex;

	SPinList listPin;	//ピンのリスト
	

	wstring str(_T(""));
	while(rce3.ReadRecord(str)!=WEOF){
		if(str[0] == '-'){
			if(str != _T("-COMP") || name==_T("") || x<=0 || y<=0 || b<=0){
				goto RdErr;
			}else{
				break;
			}
		}else if(str[0] == '+'){
			if(str==_T("+PIN")){
				if(x<=0 || y<=0 || b<=0) goto RdErr;
				SPin* pPin = new SPin;
				if(!pPin->readCe3(rce3,b)){
					delete pPin;
					goto RdErr;
				}
				listPin.push_back(pPin);
			}else{
				str[0]='-';
				if(rce3.SkipTo(str) == WEOF) goto RdErr;
			}
		}else{
			int n;
			int l=str.length();
			for(n=0;n<l;n++){
				if(str[n]==':') break;
			}
			if(0<n && n<l){	// : で分けられたレコードである
				wstring var = str.substr(0,n);				//先頭から:の手前まで
				n++;
				if(var==_T("N")){
					name=str.substr(n);
				}else if(var==_T("X")){
					x=_tstoi(str.substr(n).c_str());
					if(x<0) goto RdErr;
				}else if(var==_T("Y")){
					y=_tstoi(str.substr(n).c_str());
					if(y<0) goto RdErr;
				}else if(var==_T("B")){
					if(b!=0) goto RdErr;
					b=_tstoi(str.substr(n).c_str());
					if(b<0 || b>255) goto RdErr;
				}else if(var==_T("FB")){
					if(fb!=0) goto RdErr;
					fb=_tstoi(str.substr(n).c_str());
					if(fb<0 || fb>255) goto RdErr;
				}else if(var==_T("R")){
					ref=str.substr(n);
				}else if(var==_T("P")){
					ptn=str.substr(n);
				}else if(var==_T("TYPE")){
					strval=str.substr(n);
				}else if(var==_T("NOTE")){
					m_note=str.substr(n);
				}else if(var==_T("MFR")){
					m_mfr=str.substr(n);
				}else if(var==_T("MFRPN")){
					m_mfrpn=str.substr(n);
				}else if(var==_T("PKG")){
					m_pkg=str.substr(n);
				}
			}
		}
	}

//	if(ptn!=""){	//パターンが設定ずみならパターンへのポインタを検索する。
//		SPtnListIterator ite = m_listPtn.begin();
//		while(ite != m_listPtn.end()){
//			pPtn = *ite;
//			if(pPtn->m_pszName == ptn){
//				//pComp->m_pPtn = pPtn;
//				pComp->m_pPtn = new SPtn(*pPtn);	//複数の部品定義でパターンを共有しない
//				break;
//			}
//			ite++;
//		}
//	}
	m_size.setW(x);
	m_size.setH(y);

	if(strval == TYPE_WD[(int)CITYPE::FIXED_BLOCK]){
		m_ciType = CITYPE::FIXED_BLOCK;
		m_nBlock  = 1;
		m_nFixedBlock = fb;
	}else if(strval == TYPE_WD[(int)CITYPE::SHARED_BLOCK]){
		m_ciType = CITYPE::SHARED_BLOCK;
		m_nBlock  = 1;
		m_nFixedBlock = 0;
	}else{
		m_ciType = CITYPE::GENERAL;
		m_nBlock  = b;
		m_nFixedBlock = 0;
	}


	m_pszRef = new TCHAR[ref.length()+1];
	_tcscpy(m_pszRef,ref.c_str());
	m_nPinCount = listPin.size();
	if(m_nPinCount>0){
		m_ppPin=new PSPin[m_nPinCount];
		int n;
		for(n=0;n<m_nPinCount;n++){
			m_ppPin[n]=NULL;
		}
		n=0;
		list<SPin*>::iterator ite = listPin.begin();
		while(ite != listPin.end() && n<m_nPinCount){ 
			SPin* pPin = *ite;
			m_ppPin[n++] = pPin;
			ite++;
		}
	}

//	pIndex = new SCompIndex;
//	pIndex->setName(name.c_str());
//	pIndex->setCompInfo(pComp);
//	m_listCompIndex.push_back(pIndex);
//	m_listCompInfo.push_back(pComp);

	return true;
RdErr:
	SPinListIterator ite = listPin.begin();
	while(ite != listPin.end()){ 
		SPin* pPin = *ite;
		delete pPin;
		ite++;
	}
	return false;
}


bool SCompInfo::writeCe3(SWriteCE3& wce3,const TCHAR* pszName)
{



	if(pszName == NULL || *pszName == '\0') return false;
	if(m_pPtn != NULL){
		if(!m_pPtn->writeCe3(wce3,pszName)) return false;//パターンの書き込みの際は共有を前提としない。
		//wce3.WriteEOL();
	}
	int x = m_size.w();
	int y = m_size.h();
	int b = m_nBlock;
	wce3.WriteRecord(_T("+COMP"));
	wce3.WriteRecordString(_T("N"),pszName);
	wce3.WriteEOL();

	wce3.WriteRecordInt(_T("X"),x);
	wce3.WriteRecordInt(_T("Y"),y);
	wce3.WriteRecordInt(_T("B"),b);
	wce3.WriteEOL();

	wce3.WriteRecordString(_T("TYPE"),TYPE_WD[(int)m_ciType]);
	wce3.WriteRecordInt(_T("FB"),m_nFixedBlock);
	wce3.WriteEOL();	

	wce3.WriteRecordString(_T("R"),m_pszRef);
	wce3.WriteEOL();
	if(m_pPtn != NULL){
		wce3.WriteRecordString(_T("P"),pszName);
		wce3.WriteEOL();
	}
	int n;
	for(n=0;n<m_nPinCount;n++){
		m_ppPin[n]->writeCe3(wce3);
		wce3.WriteEOL();
	}
	wce3.WriteRecordString(_T("NOTE"),m_note);
	wce3.WriteRecordString(_T("MFR"),m_mfr);
	wce3.WriteRecordString(_T("MFRPN"),m_mfrpn);
	wce3.WriteRecordString(_T("PKG"),m_pkg);
	wce3.WriteRecord(_T("-COMP"));
	wce3.WriteEOL();
	return true;
}


//デフォルト部品番号の設定
void SCompInfo::setRef(const TCHAR* ref)
{
	delete[]m_pszRef;
	m_pszRef = NULL;
	if(ref==NULL) return;
	int len = _tcslen(ref);
	m_pszRef = new TCHAR[len+1];
	_tcscpy(m_pszRef,ref);
}

//サイズの設定 パターンを有する場合はパターンのサイズも変更
void SCompInfo::setSize(const SSize& size,bool createPtn,bool nobit)
{
	m_size=size;
	if(m_pPtn!=NULL || createPtn){
		if(m_pPtn==NULL) m_pPtn=new SPtn;
		m_pPtn->setSize(SSize(size.w()*10+1,size.h()*10+1),nobit);
	}
}

void SCompInfo::setUsePattern(bool usepattern,bool nobit)
{
	if(!usepattern){
		delete m_pPtn;
		m_pPtn = NULL;
	}else if(m_pPtn == NULL){
		m_pPtn=new SPtn;
		m_pPtn->setSize(SSize(m_size.w()*10+1,m_size.h()*10+1),nobit);
	}
}

void SCompInfo::setCiType(CITYPE citype)
{
	m_ciType = citype;
	if(m_ciType == CITYPE::FIXED_BLOCK){
		setBlock(1);
	}else if(m_ciType == CITYPE::SHARED_BLOCK){
		setBlock(1);
		m_nFixedBlock = 0;
	}
}

void SCompInfo::setBlock(int block)
{
	if(block<1)block = 1;
	else if(block>255)block = 255;
	int n;
	for(n=0;n<m_nPinCount;n++)	m_ppPin[n]->setBlock(block);
	m_nBlock = block;
}

void SCompInfo::setFixedBlock(int fb)
{
	if(fb<0)fb = 0;
	else if(fb>255)fb = 255;
	m_nFixedBlock = fb;
}


void SCompInfo::addPin(SPin* ppin)
{
	if(ppin == NULL) return;
	PSPin*	ppPin = new PSPin[m_nPinCount+1];
	int n;
	for(n=0;n<m_nPinCount;n++){
		ppPin[n] = m_ppPin[n];
	}
	ppPin[n]=ppin;
	delete[]m_ppPin;
	m_ppPin = ppPin;
	m_nPinCount++;
}

//指定した位置のピン情報を得る
SPin* SCompInfo::pinInfo(int ltrb,int offset)
{
	int n;
	for(n=0;n<m_nPinCount;n++){
		if(m_ppPin[n]->ltrb() == ltrb && m_ppPin[n]->offset() == offset) return m_ppPin[n];
	}
	return NULL;
}

void SCompInfo::removePin(int ltrb,int offset,bool move)
{
	int n,m;
	SPin* ppin=pinInfo(ltrb,offset);
	if(ppin!=NULL){
		PSPin*	ppPin = new PSPin[m_nPinCount-1];
		for(n=0,m=0;n<m_nPinCount;n++){
			if(m_ppPin[n]!=ppin){
				ppPin[m++]=m_ppPin[n];
			}
		}
		while(m<(m_nPinCount-1)) ppPin[m++]=NULL;	//ピン配列に同じピン情報が重複していたときだけ実行される
		delete ppin;
		delete[]m_ppPin;
		m_ppPin = ppPin;
		m_nPinCount--;
	}
	if(move){
		for(n=0;n<m_nPinCount;n++){
			if(m_ppPin[n]->ltrb() == ltrb && m_ppPin[n]->offset() > offset){
				m_ppPin[n]->setOffset(m_ppPin[n]->offset()-1);
			}
		}
	}
}


void SCompInfo::insertNullPin(int ltrb,int offset)
{
	int n;
	for(n=0;n<m_nPinCount;n++){
		if(m_ppPin[n]->ltrb() == ltrb && m_ppPin[n]->offset() >= offset){
			m_ppPin[n]->setOffset(m_ppPin[n]->offset()+1);
		}
	}
}

void SCompIndex::setName(const TCHAR* name)
{
	delete[]m_lpszName;
	m_lpszName=NULL;
	if(name== NULL)	return;
	int len = _tcslen(name);
	m_lpszName = new TCHAR[len+1];
	_tcscpy(m_lpszName,name);
}



//部品番号を得る
const TCHAR* SCompIndex::number() const
{
	if(m_pCompInfo==NULL) return NULL;
	return m_pCompInfo->m_pszRef;
}

//サイズを得る
SSize SCompIndex::size() const
{
	if(m_pCompInfo==NULL) return SSize(0,0);
	return m_pCompInfo->m_size;
}

//配置時の図面上のサイズを得る
SSize SCompIndex::sizePlace(int dir) const
{
	SSize wh = size();
	int w = wh.w()*10;
	int h = wh.h()*10;
	if(dir & 1){	//横サイズと縦サイズが入れ替わる
		int n=w; w=h; h=n;
	}
	return SSize(w,h);
}

//パッケージあたりのブロック数を得る
int	  SCompIndex::block() const
{
	if(m_pCompInfo==NULL) return 0;
	return m_pCompInfo->m_nBlock;
}

//パターンへのポインタを得る
SPtn*	SCompIndex::pattern() const
{
	if(m_pCompInfo==NULL) return NULL;
	return m_pCompInfo->m_pPtn;
}

//ピンの数を得る
int	SCompIndex::pinCount() const
{
	if(m_pCompInfo==NULL) return 0;
	return m_pCompInfo->m_nPinCount;
}


//ピンの情報を得る
SPin* SCompIndex::pin(int index) const
{
	if(m_pCompInfo==NULL) return NULL;
	if(index<0 || index>= m_pCompInfo->m_nPinCount) return NULL;
	return m_pCompInfo->m_ppPin[index];
}

//ピンの情報を得る
//向きの情報から、ピンの位置情報(LTRBと、原点からのオフセット)を返す
SPin* SCompIndex::pin(int index,int dir,int& nLtrb,SPoint& ptEnd) const
{
	if(m_pCompInfo==NULL) return NULL;
	if(index<0 || index>= m_pCompInfo->m_nPinCount) return NULL;
	SPin* pininfo = m_pCompInfo->m_ppPin[index];

	int ltrb = pininfo->ltrb(); // L:0 T:1 R:2 B:3
	int offset = pininfo->offset()*10;

	nLtrb = (ltrb + (dir & 3)) &3;
	if(dir & 4){
		if(nLtrb==0){
			nLtrb = 2;
		}else if(nLtrb==2){
			nLtrb = 0;
		}
	}
	
	SSize wh = sizePlace(dir);

	int w = wh.w();
	int h = wh.h();
//	int x = m_p1.x();
//	int y = m_p1.y();
	switch(nLtrb){
	case 0:		// L
		ptEnd.setX(-w-PIN_LENGTH);
		if(dir == 2 || dir == 3 || dir == 6 || dir == 7){
			ptEnd.setY(-offset);
		}else{
			ptEnd.setY(-h+offset);
		}
		break;
	case 1:		// T
		ptEnd.setY(-h-PIN_LENGTH);
		if(dir == 1 || dir == 2 || dir == 4 || dir == 7){
			ptEnd.setX(-offset);
		}else{
			ptEnd.setX(-w+offset);
		}
		break;
	case 2:		// R
		ptEnd.setX(+PIN_LENGTH);
		if(dir == 2 || dir == 3 || dir == 6 || dir == 7){
			ptEnd.setY(-offset);
		}else{
			ptEnd.setY(-h+offset);
		}
		break;
	case 3:		// B
		ptEnd.setY(+PIN_LENGTH);
		if(dir == 1 || dir == 2 || dir == 4 || dir == 7){
			ptEnd.setX(-offset);
		}else{
			ptEnd.setX(-w+offset);
		}
		break;
	}
	return pininfo;	

}



//デストラクタ
SCompLib::~SCompLib()
{
//	SPtnListIterator itePtn = m_listPtn.begin();
//	while(itePtn != m_listPtn.end()){
//		delete (*itePtn);
//		itePtn++;
//	}
	deleteContent();//内容の破棄
}

//ライブラリ名の設定
void SCompLib::setLibraryName(const TCHAR* name)
{
	delete[]m_lpszLibraryName;
	int length = _tcslen(name);
	m_lpszLibraryName = new TCHAR[length+1];
	_tcscpy(m_lpszLibraryName,name);
}


//ライブラリ名を得る
const TCHAR* SCompLib::libraryName() const
{
	return m_lpszLibraryName;
}

//n番目の部品のインデックスを得る
SCompIndex* SCompLib::compIndex(int nIndexNum) const
{
	//引数のインデックス順が部品インデックス数より多い場合は何もしない
	if(nIndexNum<0 || nIndexNum >= count()){	
		return NULL;
	}
	return m_ppCompIndex[nIndexNum];
}


//パターンのビットマップの読み込み
//ビットマップデータを読み込んだバッファを返す
//ワードアラインメント
//水平方向はバイトアライン
unsigned char* SPtn::readBmp(SReadCE3& rce3)
{
	int x=m_size.w();
	int y=m_size.h();
	
	assert(x>0 && y>0);
	if(x>501) x=501;				//xもｙも50グリッドぶんを上限とする。
	if(y>501) y=501;				//
	wstring str(_T(""));
	int buffXByte = ((x+15)/16)*2;	//バッファの水平方向バイト数
	int xByte = (x+7)/8;			//読み込みの水平方向バイト数。2文字で1バイトなので読み込み文字はこれの倍
	//int nBuffSize=buffXByte * y;


	unsigned char* buff= (unsigned char*)new char[m_nBuffsize];
	
	for(int n=0;n<m_nBuffsize; n++){
		buff[n]=0;
	}

	int yCount=0;
	while(rce3.ReadRecord(str)!=WEOF){
		if(str[0] == '-'){
			if(str==_T("-BMP")) return buff; 
			else goto RdErr;
		}else if(str[0] == '+'){
			str[0]='-';
			if(rce3.SkipTo(str) == WEOF) goto RdErr;
		}else if(yCount<y){
			unsigned char* pbuff=buff+ yCount * buffXByte;
			int nMax=str.length()/2;
			if(nMax > xByte) nMax = xByte;
			int m=0;
			int n;
			for(n=0;n<nMax;n++){
				int uppr = str[m++] - '@';
				int lowr = str[m++] - '@';
				if(uppr>=0 && uppr<=15 && lowr>=0 && lowr<=15){
					pbuff[n]=(uppr<<4)+ lowr;
				}else{
					pbuff[n]=0;
				}
			}
			yCount++;
		}
	}
RdErr:
	delete[]buff;
	return NULL;
}


//パターンの読み込み
bool SCompLib::ReadLb3Ptn(SReadCE3& rce3)
{
	SPtn* pPtn=new SPtn;
	if(pPtn->readCe3(rce3)){
//		m_listPtn.push_back(pPtn);
		m_listPtn.push_front(pPtn);
		return true;
	}else{
		delete pPtn;
		return false;
	}
}

//既存の部品名と重複していないかどうか
bool SCompLib::isUniqueName(const wstring& name)
{
	SCompIndexListIterator iteIndex = m_listCompIndex.begin();
	while(iteIndex != m_listCompIndex.end()){
		if(name==(*iteIndex)->name()) return false;
		iteIndex++;
	}
	return true;
}


//既存の部品名と重ならない名前を作る
wstring SCompLib::uniqueName(const wstring& nameOld)
{
	wstring name = nameOld;
	wstring uniqueName(_T(""));
	if(isUniqueName(name)){
		return name;
	}else{
		int length=name.length();
		int pos=length-1;
		int nval=0;
		if(name[pos]==')'){			//末尾が「)」だったとき
			while(pos>0){
				pos--;
				if(name[pos]=='(' || !isdigit(name[pos]) ) break;
			}
			if(name[pos]=='('){		//末尾が「(数字)」だったとき
				nval=_tstoi(name.substr(pos+1,length-pos-2).c_str());
				name = name.substr(0,pos);
			}
		}
		TCHAR nstr[32];
		int i=4096;
		while(i--){
			nval++;
			_stprintf(nstr,_T("(%d)"),nval);
			uniqueName = name+nstr;
			if(isUniqueName(uniqueName)) break;
		}
		return uniqueName;
	}
}


bool SCompLib::ReadLb3Comp(SReadCE3& rce3)
{
	SCompIndex* pIndex;
	SPtn* pPtn;
	//SCompIndex* pIndex;

	wstring name(_T(""));
	wstring ptnname(_T(""));

	SCompInfo* pComp = new SCompInfo;
	if(pComp->readCe3(rce3,name,ptnname)){
		if(ptnname!=_T("")){	//パターンが設定ずみならパターンへのポインタを検索する。
			SPtnListIterator ite = m_listPtn.begin();
			while(ite != m_listPtn.end()){
				pPtn = *ite;
				if(pPtn->m_pszName == ptnname){
					//pComp->m_pPtn = pPtn;
					pComp->m_pPtn = new SPtn(*pPtn);	//複数の部品定義でパターンを共有しない
					break;
				}
				ite++;
			}
		}
		name = uniqueName(name);
	
		pIndex = new SCompIndex;
		pIndex->setName(name.c_str());
		pIndex->setCompInfo(pComp);

		m_listCompIndex.push_back(pIndex);
		m_listCompInfo.push_back(pComp);

		return true;
	}else{
		delete pComp;
		return false;
	}
}
		


bool SCompLib::ReadLb3Alias(SReadCE3& rce3)
{
	wstring name(_T(""));
	wstring alias(_T(""));
	SCompIndex* pIndexNew;
	SCompIndex* pIndex;
	SCompIndexListIterator ite;
	wstring uniqName(_T(""));

	wstring str(_T(""));
	while(rce3.ReadRecord(str)!=WEOF){
		if(str[0] == '-'){
			if(str != _T("-ALIAS") || name==_T("") || alias==_T("")){
				goto RdErr;
			}else{
				break;
			}
		}else if(str[0] == '+'){
			str[0]='-';
			if(rce3.SkipTo(str) == WEOF) goto RdErr;
		}else{
			int n;
			int l=str.length();
			for(n=0;n<l;n++){
				if(str[n]==':') break;
			}
			if(0<n && n<(l-1)){	// : で分けられたレコードである
				wstring var = str.substr(0,n);				//先頭から:の手前まで
				if(var==_T("N")){
					name=str.substr(n+1);
				}else if(var==_T("A")){
					alias=str.substr(n+1);
				}
			}
		} 
	}
	uniqName = uniqueName(alias);

	ite = m_listCompIndex.begin();
	while(ite != m_listCompIndex.end()){
		pIndex = *ite;
		if(pIndex->name() == name){
			pIndexNew = new SCompIndex;
			pIndexNew->setName(uniqName.c_str());
			pIndexNew->setCompInfo(pIndex->compInfo());
			m_listCompIndex.push_back(pIndexNew);
			break;
		}
		ite++;
	}
	return true;
RdErr:
	return false;
}

//LB3ライブラリの読み込み
bool SCompLib::ReadLb3(SReadCE3& rce3,bool singleComponent /* = false */)//LB3ライブラリの読み込み
{
	wstring str(_T(""));
	bool retv = true;

	while(rce3.ReadRecord(str)!=WEOF){
		if(str[0] == '-'){
			if(str != _T("-BSCH3_LIB_V.1.0")){
				retv = false;
				break;
			}else{
				clearPtnList(); //パターンリスト(m_listPtn)のクリア
				if(rce3.SkipTo(_T("+BSCH3_LIB_V.1.0"))==WEOF) break;
			}
		}else if(str[0] == '+'){
			if(str == _T("+PTN")){
				if(!ReadLb3Ptn(rce3)){
					retv = false;
					break;
				}
			}else
			if(str == _T("+COMP")){
				if(!ReadLb3Comp(rce3)){
					retv = false;
					break;
				}
				if(singleComponent)break;
			}else
			if(str == _T("+ALIAS")){
				if(!ReadLb3Alias(rce3)){
					retv = false;
					break;
				}
			}else{
				str[0]='-';
				if(rce3.SkipTo(str)==WEOF){
					retv = false;
					break;
				}
			}
		}else{
			int n;
			int l=str.length();
			for(n=0;n<l;n++){
				if(str[n]==':') break;
			}
			if(0<n && n<l){
				wstring var = str.substr(0,n);				//先頭から:の手前まで
				n++;
				if(var == _T("PROP")){
					m_prop_string= str.substr(n);
				}
			}
		}
	}
	clearPtnList(); //パターンリスト(m_listPtn)のクリア
	return retv;
}

//メンバー変数パターンリストのクリア
void SCompLib::clearPtnList()
{
	SPtnListIterator itePtn = m_listPtn.begin();
	while(itePtn != m_listPtn.end()){
		delete (*itePtn++);
	}
	m_listPtn.clear();
}

bool SCompLib::WriteAlias(SWriteCE3& wce3,const TCHAR*  alias,const TCHAR*  name)
{
	if(alias == NULL || *alias == '\0') return false;
	if(name  == NULL || *name  == '\0') return false;
	wce3.WriteRecord(_T("+ALIAS"));
	wce3.WriteRecordString(_T("A"),alias);
	wce3.WriteRecordString(_T("N"),name);
	wce3.WriteRecord(_T("-ALIAS"));
	wce3.WriteEOL();
	return true;
}

//部品の書き込み
//bool SCompLib::WriteLb3Comp(SWriteCE3& wce3,const SCompInfo& compinfo,const char*  name)
//{
//}


//内容の破棄
void SCompLib::deleteContent()
{
	SCompInfoListIterator iteInfo = m_listCompInfo.begin();
	while(iteInfo != m_listCompInfo.end()){
		delete (*iteInfo);
		iteInfo++;
	}
	m_listCompInfo.clear();

	SCompIndexListIterator iteIndex = m_listCompIndex.begin();
	while(iteIndex != m_listCompIndex.end()){
		delete (*iteIndex);
		iteIndex++;
	}
	m_listCompIndex.clear();

	delete[]m_lpszLibraryName;					//ライブラリ名バッファの解放
	m_lpszLibraryName = NULL;

	delete[]m_ppCompIndex;
	m_ppCompIndex = NULL;

}



//ファイル書き込み
//ファイルの保存には、CMainFrame::DoSave(LPCTSTR lpszFileName) を使っている。
bool SCompLib::writeLibraryFile(const TCHAR* lpszFileName)
{
	FILE* fp;
	if((fp = _tfopen(lpszFileName,_T("wt, ccs=UTF-8")))==NULL) return false;
	fseek( fp, 0, SEEK_SET ) ; //BAD KNOWHOW for removing BOM CODE.
	SWriteCE3 wce3;
	wce3.Attach(fp);
	wce3.WriteRecord(_T("+BSCH3_LIB_V.1.0"));
	wce3.WriteEOL();

	wce3.WriteRecordInt(_T("VER"),SCompLib::m_nConstDataVersion);
	wce3.WriteEOL();
	
	wce3.WriteRecordString(_T("PROP"),m_prop_string);
	wce3.WriteEOL();
	SCompIndexListIterator iteIndex = m_listCompIndex.begin();
	while(iteIndex != m_listCompIndex.end()){
		wstring name = (*iteIndex)->name();
		SCompInfo* pInfo = (*iteIndex)->compInfo();
		bool bAlias = false;
		wstring aliasFor(_T(""));
		SCompIndexListIterator iteIndex2 = m_listCompIndex.begin();
		while(iteIndex2 != iteIndex && iteIndex2 != m_listCompIndex.end()){
			if((*iteIndex2)->compInfo()==pInfo){
				bAlias = true;
				aliasFor = (*iteIndex2)->name();	//書き込み済みの名前
				break;
			}
			iteIndex2++;
		}
		if(bAlias){
			WriteAlias(wce3,name.c_str(),aliasFor.c_str());
		}else{
			pInfo->writeCe3(wce3,name.c_str());
		}
		iteIndex++;
	}
	wce3.WriteRecord(_T("-BSCH3_LIB_V.1.0"));
	wce3.WriteEOL();

	fclose(fp);
	return true;
}

//ファイル読み込み
bool SCompLib::readLibraryFile(LPCTSTR lpszFileName)
{

	int nCount;
	if(m_lpszLibraryName!=NULL){	//既に設定済みなら何もしない
		return false;
	}

	//qDebug("***** %s *****",lpszFileName);

	SReadCE3 rce3;
	wstring str(_T(""));
	SCompIndexListIterator iteIndex;

	WCHAR* wcBuff = NULL;
	char* buff = NULL;

	FILE* fp=_tfopen(lpszFileName,_T("rb"));
	//seek and get file size;
	fseek(fp,0,SEEK_END);
	fpos_t filelength;
	fgetpos(fp,&filelength); 
	fseek(fp,0,SEEK_SET);
	buff = new char[filelength+1];
	size_t readSize = fread(buff,1,filelength,fp);
	fclose(fp);

	if(readSize<filelength) goto RdErr;
	buff[filelength]=0;


	//ここからバージョン判定
	wcBuff = new wchar_t[filelength+1];
	for(int i=0;i<(filelength+1);i++){
		wcBuff[i]=buff[i];
	}

	rce3.Attach(wcBuff);

	if(rce3.ReadRecord(str)==WEOF) goto RdErr;
	if(str == _T("+BSCH3_DATA_V.1.0")){
		rce3.SkipTo(_T("+BSCH3_LIB_V.1.0"));
	}else{
		if(str != _T("+BSCH3_LIB_V.1.0")) goto RdErr;
	}

	//Version 0.80 以降は、+BSCH3_LIB_V.1.0の次には VER:xxが入る。
	if(rce3.ReadRecord(str)==WEOF) goto RdErr;
	{
		int n;
		int l=str.length();
		for(n=0;n<l;n++){
			if(str[n]==':') break;
		}
		wstring var = str.substr(0,n);
		if(var == _T("VER")){
			int val = _tstoi(str.substr(n+1).c_str());
			m_dataVersion = val;
		}else{
			m_dataVersion = 70;
		}
	}
	delete[]wcBuff;
	wcBuff = NULL;
	//ここまでバージョン判定



	//ライブラリの Data Version 0.70以下はローカルエンコードと見做す
	if(m_dataVersion <= 70){
		int wcBuffSize = ::MultiByteToWideChar(CP_THREAD_ACP, 0, buff, -1, NULL,0);
		wcBuff = new WCHAR[wcBuffSize];
		::MultiByteToWideChar(CP_THREAD_ACP, 0, buff, -1, wcBuff, wcBuffSize);
	}else{
		int wcBuffSize = ::MultiByteToWideChar(CP_UTF8, 0, buff, -1, NULL,0);
		wcBuff = new WCHAR[wcBuffSize];
		::MultiByteToWideChar(CP_UTF8, 0, buff, -1, wcBuff, wcBuffSize);
	}
	
	delete[]buff;
	buff = NULL; 

	rce3.Attach(wcBuff);
	rce3.SkipTo(_T("+BSCH3_LIB_V.1.0"));


	if(!ReadLb3(rce3)) goto RdErr;
	delete[]wcBuff;

	int n;
	int len;

	nCount = m_listCompIndex.size();

	m_ppCompIndex = new PSCompIndex[nCount];
	iteIndex = m_listCompIndex.begin();
	n=0;
	while(iteIndex != m_listCompIndex.end()){
		m_ppCompIndex[n++]=(*iteIndex);
		iteIndex++;
	}
	//ライブラリ名の設定
	setLibraryName(lpszFileName);

//	//タイムスタンプの記録 OS依存
//#ifdef _WINDOWS
//	_wfinddata64i32_t findData;
//	if(_tfindfirst(lpszFileName,&findData)!=-1){
//		m_aux1=findData.time_write;
//	}
//#endif

	return true;

RdErr:
	delete[]buff;
	delete[]wcBuff;
	return false;
}





////ファイル読み込み
//bool SCompLib::readLibraryFile(const TCHAR* lpszFileName)
//{
//
//	int nCount;
//	if(m_lpszLibraryName!=NULL){	//既に設定済みなら何もしない
//		return false;
//	}
//
//	//qDebug("***** %s *****",lpszFileName);
//
//	SReadCE3 rce3;
//	wstring str(_T(""));
//	SCompIndexListIterator iteIndex;
//
//	FILE* fp=_tfopen(lpszFileName,_T("rt, ccs=UTF-8"));
//	if(fp==NULL) return false;
//	rce3.Attach(fp);
//	if(rce3.ReadRecord(str)==WEOF) goto RdErr;
//	if(str == _T("+BSCH3_DATA_V.1.0")){
//		rce3.SkipTo(_T("+BSCH3_LIB_V.1.0"));
//	}else{
//		if(str != _T("+BSCH3_LIB_V.1.0")) goto RdErr;
//	}
//
//
//	//Version 0.80 以降は、+BSCH3_LIB_V.1.0の次には VER:xxが入る。
//	if(rce3.ReadRecord(str)==WEOF) goto RdErr;
//	{
//		int n;
//		int l=str.length();
//		for(n=0;n<l;n++){
//			if(str[n]==':') break;
//		}
//		wstring var = str.substr(0,n);
//		if(var == _T("VER")){
//			int val = _tstoi(str.substr(n+1).c_str());
//			m_dataVersion = val;
//		}else{
//			m_dataVersion = 70;
//		}
//	}
//
//	WCHAR* wcBuff = NULL;
//
//	//Data Version 0.70以下はローカルエンコードと見做す
//	if(m_dataVersion <= 70){
//		fclose(fp);
//		//バイナリモードで開きなおす。
//		FILE* fp=_tfopen(lpszFileName,_T("rb"));
//		//seek and get file size;
//		fseek(fp,0,SEEK_END);
//		fpos_t filelength;
//		fgetpos(fp,&filelength); 
//		fseek(fp,0,SEEK_SET);
//		char* buff = new char[filelength+1];
//		size_t readSize = fread(buff,1,filelength,fp);
//		if(readSize<filelength) goto RdErr;
//		buff[filelength]=0;
//
//		int wcBuffSize = ::MultiByteToWideChar(CP_THREAD_ACP, 0, buff, -1, NULL,0);
//		wcBuff = new WCHAR[wcBuffSize];
//		::MultiByteToWideChar(CP_THREAD_ACP, 0, buff, -1, wcBuff, wcBuffSize);
//		delete[]buff;
//
//		rce3.Attach(wcBuff);
//		rce3.SkipTo(_T("+BSCH3_LIB_V.1.0"));
//	}
//
//
//	if(!ReadLb3(rce3)) goto RdErr;
//	delete[]wcBuff;
//	fclose(fp);
//
//	int n;
//	int len;
//
//	nCount = m_listCompIndex.size();
//
//	m_ppCompIndex = new PSCompIndex[nCount];
//	iteIndex = m_listCompIndex.begin();
//	n=0;
//	while(iteIndex != m_listCompIndex.end()){
//		//const char* pname=(*iteIndex)->name();
//		//qDebug("   %s",pname);
//		m_ppCompIndex[n++]=(*iteIndex);
//		iteIndex++;
//	}
//	//ライブラリ名の設定
//	setLibraryName(lpszFileName);
//	return true;
//
//RdErr:
//	delete[]wcBuff;
//	fclose(fp);
//	return false;
//}
//
//
//
