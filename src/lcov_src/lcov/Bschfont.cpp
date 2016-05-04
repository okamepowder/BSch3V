/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/


///////////////////////////////////////////////////////////
// BSchのフォントクラスのインプリメント

#include "stdafx.h"
#include "draw.h"
#include "BSchFont.h"
#include <assert.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


////////////////////////////////////////////////////////////////////////////////
// CBSchFont のコンストラクタ
CBSchFont::CBSchFont()
{
	Init88Font();
	//InitWinFont();
	//m_pBmpBit=new unsigned char [4096];
	m_pFontHLabel = NULL;
	m_pFontVLabel = NULL;
	m_VExt =-1;
	m_WExt =-1;
}

////////////////////////////////////////////////////////////////////////////////
// CBSchFont のデストラクタ
CBSchFont::~CBSchFont()
{
	delete m_pFontHLabel;
	delete m_pFontVLabel;
	//delete[]m_pBmpBit;
}

// ８×８ドットフォントの初期化
void CBSchFont::Init88Font()
{
	g_IniReadLabelFontInfo(&m_logfontLabel);
}


LOGFONT CBSchFont::logfontLabel()
{
	return m_logfontLabel;
}

void  CBSchFont::setLogFontLabel(LOGFONT lf)
{
	m_logfontLabel = lf;
	m_VExt =-1;
	m_WExt =-1;
}


