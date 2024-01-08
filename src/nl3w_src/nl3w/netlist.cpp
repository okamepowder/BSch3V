/****************************************************************************
    NL3W netlist utility for BSch3V
    Copyright (C) 2004-2014 H.Okada
*****************************************************************************/

// BSch�p��NList����ڐA
// BSch�p NList�Ƃ̑���_
// �E���x���A�^�O�̕�����̑O��̋󔒂͖�������
// �E���i�͔ԍ�������Ηǂ��Ƃ���B
#include "stdafx.h"
#ifdef _MSVC
	
	#pragma warning (disable: 4786)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef _WINDOWS
	#include <direct.h>
#else
	#include <unistd.h>
#endif
	
#include <ctype.h>
#include <string>
#include <algorithm>
#include <list>
using namespace std;


#include "xbschglobal.h"

#include "netlist.h"
#include "SAddInFiles.h"

SSingleNetList::~SSingleNetList()
{
	int n;
	for(n=0;n<m_nPinCount;n++){
		delete m_pPinArray[n];
	}
	delete[]m_pPinArray;
}

string SSingleNetList::SignalName()
{
	if(m_SignalNameList.size() == 0) return string("");
	string str = "NOCONNECTION";
	if(m_SignalNameList.end() != find(m_SignalNameList.begin(),m_SignalNameList.end(),str)){
		return str;
	}else{ 
		str = m_SignalNameList.front();
		return str;
	}
}

//SXBSchObj�̃|�C���^���X�g�̉��
void SNetList::clearXBSchObjList(XBSchObjList& listObj)
{
	XBSchObjListIterator ite;
	ite = listObj.begin();
	while(ite != listObj.end()){
		delete(*ite);
		ite++;
	}
	listObj.clear();
}

void SNetList::clearPlistElem()
{
	if(m_pArrayPlistElem==NULL) return;
	int n=m_listComponent.size();
	for(int i=0;i<n;i++){
		delete m_pArrayPlistElem[i];
	}
	delete[]m_pArrayPlistElem;
}


//SNlPin�̃|�C���^���X�g�̉��
void SNetList::clearNlPin(NlPinList& listPin)
{
	NlPinListIterator ite;
	ite = listPin.begin();
	while(ite != listPin.end()){
		delete(*ite);
		ite++;
	}
	listPin.clear();
}

void SNetList::clearNetList(NetList& netlist)
{
	NetListIterator ite = netlist.begin();
	while(ite!=netlist.end()){
		delete (*ite);
		ite++;
	}
	netlist.clear();
}

SNetList::SNetList()
{
	m_xOffset=0;
	m_pArrayPlistElem=NULL;
}


SNetList::~SNetList()
{
	clear();
}

void SNetList::clear()
{
	clearPlistElem();
	clearXBSchObjList(m_listComponent);
	m_listNcTagPower.clear();	//���̃��X�g���Ǘ����Ă�����̂�m_listTagPower��delete����B
	clearXBSchObjList(m_listTagPower);
	clearXBSchObjList(m_listWire);
	clearXBSchObjList(m_listJunc);
	clearXBSchObjList(m_listLabel);
	clearNlPin(m_listNlPin);
	clearNlPin(m_listNlNcPin);
	clearNlPin(m_listNoNumPin);
	clearNetList(m_listNet);
	clearNetList(m_listNcNet);	//2012/01/18	Ver.0.69.03
	m_listNlConfPin.clear();
	m_xOffset=0;
}

//������̗��[�̃z���C�g�X�y�[�X�̍폜
//����R�[�h���폜����悤�ɕύX 2010/11/07 Version 0.69.01
string SNetList::removeWS(const string& str)
{
	int b=0;
	int e=str.length();
	if(e==0)return str;
	e--;
	while(str[b]>0 && str[b]<=0x20) b++;
	while(b<e && (str[e]>0 && str[e]<=0x20)) e--;
	//while(str[b]==' '||str[b]=='\t') b++;
	//while(b<e && (str[e]==' '||str[e]=='\t')) e--;
	return str.substr(b,e-b+1);
}

//�����܂��͐����̐��̌������e�X�g����B
//�������Ă���ꍇ��d�Ȃ��Ă���ꍇ�͌�������2�{�̗��[��Ԃ�
bool SNetList::testLineJoint(int& al,int& ar,int bl,int br)
{
	int n;

	// L<R ��ۏ؂���B
	// BSch�̃f�[�^�ł͂��̂悤�ɂȂ��Ă���͂��Ȃ̂ŏ璷��������Ȃ�
	if(al > ar){
		n=al;
		al = ar;
		ar = n;
	}
	if(bl > br){
		n=bl;
		bl = br;
		br = n;
	}
	if(ar < bl) return false;//  al----ar  [����]   bl----br
	if(br < al) return false;//  bl----br  [����]   al----ar
	if(al > bl) al=bl;
	if(ar < br) ar=br;
	return true;
}

//���C���[�̘A������
//�����܂��͐����̃��C���[�݂̂��Ώ�
void SNetList::jointWire()
{
	//������
	XBSchObjListIterator ite;
	SXBSchWireNl* pObj;
	SPoint pa1;
	SPoint pa2;

	//�����Ώ�
	XBSchObjListIterator ite2;
	XBSchObjListIterator iteOld;	//�I�u�W�F�N�g�̏����p
	SXBSchWireNl* pObj2;
	SPoint pb1;
	SPoint pb2;
	
	bool fH;	//�����t���O
	
	bool fChange=true;

	while(fChange){
		//qDebug("s_JointWire() LOOP");
		fChange=false;
		ite=m_listWire.begin();
		while(ite != m_listWire.end()){
			int lt,rb;
			bool fJoint=false;
			pObj=(SXBSchWireNl*)(*ite);
			pa1=pObj->p1();
			pa2=pObj->p2();
			if(pa1.y()==pa2.y()){		//�����Ȃ�
				fH = true;
				lt = pa1.x();
				rb = pa2.x();
			}else{
				if(pa1.x()!=pa2.x()){	//�����ł��Ȃ����
					ite++;				//���X�g�������ЂƂi�߂�
					continue;			
				}
				fH = false;
				lt = pa1.y();
				rb = pa2.y();
			}
			//�A���ł�����̂���������
			ite2=ite;
			ite2++;						//�C�e���[�^����̎��̂��̂��猟������B
			while(ite2 != m_listWire.end()){
				iteOld = ite2;
				ite2++;
				pObj2=(SXBSchWireNl*)(*iteOld);
				pb1=pObj2->p1();
				pb2=pObj2->p2();
				if(fH){
					if(pa1.y()!=pb1.y() || pb1.y()!=pb2.y()) continue;
					if(testLineJoint(lt,rb,pb1.x(),pb2.x())){
						fJoint=true;
						delete pObj2;
						ite2 = m_listWire.erase(iteOld);
					}
				}else{
					if(pa1.x()!=pb1.x() || pb1.x()!=pb2.x()) continue;
					if(testLineJoint(lt,rb,pb1.y(),pb2.y())){
						fJoint=true;
						delete pObj2;
						ite2 = m_listWire.erase(iteOld);
					}
				}
			}
			if(fJoint){
				if(fH){		
					pa1.setX(lt);	pa2.setX(rb);
				}else{
					pa1.setY(lt);	pa2.setY(rb);
				}
				pObj->setP1(pa1);
				pObj->setP2(pa2);
				fChange=true;
			}
			ite++;
		}
	}
}


