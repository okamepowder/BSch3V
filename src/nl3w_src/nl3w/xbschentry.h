/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

/****************************************************************************
** XBSch�}�ʗv�f�G���g���[�N���X
****************************************************************************/
#ifndef XBSCHENTRY_H
#define XBSCHENTRY_H

#include "xbsch.h"
#include "xbschobj.h"
#include "xbschline.h"


class SXBSchEntry:public SXBSchLine
{
public:
	enum ENTRY_ASPECT
	{
		RIGHT_DOWN=0,
		LEFT_DOWN
	};
protected:
	//m_p1 �� m_p2�̉��ɂȂ�悤�ɐݒ肷��B
	void normalize();
public:
	//�R���X�g���N�^
	SXBSchEntry();
	//�f�X�g���N�^
	virtual ~SXBSchEntry(){};
/*
	//�X�g���[������̓ǂݍ��� �` -ENTRY
	bool readStream(SCsvIStream* pStrm,const SPoint* pptOrigin);
	//�X�g���[���ւ̏������݁@+ENTRY �` -ENTRY
	bool writeStream(SCsvOStream* pStrm,const SPoint* pptOrigin);
*/
	bool readCe3(SReadCE3&,const SPoint* ,SXBSchDoc*);
	bool writeCe3(SWriteCE3& ,const SPoint* );


	//ID�̎擾
	unsigned id(){return ID_ENTRY;}
	unsigned objectFilterValue(){return OBJMASK_WENTRY;} 
public:
	//�������s��
	SXBSchObj* duplicate();
	//���W�̐ݒ�  
	//m_p1�ւ̐ݒ���s����m_p2�ւ̐ݒ����������B
	void setX1(int);
	void setY1(int);
	void setP1(const SPoint&);
	//m_p2�ւ̐ݒ�𖳌��ɂ��Ă���
	void setX2(int){}
	void setY2(int){}
	void setP2(const SPoint&){}
	//�͈͂�I�����ăh���b�O�I�����s��
	unsigned testSelectionForDrag(const SRect& rc){return testSelection(rc);} 
	//�G���g���[�̉E������A����������擾����
	unsigned aspect();
	//�G���g���[�̉E������A���������ݒ肷��
	void setAspect(int aspect);
};


class SXBSchBusEntry:public SXBSchEntry
{
public:
	//�R���X�g���N�^
	SXBSchBusEntry(){}
	//�f�X�g���N�^
	virtual ~SXBSchBusEntry(){}
/*
	//�X�g���[������̓ǂݍ��� �` -BENTRY
	bool readStream(SCsvIStream* pStrm,const SPoint* pptOrigin);
	//�X�g���[���ւ̏������݁@+BENTRY �` -BENTRY
	bool writeStream(SCsvOStream* pStrm,const SPoint* pptOrigin);
*/
	bool readCe3(SReadCE3&,const SPoint* ,SXBSchDoc*);
	bool writeCe3(SWriteCE3& ,const SPoint* );

	//ID�̎擾
	unsigned id(){return ID_BENTRY;}
	unsigned objectFilterValue(){return OBJMASK_BENTRY;} 
public:
	//�������s��
	SXBSchObj* duplicate();
};

#endif
