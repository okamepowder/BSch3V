/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

#pragma once

class SCompEditElm
{
public:
	SCompEditElm(void);
	virtual ~SCompEditElm(void);
	
	int m_placeOpCount;			//配置された操作カウント
	int m_deleteOpCount;		//ここで示された操作カウントにおいて消去されている
	SCompEditElm* m_pOrigin;	//一時データにコピーしたときの元のデータ

	int m_dataType;
	void* m_pData;
};
