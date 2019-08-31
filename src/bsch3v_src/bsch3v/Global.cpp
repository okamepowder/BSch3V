/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// Global.cpp : グローバル変数の定義を行う
//

#include "stdafx.h"
#include "BSch.h"

#include <string>
using namespace std;

//#include "PartLib.h"
//#include "BSchFont.h"

//////////////////////////////////////////////////////////////////////
//画面表示関連
BOOL g_bDisplayPartFrame;	//部品枠の表示   1997/01/01 CBSchViewから移動
BOOL g_bDisplayNcPinMark;	//NCピンのマークの表示
BOOL g_bDisplayHiddenPinNum;	//非表示ピン番号の表示 0.63.01


CWnd* g_pViewWindow;


//////////////////////////////////////////////////////////////////////
//環境関連
BOOL g_bJapaneseLangEnv;

//////////////////////////////////////////////////////////////////////
//アクティブなドキュメントのパス名のコピー

wstring g_docPath;
wstring g_docTimeStamp;
wstring g_docDate;
wstring g_printTime;



//////////////////////////////////////////////////////////////////////
//ログ関連

//wstring g_log;
//TCHAR g_logPath[_MAX_PATH];



//////////////////////////////////////////////////////////////////////
//座標関連
//２点の座標から矩形範囲を設定する
//void g_MakeRECT(LPPOINT p1,LPPOINT p2,LPRECT prc)
//{
//	if(p1->x < p2->x){
//		prc->left =p1->x;
//		prc->right=p2->x;
//	}else{
//		prc->left =p2->x;
//		prc->right=p1->x;
//	}
//	if(p1->y < p2->y){
//		prc->top   =p1->y;
//		prc->bottom=p2->y;
//	}else{
//		prc->top   =p2->y;
//		prc->bottom=p1->y;
//	}
//}

////矩形座標の上下左右が入れ替わっている場合に正しくする
//void g_CorrectRECT(LPRECT prc)
//{
//	int n;
//	if(prc->top > prc->bottom){
//		n=prc->top;
//		prc->top=prc->bottom;
//		prc->bottom=n;
//	}
//	if(prc->left > prc->right){
//		n=prc->left;
//		prc->left=prc->right;
//		prc->right=n;
//	}
//}

//矩形2が矩形1の内側（または同じ）にあるかどうかを確かめる
//BOOL g_IsRect2insideRect1(LPRECT prc1,LPRECT prc2)
//{
//	if(  prc2->left   >= prc1->left
//	  && prc2->left   <= prc1->right
//	  && prc2->right  >= prc1->left
//	  && prc2->right  <= prc1->right
//	  && prc2->top    >= prc1->top
//	  && prc2->top    <= prc1->bottom
//	  && prc2->bottom >= prc1->top
//	  && prc2->bottom <= prc1->bottom
//	)return TRUE;
//	else return FALSE;
//}

//////////////////////////////////////////////////////////////////////
//クリップボード関係
//const char szClipboardFormatName[]="BSch CE2 format data";
//UINT nClipboardFormat;

////クリップボードのフォーマットの値を得る
//UINT g_GetClipboardFormat()
//{
//	return nClipboardFormat;
//}

////クリップボードのフォーマットの登録をする
//UINT g_RegisterClipboardFormat()
//{
//	nClipboardFormat=::RegisterClipboardFormat(szClipboardFormatName);
//	return nClipboardFormat;
//}


