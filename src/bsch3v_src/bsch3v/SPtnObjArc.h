/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#pragma once
#include "sptnobj.h"

class SPtnObjArc :
	public SPtnObj
{
public:
	//���ʂ̃C���^�[�t�F�[�X
	SPtnObjArc(void);
	~SPtnObjArc(void);
	int id(){return VECTPTN_AR;}
//	SPtnObjArc(const SPtnObjArc&);					//�R�s�[�R���X�g���N�^
//	SPtnObjArc& operator=(const SPtnObjArc&);		//������Z�q
	SPtnObj* duplicate();							//���������
	bool qValid();									//�L���ȃf�[�^���ǂ�����Ԃ��B

	//�O���b�v�̐���Ԃ�
	int gripCount(){return 7;}
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
	
	void setBeginPoint(int x,int y);			//�J�n�p�����W�Őݒ�B��ɒ��S�����߂Ă�������
	void setEndPoint(int x,int y);				//�I���p�����W�Őݒ�B��ɒ��S�����߂Ă�������
	SPoint AngleToPoint(int angle);				//�p�x(deg*16)����~����̓_�����߂�
	SPoint AngleToPoint500(int angle);			//�p�x(deg*16)���甼�a500�̉~����̓_�����߂�


public:
	SPoint m_ptC;		//���S���W
	int    m_nR;		//���a
	int	   m_nBegin;	//�J�n�p
	int	   m_nEnd;		//�I���p

	SPoint m_ptBegin;
	SPoint m_ptEnd;


protected:
	void normalizeAngle();
public:
	//static bool  Conv3PointArcToAR(SPoint pE0,SPoint pE1,SPoint p,SPoint& center,int& r,int& begin, int& end);
	static double  PointRad(double cx,double cy,double r,double x0,double y0);
};
