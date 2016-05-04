/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#include "stdafx.h"
#include "sptnobjpolygon.h"

SPtnObjPolygon::SPtnObjPolygon(void)
{
	m_nCount = 0;			//���_�̐�
	m_pPt = NULL;			//���_���W
	//m_nFill = -1;
}

SPtnObjPolygon::~SPtnObjPolygon(void)
{
	delete[]m_pPt;
}

//���K�� �אڂ������_���������W�̂Ƃ��ɒ��_���폜����
void SPtnObjPolygon::normalize()
{
	int i,j,n,m;
	m=m_nCount-1;
	n=0;
	//�אڂ������_�̃X�L����
	for(i=0;i<m;i++){
		if(m_pPt[i]==m_pPt[i+1]) n++;
	}
	if(m_pPt[m]==m_pPt[0]) n++;
	if(n==0) return;

	//�V�����|���S���̒��_�̊m��
	int newCount = m_nCount - n;
	if(newCount<=1){
		m_nCount = 0;
		delete[]m_pPt;
		m_pPt = NULL;
		return;
	}
	SPoint* pPt = new SPoint[newCount];

	//���_�̃R�s�[
	j=0;
	for(i=0;i<m;i++){
		if(m_pPt[i]!=m_pPt[i+1]){
			pPt[j++] = m_pPt[i];
		}
	}
	if(m_pPt[m]!=m_pPt[0]) pPt[j] = m_pPt[m];

	m_nCount = newCount;
	delete[]m_pPt;
	m_pPt = pPt;
}

//��L�͈͂�Ԃ�
SRect SPtnObjPolygon::area()
{

	int left = INT_MAX;
	int top	= INT_MAX;
	int right = INT_MAX;
	int bottom = INT_MAX;
	if(m_nCount>0){
		left = right = m_pPt[0].x();
		top = bottom = m_pPt[0].y();
		int i;
		for(i=1;i<m_nCount;i++){
			int x = m_pPt[i].x();
			int y = m_pPt[i].y();
            if(left>x)		left = x;
			if(right<x)		right =x;
			if(top>y)		top	= y;
			if(bottom<y)	bottom = y;
		}
	}
	return SRect(left,top,(right-left),(bottom-top));
}


//�R�s�[�R���X�g���N�^
SPtnObjPolygon::SPtnObjPolygon(const SPtnObjPolygon& obj)
{
	m_pPt = NULL;
	*this = obj;
}

//������Z�q
SPtnObjPolygon& SPtnObjPolygon::operator=(const SPtnObjPolygon& obj)
{
	if(this != &obj){
		(SPtnObj&)*this = obj;	//��{�N���X�̃����o�[�̃R�s�[
		m_nFill = obj.m_nFill;
		delete[]m_pPt;
		m_nCount = obj.m_nCount;
		if(m_nCount>0){
			m_pPt = new SPoint[m_nCount];
			int i;
			for(i=0;i<m_nCount;i++){
				m_pPt[i] = obj.m_pPt[i];
			}
		}else{
			m_pPt = NULL;
		}
	}
	return *this;
}

//���������
SPtnObj* SPtnObjPolygon::duplicate()
{
	return new SPtnObjPolygon(*this);
}

//�O���b�v�̈ʒu��Ԃ�
SPoint SPtnObjPolygon::gripPos(int grip)
{
	if(m_nCount==0 || m_pPt==NULL) return SPoint(0,0);
	if(grip<0) grip = 0;
	else if(grip>=m_nCount) grip = m_nCount;
	return m_pPt[grip];
}

//�O���b�v�̈ʒu�𓮂���
void SPtnObjPolygon::setGripPos(int grip,int x,int y)
{
	if(m_nCount==0 || m_pPt==NULL) return;
	if(grip<0) grip = 0;
	else if(grip>=m_nCount) grip = m_nCount;
	m_pPt[grip] = SPoint(x,y);
}

