/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// Print.cpp : CBSchView クラスのうちプリンタ関連の動作の定義を行います。

#include "stdafx.h"
#include <stdlib.h> //_splitpath()のためにインクルード
#include "BSch.h"
#include <assert.h>
#include <string>
#include <list>
using namespace std;
#include <atlimage.h>	//CImageのためのインクルード。GDIPLUS.DLLはリンカ指定で、遅延ロード指定をすること。



//#include "SetLib.h"

//#include "BSchObj.h"

#include "BSchDoc.h"
#include "BSchView.h"
#include "MainFrm.h"

//#include "PartLib.h"
#include "BSchFont.h"
#include "Global.h"
#include "PrintOpt.h"	//暫定仕様の印刷オプション設定ダイアログ
//#include "BSchPart.h"

#include "drawobj.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PRINT_OVERLAP 24
//#define BMP_BANDSIZE  500
/////////////////////////////////////////////////////////////////////////////
// CBSchView クラスの印刷

BOOL CBSchView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// デフォルトの印刷準備
	return DoPreparePrinting(pInfo);
}

void CBSchView::OnBeginPrinting(CDC* pDC , CPrintInfo* pInfo)
{
	// TODO: 印刷前の特別な初期化処理を追加してください。
	TRACE("\nTRACE:OnBeginPrinting()");
	int nRes;
	nRes=IniReadPrintRes();
	nRes=15000/nRes;
	//if(nRes==0){
	//	nRes=GetDefaultPrintRes(pDC);
	//}
    m_bPrintPageFit =IniReadPrintPageFit();
		
	SetPageSize(pDC,nRes);
	int nCountPage=GetCountPage();
	pInfo->SetMaxPage(nCountPage);
}


void CBSchView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 印刷後の後処理を追加してください。
	TRACE("\nTRACE:OnEndPrinting()");
}

//印刷のための描画フラグの準備
//	1997/01/28新設
void CBSchView::PrepareDrawFlagForPrint(CDC* pDC)
{
	TRACE("TRACE:PrepareDrawFlagForPrint()\n");

	int nPrintRes=IniReadPrintRes();
	nPrintRes=15000/nPrintRes;

	//if(nPrintRes==0 && pDC!=NULL){
	//	nPrintRes=GetDefaultPrintRes(pDC);
	//}

	//m_bPrintPageFit =IniReadPrintPageFit();


	//印刷用の設定
	if(pDC){
		if(m_bPrintPageFit){
			double dmx=(double)m_sizePageDevicePixel.cx/m_sizePrintPixel.cx;
			double dmy=(double)m_sizePageDevicePixel.cy/m_sizePrintPixel.cy;
			double dm=(dmx<dmy ? dmx : dmy);
			m_nPrintVExt=(int)(dm*1000);
			m_nPrintWExt=1000;
		}else{
			int nDpi=pDC->GetDeviceCaps(LOGPIXELSX);
			m_nPrintVExt = nDpi;
			m_nPrintWExt = nPrintRes;
		}
	}else{
		m_nPrintVExt=1;
		m_nPrintWExt=1;
	}
	m_bPrintFrame	=AfxGetApp()->GetProfileInt(_T("Option"),_T("PrintSheetFrame"),TRUE);
	m_bBitmapPrint	=AfxGetApp()->GetProfileInt(_T("Option"),_T("PrintBitmap"),FALSE);
	m_bPrintColor	=AfxGetApp()->GetProfileInt(_T("Option"),_T("PrintColor"),FALSE);
	
}

//印刷前にPrepareDrawFlagForPrint()で操作した描画フラグを元に戻す	
void CBSchView::PopDrawFlagForPrint()
{
}


//標準の解像度を得る 1998/07/04
int CBSchView::GetDefaultPrintRes(CDC* pDC)
{
	int nDpi,nDpiY;
	//解像度を得る
	nDpi=pDC->GetDeviceCaps(LOGPIXELSX);
	nDpiY=pDC->GetDeviceCaps(LOGPIXELSX);
	if(nDpi>nDpiY)nDpi=nDpiY;
	int mul=nDpi/150;
	if(mul==0) mul+=1;
	return nDpi/mul;
}