//���C���[��ڍ��_�ŕ�������
//�����܂��͐����̃��C���[�݂̂��Ώ�
void SNetList::divideWire()
{
	XBSchObjListIterator iteJ;
	SXBSchJunc* pJ;
	XBSchObjListIterator iteW;
	SXBSchWireNl* pW;
	SXBSchWireNl* pWNew;
	SPoint point;
	
	iteJ = m_listJunc.begin();
	while(iteJ != m_listJunc.end()){
		pJ = (SXBSchJunc*)(*iteJ);
		point = pJ->p1();
		iteW = m_listWire.begin();
		while(iteW != m_listWire.end()){
			//int wcount = m_listWire.size();
			//printf("%d\n",wcount);
			pW = (SXBSchWireNl*)(*iteW);
			pWNew = pW->divide(point);
			if(pWNew){
				m_listWire.push_back(pWNew);
			}
			iteW++;
		}
		iteJ++;
	}
}



//�^�O�Ɠd�����i�����ɐ��ƃs���ɖ��O������
void SNetList::nameFromTagPower()
{
	XBSchObjListIterator iteTP;
	SXBSchComponent* pPC;
	SXBSchTag* pTag;

	XBSchObjListIterator iteW;
	SXBSchWireNl* pW;
	NlPinListIterator itePin;
	SNlPin* pPin;

	SPoint p1;
	SPoint p2;
	SPoint point;

	string signalName;

	int nPinCount;
	int i;

	iteTP = m_listTagPower.begin();
	while(iteTP != m_listTagPower.end()){
		if((*iteTP)->id() == SXBSchObj::ID_COMPONENT){
			pPC = (SXBSchComponent*)(*iteTP);
			signalName = pPC->name();			//�M�����Ɏg�����߂ɕ��i���𓾂�
			if(signalName.length() == 0){		//���i�����󔒂�������...
				signalName = pPC->orgname();	//����Ƀ��C�u�������i�����g��
			}
		}else{
			pTag = (SXBSchTag*)(*iteTP);
			signalName = pTag->text();//�M�����Ɏg�����߂Ƀ^�O�e�L�X�g�𓾂�
		}
		signalName = removeWS(signalName);
		if(signalName.length()==0){
			iteTP++;
			continue;
		}
		nPinCount=(*iteTP)->pinCount();

		bool bUsed=false;

		for(i=0;i<nPinCount;i++){
			point=(*iteTP)->pinPosition(i);
			iteW = m_listWire.begin();
			while(iteW != m_listWire.end()){
				pW = (SXBSchWireNl*)(*iteW); 
				p1 = pW->p1();
				p2 = pW->p2();
				if(point == p1 || point == p2){
					pW->m_listSignalName.push_back(signalName);
					bUsed=true;
				}
				iteW++;
			}
			itePin = m_listNlPin.begin();
			while(itePin != m_listNlPin.end()){
				pPin = (SNlPin*)(*itePin); 
				if(point == pPin->m_point){
					pPin->m_listSigName.push_back(signalName);
					bUsed=true;
				}
				itePin++;
			}
		}
		if(!bUsed){
			m_listNcTagPower.push_back(*iteTP);
		}
		iteTP++;
	}
}


//���x�������ɐ��ɐ�����ǉ�
void SNetList::nameFromLabel()
{
	XBSchObjListIterator iteLabel;
	SXBSchLabel* pLabel;
	XBSchObjListIterator iteW;
	SXBSchWireNl* pWire;


	SPoint p1;
	SPoint p2;
	SPoint point;
	string signalName;
	bool fH;

	iteLabel = m_listLabel.begin();
	while(iteLabel != m_listLabel.end()){
		pLabel = (SXBSchLabel*)(*iteLabel);
		iteLabel++;
		signalName = pLabel->text();//�����Ɏg�����߂Ƀ��x��������𓾂�
		signalName = removeWS(signalName);
		if(signalName.length()==0) continue;
		fH = pLabel->horizontal();
		point = pLabel->p1();

		iteW = m_listWire.begin();
		while(iteW != m_listWire.end()){
			pWire = (SXBSchWireNl*)(*iteW);
			iteW++;
			p1 = pWire->p1();
			p2 = pWire->p2();
			if(!fH){
				if( point.x()!=p1.x() || p1.x()!=p2.x() )continue;
				if(   point.y() >= p1.y() && point.y() <= p2.y() 
				   || point.y() <= p1.y() && point.y() >= p2.y() )
					pWire->m_listSignalName.push_back(signalName);
			}else{
				if( point.y()!=p1.y() || p1.y()!=p2.y() )continue;
				if(   point.x() >= p1.x() && point.x() <= p2.x() 
				   || point.x() <= p1.x() && point.x() >= p2.x() )
					pWire->m_listSignalName.push_back(signalName);
			}

		}
	}
}

//SXBSchDoc���Ǘ����Ă���}�ʗv�f���玟�̗v�f���R�s�[���ĐU�蕪����̃��X�g�ɒǉ�����B
//  �ڍ��_
//  ���C���[		: ���C���[��SXBSchWireNl�ɕϊ�����
//  ���x��
//  �^�O/�d�����i
//  ���i			: ���i�ԍ��������Ă�����́A���i�ԍ��������ŕ��i�����قȂ镔�i�͏���
void SNetList::setData(const TCHAR* filename, XBSchObjList* plistSrc)
{
	SXBSchObj* pNewObj;
	int x=m_xOffset;
	int y=0;
	m_xOffset+=10000;

	m_srcfilePathList.push_back(filename);

	XBSchObjListIterator ite=plistSrc->begin();
	while(ite!=plistSrc->end()){
		SXBSchObj* pObj = (*ite++);
		switch(pObj->id()){
		case SXBSchObj::ID_JUNCTION:
			pNewObj = pObj->duplicate();
			pNewObj->move(x,y);
			m_listJunc.push_back(pNewObj);
			break;
		case SXBSchObj::ID_WIRE:
			{
				SXBSchWire* pWire = (SXBSchWire*)pObj;
				pNewObj =(SXBSchObj*)(new SXBSchWireNl(*pWire));
				pNewObj->move(x,y);
				m_listWire.push_back(pNewObj);
			}
			break;
		case SXBSchObj::ID_LABEL:
			pNewObj = pObj->duplicate();
			pNewObj->move(x,y);
			m_listLabel.push_back(pNewObj);
			break;
		case SXBSchObj::ID_TAG:
			pNewObj = pObj->duplicate();
			pNewObj->move(x,y);
			m_listTagPower.push_back(pNewObj);
			break;
		case SXBSchObj::ID_COMPONENT:
			{
				SXBSchComponent* pComp=(SXBSchComponent*)pObj;
				if(strlen(pComp->refnum())==0){		//���i�ԍ����Ȃ��ꍇ�͒ʏ핔�i�Ƃ��Ă͈���Ȃ�
					if(pComp->pinCount()==1){		//�s������1�̂Ƃ������d�����i�Ƃ��Ĉ���
						pNewObj = pObj->duplicate();
						pNewObj->move(x,y);
						m_listTagPower.push_back(pNewObj);
					}
				}else{
					pNewObj = pObj->duplicate();
					pNewObj->move(x,y);
					m_listComponent.push_back(pNewObj);
				}
			}
			break;
		}
	}
	//fprintf(stderr,"Junction :%d\n",m_listJunc.size());
	//fprintf(stderr,"Wire     :%d\n",m_listWire.size());
	//fprintf(stderr,"Component:%d\n",m_listComponent.size());
	//fprintf(stderr,"Tag&Power:%d\n",m_listTagPower.size());
	//fprintf(stderr,"Label    :%d\n",m_listLabel.size());
}
			

