/****************************************************************************
    NL3W netlist utility for BSch3V
    Copyright (C) 2004-2014 H.Okada
*****************************************************************************/


#ifndef NETLIST_H
#define NETLIST_H

#ifdef _MSVC
	#pragma warning (disable: 4786)
#endif

#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <string>
#include <algorithm>
#include <list>
using namespace std;
#include "xbschstringlist.h"
#include "ce3io.h"
#include "xbschobj.h"
#include "xbschjunc.h"
#include "xbschlabel.h"
#include "xbschcomment.h"
#include "xbschtag.h"
#include "xbschcomponent.h"

#include "xbschline.h"
#include "xbschwirenl.h"
#include "xbschentry.h"
#include "xbschdelobj.h"
#include "xbschsheetinfo.h"
#include "xbschdoc.h"
#include "nlpin.h"
#include "dlliftype.h"


typedef list<SNlPin*> NlPinList;
typedef list<SNlPin*>::iterator NlPinListIterator;

typedef list<SPoint> PointList;
typedef list<SPoint>::iterator PointListIterator;


typedef SNlPin* PSNlPin;





class SSingleNetList
{
public:
	stringList m_SignalNameList;
	//string m_SignalName;
	SNlPin** m_pPinArray;
	int		m_nPinCount;
	SSingleNetList(){
		//m_SignalName=="";
		m_pPinArray	=NULL;
		m_nPinCount =0;
	}
	~SSingleNetList();

	string SignalName();
};

class SPartsListElement
{
public:
	string	m_prefix;
	int		m_suffix;
	string	m_name;
	string	m_note;
	string	m_mfr;
	string	m_mfrpn;
	string	m_package;
};

typedef SPartsListElement* PSPartsListElement;



typedef list<SSingleNetList*> NetList;
typedef list<SSingleNetList*>::iterator NetListIterator;

//#include "netclass.h"

struct SNetlistOption{
	const TCHAR* pszDstPath;
	//FILE* fpDst;
	FILE* fpRpt;
	int   listType;
	bool  bPlistMode;
	bool  bExcludeNC;
	bool  bSigNamePinInfo;
};

class SNetList{
protected:
	XBSchObjList m_listTagPower;		//�^�O�Ɠd�����i
	XBSchObjList m_listNcTagPower;		//�g���Ȃ������^�O�Ɠd�����i
	XBSchObjList m_listWire;			//���C���[
	XBSchObjList m_listJunc;			//�ڍ��_
	XBSchObjList m_listLabel;			//���x��
	NlPinList	 m_listNlPin;			//�񋓂����s��
	NlPinList	 m_listNlNcPin;			//�g���Ȃ������s��
	NlPinList	 m_listNlConfPin;		//�d���s��
	NlPinList	 m_listNoNumPin;		//�s���ԍ��������Ȃ��s��
	int			 m_xOffset;				//setData�����s���邽�т�10000��������
	PSPartsListElement* m_pArrayPlistElem;

	stringList m_signalNameList;

	wstringList	m_srcfilePathList;		//�\�[�X�t�@�C���̃p�X��ۑ�����B

public:
	XBSchObjList m_listComponent;		//�d�����i�ȊO�̕��i
	NetList		 m_listNet;
	NetList		 m_listNcNet;			//�ڑ�����Ă��Ȃ��l�b�g


public:
	SNetList();
	~SNetList();
	static string removeWS(const string& str);				//������̗��[�̃z���C�g�X�y�[�X�̍폜 
	static void clearXBSchObjList(XBSchObjList& listObj);	//SXBSchObj�̃|�C���^���X�g�̉��
	static void clearNlPin(NlPinList& listPin);				//SNlPin�̃|�C���^���X�g�̉��
	void clear();
	void clearNetList(NetList& netlist);
	void setData(const TCHAR* filename, XBSchObjList* plistSrc);//SXBSchDoc���Ǘ����Ă���}�ʗv�f���玟�̗v�f���R�s�[���ĐU�蕪����̃��X�g�ɒǉ�����B
	void createNetList(SNetlistOption& option); //bool bExcludeNC);
	void createPartsList();
	void clearPlistElem();
	int writeReport(FILE* fp,bool bExcludeNC);			//���|�[�g�t�@�C���o��
	//int writePADSNetList(FILE* fp,bool bExcludeNC);		//PADS�`���̃l�b�g���X�g�o��
	//int writeTeleNetList(FILE* fp,bool bExcludeNC);		//�e���V�X�`���̃l�b�g���X�g�o��
	//int writeProtNetList(FILE* fp,bool bExcludeNC);		//�v���e���`���̃l�b�g���X�g�o��
	int writeTeleNetList(const TCHAR* filename,bool bExcludeNC);		//�e���V�X�`���̃l�b�g���X�g�o��
	int writeProtNetList(const TCHAR* filename,bool bExcludeNC);		//�v���e���`���̃l�b�g���X�g�o��
	int writeUserTypeNetList(const TCHAR* filename, bool bExcludeNC, int( __stdcall *fnWriteNetList)(const TCHAR*, const SExportNetlistInfo*));
	int writePartsList(const TCHAR* filename);		//CSV�`���̕��i���X�g�o��
	//int writePartsList(FILE* fp);		//CSV�`���̕��i���X�g�o��

protected:
	bool testLineJoint(int& al,int& ar,int bl,int br); //�����܂��͐����̐��̌������e�X�g����B
	void jointWire();			//���C���[�̘A������
	void divideWire();			//���C���[��ڍ��_�ŕ�������
	void nameFromTagPower();	//�^�O�Ɠd�����i�����ɐ��ƃs���ɖ��O������
	void nameFromLabel();		//���x�������ɐ��ɐ�����ǉ�

	bool isUniqueSignalName(string str);	//20130914 �M���������j�[�N���ǂ����̃`�F�b�N�B

	
	void enumPin();//�L���ȕ��i�̃s����񋓂���
	bool quaryExist(stringList& strlist,string& str);//�w�蕶���񂪕����񃊃X�g�ɑ��݂��邩�ǂ����ǂ����̃`�F�b�N
	bool quaryExist(PointList& pointlist,SPoint point);//�w��_���_���X�g�ɑ��݂��邩�ǂ����ǂ����̃`�F�b�N
	SSingleNetList* NetList1();//�ЂƂ̃m�[�h�̃l�b�g
	void enumConflictingPin();

	static void parseRefNum(const string& refnum,string& prefix,int& suffix);
	static int CompareSNlPin( const void *ppin1, const void *ppin2 );//SNlPin�̃s�����ɂ��召��r
	static int CompareSPartsListElement( const void *p1, const void *p2 ); //SPartsListElement�̕��i�ԍ��ɂ���r

};




//�l�b�g���X�g���쐬����B
//int netlist(int srcfiles, const TCHAR* srcfile[],const TCHAR* cfgpath,FILE* fp,int listtype,bool bPlist);
int netlist(int srcfiles, const TCHAR* srcfile[],SNetlistOption& option);







typedef SXBSchObj* PSXBSchObj;

#endif