/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

//XBSch�̃h�L�������g�̊Ǘ����s���B
#include "stdafx.h"
//#include <qapplication.h>
//#include <qwidget.h>
//#include <qstring.h>
//#include <SSize.h>
//#include <SPoint.h>
//#include <qclipboard.h>
#include <stdio.h>
#include <assert.h>
#include <shlwapi.h>
#include <string>
#include <list>
using namespace std;
#include "ce3io.h"
#include "xbschobj.h"
#include "xbschjunc.h"
#include "xbschlabel.h"
#include "xbschcomment.h"
#include "xbschtag.h"
#include "xbschcomponent.h"

#include "xbschline.h"
#include "xbschentry.h"
#include "xbschdelobj.h"
#include "xbschsheetinfo.h"
#include "xbschdoc.h"
#include "xbschdocsch.h"

//***** Qt�ˑ� *****
//�R�s�[
//�R�s�[���s��ꂽ�Ƃ���true��Ԃ�
bool SXBSchDocSch::copy()
{
	if(!m_listTemp.size())return false;
	SPoint ptOrigin = SPoint(m_rcTempArea.l(),m_rcTempArea.t());
	ptOrigin.setX(((ptOrigin.x()+5)/10)*10);
	ptOrigin.setY(((ptOrigin.y()+5)/10)*10);

	wstring str(_T(""));
	SWriteCE3 wce3;
	wce3.Attach(&str);

	wce3.WriteRecord(_T("+BSCH3_DATA_V.1.0"));
	wce3.WriteEOL();
	writeCe3(wce3,&m_listTemp,true,&ptOrigin);
	wce3.WriteRecord(_T("-BSCH3_DATA_V.1.0"));
	wce3.WriteEOL();


	int nLength = str.size()+1;
	//BYTE* pBuff=memfile.Detach();		//�������o�b�t�@���������t�@�C������؂藣���ăN���[�Y
	HGLOBAL hGMem=::GlobalAlloc(GMEM_MOVEABLE,nLength*sizeof(TCHAR));//�N���b�v�{�[�h�ɓn���������̊m��
	if(hGMem==NULL){
		return false;			//���������蓖�ĂɎ��s�����牽�����Ȃ�
	}
	BYTE* pgBuff=(BYTE*)::GlobalLock(hGMem);	//���蓖�Ă��O���[�o�������������b�N����
	memcpy(pgBuff,str.c_str(),nLength*sizeof(TCHAR));		//���蓖�Ă��O���[�o���������Ƀ������t�@�C���̓��e���R�s�[
//	free(pBuff);						//�������t�@�C������؂藣�����������̉���B
	::GlobalUnlock(hGMem);				//�O���[�o���������̃A�����b�N
	::OpenClipboard(AfxGetMainWnd()->m_hWnd);//�N���b�v�{�[�h�̃I�[�v��
	::EmptyClipboard();					//�N���b�v�{�[�h����ɂ���
	::SetClipboardData(CF_UNICODETEXT,hGMem);	//�N���b�v�{�[�h�Ƀf�[�^��n��
	::CloseClipboard();					//�N���b�v�{�[�h�����

//    QClipboard *cb = QApplication::clipboard();
//    cb->setText(QString::fromLocal8Bit(str.c_str()));
	
	return true;
}


//�\��t��
//�h�L�������g�ɕύX���������Ƃ�true��Ԃ�
bool SXBSchDocSch::paste(const SPoint& pt)
{
	if(!canPaste()) return false;
	::OpenClipboard(AfxGetMainWnd()->m_hWnd);		//Open clip board.
	HGLOBAL hGMem=::GetClipboardData(CF_UNICODETEXT);		//Get memory handle.
	if(hGMem==NULL){
		::CloseClipboard();						//Close clip board.
		return false;
	}
	TCHAR* pgBuff=(TCHAR*)::GlobalLock(hGMem);	//Lock global memory.
	wstring cbbuff=wstring(pgBuff);				//Initialize string with global memory.
	::GlobalUnlock(hGMem);						//Unock global memory.
	::CloseClipboard();							//Close clip board.


	SReadCE3 rce3;
	rce3.Attach(cbbuff.c_str());

	wstring str(_T(""));

	rce3.ReadRecord(str);
	if(str != _T("+BSCH3_DATA_V.1.0")) return false;

	readCe3(rce3,&m_listTemp,&pt,NULL);
	if(!m_listTemp.size()) return false;

	moveToLayer(m_editLayer);

	updateSelectedTypeFlag();
	setupRcTempArea();

	m_bTempNewPlace = true;
	m_bFindCommandActive = false;
	
	return true;
}

//Check clip board data.
//Return true if data is available.
bool SXBSchDocSch::canPaste()
{
	static const TCHAR* pcszID= _T("+BSCH3_DATA_V.1.0");
	int nIdLength = _tcslen(pcszID);
	bool retVal = false;
	if(::IsClipboardFormatAvailable(CF_UNICODETEXT)){
		::OpenClipboard(AfxGetMainWnd()->m_hWnd);		//Open clip board.
		HGLOBAL hGMem=::GetClipboardData(CF_UNICODETEXT);		//Get memory handle.
		if(hGMem!=NULL){
			int nLength=GlobalSize(hGMem);				//Size of clip board data.
			if(nLength>nIdLength){
				TCHAR* pgBuff=(TCHAR*)::GlobalLock(hGMem);	//Lock global memory.
				if(_tcsncmp(pcszID,pgBuff,nIdLength)==0){
					retVal = true;
				}
				::GlobalUnlock(hGMem);				//Unock global memory.
			}
		}
		::CloseClipboard();
	}
//	QString strID("+BSCH3_DATA_V.1.0");
//	QClipboard *cb = QApplication::clipboard();
//    QString str = cb->text().left(strID.length());	
//	return (str == strID);
	return retVal;
}

bool SXBSchDocSch::findStringCompare(const TCHAR* szTarget,const TCHAR* sz,bool bMatchWhole,bool bCaseSensitive)
{
	if(szTarget==NULL || sz==NULL)return false;
	if(*szTarget=='\0' || *sz=='\0') return false;

	if(bMatchWhole){
		if(bCaseSensitive){
			return !(_tcscmp(szTarget,sz));
		}else{
			return !(_tcsicmp(szTarget,sz));
		}
	}else{
		if(bCaseSensitive){
			return (_tcsstr(szTarget,sz)!=NULL);
		}else{
			return (StrStrI(szTarget,sz)!=NULL);
		}
	}
}

