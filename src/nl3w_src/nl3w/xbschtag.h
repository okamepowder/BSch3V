/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// Copyright (C) 2002 H.Okada(suigyodo) All rights reserved.
/****************************************************************************
** XBSch�}�ʗv�f�^�O�N���X
****************************************************************************/
#ifndef XBSCHTAG_H
#define XBSCHTAG_H

#include "xbsch.h"
#include "xbschobj.h"

class SXBSchTag:public SXBSchObj
{

public:

	enum BSCHTAG_TYPE{
		TAG_SQ =0,
		TAG_LU =1,
		TAG_RD =2,
		TAG_BI =3
	};

	//�R���X�g���N�^
	SXBSchTag();
	//�f�X�g���N�^
	virtual ~SXBSchTag(){};

	bool readCe3(SReadCE3&,const SPoint* ,SXBSchDoc*);
	bool writeCe3(SWriteCE3& ,const SPoint* );
	

	//ID�̎擾
	unsigned id(){return ID_TAG;}
	unsigned objectFilterValue(){return OBJMASK_TAG;} 
protected:
	string m_strText;
	bool m_bHoriz;
	int m_nType;
public:
	int width();
	void setp2();
	// m_p1�����܂��͉��ɂȂ�悤�ɒ�������
	void normalize();
	//�������s��
	SXBSchObj* duplicate();
	//���W�̐ݒ�  m_p2�ւ̐ݒ�𖳌��ɂ��Ă���
	void setX2(int){}
	void setY2(int){}
	void setP2(const SPoint&){}
	//X���W���w�肵�č��E���]���s��
	void mirrorH(int x);
	//XY���S���W���w�肵�āA��]���s��
	void rotate(const SPoint& p);
	//��]���s��
	void rotate();
	//�_���w�肵�đI���d�v�x��Ԃ�
	int qSelectWeight(const SPoint& p);
	//�͈͂��w�肵�đI�����s��
	unsigned testSelection(const SRect& rc);
	//�͈͂�I�����ăh���b�O�I�����s��
	unsigned testSelectionForDrag(const SRect& rc);
	//�`�悪�K�v���ǂ�����Ԃ�
	bool qRedraw(const SRect& rc);
	//��L�͈͂�Ԃ�
	SRect area();
	SRect rotateRect();
	//�s������Ԃ�
	int pinCount()const {return 2;}
	//n�Ԗڂ̃s���̈ʒu�𓾂�B
	SPoint pinPosition(int n) const;

public:
	bool canAttribute(){return true;}
	//�L���ȃI�u�W�F�N�g�Ȃ�true��Ԃ�
	bool qValid(){return true;}

	bool horizontal(){ return m_bHoriz; }
	void setHorizontal(bool horizFlag){m_bHoriz = horizFlag;}
	int	 tagType(){ return m_nType; }
	void setTagType(int nType){ assert(nType>=0 && nType<=3); m_nType = nType ; }
	void setText(const char* psz);
	const char* text(){return m_strText.c_str();}

	//�P�ƑI���̂Ƃ��Ƀ~���[�\���ǂ�����Ԃ�
	bool canMirrorSingleSelected() { //2008/06/26
		if(!m_bHoriz) return false;
		if(m_nType == TAG_SQ || m_nType == TAG_BI) return false;
		return true;
	}


};

#endif
