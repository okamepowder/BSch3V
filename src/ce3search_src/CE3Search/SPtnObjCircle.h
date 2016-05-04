/****************************************************************************
    BSch3V schematic capture
	Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#pragma once
#include "sptnobj.h"

class SPtnObjCircle :
	public SPtnObj
{
public:
//���ʂ̃C���^�[�t�F�[�X
	SPtnObjCircle(void);
	~SPtnObjCircle(void);
	int id(){return VECTPTN_C; }
	SPtnObj* duplicate();								//���������
	bool qValid();									//�L���ȃf�[�^���ǂ�����Ԃ��B
	//�O���b�v�̐���Ԃ�
	int gripCount(){return 2;}
	//�O���b�v�̈ʒu��Ԃ�
	SPoint gripPos(int grip);
	//�O���b�v�̈ʒu�𓮂���
	void setGripPos(int grip,int x,int y);

	void move(int offsetx,int offsety);				//�ʒu�𓮂���
	void mirrorH(int x); 							//X���W���w�肵�č��E���]���s��
	void mirrorH();									//���E���]���s��
	void rotate(const SPoint& p);					//XY���S���W���w�肵�āA��]���s��
	void rotate();									//��]���s��
	bool testIntersect(const SRect& );				//�͈͂��w�肵�đI���`�F�b�N���s��
	bool testIntersect(const SPoint& );				//�_���w�肵�đI���`�F�b�N���s��
	SRect area();									//��L�͈͂�Ԃ�
	bool readCe3(SReadCE3& rce3);
	bool writeCe3(SWriteCE3& wce3,SPoint* pOrigin=NULL);
public:
	SPoint m_p0;
	SPoint m_p1;
	//int m_nFill;
};
