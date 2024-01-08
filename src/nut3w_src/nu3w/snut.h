/****************************************************************************
    NUT3W Numbering software for BSch3V
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#pragma once
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
#include "coord.h"
//#include "xbschobj.h"
//#include "xbschjunc.h"
//#include "xbschlabel.h"
//#include "xbschcomment.h"
//#include "xbschtag.h"
//#include "xbschcomponent.h"
//
//#include "xbschline.h"
////#include "xbschwirenl.h"
//#include "xbschentry.h"
//#include "xbschdelobj.h"
//#include "xbschsheetinfo.h"
//#include "xbschdoc.h"

#define MAX_SCH_FILES 64 

class SNutDataElement
{
public:
	wstring record;
	bool	newline;
	SNutDataElement(){newline = false;}
	~SNutDataElement(){}
};


typedef list<SNutDataElement*> SNutDataElementList;
typedef list<SNutDataElement*>::iterator SNutDataElementListIterator;


// SNutDataFile
// CE3ファイルごとの管理クラス
class SNutDataFile
{
public:
	wstring m_filename;
	SNutDataElementList m_recordList;

	SNutDataFile(){}
	~SNutDataFile();
};

class SNutComponent
{
public:
	//ファイル番号
	int m_index;

	int m_layer;

	//部品の座標
	SPoint m_p;

	//ライブラリからの呼び出し名
	wstring m_orgname;

	//部品名
	wstring m_name;
	
	//リファレンス番号
	SNutDataElement* m_refNumElement;	//SNutDataElementの実体を管理するのは SNutDataFileクラスなので ここでdeleteしない
	wstring m_prefix;
	int m_suffix;
	bool m_validSuffix;
	
	//ブロック番号
	SNutDataElement* m_blockElement;	//SNutDataElementの実体を管理するのは SNutDataFileクラスなので ここでdeleteしない
	int m_block;
	int m_blocks;
	int m_fixedblock;
	bool m_useFixedBlock;
	bool m_powComp;
	bool m_ignore;
	bool m_keep;	//更新処理しない場合にtrueにする。
	bool m_checked;
	bool m_setPowComp;


	void SetRef(SNutDataElement* pElement,const TCHAR* pcszRefnum);
	void SetBlock(SNutDataElement* pElement,const TCHAR* block);
	
	void update(); //参照番号とブロック番号のSNutDataElementの実体に反映する
	SNutComponent()
	{
		m_refNumElement = NULL;
		m_suffix = 0;
		m_validSuffix = false;	
		m_blockElement = NULL;
		m_block = 0;
		m_blocks = 1;
		m_powComp = false;
		m_ignore = false;
		m_keep= false;
		m_checked = false;
		m_setPowComp = false;
		m_useFixedBlock = false;
		m_fixedblock = 0;
		m_layer = 0;
		m_index = 0;
	}

};


typedef list<SNutComponent*> NutCompList;
typedef list<SNutComponent*>::iterator NutCompListIterator;
typedef SNutComponent* PSNutComponent;

class SNut
{
	int m_dataFileCount;
	SNutDataFile m_schFiles[MAX_SCH_FILES];
	
	wstring* m_prefixFilterArray;
	int m_prefixFilterCount;

	NutCompList m_complist;

	PSNutComponent* m_compArray;
	int m_compArrayCount;

	//PSNutComponent* m_powcompArray;
	//int m_powcompArrayCount;
	void pickupComponent(unsigned int option);
	void clearCompArray();
	void clearSchFiles();
	//void sortPOS();
	//void sortREF();
	//void createBackup();
	//bool DoPrefixFilter(const char* pcszPrefix);

	enum CE3_FILE_CHECK_RESULT{
		CE3FCHK_ERROR		=0,
		CE3FCHK_V070_OR_E	=1,
		CE3FCHK_V080_OR_L	=2
	};
	


	//バージョン判定
	int ce3FileCheck(const TCHAR* fname);


public:
	enum NUT_OPTION{
		NUTOPTION_NON			=0,
		NUTOPTION_NEWONLY		=1,
		NUTOPTION_EN_LAYER		=2
	};

	bool m_enablePrefixFilter;
	bool m_prefixFilterDontMode;
	//bool m_newlyOnly;

	//string m_prefixFilterString;


public:
	SNut();
	~SNut();
	int fileCount(){return m_dataFileCount;}

	int readFile(const TCHAR* path);	//読み込みに成功したら0を返す
	int renameToBackup();
	void restore();
	int saveFiles();
	//int anumNewlyOnly(int init,int step);
	int anum(int init,int step,unsigned int option);
	int renum(int init,int step);
	const TCHAR* filename(int n);

	void setPrefixFilter(const TCHAR* filterPrefixes);

};
