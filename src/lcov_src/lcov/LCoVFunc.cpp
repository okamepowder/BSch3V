#include "StdAfx.h"
#include ".\lcovfunc.h"
#include "LCoV.h"

#include "pintype.h"

//PinProps���[�h���X�_�C�A���O�̃O���b�h�ŁA�s���^�C�v��`�悷��̂Ɏg���B
//�����A�O���b�h�Ŏg���s���^�C�v�h���b�v�_�E�����X�g���I�[�i�[�h���E��
//�������悤�Ƃ������߁A���̂悤�ɓƗ������`��N���X�ɂ����B

void LCoVFunc::DrawResourceBMP(CDC* pDC,int x,int y,int cx,int cy,UINT id,int sx,int sy)
{
	CBitmap bmp;
	bmp.LoadBitmap(id);
	CDC dcmem;
	dcmem.CreateCompatibleDC(pDC);
	CBitmap* pOldBmp = dcmem.SelectObject(&bmp);

	CBrush* pBrush = (CBrush*)(pDC->SelectStockObject(WHITE_BRUSH));
	pDC->BitBlt(x,y,cx,cy,&dcmem,sx,sy,0x008E1D7C);
	// �s���^�C�v���`�����r�b�g�}�b�v�̐F�����ߐF�ƈ�v�����Ƃ��́ADestination��ω������Ȃ�
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