//ピン名の描画
CSize CBSchFont::PutFontPinName(		
		CDC* pDC,				//出力先のデバイスコンテキスト
		int x,int y,			//出力位置
		LPCTSTR lpstr,			//出力文字列
		unsigned long pinNameDispFlag,
		int nStrLen,			//文字列長
		COLORREF col,			//出力色
		COLORREF col_hide,		//出力色
		UINT nMode,				//描画モード
		BOOL bVert,				//垂直フラグ
		int  nPointPos,			//指定した出力位置が 0:文字列の前 1:文字列の後ろ 2:文字列の中央
		int  nVExt,				//表示倍率(View)
		int  nWExt,				//表示倍率(Window)
		BOOL bUseUpperLine)		//上線の使用
{
	COLORREF colOldText,colOldBk;
	CSize size,sizeLog;
	int      nOldBkMode;
	CFont *pFont;
	TCHAR buff[1024];

	bool upperLine=false;
	const TCHAR *pch;
	TCHAR ch;
	if(nStrLen>1024 || nStrLen<0){
		nStrLen=1024;
	}
	
	for(pch = lpstr,m_nStrLen = 0; *pch !='\0' && m_nStrLen<nStrLen;pch++, m_nStrLen++){
		ch = *pch;
		if(ch & 0x80){
			upperLine = true;
			ch &= 0x7f;
		}
		if(ch<0x20 || ch>0x7e) ch = ' ';
		buff[m_nStrLen]=ch;
	}
	buff[m_nStrLen]=_T('\0');


//	if(nStrLen<0) m_nStrLen=lstrlen(lpstr);		//文字列長の設定
//	else		  m_nStrLen=nStrLen;
	if(m_nStrLen==0) return CSize(0,0);		//文字列長が０なら何もしない
//	if(m_nStrLen>255) m_nStrLen=255;//文字列長は最大255文字
	



	if(nVExt != m_VExt || nWExt != m_WExt||m_pFontHLabel==NULL||m_pFontVLabel==NULL){
		delete m_pFontHLabel;
		delete m_pFontVLabel;
		m_pFontHLabel = new CFont;
		m_pFontVLabel = new CFont;
		LOGFONT lf = m_logfontLabel;
		if(lf.lfHeight>0) lf.lfHeight = -lf.lfHeight;
		lf.lfOutPrecision = OUT_TT_ONLY_PRECIS;	
		lf.lfHeight = lf.lfHeight*nVExt/nWExt;
		m_pFontHLabel->CreateFontIndirect(&lf);	//フォントクラスの初期化
		lf.lfEscapement = 900;
		lf.lfOrientation = 900;
		m_pFontVLabel->CreateFontIndirect(&lf);	//フォントクラスの初期化
		m_VExt = nVExt;
		m_WExt = nWExt;
		if(bVert){
			pFont=pDC->SelectObject(m_pFontVLabel);			//垂直フォントの選択
		}else{
			pFont=pDC->SelectObject(m_pFontHLabel);			//水平フォントの選択
		}
		TEXTMETRIC tm;
		pDC->GetTextMetrics(&tm);
		m_vOffset = tm.tmHeight - tm.tmDescent;
	}else{
		if(bVert){
			pFont=pDC->SelectObject(m_pFontVLabel);			//垂直フォントの選択
		}else{
			pFont=pDC->SelectObject(m_pFontHLabel);			//水平フォントの選択
		}
	}

	size=pDC->GetTextExtent(buff,m_nStrLen);			//描画サイズを得る
	x=x*nVExt/nWExt;
	y=y*nVExt/nWExt;

	int nPointPosH = nPointPos&3;
	int nPointPosV = (nPointPos>>2)&3;
	int vOffset;

	if(nPointPosV == 0){
		vOffset = m_vOffset;
	}else if(nPointPosV == 1){
		vOffset = 0;
	}else{
		vOffset = m_vOffset/2;
	}

	if(bVert){
		x-=vOffset;
		if(nPointPosH == 1){
			y+=size.cx;
		}else if(nPointPosH == 2){
			y+=size.cx/2;
		}
	}else{
		y-=vOffset;
		if(nPointPosH == 1){
			x-=size.cx;
		}else if(nPointPosH == 2){
			x-=size.cx/2;
		}
	}
	sizeLog=size;

	colOldText	= pDC->SetTextColor(col);			//TextColorの設定
	nOldBkMode  = pDC->SetBkMode(TRANSPARENT);		//描画は透過モード

	if(pinNameDispFlag==0xFFFFFFFF){
		pDC->TextOut(x,y,buff,m_nStrLen);				//文字描画
	}else{
		int s=0;//文字描画スタートインデックス
		int i=0;//文字インデックス
		int j=0;//スラッシュのカウント
		int drawWidth;
		int x1=x;
		int y1=y;

		unsigned long flagBit = 0x00000001;
		while(1){
			if(buff[i]==_T('\0') || buff[i]==_T('/')){
				size=pDC->GetTextExtent(buff+s,i-s);
				drawWidth = size.cx;
				if((pinNameDispFlag & flagBit)!=0 || j>=32){
					 pDC->SetTextColor(col);
				}else{
					pDC->SetTextColor(col_hide);
				}
				pDC->TextOut(x1,y1,buff+s,i-s);
				if(bVert){
					y1 -= drawWidth;
				}else{
					x1 += drawWidth;
				}
				flagBit<<=1;
				s = i;
				j++;
			}
			if(buff[i]==_T('\0')) break;
			if(j>=32){
				i=m_nStrLen;
			}else{
				i++;
			}
		}

	}
	
	if(bUseUpperLine && upperLine){
		CPen penNew;
		CPen penNewHide;
		CPen* ppenOld;
		int penWidth = nVExt/nWExt;
		//描画モードによってペンの種類・色とCOPY or XORを設定する
		penNew.CreatePen(PS_SOLID,penWidth,col);
		penNewHide.CreatePen(PS_SOLID,penWidth,col_hide);
		ppenOld=pDC->SelectObject(&penNew);			//作成したペンの選択
		int i,begin,end;
		int lx,ly;
		if(bVert){
			lx = x;
		}else{
			ly = y;
		}
		i=0;
		int j=0;//スラッシュのカウント
		unsigned long flagBit = 0x00000001;
		if((pinNameDispFlag & flagBit)==0){
			pDC->SelectObject(&penNewHide);
		}
		while(i<m_nStrLen){
			if(j<32){
				if(lpstr[i] == _T('/')){
					j++;
					flagBit<<=1;
					if(j>=32 || (pinNameDispFlag & flagBit)!=0){
						pDC->SelectObject(&penNew);
					}else{
						pDC->SelectObject(&penNewHide);
					}
				}
			}

			if(lpstr[i]&0x80){
				size=pDC->GetTextExtent(buff,i);
				begin = size.cx;
				i++;
				while(i<m_nStrLen && lpstr[i]&0x80){
					i++;
				}
				size=pDC->GetTextExtent(buff,i);
				end = size.cx;	
				//TRACE("%d->%d\n",begin,end);
				if(bVert){
					pDC->MoveTo(lx,y-begin);
					pDC->LineTo(lx,y-end);
				}else{
					pDC->MoveTo(x+begin,ly);
					pDC->LineTo(x+end,ly);
				}
			}
			i++;
		}
		pDC->SelectObject(ppenOld);
	}

	
//	if(nMode&DRAW_ON || nMode&DRAW_TEMP){
//		colOldText	= pDC->SetTextColor(col);			//TextColorの設定
//		nOldBkMode  = pDC->SetBkMode(TRANSPARENT);		//描画は透過モード
//		pDC->TextOut(x,y,buff,m_nStrLen);				//文字描画
//		pDC->SetBkMode(nOldBkMode);						//背景描画モードの復帰
//		pDC->SetTextColor(colOldText);					//TextColorの復帰
//	}else if(nMode&DRAW_OFF){
//		assert(0);
//		colOldText	= pDC->SetTextColor(0);				//TextColorの設定（黒で描画）
//		nOldBkMode  = pDC->SetBkMode(TRANSPARENT);		//描画は透過モード
//		pDC->TextOut(x,y,buff,m_nStrLen);				//文字描画
//		pDC->SetBkMode(nOldBkMode);						//背景描画モードの復帰
//		pDC->SetTextColor(colOldText);					//TextColorの復帰
//	}else{
//		TRACE("\n不正な描画モード(CBSchFont::PutWinFont())");
//	}
	pDC->SetBkMode(nOldBkMode);						//背景描画モードの復帰
	pDC->SetTextColor(colOldText);					//TextColorの復帰
	pDC->SelectObject(pFont);						//フォントの復帰
	
	return sizeLog;
}


