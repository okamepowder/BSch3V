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
	
	int m_placeOpCount;			//�z�u���ꂽ����J�E���g
	int m_deleteOpCount;		//�����Ŏ����ꂽ����J�E���g�ɂ����ď�������Ă���
	SCompEditElm* m_pOrigin;	//�ꎞ�f�[�^�ɃR�s�[�����Ƃ��̌��̃f�[�^

	int m_dataType;
	void* m_pData;
};
