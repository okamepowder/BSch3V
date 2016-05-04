/****************************************************************************
    NL3W netlist utility for BSch3V
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#ifndef XBSCHWIRENL_H
#define XBSCHWIRENL_H

#include "xbschline.h"
//ワイヤークラス
class SXBSchWireNl:public SXBSchWire
{
public:
	//コンストラクタ
	SXBSchWireNl(){}
	SXBSchWireNl(SXBSchWire& w);
	//デストラクタ
	~SXBSchWireNl(){}
	//指定したポイントで分割する。
	SXBSchWireNl* divide(const SPoint& pt);
	stringList m_listSignalName;

};

#endif
