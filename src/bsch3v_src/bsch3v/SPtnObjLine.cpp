/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

#include "stdafx.h"
#include "sptnobjline.h"
#include "SBezier.h"


SPtnObjLine::SPtnObjLine(void)
{
	m_p0 = SPoint(-1,-1);
	m_p1 = SPoint(-1,-1);
	m_pC0 = SPoint(-1,-1);
	m_pC1 = SPoint(-1,-1);
	m_bCurve = false;
	m_validArea = false;
}

SPtnObjLine::~SPtnObjLine(void)
{
}
/*
//�R�s�[�R���X�g���N�^
SPtnObjLine::SPtnObjLine(const SPtnObjLine& obj)
{
	*this = obj;
}

//������Z�q
SPtnObjLine& SPtnObjLine::operator=(const SPtnObjLine& obj)
{
	if(this != &obj){
		*((SPtnObj*)this) = *((SPtnObj*)&obj);	//��{�N���X�̃����o�[�̃R�s�[
		m_p0 = obj.m_p0;
		m_p1 = obj.m_p1;
		m_nWidth= obj.m_nWidth;
	}
	return *this;
}
*/

//�O���b�v�̐���Ԃ�
int  SPtnObjLine::gripCount()
{
	if(m_bCurve){
		return 4;
	}else{ 
		return 2;
	}
}

void SPtnObjLine::SetP0(const SPoint& p)
{
	m_p0 = p;
	m_validArea = false;
}

void SPtnObjLine::SetP1(const SPoint& p)
{
	m_p1 = p;
	m_validArea = false;
}

void SPtnObjLine::SetPC0(const SPoint& p)
{
	m_pC0 = p;
	m_validArea = false;
}

void SPtnObjLine::SetPC1(const SPoint& p)
{
	m_pC1 = p;
	m_validArea = false;
}



//���������
SPtnObj* SPtnObjLine::duplicate()
{
	return new SPtnObjLine(*this);
}

//�L���ȃf�[�^���ǂ�����Ԃ��B
bool SPtnObjLine::qValid()
{
	return true;
//	if(m_p0 == m_p1) return false;
//	else return true;
}

//�ʒu�𓮂���
void SPtnObjLine::move(int offsetx,int offsety)
{
	m_p0.setX(m_p0.x()+offsetx);
	m_p0.setY(m_p0.y()+offsety);
	m_p1.setX(m_p1.x()+offsetx);
	m_p1.setY(m_p1.y()+offsety);

	m_pC0.setX(m_pC0.x()+offsetx);
	m_pC0.setY(m_pC0.y()+offsety);
	m_pC1.setX(m_pC1.x()+offsetx);
	m_pC1.setY(m_pC1.y()+offsety);

	m_validArea = false;

}

//X���W���w�肵�č��E���]���s��
void SPtnObjLine::mirrorH(int x)
{
	m_p0 = mirrorPoint(m_p0,x);
	m_p1 = mirrorPoint(m_p1,x);

	m_pC0 = mirrorPoint(m_pC0,x);
	m_pC1 = mirrorPoint(m_pC1,x);

	m_validArea = false;
}

//���E���]���s��
void SPtnObjLine::mirrorH()
{
	int x = (m_p0.x()+m_p1.x())/2;
	mirrorH(x);
}

//XY���S���W���w�肵�āA��]���s��
void SPtnObjLine::rotate(const SPoint& p)
{
	m_p0 = rotatePoint(m_p0,p);
	m_p1 = rotatePoint(m_p1,p);

	m_pC0 = rotatePoint(m_pC0,p);
	m_pC1 = rotatePoint(m_pC1,p);

	m_validArea = false;
}

//��]���s��
void SPtnObjLine::rotate()
{
	int x = (m_p0.x()+m_p1.x())/2;
	int y = (m_p0.y()+m_p1.y())/2;
	SPoint p(x,y);
	rotate(p);
}

