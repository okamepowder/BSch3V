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

	//***** 編集 *****
	//コピー
	//コピーが行われたときにtrueを返す
	bool copy();

	//貼り付け
	//ドキュメントに変更があったときtrueを返す
	bool paste(const SPoint& pt);

	//貼り付け可能か？
	bool canPaste();

//	bool findStringCompare(const char* szTarget,const char* sz,bool bMatchWhole,bool bCaseSensitive);

};

#endif
