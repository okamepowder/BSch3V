/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/


#include "StdAfx.h"
#include "CompEditElm.h"

SCompEditElm::SCompEditElm(void)
{
	m_placeOpCount = -1;		//�z�u���ꂽ����J�E���g
	m_deleteOpCount= -1;		//�����Ŏ����ꂽ����J�E���g�ɂ����ď�������Ă���
	m_pOrigin = NULL;			//�ꎞ�f�[�^�ɃR�s�[�����Ƃ��̌��̃f�[�^

	m_dataType	= -1;
	m_pData		= NULL;
}

SCompEditElm::~SCompEditElm(void)
{
}


