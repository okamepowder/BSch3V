/****************************************************************************
    NL3W netlist utility for BSch3V
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#ifndef SNLPIN_H
#define SNLPIN_H

class SNlPin  
{
public:
	SNlPin(){m_checked=false;}
	virtual ~SNlPin(){}
	bool		m_checked;
	string		m_strRefNum;	//部品番号
	string		m_strPinNum;	//ピン名またはピン番号
	SPoint		m_point;		//ピン位置
	stringList  m_listSigName;	//信号名のリスト
};

#endif