//ページの各座標を求める
void CBSchView::SetPageSize(CDC* pDC,int nRes)
{
	int nX,nY;
	int nDpiX,nDpiY;
//	int nMarginX;
//	int nMarginY;

	
	//ピクセル数と解像度を得る
	nX   =pDC->GetDeviceCaps(HORZRES);
	nY   =pDC->GetDeviceCaps(VERTRES);
	nDpiX=pDC->GetDeviceCaps(LOGPIXELSX);
	nDpiY=pDC->GetDeviceCaps(LOGPIXELSX);
	int px=pDC->GetDeviceCaps(PHYSICALOFFSETX);
	int py=pDC->GetDeviceCaps(PHYSICALOFFSETY);
	int pw=pDC->GetDeviceCaps(PHYSICALWIDTH);
	int ph=pDC->GetDeviceCaps(PHYSICALHEIGHT);


	TRACE("HORZRES,VERTRES : %d,%d\n",nX,nY);
	TRACE("LOGPIXELSX,LOGPIXELSX : %d,%d\n",nDpiX,nDpiY);
	TRACE("PHYSICALOFFSETX,PHYSICALOFFSETY : %d,%d\n",px,py);
	TRACE("PHYSICALWIDTH,PHYSICALHEIGHT : %d,%d\n",pw,ph);

	//    2005/09/11追加>>>>>>
	int left,upper,right,lower;
	IniReadPrintMargin(left,upper,right,lower);

	left  = left  * nDpiX * 10 /254;
	right = right * nDpiX * 10 /254;
	upper = upper * nDpiY * 10 /254;
	lower = lower * nDpiY * 10 /254;

	left -= px;
	if(left<0) left=0;
	upper -= py;
	if(upper<0) upper=0;

	int physicalRightMargin=pw-(px+nX);
	if(physicalRightMargin<0) physicalRightMargin=0;

	int physicalLowerMargin=ph-(py+nY);
	if(physicalLowerMargin<0) physicalLowerMargin=0;

	right -= physicalRightMargin;
	if(right<0) right=0;

	lower -= physicalLowerMargin;
	if(lower<0) lower=0;

	if((left+right)>(nX/2)){
		left=right=0;
	}

	if((upper+lower)>(nY/2)){
		upper=lower=0;
	}


	//印刷の左上のピクセルの決定
	m_pointPageLT.x=left;
	m_pointPageLT.y=upper;
	m_sizePageDevicePixel.cx=nX-(left+right);
	m_sizePageDevicePixel.cy=nY-(upper+lower);

	//１ページに出力可能な図面上でのサイズの決定
	if(m_bPrintPageFit){
		CBSchDoc* pDoc = GetDocument();
		m_sizePrintPixel=pDoc->GetSheetSize();	//図面サイズを得る
	}else{
		m_sizePrintPixel.cx=m_sizePageDevicePixel.cx*nRes/nDpiX;
		m_sizePrintPixel.cy=m_sizePageDevicePixel.cy*nRes/nDpiY;
	}
}

//総ページ数を得る
int CBSchView::GetCountPage()
{
	m_nPageX=1;
	m_nPageY=1;

	if(m_bPrintPageFit)return 1;

	int n,m;
	CBSchDoc* pDoc = GetDocument();
	CSize size;
	size=pDoc->GetSheetSize();	//図面サイズを得る
	n=size.cx-m_sizePrintPixel.cx;
	if(n>0){
		m=m_sizePrintPixel.cx-PRINT_OVERLAP;
		m_nPageX+=(n+(m-1))/m;
	}
	n=size.cy-m_sizePrintPixel.cy;
	if(n>0){
		m=m_sizePrintPixel.cy-PRINT_OVERLAP;
		m_nPageY+=(n+(m-1))/m;
	}
	return m_nPageX*m_nPageY;
}


void CBSchView::OnFilePrint() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	ChangeTool(m_nCurrentTool,m_nCurrentTool);
//	CScrollView::OnFilePrint();
	((CBSchApp*)AfxGetApp())->UpdatePrinterSelection(FALSE);
	preparePrintVars();
	CScrollView::OnFilePrint();
	((CBSchApp*)AfxGetApp())->IniWritePrinterSetup();
}

void CBSchView::OnFilePrintPreview() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	ChangeTool(m_nCurrentTool,m_nCurrentTool);
	((CBSchApp*)AfxGetApp())->UpdatePrinterSelection(FALSE);
	preparePrintVars();
	CScrollView::OnFilePrintPreview();
}

#define FOOTER_H_MARGIN	2

void CBSchView::preparePrintVars()
{

	CTime time = CTime::GetCurrentTime();
	CString s = time.Format(_T("%c"));
	g_printTime = s;

	DWORD dwMode = DRAW_ON|DRAW_FOR_PRINT|DRAW_FOR_PRINTER;

	//SXBSchComment footer;
	m_bPrintFooter = false;
	if(AfxGetApp()->GetProfileInt(_T("Option"),_T("PrintFooter"),FALSE)!=FALSE){
		CString strFooter = AfxGetApp()->GetProfileString(_T("Option"),_T("FooterText"),_T(""));
		if(_tcslen(strFooter)>0){
			m_bPrintFooter = true;
			LOGFONT logfont;
			g_IniReadFontInfo(&logfont);

			m_cmntFooter.setFontSize(abs(logfont.lfHeight));
			m_cmntFooter.setFontBold(logfont.lfWeight>=FW_BOLD);
			m_cmntFooter.setFontItalic(logfont.lfItalic!=0);
			m_cmntFooter.setFontName(logfont.lfFaceName);

			m_cmntFooter.setText(strFooter);
			m_cmntFooter.setEnableTag(true);
			m_cmntFooter.setP1(SPoint(100,100));
			CClientDC dc(this);	
			CDC dcMem;						
			dcMem.CreateCompatibleDC(&dc);
			g_DrawComment(&dcMem,&m_cmntFooter,dwMode,4,1);
			SSize sizeDraw = m_cmntFooter.SizeDraw();
			TRACE(_T("FOOTER SIZE (%d, %d)\n"),sizeDraw.w(),sizeDraw.h());
			CBSchDoc* pDoc = GetDocument();
			CSize sizeSheet=pDoc->GetSheetSize();	//図面サイズを得る
			int x,y;
			y = sizeSheet.cy+FOOTER_H_MARGIN+sizeDraw.h();
			x = sizeSheet.cx-20-+sizeDraw.w();
			m_cmntFooter.setP1(SPoint(x,y));

		}
	}
}


