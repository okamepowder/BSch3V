/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#ifndef DRAWOBJ_H
#define DRAWOBJ_H

extern bool g_bFillEndOfPtnLine;

//バス線の描画
void g_DrawBus(
			CDC* pDC,						//描画
			SXBSchObj* pObj,					//描画オブジェクト
			DWORD dwMode,					//描画モード
			int  nVExt=1,					//表示倍率(View)
			int  nWExt=1,					//表示倍率(Window)
			LPCRECT lprcClip=NULL);			//クリップ情報

//破線の描画
void g_DrawDash(
			CDC* pDC,						//描画先
			SXBSchObj* pObj,					//描画オブジェクト
			DWORD dwMode,					//描画モード
			int  nVExt=1,					//表示倍率(View)
			int  nWExt=1,					//表示倍率(Window)
			LPCRECT lprcClip=NULL);			//クリップ情報

//ワイヤーの描画
void g_DrawWire(
			CDC* pDC,						//描画先
			SXBSchObj* pObj,					//描画オブジェクト
			DWORD dwMode,					//描画モード
			int  nVExt=1,					//表示倍率(View)
			int  nWExt=1,					//表示倍率(Window)
			LPCRECT lprcClip=NULL);			//クリップ情報

//エントリーの描画
void g_DrawEntry(
			CDC* pDC,						//描画先
			SXBSchObj* pObj,					//描画オブジェクト
			DWORD dwMode,					//描画モード
			int  nVExt=1,					//表示倍率(View)
			int  nWExt=1,					//表示倍率(Window)
			LPCRECT lprcClip=NULL);			//クリップ情報

//ジャンクションの描画
void g_DrawJunc(
			CDC* pDC,						//描画先
			SXBSchObj* pObj,					//描画オブジェクト
			DWORD dwMode,					//描画モード
			int  nVExt=1,					//表示倍率(View)
			int  nWExt=1,					//表示倍率(Window)
			LPCRECT lprcClip=NULL);			//クリップ情報

//コメントの描画
void g_DrawComment(
			CDC* pDC,						//描画先
			SXBSchObj* pObj,					//描画オブジェクト
			DWORD dwMode,					//描画モード
			int  nVExt=1,					//表示倍率(View)
			int  nWExt=1,					//表示倍率(Window)
			LPCRECT lprcClip=NULL);			//クリップ情報
			
//イメージの描画
void g_DrawImageObj(
	CDC* pDC,					//描画先
	SXBSchObj* pObj,
	DWORD dwMode,				//描画モード
	int nVExt,					//表示倍率(View)
	int nWExt,
	LPCRECT lprcClip=NULL);			//クリップ情報


//ラベルの描画
void g_DrawLabel(
			CDC* pDC,						//描画先
			SXBSchObj* pObj,					//描画オブジェクト
			DWORD dwMode,					//描画モード
			int  nVExt=1,					//表示倍率(View)
			int  nWExt=1,					//表示倍率(Window)
			LPCRECT lprcClip=NULL);			//クリップ情報

//タグの描画
void g_DrawTag(
			CDC* pDC,						//描画先
			SXBSchObj* pObj,					//描画オブジェクト
			DWORD dwMode,					//描画モード
			int  nVExt=1,					//表示倍率(View)
			int  nWExt=1,					//表示倍率(Window)
			LPCRECT lprcClip=NULL);			//クリップ情報

//マーカーラインの描画
void g_DrawMarkerLine(
			CDC* pDC,						//描画先
			SXBSchObj* pObj,				//描画オブジェクト
			DWORD dwMode,					//描画モード
			int  nVExt /* =1 */,			//表示倍率(View)
			int  nWExt /* =1 */,			//表示倍率(Window)
			LPCRECT lprcClip /*=NULL*/);		//クリップ情報

//部品名のXOR描画
//void g_drawComponentNameXOR(CDC* pDC,SXBSchComponent* pObj,int nVExt,int nWExt);

//部品名のXOR描画
//void g_drawComponentNumXOR(CDC* pDC,SXBSchComponent* pObj,int nVExt,int nWExt);

//NCマークの描画
void	g_drawNcMark(	
			CDC* pDC,
			const SPoint& pt,						//位置
			DWORD dwMode,					//描画モード
			int nVExt,					//表示倍率(View)
			int nWExt,					//表示倍率(Window)
			LPCRECT);					//クリップ情報


//部品の描画
void g_DrawComponent(
			CDC* pDC,						//描画先
			SXBSchObj* pObj,
			DWORD dwMode,					//描画モード
			int nVExt=1,					//表示倍率(View)
			int nWExt=1,					//表示倍率(Window)
			LPCRECT lprcClip =NULL,		//クリップ情報
			DISP_BLOCKNUM_TYPE dispBN = DISP_BLOCKNUM_TYPE::NOTDISPLAY_BLOCKNUM);	//ブロック番号の表示

#endif