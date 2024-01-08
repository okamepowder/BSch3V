/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

/****************************************************************************
** XBSch�}�ʗv�f�C���[�W�I�u�W�F�N�g
****************************************************************************/
#ifndef XBSCHIMAGEOBJ_H
#define XBSCHIMAGEOBJ_H

#include "xbsch.h"
#include "xbschobj.h"

class SImageData;

#define IMAGEOBJ_MIN_MAGNIFICATION  5
#define IMAGEOBJ_MAX_MAGNIFICATION 200


class SXBSchImageObj :	public SXBSchObj
{
public:
	SXBSchImageObj(void);
	virtual ~SXBSchImageObj(void);

	bool readCe3(SReadCE3&,const SPoint* ,SXBSchDoc* );
	bool writeCe3(SWriteCE3& ,const SPoint* );


protected:
	SImageData*	m_pImageData;
	int m_drawMagnification;

protected:
	void DeleteImageData();


public:
	int		ImageWidth();
	int		ImageHeight();
	int		DrawMagnification(){return m_drawMagnification;}
	void	SetDrawMagnification(int n);
	const SImageData* ImageData(){return m_pImageData;}

	bool LoadDIB(const char* fname);

public:
	bool keepOrder(){return true;}

public:
	//ID�̎擾
	unsigned id(){return ID_IMAGEOBJECT;}
	unsigned objectFilterValue(){return OBJMASK_IMAGE;} 
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

	//��L�͈͂�Ԃ�
	SRect area();
	SRect rotateRect();


	//�_���w�肵�đI���d�v�x��Ԃ�
	int qSelectWeight(const SPoint& p);
	//�͈͂��w�肵�đI�����s��
	unsigned testSelection(const SRect& rc);
	//�͈͂�I�����ăh���b�O�I�����s��
	unsigned testSelectionForDrag(const SRect& rc);
	//�`�悪�K�v���ǂ�����Ԃ�
	bool qRedraw(const SRect& rc);

	//�P�ƑI���̂Ƃ��Ƀ~���[�\���ǂ�����Ԃ�
	bool canMirrorSingleSelected() {return false;}
	//�P�ƑI���̂Ƃ��ɉ�]�\���ǂ�����Ԃ�
	bool canRotateSingleSelected() {return false;}	
	//�A�g���r���[�g�̕ύX���\�ȏꍇ��true��Ԃ��B
	bool canAttribute(){return true;}

};
#endif
