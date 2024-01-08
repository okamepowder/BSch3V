/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

/****************************************************************************
** XBSch�}�ʗv�f�ڍ��_�N���X�̎���
****************************************************************************/
#include "stdafx.h"
#include <stdio.h>
//#include <qnamespace.h>
#include <string>
using namespace std;
#include "ce3io.h"
#include "xbschobj.h"
#include "xbschjunc.h"


//�R���X�g���N�^
SXBSchJunc::SXBSchJunc()
{
}


bool SXBSchJunc::readCe3(SReadCE3& rce3,const SPoint* pptOrigin,SXBSchDoc*)
{
	wstring str(_T(""));
	bool bInitX1 = false;
	bool bInitY1 = false;
	while(1){
		if(rce3.ReadRecord(str)==WEOF) return false;
		if(str[0] == '-'){
			if( (str != _T("-JUNCTION")) || !bInitX1 || !bInitY1){
				return false;
			}else{
				break;	//break while(1){ }
			}
		}else if(str[0] == '+'){
			str[0]='-';
			if(rce3.SkipTo(str)==WEOF) return false;
		}else{
			int n;
			int l=str.length();
			for(n=0;n<l;n++){
				if(str[n]==':') break;
			}
			if(0<n && n<(l-1)){	// : �ŕ�����ꂽ���R�[�h�ł���
				wstring var = str.substr(0,n);				//�擪����:�̎�O�܂�
				int nParam = _tstoi(str.substr(n+1).c_str()); //:����I�[�܂ł𐔒l�ɕϊ�

//#ifdef _MFCBSCH	//Qt-BSch doesn't support layer.
				if(var==_T("L")){
					if(isValidLayer(nParam)){
						m_Layer	= nParam;			
					}
				}else
//#endif
				if(var==_T("X")){
					if(pptOrigin) nParam += pptOrigin->x();		
					m_p1.setX(nParam);
					bInitX1 = true;
				}else 
				if(var==_T("Y")){
					if(pptOrigin) nParam += pptOrigin->y();		
					m_p1.setY(nParam);
					bInitY1 = true;
				}
			}
		}
	}

	return true;
}

/*
//�X�g���[������̓ǂݍ��� �`-JUNCTION
bool SXBSchJunc::readStream(SCsvIStream* pStrm,const SPoint* pptOrigin)
{
	if(!pStrm) return false;
	QString str;
	QString var;
	QString val;
	int nParam=0;
	bool bInitX1 = false;
	bool bInitY1 = false;
	while(1){
		int nResult = pStrm->getRecord(str);
		if(nResult == SCsvIStream::GETRECORD_EOF || nResult == SCsvIStream::GETRECORD_ERROR){
			return false;
		}
		//str = deleteLeftSpace(str);
		if(str[0] == '-'){
			if( (str != "-JUNCTION") || !bInitX1 || !bInitY1){
				return false;
			}else{
				break;	//break while(1){ }
			}
		}else if(str[0] == '+'){
			if(pStrm->skipTo(str.replace(1,1,"-")) != SCsvIStream::GETRECORD_NORMAL)return false;
		}else{
			if(SCsvIStream::splitToVariableValue(str,var,val)){
				nParam = val.toInt();
				if(var=="X"){
					if(pptOrigin) nParam += pptOrigin->x();		
					m_p1.setX(nParam);
					bInitX1 = true;
				}else 
				if(var=="Y"){
					if(pptOrigin) nParam += pptOrigin->y();		
					m_p1.setY(nParam);
					bInitY1 = true;
				}
			}
		}
	}

	return true;
}
*/

bool SXBSchJunc::writeCe3(SWriteCE3& wce3,const SPoint* pptOrigin)
{
//	TCHAR sz[32];
	int x = m_p1.x();
	int y = m_p1.y();
	if(pptOrigin){
		x -= pptOrigin->x();
		y -= pptOrigin->y();
	}

	wce3.WriteRecord(_T("+JUNCTION"));
	wce3.WriteRecordInt(_T("L"),m_Layer);
	wce3.WriteRecordInt(_T("X"),x);
	wce3.WriteRecordInt(_T("Y"),y);


	wce3.WriteRecord(_T("-JUNCTION"));
	wce3.WriteEOL();
	return true;
}



//�������s��
SXBSchObj* SXBSchJunc::duplicate()
{
	SXBSchObj* newObj =new SXBSchJunc(*this);
	return newObj;
}

//�_���w�肵�đI���d�v�x��Ԃ�
int SXBSchJunc::qSelectWeight(const SPoint& p)
{
	int nResult = PointAndRectMag(p,area());
	if(nResult) return (nResult | ON_OBJ);
	else		return 0;
}

//�͈͂��w�肵�đI�����s��
unsigned SXBSchJunc::testSelection(const SRect& rc)
{
	if( rc.intersect(area()) ){
		return SELECT_ALL;
	}else{
		return 0;
	}
}

//�͈͂�I�����ăh���b�O�I�����s��
unsigned SXBSchJunc::testSelectionForDrag(const SRect& rc)
{
	if( rc.intersect(m_p1) ){
		return SELECT_ALL;
	}else{
		return 0;
	}
}

//�`�悪�K�v���ǂ�����Ԃ�
bool SXBSchJunc::qRedraw(const SRect& rc)
{
	return rc.intersect(area());
}

//��L�͈͂�Ԃ�
SRect SXBSchJunc::area()
{
	SRect rc(m_p1.x()-2,m_p1.y()-2,5,5);
	return rc;
}

SRect SXBSchJunc::rotateRect()
{
	return SRect(m_p1,m_p1);
}