//�L���ȃf�[�^���ǂ�����Ԃ��B
bool SPtnObjPolygon::qValid()
{
	normalize();
	if(m_nCount>0) return true;
	else return false;
}

//�ʒu�𓮂���
void SPtnObjPolygon::move(int offsetx,int offsety)
{
	SPoint ptOffset(offsetx,offsety);
	int i;
	for(i=0;i<m_nCount;i++){
		m_pPt[i]+=ptOffset;
	}
}

//X���W���w�肵�č��E���]���s��
void SPtnObjPolygon::mirrorH(int x)
{
	int i;
	for(i=0;i<m_nCount;i++){
		m_pPt[i] = mirrorPoint(m_pPt[i],x);
	}
}

//���E���]���s��
void SPtnObjPolygon::mirrorH()
{
	SRect rc = area();
	int x = rc.center().x();
	mirrorH(x);
}

//XY���S���W���w�肵�āA��]���s��
void SPtnObjPolygon::rotate(const SPoint& p)
{
	int i;
	for(i=0;i<m_nCount;i++){
		m_pPt[i] = rotatePoint(m_pPt[i],p);
	}
}

//��]���s��
void SPtnObjPolygon::rotate()
{
	SRect rc = area();
	SPoint p = rc.center();
	rotate(p);
}

//�͈͂��w�肵�đI���`�F�b�N���s��
bool SPtnObjPolygon::testIntersect(const SRect& rc)
{
	SRect rc1 = area();
	if(!rc.intersect( SPoint(rc1.l(),rc1.t()) ) ) return false;
	if(!rc.intersect( SPoint(rc1.r(),rc1.b()) ) ) return false;
	return true;
}

//�_���w�肵�đI���`�F�b�N���s��
//2�_�̂Ƃ��͐����Ɩʐς��������_�Ƃ̌����Ŕ���
//3�_�ȏ�̂Ƃ��́A(p.x(),INT_MIN)(p.x(),p.y()) �̐������p�`�̐����Ɗ��������邩�ǂ����Ŕ���
bool SPtnObjPolygon::testIntersect(const SPoint& p)
{
	int x,y,x0,y0,x1,y1;
	normalize();
	if(m_nCount<2) return false;
	if(m_nCount==2){
		return testIntersectLinePoint(m_pPt[0],m_pPt[1],p);
	}

	SRect rc=area();
	if(!rc.intersect(p)) return false;

	int i,j;
	unsigned acrossFlag=0;
	x=p.x();
	y=p.y();
	for(i=0;i<m_nCount;i++){
		j=i+1;
		if(j==m_nCount)j=0;
		if(testIntersectLinePoint(m_pPt[i],m_pPt[j],p))return true;	//�_��������ɂ����true
		if(m_nFill<0)continue;
		x0 = m_pPt[i].x();
		y0 = m_pPt[i].y();
		x1 = m_pPt[j].x();
		y1 = m_pPt[j].y();
		if(x0==x1) continue;							//�����̐��͔���ɉe�����Ȃ�
		if((x0<x && x1<x)||(x0>x && x1>x)) continue;	//x��ʂ鐂�������A�܂����Ȃ����͔̂���ɉe�����Ȃ��B
		if(y0>y && y1>y) continue;						//���[�_��p�_�������ɂ�����͔̂���ɉe�����Ȃ��B
		if(y0<y && y1<y){								//���[�_��p�_������ɂ�����͔̂��萂���ƌ�������
				acrossFlag ^=3;
				continue;
		}
		if(x==x0 && y>y0){			//p0�_�����萂����ɂ���ꍇ
			if(x1>x){
				acrossFlag ^=1;
			}else{
				acrossFlag ^=2;
			}
			continue;
		}
		if(x==x1 && y>y1){			//p1�_�����萂����ɂ���ꍇ
			if(x0>x){
				acrossFlag ^=1;
			}else{
				acrossFlag ^=2;
			}
			continue;
		}
		if(((y1-y0)*(x-x0))/(x1-x0)+y0 < y) acrossFlag ^=3;
	}
	if(acrossFlag == 3) return true;
	else return false;
}
			

