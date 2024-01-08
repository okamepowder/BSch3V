/****************************************************************************
    BSch3V schematic capture
     Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#pragma once

#include "ce3io.h"
#include "coord.h"

enum VECTPTN
{
	VECTPTN_L			=1,
	VECTPTN_C			=2,
	VECTPTN_PG			=3,
	VECTPTN_AR			=4,
	VECTPTN_TXT			=5
};

class SPtnObj
{
public:
	int m_style;
	int m_width;
	int m_nFill;
public:
	SPtnObj(void);
	virtual ~SPtnObj(void);
public:
	virtual  int id(){return 0;}
	//����
	virtual SPtnObj* duplicate() = 0;
	//�L���ȃf�[�^���ǂ�����Ԃ��B
	virtual bool qValid(){return true;}
	//�ʒu�𓮂���
	virtual void move(int offsetx,int offsety) = 0;
	//�O���b�v�̐���Ԃ�
	virtual int gripCount() = 0;
	//�O���b�v�̈ʒu��Ԃ�
	virtual SPoint gripPos(int grip) = 0;
	//�O���b�v�̈ʒu�𓮂���
	virtual void setGripPos(int grip,int x,int y) = 0;
	

	//X���W���w�肵�č��E���]���s��
	virtual void mirrorH(int x) = 0;
	//���E���]���s��
	virtual void mirrorH() = 0;
	//XY���S���W���w�肵�āA��]���s��
	virtual void rotate(const SPoint& p) = 0;
	//��]���s��
	virtual void rotate() = 0;
	//�͈͂��w�肵�đI���`�F�b�N���s��
	virtual bool testIntersect(const SRect& ){return false;}
	//�_���w�肵�đI���`�F�b�N���s��
	virtual bool testIntersect(const SPoint& ){return false;}
	//��L�͈͂�Ԃ�
	virtual SRect area() = 0;

	virtual bool readCe3(SReadCE3& rce3) = 0;
	virtual bool writeCe3(SWriteCE3& wce3,SPoint* pOrigin=NULL) = 0;

public:
	//XY���S���W���w�肵�āA��]���s��
	static SPoint rotatePoint(SPoint pt,SPoint pCenter);
	//X���W���w�肵�č��E���]���s��
	static SPoint mirrorPoint(SPoint pt,int xCenter);
	//p0��p1��[�_���W�Ƃ������p�̏d�Ȃ���e�X�g
	static bool testIntersectLinePoint(const SPoint& p0,const SPoint& p1,const SPoint& p);

};
