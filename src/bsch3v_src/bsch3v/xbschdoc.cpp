/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

//XBSch�̃h�L�������g�̊Ǘ����s���B
#include "stdafx.h"
//#include <qapplication.h>
//#include <qwidget.h>
//#include <SSize.h>
//#include <SPoint.h>
//#include <qclipboard.h>
#include <stdio.h>
#include <assert.h>
#include <string>
#include <list>
using namespace std;
#include "ce3io.h"
//#include "xbschobj.h"
//#include "xbschjunc.h"
//#include "xbschlabel.h"
//#include "xbschcomment.h"
//#include "xbschtag.h"
//#include "xbschcomponent.h"

//#include "xbschline.h"
//#include "xbschentry.h"
//#include "xbschdelobj.h"
//#include "xbschsheetinfo.h"
#include "SheetSizeInfo.h"
#include "xbschdoc.h"


unsigned  SLayer::wLayerTable[]
		={	0x0001,0x0002,0x0004,0x0008,
									0x0010,0x0020,0x0040,0x0080,
									0x0100,0x0200,0x0400,0x0800,
								0x1000,0x2000,0x4000,0x8000};


SPointEmum::SPointEmum()
{
	m_list.clear();
}

SPointEmum::~SPointEmum()
{
	m_list.clear();
}

bool SPointEmum::addSPoint(SPoint point)
{
	SPointListIterator ite;
	ite = m_list.begin();
	while(ite !=m_list.end()){
		if( (ite->x()==point.x()) &&  (ite->y()==point.y()) ){
			TRACE("SPointEmum::addSPoint() returns false\n");
			return false;
		}
		ite++;
	}
	m_list.push_back(point);
	return true;
}
//
//	SPointPtrList m_list;
//};




SXBSchDoc::SXBSchDoc()
{
//	m_bDirty = false;
//	m_bTempDirty = false;
//	m_bTempNewPlace = false;
//	m_pView = NULL;
//	m_bSelectForDrag = false;
	m_sizeSheet = SSize(1500,1000);
	init();
}

SXBSchDoc::~SXBSchDoc()
{
	init();
}

bool SXBSchDoc::isEmpty()
{
	if( (m_listMain.size() > 0) || (m_listTemp.size() > 0 )) return false;
	else return true;
}


//#define SHEETSIZE_X_MIN		640
//#define SHEETSIZE_X_MAX		5000
//#define SHEETSIZE_Y_MIN		400
//#define SHEETSIZE_Y_MAX		4000

//�V�[�g�T�C�Y�̐ݒ�
void SXBSchDoc::SetSheetSize(const SSize& size)
{
	if(   size.w()<SHEETSIZE_X_MIN
		||size.w()>SHEETSIZE_X_MAX
		||size.h()<SHEETSIZE_Y_MIN
		||size.h()>SHEETSIZE_Y_MAX) return;


	m_sizeSheet = size;
}

//20060603	//�}�ʗv�f�̒ǉ� ����ԍ���Ԃ�
//20060603	int SXBSchDoc::add(SXBSchObj* pObj)
//20060603	{
//20060603		if(pObj == NULL) return m_nOpCount;
//20060603		m_nOpCount++;
//20060603		m_bDirty = true;	
//20060603		pObj->setPlaceNum(m_nOpCount);	
//20060603		m_listMain.push_back(pObj);
//20060603		m_bFindCommandActive = false;
//20060603		return m_nOpCount;
//20060603	}

//�I����ԂŒǉ��}�ʗv�f�̒ǉ� ����ԍ���Ԃ�
int  SXBSchDoc::addSelect(SXBSchObj* pObj)
{
	m_listTemp.push_back(pObj);
	setupRcTempArea();
	m_bTempNewPlace = true;
	m_bFindCommandActive = false;

	//m_bDirty = true;	
	return m_nOpCount;
}

//�_���w�肵�Ă̑I��
//�_���w�肵�Ă̑I���ł́A1�̃I�u�W�F�N�g�����I�΂Ȃ��B
int SXBSchDoc::select(const SPoint& pt,SelectOption& refSelOption)
{
	//resetSelect();		//2009/05/10 �R�����g�A�E�g
	SXBSchObj* pObjSelected = NULL;
	//m_resetSelectOption = RESET_SELECT_NORMAL;

	if(m_bValidContinuousSingleSelect){
		if(abs(m_ptBiginSingleSelectAt.x()-pt.x())>5 || abs(m_ptBiginSingleSelectAt.y()-pt.y())>5){
			m_bValidContinuousSingleSelect = false;
		}
	}


	if(!m_bValidContinuousSingleSelect){
		TRACE("Init SingleSelect\n");
		int maxSelectWeight=0;
		XBSchObjListIterator pObjIte = m_listMain.begin();
		while(pObjIte !=  m_listMain.end()){
			if( (*pObjIte)->layer() == m_editLayer
				&&(*pObjIte)->deleteNum()<0
			  &&(*pObjIte)->refObj()==NULL	//�Q�ƃI�u�W�F�N�g�����݂�����͖̂����ł���
			 && ( ((refSelOption.m_mask & OBJMASK_USEFILTER) == 0)||( (refSelOption.m_mask & (*pObjIte)->objectFilterValue()) == 0 ))
			){
				int SelectWeight = (*pObjIte)->qSelectWeight(pt) & 0xFFFF;
				if(SelectWeight > maxSelectWeight){
					m_singleSelIte = pObjIte;
					maxSelectWeight = SelectWeight;
					pObjSelected = (*pObjIte);
	//				pObjSelected->setOldDataIterator(pObjIte);
				}
			}
			pObjIte++;
		}

	}else{
		TRACE("\nContinuous SingleSelect\n");
		int searchSize = m_listMain.size();
		int searchCount = 0;
		bool hitFlag = false;
		while(searchCount<searchSize && !hitFlag){
			m_singleSelIte++;
			searchCount++;
			if(m_singleSelIte == m_listMain.end()){
				m_singleSelIte = m_listMain.begin();
			}
			if(m_singleSelIte == m_listMain.end()){
				m_bValidContinuousSingleSelect = false;
				break;
			}			
			if( (*m_singleSelIte)->layer() == m_editLayer
				&&(*m_singleSelIte)->deleteNum()<0
			  &&(*m_singleSelIte)->refObj()==NULL	//�Q�ƃI�u�W�F�N�g�����݂�����͖̂����ł���
			 && ( ((refSelOption.m_mask & OBJMASK_USEFILTER) == 0)||( (refSelOption.m_mask & (*m_singleSelIte)->objectFilterValue()) == 0 ))
			){
				if((*m_singleSelIte)->qSelectWeight(pt)){
					pObjSelected = (*m_singleSelIte);
					hitFlag = true;
					TRACE("Continuous SingleSelect searchCount = %d\n",searchCount);
				}
			}
		}
	}


	if(pObjSelected != NULL){
		if(!m_bValidContinuousSingleSelect){
			m_bValidContinuousSingleSelect = true;
			m_ptBiginSingleSelectAt = pt;
		}

		SXBSchObj* pNewObj = pObjSelected->duplicate();
		pObjSelected->setRefObj(pNewObj);
		pNewObj->setRefObj(pObjSelected);
		pNewObj->setSelection(SXBSchObj::SELECT_ALL);
		m_listTemp.push_back(pNewObj);
		m_bFindCommandActive = false;
	}

	setupRcTempArea();
	setupRotateRect();
	m_bSelectForDrag = false;
	int selCount = m_listTemp.size();
	if(selCount>0){
		updateSelectedTypeFlag();
		discardRedo();
	}
	return selCount; 
}

//�͈͂��w�肵�Ă̑I��
//�I���Ƃ́A
//  ���C���̃��X�g����Y��������̂������A
//  �������ꎞ�I�u�W�F�N�g�ɒǉ�����
//�I����������Ԃ�
int SXBSchDoc::select(const SRect& rc,SelectOption& refSelOption)
{
	//resetSelect();		//2009/05/10 �R�����g�A�E�g
	//m_resetSelectOption = RESET_SELECT_NORMAL;
	XBSchObjListIterator pObjIte = m_listMain.begin();
	while(pObjIte != m_listMain.end()){
		if( (*pObjIte)->layer() == m_editLayer
		 && (*pObjIte)->deleteNum()<0
		 && (*pObjIte)->refObj()==NULL	//�Q�ƃI�u�W�F�N�g�����݂�����͖̂����ł���
		 && ( ((refSelOption.m_mask & OBJMASK_USEFILTER) == 0)||( (refSelOption.m_mask & (*pObjIte)->objectFilterValue()) == 0 ))
		){
			unsigned nSelStatus = (*pObjIte)->testSelection(rc);
			if(nSelStatus){
				SXBSchObj* pNewObj = (*pObjIte)->duplicate();
				(*pObjIte)->setRefObj(pNewObj);
				pNewObj->setRefObj(*pObjIte);
				pNewObj->setSelection(nSelStatus);
				m_listTemp.push_back(pNewObj);
			}
		}
		pObjIte++;
	}
	setupRcTempArea();
	setupRotateRect();
	m_bSelectForDrag = false;
	int n = m_listTemp.size();
	if(n>0){
		m_bFindCommandActive = false;
		updateSelectedTypeFlag();
		discardRedo();
	}
	return n;
}


//�}�ʗv�f�����C�����X�g��push_back����B
void SXBSchDoc::pushBackToMainList(SXBSchObj* pObj,bool doTidyUp)
{
	//doTidyUp = false;
	if(m_bDoTidyUp && doTidyUp){
		if(		pObj->id()==SXBSchObj::ID_JUNCTION
			||  pObj->id()==SXBSchObj::ID_BENTRY
			||  pObj->id()==SXBSchObj::ID_ENTRY	){		//20090207 �d���`�F�b�N�ɃG���g���[�ǉ�
			//if(checkRedundancyJunction((SXBSchJunc*)pObj)){
			if(checkRedundancy(pObj)){
				delete pObj;
				return;
			}
		}else if(pObj->id()==SXBSchObj::ID_WIRE || pObj->id()==SXBSchObj::ID_BUS){
			tidyUpWireBus((SXBSchLine*)pObj);
			return;
		}
	}
	m_listMain.push_back(pObj);
}

//���C���[,�W�����N�V�����̓�������  �X�e�b�v1
//�������W�����N�V�����̏ꏊ�ŌÂ����C���[����������
void SXBSchDoc::tidyUpWireJunction_1(int opCount)
{
	XBSchObjListIterator pObjIte = m_listMain.begin();
	while(pObjIte != m_listMain.end()){
		//if(   (*pObjIte)->placeNum()<opCount
		//	//||(*pObjIte)->id() != SXBSchObj::ID_DELETEOBJ
		//	||(*pObjIte)->deleteNum()<0							//0.69
		//	||(*pObjIte)->refObj() == NULL){
		//	pObjIte++;
		//	continue;
		//}
		//if((*pObjIte)->refObj()->id() != SXBSchObj::ID_JUNCTION){
		//	
		//	continue;
		//}

		if((*pObjIte)->id() != SXBSchObj::ID_JUNCTION || (*pObjIte)->deleteNum()!=opCount){
			pObjIte++;
			continue;
		}

		//SXBSchObj* pObj = (*pObjIte)->refObj(); //����������ꂽ�W�����N�V����
		SXBSchObj* pObj = *pObjIte; //����������ꂽ�W�����N�V����
		int nLayer = pObj->layer();	//����������ꂽ�W�����N�V�����̃��C���[
		SPoint pj = pObj->p1();		//����������ꂽ�W�����N�V�����̈ʒu
		XBSchObjListIterator pObjIteW = m_listMain.begin();
		SXBSchWire* pWireVl = NULL;
		SXBSchWire* pWireVr = NULL;
		SXBSchWire* pWireHl = NULL;
		SXBSchWire* pWireHr = NULL;
		while(pObjIteW != m_listMain.end()){
			if(   (*pObjIteW)->id() != SXBSchObj::ID_WIRE
				//||(*pObjIte)->placeNum()>=opCount
				||(*pObjIteW)->layer() != nLayer 
				||(*pObjIteW)->deleteNum()>=0		//2009/12/18
				||(*pObjIteW)->refObj()!=NULL){
				pObjIteW++;
				continue;
			}
			if((*pObjIteW)->p1()!=pj && (*pObjIteW)->p2()!=pj){
				pObjIteW++;
				continue;
			}
			bool horz = ((*pObjIteW)->y1() == (*pObjIteW)->y2());
			bool vert = ((*pObjIteW)->x1() == (*pObjIteW)->x2());
			if((!horz && !vert)||(horz && vert)){
				pObjIteW++;
				continue;
			}
			if(horz){
				if( ((*pObjIteW)->x1()+(*pObjIteW)->x2()) > pj.x()*2 ){
					pWireHr = (SXBSchWire*)(*pObjIteW);
				}else{
					pWireHl = (SXBSchWire*)(*pObjIteW);
				}
			}else{
				if( ((*pObjIteW)->y1()+(*pObjIteW)->y2()) > pj.y()*2 ){
					pWireVr = (SXBSchWire*)(*pObjIteW);
				}else{
					pWireVl = (SXBSchWire*)(*pObjIteW);
				}
			}
			pObjIteW++;
			continue;
		}
		if(pWireHr && pWireHl){
			SXBSchWire* pNewWire = (SXBSchWire*)(pWireHl->duplicate());
			pNewWire->setX1(pWireHl->x1()+pWireHl->x2()-pj.x());
			pNewWire->setX2(pWireHr->x1()+pWireHr->x2()-pj.x());
			pNewWire->setPlaceNum(opCount);
			m_listMain.push_back(pNewWire);

			
			//���C���[�̏���
			pWireHl->setDeleteNum(opCount);		//0.69
			pWireHr->setDeleteNum(opCount);		//0.69

			//��������0.69�ō폜
			//SXBSchDelObj* pDelObj;
			//pDelObj = new SXBSchDelObj;		//���I�u�W�F�N�g���������邽�߂̏����I�u�W�F�N�g���쐬
			//pWireHl->setRefObj(pDelObj);					//���I�u�W�F�N�g�ɏ����I�u�W�F�N�g��ݒ�
			//pDelObj->setRefObj(pWireHl);					//�����I�u�W�F�N�g�ɋ��I�u�W�F�N�g��ݒ�
			//pDelObj->setPlaceNum(opCount);				
			//m_listMain.push_back(pDelObj);
			//pDelObj = new SXBSchDelObj;						//���I�u�W�F�N�g���������邽�߂̏����I�u�W�F�N�g���쐬
			//pWireHr->setRefObj(pDelObj);					//���I�u�W�F�N�g�ɏ����I�u�W�F�N�g��ݒ�
			//pDelObj->setRefObj(pWireHr);					//�����I�u�W�F�N�g�ɋ��I�u�W�F�N�g��ݒ�
			//pDelObj->setPlaceNum(opCount);				
			//m_listMain.push_back(pDelObj);
			//�����܂�0.69�ō폜
		}
		if(pWireVr && pWireVl){
			SXBSchWire* pNewWire = (SXBSchWire*)(pWireVl->duplicate());
			pNewWire->setY1(pWireVl->y1()+pWireVl->y2()-pj.y());
			pNewWire->setY2(pWireVr->y1()+pWireVr->y2()-pj.y());
			pNewWire->setPlaceNum(opCount);
			m_listMain.push_back(pNewWire);

			//���C���[�̏���
			pWireVl->setDeleteNum(opCount);		//0.69
			pWireVr->setDeleteNum(opCount);		//0.69

			//��������0.69�ō폜
			//SXBSchDelObj* pDelObj;
			//pDelObj = new SXBSchDelObj;		//���I�u�W�F�N�g���������邽�߂̏����I�u�W�F�N�g���쐬
			//pWireVl->setRefObj(pDelObj);					//���I�u�W�F�N�g�ɏ����I�u�W�F�N�g��ݒ�
			//pDelObj->setRefObj(pWireVl);					//�����I�u�W�F�N�g�ɋ��I�u�W�F�N�g��ݒ�
			//pDelObj->setPlaceNum(opCount);				
			//m_listMain.push_back(pDelObj);
			//pDelObj = new SXBSchDelObj;						//���I�u�W�F�N�g���������邽�߂̏����I�u�W�F�N�g���쐬
			//pWireVr->setRefObj(pDelObj);					//���I�u�W�F�N�g�ɏ����I�u�W�F�N�g��ݒ�
			//pDelObj->setRefObj(pWireVr);					//�����I�u�W�F�N�g�ɋ��I�u�W�F�N�g��ݒ�
			//pDelObj->setPlaceNum(opCount);				
			//m_listMain.push_back(pDelObj);
			//�����܂�0.69�ō폜
		}
		pObjIte++;
		continue;
	}
}