bool SPtnObjPolygon::readCe3(SReadCE3& rce3)
{
	SPoint ptbuff[VECTPLOYGON_MAXNODE];

	delete[]m_pPt;
	m_pPt = NULL;
	m_nCount=0;

	int width = 1;
	int style = 0;
	int fill = -1;
	int x,y,prevX,prevY;
	int sety = 0;

	int node,realnode;

	realnode=0;

	node = 0;
	x=0;
	y=0;
	prevX=INT_MIN;	//�K���ȏ����l
	prevY=INT_MIN;	//�K���ȏ����l
	wstring str(_T(""));

	while(rce3.ReadRecord(str)!=WEOF){
		if(str[0] == '-'){
			if(str != _T("-PG")){
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
				if(var==_T("W")){
					width = _tstoi(str.substr(n+1).c_str()); //:����I�[�܂ł𐔒l�ɕϊ�
				}else
				if(var==_T("S")){
					style = _tstoi(str.substr(n+1).c_str()); //:����I�[�܂ł𐔒l�ɕϊ�
				}else
				if(var==_T("F")){
					fill = _tstoi(str.substr(n+1).c_str()); //:����I�[�܂ł𐔒l�ɕϊ�
				}else
				if(var==_T("N")){
					node = _tstoi(str.substr(n+1).c_str()); //:����I�[�܂ł𐔒l�ɕϊ�
					if(node<2 || node>VECTPLOYGON_MAXNODE) goto PARAMERR;
				}
				if(var==_T("X")){
					if(node==0) goto PARAMERR;
					if(sety<node){
						x = _tstoi(str.substr(n+1).c_str()); //:����I�[�܂ł𐔒l�ɕϊ�
					}
				}else
				if(var==_T("Y")){
					if(node==0) goto PARAMERR;
					if(sety<node){
						y = _tstoi(str.substr(n+1).c_str()); //:����I�[�܂ł𐔒l�ɕϊ�
						if(sety==0|| x!=prevX || y!=prevY){
							ptbuff[realnode].setX(x);
							ptbuff[realnode].setY(y);
							prevX=x;
							prevY=y;
							realnode++;
							if(sety==(node-1)){
								if(x==ptbuff[0].x() && y==ptbuff[0].y()) realnode--;
							}
						}
						sety++;
					}
				}
			}
		}
	}
	

	if(realnode<2){
		return false;
	}else{
		m_width=width;
		m_style=style;
		m_nFill=fill;
		m_nCount=realnode;
		m_pPt=new SPoint[realnode];
		for(int i=0;i<realnode;i++){
			m_pPt[i] = ptbuff[i];
		}
		return true;
	}
PARAMERR:
	rce3.SkipTo(_T("-PG"));
	return false;

}

bool SPtnObjPolygon::writeCe3(SWriteCE3& wce3,SPoint* pOrigin)
{
	int i;
	if(m_nCount<2 || m_nCount>VECTPLOYGON_MAXNODE)return false;
	wce3.WriteRecord(_T("+PG"));
	wce3.WriteRecordInt(_T("W"),m_width);
	wce3.WriteRecordInt(_T("S"),m_style);
	wce3.WriteRecordInt(_T("F"),m_nFill);
	wce3.WriteRecordInt(_T("N"),m_nCount);
	for(i=0;i<m_nCount;i++){
		if(pOrigin!=NULL){
			wce3.WriteRecordInt(_T("X"),m_pPt[i].x() - pOrigin->x());
			wce3.WriteRecordInt(_T("Y"),m_pPt[i].y() - pOrigin->y());
		}else{
			wce3.WriteRecordInt(_T("X"),m_pPt[i].x());
			wce3.WriteRecordInt(_T("Y"),m_pPt[i].y());
		}
	}
	wce3.WriteRecord(_T("-PG"));
	wce3.WriteEOL();
	return true;
}






