/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#include "stdafx.h"
#include "sptnobjcircle.h"
#include <math.h>

SPtnObjCircle::SPtnObjCircle(void)
{
	m_p0 = SPoint(-1,-1);
	m_p1 = SPoint(-1,-1);
	//m_nFill	= -1;
}

SPtnObjCircle::~SPtnObjCircle(void)
{
}

//���������
SPtnObj* SPtnObjCircle::duplicate()
{
	return new SPtnObjCircle(*this);
}


//�L���ȃf�[�^���ǂ�����Ԃ��B
bool SPtnObjCircle::qValid()
{
	if(m_p0 == m_p1) return false;
	else return true;
}


//�ʒu�𓮂���
void SPtnObjCircle::move(int offsetx,int offsety)
{
	m_p0.setX(m_p0.x()+offsetx);
	m_p0.setY(m_p0.y()+offsety);
	m_p1.setX(m_p1.x()+offsetx);
	m_p1.setY(m_p1.y()+offsety);
}


//X���W���w�肵�č��E���]���s��
void SPtnObjCircle::mirrorH(int x)
{
	m_p0 = mirrorPoint(m_p0,x);
	m_p1 = mirrorPoint(m_p1,x);
}

//���E���]���s��
void SPtnObjCircle::mirrorH()
{
	return;
}


//XY���S���W���w�肵�āA��]���s��
void SPtnObjCircle::rotate(const SPoint& p)
{
	m_p0 = rotatePoint(m_p0,p);
	m_p1 = rotatePoint(m_p1,p);
}



//��]���s��
void SPtnObjCircle::rotate()
{
	int x = (m_p0.x()+m_p1.x())/2;
	int y = (m_p0.y()+m_p1.y())/2;
	SPoint p(x,y);
	rotate(p);
}

//�͈͂��w�肵�đI���`�F�b�N���s��
bool SPtnObjCircle::testIntersect(const SRect& rc)
{
	if(!rc.intersect(m_p0)) return false;
	if(!rc.intersect(m_p1)) return false;
	return true;
}


//�_���w�肵�đI���`�F�b�N���s��
bool SPtnObjCircle::testIntersect(const SPoint& p)
{
	int l,t,r,b;
	l=p.x()-2;
	r=p.x()+2;
	t=p.y()-2;
	b=p.y()+2;


	double xc,yc,k,r2,x0,y0,a0,a1;
	xc=((double)(m_p0.x()+m_p1.x()))/2;
	yc=((double)(m_p0.y()+m_p1.y()))/2;
	k=abs((double)(m_p0.x()-m_p1.x())/(double)(m_p0.y()-m_p1.y()));
	r2=(((double)(m_p0.y()-m_p1.y()))/2);
	r2=r2*r2;

	if(m_nFill<0){
		x0=(p.x()-xc)/k;
		y0=t-yc;
		a0=x0*x0+y0*y0; 
		y0=b-yc;
		a1=x0*x0+y0*y0;
		if((a0<r2)&&(r2<a1)) return true;
		if((a0>r2)&&(r2>a1)) return true;

		x0=(l-xc)/k;
		y0=p.y()-yc;
		a0=x0*x0+y0*y0; 
		x0=(r-xc)/k;
		a1=x0*x0+y0*y0;
		if((a0<r2)&&(r2<a1)) return true;
		if((a0>r2)&&(r2>a1)) return true;
	}else{
		x0=(p.x()-xc)/k;
		y0=p.y()-yc;
		a0=x0*x0+y0*y0; 
		if(a0<=r2) return true;
	}
	return false;
}


//��L�͈͂�Ԃ�
SRect SPtnObjCircle::area()
{
	SRect rc(m_p0,m_p1);
	rc.normalize();
	return rc;
}

//�O���b�v�̈ʒu��Ԃ�
SPoint SPtnObjCircle::gripPos(int grip)
{
	if(grip==0) return m_p0;
	else		return m_p1;
}

//�O���b�v�̈ʒu�𓮂���
void SPtnObjCircle::setGripPos(int grip,int x,int y)
{
	if(grip==0) m_p0 = SPoint(x,y);
	else		m_p1 = SPoint(x,y);
}


bool SPtnObjCircle::readCe3(SReadCE3& rce3)
{
	bool f_y1 = false;
	int i;

	int width = 1;
	int style = 0;
	int fill  = -1;
	int x0    = 0;
	int y0    = 0;
	int x1    = 0;
	int y1    = 0;

	wstring str(_T(""));
	while(rce3.ReadRecord(str)!=WEOF){
		if(str[0] == '-'){
			if(str != _T("-C")){
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
				//int nParam = atoi(str.substr(n+1).c_str()); //:����I�[�܂ł𐔒l�ɕϊ�
				if(var==_T("W")){
					width = _tstoi(str.substr(n+1).c_str()); //:����I�[�܂ł𐔒l�ɕϊ�
				}else
				if(var==_T("S")){
					style = _tstoi(str.substr(n+1).c_str()); //:����I�[�܂ł𐔒l�ɕϊ�
				}else
				if(var==_T("F")){
					fill = _tstoi(str.substr(n+1).c_str()); //:����I�[�܂ł𐔒l�ɕϊ�
				}else
				if(var==_T("X")){
					i = _tstoi(str.substr(n+1).c_str()); //:����I�[�܂ł𐔒l�ɕϊ�
					if(!f_y1){
						x0=i;
					}else{
						x1=i;
					}
				}else
				if(var==_T("Y")){
					i = _tstoi(str.substr(n+1).c_str()); //:����I�[�܂ł𐔒l�ɕϊ�
					if(!f_y1){
						y0=i;
						f_y1=true;
					}else{
						y1=i;
					}
				}
			}
		}
	}
	m_width=width;
	m_style=style;
	m_nFill=fill;
	m_p0=SPoint(x0,y0);
	m_p1=SPoint(x1,y1);
	return true;
}

bool SPtnObjCircle::writeCe3(SWriteCE3& wce3,SPoint* pOrigin)
{
	SPoint p0=m_p0;
	SPoint p1=m_p1;

	if(pOrigin!=NULL){
		p0 -= *pOrigin;
		p1 -= *pOrigin;
	}
	wce3.WriteRecord(_T("+C"));
	wce3.WriteRecordInt(_T("W"),m_width);
	wce3.WriteRecordInt(_T("S"),m_style);
	wce3.WriteRecordInt(_T("F"),m_nFill);
	wce3.WriteRecordInt(_T("X"),p0.x());
	wce3.WriteRecordInt(_T("Y"),p0.y());
	wce3.WriteRecordInt(_T("X"),p1.x());
	wce3.WriteRecordInt(_T("Y"),p1.y());
	wce3.WriteRecord(_T("-C"));
	wce3.WriteEOL();
	return true;
}