//���C���[,�W�����N�V�����̓�������  �X�e�b�v2
//�V�K�W�����N�V�����ŌÂ����C���[�̕������s���B
void SXBSchDoc::tidyUpWireJunction_2(int opCount)
{
	XBSchObjListIterator pObjIte = m_listMain.begin();
	while(pObjIte != m_listMain.end()){
		if(   (*pObjIte)->placeNum()<opCount
			||(*pObjIte)->id() != SXBSchObj::ID_JUNCTION
		    //||(*pObjIte)->layer() != m_editLayer
			||(*pObjIte)->deleteNum()>=0	//20091218
			||(*pObjIte)->refObj()!=NULL){
			pObjIte++;
			continue;
		}
		int nLayer = (*pObjIte)->layer();
		SPoint pj = (*pObjIte)->p1();
		XBSchObjListIterator pObjIteW = m_listMain.begin();
		while(pObjIteW != m_listMain.end()){
			if(   (*pObjIteW)->placeNum() >= opCount
				||(*pObjIteW)->id() != SXBSchObj::ID_WIRE
				//||(*pObjIteW)->layer() != m_editLayer 
				||(*pObjIteW)->layer() != nLayer 
				||(*pObjIteW)->deleteNum()>=0	//20091218
				||(*pObjIteW)->refObj()!=NULL){
				pObjIteW++;
				continue;
			}
			SPoint p1 = (*pObjIteW)->p1();
			SPoint p2 = (*pObjIteW)->p2();
			if(p1.x()==p2.x() && p1.x()== pj.x()){
				if( (p1.y() <= pj.y() && p2.y() <= pj.y())
				 || (p1.y() >= pj.y() && p2.y() >= pj.y())){
					pObjIteW++;
					continue;
				}
			}else if(p1.y()==p2.y() && p1.y()== pj.y()){
				if( (p1.x() <= pj.x() && p2.x() <= pj.x())
				 || (p1.x() >= pj.x() && p2.x() >= pj.x())){
					pObjIteW++;
					continue;
				}
			}else{
				pObjIteW++;
				continue;
			}
			SXBSchWire* pWire1 = (SXBSchWire*)((*pObjIteW)->duplicate());
			SXBSchWire* pWire2 = (SXBSchWire*)((*pObjIteW)->duplicate());
			pWire1->setP2(pj);
			pWire1->setPlaceNum(opCount);
			pWire2->setP1(pj);
			pWire1->setPlaceNum(opCount);
			pWire2->setPlaceNum(opCount);
			
			
			(*pObjIteW)->setDeleteNum(opCount);		//0.69
			//��������0.69�ō폜
			//SXBSchDelObj* pDelObj = new SXBSchDelObj;		//���I�u�W�F�N�g���������邽�߂̏����I�u�W�F�N�g���쐬
			//(*pObjIteW)->setRefObj(pDelObj);					//���I�u�W�F�N�g�ɏ����I�u�W�F�N�g��ݒ�
			//pDelObj->setRefObj(*pObjIteW);					//�����I�u�W�F�N�g�ɋ��I�u�W�F�N�g��ݒ�
			//pDelObj->setPlaceNum(opCount);				
			//m_listMain.push_back(pDelObj);
			//�����܂�0.69�ō폜

			m_listMain.push_back(pWire1);
			m_listMain.push_back(pWire2);
			pObjIteW++;
			continue;
		}
		pObjIte++;
		continue;
	}
}

//���C���[,�W�����N�V�����̓�������  �X�e�b�v3
//�V�K���C���[��S�W�����N�V�����ŕ�������B
void SXBSchDoc::tidyUpWireJunction_3(int opCount)
{
	XBSchObjListIterator pObjIte = m_listMain.begin();
	while(pObjIte != m_listMain.end()){
		if(   (*pObjIte)->placeNum()<opCount
			||(*pObjIte)->id() != SXBSchObj::ID_WIRE
		    //||(*pObjIte)->layer() != m_editLayer 
			||(*pObjIte)->deleteNum()>=0	//20091218
			||(*pObjIte)->refObj()!=NULL){
			pObjIte++;
			continue;
		}
		int nLayer = (*pObjIte)->layer();
		SPoint p1 = (*pObjIte)->p1();
		SPoint p2 = (*pObjIte)->p2();
		XBSchObjListIterator pObjIteJ = m_listMain.begin();
		while(pObjIteJ != m_listMain.end()){
			if( //  (*pObjIteW)->placeNum() >= opCount
				  (*pObjIteJ)->id() != SXBSchObj::ID_JUNCTION
				//||(*pObjIteJ)->layer() != m_editLayer 
				||(*pObjIteJ)->layer() != nLayer 
				||(*pObjIteJ)->deleteNum()>=0	//20091218
				||(*pObjIteJ)->refObj()!=NULL){
				pObjIteJ++;
				continue;
			}
			SPoint pj = (*pObjIteJ)->p1();

			if(p1.x()==p2.x() && p1.x()== pj.x()){
				if( (p1.y() <= pj.y() && p2.y() <= pj.y())
				 || (p1.y() >= pj.y() && p2.y() >= pj.y())){
					pObjIteJ++;
					continue;
				}
			}else if(p1.y()==p2.y() && p1.y()== pj.y()){
				if( (p1.x() <= pj.x() && p2.x() <= pj.x())
				 || (p1.x() >= pj.x() && p2.x() >= pj.x())){
					pObjIteJ++;
					continue;
				}
			}else{
				pObjIteJ++;
				continue;
			}
			SXBSchWire* pWire1 = (SXBSchWire*)((*pObjIte)->duplicate());
			SXBSchWire* pWire2 = (SXBSchWire*)((*pObjIte)->duplicate());
			pWire1->setP2(pj);
			pWire1->setPlaceNum(opCount);
			pWire2->setP1(pj);
			pWire1->setPlaceNum(opCount);
			pWire2->setPlaceNum(opCount);
			
			(*pObjIte)->setDeleteNum(opCount);		//0.69
			//��������0.69�ō폜
			//SXBSchDelObj* pDelObj = new SXBSchDelObj;		//���I�u�W�F�N�g���������邽�߂̏����I�u�W�F�N�g���쐬
			//(*pObjIte)->setRefObj(pDelObj);					//���I�u�W�F�N�g�ɏ����I�u�W�F�N�g��ݒ�
			//pDelObj->setRefObj(*pObjIte);					//�����I�u�W�F�N�g�ɋ��I�u�W�F�N�g��ݒ�
			//pDelObj->setPlaceNum(opCount);				
			//m_listMain.push_back(pDelObj);
			//�����܂�0.69�ō폜

			m_listMain.push_back(pWire1);
			m_listMain.push_back(pWire2);
			break;
		}
		pObjIte++;
		continue;
	}
}






//���C���[,�o�X�̌�������
void SXBSchDoc::tidyUpWireBus(SXBSchLine* pObj)
{
	unsigned int targetID = pObj->id();
	bool horz = (pObj->y1() == pObj->y2()); 
	bool vert = (pObj->x1() == pObj->x2());
	if(horz && vert){
		delete pObj;
		return;
	}

	int k,pl,pr,n;
	if(horz){
		k=pObj->y1();
		pl = pObj->x1();
		pr = pObj->x2();
	}else if(vert){
		k=pObj->x1();
		pl = pObj->y1();
		pr = pObj->y2();
	}else{
		m_listMain.push_back(pObj);
		return;
	}
	if(pl>pr){
		n=pl;pl=pr;pr=n;
	}

	int nLayer = pObj->layer();

	
	//TRACE("Start tidyUpWireBus\n");

	bool bExecJoint = true;
	while(bExecJoint){	
		bExecJoint = false;
		XBSchObjListIterator pObjIte = m_listMain.begin();
		while(pObjIte != m_listMain.end()){
			if(   (*pObjIte)->id() == targetID
				//&&(*pObjIte)->layer() == m_editLayer 
				&&(*pObjIte)->layer() == nLayer
				&&(*pObjIte)->deleteNum()<0
				&&(*pObjIte)->refObj()==NULL){
				int plM,prM;
				SXBSchLine* pObjM = (SXBSchLine*)(*pObjIte);
				if(horz){
					if(pObjM->y1()!=k || pObjM->y2()!=k){
						pObjIte++;
						continue;
					}
					plM = pObjM->x1();
					prM = pObjM->x2();
					if(plM>prM){
						n=plM;plM=prM;prM=n;
					}
					if(prM<pl || pr<plM){
						pObjIte++;
						continue;
					}
					if(plM < pl) pl=plM;
					if(prM > pr) pr=prM;

					(*pObjIte)->setDeleteNum(m_nOpCount);	//0.69
					//��������0.69�ō폜
					//SXBSchDelObj* pDelObj = new SXBSchDelObj;		//���I�u�W�F�N�g���������邽�߂̏����I�u�W�F�N�g���쐬
					//(*pObjIte)->setRefObj(pDelObj);					//���I�u�W�F�N�g�ɏ����I�u�W�F�N�g��ݒ�
					//pDelObj->setRefObj(*pObjIte);					//�����I�u�W�F�N�g�ɋ��I�u�W�F�N�g��ݒ�
					//pDelObj->setPlaceNum(m_nOpCount);				
					//m_listMain.push_back(pDelObj);
					//�����܂�0.69�ō폜
					bExecJoint = true;
				}else{
					if(pObjM->x1()!=k || pObjM->x2()!=k){
						pObjIte++;
						continue;
					}
					plM = pObjM->y1();
					prM = pObjM->y2();
					if(plM>prM){
						n=plM;plM=prM;prM=n;
					}
					if(prM<pl || pr<plM){
						pObjIte++;
						continue;
					}
					if(plM < pl) pl=plM;
					if(prM > pr) pr=prM;

					(*pObjIte)->setDeleteNum(m_nOpCount);	//0.69
					//��������0.69�ō폜
					//SXBSchDelObj* pDelObj = new SXBSchDelObj;		//���I�u�W�F�N�g���������邽�߂̏����I�u�W�F�N�g���쐬
					//(*pObjIte)->setRefObj(pDelObj);					//���I�u�W�F�N�g�ɏ����I�u�W�F�N�g��ݒ�
					//pDelObj->setRefObj(*pObjIte);					//�����I�u�W�F�N�g�ɋ��I�u�W�F�N�g��ݒ�
					//pDelObj->setPlaceNum(m_nOpCount);				
					//m_listMain.push_back(pDelObj);
					//�����܂�0.69�ō폜
					bExecJoint = true;
				}
			}
			pObjIte++;
		}
	}
	if(horz){
		pObj->setX1(pl);
		pObj->setX2(pr);
	}else{
		pObj->setY1(pl);
		pObj->setY2(pr);
	}
	m_listMain.push_back(pObj);
}	


//�W�����N�V�����A�G���g���[�̏d���`�F�b�N���s��
bool SXBSchDoc::checkRedundancy(SXBSchObj* pObj)
{
	XBSchObjListIterator pObjIte = m_listMain.begin();
	//int nLayer = pObj->layer();
	while(pObjIte != m_listMain.end()){
		if(     (*pObjIte)->id() == pObj->id() 
			&& (*pObjIte)->deleteNum()<0
			&& (*pObjIte)->layer() == pObj->layer()  
			&& (*pObjIte)->refObj()==NULL ) {
			switch((*pObjIte)->id()){
				case SXBSchObj::ID_JUNCTION:
					{
						SXBSchJunc* pObjM = (SXBSchJunc*)(*pObjIte);
						if(pObjM->p1()==pObj->p1()){
							TRACE("checkRedundancyJunction() true\n");
							return true;
						}
					}
					break;
				case SXBSchObj::ID_ENTRY:
				case SXBSchObj::ID_BENTRY:
					{
						SXBSchEntry* pObjM = (SXBSchEntry*)(*pObjIte);
						//if(pObjM->p1()==((SXBSchEntry*)pObj)->p1() && pObjM->aspect() == ((SXBSchEntry*)pObj)->aspect()){
						if(pObjM->p1()==((SXBSchEntry*)pObj)->p1() && pObjM->p2()==((SXBSchEntry*)pObj)->p2()){
							return true;
						}
					}
					break;
			}
		}
		pObjIte++;
	}
	return false;
}


////�W�����N�V�����̏d���`�F�b�N���s��
//bool SXBSchDoc::checkRedundancyJunction(SXBSchJunc* pObj)
//{
//	XBSchObjListIterator pObjIte = m_listMain.begin();
//	int nLayer = pObj->layer();
//	while(pObjIte != m_listMain.end()){
//		if(   (*pObjIte)->id() == SXBSchObj::ID_JUNCTION
//		    //&&(*pObjIte)->layer() == m_editLayer 
//			&&(*pObjIte)->layer() == nLayer 
//			&&(*pObjIte)->refObj()==NULL){
//			SXBSchJunc* pObjM = (SXBSchJunc*)(*pObjIte);
//			if(pObjM->p1()==pObj->p1()){
//				TRACE("checkRedundancyJunction() true\n");
//				return true;
//			}
//		}
//		pObjIte++;
//	}
//	return false;
//}


