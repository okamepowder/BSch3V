/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

//////////////////////////////////////////////////////////////
//  Pattern.CPP
//
//   ビットパターンの描画を行う CPatternクラスの動作の定義を行う
//

//パターンをビットマップ初期化バッファにコピー
//あらかじめm_sizeSrc、m_pbyteSrcに値をセットしておく

#include "stdafx.h"

#include "draw.h"
//#include "BSch.h"
#include "Pattern.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPattern g_Pattern;

void CPattern::Copy()
{
	int srcXbyte=((m_sizeSrc.cx+15)/16)*2;
	int x,y;
	m_sizeBmp=m_sizeSrc;
	const BYTE* pSrc=m_pbyteSrc;
	BYTE* pDst=m_pbyteBuff;
	for(y=0;y<m_sizeSrc.cy;y++){
		for(x=0;x<srcXbyte;x++){
			*pDst++=*pSrc++;
		}
	}
}

//パターンをビットマップ初期化バッファに90度回転コピー
//あらかじめm_sizeSrc、m_pbyteSrcに値をセットしておく
//ソースを左下から縦スキャン、デスティネーションを最上ラインから横スキャン
//                 →
//     ●○○    ●●●               
//   ↑●○○    ○○○          
//     ●○○    ○○○           
//
void CPattern::Rot90()
{
	int srcXbyte=((m_sizeSrc.cx+15)/16)*2;
	int dstXbyte=((m_sizeSrc.cy+15)/16)*2;
	int x,y;
	BYTE bitSrc,bitDst;
	m_sizeBmp.cx=m_sizeSrc.cy;
	m_sizeBmp.cy=m_sizeSrc.cx;
	const BYTE* pSrcTop=m_pbyteSrc+srcXbyte*(m_sizeSrc.cy-1);//左下のバイトを指す
	BYTE* pDstTop=m_pbyteBuff;			//左上のバイトを指す
	bitSrc=0x80;
	for(x=0;x<m_sizeSrc.cx;x++){
		const BYTE* pSrc=pSrcTop;
		BYTE* pDst=pDstTop;
		*pDst=0;
		bitDst=0x80;
		for(y=0;y<m_sizeSrc.cy;y++){
			if(*pSrc & bitSrc) *pDst |= bitDst;
			pSrc-=srcXbyte;		//ソースは1列上に移動
			if(bitDst==0x01){	//デスティネーションは左のビットに移動
				bitDst=0x80;	//隣のバイトに移るときは
				pDst++;
				*pDst=0;		//移った先のバイトを初期化する
			}else{
				bitDst>>=1;
			}
		}
		//１列が終わったら
		if(bitSrc==0x01){		//ソースを隣のビットに移動
			bitSrc=0x80;
			pSrcTop++;
		}else{
			bitSrc>>=1;
		}
		pDstTop+=dstXbyte;		//デスティネーションを次の行に移動
	}
}

//パターンをビットマップ初期化バッファに180度回転コピー
//あらかじめm_sizeSrc、m_pbyteSrcに値をセットしておく
//ソースを右上から横スキャン、デスティネーションを左下から横スキャン
//       ←      
//     ●●●  ○○○               
//     ○○○  ○○○        
//     ○○○  ●●●         
//               →
void CPattern::Rot180()
{
	int srcXbyte=((m_sizeSrc.cx+15)/16)*2;
	int x,y;
	BYTE bitSrc,bitDst,bitRight;
	m_sizeBmp=m_sizeSrc;
	
	BYTE* pDstTop=m_pbyteBuff+srcXbyte*(m_sizeSrc.cy-1);//左下のバイトを指す
	const BYTE* pSrcTop=m_pbyteSrc+(m_sizeSrc.cx+7)/8-1;		//右上のバイトを指す
	bitRight=0x80>>((m_sizeSrc.cx+7) % 8);				//右端のビット

	for(y=0;y<m_sizeSrc.cy;y++){
		BYTE* pDst=pDstTop;
		const BYTE* pSrc=pSrcTop;
		*pDst=0;
		bitDst=0x80;
		bitSrc=bitRight;
		for(x=0;x<m_sizeSrc.cx;x++){
			if(*pSrc & bitSrc) *pDst |= bitDst;
			if(bitSrc==0x80){
				pSrc--;
				bitSrc=0x01;
			}else{
				bitSrc<<=1;
			}
			if(bitDst==0x01){
				pDst++;
				*pDst=0;
				bitDst=0x80;
			}else{
				bitDst>>=1;
			}
		}
		pDstTop-=srcXbyte;	//デスティネーションは１つ上に移動
		pSrcTop+=srcXbyte;	//ソースは１つ下に移動
	}
}


