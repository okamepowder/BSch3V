/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

/****************************************************************************
** XBSch�}�ʗv�f�G���g���[�N���X�̎���
****************************************************************************/
#include "stdafx.h"
#include <stdio.h>
#include <assert.h>
#include <string>
using namespace std;
#include "ce3io.h"
#include "xbschentry.h"
#include "xbschline.h"
#include "xbschentry.h"

//RIGHT_DOWN
//m_p2
//����������������������
//����������������������
//����������������������
//����������������������
//����������������������
//����������������������
//����������������������
//����������������������
//����������������������
//����������������������
//���������������������� m_p1

//LEFT_DOWN
//���������������������� m_p2
//����������������������
//����������������������
//����������������������
//����������������������
//����������������������
//����������������������
//����������������������
//����������������������
//����������������������
//����������������������
//m_p1


//�R���X�g���N�^
SXBSchEntry::SXBSchEntry()
{
	//�f�t�H���g�ō�������ɂȂ�
	m_p1=SPoint(0,10);
	m_p2=SPoint(10,0);
}

//m_p1 �� m_p2�̉��ɂȂ�悤�ɐݒ肷��B
void SXBSchEntry::normalize()
{
	if(m_p1.y() < m_p2.y()){
		SPoint point = m_p1;
		m_p1 = m_p2;
		m_p2 = point;
	}
}


//�G���g���[�̉E������A����������擾����
unsigned SXBSchEntry::aspect()
{
	normalize();
	if(m_p1.x() < m_p2.x()) return LEFT_DOWN;
	else	return RIGHT_DOWN;
}


//�G���g���[�̉E������A���������ݒ肷��
void SXBSchEntry::setAspect(int aspect)
{
	m_p2.setY( m_p1.y() - 10);
	if(aspect == RIGHT_DOWN){
		m_p2.setX( m_p1.x() - 10);
	}else{
		m_p2.setX( m_p1.x() + 10);
	}
}
		
/*
//�X�g���[������̓ǂݍ��� �`-ENTRY
bool SXBSchEntry::readStream(SCsvIStream* pStrm,const SPoint* pptOrigin)
{
	if(!readStreamEx(pStrm,pptOrigin,"ENTRY")) return false;
	//Entry��m_p2���Đݒ肷��B
	setAspect(aspect());
	return true;
}
*/


bool SXBSchEntry::readCe3(SReadCE3& rce3,const SPoint* pptOrigin,SXBSchDoc*)
{
	if(!readCe3Ex(rce3,pptOrigin,_T("ENTRY"))) return false;
	setAspect(aspect());
	return true;
}

bool SXBSchEntry::writeCe3(SWriteCE3& wce3,const SPoint* pptOrigin)
{
	normalize();
	return writeCe3Ex(wce3,pptOrigin,_T("ENTRY"));
}


/*
//�X�g���[���ւ̏������݁@+ENTRY �` -ENTRY
bool SXBSchEntry::writeStream(SCsvOStream* pStrm,const SPoint* pptOrigin)
{
	normalize();
	return writeStreamEx(pStrm,pptOrigin,"ENTRY");
}
*/

//�������s��
SXBSchObj* SXBSchEntry::duplicate()
{
	SXBSchObj* newObj =new SXBSchEntry(*this);
	return newObj;
}

//�_���w�肵�đI���d�v�x��Ԃ�
//int SXBSchEntry::qSelectWeight(const SPoint& p)
//{
//	int nResult = PointAndRectMag(p,area());
//	if(nResult) return (nResult | ON_OBJ);
//	else		return 0;
//}

//�͈͂��w�肵�đI�����s��
//unsigned SXBSchEntry::testSelection(const SRect& rc)
//{
//	if( rc.intersects(area()) ){
//		return SELECT_ALL;
//	}else{
//		return 0;
//	}
//}

//���W�̐ݒ�  
//m_p1�ւ̐ݒ���s����m_p2�ւ̐ݒ����������B
void SXBSchEntry::setX1(int x)
{
	if(aspect()==RIGHT_DOWN){
		m_p2.setX(x-10);
	}else{
		m_p2.setX(x+10);
	}
	m_p1.setX(x);
}

void SXBSchEntry::setY1(int y)
{
	m_p1.setY(y);
	m_p2.setY(y-10);
}

void SXBSchEntry::setP1(const SPoint& p1)
{
	setX1(p1.x());
	setY1(p1.y());
}


bool SXBSchBusEntry::readCe3(SReadCE3& rce3,const SPoint* pptOrigin,SXBSchDoc*)
{
	if(!readCe3Ex(rce3,pptOrigin,_T("BENTRY"))) return false;
	setAspect(aspect());
	return true;
}

bool SXBSchBusEntry::writeCe3(SWriteCE3& wce3,const SPoint* pptOrigin)
{
	normalize();
	return writeCe3Ex(wce3,pptOrigin,_T("BENTRY"));
}


//�������s��
SXBSchObj* SXBSchBusEntry::duplicate()
{
	SXBSchObj* newObj =new SXBSchBusEntry(*this);
	return newObj;
}
