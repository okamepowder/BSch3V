/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

/****************************************************************************
** XBSch�}�ʗv�f�N���X�̎���
****************************************************************************/
#include "stdafx.h"
#include <stdio.h>
//#include <qnamespace.h>
#include "xbschobj.h"


//�R���X�g���N�^
SXBSchObj::SXBSchObj()
{
	m_PlaceNum = 0;					//�z�u����ԍ��̏�����
	m_DeleteNum = -1;
	m_pRefObj = NULL;	//�Q�ƃI�u�W�F�N�g�ւ̃|�C���^
	m_Layer = 0;					//�z�u���C���[
	m_Selection = NOT_SELECTED;		//�I�����
	m_p1.setX(-1);
	m_p1.setY(-1);
}

//�ʒu�𓮂���
void SXBSchObj::move(int offsetx,int offsety)
{
	SPoint p(offsetx,offsety);
	switch(m_Selection & SELECT_ALL){
	case NOT_SELECTED:
	case SELECT_ALL:
		m_p1 += p;
		m_p2 += p;
		break;
	case SELECT_P1:
		m_p1 += p;
		break;
	case SELECT_P2:
		m_p2 += p;
		break;
	}
}	
	
//X���W���w�肵�č��E���]���s��
void SXBSchObj::mirrorH(int x)
{
	m_p1.setX(x*2 -m_p1.x());
	m_p2.setX(x*2 -m_p2.x());
}

//���E���]���s��
void SXBSchObj::mirrorH()
{
	int x= (m_p1.x() + m_p2.x())/2;
	mirrorH(x);
}
	
//XY���S���W���w�肵�āA��]���s��
void SXBSchObj::rotate(const SPoint& p)
{
	int newX,newY;
	newY = p.y() + (m_p1.x() - p.x()) ;
	newX = p.x() - (m_p1.y() - p.y()) ;
	m_p1.setY( newY );
	m_p1.setX( newX );

	newY = p.y() + (m_p2.x() - p.x()) ;
	newX = p.x() - (m_p2.y() - p.y()) ;
	m_p2.setY( newY );
	m_p2.setX( newX );
}

//��]���s��
void SXBSchObj::rotate()
{
	SPoint p;
	p.setX((m_p1.x()+m_p2.x())/2);
	p.setY((m_p1.y()+m_p2.y())/2);
	rotate(p);
}

//�I���t���O���Z�b�g����
void  SXBSchObj::setSelection(unsigned sel)
{
	m_Selection = sel;
}

//�I���t���O�����Z�b�g����
void  SXBSchObj::resetSelection()
{
	m_Selection = 0;
}



////p��rcArea�̓��O�R�[�h��Ԃ�
//unsigned SXBSchObj::SetOutCode(const SPoint& p,const SRect& rcArea)
//{
//	unsigned outcode=0;											//  UL  | U | UR
//	if		(p.x() < rcArea.l() )	outcode |= OUTCODE_LEFT;	//  ----+---+----
//	else if (p.x() > rcArea.r() )	outcode |= OUTCODE_RIGHT;	//   L  | 0 |  R
//	if		(p.y() < rcArea.t() )	outcode |= OUTCODE_UPPER;	//  ----+---+----
//	else if	(p.y() > rcArea.b() )	outcode |= OUTCODE_LOWER;	//  LL  | L | LR
//	return outcode;
//}

//�_�Ƌ�`�̏d�Ȃ�����A�d�ݏ��t���ŕԂ�
int SXBSchObj::PointAndRectMag(const SPoint& p,const SRect& rcArea)
{
	int lp,rp,tp,bp,xp,yp;								//   left						  right
	if(( lp = p.x() - rcArea.l()  )<=0) return 0;	// ---+----------P------------------+----
	if(( rp = rcArea.r() - p.x() )<=0) return 0;	//         lp              rp
	if(( tp = p.y() - rcArea.t()   )<=0) return 0;
	if(( bp = rcArea.b()- p.y() )<=0) return 0;

	if(lp<rp) xp=lp; else xp=rp;						//���������̐��l���u�G�b�W���痣��Ă���l�v�Ƃ��Ďg��
	if(tp<bp) yp=tp; else yp=bp;
	xp=(xp*16)/(lp+rp);				//�ő�l�͂W
	yp=(yp*16)/(tp+bp);				//�ő�l�͂W
	return (xp*yp)+1;				//�ő�l�͂U�T
}

