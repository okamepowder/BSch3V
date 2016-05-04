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
	const char*	m_buff;
	const char*	m_pread;
	//CFile*	m_pCfile;
	FILE*	m_pfile;
	int		m_lineCount;	 
	int  getC();
	bool isSpace(char cc);
	bool isDelim(char cc);
public:
	SReadCE3();		//コンストラクタ
	~SReadCE3();	//デストラクタ
	//bool Attach(CFile* pCfile);
	bool Attach(FILE* pFile);
	bool Attach(const char* buff);
	int ReadRecord(string& s);
	//int ReadRecord(char* pBuff,int nBuffLen);
	int Line(){return m_lineCount;}
	int SkipTo(char* pszSkipTo);
	int SkipTo(const string& strSkipTo);
};

class SWriteCE3{
protected:
	//CFile*	m_pCfile;
	FILE*	m_pfile;
	string*	m_pstr;
	bool	m_bNewLine; 
	int  putC(int c);

public:
	SWriteCE3();	//コンストラクタ
	~SWriteCE3();	//デストラクタ
	//bool Attach(CFile* pCfile);
	bool Attach(FILE* pFile);
	bool Attach(string* pstr);
	int WriteRecord(const char* pBuff);
	int WriteRecord(const string& s);
	int WriteRecordInt(const string& var,int val);
	int WriteRecordString(const string& var,const string& val);
	int WriteEOL();
};

#endif