//�L���ȕ��i�̃s����񋓂���
void SNetList::enumPin()
{
	XBSchObjListIterator ite;
	SXBSchComponent* pComp;
	string refnum;
	string pinnum;
	SPoint point;
	int nPinCount;
	int i;
	int nBlock;

	ite = m_listComponent.begin();
	while(ite != m_listComponent.end()){
		pComp = (SXBSchComponent*)(*ite);
		ite++;
		refnum  = pComp->refnum();
		nPinCount = pComp->pinCount();
		nBlock = pComp->block();
		for(i=0;i<nPinCount;i++){
			int nLTRB;												//�Ăяo���̂��߂̃_�~�[
			const SPin* pPinInfo = pComp->pinEnd(i,nLTRB,point);	//�s�����̎擾
			if(pPinInfo == NULL) break;
			pinnum = pPinInfo->pinNum(nBlock);
			//if(pinnum.length()==0) break;
			SNlPin* pNlPin = new SNlPin;							//�l�b�g���X�^���g���s���`���𐶐�
			pNlPin->m_strRefNum = refnum;							//���i�ԍ�
			pNlPin->m_strPinNum = pinnum;							//�s���ԍ�
			pNlPin->m_point = point;								//�s���ʒu

			//TRACE("%s.%s  (%d,%d)\n",refnum.c_str(),pinnum.c_str(),point.x(),point.y());

			if(pinnum.length()==0){
				m_listNoNumPin.push_back(pNlPin);
			}else{	
				//�s�����Ǘ����郊�X�g�ɒǉ�
				m_listNlPin.push_back(pNlPin);
			}
		}
	}
}



//�w�蕶���񂪕����񃊃X�g�ɑ��݂��邩�ǂ����ǂ����̃`�F�b�N
bool SNetList::quaryExist(stringList& strlist,string& str)
{
	if(strlist.end() != find(strlist.begin(),strlist.end(),str)) return true;
	else return false;
}

//�w��_���_���X�g�ɑ��݂��邩�ǂ����ǂ����̃`�F�b�N
bool SNetList::quaryExist(PointList& pointlist,SPoint point)
{
	if(pointlist.end() != find(pointlist.begin(),pointlist.end(),point)) return true;
	else return false;
}



//�s���̖��O(�����ł͕��i�ԍ��ƃs���ԍ�����\�������A�l�b�g���X�g�ł̃s���̖��O)�ɂ��召��r
int SNetList::CompareSNlPin( const void *ppin1, const void *ppin2 )
{
	string strPrefix1;
	string strSuffix1;
	string strPrefix2;
	string strSuffix2;
	int nComp;
	int num1,num2;

	SNlPin* pPin1 = *(SNlPin**)(ppin1);
	SNlPin* pPin2 = *(SNlPin**)(ppin2);

	//���t�@�����X�ԍ��Ŕ�r
	parseRefNum(pPin1->m_strRefNum,strPrefix1,num1);
	parseRefNum(pPin2->m_strRefNum,strPrefix2,num2);

	nComp = strcmp(strPrefix1.c_str(),strPrefix2.c_str());
	if(nComp) return nComp;
	nComp = num1-num2;
	if(nComp) return nComp;


	//�s���ԍ��Ŕ�r
	parseRefNum(pPin1->m_strPinNum,strPrefix1,num1);
	parseRefNum(pPin2->m_strPinNum,strPrefix2,num2);

	nComp = strcmp(strPrefix1.c_str(),strPrefix2.c_str());
	if(nComp) return nComp;
	num1 = atoi(strSuffix1.c_str());
	num2 = atoi(strSuffix2.c_str());
	nComp = num1-num2;
	return nComp;

}


//SPartsListElement�̕��i�ԍ��ɂ���r
int SNetList::CompareSPartsListElement( const void *p1, const void *p2 )
{
	SPartsListElement* pElem1 = *(SPartsListElement**)(p1);
	SPartsListElement* pElem2 = *(SPartsListElement**)(p2);
	int nComp = strcmp(pElem1->m_prefix.c_str(),pElem2->m_prefix.c_str());
	if(nComp) return nComp;
	int n = pElem1->m_suffix - pElem2->m_suffix;
	if(n!=0) return n;
	return strcmp(pElem1->m_name.c_str(),pElem2->m_name.c_str());
}


void SNetList::enumConflictingPin()
{
	NlPinListIterator itePin1 = m_listNlPin.begin();
	while(itePin1 != m_listNlPin.end()){
		if(!(*itePin1)->m_checked){
			NlPinListIterator itePin2=itePin1;
			itePin2++;
			bool firstFind=true;
			while(itePin2 != m_listNlPin.end()){
				if(!(*itePin2)->m_checked){
					if(   ((*itePin1)->m_strRefNum == (*itePin2)->m_strRefNum)
						&&((*itePin1)->m_strPinNum == (*itePin2)->m_strPinNum) ){
						if(firstFind){
							firstFind=false;
							m_listNlConfPin.push_back(*itePin1);
						}
						(*itePin2)->m_checked = true;
						m_listNlConfPin.push_back(*itePin2);
					}
				}
				itePin2++;
			}
			if(!firstFind){		//�d������������
				m_listNlConfPin.push_back(NULL);
			}
		}
		itePin1++;
	}
}



