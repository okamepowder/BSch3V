/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/


///////////////////////////////////////////////////////////
// BSch�̃t�H���g�N���X�̃C���v�������g

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
// CBSchFont �̃R���X�g���N�^
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
// CBSchFont �̃f�X�g���N�^
CBSchFont::~CBSchFont()
{
	delete m_pFontHLabel;
	delete m_pFontVLabel;
	//delete[]m_pBmpBit;
}

// �W�~�W�h�b�g�t�H���g�̏�����
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


//�s�����̕`��
CSize CBSchFont::PutFontPinName(		
		CDC* pDC,				//�o�͐�̃f�o�C�X�R���e�L�X�g
		int x,int y,			//�o�͈ʒu
		LPCTSTR lpstr,			//�o�͕�����
		unsigned long pinNameDispFlag,
		int nStrLen,			//������
		COLORREF col,			//�o�͐F
		COLORREF col_hide,		//�o�͐F
		UINT nMode,				//�`�惂�[�h
		BOOL bVert,				//�����t���O
		int  nPointPos,			//�w�肵���o�͈ʒu�� 0:������̑O 1:������̌�� 2:������̒���
		int  nVExt,				//�\���{��(View)
		int  nWExt,				//�\���{��(Window)
		BOOL bUseUpperLine)		//����̎g�p
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


//	if(nStrLen<0) m_nStrLen=lstrlen(lpstr);		//�����񒷂̐ݒ�
//	else		  m_nStrLen=nStrLen;
	if(m_nStrLen==0) return CSize(0,0);		//�����񒷂��O�Ȃ牽�����Ȃ�
//	if(m_nStrLen>255) m_nStrLen=255;//�����񒷂͍ő�255����
	



	if(nVExt != m_VExt || nWExt != m_WExt||m_pFontHLabel==NULL||m_pFontVLabel==NULL){
		delete m_pFontHLabel;
		delete m_pFontVLabel;
		m_pFontHLabel = new CFont;
		m_pFontVLabel = new CFont;
		LOGFONT lf = m_logfontLabel;
		if(lf.lfHeight>0) lf.lfHeight = -lf.lfHeight;
		lf.lfOutPrecision = OUT_TT_ONLY_PRECIS;	
		lf.lfHeight = lf.lfHeight*nVExt/nWExt;
		m_pFontHLabel->CreateFontIndirect(&lf);	//�t�H���g�N���X�̏�����
		lf.lfEscapement = 900;
		lf.lfOrientation = 900;
		m_pFontVLabel->CreateFontIndirect(&lf);	//�t�H���g�N���X�̏�����
		m_VExt = nVExt;
		m_WExt = nWExt;
		if(bVert){
			pFont=pDC->SelectObject(m_pFontVLabel);			//�����t�H���g�̑I��
		}else{
			pFont=pDC->SelectObject(m_pFontHLabel);			//�����t�H���g�̑I��
		}
		TEXTMETRIC tm;
		pDC->GetTextMetrics(&tm);
		m_vOffset = tm.tmHeight - tm.tmDescent;
	}else{
		if(bVert){
			pFont=pDC->SelectObject(m_pFontVLabel);			//�����t�H���g�̑I��
		}else{
			pFont=pDC->SelectObject(m_pFontHLabel);			//�����t�H���g�̑I��
		}
	}

	size=pDC->GetTextExtent(buff,m_nStrLen);			//�`��T�C�Y�𓾂�
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

	colOldText	= pDC->SetTextColor(col);			//TextColor�̐ݒ�
	nOldBkMode  = pDC->SetBkMode(TRANSPARENT);		//�`��͓��߃��[�h

	if(pinNameDispFlag==0xFFFFFFFF){
		pDC->TextOut(x,y,buff,m_nStrLen);				//�����`��
	}else{
		int s=0;//�����`��X�^�[�g�C���f�b�N�X
		int i=0;//�����C���f�b�N�X
		int j=0;//�X���b�V���̃J�E���g
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
		//�`�惂�[�h�ɂ���ăy���̎�ށE�F��COPY or XOR��ݒ肷��
		penNew.CreatePen(PS_SOLID,penWidth,col);
		penNewHide.CreatePen(PS_SOLID,penWidth,col_hide);
		ppenOld=pDC->SelectObject(&penNew);			//�쐬�����y���̑I��
		int i,begin,end;
		int lx,ly;
		if(bVert){
			lx = x;
		}else{
			ly = y;
		}
		i=0;
		int j=0;//�X���b�V���̃J�E���g
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
//		colOldText	= pDC->SetTextColor(col);			//TextColor�̐ݒ�
//		nOldBkMode  = pDC->SetBkMode(TRANSPARENT);		//�`��͓��߃��[�h
//		pDC->TextOut(x,y,buff,m_nStrLen);				//�����`��
//		pDC->SetBkMode(nOldBkMode);						//�w�i�`�惂�[�h�̕��A
//		pDC->SetTextColor(colOldText);					//TextColor�̕��A
//	}else if(nMode&DRAW_OFF){
//		assert(0);
//		colOldText	= pDC->SetTextColor(0);				//TextColor�̐ݒ�i���ŕ`��j
//		nOldBkMode  = pDC->SetBkMode(TRANSPARENT);		//�`��͓��߃��[�h
//		pDC->TextOut(x,y,buff,m_nStrLen);				//�����`��
//		pDC->SetBkMode(nOldBkMode);						//�w�i�`�惂�[�h�̕��A
//		pDC->SetTextColor(colOldText);					//TextColor�̕��A
//	}else{
//		TRACE("\n�s���ȕ`�惂�[�h(CBSchFont::PutWinFont())");
//	}
	pDC->SetBkMode(nOldBkMode);						//�w�i�`�惂�[�h�̕��A
	pDC->SetTextColor(colOldText);					//TextColor�̕��A
	pDC->SelectObject(pFont);						//�t�H���g�̕��A
	
	return sizeLog;
}


