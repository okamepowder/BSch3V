/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/


#include "StdAfx.h"
#include "CompEditElm.h"

SCompEditElm::SCompEditElm(void)
{
	m_placeOpCount = -1;		//配置された操作カウント
	m_deleteOpCount= -1;		//ここで示された操作カウントにおいて消去されている
	m_pOrigin = NULL;			//一時データにコピーしたときの元のデータ

	m_dataType	= -1;
	m_pData		= NULL;
}

SCompEditElm::~SCompEditElm(void)
{
}


