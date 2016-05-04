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
	XBSchObjList m_listTagPower;		//タグと電源部品
	XBSchObjList m_listNcTagPower;		//使われなかったタグと電源部品
	XBSchObjList m_listWire;			//ワイヤー
	XBSchObjList m_listJunc;			//接合点
	XBSchObjList m_listLabel;			//ラベル
	NlPinList	 m_listNlPin;			//列挙したピン
	NlPinList	 m_listNlNcPin;			//使われなかったピン
	NlPinList	 m_listNlConfPin;		//重複ピン
	NlPinList	 m_listNoNumPin;		//ピン番号を持たないピン
	int			 m_xOffset;				//setDataを実行するたびに10000ずつ増える
	PSPartsListElement* m_pArrayPlistElem;

	stringList m_signalNameList;

	wstringList	m_srcfilePathList;		//ソースファイルのパスを保存する。

public:
	XBSchObjList m_listComponent;		//電源部品以外の部品
	NetList		 m_listNet;
	NetList		 m_listNcNet;			//接続されていないネット


public:
	SNetList();
	~SNetList();
	static string removeWS(const string& str);				//文字列の両端のホワイトスペースの削除 
	static void clearXBSchObjList(XBSchObjList& listObj);	//SXBSchObjのポインタリストの解放
	static void clearNlPin(NlPinList& listPin);				//SNlPinのポインタリストの解放
	void clear();
	void clearNetList(NetList& netlist);
	void setData(const TCHAR* filename, XBSchObjList* plistSrc);//SXBSchDocが管理している図面要素から次の要素をコピーして振り分け先のリストに追加する。
	void createNetList(SNetlistOption& option); //bool bExcludeNC);
	void createPartsList();
	void clearPlistElem();
	int writeReport(FILE* fp,bool bExcludeNC);			//レポートファイル出力
	//int writePADSNetList(FILE* fp,bool bExcludeNC);		//PADS形式のネットリスト出力
	//int writeTeleNetList(FILE* fp,bool bExcludeNC);		//テレシス形式のネットリスト出力
	//int writeProtNetList(FILE* fp,bool bExcludeNC);		//プロテル形式のネットリスト出力
	int writeTeleNetList(const TCHAR* filename,bool bExcludeNC);		//テレシス形式のネットリスト出力
	int writeProtNetList(const TCHAR* filename,bool bExcludeNC);		//プロテル形式のネットリスト出力
	int writeUserTypeNetList(const TCHAR* filename, bool bExcludeNC, int( __stdcall *fnWriteNetList)(const TCHAR*, const SExportNetlistInfo*));
	int writePartsList(const TCHAR* filename);		//CSV形式の部品リスト出力
	//int writePartsList(FILE* fp);		//CSV形式の部品リスト出力

protected:
	bool testLineJoint(int& al,int& ar,int bl,int br); //水平または垂直の線の結合をテストする。
	void jointWire();			//ワイヤーの連結整理
	void divideWire();			//ワイヤーを接合点で分割する
	void nameFromTagPower();	//タグと電源部品を元に線とピンに名前をつける
	void nameFromLabel();		//ラベルを元に線に線名を追加

	bool isUniqueSignalName(string str);	//20130914 信号名がユニークかどうかのチェック。

	
	void enumPin();//有効な部品のピンを列挙する
	bool quaryExist(stringList& strlist,string& str);//指定文字列が文字列リストに存在するかどうかどうかのチェック
	bool quaryExist(PointList& pointlist,SPoint point);//指定点が点リストに存在するかどうかどうかのチェック
	SSingleNetList* NetList1();//ひとつのノードのネット
	void enumConflictingPin();

	static void parseRefNum(const string& refnum,string& prefix,int& suffix);
	static int CompareSNlPin( const void *ppin1, const void *ppin2 );//SNlPinのピン名による大小比較
	static int CompareSPartsListElement( const void *p1, const void *p2 ); //SPartsListElementの部品番号による比較

};




//ネットリストを作成する。
//int netlist(int srcfiles, const TCHAR* srcfile[],const TCHAR* cfgpath,FILE* fp,int listtype,bool bPlist);
int netlist(int srcfiles, const TCHAR* srcfile[],SNetlistOption& option);







typedef SXBSchObj* PSXBSchObj;

#endif