/////////////////////////////////////////////////////////////
//   ページ印刷の実際
//
// ** Bitmap print **
// １．ページ番号をもとにページに印刷する矩形範囲を得る
// ２．１で得た矩形範囲大のモノクロビットマップを作成する
// ３．画面とコンパチなメモリデバイスコンテキストを作成する
//      (プリンタとコンパチだと何故か描画ができないことがあった)
// ４．メモリデバイスコンテキストに２で作成したビットマップを選択
// ５．メモリデバイスコンテキストのWindowOrgを移動
// ６．描画
// ７．メモリデバイスコンテキストのWindowOrgを(0,0)に戻す
void CBSchView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	
	CPoint pointOrigin;
	CSize sizePage;
	CSize sizeSheet;

	PrepareDrawFlagForPrint(pDC); // 1997/01/28




	int nPage=pInfo->m_nCurPage-1;	//印刷するページ
	pointOrigin.y=(m_sizePrintPixel.cy-PRINT_OVERLAP)*(nPage/m_nPageX);
	pointOrigin.x=(m_sizePrintPixel.cx-PRINT_OVERLAP)*(nPage%m_nPageX);

	CBSchDoc* pDoc = GetDocument();
	sizeSheet=pDoc->GetSheetSize();	//図面サイズを得る
	if((pointOrigin.x+m_sizePrintPixel.cx)>sizeSheet.cx){
		sizePage.cx=sizeSheet.cx-pointOrigin.x;
	}else{
		sizePage.cx=m_sizePrintPixel.cx;
	}
	if((pointOrigin.y+m_sizePrintPixel.cy)>sizeSheet.cy){
		sizePage.cy=sizeSheet.cy-pointOrigin.y;
	}else{
		sizePage.cy=m_sizePrintPixel.cy;
	}

