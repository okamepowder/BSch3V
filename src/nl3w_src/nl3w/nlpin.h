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
	string		m_strRefNum;	//���i�ԍ�
	string		m_strPinNum;	//�s�����܂��̓s���ԍ�
	SPoint		m_point;		//�s���ʒu
	stringList  m_listSigName;	//�M�����̃��X�g
};

#endif
