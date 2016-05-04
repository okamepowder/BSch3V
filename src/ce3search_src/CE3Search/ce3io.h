/****************************************************************************
    BSch3V schematic capture
	Copyright (C) 1997-2014 H.Okada
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
	LPCTSTR	m_buff;
	LPCTSTR	m_pread;
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
	bool Attach(LPCTSTR buff);
	int ReadRecord(wstring& s);
//	int ReadRecord(LPTSTR pBuff,int nBuffLen);
	int Line(){return m_lineCount;}
	int SkipTo(LPTSTR pszSkipTo);
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
	int WriteRecord(LPCTSTR pBuff);
	int WriteRecord(const wstring& s);
	int WriteRecordInt(const wstring& var,int val);
	int WriteRecordString(const wstring& var,const wstring& val);
	int WriteEOL();
};

#endif