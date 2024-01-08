/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#ifndef XBSCHDOCSCH_H
#define XBSCHDOCSCH_H

#include "xbschdoc.h"


class SXBSchDocSch : public SXBSchDoc
{

public:
	SXBSchDocSch(){};
	~SXBSchDocSch(){};

	//***** �ҏW *****
	//�R�s�[
	//�R�s�[���s��ꂽ�Ƃ���true��Ԃ�
	bool copy();

	//�\��t��
	//�h�L�������g�ɕύX���������Ƃ�true��Ԃ�
	bool paste(const SPoint& pt);

	//�\��t���\���H
	bool canPaste();

//	bool findStringCompare(const char* szTarget,const char* sz,bool bMatchWhole,bool bCaseSensitive);

};

#endif
