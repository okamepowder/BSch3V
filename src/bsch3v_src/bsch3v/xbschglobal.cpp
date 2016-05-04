/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#include "stdafx.h"
#include "shlwapi.h"
#ifdef _BSCHCAPTURE
	#include "bsch.h"
#endif
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <list>
#include <string>
using namespace std;

//#include <qstring.h>
//#include <qcstring.h>

#include "xbsch.h"
#include "coord.h"
#include "xbschglobal.h"
#include "complib.h"
//#include "cfgdata.h"


////////////////////////////////////////////////////////////////////////
//// 大文字小文字を区別しない文字列比較
//// Visual C++ には stricmpがあるが標準のC++には無いようなのでここで定義
//int strcmp_i(const char* s1,const char* s2)
//{
//	int c1;
//	int c2;
//
//	while(1){
//		if(*s1=='\0' && *s2=='\0') return 0; 
//		c1 = toupper(*s1++);
//		c2 = toupper(*s2++);
//		if(c1 != c2) return c1-c2;
//	}
//}



//////////////////////////////////////////////////////////////////////
//コンフィギュレーションファイル関係
//SCfgData g_cfg;
//string g_strCfgFileName;	//コンフィギュレーションファイル名

//////////////////////////////////////////////////////////////////////
//ライブラリ関連
static SCompLib* g_pCompLib[MAX_LIB];	//部品ライブラリ
static int s_libraryCount=0;

#ifdef _BSCHCAPTURE

//ライブラリの情報の差し替え
bool g_ReplaceLibrary(int index,SCompLib* pCompLib)
{
	if(index<0 || index>=s_libraryCount) return false;
	if(!pCompLib) return false;

	delete g_pCompLib[index];
	g_pCompLib[index] = pCompLib;

	return true;
}



//コンフィギュレーションファイルからライブラリの情報を得て読み込む
bool g_ReadLibrary()
{
//	string filename;
	int n;
	TCHAR szKey[32];

	for(n=0;n<MAX_LIB;n++) g_pCompLib[n]=NULL;
	int entryNum=0;
	n=0;
	while(entryNum<MAX_LIB){
		wsprintf(szKey,_T("LIB%d"),entryNum);			//エントリ文字列の作成
		entryNum++;
		CString strPath=AfxGetApp()->GetProfileString(_T("Library"),szKey);//ライブラリ名の取得
		if(strPath.IsEmpty()){		//文字列（ライブラリ名）が獲得できなかったらbreak
			break;
		}else{
			if(PathIsRelative(strPath)){
				TCHAR fullPath[_MAX_PATH];
				PathCombine(fullPath,((CBSchApp*)AfxGetApp())->m_szIniFileDir,strPath);
				strPath = fullPath;
			}
			g_pCompLib[n]=new SCompLib;
			if(!g_pCompLib[n]->readLibraryFile(strPath)){
				delete g_pCompLib[n];
				g_pCompLib[n]=NULL;
			}else{
				n++;
			}
		}
	}
	s_libraryCount = n;
	return true;
}

//ライブラリのメモリの解放を行う
void g_FreeLibrary()
{
	int n;
	for(n=0;n<s_libraryCount;n++){
		delete g_pCompLib[n];
		g_pCompLib[n] = NULL;
	}
	while(n<MAX_LIB) g_pCompLib[n++]=NULL;
	s_libraryCount=0;
}

//ライブラリから部品インデックスを得る
const SCompIndex* g_SearchComponentIndex(const TCHAR* pszName,int *pnLib,int *pnIndex,SCompLib* pLibOptional)
{
	int i,n;
	int nCount;
	const SCompIndex* pCompIndex;
	for(i=0;i<=s_libraryCount;i++){
		SCompLib* pLib = NULL;
		if(i<s_libraryCount){
			pLib = g_pCompLib[i];
		}else{
			pLib=pLibOptional;
		}
		if(pLib==NULL) return NULL;
		nCount=pLib->count();	//ライブラリに登録されている部品数
		for(n=0;n<nCount;n++){
			pCompIndex=pLib->compIndex(n);
			if(!_tcsicmp(pszName,pCompIndex->name())){
				if(pnLib)  *pnLib=i;
				if(pnIndex) *pnIndex=n;
				return pCompIndex;
			}
		}
	}
	return NULL;
}

//ライブラリから論理反転部品のインデックスを得る
const SCompIndex* g_SearchLogicalInvertComponentIndex(const TCHAR* pszName)
{
	if(pszName==NULL)return NULL;

	//元の部品のライブラリ番号とインデックス情報の取得
	int nCompLibNum;
	const SCompIndex* pIndex		= g_SearchComponentIndex(pszName,&nCompLibNum,NULL,NULL);
	if(pIndex == NULL) return NULL;

	wstring strInvertName=pszName;
	int n=strInvertName.length();
	if(n>1 && (strInvertName[n-1]=='i' || strInvertName[n-1]=='I')){
		//末尾がiなら末尾を消去
		strInvertName=strInvertName.substr(0,n-1);
	}else{
		//末尾がiでなければiを追加
		strInvertName=strInvertName+_T("i");
	}

	int nInvCompLibNum;
	const SCompIndex* pInvIndex	= g_SearchComponentIndex(strInvertName.c_str(),&nInvCompLibNum,NULL,NULL);
	if(pInvIndex == NULL || nInvCompLibNum != nCompLibNum) return NULL;


	//その他適合条件のチェック
	//ピン数
	if(pIndex->pinCount() != pInvIndex->pinCount()) return NULL;

	//ブロック数
	if(pIndex->block() != pInvIndex->block()) return NULL;

	//部品サイズ
	if(pIndex->size() != pInvIndex->size()) return NULL;	

	return pInvIndex;
}

//管理しているライブラリの数を返す
int g_LibraryCount(){return s_libraryCount;}

//ライブラリへのポインタを返す
const SCompLib* g_LibraryInfo(int nLib){
	if(nLib>=0 && nLib<s_libraryCount) return g_pCompLib[nLib];
	else return NULL;
}
#else
//ライブラリから部品インデックスを得る
const SCompIndex* g_SearchComponentIndex(const TCHAR* pszName,int *pnLib,int *pnIndex,SCompLib* pLibOptional)
{
	return NULL;
}

#endif