//��L�͈͂�Ԃ�
SRect SPtnObjLine::area()
{
	TRACE("SPtnObjLine::area()\n");
	if(!m_validArea){
		if(!m_bCurve){
			m_area = SRect(m_p0,m_p1);
			m_area.normalize();
		}else{
			SPoint ptArray[SBezier::m_nBezierNodes+1];
			SBezier::CreateNodes(m_p0,m_p1,m_pC0,m_pC1,ptArray);
			m_area = SRect(m_p0,m_p1);
			m_area.normalize();
			for(int i=1;i<SBezier::m_nBezierNodes;i++){
				m_area |= ptArray[i];
			}
		}
		m_validArea = true;
	}
	return m_area;
}




//�͈͂��w�肵�đI���`�F�b�N���s��
bool SPtnObjLine::testIntersect(const SRect& rc)
{
	SRect rcObj = area();
	if(	(rc.t()>rcObj.b())
	||	(rc.b()<rcObj.t())
	||	(rc.l()>rcObj.r())
	||	(rc.r()<rcObj.l())) return false;


	return  true;
	//	(rc.t()<rcObj.t())
	//&&	(rc.b()>rcObj.b())
	//&&	(rc.l()<rcObj.l())
	//&&	(rc.r()>rcObj.r());

	//if(!rc.intersect(m_p0)) return false;
	//if(!rc.intersect(m_p1)) return false;
	//return true;
}

//�_���w�肵�đI���`�F�b�N���s��	LCoV�ł͂��̊֐��͎g���Ă��Ȃ��͂�
bool SPtnObjLine::testIntersect(const SPoint& p)
{
	return this->testIntersectLinePoint(m_p0,m_p1,p);
}

//---------------------------------


//�O���b�v�̈ʒu��Ԃ�
SPoint SPtnObjLine::gripPos(int grip)
{
	switch(grip){
	case 0:
		return m_p0;
	case 1:
		return m_p1;
	case 2:
		return m_pC0;
	default:	//case 3:
		return m_pC1;
	}
}

//�O���b�v�̈ʒu�𓮂���
void SPtnObjLine::setGripPos(int grip,int x,int y)
{
	int dx,dy;
	switch(grip){
	case 0:
		dx = x-m_p0.x();
		dy = y-m_p0.y();
		m_p0=SPoint(x,y);
		m_pC0.setX(m_pC0.x()+dx);
		m_pC0.setY(m_pC0.y()+dy);
		break;
	case 1:
		dx = x-m_p1.x();
		dy = y-m_p1.y();
		m_p1=SPoint(x,y);
		m_pC1.setX(m_pC1.x()+dx);
		m_pC1.setY(m_pC1.y()+dy);
		break;
	case 2:
		m_pC0=SPoint(x,y);
		break;
	default:	//case 3:
		m_pC1=SPoint(x,y);
	}
	m_validArea = false;
}


//�Ȑ��t���O�̐ݒ�
void SPtnObjLine::setCurve(bool curve){
	m_validArea=false; 

	if(!m_bCurve && curve){
		setDefaultCtrlPos();
	}
	m_bCurve = curve;
}


//�Ȑ��̐���_���f�t�H���g�ʒu�ɐݒ肷��B
void SPtnObjLine::setDefaultCtrlPos()
{
	if(m_p0==m_p1){
		m_pC0 = SPoint(m_p1.x()+10,m_p1.y());
		m_pC1 = SPoint(m_p1.x()-10,m_p1.y());
		return;
	}
	int dx = m_p1.x()-m_p0.x();
	int dy = m_p1.y()-m_p0.y();
	if(abs(dx)>20 || abs(dy)>20){	//���̐��ɏ\���Ȓ���������Ƃ��́A����_�𗼒[�_�������1/3�̈ʒu�ɂ���
		m_pC0 = SPoint(m_p0.x()+dx/3, m_p0.y()+dy/3);
		m_pC1 = SPoint(m_p1.x()-dx/3, m_p1.y()-dy/3);
	}else{ //���̐����Z���Ƃ��́A���Α��̒[�_�𒴂����ʒu�ɂ���
		m_pC0 = SPoint(m_p0.x()+dx*3/2, m_p0.y()+dy*3/2);
		m_pC1 = SPoint(m_p1.x()-dx*3/2, m_p1.y()-dy*3/2);
	}
}