//�ꎞ�f�[�^�����C���f�[�^�Ɉڂ��āA�ꎞ�f�[�^���N���A����
void SXBSchDoc::moveTempDataToMain(bool newPlace,int option)
{


	if(option == RESET_SELECT_BRING_TO_FRONT){
		XBSchObjListIterator pObjIte = m_listTemp.begin();
		while(pObjIte != m_listTemp.end()){
			//�ꎞ�f�[�^�����C���̃��X�g�ɒǉ�
			if((*pObjIte)->qValid()){
				(*pObjIte)->setPlaceNum(m_nOpCount);		//����ԍ����X�V����
				(*pObjIte)->setRefObj(NULL);				//�Q�ƃI�u�W�F�N�g�͋�ɂ���i�L���f�[�^�ɂȂ�j
				(*pObjIte)->resetSelection();				//�I���t���O���N���A����B
				pushBackToMainList(*pObjIte,true);
			}else{
				delete (*pObjIte);
			}
			*pObjIte++;
		}
	}else if(option == RESET_SELECT_BRING_TO_BACK){
		XBSchObjListRevIterator pObjRevIte = m_listTemp.rbegin();
		while(pObjRevIte != m_listTemp.rend()){
			//�ꎞ�f�[�^�����C���̃��X�g�ɒǉ�
			if((*pObjRevIte)->qValid()){
				(*pObjRevIte)->setPlaceNum(m_nOpCount);		//����ԍ����X�V����
				(*pObjRevIte)->setRefObj(NULL);				//�Q�ƃI�u�W�F�N�g�͋�ɂ���i�L���f�[�^�ɂȂ�j
				(*pObjRevIte)->resetSelection();				//�I���t���O���N���A����B
				if((*pObjRevIte)->keepOrder()){
					m_listMain.push_front(*pObjRevIte);
				}else{
					pushBackToMainList(*pObjRevIte,true);
				}
			}else{
				delete (*pObjRevIte);
			}
			*pObjRevIte++;
		}
	}else{
		XBSchObjListIterator pObjMainPos = m_listMain.begin();

		XBSchObjListIterator pObjIte = m_listTemp.begin();
		while(pObjIte != m_listTemp.end()){
			//�ꎞ�f�[�^�����C���̃��X�g�ɒǉ�
			if((*pObjIte)->qValid()){
				SXBSchObj* pRefObj=NULL;
				if(!newPlace) pRefObj = (*pObjIte)->refObj();

				(*pObjIte)->setPlaceNum(m_nOpCount);		//����ԍ����X�V����
				(*pObjIte)->setRefObj(NULL);				//�Q�ƃI�u�W�F�N�g�͋�ɂ���i�L���f�[�^�ɂȂ�j
				(*pObjIte)->resetSelection();				//�I���t���O���N���A����B

				if(pRefObj!=NULL && (*pObjIte)->keepOrder()){
					int searchSize = m_listMain.size();
					int searchCount = 0;
					bool findFlag = false;
					while(searchCount<searchSize && !findFlag){
						if(pObjMainPos == m_listMain.end()){
							pObjMainPos = m_listMain.begin();
						}
						if(*pObjMainPos == pRefObj){
							findFlag = true;
							TRACE("Find restore position. searchCount = %d/%d\n",searchCount,searchSize);
						}
						pObjMainPos++;
						searchCount++;
					}
					if(findFlag){
						m_listMain.insert(pObjMainPos,*pObjIte);
					}else{
						m_listMain.push_back(*pObjIte);
					}
				}else{
					pushBackToMainList(*pObjIte,true);
				}
			}else{
				delete (*pObjIte);
			}
			*pObjIte++;
		}

	}
	m_listTemp.clear();
}



bool SXBSchDoc::resetSelect_1(int option)
{
	TRACE("SXBSchDoc::resetSelect_1(bool bDel)\n");
	m_selectedTypeFlag = 0;
	//�ꎞ�I�u�W�F�N�g�̃��X�g����Ȃ牽�����Ȃ�
	if(m_listTemp.size() == 0){
		m_bTempDirty = false;
		m_bTempNewPlace = false;
		return false;
	}

	bool bModifyMain = false;

	XBSchObjListIterator pObjIte;
	discardRedo();

	if(option == RESET_SELECT_BRING_TO_BACK || option == RESET_SELECT_BRING_TO_FRONT){ 
		m_bTempDirty = true;
	}


///////////////////////////////////////////////////////////////
	if(m_bTempNewPlace){
		m_bValidContinuousSingleSelect = false;
		if(option != RESET_SELECT_DELETE){		//�V�K�ǉ��f�[�^���t�B�b�N�X����
			m_nOpCount++;
			bModifyMain = true;
			moveTempDataToMain(true,option);
			//pObjIte = m_listTemp.begin();
			//while(pObjIte != m_listTemp.end()){
			//	//�ꎞ�f�[�^�����C���̃��X�g�ɒǉ�
			//	if((*pObjIte)->qValid()){
			//		(*pObjIte)->setPlaceNum(m_nOpCount);		//����ԍ����X�V����
			//		(*pObjIte)->setRefObj(NULL);				//�Q�ƃI�u�W�F�N�g�͋�ɂ���i�L���f�[�^�ɂȂ�j
			//		(*pObjIte)->resetSelection();				//�I���t���O���N���A����B
			//		pushBackToMainList(*pObjIte,true);
			//	}
			//	*pObjIte++;
			//}
		}else{			//�V�K�ǉ��f�[�^���폜����
			pObjIte = m_listTemp.begin();
			while(pObjIte != m_listTemp.end()){
				delete (*pObjIte);
				*pObjIte++;
			}
			m_listTemp.clear();
		}
		//m_bTempDirty = false;
		//m_bTempNewPlace = false;
	}else{
		if((option != RESET_SELECT_DELETE) && !m_bTempDirty){	//�I���������A���ɂȂɂ��ύX�������Ȃ������ꍇ
			pObjIte = m_listTemp.begin();
			while(pObjIte != m_listTemp.end()){
				if((*pObjIte)->refObj()){ //�ꎞ�I�u�W�F�N�g���Q�ƃI�u�W�F�N�g�����ꍇ�A�Q�ƃI�u�W�F�N�g�͌��̃f�[�^���w���B
					SXBSchObj* pOldObj = (*pObjIte)->refObj();
					pOldObj->setRefObj(NULL);
				}
				delete (*pObjIte);
				*pObjIte++;
			}
			m_listTemp.clear();
			m_bTempDirty = false;
		}else{				//�I�������f�[�^�ɕύX���������ꍇ
			m_nOpCount++;
			bModifyMain = true;
			m_bValidContinuousSingleSelect = false;

			pObjIte = m_listTemp.begin();
			while(pObjIte != m_listTemp.end()){
				if((*pObjIte)->refObj()){ //�ꎞ�I�u�W�F�N�g���Q�ƃI�u�W�F�N�g�����ꍇ�A�Q�ƃI�u�W�F�N�g�͌��̃f�[�^���w���B
					SXBSchObj* pOldObj = (*pObjIte)->refObj();

					pOldObj->setDeleteNum(m_nOpCount);	//0.69
					pOldObj->setRefObj(NULL); //0.69
					//��������0.69�ō폜
					////�폜�I�u�W�F�N�g�̒ǉ�
					//SXBSchDelObj* pDelObj = new SXBSchDelObj;
					////qDebug("Add DeleteObj");
					//pDelObj->setPlaceNum(m_nOpCount);	
					//pDelObj->setRefObj(pOldObj); //�폜�I�u�W�F�N�g��refObj()�ɂ͑ΏۃI�u�W�F�N�g�ւ̃|�C���^���Z�b�g����
					//pOldObj->setRefObj(pDelObj); //���̃I�u�W�F�N�g�ɂ͍폜�I�u�W�F�N�g�ւ̃|�C���^���Z�b�g����
					//m_listMain.push_back(pDelObj);
					//�����܂�0.69�ō폜

				}
				pObjIte++;
			}
			if(option == RESET_SELECT_DELETE){
				pObjIte = m_listTemp.begin();
				while(pObjIte != m_listTemp.end()){
					delete (*pObjIte);
					pObjIte++;
				}
				m_listTemp.clear();
			}else{
				moveTempDataToMain(false,option);
				//pObjIte = m_listTemp.begin();
				//while(pObjIte != m_listTemp.end()){
				//	//�ꎞ�f�[�^�����C���̃��X�g�ɒǉ�
				//	if((*pObjIte)->qValid()){
				//		(*pObjIte)->setPlaceNum(m_nOpCount);		//����ԍ����X�V����
				//		(*pObjIte)->setRefObj(NULL);				//�Q�ƃI�u�W�F�N�g�͋�ɂ���i�L���f�[�^�ɂȂ�j
				//		(*pObjIte)->resetSelection();				//�I���t���O���N���A����B
				//		if((*pObjIte)->keepOrder()){
				//			pushBackToMainList(*pObjIte,true);
				//		}else{
				//			pushBackToMainList(*pObjIte,true);
				//		}
				//	}else{
				//		//�ύX�����������ʖ����ɂȂ����Ƃ��͈ꎞ�f�[�^������
				//		delete (*pObjIte);
				//	}
				//	*pObjIte++;
				//}
				//m_listTemp.clear();
			}
		}
	}
///////////////////////////////////////////////////////////////

	if(bModifyMain && m_bDoTidyUp){
		tidyUpWireJunction_1(m_nOpCount);
		tidyUpWireJunction_2(m_nOpCount);
		tidyUpWireJunction_3(m_nOpCount);
	}

	m_bSelectForDrag = false;
	m_bTempDirty = false;
	m_bTempNewPlace = false;
	//m_resetSelectOption = RESET_SELECT_NORMAL;


	delOldData(m_nOpCount-50);		//Undo�񐔂̐���
	if(bModifyMain){
		m_bDirty = true;
		return true;
	}else{
		return false;
	}
}



//0.63.02
bool SXBSchDoc::isOnGrid(SPoint pt)
{
	return (pt.x()%10 == 0)&&(pt.y()%10 == 0);
}


//m_rcTempArea �� m_ptTempSnapPoint ���ꎞ�I�u�W�F�N�g�̃��X�g����ݒ肷��B
void SXBSchDoc::setupRcTempArea()
{
	if(m_listTemp.size() == 0)return;
	m_rcTempArea = SRect(SPoint(INT_MAX,INT_MAX),SPoint(INT_MIN,INT_MIN));

	int tempCount = m_listTemp.size();

	XBSchObjListIterator pObjIte = m_listTemp.begin();
	//bool bFind1st = true;
	bool bFindOnGridObject = false;
	while(pObjIte != m_listTemp.end()){
		m_rcTempArea |= (*pObjIte)->area();

		//�I�������I�u�W�F�N�g�̒��ɃI���O���b�h�̂��̂�����΂�����X�i�b�v�|�C���g�Ƃ���B0.63.02
		if(!bFindOnGridObject){
			m_ptTempSnapPoint = (*pObjIte)->snapPoint();
			if(isOnGrid(m_ptTempSnapPoint)){
				bFindOnGridObject = true;
			}
		}
		//if(bFind1st){
		//	m_ptTempSnapPoint = (*pObjIte)->snapPoint();
		//	bFind1st = false;
		//}
		pObjIte++;
	}

	//�����̃I�t�O���b�h�I�u�W�F�N�g�����̏ꍇ�́A�I�t�O���b�h�ł��邱�Ƃ��ێ����� 0.63.02
	if(!bFindOnGridObject && (tempCount>1)){
		m_ptTempSnapPoint.setX((m_ptTempSnapPoint.x()+5)/10*10);
		m_ptTempSnapPoint.setY((m_ptTempSnapPoint.y()+5)/10*10);
	}
}

void SXBSchDoc::setupRotateRect()
{
	if(m_listTemp.size() == 0)return;
	m_rcRotateRect = SRect(SPoint(INT_MAX,INT_MAX),SPoint(INT_MIN,INT_MIN));

	XBSchObjListIterator pObjIte = m_listTemp.begin();
	while(pObjIte != m_listTemp.end()){
		m_rcRotateRect |= (*pObjIte)->rotateRect();
		pObjIte++;
	}
	//qDebug("%d,%d\n",m_rcRotateRect.l(),m_rcRotateRect.r());
}



bool SXBSchDoc::writeMinimumLb3(SWriteCE3& wce3,XBSchObjList* pList,bool bOwnOnly)
{
//typedef list<SCompIndex*> SCompIndexList;
//typedef list<SCompIndex*>::iterator SCompIndexListIterator;

	return true;
}


bool SXBSchDoc::writeCe3(SWriteCE3& wce3,XBSchObjList* pList,bool bAllObj,const SPoint* pptOrigin)
{
	SRect rcSheet(0,0,m_sizeSheet.w(),m_sizeSheet.h());
	if(pList){
		XBSchObjListIterator pObjIte = pList->begin();
		while(pObjIte != pList->end()){
			//if(bAllObj || !(*pObjIte)->refObj()){
			if(bAllObj || (*pObjIte)->deleteNum()<0){
				if(rcSheet.intersect((*pObjIte)->area())){
					(*pObjIte)->writeCe3(wce3,pptOrigin);
				}
			}
			pObjIte++;
		}
	}
	return true;
}


bool SXBSchDoc::readCe3(SReadCE3& rce3,
						XBSchObjList* pList,
						const SPoint* pptOrigin,
						SXBSchSheetInfo* pSheetInfo)
{

	assert(pList!=NULL);
	wstring str(_T(""));
	bool bFirstSheetInfo = true;
	SXBSchObj* pObj;
	while(rce3.ReadRecord(str)!=WEOF){
		if(str == _T("")) break;		//��̃��R�[�h�͖�������
		pObj = NULL;
		if(str == _T("+SHEETINFO") && bFirstSheetInfo && pSheetInfo){
			bFirstSheetInfo = false;
			if(! pSheetInfo->readCe3(rce3,pptOrigin,this)) 
				return false;
		}else if(str == _T("+JUNCTION")){
				pObj = new SXBSchJunc;
		}else if(str == _T("+LABEL")){
				pObj = new SXBSchLabel;
		}else if(str == _T("+COMMENT")){
				pObj = new SXBSchComment;
		}else if(str == _T("+TAG")){
				pObj = new SXBSchTag;
		}else if(str == _T("+WIRE")){
				pObj = new SXBSchWire;
		}else if(str == _T("+BUS")){
				pObj = new SXBSchBus;
		}else if(str == _T("+DASH")){
				pObj = new SXBSchDash;
		}else if(str == _T("+ALINE")){
				pObj = new SXBSchMarkerLine;
		}else if(str == _T("+ENTRY")){
				pObj = new SXBSchEntry;
		}else if(str == _T("+BENTRY")){
				pObj = new SXBSchBusEntry;
		}else if(str == _T("+COMPONENT")){
				pObj = new SXBSchComponent;
		}else if(str == _T("+IMAGEOBJECT")){
				pObj = new SXBSchImageObj;
		}else if(str == _T("-BSCH3_DATA_V.1.0")){
			return true;
		}else{
			if(str[0] != '+') return false;
			str[0]='-';
			if(rce3.SkipTo(str)==WEOF) 
				return false;
			//if(rce3.SkipTo(str.replace(1,1,"-")) != SCsvIStream::GETRECORD_NORMAL)return false;
		}
		if(pObj){
			if(pObj->readCe3(rce3,pptOrigin,this)){
				if(pObj->qValid()){
					pList->push_back(pObj);
				}else{
					delete pObj;
				}
			}else{
				delete pObj;
				return false;
			}
		}
	}
	return true;
}





//�I���̉���
//�I���̉����Ƃ́A
//  �ꎞ�f�[�^���w�����̃I�u�W�F�N�g���w���u�폜�I�u�W�F�N�g�v�����C���̃��X�g�ɒǉ���
//                      �ꎞ�f�[�^�����C���̃��X�g�ɒǉ����邱��
bool SXBSchDoc::resetSelect(int option)
{
	return resetSelect_1(option);
}


//������
void SXBSchDoc::init()
{
	m_bDoTidyUp = true;
	m_bFindCommandActive = false;
	m_bSelectForDrag = false;
	//m_resetSelectOption = RESET_SELECT_NORMAL;
	m_bDirty = false;
	m_bTempDirty = false;
	m_bTempNewPlace = false;
	m_nOpCount = 0;
	m_nRedoActive = -1;
	m_nOldestOpCount = 0;
	m_visibleLayer =0xFF;
	m_editLayer = 0;
	m_strFind =_T("");
	m_bFindMatchWhole = true;
	m_bCaseSeinsitive = true;
	

	m_totalPages = 1;
	m_page = 1;
	m_strProject = _T("");
	m_version = 0;
	m_initPos = 0;

	m_bValidContinuousSingleSelect = false;

	

	XBSchObjListIterator pObjIte;

	pObjIte = m_listMain.begin();

	while(pObjIte != m_listMain.end()){
		delete (*pObjIte);
		pObjIte++;
	}
	m_listMain.clear();

 	pObjIte = m_listTemp.begin();
	while(pObjIte != m_listTemp.end()){
		delete (*pObjIte);
		pObjIte++;
	}
	m_listTemp.clear();
	discardRedo();

//printf("\n");
//printf("SXBSchDoc::init() m_listMain.count()=%d,m_listTemp.count()=%d\n",m_listMain.count(),m_listTemp.count());
}