//	DWORD dwMode = (m_bPrintColor ? DRAW_ON : DRAW_OFF);
	DWORD dwMode;
	if(m_bPrintColor){	// && m_COL_BG==RGB(255,255,255)){
		dwMode = DRAW_ON;
	}else{
		dwMode = DRAW_OFF;
	}

	if(g_bDisplayNcPinMark)dwMode|=DRAW_NC_MARK;

	if(!m_bBitmapPrint){
		//pDC->SetTextColor(RGB(0,0,0));
		//pDC->SetBkColor(RGB(255,255,255));
		int printX1,printY1,printX2,printY2;
		printX1 = pointOrigin.x*m_nPrintVExt/m_nPrintWExt;
		printY1 = pointOrigin.y*m_nPrintVExt/m_nPrintWExt;
		printX2	= sizePage.cx*m_nPrintVExt/m_nPrintWExt+printX1;
		printY2	= sizePage.cy*m_nPrintVExt/m_nPrintWExt+printY1;

		pDC->SetWindowOrg(printX1-m_pointPageLT.x,printY1-m_pointPageLT.y);



		pDC->SelectClipRgn(NULL);

		int frameWidth = (m_nPrintVExt/m_nPrintWExt)/2;	//最外枠線の線幅の半分

		pDC->IntersectClipRect(printX1-frameWidth,printY1-frameWidth,
							   printX2+frameWidth,printY2+frameWidth);

		CRect rcClip=CRect( pointOrigin.x,pointOrigin.y,
						pointOrigin.x+sizePage.cx,pointOrigin.y+sizePage.cy);

		
		//Version 0.63 印刷時の背景色
		if(m_bPrintColor && m_COL.colBG!=RGB(255,255,255) ){
			if(AfxGetApp()->GetProfileInt(_T("Option"),_T("PrintBgWhite"),1)!=1){
				DrawPrintBg(pDC,dwMode,m_nPrintVExt,m_nPrintWExt,NULL);
			}
		}


		if(m_bPrintFrame){	
			DrawSheetFrame(pDC,dwMode,m_nPrintVExt,m_nPrintWExt,rcClip);
		}
		//DWORD dwMode = DRAW_OFF;
		if(!AfxGetApp()->GetProfileInt(_T("Option"),_T("PrintPartNum"),TRUE)){
			dwMode |= DRAW_INH_PARTNUM;
		}
		if(!AfxGetApp()->GetProfileInt(_T("Option"),_T("PrintPartName"),TRUE)){
			dwMode |= DRAW_INH_PARTNAME;
		}
		dwMode |= (DRAW_FOR_PRINT|DRAW_FOR_PRINTER);
		DrawMainData(pDC,pDoc,dwMode,m_nPrintVExt,m_nPrintWExt,rcClip);

		if(m_bPrintFooter){
			pDC->SelectClipRgn(NULL);
			g_DrawComment(pDC,&m_cmntFooter,dwMode,m_nPrintVExt,m_nPrintWExt);
		}
	}else{

		int drawOrgX		= pointOrigin.x*2;
		int drawOrgY		= pointOrigin.y*2;
		int drawSizeWidth	= sizePage.cx*2;
		int drawSizeHeight	= sizePage.cy*2;
		

		CBitmap bmp;
		CBitmap* pOldBmp;
		//描画用メモリデバイスコンテキストの作成
		CClientDC dc(this);	
		CDC dcMem;						
		dcMem.CreateCompatibleDC(&dc);
		bmp.CreateBitmap(drawSizeWidth,drawSizeHeight,1,1,NULL);
		pOldBmp=dcMem.SelectObject(&bmp);
		COLORREF colBk=dcMem.GetBkColor();//白で初期化
		dcMem.FillSolidRect(0,0,drawSizeWidth,drawSizeHeight,RGB(255,255,255));
		dcMem.SetBkColor(colBk);
		dcMem.SetWindowOrg(drawOrgX,drawOrgY);		//左上隅の論理座標を設定
		CRect rcClip=CRect( pointOrigin.x,pointOrigin.y,
						pointOrigin.x+sizePage.cx, pointOrigin.y+sizePage.cy);
		//実際の描画
		if(m_bPrintFrame)	DrawSheetFrame(&dcMem,DRAW_OFF,2,1,rcClip);
		dwMode = DRAW_OFF;
		if(!AfxGetApp()->GetProfileInt(_T("Option"),_T("PrintPartNum"),TRUE)){
			dwMode |= DRAW_INH_PARTNUM;
		}
		if(!AfxGetApp()->GetProfileInt(_T("Option"),_T("PrintPartName"),TRUE)){
			dwMode |= DRAW_INH_PARTNAME;
		}
		dwMode |= (DRAW_FOR_PRINT|DRAW_FOR_PRINTER);
		DrawMainData(&dcMem,pDoc,dwMode,2,1,rcClip);
		//プリンタへの転送
		dcMem.SetWindowOrg(0,0);
		pDC->SetTextColor(RGB(0,0,0));		//1997/11/13
		pDC->SetBkColor(RGB(255,255,255));	//1997/11/13
		pDC->StretchBlt(m_pointPageLT.x,m_pointPageLT.y,
						sizePage.cx*m_nPrintVExt/m_nPrintWExt,
						sizePage.cy*m_nPrintVExt/m_nPrintWExt,
//						sizePage.cx*m_nPrintMagX/m_nPrintMagX2,
//						sizePage.cy*m_nPrintMagY/m_nPrintMagY2,
						&dcMem,
						0,0,
						drawSizeWidth,drawSizeHeight,
						SRCCOPY);
		dcMem.SelectObject(pOldBmp);
		//dcとdcMemとbmpの削除は自動的に行われる、はず。

		if(m_bPrintFooter){
			int printX1,printY1;
			printX1 = pointOrigin.x*m_nPrintVExt/m_nPrintWExt;
			printY1 = pointOrigin.y*m_nPrintVExt/m_nPrintWExt;
		
			pDC->SetWindowOrg(printX1-m_pointPageLT.x,printY1-m_pointPageLT.y);
	
			//pDC->SelectClipRgn(NULL);
			g_DrawComment(pDC,&m_cmntFooter,dwMode,m_nPrintVExt,m_nPrintWExt);
		}


	}

	PopDrawFlagForPrint();	// 1997/01/28

}

void CBSchView::OnFileExpoEmf()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	//一時データがあれば、エクスポートする前にフィックスする
	ChangeTool(m_nCurrentTool,m_nCurrentTool);

	//現在のファイルの名前を得る
	CBSchDoc* pDoc = GetDocument();
	CString rTitle=pDoc->GetTitle();
	TCHAR szName[_MAX_FNAME];
	::_tsplitpath(rTitle,NULL,NULL,szName,NULL);

	//ファイルダイアログを起動してファイル名を決める
	CString rCaption;
	CFileDialog dlg(FALSE,				//保存ダイアログ
					_T("emf"),				//拡張子が付けられなかった場合のデフォルト
					szName,				//初期ファイル名
					OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
					_T("Meta file(*.emf)|*.EMF||"),	//フィルタ文字列
					this);				//親ウインドウ
	rCaption.LoadString(IDS_FNAME_EXPO_EMF);//ダイアログボックスのキャプションをリソースからロード
	dlg.m_ofn.lpstrTitle=rCaption;		//ファイルダイアログクラスにキャプションの設定
	if(dlg.DoModal()==IDOK){			//ファイルダイアログの実行
		CString emfName=CString(dlg.m_ofn.lpstrFile);		//ファイルダイアログからファイル名を取得して

		PrepareDrawFlagForPrint(NULL);

		HCURSOR hCursor;
		hCursor=AfxGetApp()->LoadStandardCursor(IDC_WAIT);//砂時計カーソル
		hCursor=::SetCursor(hCursor);
		if(!ExportEmf(emfName)){
			::SetCursor(hCursor);
			AfxMessageBox(IDS_EXPO_EMF_ERROR,MB_OK|MB_ICONEXCLAMATION);
		}else{
			::SetCursor(hCursor);
		}
		PopDrawFlagForPrint();
	}


}

