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


class PListParam{

public:
enum PL_FILETYPE
{
	PL_FILETYPE_CSV =0,
	PL_FILETYPE_XML =2
};

public:
	int listtype;
	//bool plAvoidZeroSuppressingOnMsExcel;
	bool plFlagMfr;
	bool plFlagMfrPn;
	bool plFlagPackage;
	bool plFlagNote;
	bool plFlagInfo;
	PListParam(){
		listtype = 0;	//PL3Wでは 0,1: CSV,  2:SYLK
		//plAvoidZeroSuppressingOnMsExcel = false;
		plFlagMfr = false;
		plFlagMfrPn = false;
		plFlagPackage = false;
		plFlagNote = false;
		plFlagInfo = false;
	}
};

//class SNutDataElement
//{
//public:
//	wstring record;
//	bool	newline;
//	SNutDataElement(){newline = false;}
//	~SNutDataElement(){}
//};


//typedef list<SNutDataElement*> SNutDataElementList;
//typedef list<SNutDataElement*>::iterator SNutDataElementListIterator;


// SNutDataFile
// CE3ファイルごとの管理クラス
class SNutDataFile
{
public:
	wstring m_filename;

	SNutDataFile(){}
	~SNutDataFile();
};

class SNutComponent
{
public:
	//ファイル番号
	int m_index;

	//部品の座標
	SPoint m_p;

	//ライブラリからの呼び出し名
	wstring m_orgname;

	//部品名
	wstring m_name;
	
	//リファレンス番号
	//SNutDataElement* m_refNumElement;	//SNutDataElementの実体を管理するのは SNutDataFileクラスなので ここでdeleteしない
	wstring m_prefix;
	int m_suffix;
	bool m_validSuffix;

	wstring m_mfr;
	wstring m_mfrpn;
	wstring m_package;
	wstring m_note;
	wstring m_componentNote;




	
	//ブロック番号
	//SNutDataElement* m_blockElement;	//SNutDataElementの実体を管理するのは SNutDataFileクラスなので ここでdeleteしない
	//int m_block;
	//int m_blocks;
	//bool m_powComp;
	//bool m_ignore;
	//bool m_keep;	//更新処理しない場合にtrueにする。
	//bool m_checked;
	//bool m_setPowComp;


	void SetRef(/*SNutDataElement* pElement,*/const TCHAR* pcszRefnum);
	//void SetBlock(SNutDataElement* pElement,const TCHAR* block);
	//void update(); //参照番号とブロック番号のSNutDataElementの実体に反映する
	SNutComponent()
	{
		//m_refNumElement = NULL;
		m_suffix = 0;
		m_validSuffix = false;	
		//m_blockElement = NULL;
		//m_block = 0;
		//m_blocks = 1;
		//m_powComp = false;
		//m_ignore = false;
		//m_keep= false;
		//m_checked = false;
		//m_setPowComp = false;
	}

};


typedef list<SNutComponent*> NutCompList;
typedef list<SNutComponent*>::iterator NutCompListIterator;
typedef SNutComponent* PSNutComponent;

class SPlist
{
	int m_dataFileCount;
	SNutDataFile m_schFiles[MAX_SCH_FILES];
	
	//wstring* m_prefixFilterArray;
	//int m_prefixFilterCount;

	NutCompList m_complist;

	PSNutComponent* m_compArray;
	int m_compArrayCount;

	//PSNutComponent* m_powcompArray;
	//int m_powcompArrayCount;
	void pickupComponent();
	void clearCompArray();
	void clearSchFiles();
	//void sortPOS();
	//void sortREF();
	//void createBackup();
	//bool DoPrefixFilter(const char* pcszPrefix);

public:
	//bool m_enablePrefixFilter;
	//bool m_prefixFilterDontMode;
	//bool m_newlyOnly;

	//string m_prefixFilterString;


public:
	SPlist();
	~SPlist();
	int fileCount(){return m_dataFileCount;}
	int readFile(const TCHAR* path);	//読み込みに成功したら0を返す


	PListParam m_param;

	void convertToCsvRecord(const wstring& src,wstring& dest);
	void encodeXMLstring(const wstring& src,wstring& dest);
	int writePartsList(const TCHAR* filename);


	//CSV形式の部品リスト出力
	int writePartsList_csv(const TCHAR* filename);


	//XML形式の部品リスト出力
	int writePartsList_xml(const TCHAR* filename);


	static const TCHAR* xmlHeader;

enum COLUMN_WIDTH
{
	COLUMN_WIDTH_STATUS = 40,
	COLUMN_WIDTH_PREFIX = 50,
	COLUMN_WIDTH_SUFFIX = 50,
	COLUMN_WIDTH_NAME = 120,
	COLUMN_WIDTH_MFR = 80,
	COLUMN_WIDTH_MFRPN = 120,
	COLUMN_WIDTH_PKG = 80,
	COLUMN_WIDTH_NOTE = 200,
	COLUMN_WIDTH_COMP_NOTE = 200
};


	//int renameToBackup();
	//void restore();
	//int saveFiles();
	//int anumNewlyOnly(int init,int step);
	//int anum(int init,int step,bool newonly);
	//int renum(int init,int step);
	//const TCHAR* filename(int n);

	//void setPrefixFilter(const TCHAR* filterPrefixes);

};
