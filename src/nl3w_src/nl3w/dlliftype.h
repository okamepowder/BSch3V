/****************************************************************************
    NL3W netlist utility for BSch3V
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/
#ifndef DLLIFTYPE_H
#define DLLIFTYPE_H

//DLLに渡すピン値構造体 (DLLには下記のネットリスト構造体に含まれる)
struct SExportPinValue{
	const char* pszRefNum;
	const char* pszPinNum;
};

//DLLに渡すシングルネットデータ構造体
struct SExportSingleNetValue{
	const char* pszNetName;
	int nPinCount;
	SExportPinValue* pArrayPinValue;
};

typedef SExportSingleNetValue* PSExportSingleNetValue;

//DLLに渡す部品情報値構造体
struct SExportComponentInfo{
	const char* pszPrefix;	//部品番号のプレフィクス
	int   nSuffix;		//部品番号のサフィックス
	const char* pszName;		//部品名
	const char* pszPackage;	//パッケージ名
};

//typedef SExportComponentInfo* PSExportComponentInfo;

struct SExportNetlistInfo{
	int nComponentInfoCount;	//格納した部品情報の数
	SExportComponentInfo* array_CompInfo;	//部品情報

	int nNetValueCount;			//格納したネット情報の数
	SExportSingleNetValue* array_NetValue;	//ネット情報
};



#endif