//�l�b�g���o
SSingleNetList* SNetList::NetList1()
{
	//printf("newnet\n");
	SNlPin*		pPin;
	SPoint		point;
	NlPinList	listPin;							//�l�b�g�Ɋ܂܂��s��
	stringList  listSignalNameAll;					//�l�b�g�Ɋւ�関�����M����
	stringList  listSignalName;						//�l�b�g�Ɋւ�関�����M����
	PointList	listPoint;							//�l�b�g�Ɋւ����W

	XBSchObjListIterator ite1;
	SSingleNetList* pSingleNet;
	
	NlPinListIterator itePin = m_listNlPin.begin();	//�s���̃��X�g����s�����ЂƂ��o��
	if(itePin == m_listNlPin.end()) return NULL;	//�s�����Ȃ���Ή��������ɖ߂�

	pPin=(*itePin);
	m_listNlPin.erase(itePin);						//���o�����s���͌��̃��X�g�����������
	pSingleNet = new SSingleNetList;
	listPin.push_back(pPin);						//�l�b�g�ɓo�^
	listPoint.push_back(pPin->m_point);				//�s���ʒu�����W�̃��X�g�ɓo�^����B
	
	//�s���̐M�������擾���āA���ׂĐM�����̃��X�g�ɓo�^����B
	stringListIterator iteStr=pPin->m_listSigName.begin();
	while(iteStr!=pPin->m_listSigName.end()){
		listSignalNameAll.push_back(*iteStr);
		listSignalName.push_back(*iteStr);
		iteStr++;
	}

	//�������������Ȃ��Ȃ�܂łP. �Q.���J��Ԃ�
	//�P�D�������M�������Ȃ��Ȃ�܂�(1)(2)���J��Ԃ����s����B
	//	(1)�M�����ɂ��s���̒��o
	//	    �����M���������s���̎��o��
	//	    ���o�����s�������ʂ̐M�����̒ǉ��o�^
	//	    ���o�����s���̍��W�̓o�^
	//	(2)�M�����ɂ�郏�C���[�̒��o
	//	    �����M�����������C���[�̎��o��
	//		���o�������C���[�����ʂ̐M�����̒ǉ��o�^
	//	    ���o�������C���[�̍��W�̓o�^
	//
	//�Q�D���������W���Ȃ��Ȃ�܂�(3)(4)���J��Ԃ����s����B
	//	(3)���W�ɂ��s���̒��o
	//		���̍��W�����s���̎��o��
	//		���o�����s�������ʂ̐M�����̒ǉ��o�^
	//	(4)���W�ɂ�郏�C���[�̎��o��
	//		���̍��W�������C���[�̎��o��
	//		���o�������C���[�����ʂ̐M�����̒ǉ��o�^
	//		���o�������C���[�̍��W�̓o�^
	//

	stringListIterator iteSignalName;
	PointListIterator  itePoint;

	bool fDone = true;
	int  loopCount = 0;
	while(fDone){
		//printf("%d\n",loopCount);
		fDone = false;
		//***** �P�D�M�������� *****
		iteSignalName = listSignalName.begin();
		while(iteSignalName != listSignalName.end()){
			//printf("SigName");
			fDone = true;
			string strSignalName = (*iteSignalName);
			
			//***** (1)�M�����ɂ��s���̒��o *****
			NlPinListIterator itePin = m_listNlPin.begin();
			NlPinListIterator itePinBackUp;
			while(itePin != m_listNlPin.end()){				//�M�����ɂ��đS�Ẵs�����X�L��������
				SNlPin* pPin =(*itePin);
				itePinBackUp = itePin;
				itePin++;
				if(quaryExist(pPin->m_listSigName,strSignalName)){							//�s�������M�����̂ЂƂ��������̐M�����ɍ��v
					m_listNlPin.erase(itePinBackUp);										//�s���̃��X�g����͂���
					listPin.push_back(pPin);												//�l�b�g�̃s���̃��X�g�ɒǉ�����
					stringListIterator itePinSignalName = pPin->m_listSigName.begin();		//�s�������M���������ׂāA�l�b�g�̐M�����ɓo�^
					while(itePinSignalName != pPin->m_listSigName.end()){					//
						if(!quaryExist(listSignalNameAll,(*itePinSignalName))){				//�������A�V�K�ɓo�^����̂́A���o�^�̂��̂���
							listSignalNameAll.push_back(*itePinSignalName);
							listSignalName.push_back(*itePinSignalName);
						}
						itePinSignalName++;
					}
					if(!quaryExist(listPoint,pPin->m_point)){
						listPoint.push_back(pPin->m_point);
					}
				}
    		}// ***** (1)�M�����ɂ��s���̒��o *****
	

			// ***** (2)�M�����ɂ�郏�C���[�̒��o *****
			XBSchObjListIterator iteWire = m_listWire.begin();
			XBSchObjListIterator iteWireBackUp;
			while(iteWire != m_listWire.end()){				//�M�����ɂ��đS�Ẵ��C���[���X�L��������
				SXBSchWireNl* pWire =(SXBSchWireNl*)(*iteWire);
				iteWireBackUp = iteWire;
				iteWire++;
				if(quaryExist(pWire->m_listSignalName,strSignalName)){						//���C���[�����M�����̂ЂƂ��������̐M�����ɍ��v
					m_listWire.erase(iteWireBackUp);										//���C���[�̃��X�g����͂���
					stringListIterator iteWireSignalName = pWire->m_listSignalName.begin();	//���C���[�����M���������ׂāA�l�b�g�̐M�����ɓo�^
					while(iteWireSignalName != pWire->m_listSignalName.end()){					//
						if(!quaryExist(listSignalNameAll,(*iteWireSignalName))){	//�������A�V�K�ɓo�^����̂́A���o�^�̂��̂���
							listSignalNameAll.push_back(*iteWireSignalName);
							listSignalName.push_back(*iteWireSignalName);
						}
						iteWireSignalName++;
					}
					if(!quaryExist(listPoint,pWire->p1())){
						listPoint.push_back(pWire->p1());
					}
					if(!quaryExist(listPoint,pWire->p2())){
						listPoint.push_back(pWire->p2());
					}
					delete pWire;
				}
			} // ***** (2)�M�����ɂ�郏�C���[�̒��o *****
			iteSignalName = listSignalName.erase(iteSignalName);
		}// �����܂� ***** �P�D�M�������� *****

		itePoint = listPoint.begin();
		// ***** �Q�D���W���� *****
		while(itePoint != listPoint.end()){//���W�����J��Ԃ�
			//printf("Point");
			fDone = true;
			SPoint point = (*itePoint);
			// ***** (3)���W�ɂ��s���̒��o *****
			NlPinListIterator itePin = m_listNlPin.begin();
			NlPinListIterator itePinBackUp;
			while(itePin != m_listNlPin.end()){	//���W�ɂ��đS�Ẵs�����X�L��������
				SNlPin* pPin =(*itePin);
				itePinBackUp = itePin;
				itePin++;
				if(pPin->m_point == point){													//�s���̍��W���������̍��W�ƍ��v
					m_listNlPin.erase(itePinBackUp);										//�s���̃��X�g����͂���
					listPin.push_back(pPin);												//�l�b�g�̃s���̃��X�g�ɒǉ�����
					stringListIterator itePinSignalName = pPin->m_listSigName.begin();		//�s�������M���������ׂāA�l�b�g�̐M�����ɓo�^
					while(itePinSignalName != pPin->m_listSigName.end()){					//
						if(!quaryExist(listSignalNameAll,(*itePinSignalName))){	//�������A�V�K�ɓo�^����̂́A���o�^�̂��̂���
							listSignalNameAll.push_back(*itePinSignalName);
							listSignalName.push_back(*itePinSignalName);
						}
						itePinSignalName++;
					}
				}
			} //�����܂�***** (3)���W�ɂ��s���̒��o *****

			// ***** (4)���W�ɂ�郏�C���[�̒��o *****
			XBSchObjListIterator iteWire = m_listWire.begin();
			XBSchObjListIterator iteWireBackUp;
			while(iteWire != m_listWire.end()){		//���W�ɂ��đS�Ẵ��C���[���X�L��������
				SXBSchWireNl* pWire =(SXBSchWireNl*)(*iteWire);
				iteWireBackUp = iteWire;
				iteWire++;
				bool fHit=false;
				if(pWire->p1() == point){
					fHit=true;
					if(!quaryExist(listPoint,pWire->p2())){
						listPoint.push_back(pWire->p2());
					}
				}else if(pWire->p2() == point){
					fHit=true;
					if(!quaryExist(listPoint,pWire->p1())){
						listPoint.push_back(pWire->p1());
					}
				}
				if(fHit){
					stringListIterator iteWireSignalName = pWire->m_listSignalName.begin();	//���C���[�����M���������ׂāA�l�b�g�̐M�����ɓo�^
					while(iteWireSignalName != pWire->m_listSignalName.end()){				//
						if(!quaryExist(listSignalName,(*iteWireSignalName))){	//�������A�V�K�ɓo�^����̂́A���o�^�̂��̂���
							listSignalNameAll.push_back(*iteWireSignalName);
							listSignalName.push_back(*iteWireSignalName);
						}
						iteWireSignalName++;
					}
					delete pWire;
					m_listWire.erase(iteWireBackUp);										//���C���[�̃��X�g����͂���
				}
    		}// ***** (4)���W�ɂ�郏�C���[�̒��o *****
		
			itePoint=listPoint.erase(itePoint);
		}// while(itePoint != listPoint.end()){
		loopCount++ ;
	}


	if(listSignalNameAll.size()>0){
		//pSingleNet->m_SignalName=listSignalNameAll.front();
		stringListIterator ite=listSignalNameAll.begin();
		while(ite != listSignalNameAll.end()){
			pSingleNet->m_SignalNameList.push_back(*ite);
			ite++;
		}
	}


	pSingleNet->m_nPinCount=listPin.size();
	if(pSingleNet->m_nPinCount>0){
		pSingleNet->m_pPinArray = new PSNlPin[pSingleNet->m_nPinCount];
	
		NlPinListIterator itePin=listPin.begin();
		int n=0;
		while(itePin!=listPin.end()){
			pSingleNet->m_pPinArray[n++]=(*itePin++);
		}
		qsort(pSingleNet->m_pPinArray,pSingleNet->m_nPinCount, sizeof(PSNlPin), CompareSNlPin );
	}
	return pSingleNet;
}




