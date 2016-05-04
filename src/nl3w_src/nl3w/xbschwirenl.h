/****************************************************************************
    NL3W netlist utility for BSch3V
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#ifndef XBSCHWIRENL_H
#define XBSCHWIRENL_H

#include "xbschline.h"
//���C���[�N���X
class SXBSchWireNl:public SXBSchWire
{
public:
	//�R���X�g���N�^
	SXBSchWireNl(){}
	SXBSchWireNl(SXBSchWire& w);
	//�f�X�g���N�^
	~SXBSchWireNl(){}
	//�w�肵���|�C���g�ŕ�������B
	SXBSchWireNl* divide(const SPoint& pt);
	stringList m_listSignalName;

};

#endif
