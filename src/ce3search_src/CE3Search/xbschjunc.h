/****************************************************************************
    BSch3V schematic capture
	Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

/****************************************************************************
** XBSch�}�ʗv�f�W�����N�V�����N���X
****************************************************************************/
#ifndef XBSCHJUNC_H
#define XBSCHJUNC_H

#include "xbsch.h"
#include "xbschobj.h"

class SXBSchJunc:public SXBSchObj
{
public:
	//�R���X�g���N�^
	SXBSchJunc();
	//�f�X�g���N�^
	virtual ~SXBSchJunc(){};

/*	//�X�g���[������̓ǂݍ��� �` -JUNCTION
	bool readStream(SCsvIStream* pStrm,const QPoint* pptOrigin);
	//�X�g���[���ւ̏������݁@+JUNCTION �` -JUNCTION
	bool writeStream(SCsvOStream* pStrm,const QPoint* pptOrigin);
*/
	bool readCe3(SReadCE3&,const SPoint* ,SXBSchDoc* );
	bool writeCe3(SWriteCE3& ,const SPoint* );

	
	
	//ID�̎擾
	unsigned id(){return ID_JUNCTION;}
	unsigned objectFilterValue(){return OBJMASK_JUNCTION;} 
public:
	//�������s��
	SXBSchObj* duplicate();
	//���W�̐ݒ�  m_p2�ւ̐ݒ�𖳌��ɂ��Ă���
	void setX2(int){}
	void setY2(int){}
	void setP2(const SPoint&){}
	//���E���]���s��
	void mirrorH(){}	//�������Ȃ�
	//��]���s��
	void rotate(){}		//�������Ȃ�
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

	//�P�ƑI���̂Ƃ��Ƀ~���[�\���ǂ�����Ԃ�
	bool canMirrorSingleSelected() {return false;}//2008/06/26
	//�P�ƑI���̂Ƃ��ɉ�]�\���ǂ�����Ԃ�
	bool canRotateSingleSelected() {return false;}//2008/06/26


};

#endif