//パターンをビットマップ初期化バッファに270度回転コピー
//あらかじめm_sizeSrc、m_pbyteSrcに値をセットしておく
//ソースを左上から縦スキャン、デスティネーションを左下から横スキャン
//                 
//     ●○○    ○○○               
//   ↓●○○    ○○○          
//     ●○○    ●●●           
//                 →
void CPattern::Rot270()
{
	int srcXbyte=((m_sizeSrc.cx+15)/16)*2;
	int dstXbyte=((m_sizeSrc.cy+15)/16)*2;
	int x,y;
	BYTE bitSrc,bitDst;
	m_sizeBmp.cx=m_sizeSrc.cy;
	m_sizeBmp.cy=m_sizeSrc.cx;
	const BYTE* pSrcTop=m_pbyteSrc;	//左上のバイトを指す
	BYTE* pDstTop=m_pbyteBuff+dstXbyte*(m_sizeSrc.cx-1);//左下のバイトを指す
	bitSrc=0x80;
	for(x=0;x<m_sizeSrc.cx;x++){
		const BYTE* pSrc=pSrcTop;
		BYTE* pDst=pDstTop;
		*pDst=0;
		bitDst=0x80;
		for(y=0;y<m_sizeSrc.cy;y++){
			if(*pSrc & bitSrc) *pDst |= bitDst;
			pSrc+=srcXbyte;		//ソースは1列下に移動
			if(bitDst==0x01){	//デスティネーションは左のビットに移動
				bitDst=0x80;	//隣のバイトに移るときは
				pDst++;
				*pDst=0;		//移った先のバイトを初期化する
			}else{
				bitDst>>=1;
			}
		}
		//１列が終わったら
		if(bitSrc==0x01){		//ソースを隣のビットに移動
			bitSrc=0x80;
			pSrcTop++;
		}else{
			bitSrc>>=1;
		}
		pDstTop-=dstXbyte;		//デスティネーションを次の行に移動
	}
}


//ビットマップ初期化バッファの上下反転
void CPattern::Vrev()
{
	BYTE buff[256];						//１ライン転送用バッファ
	int nXbyte=((m_sizeBmp.cx+15)/16)*2;//１ラインバイト数
	if(nXbyte>256) return;
	int x,y,l;
	BYTE* p0Top=m_pbyteBuff;			//左上のバイトを指す
	BYTE* p1Top=m_pbyteBuff+nXbyte*(m_sizeBmp.cy-1);//左下のバイトを指す
	BYTE* p0;
	BYTE* p1;

	l=m_sizeBmp.cy/2;
	for(y=0;y<l;y++){
		p0=p0Top;
		for(x=0;x<nXbyte;x++) buff[x]=*p0++;
		p0=p0Top;
		p1=p1Top;
		for(x=0;x<nXbyte;x++) *p0++=*p1++;
		p1=p1Top;
		for(x=0;x<nXbyte;x++) *p1++=buff[x];
		p0Top+=nXbyte;
		p1Top-=nXbyte;
	}
}


//パターンの描画	
//BSchでは部品を回転と左右反転で表現しているが、パターンの準備の手間から
//実際の処理は回転と上下反転で行う
//                        m_nRot(回転角)
//              (0)        (1)        (2)        (3)
// m_bMirror    →         ←         ←         →
//            ●○○     ○○●     ○○○     ○○○     
//  (FALSE) ↓○ａ○↓ ↓○ｂ○↓ ↑○ｃ○↑ ↑○ｄ○↑
//            ○○○     ○○○     ○○●     ●○○     
//              →         ←         ←         →
//
//
//              ←         →         →         ←
//            ○○●     ●○○     ○○○     ○○○     
//  (TRUE ) ↓○ｅ○↓ ↓○ｆ○↓ ↑○ｇ○↑ ↑○ｈ○↑
//            ○○○     ○○○     ●○○     ○○●     
//              ←         →         →         ←
//
//     ｅはｃの上下反転
//     ｆはｄの上下反転
//     ｇはａの上下反転
//     ｈはｂの上下反転



