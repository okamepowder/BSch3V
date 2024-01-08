/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#include "stdafx.h"
#include ".\sptnobjarc.h"
#include <math.h>

#define M_PI       3.14159265358979323846

SPtnObjArc::SPtnObjArc(void)
{
	m_ptC = SPoint(10,10);		//���S���W
	m_nR  = 10;					//���a
	m_nBegin = 0;				//�J�n�p
	m_nEnd	= 360*16;			//�I���p
	m_ptBegin = AngleToPoint500(m_nBegin);
	m_ptEnd   = AngleToPoint500(m_nEnd);
}

SPtnObjArc::~SPtnObjArc(void)
{
}

//���������
SPtnObj* SPtnObjArc::duplicate()
{
	return (SPtnObj*)new SPtnObjArc(*this);
}

//�L���ȃf�[�^���ǂ�����Ԃ��B
bool SPtnObjArc::qValid()
{
	if(m_nR<=0) return false;
	else return true;
}

//�ʒu�𓮂���
void SPtnObjArc::move(int offsetx,int offsety)
{
	m_ptC.setX(m_ptC.x()+offsetx);
	m_ptC.setY(m_ptC.y()+offsety);
}


//X���W���w�肵�č��E���]���s��
void SPtnObjArc::mirrorH(int x)
{
	normalizeAngle();
	m_ptC = mirrorPoint(m_ptC,x);
	int b,e;
	if(m_nBegin<=180*16){
		e=180*16 - m_nBegin;
	}else{
		e=(360+180)*16 - m_nBegin;
	}
	if(m_nEnd<=180*16){
		b=180*16 - m_nEnd;
	}else{
		b=(360+180)*16 - m_nEnd;
	}
	m_nBegin = b;
	m_nEnd = e;
	m_ptBegin = AngleToPoint500(m_nBegin);
	m_ptEnd   = AngleToPoint500(m_nEnd);
}

//���E���]���s��
void SPtnObjArc::mirrorH()
{
	SRect rc = area();
	int x = (rc.l()+rc.r())/2;
	mirrorH(x);
}

//XY���S���W���w�肵�āA��]���s��
void SPtnObjArc::rotate(const SPoint& p)
{
	m_ptC = rotatePoint(m_ptC,p);
	m_nBegin += 270*16;
	m_nEnd += 270*16;
	normalizeAngle();
	m_ptBegin = AngleToPoint500(m_nBegin);
	m_ptEnd   = AngleToPoint500(m_nEnd);
}


//��]���s��
void SPtnObjArc::rotate()
{
	SRect rc = area();
	SPoint p = rc.center();
	rotate(p);
}

//�͈͂��w�肵�đI���`�F�b�N���s��
bool SPtnObjArc::testIntersect(const SRect& rc)
{
	SRect rc1 = area();
	if(!rc.intersect( SPoint(rc1.l(),rc1.t()) ) ) return false;
	if(!rc.intersect( SPoint(rc1.r(),rc1.b()) ) ) return false;
	return true;
}

//�_���w�肵�đI���`�F�b�N���s��
bool SPtnObjArc::testIntersect(const SPoint& ptest)
{
	double dx = ptest.x() - m_ptC.x();
	double dy = ptest.y() - m_ptC.y();
	double r = sqrt(dx*dx + dy*dy);
	double e = abs(r-m_nR);
	if(e>1) return false;

	double rad = PointRad(m_ptC.x(),m_ptC.y(),r,ptest.x(),ptest.y());
	int deg = (180.0*16*rad)/M_PI+0.5;
	int nEnd;
	normalizeAngle();
	if(m_nBegin>m_nEnd){
		nEnd = m_nEnd+360*16;
		deg += 360*16;
	}else{
		nEnd = m_nEnd;
	}
	if(m_nBegin<=deg && deg<=m_nEnd) return true;
	else return false;
}
	
