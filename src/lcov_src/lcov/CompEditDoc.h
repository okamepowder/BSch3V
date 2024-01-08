/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

#pragma once

#include <list>
#include <string>
using namespace std;

#include "complib.h"
#include "CompEditElm.h"

typedef list<SCompEditElm> SCompEditElmList;
typedef list<SCompEditElm>::iterator SCompEditElmListIterator;
typedef list<SCompEditElm>::reverse_iterator SCompEditElmListRevIterator;

class SCompEditDoc
{
public:
	static bool m_noChechAreaOnSave;
public:
	enum{
		DATATYPE_PIN = 0,
		DATATYPE_PTN = 1
	};
protected:
	int m_nOpCount;

public:
	SCompEditElmList m_listMain;
	SCompEditElmList m_listTemp;
	bool m_dirtyFlag;
	bool m_tempDirtyFlag;
	SSize m_size;		//�O���b�h�P�ʂł̃T�C�Y
	int  m_block;
	SRect m_tempArea;

	//string m_prop_string;

	wstring m_lastPinNumber;
	wstring m_lastPinName;

	void setPin(const PSPin* pPinArray, int count);
	void setPtnObj(const PSPtnObj* pPtnArray, int count);
	bool releaseTemp();
	bool isPinSelected();
	bool delTemp();
	void setupTempArea();
	bool isOnTmpData(const SPoint& pt);
	//int isOnGrip(const SPoint& pt);
	int incrementOpCount(){m_nOpCount++; return m_nOpCount;}
	int decrementOpCount(){m_nOpCount--; return m_nOpCount;}

	int opCount(){return m_nOpCount;}

	bool addPinToTemp(SPin* pPin);//�ꎞ�f�[�^�ւ̃s���̒ǉ�
	bool addPtnToTemp(SPtnObj* pPtn);//�ꎞ�f�[�^�ւ̃p�^�[���̒ǉ�

public:
	SCompEditDoc(void);
	virtual ~SCompEditDoc(void);
	void init();
	void loadCompInfo(const SCompInfo* pComp);
	void saveCompInfo(SCompInfo* pComp);

	int doSelect(const SPoint& pt);
	int doSelect(const SRect& rc);
	int doSelect(int ltrb,int offset);
	
	bool copy();
	bool canUndo();
	bool undo();

	bool move(int offsetx,int offsety);
	bool mirror();
	bool rotate();


	void pinPlacePoint(const SPoint& pt,int &ltrb,int &offset);	//pt�ɍł��߂��s���z�u���W�𓾂�
	SRect pinArea(int ltrb,int offset);//�s�����W����L���Ȑ�L�͈͂����߂�
	SPin* pinConflict(const SPin* pPin);//�����s���Ɠ����ꏊ�Ȃ�A�����s���̃|�C���^��Ԃ�
	SPoint pinPosition(int ltrb,int offset); //�s�����W���畔�i�ҏW���W�𓾂�

	static wstring nextString(const wstring&  str);

};