/*
void dumpobj(XBSchObjList* listobj)
{
	SWriteCE3 wce3;
	wce3.Attach(stdout);
	XBSchObjListIterator ite=listobj->begin();
	int count=0;
	while(ite != listobj->end()){
		fprintf(stderr,"%d\n",count++);
		(*ite++)->writeCe3(wce3,NULL);
	}
}
*/

void  SNetList::createNetList(SNetlistOption& option)//bool bExcludeNC)
{
	jointWire();			//���C���[�̘A������
	divideWire();			//���C���[��ڍ��_�ŕ�������
	enumPin();				//�L���ȕ��i�̃s����񋓂���
	enumConflictingPin();	//�d���s����񋓂���
	nameFromTagPower();		//�^�O�Ɠd�����i�����ɐ��ƃs���ɖ��O������
	nameFromLabel();		//���x�������ɐ��ɐ�����ǉ�

	SSingleNetList* pSingleNet; 
	string str;

	m_signalNameList.clear();
	NetList	listNoNameNet;
	listNoNameNet.clear();

	while(NULL!=(pSingleNet=NetList1())){
		if(pSingleNet->m_nPinCount<=1){
			if(pSingleNet->m_nPinCount==1){
				SNlPin* pNlNcPin = new SNlPin;
				pNlNcPin->m_strPinNum =	pSingleNet->m_pPinArray[0]->m_strPinNum;
				pNlNcPin->m_strRefNum =	pSingleNet->m_pPinArray[0]->m_strRefNum;
				pNlNcPin->m_point =		pSingleNet->m_pPinArray[0]->m_point;
				pNlNcPin->m_listSigName.push_back(pSingleNet->SignalName());
				m_listNlNcPin.push_back(pNlNcPin);
			}
			m_listNcNet.push_back(pSingleNet);	//2012/01/18   Ver0.69.03
			//delete pSingleNet;
			continue;
		}else{
			//if(bExcludeNC && (pSingleNet->m_SignalName=="NOCONNECTION")){
//			if(bExcludeNC && (pSingleNet->SignalName()=="NOCONNECTION")){
//				delete pSingleNet;
//				continue;
//			}
			//if(pSingleNet->m_SignalName==""){

			str = pSingleNet->SignalName();

			if(str==""){
				listNoNameNet.push_back(pSingleNet);
			}else{
				m_signalNameList.push_back(str);
				m_listNet.push_back(pSingleNet);
			}



			//if(str==""){
			//	sprintf(signalName,"N%05d",signalNumber++);
			//	//pSingleNet->m_SignalName=string(signalName);


		}
	}

	char signalName[128];
	int  signalNumber=0;

	NetListIterator iteNet=listNoNameNet.begin();
	while(iteNet!=listNoNameNet.end()){
		pSingleNet = (*iteNet++);

		if(option.bSigNamePinInfo){
			int n=0;
			do{
				sprintf(signalName,"NET%03d_%s_%s",n++,pSingleNet->m_pPinArray[0]->m_strRefNum.c_str(),pSingleNet->m_pPinArray[0]->m_strPinNum.c_str());
				str = string(signalName);
			}while(!isUniqueSignalName(str));
		}else{
			do{
				sprintf(signalName,"N%05d",signalNumber++);
				str = string(signalName);

			}while(!isUniqueSignalName(str));
		}		

		pSingleNet->m_SignalNameList.push_back(str);
		
		m_signalNameList.push_back(str);

		m_listNet.push_back(pSingleNet);

	}

	listNoNameNet.clear();
	m_signalNameList.clear();
}

//20130914 �M���������j�[�N���ǂ����̃`�F�b�N�B
bool  SNetList::isUniqueSignalName(string str)
{
	return (m_signalNameList.end() == find(m_signalNameList.begin(),m_signalNameList.end(),str));
}

void SNetList::parseRefNum(const string& refnum,string& prefix,int& suffix)
{
	int len = refnum.length();
	int pos = len-1;
	while(pos>=0){
		if(!isdigit(refnum[pos])) break;
		pos--;
	}
	pos++;	//�v���t�B�b�N�X�̕�������������
	prefix = refnum.substr(0,pos);
	if(len<=pos)suffix=0;
	else suffix = atoi(refnum.substr(pos,len-pos).c_str());
}



void SNetList::createPartsList()
{
	
	int i;
	int nCount = m_listComponent.size();
	m_pArrayPlistElem = new PSPartsListElement[nCount];
	for(i=0;i<nCount;i++) m_pArrayPlistElem[i]=NULL;

	XBSchObjListIterator ite = m_listComponent.begin();
	
	i=0;
	while(ite != m_listComponent.end()){
		SXBSchComponent* pComp=(SXBSchComponent*)(*ite++);
		string prefix;
		int  suffix;
		parseRefNum(pComp->refnum(),prefix,suffix);
		m_pArrayPlistElem[i]=new SPartsListElement;
		m_pArrayPlistElem[i]->m_prefix = prefix;
		m_pArrayPlistElem[i]->m_suffix = suffix;
		m_pArrayPlistElem[i]->m_name = pComp->name();
		m_pArrayPlistElem[i]->m_note = pComp->note();
		m_pArrayPlistElem[i]->m_mfr	 = pComp->mfr();
		m_pArrayPlistElem[i]->m_mfrpn = pComp->mfrpnum();
		m_pArrayPlistElem[i]->m_package = pComp->packagename();
		i++;
	}
	qsort(m_pArrayPlistElem,nCount, sizeof(PSPartsListElement), CompareSPartsListElement );
}


void convertToCsvRecord(const string& src,string& dest)
{
	int len= src.size();
	dest="\"";
	for(int i=0;i<len;i++){
		if(src[i]=='\"'){
			dest+="\"\"";
		}else{
			dest+=src[i];
		}
	}
	dest+="\"";
}	


//CSV�`���̕��i���X�g�o��
int SNetList::writePartsList(const TCHAR* filename)
{
	if(m_pArrayPlistElem==NULL) return -2;


	FILE* fp = _wfopen(filename,_T("wb"));
	if(fp == NULL) return -1;


	string name;
	string prefix;
	int suffix;
	string outString;

	int n=m_listComponent.size();
	for(int i=0;i<n;i++){
		if(i>0 && (prefix==m_pArrayPlistElem[i]->m_prefix && suffix==m_pArrayPlistElem[i]->m_suffix)){
			if(name == m_pArrayPlistElem[i]->m_name){
				outString = "*";
			}else{
				outString = "?";
			}
		}else{
			outString = "";
		}
		fprintf(fp,"%s,",outString.c_str());
		convertToCsvRecord(m_pArrayPlistElem[i]->m_prefix,outString);
		fprintf(fp,"%s,%d,",outString.c_str(),m_pArrayPlistElem[i]->m_suffix);
		convertToCsvRecord(m_pArrayPlistElem[i]->m_name,outString);
		fprintf(fp,"%s\r\n",outString.c_str());
		prefix	= m_pArrayPlistElem[i]->m_prefix;
		suffix	= m_pArrayPlistElem[i]->m_suffix;
		name	= m_pArrayPlistElem[i]->m_name;
	}
	fclose(fp);
	return 0;
}