/*

//�t�@�C������̓ǂݍ���
//����I����0��Ԃ�
//  1:�t�@�C���ǂݍ��݃G���[
//  2:BSCH3�t�@�C���ł͂Ȃ�
int SXBSchDoc::readFile(FILE* fp)
{
	if(fp ==NULL) return 1;
	SReadCE3 rce3;
	rce3.Attach(fp);

	wstring str(_T(""));
	if(rce3.ReadRecord(str)==WEOF){	//��̃t�@�C���̂Ƃ��̏���
		fclose(fp);
		return 0;
	}
	SXBSchSheetInfo sheetInfo;
	if(str != _T("+BSCH3_DATA_V.1.0")) return 2;
//	if(readCe3(rce3,&m_listMain,NULL,&sheetInfo)){
	m_bTempNewPlace = true;
	if(readCe3(rce3,&m_listTemp,NULL,&sheetInfo)){
		resetSelect_1(RESET_SELECT_NORMAL);
		m_sizeSheet.setW(sheetInfo.width());
		m_sizeSheet.setH(sheetInfo.height());
		m_editLayer = sheetInfo.editLayer();
		m_visibleLayer = sheetInfo.visbleLayer();

		m_totalPages = sheetInfo.totalPages();
		m_page = sheetInfo.page();
		m_strProject = sheetInfo.projectName();
		m_version = sheetInfo.dataVersion();
		m_initPos = sheetInfo.initPos();


		m_nOldestOpCount = m_nOpCount;
		m_bDirty = false;
		m_bTempDirty = false;
		return 0;
	}else{
		resetSelect_1(RESET_SELECT_DELETE);
		fclose(fp);
		return 1;
	}
}

*/


//�o�[�W��������(Ver0.80�ȍ~������ȑO��)
int SXBSchDoc::ce3FileCheck(const TCHAR* fname)
{
	FILE* fp;
	char charBuff[1024];
	wchar_t wcharBuff[1024];
	size_t readSize;

	if((fp = _tfopen(fname,_T("rb")))==NULL) return CE3FCHK_ERROR;
	readSize = fread(charBuff,1,1023,fp);
	charBuff[readSize] = 0;
	fclose(fp);
	for(int i=0;i<=readSize;i++){
		wcharBuff[i] = charBuff[i];
	}
	SReadCE3 rce3;
	rce3.Attach(wcharBuff);

	wstring str(_T(""));
	if(rce3.ReadRecord(str)==WEOF){	//��̃t�@�C���̂Ƃ��̏���
			return CE3FCHK_EMPTY;
	}

	if(str != _T("+BSCH3_DATA_V.1.0")){
		return CE3FCHK_ERROR;
	}
	if(rce3.SkipTo(_T("+SHEETINFO"))==WEOF){
		return CE3FCHK_ERROR;
	}
	SXBSchSheetInfo sheetInfo;
	if(! sheetInfo.readCe3(rce3,NULL,NULL)){
		return CE3FCHK_V080_OR_L;	//SheetInfo���������邩������Ȃ��B����SheetInfo��070�܂��͈ȑO�ɂ͂Ȃ��B
	}
	//Data Version 0.80�����̓��[�J���G���R�[�h�ƌ���
	if(sheetInfo.dataVersion()<80){
		return CE3FCHK_V070_OR_E;
	}else{
		return CE3FCHK_V080_OR_L;
	}
}


int SXBSchDoc::readFile(const TCHAR* fname)
{
	init();
	FILE* fp;
	int retval = 0; 
	WCHAR* wcBuff = NULL;
	SReadCE3 rce3;
	SXBSchSheetInfo sheetInfo;


	int filecheck = ce3FileCheck(fname);
	if(filecheck == CE3FCHK_ERROR){
		return 1;
	}else if(filecheck == CE3FCHK_EMPTY){
		return 0;
	}else if(filecheck == CE3FCHK_V070_OR_E){
		//�o�C�i�����[�h�ŊJ���Ȃ����B
		fp=_tfopen(fname,_T("rb"));
		//seek and get file size;
		fseek(fp,0,SEEK_END);
		fpos_t filelength;
		fgetpos(fp,&filelength); 
		fseek(fp,0,SEEK_SET);
		char* buff = new char[filelength+1];
		size_t readSize = fread(buff,1,filelength,fp);
		if(readSize<filelength) goto RdErr;
		buff[filelength]=0;
		int wcBuffSize = ::MultiByteToWideChar(CP_THREAD_ACP, 0, buff, -1, NULL,0);
		wcBuff = new WCHAR[wcBuffSize];
		::MultiByteToWideChar(CP_THREAD_ACP, 0, buff, -1, wcBuff, wcBuffSize);
		delete[]buff;

		rce3.Attach(wcBuff);

	}else{
		if((fp = _tfopen(fname,_T("rt, ccs=UTF-8")))==NULL) return 1;
		rce3.Attach(fp);
	}
	rce3.SkipTo(_T("+BSCH3_DATA_V.1.0"));


	m_bTempNewPlace = true;
	if(readCe3(rce3,&m_listTemp,NULL,&sheetInfo)){
		resetSelect_1(RESET_SELECT_NORMAL);
		m_sizeSheet.setW(sheetInfo.width());
		m_sizeSheet.setH(sheetInfo.height());
		m_editLayer = sheetInfo.editLayer();
		m_visibleLayer = sheetInfo.visbleLayer();
		
		m_totalPages = sheetInfo.totalPages();
		m_page = sheetInfo.page();
		m_strProject = sheetInfo.projectName();
		m_version = sheetInfo.dataVersion();
		m_initPos = sheetInfo.initPos();


		m_nOldestOpCount = m_nOpCount;
		m_bDirty = false;
		m_bTempDirty = false;
		fclose(fp);
		delete[]wcBuff;
		return 0;
	}else{
		resetSelect_1(RESET_SELECT_DELETE);
		fclose(fp);
		delete[]wcBuff;
		return 1;
	}

RdErr:
	fclose(fp);
	delete[]wcBuff;
	return retval;
}




int SXBSchDoc::writeFile(FILE* fp)
{
	if(fp ==NULL) return 1;
	SWriteCE3 wce3;
	wce3.Attach(fp);
	SXBSchSheetInfo sheetInfo;
	sheetInfo.setWidth( m_sizeSheet.w() );
	sheetInfo.setHeight( m_sizeSheet.h() );
	sheetInfo.setEditLayer(m_editLayer);
	sheetInfo.setVisbleLayer(m_visibleLayer);
	sheetInfo.setTotalPages(m_totalPages);
	sheetInfo.setPage(m_page);
	sheetInfo.setProjectName(m_strProject.c_str());
	sheetInfo.setInitPos(m_initPos);


	wce3.WriteRecord(_T("+BSCH3_DATA_V.1.0"));
	wce3.WriteEOL();
	sheetInfo.writeCe3(wce3,NULL);
	writeCe3(wce3,&m_listMain,false,NULL);
	wce3.WriteRecord(_T("-BSCH3_DATA_V.1.0"));
	if(wce3.WriteEOL()==WEOF){
		return 1;
	}else{		
		return 0;
	}
}

//�t�@�C���ւ̏�������
//����I����0��Ԃ�
int SXBSchDoc::writeFile(const TCHAR* fname)
{
	resetSelect(RESET_SELECT_NORMAL);
	FILE* fp;
	if((fp = _tfopen(fname,_T("wt, ccs=UTF-8")))==NULL) return 1;
	fseek( fp, 0, SEEK_SET ) ; //BAD KNOWHOW for removing BOM CODE.
	int retval = writeFile(fp);
	fclose(fp);
	if(retval==0){
		init();
		readFile(fname);
	}
	return retval;
}


//�폜
//�h�L�������g�ɕύX���������Ƃ�true��Ԃ�
bool SXBSchDoc::del()
{
	// >>> 2005/01/30 
	//     �_�[�e�B�t���O�̐ݒ�� resetSelect_1()�̒��ōs���悤�ɕύX
	//	m_bDirty = true;	
	// <<< 2005/01/30
	return resetSelect_1(RESET_SELECT_DELETE);
}

//�؂���
//�h�L�������g�ɕύX���������Ƃ�true��Ԃ�
bool SXBSchDoc::cut()
{
	if(copy()){
		return del();
	}
	return false;
}


//Undo
//�h�L�������g�ɕύX���������Ƃ�true��Ԃ�
bool SXBSchDoc::undo()
{
//printf("SXBSchDoc::undo() %d %d\n",m_nOpCount, m_nOldestOpCount);
TRACE("SXBSchDoc::undo() %d %d\n",m_nOpCount, m_nOldestOpCount);
	bool bResult = 	resetSelect(RESET_SELECT_NORMAL);
	if(m_nOpCount <= m_nOldestOpCount){
		TRACE("exit undo 1\n");
		return bResult;
	}

	int nCount = m_listMain.size();
	if(nCount==0){
		TRACE("exit undo 2\n");
		return bResult;
	}

	if(m_nRedoActive <0){
		m_nRedoActive = m_nOpCount;
	}

	//�폜�I�u�W�F�N�g�̕����ƁA�ǉ��I�u�W�F�N�g�̎��O����ʁX�ɍs�����߂�
	//m_listMain�̃X�L������2��s���Ă���B
	//1��̃X�L�����ł������_���������̂���?

	XBSchObjListIterator pObjIte =m_listMain.begin();
	while(pObjIte!=m_listMain.end()){
		//��������0.69�Œǉ�
		if((*pObjIte)->deleteNum() == m_nOpCount){ //Undo�Ώۂ̍폜�ς݃I�u�W�F�N�g�Ȃ�...
			TRACE("UNDO deleted object\n");
			(*pObjIte)->setDeleteNum(-1);
			SRedoInfo redoInfo;
			redoInfo.m_pObj = *pObjIte;
			redoInfo.m_undoCount = m_nOpCount;
			redoInfo.m_undoType = SRedoInfo::UndoDeleteCopyToStack; 
			m_stackRedoInfo.push(redoInfo);
		}
		pObjIte++;
		//�����܂�0.69�Œǉ�

		//��������0.69�ō폜
		//if((*pObjIte)->placeNum() == m_nOpCount){ //Undo�Ώۂ̃I�u�W�F�N�g�Ȃ�...
		//	if((*pObjIte)->id() == SXBSchObj::ID_DELETEOBJ){		//   �������ꂪ�A�폜�I�u�W�F�N�g�Ȃ�
		//		(*pObjIte)->refObj()->setRefObj(NULL);				//		�폜�I�u�W�F�N�g�̎Q�Ƃ��Ă���I�u�W�F�N�g���폜�O�̂���
		//		SRedoInfo redoInfo;
		//		redoInfo.m_pObj = *pObjIte;
		//		redoInfo.m_undoCount = m_nOpCount;
		//		m_stackRedoInfo.push(redoInfo);
		//		//delete (*pObjIte);									//Undo�Ώۂ̃I�u�W�F�N�g������
		//		pObjIte = m_listMain.erase(pObjIte);				//erase()�ŃJ�����g�m�[�h�͈�i��
		//	}else{													
		//		pObjIte++;
		//	}
		//}else{
		//	pObjIte++;
		//}
		//�����܂�0.69�ō폜

	}

	pObjIte =m_listMain.begin();
	while(pObjIte!=m_listMain.end()){
		if((*pObjIte)->placeNum() == m_nOpCount){ //Undo�Ώۂ̃I�u�W�F�N�g�Ȃ�...
			TRACE("UNDO added object\n");
			SRedoInfo redoInfo;
			redoInfo.m_pObj = *pObjIte;
			redoInfo.m_undoCount = m_nOpCount;
			redoInfo.m_undoType = SRedoInfo::UndoAddMoveToStack;	//0.69
			m_stackRedoInfo.push(redoInfo);
			//delete (*pObjIte);									//Undo�Ώۂ̃I�u�W�F�N�g������
			pObjIte = m_listMain.erase(pObjIte);					//erase()�ŃJ�����g�m�[�h�͈�i��
		}else{
			pObjIte++;
		}
	}


	m_nOpCount--;
TRACE("exit undo 3\n");
	return true;
}

bool SXBSchDoc::redo()
{
	if(!canRedo()) return false;
	m_nOpCount++;
	while(!m_stackRedoInfo.empty()){
		SRedoInfo redoInfo = m_stackRedoInfo.top();
		if(redoInfo.m_undoCount!=m_nOpCount) break;
		m_stackRedoInfo.pop();
		SXBSchObj* pObj= redoInfo.m_pObj;

		//��������0.69�Œǉ�
		if(redoInfo.m_undoType == SRedoInfo::UndoAddMoveToStack){	//Add�����I�u�W�F�N�g��UNDO�ō폜���ăX�^�b�N�Ɉړ������Ƃ�
			m_listMain.push_back(pObj);
		}else{	// redoInfo.m_undoType == SRedoInfo::UndoDeleteCopyToStack Delete�����I�u�W�F�N�g��UNDO�ŕ������Ă����Ƃ��BReDO�ō폜����B
			pObj->setDeleteNum(redoInfo.m_undoCount);
		}

		
		//�����܂�0.69�Œǉ�

		//��������0.69�ō폜
		//if(pObj->id()== SXBSchObj::ID_DELETEOBJ){
		//	pObj->refObj()->setRefObj(pObj);
		//}
		//m_listMain.push_back(pObj);
		//�����܂�0.69�ō폜


	}
	if(m_nRedoActive == m_nOpCount){
		discardRedo();
	}
	return true;
}

void SXBSchDoc::discardRedo()
{
	int count = 0;
	while(!m_stackRedoInfo.empty()){
		SRedoInfo redoInfo = m_stackRedoInfo.top();
		m_stackRedoInfo.pop();
		if(redoInfo.m_undoType == SRedoInfo::UndoAddMoveToStack){	//Add�����I�u�W�F�N�g��UNDO�ō폜���ăX�^�b�N�Ɉړ������Ƃ�
			SXBSchObj* pObj= redoInfo.m_pObj;
			delete pObj;
		}
		count++;
	}

	TRACE("discardRedo() Count:%d  RedoActive:%d\n",count,m_nRedoActive);

	m_nRedoActive = -1;
}

bool SXBSchDoc::canRedo()
{
	return (m_nRedoActive>=0);
}

//���i��1�����P�ƂőI������Ă��邩
const SXBSchComponent* SXBSchDoc::isComponentSelectedSingly()
{
	int n=m_listTemp.size();
	if(n!=1) return false;
	XBSchObjListIterator pObjIte =m_listTemp.begin();
	if((*pObjIte)->id()==SXBSchObj::ID_COMPONENT){
		return (SXBSchComponent*)(*pObjIte);
	}else{
		return NULL;
	}
}

//�J�[�\�������i�̏�ɂ��邩�ǂ���
const SXBSchComponent* SXBSchDoc::isOnComponent(const SPoint& pt)
{
	SXBSchObj* pObjSelected = NULL;

	int maxSelectWeight=0;
	XBSchObjListIterator pObjIte = m_listMain.begin();
	while(pObjIte !=  m_listMain.end()){
		if( (*pObjIte)->layer() == m_editLayer
		  && (*pObjIte)->deleteNum()<0
		  &&(*pObjIte)->refObj()==NULL	//�Q�ƃI�u�W�F�N�g�����݂�����͖̂����ł���
		  &&(*pObjIte)->id() == SXBSchObj::ID_COMPONENT){
			int SelectWeight = (*pObjIte)->qSelectWeight(pt) & 0xFFFF;
			if(SelectWeight > maxSelectWeight){
				maxSelectWeight = SelectWeight;
				pObjSelected = (*pObjIte);
			}
		}
		pObjIte++;
	}

	pObjIte =m_listTemp.begin();
		while(pObjIte !=  m_listTemp.end()){
		if((*pObjIte)->id()==SXBSchObj::ID_COMPONENT){
			int SelectWeight = (*pObjIte)->qSelectWeight(pt) & 0xFFFF;
			if(SelectWeight > maxSelectWeight){
				maxSelectWeight = SelectWeight;
				pObjSelected = (*pObjIte);
			}
		}
		pObjIte++;
	}

	return(SXBSchComponent*)pObjSelected;
}

