/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#pragma once
#include "sptnobj.h"

#define VECTPLOYGON_MAXNODE 32	//���p�`�̍ő咸�_��

class SPtnObjPolygon :
	public SPtnObj
{
public:
	SPtnObjPolygon(void);
	~SPtnObjPolygon(void);
	int id(){return VECTPTN_PG;}
	SPtnObjPolygon(const SPtnObjPolygon&);					//�R�s�[�R���X�g���N�^
	SPtnObjPolygon& operator=(const SPtnObjPolygon&);		//������Z�q
	SPtnObj* duplicate();							//���������
	bool qValid();									//�L���ȃf�[�^���ǂ�����Ԃ��B

	//�O���b�v�̐���Ԃ�
	int gripCount(){return m_nCount;}
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
	int	    m_nCount;		//���_�̐�
	SPoint* m_pPt;			//���_���W
	//int m_nFill;
protected:
	void normalize();
};