//��L�͈͂�Ԃ�
SRect SPtnObjArc::area()
{
	int l,t,r,b;
	normalizeAngle();
	if(m_nBegin==m_nEnd){
		l=m_ptC.x()-m_nR;
		t=m_ptC.y()-m_nR;
		r=m_ptC.x()+m_nR;
		b=m_ptC.y()+m_nR;
	}else{
		int xb,yb,xe,ye,nEnd,n,m;
		SPoint pt;
		pt = AngleToPoint(m_nBegin);
		xb = pt.x();
		yb = pt.y();
		pt = AngleToPoint(m_nEnd);
		xe = pt.x();
		ye = pt.y();

		//double angle;
		//angle = m_nBegin*M_PI/(180*16); 
		//xb =m_ptC.x() + m_nR*cos(angle);
		//yb =m_ptC.y() - m_nR*sin(angle);
		//angle = m_nEnd*M_PI/(180*16); 
		//xe =m_ptC.x() + m_nR*cos(angle);
		//ye =m_ptC.y() - m_nR*sin(angle);

		l=(xb<xe ? xb : xe);
		r=(xb>xe ? xb : xe);
		t=(yb<ye ? yb : ye);
		b=(yb>ye ? yb : ye);

		
		//�n�_�A�I�_��0�x���܂����ł��邩�ǂ���
		if(m_nBegin>m_nEnd){	//�܂����ł���ꍇ
			r=m_ptC.x()+m_nR;
			//�ȍ~�̔���̂��߂̏���
			nEnd = m_nEnd+360*16;
		}else{
			//t=(xb>xe ? xb : xe);
			//�ȍ~�̔���̂��߂̏���
			nEnd = m_nEnd;
		}

		n=90*16;
		m=(90+360)*16;
		//�n�_�A�I�_��90�x���܂����ł��邩�ǂ���
		if((m_nBegin<=n && nEnd>=n)||(m_nBegin<=m && nEnd>=m)) {	//�܂����ł���ꍇ
			t=m_ptC.y()-m_nR;
		}

		//�n�_�A�I�_��180�x���܂����ł��邩�ǂ���
		n=180*16;
		m=(180+360)*16;
		if((m_nBegin<=n && nEnd>=n)||(m_nBegin<=m && nEnd>=m)) {	//�܂����ł���ꍇ
			l=m_ptC.x()-m_nR;
		}

		//�n�_�A�I�_��270�x���܂����ł��邩�ǂ���
		n=270*16;
		m=(270+360)*16;
		if((m_nBegin<=n && nEnd>=n)||(m_nBegin<=m && nEnd>=m)) {	//�܂����ł���ꍇ
			b=m_ptC.y()+m_nR;
		}
	}
	return SRect(l,t,r-l,b-t);
}


void SPtnObjArc::normalizeAngle()
{
	m_nBegin = m_nBegin %5760;
	m_nEnd	 = m_nEnd   %5760;
}

double SPtnObjArc::PointRad(double cx,double cy,double r,double x0,double y0)
{
	double x = (x0-cx)/r;
	double y = -(y0-cy);
	double rad = acos(x);
	if(y<0) rad = 2*M_PI-rad;
	return rad;
}

//�J�n�p�����W�Őݒ�B��ɒ��S�����߂Ă�������
void SPtnObjArc::setBeginPoint(int x,int y)
{
	double dx = x - m_ptC.x();
	double dy = y - m_ptC.y();
	double r = sqrt(dx*dx + dy*dy);

	double rad = PointRad(m_ptC.x(),m_ptC.y(),r,x,y);
	m_nBegin = (180.0*16*rad)/M_PI+0.5;
	m_ptBegin = AngleToPoint500(m_nBegin);
}

//�I���p�����W�Őݒ�B��ɒ��S�����߂Ă�������
void SPtnObjArc::setEndPoint(int x,int y)
{
	double dx = x - m_ptC.x();
	double dy = y - m_ptC.y();
	double r = sqrt(dx*dx + dy*dy);

	double rad = PointRad(m_ptC.x(),m_ptC.y(),r,x,y);
	m_nEnd = (180.0*16*rad)/M_PI+0.5;
	m_ptEnd   = AngleToPoint500(m_nEnd);
}

//�p�x(deg*16)����~����̓_�����߂�
SPoint SPtnObjArc::AngleToPoint(int nAngle)
{
	int x,y;
	double angle;
	angle = nAngle*M_PI/(180*16); 
	x =m_ptC.x() + ((double)m_nR*cos(angle));
	y =m_ptC.y() - ((double)m_nR*sin(angle));
	return SPoint(x,y);
}

SPoint SPtnObjArc::AngleToPoint500(int nAngle)
{
	int x,y;
	double angle;
	angle = nAngle*M_PI/(180*16); 
	x =m_ptC.x() + (500.0*cos(angle));
	y =m_ptC.y() - (500.0*sin(angle));
	return SPoint(x,y);
}