/////////////////////////////////////////////////////////
//ビットマップファイルのExportコマンドハンドラ
void CBSchView::OnFileExpoBmp() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください


	//0.64.03で、GDIPLUS.DLLが使えるときは、CImageを使ってPNGやGIFで保存できるようにした。
	CString strFilter;
	
	//----- 2016/05/03 GDI+のない環境を前提にしない
	//if(g_bAvailableGDIplus){
	//	strFilter = "Bitmap file(*.bmp)|*.BMP|PNG file(*.png)|*.PNG||";
	//}else{
	//	strFilter = "Bitmap file(*.bmp)|*.BMP||";
	//}

	//----- 2016/05/03 pngを優先度高にする 
	strFilter = _T("PNG file(*.png)|*.PNG|Bitmap file(*.bmp)|*.BMP||");


	//一時データがあれば、エクスポートする前にフィックスする
	ChangeTool(m_nCurrentTool,m_nCurrentTool);

	//現在のファイルの名前を得る
	CBSchDoc* pDoc = GetDocument();
	CString rTitle=pDoc->GetTitle();
	TCHAR szName[_MAX_FNAME];
	::_tsplitpath(rTitle,NULL,NULL,szName,NULL);

	//ファイルダイアログを起動してファイル名を決める
	CString rCaption;
	CFileDialog dlg(FALSE,				//保存ダイアログ
					_T("png"),				//拡張子が付けられなかった場合のデフォルト
					szName,				//初期ファイル名
					OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
					strFilter,	//フィルタ文字列
					this);				//親ウインドウ
	rCaption.LoadString(IDS_FNAME_EXPO_BMP);//ダイアログボックスのキャプションをリソースからロード
	dlg.m_ofn.lpstrTitle=rCaption;		//ファイルダイアログクラスにキャプションの設定
	if(dlg.DoModal()==IDOK){			//ファイルダイアログの実行
		CString rBmpName=CString(dlg.m_ofn.lpstrFile);		//ファイルダイアログからファイル名を取得して
		TRACE(_T("\nrBmpName=%s"),(const TCHAR*)rBmpName);

		PrepareDrawFlagForPrint(NULL); // 1997/01/28
		HCURSOR hCursor;
		hCursor=AfxGetApp()->LoadStandardCursor(IDC_WAIT);//砂時計カーソル
		hCursor=::SetCursor(hCursor);
		if(!ExportBitmapFile(rBmpName)){
			::SetCursor(hCursor);
			AfxMessageBox(IDS_EXPO_BMP_ERROR,MB_OK|MB_ICONEXCLAMATION);
		}else{
			::SetCursor(hCursor);
		}
		PopDrawFlagForPrint();
	}
}