void CPattern::PutPtn(	
	CDC* pDC,		//出力先デバイスコンテキスト
	int x,int y,	//出力位置
	const BYTE* lpPtn,	//出力パターン
	int  width,
	int  height,
	COLORREF col,	//出力色
	UINT nMode/*=DRAW_ON*/,	//描画モード
	int  nRot/*=0*/,		//回転角
	BOOL bMirror/*=FALSE*/,	//左右反転フラグ
	int  nVExt/*=1*/,		//表示倍率(View)
	int  nWExt/*=1*/)		//表示倍率(Window)
{
	COLORREF colOldText,colOldBk;
	CSize    sizeOut;
	//サイズの設定
	m_sizeSrc.cx=width;				//ソースのパターンのｘサイズ
	m_sizeSrc.cy=height;			//ソースのパターンのｙサイズ
	m_pbyteSrc=lpPtn;
	//パターンの準備
	if(bMirror) nRot=(nRot+2)&3;
	switch(nRot){
	case 0:Copy();	 break;	//そのままコピー
	case 1:Rot90();  break;	//90度回転コピー
	case 2:Rot180(); break;	//180度回転コピー
	case 3:Rot270(); break;	//270度回転コピー
	}
	if(bMirror) Vrev();		//上下反転
	CBitmap bitmap;
	bitmap.CreateBitmap(m_sizeBmp.cx,m_sizeBmp.cy,1,1,m_pbyteBuff);
	CDC dcMem;						//メモリデバイスコンテキストオブジェクトの作成

	dcMem.CreateCompatibleDC(pDC);	//pDCと互換性のあるメモリデバイスコンテキスト
	dcMem.SelectObject(&bitmap);

	int expandOffset=nVExt/nWExt/2;
	//出力位置の計算
	x=(x*nVExt)/nWExt-expandOffset;
	y=(y*nVExt)/nWExt-expandOffset;
	
	//出力サイズの計算
	sizeOut.cx=(m_sizeBmp.cx*nVExt)/nWExt;
	sizeOut.cy=(m_sizeBmp.cy*nVExt)/nWExt;
	//
	if(nVExt<nWExt){ sizeOut.cx++; sizeOut.cy++; }

  
	if(nMode&DRAW_ON || nMode&DRAW_TEMP){
		CBrush brush;
		brush.CreateSolidBrush(col);
		CBrush* pOldBrush = pDC->SelectObject(&brush);
//		colOldBk	= pDC->SetBkColor(col);	//BkColorの設定（実際にはビットマップの色)
//		colOldText	= pDC->SetTextColor(0);	//TextColorの設定
		pDC->SetStretchBltMode(WHITEONBLACK);
  		pDC->StretchBlt(       x,y,sizeOut.cx,  sizeOut.cy,
			            &dcMem,0,0,m_sizeBmp.cx,m_sizeBmp.cy,
						0x00E20746);
				// [P]   1   1   1   1   0   0   0   0
				// [S]   1   1   0   0   1   1   0   0
				// [D]   1   0   1   0   1   0   1   0
				// -------------------------------------
				//       1   1   1   0   0   0   1   0  ----> 0xE2
//		pDC->SetBkColor(colOldBk);			//BkColorの復帰
//		pDC->SetTextColor(colOldText);		//TextColorの復帰
		pDC->SelectObject(pOldBrush);
	}else if(nMode&DRAW_OFF){
		colOldBk	= pDC->SetBkColor(RGB(255,255,255));//BkColorの設定（実際には文字の色)
		colOldText	= pDC->SetTextColor(0);	//TextColorの設定


		dcMem.BitBlt(0,0,m_sizeBmp.cx,m_sizeBmp.cy,&dcMem,0,0,DSTINVERT);
		int oldbkmode = pDC->SetBkMode(TRANSPARENT);
		pDC->SetStretchBltMode(BLACKONWHITE);

		pDC->StretchBlt(x,y,sizeOut.cx,  sizeOut.cy,
		            &dcMem,0,0,m_sizeBmp.cx,m_sizeBmp.cy,
						SRCAND);
		pDC->SetBkMode(oldbkmode);

//		pDC->StretchBlt(x,y,sizeOut.cx,  sizeOut.cy,
//		            &dcMem,0,0,m_sizeBmp.cx,m_sizeBmp.cy,
//						0x00220326L);

		

		pDC->SetBkColor(colOldBk);			//BkColorの復帰
		pDC->SetTextColor(colOldText);		//TextColorの復帰

	}else{
		TRACE("\n不正な描画モード(CPattern::PutPtn())");
	}
}


