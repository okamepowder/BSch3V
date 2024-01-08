/****************************************************************************
    BSch3V schematic capture
	Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

/****************************************************************************
** XBSch�}�ʗv�f�N���X
****************************************************************************/
#ifndef XBSCHOBJ_H
#define XBSCHOBJ_H

#include <list>
#include <stack>
using namespace std;

#include "xbsch.h"
//#include <SPoint.h>
//#include <SRect.h>
#include "coord.h"
#include "ce3io.h"
#include "ObjFilter.h"

class SXBSchDoc;

/*
#define		NOT_SELECT	0
#define		SELECT_P1	1
#define		SELECT_P2	2
#define		SELECT_ALL	3

#define		OUTCODE_LEFT		0x0001
#define		OUTCODE_UPPER		0x0002
#define		OUTCODE_RIGHT		0x0004
#define		OUTCODE_LOWER		0x0008
*/

enum BSCH_TOOL_MODE
{
	TOOL_SELECTOR,
	TOOL_DRAG,
	TOOL_COMPONENT,
	TOOL_BUS,
	TOOL_WIRE,
	TOOL_DASH,
	TOOL_JUNCTION,
	TOOL_BENTRY,
	TOOL_ENTRY,
	TOOL_TAG,
	TOOL_LABEL,
	TOOL_COMMENT
};

enum ON_OBJECT
{
	ON_OBJ				=0x010000,
	ON_NAME				=0x020000,
	ON_NUM				=0x040000,
	ON_NODE				=0x080000,
	ON_CTRLPOINT		=0x100000
};


class SXBSchObj;

//typedef list<SXBSchObj*>::iterator XBSchObjListIterator;


class SXBSchObj
{
public:
	enum BSCHOBJ_ID
	{
		ID_JUNCTION		=0,
		ID_ENTRY		=1,
		ID_BENTRY		=2,
		ID_BUS			=3,
		ID_WIRE			=4,
		ID_DASH			=5,
		ID_LABEL		=6,
		ID_TAG			=7,
		ID_COMPONENT	=8,
		ID_SHEETINFO	=9,
		ID_COMMENT		=10,
		ID_MARKER		=11,
		ID_IMAGEOBJECT	=12,
//		ID_DELETEOBJ	=254,
		ID_ENDOFDATA	=255
	};

	enum SELECTION
	{
		NOT_SELECTED	=0,
		SELECT_P1		=1,
		SELECT_P2		=2,
		SELECT_ALL		=3
	};

	//enum OUTCODE
	//{
	//	OUTCODE_CENTER	=	0x0000,
	//	OUTCODE_LEFT	=	0x0001,
	//	OUTCODE_UPPER	=	0x0002,
	//	OUTCODE_RIGHT	=	0x0004,
	//	OUTCODE_LOWER	=	0x0008
	//};

protected:
	//�z�u�����Ƃ��̑���ԍ�
	int m_PlaceNum;

	//�폜�����Ƃ��̑���ԍ� -1�ł͂Ȃ��Ƃ��͏�������Ă��邱�Ƃ�����
	int m_DeleteNum;

	//�Q�ƃI�u�W�F�N�g�ւ̃|�C���^
	SXBSchObj* m_pRefObj;
	//�z�u���C���[
	int m_Layer;
	//�I�u�W�F�N�g�̍��W
	SPoint m_p1;
	SPoint m_p2;
	//�I�����
	unsigned m_Selection;

	bool isValidLayer(int n){return (n>=0 && n<8);}

protected:
	//XBSchObjListIterator m_OldDataIterator;
public:
	//virtual XBSchObjListIterator oldDataIterator(){return m_OldDataIterator;}
	//virtual void setOldDataIterator(XBSchObjListIterator oldDataIterator){m_OldDataIterator = oldDataIterator;}
	virtual bool keepOrder(){return false;}


public:
	//�R���X�g���N�^
	SXBSchObj();
	//�f�X�g���N�^
	virtual ~SXBSchObj(){};

//	virtual bool readStream(SCsvIStream* ,const SPoint* ){return false;}
//	virtual bool writeStream(SCsvOStream* ,const SPoint* ){return false;}

	virtual bool readCe3(SReadCE3&,const SPoint* ,SXBSchDoc* ){return false;}
//	virtual bool readCe3(SReadCE3&,const SPoint* ){return false;}
	virtual bool writeCe3(SWriteCE3& ,const SPoint* ){return false;}

	//ID�̎擾
	virtual unsigned id()=0;

	//�I���t�B���^�̂��߂̃r�b�g�l�擾	�����20091012�ɒǉ��������́Aid()�Ɠ������Ă��ǂ��悤�Ȃ���
	virtual unsigned objectFilterValue()=0;

	//�z�u�����Ƃ��̑���ԍ�
	void setPlaceNum(int n){m_PlaceNum = n;}
	//�z�u�����Ƃ��̑���ԍ��̎擾
	int placeNum(){return m_PlaceNum;}

