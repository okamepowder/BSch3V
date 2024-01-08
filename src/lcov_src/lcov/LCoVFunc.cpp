#include "StdAfx.h"
#include ".\lcovfunc.h"
#include "LCoV.h"

#include "pintype.h"

//PinPropsモードレスダイアログのグリッドで、ピンタイプを描画するのに使う。
//当初、グリッドで使うピンタイプドロップダウンリストをオーナードロウで
//実装しようとしたため、このように独立した描画クラスにした。

void LCoVFunc::DrawResourceBMP(CDC* pDC,int x,int y,int cx,int cy,UINT id,int sx,int sy)
{
	CBitmap bmp;
	bmp.LoadBitmap(id);
	CDC dcmem;
	dcmem.CreateCompatibleDC(pDC);
	CBitmap* pOldBmp = dcmem.SelectObject(&bmp);

	CBrush* pBrush = (CBrush*)(pDC->SelectStockObject(WHITE_BRUSH));
	pDC->BitBlt(x,y,cx,cy,&dcmem,sx,sy,0x008E1D7C);
	// ピンタイプを定義したビットマップの色が透過色と一致したときは、Destinationを変化させない
	// [P]   1   1   1   1   0   0   0   0
	// [S]   1   1   0   0   1   1   0   0
	// [D]   1   0   1   0   1   0   1   0
	// -------------------------------------
	//       1   0   0   0   1   1   1   0  ----> 0x8E
	pDC->SelectObject(pBrush);

	dcmem.SelectObject(pOldBmp);
}

void LCoVFunc::DrawPinType(CDC* pDC,int x,int y,int pintype)
{
	UINT bmpID;

	pintype &= 
		(PIN_TYPE_NOMAL |
		PIN_TYPE_SMALL |
		PIN_TYPE_ZLENG |
		PIN_TYPE_CLOCK |
		PIN_TYPE_NEGATIVE);

	switch(pintype){
		case PIN_TYPE_NEGATIVE:
			bmpID = IDB_BMP_PIN_NEGA;
			break;
		case PIN_TYPE_NEGATIVE | PIN_TYPE_CLOCK:
			bmpID = IDB_BMP_PIN_NEGACK;
			break;
		case PIN_TYPE_CLOCK:
			bmpID = IDB_BMP_PIN_CK;
			break;
		case PIN_TYPE_SMALL:
			bmpID = IDB_BMP_PIN_SMALL;
			break;
		case PIN_TYPE_ZLENG:
			bmpID = IDB_BMP_PIN_ZLENG;
			break;
		default:
			bmpID = IDB_BMP_PIN_NORM;
	}
	
	DrawResourceBMP(pDC,x,y,72,15,bmpID,3,3);

}