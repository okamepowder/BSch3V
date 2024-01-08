/****************************************************************************
    BSch3V schematic capture
	Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

/****************************************************************************
** XBSch�}�ʗv�f���N���X
****************************************************************************/
#ifndef XBSCHLINE_H
#define XBSCHLINE_H

#include "xbsch.h"
#include "xbschobj.h"
#include "SBezier.h"
//#include <SPoint.h>
//#include <SRect.h>

class SXBSchLine:public SXBSchObj
{
public:
	//�R���X�g���N�^
	SXBSchLine();
	//�f�X�g���N�^
	virtual ~SXBSchLine(){}
	//�I����Ԃ̂Ƃ��̃X�i�b�v�|�C���g���擾����
	SPoint snapPoint();

	enum EDITPOINT{
		EDITPOINT_P1			=0,
		EDITPOINT_P2			=1,
		EDITPOINT_C1			=2,
		EDITPOINT_C2			=3
	};


public:
	//�_���w�肵�đI���d�v�x��Ԃ�
	int qSelectWeight(const SPoint&);
	//�͈͂��w�肵�đI�����s��
	unsigned testSelection(const SRect& );
	//�͈͂�I�����ăh���b�O�I�����s��
	unsigned testSelectionForDrag(const SRect& );
	//�`�悪�K�v���ǂ�����Ԃ�
	bool qRedraw(const SRect& );
	//�s������Ԃ�
	//int pinCount(){return 2;}
	//n�Ԗڂ̃s���̈ʒu�𓾂�B
	//SPoint pinPosition(int);
	SRect area();
	SRect rotateRect();
	//�L���ȃI�u�W�F�N�g�Ȃ�true��Ԃ��B�����̂Ȃ������͗L���ł͂Ȃ��B
	virtual bool qValid(){return(m_p1 != m_p2);}

	virtual bool isHorizontal(){return (m_p1.y() == m_p2.y());}	//2008/06/26
	virtual bool isVertical(){return (m_p1.x() == m_p2.x());}	//2008/06/26

		//�P�ƑI���̂Ƃ��Ƀ~���[�\���ǂ�����Ԃ�
	bool canMirrorSingleSelected() {return (!isHorizontal() && !isVertical()); }//2008/06/26

	//X���W���w�肵�č��E���]���s��
	void mirrorH(int x);
	//���E���]���s��
	void mirrorH();
	//XY���S���W���w�肵�āA��]���s��
	void rotate(const SPoint& p);
	//��]���s��
	void rotate();

protected:
	//�X�g���[������̓ǂݍ���
	//bool readStreamEx(SCsvIStream* pStrm,const SPoint* pptOrigin,const TCHAR* pTag);
	//�X�g���[���ւ̏�������
	//bool writeStreamEx(SCsvOStream* pStrm,const SPoint* pptOrigin,const TCHAR* pTag);

	//�X�g���[������̓ǂݍ���
	virtual bool readCe3Ex(SReadCE3& rce3,const SPoint* pptOrigin,const TCHAR* pTag);
	//�X�g���[���ւ̏�������
	virtual bool writeCe3Ex(SWriteCE3& wce3,const SPoint* pptOrigin,const TCHAR* pTag);

};

//���C���[�N���X
class SXBSchWire:public SXBSchLine
{
public:
	//�R���X�g���N�^
	SXBSchWire(){}
	//�f�X�g���N�^
	~SXBSchWire(){}
/*
	//�X�g���[������̓ǂݍ��� �` -WIRE
	bool readStream(SCsvIStream* pStrm,const SPoint* pptOrigin);
	//�X�g���[���ւ̏������݁@+WIRE �` -WIRE
	bool writeStream(SCsvOStream* pStrm,const SPoint* pptOrigin);
*/
	bool readCe3(SReadCE3&,const SPoint* ,SXBSchDoc*);
	bool writeCe3(SWriteCE3& ,const SPoint* );

	//ID�̎擾
	unsigned id(){return ID_WIRE;}
	unsigned objectFilterValue(){return OBJMASK_WIRE;} 
	//�������s��
	SXBSchObj* duplicate();
};

//�o�X�N���X
class SXBSchBus:public SXBSchLine
{
public:
	//�R���X�g���N�^
	SXBSchBus(){}
	//�f�X�g���N�^
	~SXBSchBus(){}
	//ID�̎擾
	unsigned id(){return ID_BUS;}
	unsigned objectFilterValue(){return OBJMASK_BUS;}
/*
	//�X�g���[������̓ǂݍ��� �` -BUS
	bool readStream(SCsvIStream* pStrm,const SPoint* pptOrigin);
	//�X�g���[���ւ̏������݁@+BUS �` -BUS
	bool writeStream(SCsvOStream* pStrm,const SPoint* pptOrigin);
*/
	bool readCe3(SReadCE3&,const SPoint* ,SXBSchDoc*);
	bool writeCe3(SWriteCE3& ,const SPoint* );

	//�������s��
	SXBSchObj* duplicate();
};


//#define MIN_MARKER_WIDTH 3
//#define MAX_MARKER_WIDTH 101


//�}�[�J�[���C���N���X
class SXBSchMarkerLine:public SXBSchLine
{
public:

	static const unsigned int DEFAULT_COLOR = 8454016;
	static const int MIN_MARKER_WIDTH = 3;
	static const int DEFAULT_MARKER_WIDTH = 7;
	static const int MAX_MARKER_WIDTH = 100;


	//�R���X�g���N�^
	SXBSchMarkerLine(){
		m_width = DEFAULT_MARKER_WIDTH;
		m_nColor = DEFAULT_COLOR;
	}
	//�f�X�g���N�^
	~SXBSchMarkerLine(){}

	//�͈͂��w�肵�đI�����s��
	unsigned testSelection(const SRect& );
	//�͈͂�I�����ăh���b�O�I�����s��
	unsigned testSelectionForDrag(const SRect& );


	
/*
	//�X�g���[������̓ǂݍ��� �` -DASH
	bool readStream(SCsvIStream* pStrm,const SPoint* pptOrigin);
	//�X�g���[���ւ̏������݁@+DASH �` -DASH
	bool writeStream(SCsvOStream* pStrm,const SPoint* pptOrigin);
*/
	bool readCe3(SReadCE3&,const SPoint* ,SXBSchDoc*);
	bool writeCe3(SWriteCE3& ,const SPoint* );

	//�}�[�J�[���C���̊O�����`������4�̃|�C���g�̍��W�𓾂�
	void outlinePoint(SPoint*);

	SRect area();

	//�A�g���r���[�g�̕ύX���\�ȏꍇ��true��Ԃ��B
	bool canAttribute() {return true;}

	bool pointIsInside(const SPoint& ,const SPoint* );

	
	//ID�̎擾
	unsigned id(){return ID_MARKER;}
	unsigned objectFilterValue(){return OBJMASK_MARKER;} 
	//�������s��
	SXBSchObj* duplicate();

	int m_width;
	unsigned int m_nColor;
public:
	bool keepOrder(){return true;}


};



#endif