void SXBSchDoc::updateSelectedTypeFlag()
{
	m_selectedTypeFlag = 0;
	XBSchObjListIterator  pObjIte =m_listTemp.begin();
	while(pObjIte !=  m_listTemp.end()){
		switch((*pObjIte)->id()){
			case SXBSchObj::ID_TAG:
				m_selectedTypeFlag |= (unsigned)SELECTED_TYPE_TAG;
				break;
			case SXBSchObj::ID_LABEL:
				m_selectedTypeFlag |= (unsigned)SELECTED_TYPE_LABEL;
				break;
			case SXBSchObj::ID_COMMENT:
				m_selectedTypeFlag |= (unsigned)SELECTED_TYPE_COMMENT;
				break;
			case SXBSchObj::ID_COMPONENT:
				m_selectedTypeFlag |= (unsigned)SELECTED_TYPE_COMPONENT;
				break;
			case SXBSchObj::ID_MARKER:
				m_selectedTypeFlag |= (unsigned)SELECTED_TYPE_MARKER;
				break;
			case SXBSchObj::ID_DASH:
				m_selectedTypeFlag |= (unsigned)SELECTED_TYPE_DECOLINE;
				break;

		}
		pObjIte++;
	}
}

//�p�[�c���I������Ă��邩
bool SXBSchDoc::isComponentSelected()
{
	if(m_listTemp.size()==0)return false;
	return (m_selectedTypeFlag & (unsigned)SELECTED_TYPE_COMPONENT);
}

//���������I������Ă��邩
bool SXBSchDoc::isDecorationlineSelected()
{
	if(m_listTemp.size()==0)return false;
	return (m_selectedTypeFlag & (unsigned)SELECTED_TYPE_DECOLINE);
}


//�}�[�J�[���C�����I������Ă��邩
bool SXBSchDoc::isMarkerlineSelected()
{
	if(m_listTemp.size()==0)return false;
	return (m_selectedTypeFlag & (unsigned)SELECTED_TYPE_MARKER);
}

//�^�O���I������Ă��邩
bool SXBSchDoc::isTagSelected()
{
	if(m_listTemp.size()==0)return false;
	return (m_selectedTypeFlag & (unsigned)SELECTED_TYPE_TAG);
}

//���x�����I������Ă��邩
bool SXBSchDoc::isLabelSelected()
{
	if(m_listTemp.size()==0)return false;
	return (m_selectedTypeFlag & (unsigned)SELECTED_TYPE_LABEL);
}

//�R�����g���I������Ă��邩
bool SXBSchDoc::isCommentSelected()
{
	if(m_listTemp.size()==0)return false;
	return (m_selectedTypeFlag & (unsigned)SELECTED_TYPE_COMMENT);
}


//Attribute�ύX���\��?
bool SXBSchDoc::canAttribute()
{
	int n=m_listTemp.size();
	if(n!=1) return false;
	XBSchObjListIterator pObjIte =m_listTemp.begin();
	return (*pObjIte)->canAttribute();
}

//�R�s�[�\���H
bool SXBSchDoc::canCopy()
{
	return (m_listTemp.size()>0);
}

//�폜�\���H
bool SXBSchDoc::canDel()
{
	return (m_listTemp.size()>0);
}

//�؂���\���H
bool SXBSchDoc::canCut()
{
	return (m_listTemp.size()>0);
}


//Undo�\���H
bool SXBSchDoc::canUndo()
{
	return (m_nOpCount > m_nOldestOpCount);
}



//Rotate
bool SXBSchDoc::rotate()
{
	if(!canRotate()) return false;

	setupRotateRect();

	int n = m_listTemp.size();
	XBSchObjListIterator pObjIte = m_listTemp.begin();
	if(n == 1){
		(*pObjIte)->rotate();
	}else{
		SPoint ptCenter = m_rcRotateRect.center();
		ptCenter.setX(((ptCenter.x()+5)/10)*10);
		ptCenter.setY(((ptCenter.y()+5)/10)*10);
		//int x=(m_rcRotateRect.l()+m_rcRotateRect.r())/2;
		//int y=(m_rcRotateRect.t()+m_rcRotateRect.bottom())/2;
		//SPoint ptCenter(x,y);
		while(pObjIte != m_listTemp.end()){
			(*pObjIte)->rotate(ptCenter);
			pObjIte++;
		}
	}
	setupRcTempArea();

	// >>> 2005/01/30 
	//	�S�̂���ꎞ�f�[�^�̃_�[�e�B�ɕύX
	//m_bDirty = true;
	m_bTempDirty = true;
	// <<< 2005/01/30 
	
	return true;
}

//���C���[�̈ړ�
bool SXBSchDoc::moveToLayer(int nLayer)
{
	XBSchObjListIterator pObjIte = m_listTemp.begin();
	while(pObjIte != m_listTemp.end()){
		(*pObjIte)->setLayer(nLayer);
		pObjIte++;
	}
	// >>> 2005/01/30 
	//	�S�̂���ꎞ�f�[�^�̃_�[�e�B�ɕύX
	//m_bDirty = true;
	m_bTempDirty = true;
	// <<< 2005/01/30 
	return true;
}


//Mirror
bool SXBSchDoc::mirror()
{
	if(!canMirror()) return false;

	setupRotateRect();

	int n = m_listTemp.size();
	XBSchObjListIterator pObjIte = m_listTemp.begin();
	if(n == 1){
		(*pObjIte)->mirrorH();
	}else{
		int nCenter = (m_rcRotateRect.l()+m_rcRotateRect.r())/2;
		while(pObjIte != m_listTemp.end()){
			(*pObjIte)->mirrorH(nCenter);
			pObjIte++;
		}
	}
	setupRcTempArea();

	// >>> 2005/01/30 
	//	�S�̂���ꎞ�f�[�^�̃_�[�e�B�ɕύX
	//m_bDirty = true;
	m_bTempDirty = true;
	// <<< 2005/01/30 
	return true;
}


//Rotate�\���H
bool SXBSchDoc::canRotate()
{
	//if(m_bSelectForDrag) return false;

	int n=m_listTemp.size();
	if(n==0) return false;
	if(n==1){
		XBSchObjListIterator pObjIte =m_listTemp.begin();
		return (*pObjIte)->canRotateSingleSelected();
	}
	return true;
}

//Mirror�\���H
bool SXBSchDoc::canMirror()
{
	//if(m_bSelectForDrag) return false;

	int n=m_listTemp.size();
	if(n==0) return false;
	if(n==1){
		XBSchObjListIterator pObjIte =m_listTemp.begin();
		return (*pObjIte)->canMirrorSingleSelected();
	}
	return true;
}

//EditPoints���\��
bool SXBSchDoc::canEditPoints()
{
	//if(m_bSelectForDrag) return false;
	int n=m_listTemp.size();
	if(n==0) return false;
	if(n==1){
		XBSchObjListIterator pObjIte =m_listTemp.begin();
		if((*pObjIte)->id() == SXBSchObj::ID_DASH){
			// 2010/05/13
			unsigned selflag = (*pObjIte)->selection();
			if(selflag == 0){
				(*pObjIte)->setSelection(SXBSchObj::SELECT_P1 | SXBSchObj::SELECT_P2);
			}
			return true;
		}
	}
	return false;
}


//�����̕ύX���\���H
bool SXBSchDoc::canChangeOrder()
{
	int n=m_listTemp.size();
	if(n==0) return false;
	XBSchObjListIterator pObjIte =m_listTemp.begin();
	while(pObjIte != m_listTemp.end()){
		if((*pObjIte)->keepOrder()) return true;
		pObjIte++;
	}
	return false;
}



//�Â������f�[�^�̍폜
//   ���C���̃��X�g�̐擪����X�L�������āA�w�肵���l�̑���J�E���g�l
// ��菬���Ȓl�ł̍폜���s�����I�u�W�F�N�g����������B
//   �폜�����I�u�W�F�N�g�Ƃ���ɑΉ�����폜�I�u�W�F�N�g�̃��X�g�ł�
// �ʒu�ɒ��ڂ���ƁA���Ȃ炸�A�폜�����I�u�W�F�N�g�̕����擪�ɋ߂���
// ����ɑ��݂���B
void SXBSchDoc::delOldData(int nOpCount)
{
	bool bDel;
	resetSelect(RESET_SELECT_NORMAL);
	if(nOpCount <= m_nOldestOpCount) return;
	
	int nDelObj=0;
	int nDelDelObj=0;


	XBSchObjListIterator pObjIte = m_listMain.begin();
	while(pObjIte!=m_listMain.end()){
		bDel = false;
		
		//��������0.69�Œǉ�
		if((*pObjIte)->deleteNum()>=0 && (*pObjIte)->deleteNum()<nOpCount){
			bDel = true;
			nDelObj++;
		}
		//�����܂�0.69�Œǉ�

		//��������0.69�ō폜
		//if((*pObjIte)->id() != SXBSchObj::ID_DELETEOBJ){
		//	if((*pObjIte)->refObj()!=NULL){
		//		SXBSchObj* pDelObj = (*pObjIte)->refObj();
		//		if(pDelObj->placeNum() < nOpCount){
		//			bDel = true;
		//			nDelObj++;
		//		}
		//	}
		//}else{
		//	if((*pObjIte)->placeNum() < nOpCount){
		//		bDel = true;
		//		nDelDelObj++;
		//	}
		//}
		//�����܂�0.69�ō폜

		if(bDel){
			delete (*pObjIte);						//Undo�Ώۂ̃I�u�W�F�N�g������
			pObjIte = m_listMain.erase(pObjIte);	//erase()�ŃJ�����g�m�[�h�͈�i��
		}else{
			pObjIte++;
		}
	}
	if(nOpCount > m_nOpCount) nOpCount = m_nOpCount;
	m_nOldestOpCount = nOpCount;

	return;
}

//�w�肵�����W���ꎞ�f�[�^�̏ォ�ǂ���
int SXBSchDoc::isOnTempData(const SPoint& pt,int nOption, SXBSchObj** ppObj,int* pOption)
{
	if(m_listTemp.size() == 0) return 0;
	//if((m_listTemp.size() == 1)&&(m_bSelectForDrag==false)){

	int retVal = 0;



	SXBSchObj* pRetObj = NULL;
	int retOption = -1;
	int nEditablePointInfo;

	if(m_listTemp.size() == 1){
		XBSchObjListIterator pObjIte = m_listTemp.begin();
		if(ppObj!=NULL){
			pRetObj = *pObjIte;
		}
		if((*pObjIte)->id()==SXBSchObj::ID_COMPONENT){
			//int val= (*pObjIte)->qSelectWeight(pt) & (ON_OBJ|ON_NAME|ON_NUM);//|ON_COMMENTWIDTH);
			//if(val)return val;
			//retVal= (*pObjIte)->qSelectWeight(pt) & (ON_OBJ|ON_NAME|ON_NUM);//|ON_COMMENTWIDTH);
			retVal= (*pObjIte)->isEditablePoint(pt,nEditablePointInfo);
		}else if((*pObjIte)->id()==SXBSchObj::ID_DASH && (nOption & EDITPOINTS_MODE)){
			retVal |= (*pObjIte)->isEditablePoint(pt,nEditablePointInfo);
			retOption = nEditablePointInfo;
			//SXBSchDash* pDash = (SXBSchDash*)(*pObjIte);
			//SRect rc = SRect(pt.x()-2,pt.y()-2,4,4);
			//unsigned selFlag = pDash->selection();

			//if(selFlag & SXBSchObj::SELECT_P2){
			//	if(rc.intersect(pDash->p2())){
			//		retVal |= ON_NODE;
			//		retOption = SXBSchLine::EDITPOINT_P2;
			//	}
			//}
			//if(selFlag & SXBSchObj::SELECT_P1){
			//	if(rc.intersect(pDash->p1())){
			//		retVal |= ON_NODE;
			//		retOption = SXBSchLine::EDITPOINT_P1;
			//	}
			//}

			//if(pDash->curve()){
			//	if(selFlag & SXBSchObj::SELECT_P2){
			//		if(rc.intersect(pDash->Ct2())){
			//			retVal |= ON_CTRLPOINT;
			//			retOption = SXBSchLine::EDITPOINT_C2;
			//		}
			//	}
			//	if(selFlag & SXBSchObj::SELECT_P1){
			//		if(rc.intersect(pDash->Ct1())){
			//			retVal |= ON_CTRLPOINT;
			//			retOption = SXBSchLine::EDITPOINT_C1;
			//		}
			//	}
			//}
		}
	}
	if(retVal==0){
		if(m_rcTempArea.intersect(pt)){
			retVal = ON_OBJ;
		}
	}

	if(ppObj!=NULL){
		*ppObj= pRetObj;
	}
	if(pOption!=NULL){
		*pOption = retOption;
	}

	return retVal;
}

//�ꎞ�f�[�^���ړ�����
void SXBSchDoc::moveTempData(int offsetx,int offsety)
{
	XBSchObjListIterator pObjIte = m_listTemp.begin();
	while(pObjIte != m_listTemp.end()){
		(*pObjIte)->move(offsetx,offsety);
		pObjIte++;
	}
	m_rcTempArea+=SPoint(offsetx,offsety);
	m_ptTempSnapPoint.setX(m_ptTempSnapPoint.x()+offsetx);
	m_ptTempSnapPoint.setY(m_ptTempSnapPoint.y()+offsety);
	// >>> 2005/01/30 
	//	�S�̂���ꎞ�f�[�^�̃_�[�e�B�ɕύX
	//m_bDirty = true;
	m_bTempDirty = true;
	// <<< 2005/01/30 
}

//�w�肵���_�Ƀs���A�W�����N�V����������������[�������C���[��z�u����
bool SXBSchDoc::selectForDragPin(int scanMax,const SPoint pt)
{
//	if(!m_bDoTidyUp) return false;
	XBSchObjListIterator pObjIte = m_listMain.begin();
	while(scanMax>0 && pObjIte != m_listMain.end()){
		if(   (*pObjIte)->layer() == m_editLayer
		   //&& (*pObjIte)->id()!=SXBSchObj::ID_JUNCTION
		   && (*pObjIte)->id()!=SXBSchObj::ID_WIRE
		   && (*pObjIte)->deleteNum()<0
		   && (*pObjIte)->refObj()==NULL){	//�Q�ƃI�u�W�F�N�g�����݂�����͖̂����ł���
			bool bFind = false;
			if((*pObjIte)->id()==SXBSchObj::ID_JUNCTION){
				if((*pObjIte)->p1()==pt){
					bFind = true;
				}
			}else{
				int pincount=(*pObjIte)->pinCount();
				for(int n=0;n<pincount;n++){
					SPoint pinpt=(*pObjIte)->pinPosition(n);
					if(pinpt == pt){
						bFind = true;
						break;
					}
				}
			}
			if(bFind){
				SXBSchWire* pWire = new SXBSchWire;
				pWire->setP1(pt);
				pWire->setP2(pt);
				pWire->setLayer(m_editLayer);
				pWire->setSelection(SXBSchObj::SELECT_P1);
				m_listWorkWire.push_back(pWire);
				return true;
			}
		}
		pObjIte++;
		scanMax--;
	}
	return false;
}