int SNetList::writeReport(FILE* fp,bool bExcludeNC)
{
	if(fp==NULL) return -1;
	NlPinListIterator ite;
	int sheet,x,y;

	SSingleNetList* pSingleNet;


	fprintf(fp,"*** Source files ***\r\n");
	wstringListIterator iteSrcName = m_srcfilePathList.begin();
	int index = 1;
	while(iteSrcName != m_srcfilePathList.end()){
		const TCHAR* fname = (*iteSrcName).c_str();
		int mbcbuffsize = ::WideCharToMultiByte(CP_UTF8,0,fname,-1,NULL,0,NULL,NULL);
		char* mbcbuff = new char[mbcbuffsize];
		::WideCharToMultiByte(CP_UTF8,0,fname,-1,mbcbuff,mbcbuffsize,NULL,NULL);
		fprintf(fp,"%d: %s\r\n",index,mbcbuff);
		delete[]mbcbuff;
		iteSrcName++;
		index++;
	}
	fprintf(fp,"\r\n");


	fprintf(fp,"*** CONNECTED SIGNAL NAMES ***\r\n");
	NetListIterator iteNet=m_listNet.begin();
	while(iteNet!=m_listNet.end()){
		pSingleNet = (*iteNet++);
		if(pSingleNet->m_SignalNameList.size()>1){
			stringListIterator ite = pSingleNet->m_SignalNameList.begin();
			int n= 0;
			while(ite != pSingleNet->m_SignalNameList.end()){
				if(n>0){
					fprintf(fp," ");
				}
				fprintf(fp,"%s",(*ite).c_str());
				ite++;n++;
			}
			fprintf(fp,"\r\n");
		}
	}
	fprintf(fp,"\r\n");


	fprintf(fp,"*** CONFLICTING PINS ***\r\n");
	ite = m_listNlConfPin.begin();
	while(ite != m_listNlConfPin.end()){
		fprintf(fp,"%s.%s\r\n",(*ite)->m_strRefNum.c_str(),(*ite)->m_strPinNum.c_str());
		while(ite != m_listNlConfPin.end() && (*ite)!=NULL){
			x=(*ite)->m_point.x();
			y=(*ite)->m_point.y();
			sheet=x/10000+1;
			x=x%10000;
			fprintf(fp,"%d:(%d,%d)\r\n",sheet,x,y);
			ite++;
		}
		if(ite != m_listNlConfPin.end()) ite++;
	}
	fprintf(fp,"\r\n");



	fprintf(fp,"*** UNCONNECTED PINS ***\r\n");
	int n=0;
	ite = m_listNlNcPin.begin();
	while(ite != m_listNlNcPin.end()){
		if(bExcludeNC && (*ite)->m_listSigName.size() == 1 
			&& ((*(*ite)->m_listSigName.begin()))=="NOCONNECTION"){
		}else{
			fprintf(fp,"%s.%s",(*ite)->m_strRefNum.c_str(),(*ite)->m_strPinNum.c_str());
			if(n==4){
				fprintf(fp,"\r\n");
				n=0;
			}else{
				fprintf(fp," ");
				n++;
			}
		}
		ite++;
	}
	if(n!=0){
		fprintf(fp,"\r\n");
	}
	fprintf(fp,"\r\n");

	string signalName;

	XBSchObjListIterator iteTP;
	SXBSchTag* pTag;
	SXBSchComponent* pPC;

	fprintf(fp,"*** UNCONNECTED TAGS ***\r\n");
	iteTP = m_listNcTagPower.begin();
	while(iteTP != m_listNcTagPower.end()){
		if((*iteTP)->id() == SXBSchObj::ID_TAG){
			pTag = (SXBSchTag*)(*iteTP);
			x=pTag->p1().x();
			y=pTag->p1().y();
			sheet=x/10000+1;
			x=x%10000;
			signalName = pTag->text();
			fprintf(fp,"%d:(%d,%d) %s\r\n",sheet,x,y,signalName.c_str());
		}
		iteTP++;
	}
	fprintf(fp,"\r\n");

	fprintf(fp,"*** UNCONNECTED POWER SYMBOLS ***\r\n");
	iteTP = m_listNcTagPower.begin();
	while(iteTP != m_listNcTagPower.end()){
		if((*iteTP)->id() == SXBSchObj::ID_COMPONENT){
			pPC = (SXBSchComponent*)(*iteTP);
			x=pPC->p1().x();
			y=pPC->p1().y();
			sheet=x/10000+1;
			x=x%10000;
			signalName = pPC->name();			//�M�����Ɏg�����߂ɕ��i���𓾂�
			if(signalName.length() == 0){		//���i�����󔒂�������...
				signalName = pPC->orgname();	//����Ƀ��C�u�������i�����g��
			}
			fprintf(fp,"%d:(%d,%d) %s\r\n",sheet,x,y,signalName.c_str());
		}
		iteTP++;
	}
	fprintf(fp,"\r\n");

	fprintf(fp,"*** NO NUMBER PINS ***\r\n");
	n=0;
	ite = m_listNoNumPin.begin();
	while(ite != m_listNoNumPin.end()){
		if(n==2){
			fprintf(fp,"\r\n");
			n=0;
		}else if(n>0){
			fprintf(fp," ");
		}
		x=(*ite)->m_point.x();
		y=(*ite)->m_point.y();
		sheet=x/10000+1;
		x=x%10000;
		fprintf(fp,"%s %d:(%d,%d)",(*ite)->m_strRefNum.c_str(),sheet,x,y);
		n++;
		ite++;
	}
	fprintf(fp,"\r\n");



	return 0;
}