//	BSch�̃��x��������̕`��
//  �\���̃Y�[���ɑΉ����邽�߁A�O���[�o���ϐ��� g_nVExt�Ag_nWExt ���Q�Ƃ��Ă���
//  �k���E�g��\��(nVExt!=nWExt)�̂Ƃ��͕����񑊓��̕������l�p�`�ŕ\������B
//      *BSch�ł͊g��\�����T�|�[�g���Ă��Ȃ��B
CSize CBSchFont::PutFont(	
		CDC* pDC,						//�o�͐�̃f�o�C�X�R���e�L�X�g
		int x,int y,					//�o�͈ʒu
		LPCTSTR lpstr,					//�o�͕�����
		int nStrLen		/* =-1		*/,	//������
		COLORREF col	/* =RGB(255,255,255) */,	//�o�͐F
		UINT nMode		/* =DRAW_ON */,	//�`�惂�[�h
		BOOL bVert		/* =FALSE 	*/,	//�����t���O
		int  nPointPos	/* =0  		*/,	//�w�肵���o�͈ʒu�� 0:������̑O 1:������̌�� 2:������̒���
										//					 0:������̉� 4:������̏�   8:������̒�
		int  nVExt		/* =1		*/,	//�\���{��(View)
		int  nWExt		/* =1		*/, //�\���{��(Window)
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

//	if(nStrLen<0) m_nStrLen=lstrlen(lpstr);		//�����񒷂̐ݒ�
//	else		  m_nStrLen=nStrLen;
	if(m_nStrLen==0) return CSize(0,0);		//�����񒷂��O�Ȃ牽�����Ȃ�
//	if(m_nStrLen>255) m_nStrLen=255;//�����񒷂͍ő�255����
	



	if(nVExt != m_VExt || nWExt != m_WExt||m_pFontHLabel==NULL||m_pFontVLabel==NULL){
		delete m_pFontHLabel;
		delete m_pFontVLabel;
		m_pFontHLabel = new CFont;
		m_pFontVLabel = new CFont;
		LOGFONT lf = m_logfontLabel;
		if(lf.lfHeight>0) lf.lfHeight = -lf.lfHeight;
		lf.lfOutPrecision = OUT_TT_ONLY_PRECIS;	
		lf.lfHeight = lf.lfHeight*nVExt/nWExt;
		m_pFontHLabel->CreateFontIndirect(&lf);	//�t�H���g�N���X�̏�����
		lf.lfEscapement = 900;
		lf.lfOrientation = 900;
		m_pFontVLabel->CreateFontIndirect(&lf);	//�t�H���g�N���X�̏�����
		m_VExt = nVExt;
		m_WExt = nWExt;
		if(bVert){
			pFont=pDC->SelectObject(m_pFontVLabel);			//�����t�H���g�̑I��
		}else{
			pFont=pDC->SelectObject(m_pFontHLabel);			//�����t�H���g�̑I��
		}
		TEXTMETRIC tm;
		pDC->GetTextMetrics(&tm);
		m_vOffset = tm.tmHeight - tm.tmDescent;
	}else{
		if(bVert){
			pFont=pDC->SelectObject(m_pFontVLabel);			//�����t�H���g�̑I��
		}else{
			pFont=pDC->SelectObject(m_pFontHLabel);			//�����t�H���g�̑I��
		}
	}

	size=pDC->GetTextExtent(buff,m_nStrLen);			//�`��T�C�Y�𓾂�
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

	colOldText	= pDC->SetTextColor(col);			//TextColor�̐ݒ�
	nOldBkMode  = pDC->SetBkMode(TRANSPARENT);		//�`��͓��߃��[�h
	pDC->TextOut(x,y,buff,m_nStrLen);				//�����`��
	sizeLog=size;
	if(bUseUpperLine && upperLine){
		CPen penNew;
		CPen* ppenOld;
		int penWidth = nVExt/nWExt;
		//�`�惂�[�h�ɂ���ăy���̎�ށE�F��COPY or XOR��ݒ肷��
		penNew.CreatePen(PS_SOLID,penWidth,col);
		ppenOld=pDC->SelectObject(&penNew);			//�쐬�����y���̑I��
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
//		colOldText	= pDC->SetTextColor(col);			//TextColor�̐ݒ�
//		nOldBkMode  = pDC->SetBkMode(TRANSPARENT);		//�`��͓��߃��[�h
//		pDC->TextOut(x,y,buff,m_nStrLen);				//�����`��
//		pDC->SetBkMode(nOldBkMode);						//�w�i�`�惂�[�h�̕��A
//		pDC->SetTextColor(colOldText);					//TextColor�̕��A
//	}else if(nMode&DRAW_OFF){
//		assert(0);
//		colOldText	= pDC->SetTextColor(0);				//TextColor�̐ݒ�i���ŕ`��j
//		nOldBkMode  = pDC->SetBkMode(TRANSPARENT);		//�`��͓��߃��[�h
//		pDC->TextOut(x,y,buff,m_nStrLen);				//�����`��
//		pDC->SetBkMode(nOldBkMode);						//�w�i�`�惂�[�h�̕��A
//		pDC->SetTextColor(colOldText);					//TextColor�̕��A
//	}else{
//		TRACE("\n�s���ȕ`�惂�[�h(CBSchFont::PutWinFont())");
//	}
	pDC->SetBkMode(nOldBkMode);						//�w�i�`�惂�[�h�̕��A
	pDC->SetTextColor(colOldText);					//TextColor�̕��A
	pDC->SelectObject(pFont);						//�t�H���g�̕��A
	
	return sizeLog;
}

/****************************************************************************
	�R�����g�p��Windows�t�H���g������̕`��
****************************************************************************/
CSize CBSchFont::PutWinFont(
		CDC* pDC,					//�o�͐�̃f�o�C�X�R���e�L�X�g
		int x,int y,					//�o�͈ʒu
		LPCTSTR lpstr,					//�o�͕�����
		int nStrLen /* =-1		*/,		//������
		LOGFONT* plf,
		COLORREF col/* =RGB(255,255,255) */,		//�o�͐F
		UINT nMode	/* =DRAW_ON */,		//�`�惂�[�h
		BOOL bVert		/* =FALSE 	*/,	//�����t���O
		int  nPointPos	/* =0  		*/,	//�w�肵���o�͈ʒu�� 0:������̑O 1:������̌�� 2:������̒���
										//					 0:������̉� 4:������̏�   8:������̒�
		int  nVExt	/* =1		*/,		//�\���{��(View)
		int  nWExt	/* =1		*/)		//�\���{��(Window)

{
	COLORREF colOldText;//,colOldBk;
	int      nOldBkMode;
	CSize	 size,sizeLog;

	if(nStrLen<0) m_nStrLen=lstrlen(lpstr);		//�����񒷂̐ݒ�
	else		  m_nStrLen=nStrLen;
	if(m_nStrLen==0) return CSize(0,0);			//�����񒷂��O�Ȃ牽�����Ȃ�
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

	pFont->CreateFontIndirect(&lf);	//�t�H���g�N���X�̏�����
	pFont=pDC->SelectObject(pFont);					//�t�H���g�̑I��
	size=pDC->GetTextExtent(lpstr,m_nStrLen);		//�`��T�C�Y�𓾂�

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
		colOldText	= pDC->SetTextColor(col);			//TextColor�̐ݒ�
		nOldBkMode  = pDC->SetBkMode(TRANSPARENT);		//�`��͓��߃��[�h
		pDC->TextOut(x,y,lpstr,m_nStrLen);				//�����`��
		pDC->SetBkMode(nOldBkMode);						//�w�i�`�惂�[�h�̕��A
		pDC->SetTextColor(colOldText);					//TextColor�̕��A
	}else if(nMode&DRAW_OFF){
		colOldText	= pDC->SetTextColor(0);				//TextColor�̐ݒ�i���ŕ`��j
		nOldBkMode  = pDC->SetBkMode(TRANSPARENT);		//�`��͓��߃��[�h
		pDC->TextOut(x,y,lpstr,m_nStrLen);				//�����`��
		pDC->SetBkMode(nOldBkMode);						//�w�i�`�惂�[�h�̕��A
		pDC->SetTextColor(colOldText);					//TextColor�̕��A
	}else{
		TRACE("\n�s���ȕ`�惂�[�h(CBSchFont::PutWinFont())");
	}
	pFont=pDC->SelectObject(pFont);					//�t�H���g�̕��A
	sizeLog=size;
	delete pFont;
	return sizeLog;

}





