/****************************************************************************
    BSch3V schematic capture
	Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

/****************************************************************************
** XBSch�}�ʗv�f���x���N���X
****************************************************************************/
#ifndef XBSCHLABEL_H
#define XBSCHLABEL_H

#include "xbsch.h"
#include "xbschobj.h"

class SXBSchLabel:public SXBSchObj
{
public:
	//�R���X�g���N�^
	SXBSchLabel();
	//�f�X�g���N�^
	virtual ~SXBSchLabel(){};

	bool readCe3(SReadCE3&,const SPoint* ,SXBSchDoc*);
	bool writeCe3(SWriteCE3& ,const SPoint* );
/*
	//�X�g���[������̓ǂݍ��� �` -LABEL
	bool readStream(SCsvIStream* pStrm,const SPoint* pptOrigin);
	//�X�g���[���ւ̏������݁@+LABEL �` -LABEL
	bool writeStream(SCsvOStream* pStrm,const SPoint* pptOrigin);
*/
    //ID�̎擾
	unsigned id(){return ID_LABEL;}
	unsigned objectFilterValue(){return OBJMASK_LABEL;} 
protected:
	wstring m_strText;
	bool m_bHoriz;
	bool	m_drawSizeIsValid;
	SSize	m_drawSize;


public:
	//�������s��
	SXBSchObj* duplicate();
	//���W�̐ݒ�  m_p2�ւ̐ݒ�𖳌��ɂ��Ă���
	void setX2(int){}
	void setY2(int){}
	void setP2(const SPoint&){}
	//���E���]���s��
	void mirrorH(){}	//�������Ȃ�
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

	bool drawSizeIsValid(){return m_drawSizeIsValid;}
	void setDrawSize(const SSize& size);	//�`��T�C�Y�̐ݒ�


public:
	//�L���ȃI�u�W�F�N�g�Ȃ�true��Ԃ�
	bool qValid(){return (m_strText.length()>0);}

	bool horizontal(){ return m_bHoriz; }
	void setHorizontal(bool horizFlag){m_bHoriz = horizFlag;}
	void setText(const TCHAR* psz);
	const TCHAR* text(){return m_strText.c_str();}
	//�A�g���r���[�g�̕ύX���\�ȏꍇ��true��Ԃ��B
	bool canAttribute(){return true;}
	
	//�P�ƑI���̂Ƃ��Ƀ~���[�\���ǂ�����Ԃ�
	bool canMirrorSingleSelected() {return false;}//2008/06/26
};

#endif