//
int SNetList::writeUserTypeNetList(const TCHAR* filename, bool bExcludeNC, int( __stdcall *fnWriteNetList)(const TCHAR*, const SExportNetlistInfo*))
{
	SExportNetlistInfo exportNetlistInfo;

	if(!filename) return -1;
	
	if(m_pArrayPlistElem==NULL) return -2;

	if(!fnWriteNetList) return -4; 

	string outString;

	string prefix;
	int suffix;
	
	//���i�����܂Ƃ߂�
	//�܂��͈�ʂ蕔�i��񃊃X�g���X�L�������ėL���ȕ��i���̐������߂�
	int n=m_listComponent.size();
	int nCompInfo = 0;
	for(int i=0;i<n;i++){
		if(    (i>0)
			&& (prefix == m_pArrayPlistElem[i]->m_prefix)
			&& (suffix == m_pArrayPlistElem[i]->m_suffix)
			){
			continue;
		}
		prefix = m_pArrayPlistElem[i]->m_prefix;
		suffix = m_pArrayPlistElem[i]->m_suffix;
		nCompInfo++;
	}


	exportNetlistInfo.nComponentInfoCount = nCompInfo;
	exportNetlistInfo.array_CompInfo = new SExportComponentInfo[nCompInfo];


	int index;

	index = 0;
	for(int i=0;i<n;i++){
		if(    (i>0)
			&& (prefix == m_pArrayPlistElem[i]->m_prefix)
			&& (suffix == m_pArrayPlistElem[i]->m_suffix)
			){
			continue;
		}
		
		prefix = m_pArrayPlistElem[i]->m_prefix;
		suffix = m_pArrayPlistElem[i]->m_suffix;
		if(index>=n) break;

		exportNetlistInfo.array_CompInfo[index].pszPrefix = m_pArrayPlistElem[i]->m_prefix.c_str();
		exportNetlistInfo.array_CompInfo[index].nSuffix = m_pArrayPlistElem[i]->m_suffix;
		exportNetlistInfo.array_CompInfo[index].pszName = m_pArrayPlistElem[i]->m_name.c_str();
		exportNetlistInfo.array_CompInfo[index].pszPackage = m_pArrayPlistElem[i]->m_package.c_str();
		index++;
	}


	//�l�b�g�����܂Ƃ߂�
	int nNetCount = m_listNet.size();	//�Ƃ肠�������̐��Ńl�b�g��񐔂��m�ۂ���BNC���폜����ƁA���ۂ̐��͂ЂƂ��邩��
	exportNetlistInfo.array_NetValue = new SExportSingleNetValue[nNetCount];
	

	SSingleNetList* pSingleNet;
	
	//typedef list<string> stringList;
	stringList sigNameList;


	index = 0;


	NetListIterator iteNet=m_listNet.begin();
	while(iteNet!=m_listNet.end()){
		pSingleNet = (*iteNet++);
		if(bExcludeNC && pSingleNet->SignalName()=="NOCONNECTION"){
			continue;
		}
		
		exportNetlistInfo.array_NetValue[index].nPinCount = pSingleNet->m_nPinCount;
		exportNetlistInfo.array_NetValue[index].pArrayPinValue = new SExportPinValue[pSingleNet->m_nPinCount];


		string str = pSingleNet->SignalName();
		sigNameList.push_back(str);
	
		exportNetlistInfo.array_NetValue[index].pszNetName = sigNameList.back().c_str();

	
		for(int i=0;i< pSingleNet->m_nPinCount;i++){
			exportNetlistInfo.array_NetValue[index].pArrayPinValue[i].pszRefNum = pSingleNet->m_pPinArray[i]->m_strRefNum.c_str();
			exportNetlistInfo.array_NetValue[index].pArrayPinValue[i].pszPinNum = pSingleNet->m_pPinArray[i]->m_strPinNum.c_str();
		}
		index++;
	}
	exportNetlistInfo.nNetValueCount = index;



	int retValue = fnWriteNetList(filename, &exportNetlistInfo);



	//��n��
	delete[](exportNetlistInfo.array_CompInfo);
	for(int i=0;i<exportNetlistInfo.nNetValueCount;i++){
		if(exportNetlistInfo.array_NetValue[i].pArrayPinValue >0){
			delete[](exportNetlistInfo.array_NetValue[i].pArrayPinValue);
		}
	}
	delete[](exportNetlistInfo.array_NetValue);

	return retValue;
}


//�v���e���`���̃l�b�g���X�g�o��
int SNetList::writeProtNetList(const TCHAR* filename,bool bExcludeNC)
{
	string signalname;
	string pinname;
	string name;
	string package;
	string prefix;
	int suffix;
	
	
	if(m_pArrayPlistElem==NULL) return -2;

	FILE* fp = _wfopen(filename,_T("wb"));
	if(fp==NULL) return -1;

	string outString;

	int n=m_listComponent.size();
	for(int i=0;i<n;i++){
		if(    (i>0)
			&& (prefix == m_pArrayPlistElem[i]->m_prefix)
			&& (suffix == m_pArrayPlistElem[i]->m_suffix)
			){
			continue;
		}
		prefix = m_pArrayPlistElem[i]->m_prefix;
		suffix = m_pArrayPlistElem[i]->m_suffix;
		name   = m_pArrayPlistElem[i]->m_name;
		package = m_pArrayPlistElem[i]->m_package;
		fprintf(fp,"[\r\n%s%d\r\n%s\r\n%s\r\n\r\n\r\n\r\n]\r\n",prefix.c_str(),suffix,package.c_str(),name.c_str());
	}

	
	SSingleNetList* pSingleNet;

	NetListIterator iteNet=m_listNet.begin();
	while(iteNet!=m_listNet.end()){
		pSingleNet = (*iteNet++);
		if(bExcludeNC && pSingleNet->SignalName()=="NOCONNECTION"){
			continue;
		}
		//fprintf(fp,"%s;  ",pSingleNet->m_SignalName.c_str());
		fprintf(fp,"(\r\n%s\r\n",pSingleNet->SignalName().c_str());
		for(int i=0;i< pSingleNet->m_nPinCount;i++){
			fprintf(fp,"%s-%s\r\n",pSingleNet->m_pPinArray[i]->m_strRefNum.c_str(),pSingleNet->m_pPinArray[i]->m_strPinNum.c_str());
		}
		fprintf(fp,")\r\n");

	}
	fclose(fp);
	return 0;
}

