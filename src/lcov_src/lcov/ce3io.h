/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

//CE3ファイルの処理

//カンマまたは改行区切り
//コントロール文字とスペース、カンマ、% は %xx(xxはアスキーコード)に置き換える。


#ifndef CE3IO_H
#define CE3IO_H

#include <stdio.h>
#include <string>
using namespace std;

class SReadCE3{
protected:
	const TCHAR*	m_buff;
	const TCHAR*	m_pread;
	//CFile*	m_pCfile;
	FILE*	m_pfile;
	int		m_lineCount;	 
	int  getC();
	bool isSpace(TCHAR cc);
	bool isDelim(TCHAR cc);
public:
	SReadCE3();		//コンストラクタ
	~SReadCE3();	//デストラクタ
	//bool Attach(CFile* pCfile);
	bool Attach(FILE* pFile);
	bool Attach(const TCHAR* buff);
	int ReadRecord(wstring& s);
	//int ReadRecord(TCHAR* pBuff,int nBuffLen);
	int Line(){return m_lineCount;}
	int SkipTo(TCHAR* pszSkipTo);
	int SkipTo(const wstring& strSkipTo);
};

class SWriteCE3{
protected:
	//CFile*	m_pCfile;
	FILE*	m_pfile;
	wstring*	m_pstr;
	bool	m_bNewLine; 
	int  putC(wint_t c);

public:
	SWriteCE3();	//コンストラクタ
	~SWriteCE3();	//デストラクタ
	//bool Attach(CFile* pCfile);
	bool Attach(FILE* pFile);
	bool Attach(wstring* pstr);
	int WriteRecord(const TCHAR* pBuff);
	int WriteRecord(const wstring& s);
	int WriteRecordInt(const wstring& var,int val);
	int WriteRecordLongFlag(const wstring& var,unsigned long val); //Version 0.81.00 2013/07/27
	int WriteRecordString(const wstring& var,const wstring& val);
	int WriteEOL();
};

#endif