bool SPtnObjLine::readCe3(SReadCE3& rce3)
{
	bool f_y1 = false;
	int i;
	int width = 1;
	int style = 0;
	int pointReadCount =0;
	int nCurve = 0;

	//int x0    = 0;
	//int y0    = 0;
	//int x1    = 0;
	//int y1    = 0;

	wstring str;
	while(rce3.ReadRecord(str)!=WEOF){
		if(str[0] == '-'){
			if(str != _T("-L")){
				return false;
			}else{
				break;
			}
		}else if(str[0] == '+'){
			str[0]='-';
			if(rce3.SkipTo(str) == WEOF) return false;
		}else{
			int n;
			int l=str.length();
			for(n=0;n<l;n++){
				if(str[n]==':') break;
			}
			if(0<n && n<(l-1)){	// : �ŕ�����ꂽ���R�[�h�ł���
				wstring var = str.substr(0,n);				//�擪����:�̎�O�܂�
				//int nParam = _tstoi(str.substr(n+1).c_str()); //:����I�[�܂ł𐔒l�ɕϊ�
				if(var==_T("W")){
					width = _tstoi(str.substr(n+1).c_str()); //:����I�[�܂ł𐔒l�ɕϊ�
				}else
				if(var==_T("S")){
					style = _tstoi(str.substr(n+1).c_str()); //:����I�[�܂ł𐔒l�ɕϊ�
				}else
				if(var==_T("CV")){
					nCurve = _tstoi(str.substr(n+1).c_str()); //:����I�[�܂ł𐔒l�ɕϊ�
				}else
				if(var==_T("X")){
					i = _tstoi(str.substr(n+1).c_str()); //:����I�[�܂ł𐔒l�ɕϊ�
					switch(pointReadCount){
						case 0:
							m_p0.setX(i);
							break;
						case 1:
							m_p1.setX(i);
							break;
						case 2:
							m_pC0.setX(i);
							break;
						default:	//case 3:
							m_pC1.setX(i);
							break;
					}
				}else
				if(var==_T("Y")){
					i = _tstoi(str.substr(n+1).c_str()); //:����I�[�܂ł𐔒l�ɕϊ�
					switch(pointReadCount){
						case 0:
							m_p0.setY(i);
							break;
						case 1:
							m_p1.setY(i);
							break;
						case 2:
							m_pC0.setY(i);
							break;
						default:	//case 3:
							m_pC1.setY(i);
							break;
					}
					pointReadCount++;
				}
			}
		}
	}
	m_width=width;
	m_style=style;
	m_bCurve = (nCurve!=0);
	//m_p0=SPoint(x0,y0);
	//m_p1=SPoint(x1,y1);
	return true;

}

bool SPtnObjLine::writeCe3(SWriteCE3& wce3,SPoint* pOrigin)
{
	SPoint p0=m_p0;
	SPoint p1=m_p1;
	if(pOrigin!=NULL){
		p0 -= *pOrigin;
		p1 -= *pOrigin;
	}

	wce3.WriteRecord(_T("+L"));
	wce3.WriteRecordInt(_T("W"),m_width);
	wce3.WriteRecordInt(_T("S"),m_style);
	wce3.WriteRecordInt(_T("CV"),(m_bCurve?1:0));
	wce3.WriteRecordInt(_T("X"),p0.x());
	wce3.WriteRecordInt(_T("Y"),p0.y());
	wce3.WriteRecordInt(_T("X"),p1.x());
	wce3.WriteRecordInt(_T("Y"),p1.y());
	if(m_bCurve){
		p0 = m_pC0;
		p1 = m_pC1;
		if(pOrigin!=NULL){
			p0 -= *pOrigin;
			p1 -= *pOrigin;
		}
		wce3.WriteRecordInt(_T("X"),p0.x());
		wce3.WriteRecordInt(_T("Y"),p0.y());
		wce3.WriteRecordInt(_T("X"),p1.x());
		wce3.WriteRecordInt(_T("Y"),p1.y());
	}
	wce3.WriteRecord(_T("-L"));
	wce3.WriteEOL();
	return true;
}