//�e���V�X�`���̃l�b�g���X�g�o��
int SNetList::writeTeleNetList(const TCHAR* filename,bool bExcludeNC)
{
	string signalname;
	string pinname;
	string name;
	string package;
	string prefix;
	int suffix;
	
	

	if(m_pArrayPlistElem==NULL) return -2;

	FILE* fp = _wfopen(filename,_T("wb"));
	if(fp==NULL) return -1;

	fprintf(fp,"$PACKAGES\r\n");


	string outString;


	int n=m_listComponent.size();
	for(int i=0;i<n;i++){
		if(    (i>0)
			&& (prefix == m_pArrayPlistElem[i]->m_prefix)
			&& (suffix == m_pArrayPlistElem[i]->m_suffix)
			){
			continue;
		}
		prefix = m_pArrayPlistElem[i]->m_prefix;
		suffix = m_pArrayPlistElem[i]->m_suffix;
		name   = m_pArrayPlistElem[i]->m_name;
		package = m_pArrayPlistElem[i]->m_package;
		fprintf(fp,"%s! %s; %s%d\r\n",package.c_str(),name.c_str(),prefix.c_str(),suffix);
	}

	fprintf(fp,"$NETS\r\n");
	SSingleNetList* pSingleNet;

	NetListIterator iteNet=m_listNet.begin();
	while(iteNet!=m_listNet.end()){
		pSingleNet = (*iteNet++);
		if(bExcludeNC && pSingleNet->SignalName()=="NOCONNECTION"){
			continue;
		}
		//fprintf(fp,"%s;  ",pSingleNet->m_SignalName.c_str());
		fprintf(fp,"%s;  ",pSingleNet->SignalName().c_str());
		int n=0;
		for(int i=0;i< pSingleNet->m_nPinCount;i++){
			if(n==5){
				fprintf(fp," ,\r\n     ");
				n=0;
			}else if(n>0){
				fprintf(fp," ");
			}
			fprintf(fp,"%s.%s",pSingleNet->m_pPinArray[i]->m_strRefNum.c_str(),pSingleNet->m_pPinArray[i]->m_strPinNum.c_str());
			n++;
		}
		fprintf(fp,"\r\n");

	}
	fprintf(fp,"$END\r\n");
	fclose(fp);
	return 0;
}
//
////�v���e���`���̃l�b�g���X�g�o��
//int SNetList::writeProtNetList(FILE* fp,bool bExcludeNC)
//{
//	string signalname;
//	string pinname;
//	string name;
//	string package;
//	string prefix;
//	int suffix;
//	
//	if(fp==NULL) return -1;
//	
//	if(m_pArrayPlistElem==NULL) return -2;
//	string outString;
//
//	int n=m_listComponent.size();
//	for(int i=0;i<n;i++){
//		if(    (i>0)
//			&& (prefix == m_pArrayPlistElem[i]->m_prefix)
//			&& (suffix == m_pArrayPlistElem[i]->m_suffix)
//			){
//			continue;
//		}
//		prefix = m_pArrayPlistElem[i]->m_prefix;
//		suffix = m_pArrayPlistElem[i]->m_suffix;
//		name   = m_pArrayPlistElem[i]->m_name;
//		package = m_pArrayPlistElem[i]->m_package;
//		fprintf(fp,"[\n%s%d\n%s\n%s\n\n\n\n]\n",prefix.c_str(),suffix,package.c_str(),name.c_str());
//	}
//
//	
//	SSingleNetList* pSingleNet;
//
//	NetListIterator iteNet=m_listNet.begin();
//	while(iteNet!=m_listNet.end()){
//		pSingleNet = (*iteNet++);
//		if(bExcludeNC && pSingleNet->SignalName()=="NOCONNECTION"){
//			continue;
//		}
//		//fprintf(fp,"%s;  ",pSingleNet->m_SignalName.c_str());
//		fprintf(fp,"(\n%s\n",pSingleNet->SignalName().c_str());
//		for(int i=0;i< pSingleNet->m_nPinCount;i++){
//			fprintf(fp,"%s-%s\n",pSingleNet->m_pPinArray[i]->m_strRefNum.c_str(),pSingleNet->m_pPinArray[i]->m_strPinNum.c_str());
//		}
//		fprintf(fp,")\n");
//
//	}
//	return 0;
//}
//
////�e���V�X�`���̃l�b�g���X�g�o��
//int SNetList::writeTeleNetList(FILE* fp,bool bExcludeNC)
//{
//	string signalname;
//	string pinname;
//	string name;
//	string package;
//	string prefix;
//	int suffix;
//	
//	if(fp==NULL) return -1;
//	
//	fprintf(fp,"$PACKAGES\n");
//
//	if(m_pArrayPlistElem==NULL) return -2;
//	string outString;
//
//	int n=m_listComponent.size();
//	for(int i=0;i<n;i++){
//		if(    (i>0)
//			&& (prefix == m_pArrayPlistElem[i]->m_prefix)
//			&& (suffix == m_pArrayPlistElem[i]->m_suffix)
//			){
//			continue;
//		}
//		prefix = m_pArrayPlistElem[i]->m_prefix;
//		suffix = m_pArrayPlistElem[i]->m_suffix;
//		name   = m_pArrayPlistElem[i]->m_name;
//		package = m_pArrayPlistElem[i]->m_package;
//		fprintf(fp,"%s! %s; %s%d\n",package.c_str(),name.c_str(),prefix.c_str(),suffix);
//	}
//
//	fprintf(fp,"$NETS\n");
//	SSingleNetList* pSingleNet;
//
//	NetListIterator iteNet=m_listNet.begin();
//	while(iteNet!=m_listNet.end()){
//		pSingleNet = (*iteNet++);
//		if(bExcludeNC && pSingleNet->SignalName()=="NOCONNECTION"){
//			continue;
//		}
//		//fprintf(fp,"%s;  ",pSingleNet->m_SignalName.c_str());
//		fprintf(fp,"%s;  ",pSingleNet->SignalName().c_str());
//		int n=0;
//		for(int i=0;i< pSingleNet->m_nPinCount;i++){
//			if(n==5){
//				fprintf(fp," ,\n     ");
//				n=0;
//			}else if(n>0){
//				fprintf(fp," ");
//			}
//			fprintf(fp,"%s.%s",pSingleNet->m_pPinArray[i]->m_strRefNum.c_str(),pSingleNet->m_pPinArray[i]->m_strPinNum.c_str());
//			n++;
//		}
//		fprintf(fp,"\n");
//
//	}
//	fprintf(fp,"$END\n");
//	return 0;
//}





////PADS�`���̃l�b�g���X�g�o��
//int SNetList::writePADSNetList(FILE* fp,bool bExcludeNC)
//{
//	string signalname;
//	string pinname;
//	if(fp==NULL) return -1;
//	
//
//	fprintf(fp,"*PADS*\n*NET*\n\n");
//	SSingleNetList* pSingleNet;
//
//	NetListIterator iteNet=m_listNet.begin();
//	while(iteNet!=m_listNet.end()){
//		pSingleNet = (*iteNet++);
//		if(bExcludeNC && pSingleNet->SignalName()=="NOCONNECTION"){
//			continue;
//		}
//		//fprintf(fp,"*SIG %s\n",pSingleNet->m_SignalName.c_str());
//		fprintf(fp,"*SIG %s\n",pSingleNet->SignalName().c_str());
//		int n=0;
//		for(int i=0;i< pSingleNet->m_nPinCount;i++){
//			if(n==5){
//				fprintf(fp,"\n");
//				n=0;
//			}else if(n>0){
//				fprintf(fp,"    ");
//			}
//			fprintf(fp,"%s.%s",pSingleNet->m_pPinArray[i]->m_strRefNum.c_str(),pSingleNet->m_pPinArray[i]->m_strPinNum.c_str());
//			n++;
//		}
//		fprintf(fp,"\n\n");
//
//	}
//	fprintf(fp,"*END*\n");
//	return 0;
//}





//�l�b�g���X�g���쐬����B
int netlist(int srcfiles, const TCHAR* srcfile[],SNetlistOption& option)
{
	SNetList netlist;
	if(option.pszDstPath == NULL || _tcslen(option.pszDstPath)==0) return -1;

	for(int n=0;n<srcfiles;n++){
		SXBSchDoc* pDoc=new SXBSchDoc;
		_ftprintf(stderr,_T("Read %s"),srcfile[n]);
		FILE* fp = _wfopen(srcfile[n],_T("rb"));
		if(fp==NULL) return -3;
		int retval = pDoc->readFile(fp);
		fclose(fp);
		if(retval!=0){
			fprintf(stderr,"\r\n");
			return -3;
		}
		fprintf(stderr,"\r\n");
		netlist.setData(srcfile[n], &(pDoc->m_listMain));
		delete pDoc;
	}
	if(!option.bPlistMode){
		int retVal;
		netlist.createPartsList();
		netlist.createNetList(option);//option.bExcludeNC);
		if(option.listType == 1){
			retVal = netlist.writeTeleNetList(option.pszDstPath,option.bExcludeNC);
		}else if(option.listType == 2){
			retVal = netlist.writeProtNetList(option.pszDstPath,option.bExcludeNC);
		}else{
			int addinIndex = option.listType - 3;
			

			int( __stdcall *fnWriteNetList)(const TCHAR*, const SExportNetlistInfo*);
			if(addinIndex<0 || addinIndex>=g_addinFiles.m_nAddinCount) return -4;
			HINSTANCE hDll = g_addinFiles.m_hAddin[addinIndex];
			fnWriteNetList = (int( __stdcall *)(const TCHAR*, const SExportNetlistInfo*)) ::GetProcAddress( hDll,"fnWriteNetList");
			if(fnWriteNetList){
				retVal = netlist.writeUserTypeNetList(option.pszDstPath,option.bExcludeNC, fnWriteNetList);
			}
		}
		if(option.fpRpt) netlist.writeReport(option.fpRpt,option.bExcludeNC);
		return retVal;
	}else{
		netlist.createPartsList();
		netlist.writePartsList(option.pszDstPath);
		return 0;
	}
}