BOOL CBSchView::ExportBitmapFile(LPCTSTR pszFileName)
{
	BOOL bColorMode = AfxGetApp()->GetProfileInt(_T("Option"),_T("PrintColor"),FALSE);
	BOOL bPrintBgWhite = AfxGetApp()->GetProfileInt(_T("Option"),_T("PrintBgWhite"),1);


	bool bUseImage = true;

	int nMag = IniReadExpImageMag();

	int image_x, image_y;
	
		//----- 2016/05/03 GDI+のない環境を前提にしない
	//if(!g_bAvailableGDIplus){
	//	bUseImage = false;
	//}else{
	//	TCHAR szExtension[_MAX_EXT];
	//	::_tsplitpath(pszFileName,NULL,NULL,NULL,szExtension);
	//	if(_tcsicmp(szExtension,_T(".bmp"))==0){
	//		if(bColorMode == FALSE){
	//			bUseImage = false;
	//		}
	//	}
	//}

	//if(bUseImage){
		CSize sizeSheet;
		CBSchDoc* pDoc = GetDocument();
		sizeSheet=pDoc->GetSheetSize();


		image_x = sizeSheet.cx * nMag /100;		//2016/05/04
		image_y = sizeSheet.cy * nMag /100;		//2016/05/04
		

		CImage image;
		image.Create(image_x,image_y,24);

		HDC hdc = image.GetDC();
		CDC* pdc = CDC::FromHandle(hdc);

		CRect rcClip=CRect(0,0,image_x,image_y);
		pdc->SetWindowOrg(0,0);



		COLORREF colBk=pdc->GetBkColor();
		COLORREF colbg;
		if(bColorMode && !bPrintBgWhite){
			colbg = m_COL.colBG;
		}else{
			colbg = RGB(255,255,255);
		}

		pdc->FillSolidRect(0,0,image_x,image_y,colbg);
		//pdc->SetBkColor(colBk);
		pdc->SetWindowOrg(0,0);

		DWORD dwMode = (bColorMode ? DRAW_ON : DRAW_OFF);

		//実際の描画
		//if(m_bPrintFrame) DrawSheetFrame(pdc,dwMode,m_nPrintVExt,m_nPrintWExt,rcClip);
		if(m_bPrintFrame) DrawSheetFrame(pdc,dwMode,nMag,100,rcClip);

		//DWORD dwMode = DRAW_OFF;
		if(!AfxGetApp()->GetProfileInt(_T("Option"),_T("PrintPartNum"),TRUE)){
			dwMode |= DRAW_INH_PARTNUM;
		}
		if(!AfxGetApp()->GetProfileInt(_T("Option"),_T("PrintPartName"),TRUE)){
			dwMode |= DRAW_INH_PARTNAME;
		}
		dwMode |= DRAW_FOR_PRINT;
		if(g_bDisplayNcPinMark)dwMode|=DRAW_NC_MARK;
		DrawMainData(pdc,pDoc,dwMode,nMag,100,rcClip);

		image.Save(pszFileName);
		
		image.ReleaseDC();
		return TRUE;
	//}else{


	//	CSize sizeSheet;
	//	CBSchDoc* pDoc = GetDocument();
	//	sizeSheet=pDoc->GetSheetSize();

	//	int nBmpXbyte;	//デバイス依存のビットマップの水平方向バイト数
	//	int nBmpSize;	//デバイス依存のビットマップのサイズ
	//	int nDibXbyte;	//デバイス独立のビットマップの水平方向バイト数
	//	int nDibSize;	//デバイス依存のビットマップのサイズ

	//	nBmpXbyte=((sizeSheet.cx+15)/16)*2;
	//	nBmpSize=nBmpXbyte*sizeSheet.cy;
	//	nDibXbyte=((sizeSheet.cx+31)/32)*4;
	//	nDibSize=nDibXbyte*sizeSheet.cy;


	//	/////////////////////////////////////
	//	//BITMAPINFO
	//	int headersize = sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*2;
	//	BYTE* pbihBuff = new BYTE[headersize];
	//	BITMAPINFO* pbih =(BITMAPINFO*) pbihBuff;
	//	pbih->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	//	pbih->bmiHeader.biWidth =sizeSheet.cx;
	//	pbih->bmiHeader.biHeight=sizeSheet.cy;
	//	pbih->bmiHeader.biPlanes=1;
	//	pbih->bmiHeader.biBitCount=1;
	//	pbih->bmiHeader.biCompression=0;
	//	pbih->bmiHeader.biSizeImage=nDibSize;
	//	pbih->bmiHeader.biXPelsPerMeter=5906;	//150dpi
	//	pbih->bmiHeader.biYPelsPerMeter=5906;	//150dpi
	//	pbih->bmiHeader.biClrUsed=0;
	//	pbih->bmiHeader.biClrImportant=0;


	//	//描画用メモリデバイスコンテキストの作成
	//	CClientDC dc(this);
	//	CDC dcMem;
	//	dcMem.CreateCompatibleDC(&dc);
	//	//ビットマップの作成
	//	CBitmap bmp;
	//	CBitmap* pOldBmp;
	//	//if(!bmp.CreateCompatibleBitmap(&dcMem,sizeSheet.cx,sizeSheet.cy))return FALSE;
	//	if(!bmp.CreateBitmap(sizeSheet.cx,sizeSheet.cy,1,1,NULL))return FALSE;

	//	//ビットマップビット保存用のバッファ
	//	int bitBuffSize = nBmpXbyte*sizeSheet.cy;
	//	BYTE* bitbuff=new BYTE[nDibSize];
	//	if(!bitbuff) return FALSE;


	//	CRect rcClip=CRect(0,0,sizeSheet.cx,sizeSheet.cy);

	//	pOldBmp=dcMem.SelectObject(&bmp);
	//	dcMem.SetWindowOrg(0,0);

	//	//白で初期化
	//	COLORREF colBk=dcMem.GetBkColor();
	//	dcMem.FillSolidRect(0,0,sizeSheet.cx,sizeSheet.cy,RGB(255,255,255));
	//	dcMem.SetBkColor(colBk);
	//	dcMem.SetWindowOrg(0,0);
	//	//実際の描画
	//	if(m_bPrintFrame) DrawSheetFrame(&dcMem,DRAW_OFF,m_nPrintVExt,m_nPrintWExt,rcClip);

	//	DWORD dwMode = DRAW_OFF;
	//	if(!AfxGetApp()->GetProfileInt(_T("Option"),_T("PrintPartNum"),TRUE)){
	//		dwMode |= DRAW_INH_PARTNUM;
	//	}
	//	if(!AfxGetApp()->GetProfileInt(_T("Option"),_T("PrintPartName"),TRUE)){
	//		dwMode |= DRAW_INH_PARTNAME;
	//	}
	//	dwMode |= DRAW_FOR_PRINT;
	//	if(g_bDisplayNcPinMark)dwMode|=DRAW_NC_MARK;
	//	DrawMainData(&dcMem,pDoc,dwMode,m_nPrintVExt,m_nPrintWExt,rcClip);
	//	dcMem.SelectObject(pOldBmp);



	//	//int nGetLine1 = ::GetDIBits(dcMem.m_hDC,(HBITMAP)bmp,0,sizeSheet.cy,NULL,pbih,DIB_RGB_COLORS);

	//	//ビットの取得
	//	int nGetLine = ::GetDIBits(dcMem.m_hDC,(HBITMAP)bmp,0,sizeSheet.cy,bitbuff,pbih,DIB_RGB_COLORS);
	//	if(nGetLine==sizeSheet.cy){
	//		CFile file;
	//		if(!file.Open(pszFileName,CFile::modeCreate|CFile::modeWrite)) return FALSE;

	//		//////////////////////////////////
	//		//BITMAPFILEHEADERの出力
	//		BITMAPFILEHEADER bfh;
	//		bfh.bfType=0x4d42;
	//		bfh.bfSize= sizeof(BITMAPFILEHEADER)	//
	//				   +sizeof(BITMAPINFOHEADER)	//BITMAPINFOHEADERのサイズ
	//				   +sizeof(RGBQUAD)*2			//
	//				   +nDibSize;					//
	//		bfh.bfReserved1=bfh.bfReserved2=0;
	//		bfh.bfOffBits= sizeof(BITMAPFILEHEADER)	//
	//					  +sizeof(BITMAPINFOHEADER)	//BITMAPINFOHEADERのサイズ
	//					  +sizeof(RGBQUAD)*2;		//

	//		try{
	//			file.Write(&bfh,sizeof(BITMAPFILEHEADER));
	//		}
	//		catch(CFileException *e){
	//			e->Delete();
	//			return FALSE;
	//		}
	//	
	//		try{
	//			file.Write(pbih,headersize);
	//		}
	//		catch(CFileException *e){
	//			e->Delete();
	//			return FALSE;
	//		}


	//		try{
	//			file.Write(bitbuff,nDibSize);
	//		}
	//		catch(CFileException *e){
	//			e->Delete();
	//			delete[]bitbuff;
	//			return FALSE;
	//		}
	//	}
	//	delete[]pbihBuff;
	//	delete[]bitbuff;
	//	return TRUE;
	//}
}