BOOL CBSchFont::IniReadFontInfo(const TCHAR* sectionName, LPLOGFONT lplf,const TCHAR* defFont,int defHeight)
{
	int n;
	char *p;
	CString rFaceName;
	
	p=(char*)lplf;	//LOGFONT�\���̂̂O�C�j�V�����C�Y
	for(n=0;n<sizeof(LOGFONT);n++) *p++=0;

	lplf->lfHeight=AfxGetApp()->GetProfileInt(sectionName,_T("Height"),defHeight); 	//�t�H���g�̍����𓾂�B
	lplf->lfWeight=AfxGetApp()->GetProfileInt(sectionName,_T("IsBold"),FW_REGULAR);	//�t�H���g�̃E�G�C�g�𓾂�B�f�t�H���g�l��0
	lplf->lfItalic=AfxGetApp()->GetProfileInt(sectionName,_T("IsItalic"),0);		//�t�H���g�̃C�^���b�N���𓾂�B�f�t�H���g�l��0
//	lplf->lfCharSet=AfxGetApp()->GetProfileInt(sectionName,"CharSet",SHIFTJIS_CHARSET);	//�t�H���g�̃L�����N�^�Z�b�g���𓾂�B�f�t�H���g�l��SHIFTJIS_CHARSET
	lplf->lfPitchAndFamily=0;//AfxGetApp()->GetProfileInt(sectionName,"PitchAndFamily",0);	//�t�H���g�̃s�b�`���t�@�~�����𓾂�B�f�t�H���g��0
	rFaceName=AfxGetApp()->GetProfileString(sectionName,_T("FaceName"),defFont);	//�t�H���g��FaceName�𓾂�B
	lstrcpy(lplf->lfFaceName,rFaceName);
	return TRUE;
}