bool SPtnObjArc::readCe3(SReadCE3& rce3)
{
	int width = 1;
	int style = 0;
	int cx = 10;
	int cy = 10;
	int r  = 10;
	int beginDeg = 0;
	int endDeg = 0;

	string str("");
	while(rce3.ReadRecord(str)!=EOF){
		if(str[0] == '-'){
			if(str != "-AR"){
				return false;
			}else{
				break;
			}
		}else if(str[0] == '+'){
			str[0]='-';
			if(rce3.SkipTo(str) == EOF) return false;
		}else{
			int n;
			int l=str.length();
			for(n=0;n<l;n++){
				if(str[n]==':') break;
			}
			if(0<n && n<(l-1)){	// : �ŕ�����ꂽ���R�[�h�ł���
				string var = str.substr(0,n);				//�擪����:�̎�O�܂�
				//int nParam = atoi(str.substr(n+1).c_str()); //:����I�[�܂ł𐔒l�ɕϊ�
				if(var=="W"){
					width = atoi(str.substr(n+1).c_str()); //:����I�[�܂ł𐔒l�ɕϊ�
				}else
				if(var=="S"){
					style = atoi(str.substr(n+1).c_str()); //:����I�[�܂ł𐔒l�ɕϊ�
				}else
				if(var=="X"){
					cx = atoi(str.substr(n+1).c_str()); //:����I�[�܂ł𐔒l�ɕϊ�
				}else
				if(var=="Y"){
					cy = atoi(str.substr(n+1).c_str()); //:����I�[�܂ł𐔒l�ɕϊ�
				}else
				if(var=="R"){
					r = atoi(str.substr(n+1).c_str()); //:����I�[�܂ł𐔒l�ɕϊ�
				}else
				if(var=="B"){
					beginDeg = atoi(str.substr(n+1).c_str()); //:����I�[�܂ł𐔒l�ɕϊ�
				}else
				if(var=="E"){
					endDeg = atoi(str.substr(n+1).c_str()); //:����I�[�܂ł𐔒l�ɕϊ�
				}
			}
		}
	}

	this->m_width	= width;
	this->m_style	= style;
	this->m_ptC		= SPoint(cx,cy);
	this->m_nR		= r;
	this->m_nBegin	= beginDeg;
	this->m_nEnd	= endDeg;

	m_ptBegin = AngleToPoint500(beginDeg);
	m_ptEnd   = AngleToPoint500(endDeg);

	return true;
}


bool SPtnObjArc::writeCe3(SWriteCE3& wce3,SPoint* pOrigin)
{
	SPoint center=this->m_ptC;
	if(pOrigin!=NULL){
		center -= *pOrigin;
	}
	wce3.WriteRecord("+AR");
	wce3.WriteRecordInt("W",this->m_width);
	wce3.WriteRecordInt("S",this->m_style);
	wce3.WriteRecordInt("X",center.x());
	wce3.WriteRecordInt("Y",center.y());
	wce3.WriteRecordInt("R",this->m_nR);
	wce3.WriteRecordInt("B",this->m_nBegin);
	wce3.WriteRecordInt("E",this->m_nEnd);
	wce3.WriteRecord("-AR");
	wce3.WriteEOL();
	return true;
}

//�O���b�v�̈ʒu��Ԃ�
SPoint SPtnObjArc::gripPos(int grip)
{
	switch(grip){
		case 0:
			return AngleToPoint(m_nBegin);
		case 1:
			return AngleToPoint(m_nEnd);
		case 2:
			return m_ptC;
		case 3:	//0�x
			return SPoint(m_ptC.x()+m_nR,m_ptC.y()     );
		case 4:	//90�x
			return SPoint(m_ptC.x()     ,m_ptC.y()-m_nR);
		case 5:	//180�x
			return SPoint(m_ptC.x()-m_nR,m_ptC.y()     );
		default:	//case 6:	//270�x
			return SPoint(m_ptC.x()     ,m_ptC.y()+m_nR);
	}
}

//�O���b�v�̈ʒu�𓮂���
void SPtnObjArc::setGripPos(int grip,int x,int y)
{
	switch(grip){
		case 0:
			setBeginPoint(x,y);
			break;
		case 1:
			setEndPoint(x,y);
			break;
		case 2:
			m_ptC = SPoint(x,y);
			break;
		case 3:	//0�x
		case 5:	//180�x
			m_nR =  abs(m_ptC.x()-x);
			break;
		case 4:	//90�x
		default:	//case 6:	//270�x
			m_nR =  abs(m_ptC.y()-y);
			break;
	}
}