	//���������Ƃ��̑���ԍ�
	void setDeleteNum(int n){m_DeleteNum = n;}
	//���������Ƃ��̑���ԍ��̎擾
	int deleteNum(){return m_DeleteNum;}
	


	//�Q�ƃI�u�W�F�N�g�ւ̃|�C���^�̐ݒ�
	void setRefObj(SXBSchObj* pObj){m_pRefObj = pObj;}
	//�Q�ƃI�u�W�F�N�g�ւ̃|�C���^���擾
	SXBSchObj* refObj(){return m_pRefObj;}
	//���C���[�̐ݒ�
	void setLayer(int n){m_Layer=n;}
	//���C���[�̎擾
	int layer(){return m_Layer;}
	//���W�̐ݒ�
	virtual void setX1(int x1){m_p1.setX(x1);}
	virtual void setY1(int y1){m_p1.setY(y1);}
	virtual void setX2(int x2){m_p2.setX(x2);}
	virtual void setY2(int y2){m_p2.setY(y2);}
	virtual void setP1(const SPoint& p){m_p1 = p;}
	virtual void setP2(const SPoint& p){m_p2 = p;}
	//���W�̎擾
	int x1(){return m_p1.x();}
	int y1(){return m_p1.y();}
	virtual int x2(){return m_p2.x();}
	virtual int y2(){return m_p2.y();}
	SPoint p1(){return m_p1;}
	virtual SPoint p2(){return m_p2;}
	virtual SPoint snapPoint(){return m_p1;}

	//�C���f�b�N�X���g�������W�̐ݒ�
	virtual void setPointIndex(int index,SPoint pt){
		if(index == 0)	m_p1 = pt;
		else			m_p2 = pt;
	}

	//�C���f�b�N�X���g�������W�̎擾
	virtual SPoint pointIndex(int index){
		if(index == 0)	return m_p1;
		else			return m_p2;
	}

	//�I����Ԃ̎擾
	unsigned selection(){return m_Selection;}
public:
	//�������s��
	virtual SXBSchObj* duplicate(){return NULL;}
	//�ʒu�𓮂���
	virtual void move(int offsetx,int offsety);
	//X���W���w�肵�č��E���]���s��
	virtual void mirrorH(int x);
	//���E���]���s��
	virtual void mirrorH();
	//XY���S���W���w�肵�āA��]���s��
	virtual void rotate(const SPoint& p);
	//��]���s��
	virtual void rotate();
	//�_���w�肵�đI���d�v�x��Ԃ�
	virtual int qSelectWeight(const SPoint&){return 0;}

	//�w��_������\�Ȃ��̂̏�ɂ��邩�ǂ���
	virtual int isEditablePoint(const SPoint&,int&){return 0;}

	//�I���t���O���Z�b�g����
	virtual void setSelection(unsigned sel);
	//�I������������
	virtual void resetSelection();
	//�͈͂��w�肵�đI�����s��
	virtual unsigned testSelection(const SRect& ){return 0;}
	//�͈͂�I�����ăh���b�O�I�����s��
	virtual unsigned testSelectionForDrag(const SRect& ){return 0;}
	//�`�悪�K�v���ǂ�����Ԃ�
	virtual bool qRedraw(const SRect& ){return 0;}
	//�s������Ԃ�
	virtual int pinCount()const {return 0;}
	//n�Ԗڂ̃s���̈ʒu�𓾂�B
	virtual SPoint pinPosition(int) const {return SPoint(-1,-1);}
	virtual SRect area(){return SRect(-1,-1,0,0);}
	virtual SRect rotateRect(){return SRect(-1,-1,0,0);}
	//�L���ȃI�u�W�F�N�g�Ȃ�true��Ԃ��B������̂Ȃ��e�L�X�g�A�����̂Ȃ������͗L���ł͂Ȃ��B
	virtual bool qValid(){return true;}
	//�A�g���r���[�g�̕ύX���\�ȏꍇ��true��Ԃ��B
	virtual bool canAttribute() {return false;}
	//�P�ƑI���̂Ƃ��Ƀ~���[�\���ǂ�����Ԃ�
	virtual bool canMirrorSingleSelected() {return true;}//2008/06/26
	//�P�ƑI���̂Ƃ��ɉ�]�\���ǂ�����Ԃ�
	virtual bool canRotateSingleSelected() {return true;}//2008/06/26
public:
	//p��rcArea�̓��O�R�[�h��Ԃ�
	//static unsigned SetOutCode(const SPoint& p,const SRect& rcArea);
	//�_�Ƌ�`�̏d�Ȃ�����A�d�ݏ��t���ŕԂ�
	static int PointAndRectMag(const SPoint& p,const SRect& rcArea);
};

#endif
