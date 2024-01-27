/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

///////////////////////////////////////////////////////////
// BSchのフォントクラスのインプリメント

#include "stdafx.h"
#include "BSch.h"
#include "BSchFont.h"
//#include "complib.h"
#include "Global.h"
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
	//Init88Font();
	//InitWinFont();
	//m_pBmpBit=new unsigned TCHAR [4096];
	m_pFontH = NULL;
	m_pFontV = NULL;
	m_VExt =-1;
	m_WExt =-1;
}

////////////////////////////////////////////////////////////////////////////////
// CBSchFont のデストラクタ
CBSchFont::~CBSchFont()
{
	delete m_pFontH;
	delete m_pFontV;
//	delete[]pFont88v;
//	delete[]m_pBmpBit;
}



LOGFONT CBSchFont::logfont()
{
	return m_logfont;
}

void  CBSchFont::setLogFont(LOGFONT lf)
{
	m_logfont = lf;
	m_VExt =-1;
	m_WExt =-1;
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
		int  nWExt		/* =1		*/,//表示倍率(Window)
		BOOL  bUseUpperLine	/* =TRUE */)	//上線の使用

{
	COLORREF colOldText;//,colOldBk;
	CSize size,sizeLog;
	int      nOldBkMode;
	CFont *pFont;
	TCHAR buff[1025];

	bool upperLine=false;
	const TCHAR *pch;
	TCHAR ch;
	if(nStrLen<0)nStrLen = _tcslen(lpstr);
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
		//TRACE("notUL %s\n",buff);
	}

	


//	if(nStrLen<0) m_nStrLen=lstrlen(lpstr);		//文字列長の設定
//	else		  m_nStrLen=nStrLen;
	if(m_nStrLen==0) return CSize(0,0);		//文字列長が０なら何もしない
