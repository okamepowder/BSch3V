/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <string>
using namespace std;

#include "ce3io.h"

//コンストラクタ
SReadCE3::SReadCE3()
{
//	m_pCfile = NULL;
	m_buff = NULL;
	m_pread = NULL;
	m_pfile = NULL;
	m_lineCount = 0;
}

//デストラクタ
SReadCE3::~SReadCE3()
{
}

//文字の読み込み
int SReadCE3::getC()
{
	wint_t cc = 0;
//	if(m_pCfile){
//		char c;
//		if(m_pCfile->Read(&c,1) == 0) cc=WEOF;
//	}else 
	if(m_pfile){
		cc=fgetwc(m_pfile);
	}else if(m_buff){
		cc=*m_pread;
		if(cc=='\0') cc=WEOF;
		else m_pread++;
	}
	return cc;
}

bool SReadCE3::isSpace(TCHAR cc)
{
	return (cc==' '||cc=='\t'||cc=='\n'||cc=='\r');
}

bool SReadCE3::isDelim(TCHAR cc)
{
	return (cc==','||cc=='\n'||cc=='\r');
}

//bool SReadCE3::Attach(CFile* pCfile)
//{
//	m_buff = NULL;
//	m_pread = NULL;
//	m_pCfile = pCfile;
//	m_pfile = NULL;
//	m_lineCount = 1;
//	return true;
//}

bool SReadCE3::Attach(FILE* pFile)
{
	m_buff = NULL;
	m_pread = NULL;
//	m_pCfile = NULL;
	m_pfile = pFile;
	m_lineCount = 1;
	return true;
}

bool SReadCE3::Attach(const TCHAR* buff)
{
	m_buff = buff;
	m_pread = buff;
//	m_pCfile = NULL;
	m_pfile = NULL;
	m_lineCount = 1;
	return true;
}

int SReadCE3::ReadRecord(wstring& s)
{
	int cc;
	s=_T("");
	TCHAR sz[16];
	int nLine;
	int count=0;

	cc=getC();
	//レコードが始まるまでの空白の削除
	while(1){
		if(cc==WEOF) return cc;
		if(!isSpace(cc)) break;
		if(cc=='\n') m_lineCount++;
		cc=getC();
	}
	nLine = m_lineCount;
	while(!isDelim(cc) && count<0x8000){
		if(cc=='%'){
			cc=getC();
			if(isDelim(cc))return WEOF;
			sz[0]=cc;
			cc=getC(); 
			if(isDelim(cc))return WEOF;
			sz[1]=cc;
			sz[2]='\0';
			cc=_tcstoul(sz,NULL,16);
			s+=cc;
		}else{
			s+=cc;
		}
		cc=getC();
		count++;
	}
	if(cc=='\n')  m_lineCount++;

	//return nLine;
	return 0;
}

//int SReadCE3::ReadRecord(TCHAR* pBuff,int nBuffLen)
//{
//	wstring s;
//	int n =	ReadRecord(s);
//	_tcsncpy(pBuff,s.c_str(),nBuffLen);
//	pBuff[nBuffLen-1]='\0';
//	return n;
//}


int SReadCE3::SkipTo(TCHAR* pszSkipTo)
{
	assert(pszSkipTo != NULL);
	wstring s;
	while(1){
		if(ReadRecord(s)==WEOF) return WEOF;
		if(s == pszSkipTo) return 0;
	}
}

int SReadCE3::SkipTo(const wstring& strSkipTo)
{
	wstring s;
	while(1){
		if(ReadRecord(s)==WEOF) return WEOF;
		if(s == strSkipTo) return 0;
	}
}


//コンストラクタ
SWriteCE3::SWriteCE3()
{	
//	m_pCfile = NULL;
	m_pfile	= NULL;
	m_pstr  = NULL;
	m_bNewLine = true;
}
//デストラクタ
SWriteCE3::~SWriteCE3()
{
}

int SWriteCE3::putC(wint_t c)
{
//	if(m_pCfile){
//		char ch=c&0xFF;
//		m_pCfile->Write(&c,1);
//	}else 
	if(m_pfile){
		return fputwc(c,m_pfile);
	}else if(m_pstr){
		TCHAR sz[2];
		sz[0]=c;
		sz[1]=0;
		m_pstr->append(sz);
		return c;
	}
	return WEOF;
}

//bool SWriteCE3::Attach(CFile* pCfile)
//{
//	m_pCfile = pCfile;
//	m_pfile	 = NULL;
//	m_pstr   = NULL;
//	m_bNewLine = true;
//	return true;
//}


bool SWriteCE3::Attach(FILE* pFile)
{
//	m_pCfile = NULL;
	m_pfile	= pFile;
	m_pstr  = NULL;
	m_bNewLine = true;
	return true;
}

bool SWriteCE3::Attach(wstring* pstr)
{
	assert(m_pfile == NULL);
//	m_pCfile = NULL;
	m_pfile	= NULL;
	m_pstr  = pstr;
	m_bNewLine = true;
	return true;
}



int SWriteCE3::WriteRecord(LPCTSTR pBuff)
{
	if(!m_bNewLine){
		if(putC(',')==WEOF) return WEOF;
	}else{
		m_bNewLine=false;
	}
	while(*pBuff){
		if((*pBuff >= '\1')&&( *pBuff <= ' ') || ( *pBuff == '%') || ( *pBuff == ',')){
			TCHAR sz[16];
			_stprintf(sz,_T("%02X"),*pBuff);
			if(putC('%')==WEOF || putC(sz[0])==WEOF || putC(sz[1])==WEOF) return WEOF; 
		}else{
			if(putC(*pBuff)==WEOF) return WEOF;
		}
		pBuff++;
	}
	return 0;
}

int SWriteCE3::WriteRecord(const wstring& s)
{
	return WriteRecord(s.c_str());
}


int SWriteCE3::WriteEOL()
{
	m_bNewLine = true;
	if(putC('\n')==WEOF) return WEOF;
	else return 0;
}

int SWriteCE3::WriteRecordLongFlag(const wstring& var,unsigned long val)
{
	TCHAR sz[16];
	_stprintf(sz,_T(":%8X"),val);
	wstring str = var;
	str = str+sz;
	return WriteRecord(str.c_str());
}

int SWriteCE3::WriteRecordInt(const wstring& var,int val)
{
	TCHAR sz[16];
	_stprintf(sz,_T(":%d"),val);
	wstring str = var;
	str = str+sz;
	return WriteRecord(str.c_str());
}


int SWriteCE3::WriteRecordString(const wstring& var,const wstring& val)
{
	wstring str = var + _T(":");
	str = str + val;
	return WriteRecord(str.c_str());
}
