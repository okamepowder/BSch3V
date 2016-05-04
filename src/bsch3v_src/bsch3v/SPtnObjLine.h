/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

#pragma once
#include "sptnobj.h"

class SPtnObjLine :	public SPtnObj
{

protected:
	SPoint m_p0;
	SPoint m_p1;
	bool m_validArea;
	SRect m_area;
	bool m_bCurve;
	SPoint m_pC0;
	SPoint m_pC1;


public:
	//���ʂ̃C���^�[�t�F�[�X
	SPtnObjLine(void);
	~SPtnObjLine(void);
	int id(){return VECTPTN_L;}
//	SPtnObjLine(const SPtnObjLine&);				//�R�s�[�R���X�g���N�^
//	SPtnObjLine& operator=(const SPtnObjLine&);		//������Z�q
	SPtnObj* duplicate();								//���������
	bool qValid();									//�L���ȃf�[�^���ǂ�����Ԃ��B

	//�O���b�v�̐���Ԃ�
	int gripCount();
	//�O���b�v�̈ʒu��Ԃ�
	SPoint gripPos(int grip);
	//�O���b�v�̈ʒu�𓮂���
	void setGripPos(int grip,int x,int y);




	enum GRIPPOINT{
		GRIPPOINT_P0			=0,
		GRIPPOINT_P1			=1,
		GRIPPOINT_C0			=2,
		GRIPPOINT_C1			=3
	};



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
	
	void SetP0(const SPoint&);
	void SetP1(const SPoint&);
	const SPoint& GetP0(){return m_p0;}
	const SPoint& GetP1(){return m_p1;}

	void SetPC0(const SPoint&);
	void SetPC1(const SPoint&);
	const SPoint& GetPC0(){return m_pC0;}
	const SPoint& GetPC1(){return m_pC1;}

	void setCurve(bool curve);	//�Ȑ��t���O�̐ݒ�
	bool getCurve(){return m_bCurve;} //�Ȑ��t���O�̎擾

	void setDefaultCtrlPos();	//�Ȑ��̐���_���f�t�H���g�ʒu�ɐݒ肷��B






};
