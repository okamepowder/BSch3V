/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#include "stdafx.h"
#include "sptnobjline.h"

SPtnObjLine::SPtnObjLine(void)
{
	m_p0 = SPoint(-1,-1);
	m_p1 = SPoint(-1,-1);
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

//���������
SPtnObj* SPtnObjLine::duplicate()
{
	return new SPtnObjLine(*this);
}

//�L���ȃf�[�^���ǂ�����Ԃ��B
bool SPtnObjLine::qValid()
{
	if(m_p0 == m_p1) return false;
	else return true;
}

//�ʒu�𓮂���
void SPtnObjLine::move(int offsetx,int offsety)
{
	m_p0.setX(m_p0.x()+offsetx);
	m_p0.setY(m_p0.y()+offsety);
	m_p1.setX(m_p1.x()+offsetx);
	m_p1.setY(m_p1.y()+offsety);
}

//X���W���w�肵�č��E���]���s��
void SPtnObjLine::mirrorH(int x)
{
	m_p0 = mirrorPoint(m_p0,x);
	m_p1 = mirrorPoint(m_p1,x);
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
}

//��]���s��
void SPtnObjLine::rotate()
{
	int x = (m_p0.x()+m_p1.x())/2;
	int y = (m_p0.y()+m_p1.y())/2;
	SPoint p(x,y);
	rotate(p);
}


//�͈͂��w�肵�đI���`�F�b�N���s��
bool SPtnObjLine::testIntersect(const SRect& rc)
{
	if(!rc.intersect(m_p0)) return false;
	if(!rc.intersect(m_p1)) return false;
	return true;
}

//�_���w�肵�đI���`�F�b�N���s��
bool SPtnObjLine::testIntersect(const SPoint& p)
{
	return this->testIntersectLinePoint(m_p0,m_p1,p);
}

//��L�͈͂�Ԃ�
SRect SPtnObjLine::area()
{
	SRect rc(m_p0,m_p1);
	rc.normalize();
	return rc;
}

//�O���b�v�̈ʒu��Ԃ�
SPoint SPtnObjLine::gripPos(int grip)
{	
	if(grip==0){
		return m_p0;
	}else{
		return m_p1;
	}
}

//�O���b�v�̈ʒu�𓮂���
void SPtnObjLine::setGripPos(int grip,int x,int y)
{
	if(grip==0){
		m_p0=SPoint(x,y);
	}else{
		m_p1=SPoint(x,y);
	}
}


bool SPtnObjLine::readCe3(SReadCE3& rce3)
{
	bool f_y1 = false;
	int i;
	int width = 1;
	int style = 0;
	int x0    = 0;
	int y0    = 0;
	int x1    = 0;
	int y1    = 0;

	string str("");
	while(rce3.ReadRecord(str)!=EOF){
		if(str[0] == '-'){
			if(str != "-L"){
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
					i = atoi(str.substr(n+1).c_str()); //:����I�[�܂ł𐔒l�ɕϊ�
					if(!f_y1){
						x0=i;
					}else{
						x1=i;
					}
				}else
				if(var=="Y"){
					i = atoi(str.substr(n+1).c_str()); //:����I�[�܂ł𐔒l�ɕϊ�
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
	m_p0=SPoint(x0,y0);
	m_p1=SPoint(x1,y1);
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
	wce3.WriteRecord("+L");
	wce3.WriteRecordInt("W",m_width);
	wce3.WriteRecordInt("S",m_style);
	wce3.WriteRecordInt("X",p0.x());
	wce3.WriteRecordInt("Y",p0.y());
	wce3.WriteRecordInt("X",p1.x());
	wce3.WriteRecordInt("Y",p1.y());
	wce3.WriteRecord("-L");
	wce3.WriteEOL();
	return true;
}