//	if(m_nStrLen>255) m_nStrLen=255;//文字列長は最大255文字
	



	if(nVExt != m_VExt || nWExt != m_WExt||m_pFontH==NULL||m_pFontV==NULL){
		delete m_pFontH;
		delete m_pFontV;
		m_pFontH = new CFont;
		m_pFontV = new CFont;
		LOGFONT lf = m_logfont;
		if(lf.lfHeight>0) lf.lfHeight = -lf.lfHeight;
		lf.lfOutPrecision = OUT_TT_ONLY_PRECIS;	
		lf.lfHeight = lf.lfHeight*nVExt/nWExt;
		if(lf.lfHeight==0) lf.lfHeight = -1;	//2014/04/13
		lf.lfCharSet=DEFAULT_CHARSET;
		m_pFontH->CreateFontIndirect(&lf);	//フォントクラスの初期化
		lf.lfEscapement = 900;
		lf.lfOrientation = 900;
		m_pFontV->CreateFontIndirect(&lf);	//フォントクラスの初期化
		m_VExt = nVExt;
		m_WExt = nWExt;
		if(bVert){
			pFont=pDC->SelectObject(m_pFontV);			//垂直フォントの選択
		}else{
			pFont=pDC->SelectObject(m_pFontH);			//水平フォントの選択
		}
		TEXTMETRIC tm;
		pDC->GetTextMetrics(&tm);
		m_vOffset = tm.tmHeight - tm.tmDescent;
	}else{
		if(bVert){
			pFont=pDC->SelectObject(m_pFontV);			//垂直フォントの選択
		}else{
			pFont=pDC->SelectObject(m_pFontH);			//水平フォントの選択
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
//	if(!upperLine) TRACE("notUL %s\n",buff);
	if(upperLine){
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

	int nLength;

	if(nStrLen<0) nLength=lstrlen(lpstr);		//文字列長の設定
	else		  nLength=nStrLen;
	if(nLength==0) return CSize(0,0);			//文字列長が０なら何もしない
	if(nLength>255) nLength=255;
	
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
	if(lf.lfHeight==0) lf.lfHeight = -1; //2014/04/13
	if(bVert){
		lf.lfEscapement = 900;
		lf.lfOrientation = 900;
	}
	x=x*nVExt/nWExt;
	y=y*nVExt/nWExt;

	pFont->CreateFontIndirect(&lf);	//フォントクラスの初期化
	pFont=pDC->SelectObject(pFont);					//フォントの選択
	size=pDC->GetTextExtent(lpstr,nLength);		//描画サイズを得る

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
		pDC->TextOut(x,y,lpstr,nLength);				//文字描画
		pDC->SetBkMode(nOldBkMode);						//背景描画モードの復帰
		pDC->SetTextColor(colOldText);					//TextColorの復帰
	}else if(nMode&DRAW_OFF){
		colOldText	= pDC->SetTextColor(0);				//TextColorの設定（黒で描画）
		nOldBkMode  = pDC->SetBkMode(TRANSPARENT);		//描画は透過モード
		pDC->TextOut(x,y,lpstr,nLength);				//文字描画
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


CSize CBSchFont::PutWinFontML(
		CDC* pDC,					//出力先のデバイスコンテキスト
		int x,int y,				//出力位置
		LPCTSTR lpstr,				//出力文字列
		int nStrLen,				//文字列長
		LOGFONT* plf,
		int width,
		CSize& size1Line,
		COLORREF col/* =RGB(255,255,255) */,		//出力色
		UINT nMode	/* =DRAW_ON */,		//描画モード
		int  nVExt	/* =1		*/,		//表示倍率(View)
		int  nWExt	/* =1		*/)		//表示倍率(Window)

{
	COLORREF colOldText;//,colOldBk;
	int      nOldBkMode;
//	CSize	 size,sizeLog;
	CSize	 sizeLog;

	int nLength;

	if(nStrLen<0) nLength=lstrlen(lpstr);		//文字列長の設定
	else		  nLength=nStrLen;
	if(nLength==0) return CSize(0,0);			//文字列長が０なら何もしない
//	if(nLength>255) nLength=255;
	
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
	if(lf.lfHeight==0) lf.lfHeight = -1;	//2014/04/13
	//if(bVert){
	//	lf.lfEscapement = 900;
	//	lf.lfOrientation = 900;
	//}
	x=x*nVExt/nWExt;
	y=y*nVExt/nWExt;
	if(width<0)width=10000;
	width = width*nVExt/nWExt;

	pFont->CreateFontIndirect(&lf);	//フォントクラスの初期化
	pFont=pDC->SelectObject(pFont);					//フォントの選択

	size1Line=pDC->GetTextExtent(lpstr,nLength);	//1ライン時の描画サイズを得る

	//int nPointPosH = nPointPos&3;
	//int nPointPosV = (nPointPos>>2)&3;
	int vOffset;

	vOffset = size1Line.cy;


	y-=vOffset;

	int right = x+width;
	int bottom = y+10000;

	CRect rc(x,y,right,bottom);


	UINT nFormat =DT_NOPREFIX|DT_TOP|DT_LEFT|DT_WORDBREAK|DT_CALCRECT;

	pDC->DrawText(lpstr,nLength,&rc,nFormat);	//描画はしない。
	nFormat &= ~DT_CALCRECT;
	nFormat |= DT_NOCLIP;

	if(nMode&DRAW_ON || nMode&DRAW_TEMP){
		colOldText	= pDC->SetTextColor(col);			//TextColorの設定
		nOldBkMode  = pDC->SetBkMode(TRANSPARENT);		//描画は透過モード
		//pDC->TextOut(x,y,lpstr,nLength);				//文字描画
		pDC->DrawText(lpstr,nLength,&rc,nFormat);
		pDC->SetBkMode(nOldBkMode);						//背景描画モードの復帰
		pDC->SetTextColor(colOldText);					//TextColorの復帰
	}else if(nMode&DRAW_OFF){
		colOldText	= pDC->SetTextColor(0);				//TextColorの設定（黒で描画）
		nOldBkMode  = pDC->SetBkMode(TRANSPARENT);		//描画は透過モード
		//pDC->TextOut(x,y,lpstr,nLength);				//文字描画
		pDC->DrawText(lpstr,nLength,&rc,nFormat);
		pDC->SetBkMode(nOldBkMode);						//背景描画モードの復帰
		pDC->SetTextColor(colOldText);					//TextColorの復帰
	}else{
		TRACE("\n不正な描画モード(CBSchFont::PutWinFont())");
	}
	pFont=pDC->SelectObject(pFont);					//フォントの復帰
	//if(width>size1Line.cx){
	//	width = size1Line.cx;
	//}
	sizeLog=rc.Size();//CSize(width,drawHeight);
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
	_tcsncpy(lplf->lfFaceName,rFaceName,LF_FACESIZE);
	lplf->lfFaceName[LF_FACESIZE-1]='\0';
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
CBSchFont* g_pFont;			//BSchフォント描画クラス
CBSchFont* g_pFontName;		//部品名、部品番号の描画


//////////////////////////////////////////////////////////////////////
//グローバル関数

#define COMMENT_FONT_SECTION _T("CommentFont")
//レジストリからコメントのデフォルトのフォント情報を得る
BOOL g_IniReadFontInfo(LPLOGFONT lplf)
{
	return CBSchFont::IniReadFontInfo(COMMENT_FONT_SECTION,lplf,_T("Tahoma"),16);
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

#define NAME_FONT_SECTION _T("CompNameFont")
//レジストリから部品名番号のフォント情報を得る
BOOL g_IniReadNameFontInfo(LPLOGFONT lplf)
{
	return CBSchFont::IniReadFontInfo(NAME_FONT_SECTION,lplf,_T("Tahoma"),9);
}

//レジストリに部品名番号のフォント情報を書き込む
BOOL g_IniWriteNameFontInfo(LPLOGFONT lplf)
{
	return CBSchFont::IniWriteFontInfo(NAME_FONT_SECTION,lplf);
}

#define QUICKPROP_FONT_SECTION _T("QuickPropFont")

//レジストリからアトリビュートプレビューのフォント情報を得る
BOOL g_IniReadQuickPropFontInfo(LPLOGFONT lplf)
{
	return CBSchFont::IniReadFontInfo(QUICKPROP_FONT_SECTION,lplf,_T("Tahoma"),10);
}
//レジストリにアトリビュートプレビューのフォント情報を書き込む
BOOL g_IniWriteQuickPropFontInfo(LPLOGFONT lplf)
{
	return CBSchFont::IniWriteFontInfo(QUICKPROP_FONT_SECTION,lplf);
}
