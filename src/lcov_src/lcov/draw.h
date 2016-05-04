/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

#pragma once

#include "coord.h"

//描画モード （以下の値の論理和をとって使う）
#define DRAW_ON		1	// 通常の描画
#define DRAW_OFF	2	// 消去。おそらくは背景色での描画を指す
#define DRAW_TEMP	4	// XORモードでの描画
#define DRAW_FAST	8	// 高速モードでの描画。おそらくは次のようなもの）
						//		・部品ならピン番号、ピン名、部品番号、部品名を省略
						//		・タグなら文字列を省略
						//	 その他の多くの図面要素ではこのビットは意味を持たない
#define DRAW_INVERT	16	// BSchの8ドット文字列のXORだけで有効。部品番号、部品名の描画で使用
#define DRAW_PARTFRAME 		32	//部品枠の描画
#define DRAW_DARK	64	// 暗い色での描画
#define DRAW_INH_PARTNAME 	256	//部品名の描画をしない
#define DRAW_INH_PARTNUM  	512	//部品番号の描画をしない
#define DRAW_INH_PARTBODY  1024	//部品ボディの描画をしない
#define DRAW_SWAP_PINNAME_PINNUM 2048

///////////////////////////////////////////////////////////////
// 色
#define COL_JUNC			(RGB(  0,255,  0))
#define COL_WIRE			(RGB(  0,255,  0))
#define COL_BUS				(RGB(  0,255,255))
#define COL_DASH			(RGB(255,255,  0))
#define COL_TAG				(RGB(255,255,  0))
#define COL_TAG_NAME		(RGB(  0,255,255))
#define COL_COMMENT			(RGB(  0,255,255))
#define COL_LABEL			(RGB(  0,255,  0))
#define COL_XOR				(RGB(255,  0,  0))
#define COL_XOR_DARK		(RGB(255,128,128))
#define COL_PART			(RGB(  0,128,  0))
#define COL_PART_PINNAME	(RGB(  0,  0,255))	
#define COL_PART_PINNUM		(RGB(128,  0,128))
#define COL_PART_NAME		(RGB(255,255,255))	
#define COL_PART_NUM		(RGB(255,255,255))
#define COL_PART_FRAME		(RGB(  0,  0,255))
#define COL_SHEET_FRAME		(RGB(255,255,  0))
#define COL_SHEET_GRID		(RGB(255,  0,  0))
#define COL_DARK			(RGB(220,220,220))
#define COL_EDITPTN			(RGB(  0,128,255))
#define COL_OFF				(RGB(  0,  0,  0))

class SPtnObjLine;
class SPtnObjCircle;
class SPtnObjPolygon;
class SPtnObjArc;
class SPtnObjText;

extern bool g_bIsWin9x;

//パターンのLineの描画
void g_DrawPtnLine(
			CDC* pDC,				//描画
			SPtnObjLine* pObj,		//描画オブジェクト
			COLORREF col,			//描画色
			int	 dir,
			int	 cx,
			int  cy,
			int	 x0,
			int  y0,
			int  nVExt,				//表示倍率(View)
			int  nWExt)				//表示倍率(Window)
;

//パターンのCircleの描画
void g_DrawPtnCircle(
			CDC* pDC,				//描画
			SPtnObjCircle* pObj,	//描画オブジェクト
			COLORREF col,			//描画色
			int	 dir,
			int	 cx,
			int  cy,
			int	 x0,
			int  y0,
			int  nVExt,				//表示倍率(View)
			int  nWExt)				//表示倍率(Window)
;

void g_DrawPtnPolygon(
			CDC* pDC,				//描画
			SPtnObjPolygon* pObj,	//描画オブジェクト
			COLORREF col,			//描画色
			int	 dir,
			int	 cx,
			int  cy,
			int	 x0,
			int  y0,
			int  nVExt,				//表示倍率(View)
			int  nWExt)				//表示倍率(Window)
;

void g_DrawPtnArc(
			CDC* pDC,				//描画
			SPtnObjArc* pObj,		//描画オブジェクト
			COLORREF col,			//描画色
			int	 dir,
			int	 cx,
			int  cy,
			int	 x0,
			int  y0,
			int  nVExt,				//表示倍率(View)
			int  nWExt)				//表示倍率(Window)
;

void g_DrawPtnText(
			CDC* pDC,				//描画
			SPtnObjText* pObj,		//描画オブジェクト
			COLORREF col,			//描画色
			int	 dir,
			int	 cx,
			int  cy,
			int	 x0,
			int  y0,
			int  nVExt,				//表示倍率(View)
			int  nWExt)				//表示倍率(Window)
;

void g_drawComponentPin(
	CDC* pDC,						//描画先
	const SPoint& pinPos,
	int	pinType,
	int nLtrb,
	LPCTSTR pszPinName,				//ピン名
	unsigned long pinNameDispFlag,
	LPCTSTR pszPinNum,				//ピン番号
	DWORD dwMode,					//描画モード
	int nVExt,						//表示倍率(View)
	int nWExt,						//表示倍率(Window)
	LPCRECT lprcClip)				//クリップ情報
;

