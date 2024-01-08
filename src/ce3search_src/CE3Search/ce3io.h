/****************************************************************************
    BSch3V schematic capture
	Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

//CE3�t�@�C���̏���

//�J���}�܂��͉��s��؂�
//�R���g���[�������ƃX�y�[�X�A�J���}�A% �� %xx(xx�̓A�X�L�[�R�[�h)�ɒu��������B


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
	SReadCE3();		//�R���X�g���N�^
	~SReadCE3();	//�f�X�g���N�^
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
	SWriteCE3();	//�R���X�g���N�^
	~SWriteCE3();	//�f�X�g���N�^
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