//	BSchのラベル文字列の描画
//  表示のズームに対応するため、グローバル変数の g_nVExt、g_nWExt を参照している
//  縮小・拡大表示(nVExt!=nWExt)のときは文字列相当の部分を四角形で表示する。
//      *BSchでは拡大表示をサポートしていない。
CSize CBSchFont::PutFont(	
		CDC* pDC,						//出力先のデバイスコンテキスト
		int x,int y,					//出力位置
		LPCTSTR lpstr,					//出力文字列
		int nStrLen		/* =-1		*/,	//文字列長
		COLORREF col	/* =RGB(255,255,255) */,	//出力色
		UINT nMode		/* =DRAW_ON */,	//描画モード
		BOOL bVert		/* =FALSE 	*/,	//垂直フラグ
		int  nPointPos	/* =0  		*/,	//指定した出力位置が 0:文字列の前 1:文字列の後ろ 2:文字列の中央
										//					 0:文字列の下 4:文字列の上   8:文字列の中
		int  nVExt		/* =1		*/,	//表示倍率(View)
		int  nWExt		/* =1		*/, //表示倍率(Window)
		BOOL bUseUpperLine)

{
	COLORREF colOldText,colOldBk;
	CSize size,sizeLog;
	int      nOldBkMode;
	CFont *pFont;
	TCHAR buff[1025];

	bool upperLine=false;
	const TCHAR *pch;
	TCHAR ch;
	if(nStrLen>1024 || nStrLen<0){
		nStrLen=1024;
	}
	if(bUseUpperLine){
		for(pch = lpstr,m_nStrLen = 0; *pch !='\0' && m_nStrLen<nStrLen;pch++, m_nStrLen++){
			ch = *pch;
			if(ch & 0x80){
				upperLine = true;
				ch &= 0x7f;
			}
			if(ch<0x20 || ch>0x7e) ch = ' ';
			buff[m_nStrLen]=ch;
		}
		buff[m_nStrLen]='\0';
	}else{
		for(pch = lpstr,m_nStrLen = 0; *pch !='\0' && m_nStrLen<nStrLen;pch++, m_nStrLen++){
			buff[m_nStrLen]=*pch;
		}
		buff[m_nStrLen]='\0';
	}

//	if(nStrLen<0) m_nStrLen=lstrlen(lpstr);		//文字列長の設定
//	else		  m_nStrLen=nStrLen;
	if(m_nStrLen==0) return CSize(0,0);		//文字列長が０なら何もしない
//	if(m_nStrLen>255) m_nStrLen=255;//文字列長は最大255文字
	



	if(nVExt != m_VExt || nWExt != m_WExt||m_pFontHLabel==NULL||m_pFontVLabel==NULL){
		delete m_pFontHLabel;
		delete m_pFontVLabel;
		m_pFontHLabel = new CFont;
		m_pFontVLabel = new CFont;
		LOGFONT lf = m_logfontLabel;
		if(lf.lfHeight>0) lf.lfHeight = -lf.lfHeight;
		lf.lfOutPrecision = OUT_TT_ONLY_PRECIS;	
		lf.lfHeight = lf.lfHeight*nVExt/nWExt;
		m_pFontHLabel->CreateFontIndirect(&lf);	//フォントクラスの初期化
		lf.lfEscapement = 900;
		lf.lfOrientation = 900;
		m_pFontVLabel->CreateFontIndirect(&lf);	//フォントクラスの初期化
		m_VExt = nVExt;
		m_WExt = nWExt;
		if(bVert){
			pFont=pDC->SelectObject(m_pFontVLabel);			//垂直フォントの選択
		}else{
			pFont=pDC->SelectObject(m_pFontHLabel);			//水平フォントの選択
		}
		TEXTMETRIC tm;
		pDC->GetTextMetrics(&tm);
		m_vOffset = tm.tmHeight - tm.tmDescent;
	}else{
		if(bVert){
			pFont=pDC->SelectObject(m_pFontVLabel);			//垂直フォントの選択
		}else{
			pFont=pDC->SelectObject(m_pFontHLabel);			//水平フォントの選択
		}
	}

	size=pDC->GetTextExtent(buff,m_nStrLen);			//描画サイズを得る
	x=x*nVExt/nWExt;
	y=y*nVExt/nWExt;

	int nPointPosH = nPointPos&3;
	int nPointPosV = (nPointPos>>2)&3;
	int vOffset;

	if(nPointPosV == 0){
		vOffset = m_vOffset;
	}else if(nPointPosV == 1){
		vOffset = 0;
	}else{
		vOffset = m_vOffset/2;
	}

	if(bVert){
		x-=vOffset;
		if(nPointPosH == 1){
			y+=size.cx;
		}else if(nPointPosH == 2){
			y+=size.cx/2;
		}
	}else{
		y-=vOffset;
		if(nPointPosH == 1){
			x-=size.cx;
		}else if(nPointPosH == 2){
			x-=size.cx/2;
		}
	}

	colOldText	= pDC->SetTextColor(col);			//TextColorの設定
	nOldBkMode  = pDC->SetBkMode(TRANSPARENT);		//描画は透過モード
	pDC->TextOut(x,y,buff,m_nStrLen);				//文字描画
	sizeLog=size;
	if(bUseUpperLine && upperLine){
		CPen penNew;
		CPen* ppenOld;
		int penWidth = nVExt/nWExt;
		//描画モードによってペンの種類・色とCOPY or XORを設定する
		penNew.CreatePen(PS_SOLID,penWidth,col);
		ppenOld=pDC->SelectObject(&penNew);			//作成したペンの選択
		int i,begin,end;
		int lx,ly;
		if(bVert){
			lx = x;
		}else{
			ly = y;
		}
		i=0;
		while(i<m_nStrLen){
			if(lpstr[i]&0x80){
				size=pDC->GetTextExtent(buff,i);
				begin = size.cx;
				i++;
				while(i<m_nStrLen && lpstr[i]&0x80){
					i++;
				}
				size=pDC->GetTextExtent(buff,i);
				end = size.cx;	
				//TRACE("%d->%d\n",begin,end);
				if(bVert){
					pDC->MoveTo(lx,y-begin);
					pDC->LineTo(lx,y-end);
				}else{
					pDC->MoveTo(x+begin,ly);
					pDC->LineTo(x+end,ly);
				}
			}
			i++;
		}
		pDC->SelectObject(ppenOld);
	}

	
//	if(nMode&DRAW_ON || nMode&DRAW_TEMP){
//		colOldText	= pDC->SetTextColor(col);			//TextColorの設定
//		nOldBkMode  = pDC->SetBkMode(TRANSPARENT);		//描画は透過モード
//		pDC->TextOut(x,y,buff,m_nStrLen);				//文字描画
//		pDC->SetBkMode(nOldBkMode);						//背景描画モードの復帰
//		pDC->SetTextColor(colOldText);					//TextColorの復帰
//	}else if(nMode&DRAW_OFF){
//		assert(0);
//		colOldText	= pDC->SetTextColor(0);				//TextColorの設定（黒で描画）
//		nOldBkMode  = pDC->SetBkMode(TRANSPARENT);		//描画は透過モード
//		pDC->TextOut(x,y,buff,m_nStrLen);				//文字描画
//		pDC->SetBkMode(nOldBkMode);						//背景描画モードの復帰
//		pDC->SetTextColor(colOldText);					//TextColorの復帰
//	}else{
//		TRACE("\n不正な描画モード(CBSchFont::PutWinFont())");
//	}
	pDC->SetBkMode(nOldBkMode);						//背景描画モードの復帰
	pDC->SetTextColor(colOldText);					//TextColorの復帰
	pDC->SelectObject(pFont);						//フォントの復帰
	
	return sizeLog;
}

