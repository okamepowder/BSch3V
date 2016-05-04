/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#ifndef XBSCHGLOBAL_H
#define XBSCHGLOBAL_H

//#include "qstring.h"
//#include "cfgdata.h"

#define MAX_LIB 64	// ライブラリの最大数

class SCompIndex;
class SCompLib;

//////////////////////////////////////////////////////////////////////
// 大文字小文字を区別しない文字列比較
int strcmp_i(const char* s1,const char* s2);

//////////////////////////////////////////////////////////////////////
//コンフィギュレーションファイル関係
//extern SCfgData g_cfg;
//extern string g_strCfgFileName;

//////////////////////////////////////////////////////////////////////
//ライブラリ関連

//ライブラリの情報の差し替え
bool g_ReplaceLibrary(int index,SCompLib* pCompLib);

//コンフィギュレーションファイルからライブラリの情報を得て読み込む
bool g_ReadLibrary();

//ライブラリのメモリの解放を行う
void g_FreeLibrary();

//ライブラリから部品インデックスを得る
const SCompIndex* g_SearchComponentIndex(const char* pszName,int* pnLib,int* pnIndex,SCompLib* pLibOptional);

//ライブラリから論理反転部品のインデックスを得る
const SCompIndex* g_SearchLogicalInvertComponentIndex(const char* pszName);

int g_LibraryCount();
const SCompLib* g_LibraryInfo(int nLib); 

#endif