BOOL CBSchView::CopyEmf()
{
	CBSchDoc* pDoc = GetDocument();
	XBSchObjList* pTmpData;
	if(!pDoc) return FALSE;

	UINT nDrawAllBit=DRAW_TEMP_DATA;

	if((pTmpData=pDoc->GetTmpList())==NULL){		//一時データのリストを得る
		return FALSE;
	}
	if(!pTmpData->size()){
		return FALSE;
	}

	CRect rc=pDoc->GetTmpRect();		//一時データの外周を得る
	//rc.left-=2; rc.top-=2; rc.right+=2; rc.bottom+=2;
	rc.left-=100; rc.top-=100; rc.right+=100; rc.bottom+=100;

	//描画用メモリデバイスコンテキストの作成
	CMetaFileDC mDC;
	CClientDC cDC(this);
	if(!mDC.CreateEnhanced(&cDC,NULL,NULL,NULL)) return FALSE;
	mDC.SetAttribDC(cDC.m_hDC);


	int nPrintVExt=2;	//20111022
	int nPrintWExt=1;

	DWORD dwMode = DRAW_OFF;
	if(AfxGetApp()->GetProfileInt(_T("Option"),_T("PrintColor"),FALSE)){
		dwMode = DRAW_ON;
	}

	if(!AfxGetApp()->GetProfileInt(_T("Option"),_T("PrintPartNum"),TRUE)){
		dwMode |= DRAW_INH_PARTNUM;
	}
	if(!AfxGetApp()->GetProfileInt(_T("Option"),_T("PrintPartName"),TRUE)){
		dwMode |= DRAW_INH_PARTNAME;
	}

	if(m_COL.colBG!=RGB(255,255,255) ){
		if(AfxGetApp()->GetProfileInt(_T("Option"),_T("PrintBgWhite"),1)!=1){
			DrawPrintBg(&mDC,dwMode,nPrintVExt,nPrintWExt,&rc);
		}
	}
	

	dwMode |= DRAW_FOR_PRINT;
	DrawListData(&mDC,pTmpData,dwMode,nPrintVExt,nPrintWExt,rc,nDrawAllBit);	//一時データのリストの描画

	HENHMETAFILE    hmeta = mDC.CloseEnhanced();

	if(!OpenClipboard()) return FALSE;
    if(!EmptyClipboard()) return FALSE;
    SetClipboardData(CF_ENHMETAFILE, hmeta);
    CloseClipboard();

	return TRUE;
	
}


BOOL CBSchView::ExportEmf(LPCTSTR pszFileName)
{
	CSize sizeSheet;
	CBSchDoc* pDoc = GetDocument();
	sizeSheet=pDoc->GetSheetSize();
	
	//描画用メモリデバイスコンテキストの作成
	CMetaFileDC mDC;
	CClientDC cDC(this);
	if(!mDC.CreateEnhanced(&cDC,pszFileName,NULL,NULL)) return FALSE;

//	CRect rcBounds = CRect(0,0,30000,20000);
//	if(!mDC.CreateEnhanced(&cDC,pszFileName,&rcBounds,"BSch3V\0BSch3V\0")) return FALSE;
	mDC.SetAttribDC(cDC.m_hDC);

	int nPrintVExt=2;	//20111022
	int nPrintWExt=1;

//	mDC.IntersectClipRect(-2,-2,sizeSheet.cx*2+2,sizeSheet.cy*2+2);
	CRect rc=CRect(0,0,sizeSheet.cx,sizeSheet.cy);

	DWORD dwMode = DRAW_OFF;
	if(AfxGetApp()->GetProfileInt(_T("Option"),_T("PrintColor"),FALSE)){
		dwMode = DRAW_ON;
	}

	if(g_bDisplayNcPinMark)dwMode|=DRAW_NC_MARK;

	//実際の描画

	//Version 0.63 印刷時の背景色
	if(m_bPrintColor && m_COL.colBG!=RGB(255,255,255) ){
		if(AfxGetApp()->GetProfileInt(_T("Option"),_T("PrintBgWhite"),1)!=1){
			DrawPrintBg(&mDC,dwMode,nPrintVExt,nPrintWExt,NULL);
		}
	}


	if(m_bPrintFrame) DrawSheetFrame(&mDC,dwMode,nPrintVExt,nPrintWExt,rc);
	//DWORD dwMode = DRAW_OFF;
	if(!AfxGetApp()->GetProfileInt(_T("Option"),_T("PrintPartNum"),TRUE)){
		dwMode |= DRAW_INH_PARTNUM;
	}
	if(!AfxGetApp()->GetProfileInt(_T("Option"),_T("PrintPartName"),TRUE)){
		dwMode |= DRAW_INH_PARTNAME;
	}
	dwMode |= DRAW_FOR_PRINT;
	DrawMainData(&mDC,pDoc,dwMode,nPrintVExt,nPrintWExt,rc);
	HENHMETAFILE hemf=mDC.CloseEnhanced();
	::DeleteEnhMetaFile(hemf);

	return TRUE;
}