/****************************************************************************
	コメント用のWindowsフォント文字列の描画
****************************************************************************/
CSize CBSchFont::PutWinFont(
		CDC* pDC,					//出力先のデバイスコンテキスト
		int x,int y,					//出力位置
		LPCTSTR lpstr,					//出力文字列
		int nStrLen /* =-1		*/,		//文字列長
		LOGFONT* plf,
		COLORREF col/* =RGB(255,255,255) */,		//出力色
		UINT nMode	/* =DRAW_ON */,		//描画モード
		BOOL bVert		/* =FALSE 	*/,	//垂直フラグ
		int  nPointPos	/* =0  		*/,	//指定した出力位置が 0:文字列の前 1:文字列の後ろ 2:文字列の中央
										//					 0:文字列の下 4:文字列の上   8:文字列の中
		int  nVExt	/* =1		*/,		//表示倍率(View)
		int  nWExt	/* =1		*/)		//表示倍率(Window)

{
	COLORREF colOldText;//,colOldBk;
	int      nOldBkMode;
	CSize	 size,sizeLog;

	if(nStrLen<0) m_nStrLen=lstrlen(lpstr);		//文字列長の設定
	else		  m_nStrLen=nStrLen;
	if(m_nStrLen==0) return CSize(0,0);			//文字列長が０なら何もしない
	if(m_nStrLen>255) m_nStrLen=255;
	
	CFont*	pFont=new CFont;

	LOGFONT lf;
	if(!plf){
		assert(0);
		//lf = m_logfont;
	}else{
		lf = *plf;
	}

	lf.lfOutPrecision = OUT_TT_ONLY_PRECIS;
	
	lf.lfHeight = plf->lfHeight*nVExt/nWExt;
	if(bVert){
		lf.lfEscapement = 900;
		lf.lfOrientation = 900;
	}
	x=x*nVExt/nWExt;
	y=y*nVExt/nWExt;

	pFont->CreateFontIndirect(&lf);	//フォントクラスの初期化
	pFont=pDC->SelectObject(pFont);					//フォントの選択
	size=pDC->GetTextExtent(lpstr,m_nStrLen);		//描画サイズを得る

	int nPointPosH = nPointPos&3;
	int nPointPosV = (nPointPos>>2)&3;
	int vOffset;

	if(nPointPosV == 0){
		vOffset = size.cy;
	}else if(nPointPosV == 1){
		vOffset = 0;
	}else{
		vOffset = size.cy/2;
	}

	if(bVert){
		x-=vOffset;
		if(nPointPosH == 1){
			y+=size.cx;
		}else if(nPointPosH == 2){
			y+=size.cx/2;
		}
	}else{
		y-=vOffset;
		if(nPointPosH == 1){
			x-=size.cx;
		}else if(nPointPosH == 2){
			x-=size.cx/2;
		}
	}



	if(nMode&DRAW_ON || nMode&DRAW_TEMP){
		colOldText	= pDC->SetTextColor(col);			//TextColorの設定
		nOldBkMode  = pDC->SetBkMode(TRANSPARENT);		//描画は透過モード
		pDC->TextOut(x,y,lpstr,m_nStrLen);				//文字描画
		pDC->SetBkMode(nOldBkMode);						//背景描画モードの復帰
		pDC->SetTextColor(colOldText);					//TextColorの復帰
	}else if(nMode&DRAW_OFF){
		colOldText	= pDC->SetTextColor(0);				//TextColorの設定（黒で描画）
		nOldBkMode  = pDC->SetBkMode(TRANSPARENT);		//描画は透過モード
		pDC->TextOut(x,y,lpstr,m_nStrLen);				//文字描画
		pDC->SetBkMode(nOldBkMode);						//背景描画モードの復帰
		pDC->SetTextColor(colOldText);					//TextColorの復帰
	}else{
		TRACE("\n不正な描画モード(CBSchFont::PutWinFont())");
	}
	pFont=pDC->SelectObject(pFont);					//フォントの復帰
	sizeLog=size;
	delete pFont;
	return sizeLog;

}





