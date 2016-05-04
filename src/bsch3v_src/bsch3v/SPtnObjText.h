/****************************************************************************
     BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#pragma once
#include "sptnobj.h"
#include <string>
using namespace std;

class SPtnObjText :	public SPtnObj
{
public:
	//���ʂ̃C���^�[�t�F�[�X
	SPtnObjText(void);
	~SPtnObjText(void);
	int id(){return VECTPTN_TXT;}
	SPtnObjText(const SPtnObjText&);					//�R�s�[�R���X�g���N�^
	SPtnObjText& operator=(const SPtnObjText&);		//������Z�q
	SPtnObj* duplicate();							//���������
	bool qValid();									//�L���ȃf�[�^���ǂ�����Ԃ��B

	//�O���b�v�̐���Ԃ�
	int gripCount(){return 1;}
	//�O���b�v�̈ʒu��Ԃ�
	SPoint gripPos(int grip){return m_p0;}
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
	void setDrawSize(SSize size);
	void setText(const TCHAR* csz);
	bool readCe3(SReadCE3& rce3);
	bool writeCe3(SWriteCE3& wce3,SPoint* pOrigin=NULL);
	void setFontName(const TCHAR* psz);
	void setFontSize(int fontSize);
	void setFontBold(bool istrue);
	void setFontItalic(bool istrue);
	void setFontUnderline(bool istrue);
	void setFontStrikeOut(bool istrue);

public:
	SPoint m_p0;
	bool m_bHoriz;
	wstring m_strText;
	int m_align;
	bool m_drawSizeIsValid;
	SSize m_drawSize;
	wstring	m_fontName;
	int		m_fontSize;
	bool	m_fontBold;
	bool	m_fontItalic;
	bool	m_fontUnderline;
	bool	m_fontStrikeOut;


};