//���W�X�g���Ƀt�H���g������������
BOOL CBSchFont::IniWriteFontInfo(const TCHAR* sectionName, LPLOGFONT lplf)
{
	AfxGetApp()->WriteProfileInt(sectionName,_T("Height"),abs(lplf->lfHeight));		//�t�H���g�̍������Z�b�g����
	AfxGetApp()->WriteProfileInt(sectionName,_T("IsBold"),lplf->lfWeight);			//�t�H���g�̃E�G�C�g���Z�b�g����
	AfxGetApp()->WriteProfileInt(sectionName,_T("IsItalic"),lplf->lfItalic);		//�t�H���g�̃C�^���b�N�����Z�b�g����
//	AfxGetApp()->WriteProfileInt(sectionName,"CharSet",lplf->lfCharSet);		//�t�H���g�̃L�����N�^�Z�b�g�����Z�b�g����
//	AfxGetApp()->WriteProfileInt(sectionName,"PitchAndFamily",lplf->lfPitchAndFamily);	//�t�H���g�̃s�b�`���t�@�~�������Z�b�g����
	AfxGetApp()->WriteProfileString(sectionName,_T("FaceName"),lplf->lfFaceName);	//�t�H���g��FaceName���Z�b�g����
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//�O���[�o���ϐ�
CBSchFont* g_pFont;		//BSch�t�H���g�`��N���X


//////////////////////////////////////////////////////////////////////
//�O���[�o���֐�

#define COMMENT_FONT_SECTION _T("CommentFont")
//���W�X�g������R�����g�̃f�t�H���g�̃t�H���g���𓾂�
BOOL g_IniReadFontInfo(LPLOGFONT lplf)
{
	return CBSchFont::IniReadFontInfo(COMMENT_FONT_SECTION,lplf,_T("MS UI Gothic"),16);
}

//���W�X�g���ɃR�����g�̃f�t�H���g�̃t�H���g������������
BOOL g_IniWriteFontInfo(LPLOGFONT lplf)
{
	return CBSchFont::IniWriteFontInfo(COMMENT_FONT_SECTION,lplf);
}

#define LABEL_FONT_SECTION _T("LabelFont")
//���W�X�g�����烉�x���̃t�H���g���𓾂�
BOOL g_IniReadLabelFontInfo(LPLOGFONT lplf)
{
	return CBSchFont::IniReadFontInfo(LABEL_FONT_SECTION,lplf,_T("Tahoma"),9);
}

//���W�X�g���Ƀ��x���̃t�H���g������������
BOOL g_IniWriteLabelFontInfo(LPLOGFONT lplf)
{
	return CBSchFont::IniWriteFontInfo(LABEL_FONT_SECTION,lplf);
}

			
			