BOOL CBSchFont::IniReadFontInfo(const TCHAR* sectionName, LPLOGFONT lplf,const TCHAR* defFont,int defHeight)
{
	int n;
	char *p;
	CString rFaceName;
	
	p=(char*)lplf;	//LOGFONT構造体の０イニシャライズ
	for(n=0;n<sizeof(LOGFONT);n++) *p++=0;

	lplf->lfHeight=AfxGetApp()->GetProfileInt(sectionName,_T("Height"),defHeight); 	//フォントの高さを得る。
	lplf->lfWeight=AfxGetApp()->GetProfileInt(sectionName,_T("IsBold"),FW_REGULAR);	//フォントのウエイトを得る。デフォルト値は0
	lplf->lfItalic=AfxGetApp()->GetProfileInt(sectionName,_T("IsItalic"),0);		//フォントのイタリック情報を得る。デフォルト値は0
//	lplf->lfCharSet=AfxGetApp()->GetProfileInt(sectionName,"CharSet",SHIFTJIS_CHARSET);	//フォントのキャラクタセット情報を得る。デフォルト値はSHIFTJIS_CHARSET
	lplf->lfPitchAndFamily=0;//AfxGetApp()->GetProfileInt(sectionName,"PitchAndFamily",0);	//フォントのピッチ＆ファミリ情報を得る。デフォルトは0
	rFaceName=AfxGetApp()->GetProfileString(sectionName,_T("FaceName"),defFont);	//フォントのFaceNameを得る。
	lstrcpy(lplf->lfFaceName,rFaceName);
	return TRUE;
}