//�͈͓��̐ڍ��_�̑I��
void SXBSchDoc::selectForDragJunction(int scanMax,const SRect& rc)
{
	XBSchObjListIterator pObjIte = m_listMain.begin();
	while(scanMax>0 && pObjIte != m_listMain.end()){
		if(   (*pObjIte)->layer() == m_editLayer
		   && (*pObjIte)->id()==SXBSchObj::ID_JUNCTION
		   && (*pObjIte)->deleteNum()<0
		   && (*pObjIte)->refObj()==NULL){	//�Q�ƃI�u�W�F�N�g�����݂�����͖̂����ł���
			unsigned nSelStatus = (*pObjIte)->testSelectionForDrag(rc);
			if(nSelStatus){
				SXBSchObj* pNewObj = (*pObjIte)->duplicate();	//�I���I�u�W�F�N�g�̕���
				(*pObjIte)->setRefObj(pNewObj);					//���I�u�W�F�N�g�ɕ����I�u�W�F�N�g��ݒ�
				pNewObj->setRefObj(*pObjIte);					//�V�I�u�W�F�N�g�Ɍ��I�u�W�F�N�g��ݒ�
				pNewObj->setSelection(nSelStatus);				//�V�I�u�W�F�N�g�ɑI���t���O��ݒ�
				m_listTemp.push_back(pNewObj);					//�V�I�u�W�F�N�g���ꎞ�f�[�^�ɕۊ�
			}
		}
		*pObjIte++;
		scanMax--;
	}
}

//���ׂĂ̐ڍ��_�� m_listWorkJunc �ɒ��o����B
void SXBSchDoc::pickupJunction(const SRect& )
{
	XBSchObjListIterator pObjIte = m_listMain.begin();
	while(pObjIte != m_listMain.end()){
		if(    (*pObjIte)->layer() == m_editLayer  
			&& (*pObjIte)->id()==SXBSchObj::ID_JUNCTION
			&& (*pObjIte)->deleteNum()<0
			&& (*pObjIte)->refObj()==NULL){	//�Q�ƃI�u�W�F�N�g�����݂�����͖̂����ł���
			m_listWorkJunc.push_back(*pObjIte);					//�I���O�̐ڍ��_����ƃG���A�ɕۊ�
		}
		*pObjIte++;
	}
}


void SXBSchDoc::setNcPinFlag()
{
	TRACE("**SXBSchDoc::setNcPinFlag()**\n");

	SPointList	wireEndList;
	SPoint pt;
	//���ׂẴ��C���[�̒[�_�̗�
	//NC�s���t���O�̃v���Z�b�g
	XBSchObjListIterator pObjIte = m_listMain.begin();
	while(pObjIte != m_listMain.end()){
		if((*pObjIte)->refObj()!=NULL || (*pObjIte)->deleteNum()>=0){	//20091218

		}else if((*pObjIte)->id()==SXBSchObj::ID_WIRE){
			pt=(*pObjIte)->p1();
			wireEndList.push_back(pt);
			pt=(*pObjIte)->p2();
			wireEndList.push_back(pt);
		}else if((*pObjIte)->id()==SXBSchObj::ID_COMPONENT){
			SXBSchComponent* pComp =(SXBSchComponent*)(*pObjIte);
			pComp->presetPinNC();
		}
		*pObjIte++;
	}

	pObjIte = m_listMain.begin();
	while(pObjIte != m_listMain.end()){
		if(    (*pObjIte)->refObj()==NULL
			&& (*pObjIte)->deleteNum()<0
			&& (*pObjIte)->id()==SXBSchObj::ID_COMPONENT){
			SXBSchComponent* pComp1 =(SXBSchComponent*)(*pObjIte);
			int pinCount = pComp1->pinCount();
			int i;
			for(i=0;i<pinCount;i++){
				if(!pComp1->pinNC(i)) continue;
				SPoint ptPinEnd = pComp1->pinPosition(i);
				SPointListIterator ptIte = wireEndList.begin();
				//���C���[�[�_�Ƃ̏ƍ�
				while(ptIte != wireEndList.end()){
					if(ptPinEnd == (*ptIte)){
						pComp1->setPinNC(i,false);
						break;
					}
					ptIte++;
				}
				//�s�����m�̏ƍ�
				XBSchObjListIterator pObjIte2nd = pObjIte;
				pObjIte2nd++;
				while(pObjIte2nd != m_listMain.end()){
					if((*pObjIte2nd)->refObj()!=NULL || (*pObjIte2nd)->deleteNum()>=0){

					}else if((*pObjIte2nd)->id()==SXBSchObj::ID_COMPONENT){
						SXBSchComponent* pComp2 =(SXBSchComponent*)(*pObjIte2nd);
						int pinCount2nd = pComp2->pinCount();
						int j;
						for(j=0;j<pinCount2nd;j++){
							if(ptPinEnd == pComp2->pinPosition(j)){
								pComp1->setPinNC(i,false);
								pComp2->setPinNC(j,false);
								break;
							}
						}
					}else if((*pObjIte2nd)->id()==SXBSchObj::ID_TAG){
						SXBSchTag* pTag=(SXBSchTag*)(*pObjIte2nd);
						if(ptPinEnd == pTag->pinPosition(0)){
								pComp1->setPinNC(i,false);
						}else if(ptPinEnd == pTag->pinPosition(1)){
								pComp1->setPinNC(i,false);
						}
					}
					pObjIte2nd++;
				}
			}		
		}
		pObjIte++;
	}
}


//�h���b�O�̂��߂̐ڍ��_���C���[�ȊO�̑I��
void SXBSchDoc::selectForDragExceptJuncWire(int scanMax,const SRect& rc)
{
	int scan=0;
	XBSchObjListIterator pObjIte = m_listMain.begin();
	while(scan<scanMax && pObjIte != m_listMain.end()){
		if( (*pObjIte)->layer() == m_editLayer
		  && (*pObjIte)->deleteNum()<0
		  &&(*pObjIte)->id()!=SXBSchObj::ID_JUNCTION 
		  &&(*pObjIte)->id()!=SXBSchObj::ID_WIRE
		  &&(*pObjIte)->refObj()==NULL){
			unsigned nSelStatus = (*pObjIte)->testSelectionForDrag(rc);
			if(nSelStatus){
				SXBSchObj* pNewObj = (*pObjIte)->duplicate();	//�I���I�u�W�F�N�g�̕���
				(*pObjIte)->setRefObj(pNewObj);					//���I�u�W�F�N�g�ɕ����I�u�W�F�N�g��ݒ�
				pNewObj->setRefObj(*pObjIte);					//�V�I�u�W�F�N�g�Ɍ��I�u�W�F�N�g��ݒ�
				pNewObj->setSelection(nSelStatus);				//�V�I�u�W�F�N�g�ɑI���t���O��ݒ�
				m_listTemp.push_back(pNewObj);					//�V�I�u�W�F�N�g���ꎞ�f�[�^�ɕۊ�
				if(pNewObj->id()==SXBSchObj::ID_TAG || pNewObj->id()==SXBSchObj::ID_COMPONENT){
					m_listWorkTagComponent.push_back(pNewObj);
				}
			}
		}
		*pObjIte++;
		scan++;
	}
}

//�����̑I����ɁA�V���Ȕ͈͂�ǉ��I�����ꂽ�^�O�A���i�̃s���ɐڑ����ꂽ���I��[�_���Ȃ����`�F�b�N
void SXBSchDoc::selectForDragReCheckSelectedLine(const SRect& rc)
{
	XBSchObjListIterator pTempObjIte = m_listTemp.begin();
	while(pTempObjIte != m_listTemp.end()){
		if( (*pTempObjIte)->id()==SXBSchObj::ID_WIRE || 
			(*pTempObjIte)->id()==SXBSchObj::ID_BUS  || 
			(*pTempObjIte)->id()==SXBSchObj::ID_DASH ||
			(*pTempObjIte)->id()==SXBSchObj::ID_MARKER){

			unsigned selFlag = (*pTempObjIte)->selection();
			unsigned newFlag = (*pTempObjIte)->testSelectionForDrag(rc);
			(*pTempObjIte)->setSelection(selFlag | newFlag);
		}
		pTempObjIte++;
	}



	XBSchObjListIterator pObjIte = m_listWorkTagComponent.begin();
	while(pObjIte != m_listWorkTagComponent.end()){
		int pincount=(*pObjIte)->pinCount();
		for(int n=0;n<pincount;n++){
			SPoint point=(*pObjIte)->pinPosition(n);
			pTempObjIte = m_listTemp.begin();
			while(pTempObjIte != m_listTemp.end()){
				if((*pTempObjIte)->id()==SXBSchObj::ID_WIRE){
					unsigned selFlag = (*pTempObjIte)->selection();
					if(((selFlag & SXBSchObj::SELECT_P1)==0) && (*pTempObjIte)->p1()==point){
						selFlag |= SXBSchObj::SELECT_P1;
					}
					if(((selFlag & SXBSchObj::SELECT_P2)==0) && (*pTempObjIte)->p2()==point){
						selFlag |= SXBSchObj::SELECT_P2;
					}
					(*pTempObjIte)->setSelection(selFlag);
				}

				pTempObjIte++;
			}
		}
		pObjIte++;
	}
}


//�h���b�O�̂��߂̃s�������I��
void SXBSchDoc::selectForDragPinConnection(int scanMax,const SRect& rrc,bool bCutOffPinWire)
{
	SRect rc;
	rc.setL(rrc.l()+14);
	rc.setT(rrc.t()+14);
	rc.setR(rrc.r()-14);
	rc.setB(rrc.b()-14);


	//�I��͈͊O�̕��i�Ɛڑ�����Ă���\���̂���|�C���g��񋓂���B
	SPointEmum pointEnum;
	SPoint point;
	
	XBSchObjListIterator pObjIte;

	if(!bCutOffPinWire){
		pObjIte = m_listWorkWire.begin();
		while(pObjIte != m_listWorkWire.end()){
			unsigned selflag = (*pObjIte)->selection();
			if(selflag & SXBSchObj::SELECT_P1){
				point = (*pObjIte)->p1();
				if(!rc.intersect(point)){
					pointEnum.addSPoint(point);
				}
			}
			if(selflag & SXBSchObj::SELECT_P2){
				point = (*pObjIte)->p2();
				if(!rc.intersect(point)){
					pointEnum.addSPoint(point);
				}
			}
			pObjIte++;
		}
	}

	int tempSize = m_listTemp.size();
//	pObjIte = m_listTemp.begin();
//	while(pObjIte != m_listTemp.end()){

	pObjIte = m_listWorkTagComponent.begin();
	while(pObjIte != m_listWorkTagComponent.end()){
		//if( (*pObjIte)->id()==SXBSchObj::ID_COMPONENT 
		//	||(*pObjIte)->id()==SXBSchObj::ID_TAG
		//	){
			int pincount=(*pObjIte)->pinCount();
			for(int n=0;n<pincount;n++){
				point=(*pObjIte)->pinPosition(n);
				if(!rc.intersect(point)){
					pointEnum.addSPoint(point);
				}
			}
		//}
		pObjIte++;
	}


	SPointListIterator itePoint=pointEnum.m_list.begin();
	while(itePoint!=pointEnum.m_list.end()){
		point = (*itePoint); 

		if(m_bDoTidyUp){
			if(!selectForDragPin(scanMax,point)){
				SRect rcPin(point,point);
				selectForDragWire(scanMax,rcPin);
			}
		}else{
			SRect rcPin(point,point);
			selectForDragJunction(scanMax,rcPin);
			selectForDragWire(scanMax,rcPin);
		}
		itePoint++;
	}

//	int tempSize = m_listTemp.size();
//	XBSchObjListIterator pObjIte = m_listTemp.begin();
//	int scan = 0;
//	while(scan<tempSize && pObjIte != m_listTemp.end()){
//		if( (*pObjIte)->id()!=SXBSchObj::ID_JUNCTION 
//			&&(*pObjIte)->id()!=SXBSchObj::ID_WIRE){
//
//			int pincount=(*pObjIte)->pinCount();
//			//qDebug("SXBSchDoc::selectForDragExceptJuncWire()  PINCOUNT=%d",pincount);
//			for(int n=0;n<pincount;n++){
//				SPoint pt=(*pObjIte)->pinPosition(n);
//				if(!rc.intersect(pt)){
//					if(m_bDoTidyUp){
//						if(!selectForDragPin(scanMax,pt)){
//							SRect rcPin(pt,pt);
//							//selectForDragJunction(scanMax,rcPin);
//							selectForDragWire(scanMax,rcPin);
//						}
//					}else{
//						SRect rcPin(pt,pt);
//						selectForDragJunction(scanMax,rcPin);
//						selectForDragWire(scanMax,rcPin);
//					}
//				}
//			}
//		}
//		pObjIte++;
//		scan++;
//	}
}


//�h���b�O�̂��߂̃��C���[�I���B
void SXBSchDoc::selectForDragWire(int scanMax,const SRect& rc)
{
	SXBSchWire* pNewWire[5];

	int workWireCount= m_listWorkWire.size();	//m_listWorkWire�͑����邪�A���� rc�Œǉ��������͍̂ăX�L�������Ȃ��B
												//selectForDrag�ōŏ��͈͎̔w��ł̃��C���[�I���ł�workWireCount��0�ɂȂ�B

	XBSchObjListIterator pObjIte = m_listMain.begin();
	while(scanMax>0 && pObjIte != m_listMain.end()){
		if( (*pObjIte)->layer() == m_editLayer
		  &&(*pObjIte)->id()==SXBSchObj::ID_WIRE
		  &&(*pObjIte)->deleteNum()<0
		  &&(*pObjIte)->refObj()==NULL){	//�Q�ƃI�u�W�F�N�g�����݂�����͖̂����ł���
			SXBSchWire* pObj= (SXBSchWire*)(*pObjIte);
			int divcount = divideWire(pObj,rc,pNewWire);
			if(divcount>0){
				(*pObjIte)->setDeleteNum(m_nOpCount+1);		//0.69
				//��������0.69�ō폜
				//SXBSchDelObj* pDelObj = new SXBSchDelObj;		//���I�u�W�F�N�g���������邽�߂̏����I�u�W�F�N�g���쐬
				//(*pObjIte)->setRefObj(pDelObj);					//���I�u�W�F�N�g�ɏ����I�u�W�F�N�g��ݒ�
				//pDelObj->setRefObj(*pObjIte);					//�����I�u�W�F�N�g�ɋ��I�u�W�F�N�g��ݒ�
				//pDelObj->setPlaceNum(m_nOpCount+1);				
				//pushBackToMainList(pDelObj,false);
				//�����܂�0.69�ō폜
				
				//20060604				m_listMain.push_back(pDelObj);					//�����I�u�W�F�N�g�����C���f�[�^�ɕۊ�
				for(int n=0;n<divcount;n++){
					m_listWorkWire.push_back(pNewWire[n]);
				}
			}
		}
		pObjIte++;
		scanMax--;
	}

	pObjIte = m_listWorkWire.begin();
	while(workWireCount>0 && pObjIte != m_listWorkWire.end()){
		if(    (*pObjIte)->deleteNum()<0
			&& (*pObjIte)->refObj()==NULL){	//�Q�ƃI�u�W�F�N�g�����݂�����͖̂����ł���
			SXBSchWire* pObj= (SXBSchWire*)(*pObjIte);
			int divcount = divideWire(pObj,rc,pNewWire);
			if(divcount>0){
				(*pObjIte)->setRefObj(pNewWire[0]);
				for(int n=0;n<divcount;n++){
					m_listWorkWire.push_back(pNewWire[n]);
				}
			}
		}
		pObjIte++;
		workWireCount--;
	}

}


