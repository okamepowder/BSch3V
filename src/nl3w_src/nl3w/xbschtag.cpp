/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

/****************************************************************************
** XBSch�}�ʗv�f�^�O�N���X�̎���
****************************************************************************/
#include "stdafx.h"
#include <stdio.h>
#include <assert.h>
#include <string>
using namespace std;
#include "ce3io.h"
#include "xbschobj.h"
#include "xbschtag.h"


//�R���X�g���N�^
SXBSchTag::SXBSchTag()
{
	m_bHoriz = true;
	m_nType = TAG_SQ;
	m_strText ="";
}

bool SXBSchTag::readCe3(SReadCE3& rce3,const SPoint* pptOrigin,SXBSchDoc*)
{
	string str("");
	bool bInitX1 = false;
	bool bInitY1 = false;
	bool bInitDir = false;
	bool bInitText = false;
	bool bInitType = false;

	while(1){
		if(rce3.ReadRecord(str)==EOF) return false;
		if(str[0] == '-'){
			if( (str != "-TAG") || !bInitX1 || !bInitY1 || !bInitDir || !bInitText || !bInitType){
				return false;
			}else{
				break;	//break while(1){ }
			}
		}else if(str[0] == '+'){
			str[0]='-';
			if(rce3.SkipTo(str)==EOF) return false;
		}else{
			int n;
			int l=str.length();
			for(n=0;n<l;n++){
				if(str[n]==':') break;
			}
			if(0<n && n<l){	// : �ŕ�����ꂽ���R�[�h�ł���
				string var = str.substr(0,n);				//�擪����:�̎�O�܂�
				int nParam = atoi(str.substr(n+1).c_str()); //:����I�[�܂ł𐔒l�ɕϊ�
//#ifdef _MFCBSCH	//Qt-BSch doesn't support layer.
				if(var=="L"){
					if(isValidLayer(nParam)){
						m_Layer	= nParam;			
					}
				}else
//#endif
				if(var=="X"){
					if(pptOrigin) nParam += pptOrigin->x();		
					m_p1.setX(nParam);
					bInitX1 = true;
				}else
				if(var=="Y"){
					if(pptOrigin) nParam += pptOrigin->y();		
					m_p1.setY(nParam);
					bInitY1 = true;
				}else
				if(var=="D"){
					if(nParam==0)	m_bHoriz = false;
					else			m_bHoriz = true;
					bInitDir = true;
				}else
				if(var=="T"){
					if(nParam>0 && nParam<=3)	m_nType = nParam;
					else						m_nType = 0;
					bInitType = true;
				}else
				if(var=="S"){
					m_strText=str.substr(n+1);
					//if(m_strText.length()>0){
					bInitText = true;
					//}
				}
			}
		}
	}
	setp2();
	return true;
}

bool SXBSchTag::writeCe3(SWriteCE3& wce3,const SPoint* pptOrigin)
{
	//char sz[32];

	int x = m_p1.x();
	int y = m_p1.y();
	int d = (m_bHoriz ? 1 : 0);
	//string str("S:");

	if(pptOrigin){
		x -= pptOrigin->x();
		y -= pptOrigin->y();
	}

	wce3.WriteRecord("+TAG");
	wce3.WriteRecordInt("L",m_Layer);
	wce3.WriteRecordInt("X",x);
	wce3.WriteRecordInt("Y",y);
	wce3.WriteRecordInt("D",d);
	wce3.WriteRecordInt("T",m_nType);
	wce3.WriteRecordString("S",m_strText);
	wce3.WriteRecord("-TAG");
	wce3.WriteEOL();
	return true;
}



//�������s��
SXBSchObj* SXBSchTag::duplicate()
{
	SXBSchObj* newObj =new SXBSchTag(*this);
	return newObj;
}

//�������ɑΉ������^�O�̕������߂�
//��������12�ȉ��̏ꍇ��BSch�ƌ݊����̂���e�[�u�����g��
int SXBSchTag::width()
{
	static int narrayTagWidth[]={20,20,30,40,50,60,70,80,80,90,100,110,120};
	unsigned int l=m_strText.length();
	if(l<=12) return narrayTagWidth[l];
	else return ((l*8+29)/10)*10;
}


//�_���w�肵�đI���d�v�x��Ԃ�
int SXBSchTag::qSelectWeight(const SPoint& p)
{
	int nResult = PointAndRectMag(p,area());
	if(nResult) return (nResult | ON_OBJ);
	else		return 0;
}

//�͈͂��w�肵�đI�����s��
unsigned SXBSchTag::testSelection(const SRect& rc)
{
	if( rc.intersect(area()) ){
		return SELECT_ALL;
	}else{
		return 0;
	}
}

//�͈͂�I�����ăh���b�O�I�����s��
unsigned SXBSchTag::testSelectionForDrag(const SRect& rc)
{
	if( rc.intersect(area()) ){
		return SELECT_ALL;
	}else{
		return 0;
	}
}

//�`�悪�K�v���ǂ�����Ԃ�
bool SXBSchTag::qRedraw(const SRect& rc)
{
	return rc.intersect(area());
}

//��L�͈͂�Ԃ�
SRect SXBSchTag::area()
{
	if(m_bHoriz){
		return SRect(m_p1.x(),m_p1.y()-5,m_p2.x()-m_p1.x()+1,11);
	}else{
		return SRect(m_p2.x()-5,m_p2.y(),11,m_p1.y()-m_p2.y()+1);
	}
}


// m_p1�����܂��͉��ɂȂ�悤�ɒ�������
void SXBSchTag::normalize()
{
	int n;
	if(m_bHoriz){
		if(m_p1.x() > m_p2.x()){
			n = m_p1.x();
			m_p1.setX(m_p2.x());
			m_p2.setX(n);
		}
	}else{
		if(m_p1.y() < m_p2.y()){
			n = m_p1.y();
			m_p1.setY(m_p2.y());
			m_p2.setY(n);
		}
	}
}


SRect SXBSchTag::rotateRect()
{
	return SRect(m_p1,m_p2);
}

//XY���S���W���w�肵�āA��]���s��
void SXBSchTag::rotate(const SPoint& p)
{
	if(m_bHoriz){
		m_bHoriz = false;
	}else{
		m_bHoriz = true;
		switch(m_nType){
		case TAG_LU:
			m_nType = TAG_RD;
			break;
		case TAG_RD:
			m_nType = TAG_LU;
			break;
		}
	}
	SXBSchObj::rotate(p);
	normalize();
}

//��]���s��
void SXBSchTag::rotate()
{
	rotate(area().center());
}

//X���W���w�肵�č��E���]���s��
void SXBSchTag::mirrorH(int x)
{
	if(m_bHoriz){
		switch(m_nType){
		case TAG_LU:
			m_nType = TAG_RD;
			break;
		case TAG_RD:
			m_nType = TAG_LU;
			break;
		}
	}
	SXBSchObj::mirrorH(x);
	normalize();
}

SPoint SXBSchTag::pinPosition(int n) const
{
	if(n==0) return m_p1;
	else	 return m_p2;
}

void SXBSchTag::setText(const char* psz)
{
	if(psz == NULL){
		m_strText = "";
	}else{
		m_strText = psz;
	}
	setp2();
}

void SXBSchTag::setp2()
{
	int w = width();
	if(m_bHoriz){
		m_p2.setY(m_p1.y());
		m_p2.setX(m_p1.x()+w);
	}else{
		m_p2.setX(m_p1.x());
		m_p2.setY(m_p1.y()-w);
	}
}