//レジストリにフォント情報を書き込む
BOOL CBSchFont::IniWriteFontInfo(const TCHAR* sectionName, LPLOGFONT lplf)
{
	AfxGetApp()->WriteProfileInt(sectionName,_T("Height"),abs(lplf->lfHeight));		//フォントの高さをセットする
	AfxGetApp()->WriteProfileInt(sectionName,_T("IsBold"),lplf->lfWeight);			//フォントのウエイトをセットする
	AfxGetApp()->WriteProfileInt(sectionName,_T("IsItalic"),lplf->lfItalic);		//フォントのイタリック情報をセットする
//	AfxGetApp()->WriteProfileInt(sectionName,"CharSet",lplf->lfCharSet);		//フォントのキャラクタセット情報をセットする
//	AfxGetApp()->WriteProfileInt(sectionName,"PitchAndFamily",lplf->lfPitchAndFamily);	//フォントのピッチ＆ファミリ情報をセットする
	AfxGetApp()->WriteProfileString(sectionName,_T("FaceName"),lplf->lfFaceName);	//フォントのFaceNameをセットする
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//グローバル変数
CBSchFont* g_pFont;		//BSchフォント描画クラス


//////////////////////////////////////////////////////////////////////
//グローバル関数

#define COMMENT_FONT_SECTION _T("CommentFont")
//レジストリからコメントのデフォルトのフォント情報を得る
BOOL g_IniReadFontInfo(LPLOGFONT lplf)
{
	return CBSchFont::IniReadFontInfo(COMMENT_FONT_SECTION,lplf,_T("MS UI Gothic"),16);
}

//レジストリにコメントのデフォルトのフォント情報を書き込む
BOOL g_IniWriteFontInfo(LPLOGFONT lplf)
{
	return CBSchFont::IniWriteFontInfo(COMMENT_FONT_SECTION,lplf);
}

#define LABEL_FONT_SECTION _T("LabelFont")
//レジストリからラベルのフォント情報を得る
BOOL g_IniReadLabelFontInfo(LPLOGFONT lplf)
{
	return CBSchFont::IniReadFontInfo(LABEL_FONT_SECTION,lplf,_T("Tahoma"),9);
}

//レジストリにラベルのフォント情報を書き込む
BOOL g_IniWriteLabelFontInfo(LPLOGFONT lplf)
{
	return CBSchFont::IniWriteFontInfo(LABEL_FONT_SECTION,lplf);
}

			
			