///////////////////////////////////////////////////////
//印刷オプションの設定ダイアログ起動    1997/01/26 追加
//    1998/07/04 印刷解像度の標準値を得るための改訂
//    暫定仕様  将来は、プリンタの設定と統合する
void CBSchView::OnFilePrintOption() 
{
	((CBSchApp*)AfxGetApp())->UpdatePrinterSelection(FALSE);

	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CPrintOpt dlg;

	CWinApp* pApp = AfxGetApp();
	
	////標準解像度を設定
	//CPrintDialog dlgPrn(FALSE);
	//CDC dc;

	//if(	pApp->GetPrinterDeviceDefaults(&(dlgPrn.m_pd))){
	//	HDC hdc=dlgPrn.CreatePrinterDC();
	//	CDC dc;
	//	dc.Attach(hdc);
	//	//dlg.m_nDefaultRes=GetDefaultPrintRes(&dc);
	//}

	dlg.m_bPageFit=IniReadPrintPageFit();
	dlg.m_nPrintRes=IniReadPrintRes();
	dlg.m_nImageMag=IniReadExpImageMag();


	//標準解像度を設定 ここまで

	//    2005/09/11追加>>>>>>
	int left,upper,right,lower;
	IniReadPrintMargin(left,upper,right,lower);
	dlg.m_nLeftMargin	= left;
	dlg.m_nUpperMargin	= upper;
	dlg.m_nRightMargin	= right;
	dlg.m_nLowerMargin	= lower;
	//    <<<<<<2005/09/11追加


	dlg.m_bFrame  =pApp->GetProfileInt(_T("Option"),_T("PrintSheetFrame"),TRUE);
	dlg.m_bPartNum=pApp->GetProfileInt(_T("Option"),_T("PrintPartNum"),TRUE);
	dlg.m_bPartName=pApp->GetProfileInt(_T("Option"),_T("PrintPartName"),TRUE);//1998/07/22
	dlg.m_bPrintBmp=pApp->GetProfileInt(_T("Option"),_T("PrintBitmap"),FALSE);
	dlg.m_bPrintColor=pApp->GetProfileInt(_T("Option"),_T("PrintColor"),FALSE);
	dlg.m_bFooter =pApp->GetProfileInt(_T("Option"),_T("PrintFooter"),FALSE);
	dlg.m_strFooter = pApp->GetProfileString(_T("Option"),_T("FooterText"),_T(""));
	if(dlg.DoModal()==IDOK){
		pApp->WriteProfileInt(_T("Option"),_T("PrintSheetFrame"),dlg.m_bFrame);
		pApp->WriteProfileInt(_T("Option"),_T("PrintPartNum"),dlg.m_bPartNum);
		pApp->WriteProfileInt(_T("Option"),_T("PrintPartName"),dlg.m_bPartName);//1998/07/22
		pApp->WriteProfileInt(_T("Option"),_T("PrintBitmap"),dlg.m_bPrintBmp);
		pApp->WriteProfileInt(_T("Option"),_T("PrintColor"),dlg.m_bPrintColor);
		pApp->WriteProfileInt(_T("Option"),_T("PrintFooter"),dlg.m_bFooter);
		pApp->WriteProfileString(_T("Option"),_T("FooterText"),dlg.m_strFooter);
		IniWritePrintRes(dlg.m_nPrintRes);	//1998/07/04追加
		IniWritePrintPageFit(dlg.m_bPageFit);//ADD20051223
		IniWriteExpImageMag(dlg.m_nImageMag);	//2016/05/03

		//m_nPrintRes=dlg.m_nPrintRes;		//1998/07/04追加 2005/09/11コメントアウト
		m_bBitmapPrint =dlg.m_bPrintBmp;

		//    2005/09/11追加>>>>>>
		left=	dlg.m_nLeftMargin;		
		upper=	dlg.m_nUpperMargin;
		right=	dlg.m_nRightMargin;
		lower=	dlg.m_nLowerMargin;
		IniWritePrintMargin(left,upper,right,lower);
		//    <<<<<<2005/09/11追加
	}
}
