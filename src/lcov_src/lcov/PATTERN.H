/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

//////////////////////////////////////////////////////////////
//  Pattern.h
//
//   ビットパターンの描画を行う CPatternクラスの宣言とインター
// フェースの定義を行う

#ifndef PATTERN_H
#define PATTERN_H

class CPattern
{
	CSize m_sizeBmp;	//ビットマップのサイズ
	BYTE* m_pbyteBuff;	//仮展開を行うビットマップ初期化バッファ
	CSize m_sizeSrc;	//パターンのサイズ
	const BYTE* m_pbyteSrc;	//パターンへのポインタ
	void Copy();		//パターンをビットマップ初期化バッファにコピー
	void Rot90();		//パターンをビットマップ初期化バッファに90度回転コピー
	void Rot180();		//パターンをビットマップ初期化バッファに180度回転コピー
	void Rot270();		//パターンをビットマップ初期化バッファに270度回転コピー
	void Vrev();		//ビットマップ初期化バッファの上下反転
public:
	CPattern(){m_pbyteBuff=new BYTE[262144];}	//コンストラクタ
	~CPattern(){delete[]m_pbyteBuff;}			//デストラクタ
	void PutPtn(	//パターンの描画
				CDC* pDC,		//出力先デバイスコンテキスト
				int x,int y,	//出力位置
				const BYTE* lpPtn,	//出力パターン
				int	 width,
				int  height,
				COLORREF col,	//出力色
				UINT nMode=DRAW_ON,	//描画モード
				int  nRot=0,		//回転角
				BOOL bMirror=FALSE,	//左右反転フラグ
				int  nVExt=1,		//表示倍率(View)
				int  nWExt=1); //表示倍率(Window)
};

extern CPattern g_Pattern;	//実体は Pattern.cpp

#endif