//�h���b�O�I���ł̓��C���[�̕�����duplicate()�ł͂Ȃ��AdivideWire()�ōs���B
// �������ꂽ�I�u�W�F�N�g�ɂ͑I���t���O�������ݒ肳���B
//���C���[���h���b�O�ΏۂłȂ��ꍇ�͕������Ȃ��B
//�h���b�O�ΏۂƔ�h���b�O�Ώۂɕ�������ꍇ�̓h���b�O�Ώۂ̂��̂�擪�Ɋi�[����B
int SXBSchDoc::divideWire(SXBSchWire* swire,const SRect& rc,SXBSchWire** dwire)
{
	int retval=0;
	bool vFlag;
	int fixval,jfix,jp;
	int p0,p1,p2,p3,p4,p5;
	int	boundaryMin,boundaryMax;
	unsigned int nSelStatus;
	unsigned int nOldSelStatus;
	int nLayer = swire->layer();

	if(!swire->qValid()) return 0;

	nOldSelStatus = (swire->selection() & (SXBSchObj::SELECT_P1|SXBSchObj::SELECT_P2));
	nSelStatus = swire->testSelectionForDrag(rc);

	//��x�I���������̂��ăX�L�������邷��ꍇ�̃`�F�b�N
	if(nOldSelStatus){
		swire->setSelection(nOldSelStatus | nSelStatus);
		return 0;
	}

	nSelStatus = swire->testSelectionForDrag(rc);
	if((nSelStatus&(SXBSchObj::SELECT_P1|SXBSchObj::SELECT_P2)) ==SXBSchObj::SELECT_ALL){	//���[�_���͈͓��̂Ƃ�
		dwire[0] = (SXBSchWire*)(swire->duplicate());
		dwire[0]->setSelection(nSelStatus);
		return 1;
	}
	if(swire->x1()==swire->x2()){	//�������̂Ƃ�
		if(swire->x1()<rc.l() || swire->x1()>rc.r()) return 0;	//���E�͈͊O
		if(swire->y1()<rc.t() && swire->y2()<rc.t()) return 0;	//��ɔ͈͊O
		if(swire->y1()>rc.b() && swire->y2()>rc.b()) return 0;	//���ɔ͈͊O
		vFlag = true;
		fixval = swire->x1();
		p0 = swire->y1();
		p5 = swire->y2();
		boundaryMin = rc.t();
		boundaryMax = rc.b();
	}else if(swire->y1()==swire->y2()){	//�������̂Ƃ�
		if(swire->y1()<rc.t() || swire->y1()>rc.b()) return 0;	//�㉺�͈͊O
		if(swire->x1()<rc.l() && swire->x2()<rc.l()) return 0;	//���ɔ͈͊O
		if(swire->x1()>rc.r() && swire->x2()>rc.r()) return 0;	//�E�ɔ͈͊O
		vFlag = false;
		fixval = swire->y1();
		p0 = swire->x1();
		p5 = swire->x2();
		boundaryMin = rc.l();
		boundaryMax = rc.r();
	}else{	//�΂ߐ��̂Ƃ��͕������Ȃ��B
		if(nSelStatus!=0){
			dwire[0]=(SXBSchWire*)(swire->duplicate());
			dwire[0]->setSelection(nSelStatus);
			return 1;
		}else{
			return 0;
		}
	}
	if(p0>p5){ int i=p5; p5=p0; p0=i; }

	p3=p1=p0;
	p2=p4=p5;

	if(p0>=boundaryMin) p2=p0;
	if(p5<=boundaryMax) p3=p5;
	XBSchObjListIterator pObjIte = m_listWorkJunc.begin();
	while(pObjIte != m_listWorkJunc.end()){
		SXBSchJunc* pJunc = (SXBSchJunc*)(*pObjIte);
		if(vFlag){  jfix = pJunc->x1();	jp	 = pJunc->y1();	}
		else{		jfix = pJunc->y1();	jp	 = pJunc->x1();	}
		if(jfix==fixval && jp>p1 && jp<p4){
			if(jp<boundaryMin){			//p1�X�V�`�F�b�N(boundaryMin��菬�����čő�)
				p1=jp;
			}else if(jp>boundaryMax){	//p4�X�V�`�F�b�N(boundaryMax���傫���čŏ�)
				p4=jp;
			}else{	//�ڍ��_�͔͈͓�
				if(jp>p3) p3=jp;
				if(jp<p2) p2=jp;
			}
		}
		pObjIte++;
	}
	if(p2>p3){//�͈͓��ɒ[�_���ڍ��_���Ȃ��ꍇ�͕������Ȃ�
		return 0;
	}
	if(p2<p3){//�͈͓��ɗ��[�_�������C���[������
		dwire[retval]=new SXBSchWire;
		if(vFlag){
			dwire[retval]->setP1(SPoint(fixval,p2));
			dwire[retval]->setP2(SPoint(fixval,p3));
		}else{
			dwire[retval]->setP1(SPoint(p2,fixval));
			dwire[retval]->setP2(SPoint(p3,fixval));
		}
		dwire[retval]->setSelection(SXBSchObj::SELECT_ALL);
		dwire[retval]->setLayer(nLayer);
		retval++;
	}
	if(p1<p2){//�͈͓���P2�[�_�������C���[������
		dwire[retval]=new SXBSchWire;
		if(vFlag){
			dwire[retval]->setP1(SPoint(fixval,p1));
			dwire[retval]->setP2(SPoint(fixval,p2));
		}else{
			dwire[retval]->setP1(SPoint(p1,fixval));
			dwire[retval]->setP2(SPoint(p2,fixval));
		}
		dwire[retval]->setSelection(SXBSchObj::SELECT_P2);
		dwire[retval]->setLayer(nLayer);
		retval++;
	}
	if(p3<p4){//�͈͓���P1�[�_�������C���[������
		dwire[retval]=new SXBSchWire;
		if(vFlag){
			dwire[retval]->setP1(SPoint(fixval,p3));
			dwire[retval]->setP2(SPoint(fixval,p4));
		}else{
			dwire[retval]->setP1(SPoint(p3,fixval));
			dwire[retval]->setP2(SPoint(p4,fixval));
		}
		dwire[retval]->setSelection(SXBSchObj::SELECT_P1);
		dwire[retval]->setLayer(nLayer);
		retval++;
	}
	if(p0<p1){//�͈͊O�Ƀ��C���[������
		dwire[retval]=new SXBSchWire;
		if(vFlag){
			dwire[retval]->setP1(SPoint(fixval,p0));
			dwire[retval]->setP2(SPoint(fixval,p1));
		}else{
			dwire[retval]->setP1(SPoint(p0,fixval));
			dwire[retval]->setP2(SPoint(p1,fixval));
		}
		dwire[retval]->setLayer(nLayer);
		retval++;
	}
	if(p4<p5){//�͈͊O�Ƀ��C���[������
		dwire[retval]=new SXBSchWire;
		if(vFlag){
			dwire[retval]->setP1(SPoint(fixval,p4));
			dwire[retval]->setP2(SPoint(fixval,p5));
		}else{
			dwire[retval]->setP1(SPoint(p4,fixval));
			dwire[retval]->setP2(SPoint(p5,fixval));
		}
		dwire[retval]->setLayer(nLayer);
		retval++;
	}
	return retval;
}


int SXBSchDoc::selectForDrag(const SRect& rc,bool bCutOffPinWire)
{
	//�ꎞ�f�[�^�̉��
	//resetSelect();

	//m_resetSelectOption = RESET_SELECT_NORMAL;

	bool bAppendSelect = (m_listTemp.size()>0);

	m_listWorkJunc.clear();
	m_listWorkWire.clear();
	m_listWorkTagComponent.clear();

	int scanMax = m_listMain.size();

	//�ڍ��_�̑I���ƒ��o
	pickupJunction(rc);
	selectForDragJunction(scanMax,rc);

	//���C���[�̑I��
	//�͈͓��O�ɂ܂�������̂̓W�����N�V�����ɂ�镪�����s��
	selectForDragWire(scanMax,rc);

	//�ڍ��_�A���C���[�ȊO�̑I��
	selectForDragExceptJuncWire(scanMax,rc);


	//�����̑I����ɁA�V���Ȕ͈͂�ǉ��I�����ꂽ�^�O�A���i�̃s���ɐڑ����ꂽ���I��[�_���Ȃ����`�F�b�N
	if(bAppendSelect){
		selectForDragReCheckSelectedLine(rc);
	}




	//�s���ɐڑ����ꂽ���C���[�̑I��
	selectForDragPinConnection(scanMax,rc,bCutOffPinWire);

	
	XBSchObjListIterator pObjIte = m_listWorkWire.begin();
	while(pObjIte != m_listWorkWire.end()){
		if(   (*pObjIte)->deleteNum()<0
			&&(*pObjIte)->refObj()==NULL){	//�Q�ƃI�u�W�F�N�g���Ȃ����̂������L��
			if((*pObjIte)->selection()!=0){	//�I���t���O���t���Ă�����͈̂ꎞ�f�[�^�ɃR�s�[
				m_listTemp.push_back(*pObjIte);
			}else{
				(*pObjIte)->setPlaceNum(m_nOpCount+1);
				pushBackToMainList(*pObjIte,false);
//20060603				m_listMain.push_back(*pObjIte);
			}
		}else{
			delete(*pObjIte);
		}
		pObjIte++;
	}
	

	
	setupRcTempArea();
	setupRotateRect();
	m_listWorkJunc.clear();
	m_listWorkWire.clear();
	m_listWorkTagComponent.clear();

	int nSelectCount = m_listTemp.size();
	if(nSelectCount >0){
		m_bSelectForDrag = true;
		m_bTempDirty = true;
		m_bFindCommandActive = false;
		discardRedo();
	}else{
		m_bSelectForDrag = false;
	}
	return nSelectCount;
//	m_bSelectForDrag = true;
//	return m_listTemp.size();
}

bool SXBSchDoc::findStringCompare(const TCHAR* szTarget,const TCHAR* sz,bool bMatchWhole,bool bCaseSensitive)
{
	if(szTarget==NULL || sz==NULL)return false;
	if(*szTarget=='\0' || *sz=='\0') return false;

	if(bMatchWhole){
		return !(_tcscmp(szTarget,sz));
	}else{
		return (_tcsstr(szTarget,sz)!=NULL);
	}
}

const SXBSchObj* SXBSchDoc::findStart(const TCHAR* sz,bool bMatchWhole,bool bCaseSeinsitive,unsigned int targetFlags,bool selectWhenFound)
{
	resetSelect(RESET_SELECT_NORMAL);
	m_strFind = sz;
	m_bFindMatchWhole = bMatchWhole;
	m_bCaseSeinsitive = bCaseSeinsitive;
	m_nTargetFlags = targetFlags;
	m_selectWhenFound = selectWhenFound;
	return find(true);
}



const SXBSchObj* SXBSchDoc::find(bool bStart)
{
	SXBSchObj* pObj = NULL;
	SXBSchObj* pObjSelected = NULL;
	resetSelect(RESET_SELECT_NORMAL);
	m_foundType = 0;

	if(bStart){
	    m_reviteFind = m_listMain.rbegin();
		m_bFindCommandActive = true;
		m_nFindStep = 0;
	}else{
		if(!m_bFindCommandActive)return NULL;
		if(m_nFindStep == 0){
			m_reviteFind++;
		}
	}

	m_foundString.clear();

	while(m_reviteFind != m_listMain.rend()){
		if( 
			(SLayer::wLayerTable[(*m_reviteFind)->layer()] & m_visibleLayer)!=0

			//(*m_reviteFind)->layer() == m_editLayer
	      &&(*m_reviteFind)->deleteNum()<0
		  &&(*m_reviteFind)->refObj()==NULL){	//�Q�ƃI�u�W�F�N�g�����݂�����͖̂����ł���
			pObj = *m_reviteFind;
			int nId = pObj->id();
			switch(nId){
				case SXBSchObj::ID_COMPONENT:
					if(m_nFindStep==0){
						m_nFindStep = 5;
						if((m_nTargetFlags & FINDTGT_NAME)&&
						   (findStringCompare(((SXBSchComponent*)pObj)->name(),m_strFind.c_str(),m_bFindMatchWhole,m_bCaseSeinsitive))){
								pObjSelected = pObj;
								m_foundType = FINDTGT_NAME;
								m_foundString = ((SXBSchComponent*)pObj)->name();
								m_foundPoint = ((SXBSchComponent*)pObj)->p1();
								m_foundPoint += ((SXBSchComponent*)pObj)->namePos();
								break;
						}
					}
			

					if(m_nFindStep>=5){
						m_nFindStep = 4;
						if((m_nTargetFlags & FINDTGT_NAMELIB)&&
						   (findStringCompare(((SXBSchComponent*)pObj)->orgname(),m_strFind.c_str(),m_bFindMatchWhole,m_bCaseSeinsitive))){
							pObjSelected = pObj;
							m_foundType = FINDTGT_NAMELIB;
							m_foundString = ((SXBSchComponent*)pObj)->orgname();
							m_foundPoint = ((SXBSchComponent*)pObj)->p1();
							break;
						}
					}

					if(m_nFindStep>=4){
						m_nFindStep = 3;
						if((m_nTargetFlags & FINDTGT_NUM)&&
						   (findStringCompare(((SXBSchComponent*)pObj)->refnum(),m_strFind.c_str(),m_bFindMatchWhole,m_bCaseSeinsitive))){
							pObjSelected = pObj;
							m_foundType = FINDTGT_NUM;
							m_foundString = ((SXBSchComponent*)pObj)->refnum();
							m_foundPoint = ((SXBSchComponent*)pObj)->p1();
							m_foundPoint += ((SXBSchComponent*)pObj)->refnumPos();
							break;
						}
					}
					if(m_nFindStep>=3){
						m_nFindStep = 2;
						if((m_nTargetFlags & FINDTGT_MANUFACTURE)&&
						   (findStringCompare(((SXBSchComponent*)pObj)->mfr(),m_strFind.c_str(),m_bFindMatchWhole,m_bCaseSeinsitive))){
							pObjSelected = pObj;
							m_foundType = FINDTGT_MANUFACTURE;
							m_foundString = ((SXBSchComponent*)pObj)->mfr();
							m_foundPoint = ((SXBSchComponent*)pObj)->p1();
							break;
						}
					}
					if(m_nFindStep>=2){
						m_nFindStep=1;
						if((m_nTargetFlags & FINDTGT_MANUFACTUREPN)&&
						   (findStringCompare(((SXBSchComponent*)pObj)->mfrpnum(),m_strFind.c_str(),m_bFindMatchWhole,m_bCaseSeinsitive))){
							pObjSelected = pObj;
							m_foundType = FINDTGT_MANUFACTUREPN;
							m_foundString = ((SXBSchComponent*)pObj)->mfrpnum();
							m_foundPoint = ((SXBSchComponent*)pObj)->p1();
							break;
						}
					}


					if(m_nFindStep>=1){
						m_nFindStep = 0;
						if((m_nTargetFlags & FINDTGT_PACKAGE)&&
						   (findStringCompare(((SXBSchComponent*)pObj)->packagename(),m_strFind.c_str(),m_bFindMatchWhole,m_bCaseSeinsitive))){
							pObjSelected = pObj;
							m_foundType = FINDTGT_PACKAGE;
							m_foundString = ((SXBSchComponent*)pObj)->packagename();
							m_foundPoint = ((SXBSchComponent*)pObj)->p1();
							break;
						}
					}


					if(m_nTargetFlags & FINDTGT_NOTE){
						if(findStringCompare(((SXBSchComponent*)pObj)->note(),m_strFind.c_str(),m_bFindMatchWhole,m_bCaseSeinsitive)){
							pObjSelected = pObj;
							m_foundType = FINDTGT_NOTE;
							m_foundString = ((SXBSchComponent*)pObj)->note();
							m_foundPoint = ((SXBSchComponent*)pObj)->p1();
							break;
						}
					}


					break;
				case SXBSchObj::ID_TAG:
					if(m_nTargetFlags & FINDTGT_TAG){
						if(findStringCompare(((SXBSchTag*)pObj)->text(),m_strFind.c_str(),m_bFindMatchWhole,m_bCaseSeinsitive)){
							pObjSelected = pObj;
							m_foundType = FINDTGT_TAG;
							m_foundString = ((SXBSchTag*)pObj)->text();
							m_foundPoint = pObj->p1();
							break;
						}
					}
					break;
				case SXBSchObj::ID_LABEL:
					if(m_nTargetFlags & FINDTGT_LABEL){
						if(findStringCompare(((SXBSchLabel*)pObj)->text(),m_strFind.c_str(),m_bFindMatchWhole,m_bCaseSeinsitive)){
							pObjSelected = pObj;
							m_foundType = FINDTGT_LABEL;
							m_foundString = ((SXBSchLabel*)pObj)->text();
							m_foundPoint = pObj->p1();
							break;
						}
					}
					break;
				case SXBSchObj::ID_COMMENT:
					if(m_nTargetFlags & FINDTGT_COMMENT){
						if(findStringCompare(((SXBSchComment*)pObj)->text(),m_strFind.c_str(),m_bFindMatchWhole,m_bCaseSeinsitive)){
							pObjSelected = pObj;
							m_foundType = FINDTGT_COMMENT;
							m_foundString = ((SXBSchComment*)pObj)->text();
							m_foundPoint = pObj->p1();
							break;
						}
					}
					break;
				default:
					break;
			}
		}
		if(pObjSelected != NULL) break;
		m_reviteFind++;
	}
	if(pObjSelected != NULL){
		if(m_selectWhenFound){
			SXBSchObj* pNewObj = pObjSelected->duplicate();
			pObjSelected->setRefObj(pNewObj);
			pNewObj->setRefObj(pObjSelected);
			pNewObj->setSelection(SXBSchObj::SELECT_ALL);
			m_listTemp.push_back(pNewObj);
		}
	}else{
		m_bFindCommandActive = false;
	}


	if(m_selectWhenFound){
		setupRcTempArea();
		setupRotateRect();
		m_bSelectForDrag = false;
	}

	return pObjSelected;
//	return (pObjSelected!=NULL);
}

//�o���N�A�g���r���[�g ���i���̒u��
void SXBSchDoc::bulkAttributeReplaceComponentInfo(const SCompIndex* pPartIndex)
{
	XBSchObjListIterator pObjIte = m_listTemp.begin();
	while(pObjIte != m_listTemp.end()){
		if((*pObjIte)->id()==SXBSchObj::ID_COMPONENT){
			SXBSchComponent* pObj = (SXBSchComponent*)*pObjIte;
			pObj->setCompInfoIndex(pPartIndex);
		}
		pObjIte++;
	}
	//updateSelectedTypeFlag();
	setupRcTempArea();
	m_bTempDirty = true;
}


//�o���N�A�g���r���[�g ���i���̔�\���ݒ�
void SXBSchDoc::bulkAttributeHideComponentVal(bool hide)
{
	XBSchObjListIterator pObjIte = m_listTemp.begin();
	while(pObjIte != m_listTemp.end()){
		if((*pObjIte)->id()==SXBSchObj::ID_COMPONENT){
			SXBSchComponent* pObj = (SXBSchComponent*)*pObjIte;
			pObj->SetHideVal(hide);
		}
		pObjIte++;
	}
	m_bTempDirty = true;
}

//�o���N�A�g���r���[�g ���i�ԍ��̔�\���ݒ�
void SXBSchDoc::bulkAttributeHideComponentNum(bool hide)
{
	XBSchObjListIterator pObjIte = m_listTemp.begin();
	while(pObjIte != m_listTemp.end()){
		if((*pObjIte)->id()==SXBSchObj::ID_COMPONENT){
			SXBSchComponent* pObj = (SXBSchComponent*)*pObjIte;
			pObj->SetHideNum(hide);
		}
		pObjIte++;
	}
	m_bTempDirty = true;
}


//�o���N�A�g���r���[�g �R�����g�t�H���g�̕ύX
void SXBSchDoc::bulkAttributeCommentFont(SXBSchComment& refComment)
{
	XBSchObjListIterator pObjIte = m_listTemp.begin();
	while(pObjIte != m_listTemp.end()){
		if((*pObjIte)->id()==SXBSchObj::ID_COMMENT){
			SXBSchComment* pObj = (SXBSchComment*)*pObjIte;
			pObj->setFontName(refComment.fontName());
			pObj->setFontSize(refComment.fontSize());
			pObj->setFontBold(refComment.fontBold());
			pObj->setFontItalic(refComment.fontItalic());
		}
		pObjIte++;
	}
	//updateSelectedTypeFlag();
	setupRcTempArea();
	m_bTempDirty = true;
}



//�o���N�A�g���r���[�g �}�[�J�[���C���̕ύX
void SXBSchDoc::bulkAttributeMarkerLine(int width,unsigned nColor)
{
	XBSchObjListIterator pObjIte = m_listTemp.begin();
	while(pObjIte != m_listTemp.end()){
		if((*pObjIte)->id()==SXBSchObj::ID_MARKER){
			SXBSchMarkerLine* pObj = (SXBSchMarkerLine*)*pObjIte;
			pObj->m_nColor = nColor;
			pObj->m_width = width;
		}
		pObjIte++;
	}
	//updateSelectedTypeFlag();
	setupRcTempArea();
	m_bTempDirty = true;
}

//�o���N�A�g���r���[�g �����̕ύX
void SXBSchDoc::bulkAttributeDecorationLine(SXBSchDash& refDecoline)
{
	XBSchObjListIterator pObjIte = m_listTemp.begin();
	while(pObjIte != m_listTemp.end()){
		if((*pObjIte)->id()==SXBSchObj::ID_DASH){
			SXBSchDash* pObj = (SXBSchDash*)*pObjIte;
			pObj->setStartPointStyle(refDecoline.startPointStyle());
			pObj->setEndPointStyle(refDecoline.endPointStyle());
			pObj->setEndMarkSize(refDecoline.endMarkSize());
			pObj->setLineStyle(refDecoline.lineStyle());
			pObj->setLineWidth(refDecoline.lineWidth());
			pObj->setCurve(refDecoline.curve());
		}
		pObjIte++;
	}
	//updateSelectedTypeFlag();
	setupRcTempArea();
	m_bTempDirty = true;
}




//�o���N�A�g���r���[�g �^�O�g�̕ύX
void SXBSchDoc::bulkAttributeChangeTagFrameTo(int n)
{
	XBSchObjListIterator pObjIte = m_listTemp.begin();
	while(pObjIte != m_listTemp.end()){
		if((*pObjIte)->id()==SXBSchObj::ID_TAG){
			SXBSchTag* pObj = (SXBSchTag*)*pObjIte;
			pObj->setTagType(n);
		}
		pObjIte++;
	}
	//updateSelectedTypeFlag();
	setupRcTempArea();
	m_bTempDirty = true;
}



//���x������^�O�𐶐�
SXBSchTag* SXBSchDoc::generateTagFromLabel(SXBSchLabel* pObj)
{
	SXBSchTag* pNewObj=new SXBSchTag;
	pNewObj->setP1(pObj->p1());
	pNewObj->setText(pObj->text());

	pNewObj->setLayer(pObj->layer());
	pNewObj->setRefObj(pObj->refObj());
	pNewObj->setPlaceNum(pObj->placeNum());
	pNewObj->setSelection(SXBSchObj::SELECT_ALL);
	return pNewObj;
}


//���x�����^�O�ɕϊ�
void SXBSchDoc::bulkConvertLabel2Tag()
{
	XBSchObjListIterator pObjIte = m_listTemp.begin();
	while(pObjIte != m_listTemp.end()){
		if((*pObjIte)->id()==SXBSchObj::ID_LABEL){
			SXBSchObj* pObj = *pObjIte;
			SXBSchObj* pNewObj = generateTagFromLabel((SXBSchLabel*)pObj);
			*pObjIte = pNewObj;
			delete pObj;
		}
		pObjIte++;
	}
	updateSelectedTypeFlag();
	setupRcTempArea();
	m_bTempDirty = true;
}

//���x������R�����g�𐶐�
SXBSchComment* SXBSchDoc::generateCommentFromLabel(SXBSchLabel* pObj)
{	
	SXBSchComment* pNewObj=new SXBSchComment;
	pNewObj->setP1(pObj->p1());
	pNewObj->setText(pObj->text());

	pNewObj->setLayer(pObj->layer());
	pNewObj->setRefObj(pObj->refObj());
	pNewObj->setPlaceNum(pObj->placeNum());
	pNewObj->setSelection(SXBSchObj::SELECT_ALL);
	return pNewObj;
}
//���x�����R�����g�ɕϊ�
void SXBSchDoc::bulkConvertLabel2Comment()
{
	XBSchObjListIterator pObjIte = m_listTemp.begin();
	while(pObjIte != m_listTemp.end()){
		if((*pObjIte)->id()==SXBSchObj::ID_LABEL){
			SXBSchObj* pObj = *pObjIte;
			SXBSchObj* pNewObj = generateCommentFromLabel((SXBSchLabel*)pObj);
			*pObjIte = pNewObj;
			delete pObj;
		}
		pObjIte++;
	}
	updateSelectedTypeFlag();
	setupRcTempArea();
	m_bTempDirty = true;
}


//�^�O���烉�x���𐶐�
SXBSchLabel* SXBSchDoc::generateLabelFromTag(SXBSchTag* pObj)
{	
	SXBSchLabel* pNewObj=new SXBSchLabel;
	pNewObj->setP1(pObj->p1());
	pNewObj->setText(pObj->text());
	pNewObj->setLayer(pObj->layer());
	pNewObj->setRefObj(pObj->refObj());
	pNewObj->setPlaceNum(pObj->placeNum());
	pNewObj->setSelection(SXBSchObj::SELECT_ALL);
	return pNewObj;
}

//�^�O�����x���ɕϊ�
void SXBSchDoc::bulkConvertTag2Label()
{
	XBSchObjListIterator pObjIte = m_listTemp.begin();
	while(pObjIte != m_listTemp.end()){
		if((*pObjIte)->id()==SXBSchObj::ID_TAG){
			SXBSchObj* pObj = *pObjIte;
			SXBSchObj* pNewObj = generateLabelFromTag((SXBSchTag*)pObj);
			*pObjIte = pNewObj;
			delete pObj;
		}
		pObjIte++;
	}
	updateSelectedTypeFlag();
	setupRcTempArea();
	m_bTempDirty = true;
}


//�^�O����R�����g�𐶐�
SXBSchComment* SXBSchDoc::generateCommentFromTag(SXBSchTag* pObj)
{	
	SXBSchComment* pNewObj=new SXBSchComment;
	pNewObj->setP1(pObj->p1());
	pNewObj->setText(pObj->text());

	pNewObj->setLayer(pObj->layer());
	pNewObj->setRefObj(pObj->refObj());
	pNewObj->setPlaceNum(pObj->placeNum());
	pNewObj->setSelection(SXBSchObj::SELECT_ALL);
	return pNewObj;
}


//�^�O���R�����g�ɕϊ�
void SXBSchDoc::bulkConvertTag2Comment()
{
	XBSchObjListIterator pObjIte = m_listTemp.begin();
	while(pObjIte != m_listTemp.end()){
		if((*pObjIte)->id()==SXBSchObj::ID_TAG){
			SXBSchObj* pObj = *pObjIte;
			SXBSchObj* pNewObj = generateCommentFromTag((SXBSchTag*)pObj);
			*pObjIte = pNewObj;
			delete pObj;
		}
		pObjIte++;
	}
	updateSelectedTypeFlag();
	setupRcTempArea();
	m_bTempDirty = true;
}


//�R�����g�����x���ɕϊ�
void SXBSchDoc::bulkConvertComment2Label()
{
	XBSchObjListIterator pObjIte = m_listTemp.begin();
	while(pObjIte != m_listTemp.end()){
		if((*pObjIte)->id()==SXBSchObj::ID_COMMENT){
			SXBSchComment* pObj = (SXBSchComment*)*pObjIte;
			int x = pObj->p1().x();
			int y = pObj->p1().y();


			wstring strText = pObj->text();
			int len = strText.length();
			int s=0;
			bool firstLine = true;
			for(int e=0;e<len;e++){
				if(strText[e]=='\r'||strText[e]=='\n'||(e==(len-1))){
					if(s<e ||(strText[e]!='\r'&&strText[e]!='\n')){
						int copyLen;
						if(strText[e]=='\r'||strText[e]=='\n'){
							copyLen = e-s;
						}else{
							copyLen = e-s+1;
						}
						wstring newStr = strText.substr(s,copyLen);

						SXBSchLabel* pNewObj=new SXBSchLabel;
						pNewObj->setP1(SPoint(x,y));
						y+=10;
						pNewObj->setText(newStr.c_str());
						pNewObj->setLayer(pObj->layer());
						pNewObj->setRefObj((firstLine? pObj->refObj():NULL));
						pNewObj->setPlaceNum(pObj->placeNum());
						pNewObj->setSelection(SXBSchObj::SELECT_ALL);
						
						if(firstLine){
							*pObjIte = pNewObj;
							firstLine = false;
						}else{
							m_listTemp.push_front(pNewObj);
						}

						

					}
					s=e+1;
				}
			}
			if(!firstLine){	//�V�K���x���I�u�W�F�N�g�ɒu����������
				delete pObj;	//���R�����g�I�u�W�F�N�g�̍폜
			}

		}
		pObjIte++;
	}
	updateSelectedTypeFlag();
	setupRcTempArea();
	m_bTempDirty = true;
}


//�R�����g���^�O�ɕϊ�
void SXBSchDoc::bulkConvertComment2Tag()
{
	XBSchObjListIterator pObjIte = m_listTemp.begin();
	while(pObjIte != m_listTemp.end()){
		if((*pObjIte)->id()==SXBSchObj::ID_COMMENT){
			SXBSchComment* pObj = (SXBSchComment*)*pObjIte;
			int x = pObj->p1().x();
			int y = pObj->p1().y();


			wstring strText = pObj->text();
			int len = strText.length();
			int s=0;
			bool firstLine = true;
			for(int e=0;e<len;e++){
				if(strText[e]=='\r'||strText[e]=='\n'||(e==(len-1))){
					if(s<e ||(strText[e]!='\r'&&strText[e]!='\n')){
						int copyLen;
						if(strText[e]=='\r'||strText[e]=='\n'){
							copyLen = e-s;
						}else{
							copyLen = e-s+1;
						}
						wstring newStr = strText.substr(s,copyLen);

						SXBSchTag* pNewObj=new SXBSchTag;
						pNewObj->setP1(SPoint(x,y));
						y+=10;
						pNewObj->setText(newStr.c_str());
						pNewObj->setLayer(pObj->layer());
						pNewObj->setRefObj((firstLine? pObj->refObj():NULL));
						pNewObj->setPlaceNum(pObj->placeNum());
						pNewObj->setSelection(SXBSchObj::SELECT_ALL);
						
						if(firstLine){
							*pObjIte = pNewObj;
							firstLine = false;
						}else{
							m_listTemp.push_front(pNewObj);
						}

					}
					s=e+1;
				}
			}
			if(!firstLine){//�V�K�^�O�I�u�W�F�N�g�ɒu����������
				delete pObj;	//���R�����g�I�u�W�F�N�g�̍폜
			}
		}
		pObjIte++;
	}
	updateSelectedTypeFlag();
	setupRcTempArea();
	m_bTempDirty = true;
}
