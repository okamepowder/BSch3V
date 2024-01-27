/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

///////////////////////////////////////////////////////////
// BSch�̃t�H���g�N���X�̃C���v�������g

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
// CBSchFont �̃R���X�g���N�^
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
// CBSchFont �̃f�X�g���N�^
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
		int  nWExt		/* =1		*/,//�\���{��(Window)
		BOOL  bUseUpperLine	/* =TRUE */)	//����̎g�p

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

	


//	if(nStrLen<0) m_nStrLen=lstrlen(lpstr);		//�����񒷂̐ݒ�
//	else		  m_nStrLen=nStrLen;
	if(m_nStrLen==0) return CSize(0,0);		//�����񒷂��O�Ȃ牽�����Ȃ�
//	if(m_nStrLen>255) m_nStrLen=255;//�����񒷂͍ő�255����
	



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
		m_pFontH->CreateFontIndirect(&lf);	//�t�H���g�N���X�̏�����
		lf.lfEscapement = 900;
		lf.lfOrientation = 900;
		m_pFontV->CreateFontIndirect(&lf);	//�t�H���g�N���X�̏�����
		m_VExt = nVExt;
		m_WExt = nWExt;
		if(bVert){
			pFont=pDC->SelectObject(m_pFontV);			//�����t�H���g�̑I��
		}else{
			pFont=pDC->SelectObject(m_pFontH);			//�����t�H���g�̑I��
		}
		TEXTMETRIC tm;
		pDC->GetTextMetrics(&tm);
		m_vOffset = tm.tmHeight - tm.tmDescent;
	}else{
		if(bVert){
			pFont=pDC->SelectObject(m_pFontV);			//�����t�H���g�̑I��
		}else{
			pFont=pDC->SelectObject(m_pFontH);			//�����t�H���g�̑I��
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
//	if(!upperLine) TRACE("notUL %s\n",buff);
	if(upperLine){
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

	int nLength;

	if(nStrLen<0) nLength=lstrlen(lpstr);		//�����񒷂̐ݒ�
	else		  nLength=nStrLen;
	if(nLength==0) return CSize(0,0);			//�����񒷂��O�Ȃ牽�����Ȃ�
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

	pFont->CreateFontIndirect(&lf);	//�t�H���g�N���X�̏�����
	pFont=pDC->SelectObject(pFont);					//�t�H���g�̑I��
	size=pDC->GetTextExtent(lpstr,nLength);		//�`��T�C�Y�𓾂�

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
		pDC->TextOut(x,y,lpstr,nLength);				//�����`��
		pDC->SetBkMode(nOldBkMode);						//�w�i�`�惂�[�h�̕��A
		pDC->SetTextColor(colOldText);					//TextColor�̕��A
	}else if(nMode&DRAW_OFF){
		colOldText	= pDC->SetTextColor(0);				//TextColor�̐ݒ�i���ŕ`��j
		nOldBkMode  = pDC->SetBkMode(TRANSPARENT);		//�`��͓��߃��[�h
		pDC->TextOut(x,y,lpstr,nLength);				//�����`��
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


CSize CBSchFont::PutWinFontML(
		CDC* pDC,					//�o�͐�̃f�o�C�X�R���e�L�X�g
		int x,int y,				//�o�͈ʒu
		LPCTSTR lpstr,				//�o�͕�����
		int nStrLen,				//������
		LOGFONT* plf,
		int width,
		CSize& size1Line,
		COLORREF col/* =RGB(255,255,255) */,		//�o�͐F
		UINT nMode	/* =DRAW_ON */,		//�`�惂�[�h
		int  nVExt	/* =1		*/,		//�\���{��(View)
		int  nWExt	/* =1		*/)		//�\���{��(Window)

{
	COLORREF colOldText;//,colOldBk;
	int      nOldBkMode;
//	CSize	 size,sizeLog;
	CSize	 sizeLog;

	int nLength;

	if(nStrLen<0) nLength=lstrlen(lpstr);		//�����񒷂̐ݒ�
	else		  nLength=nStrLen;
	if(nLength==0) return CSize(0,0);			//�����񒷂��O�Ȃ牽�����Ȃ�
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

	pFont->CreateFontIndirect(&lf);	//�t�H���g�N���X�̏�����
	pFont=pDC->SelectObject(pFont);					//�t�H���g�̑I��

	size1Line=pDC->GetTextExtent(lpstr,nLength);	//1���C�����̕`��T�C�Y�𓾂�

	//int nPointPosH = nPointPos&3;
	//int nPointPosV = (nPointPos>>2)&3;
	int vOffset;

	vOffset = size1Line.cy;


	y-=vOffset;

	int right = x+width;
	int bottom = y+10000;

	CRect rc(x,y,right,bottom);


	UINT nFormat =DT_NOPREFIX|DT_TOP|DT_LEFT|DT_WORDBREAK|DT_CALCRECT;

	pDC->DrawText(lpstr,nLength,&rc,nFormat);	//�`��͂��Ȃ��B
	nFormat &= ~DT_CALCRECT;
	nFormat |= DT_NOCLIP;

	if(nMode&DRAW_ON || nMode&DRAW_TEMP){
		colOldText	= pDC->SetTextColor(col);			//TextColor�̐ݒ�
		nOldBkMode  = pDC->SetBkMode(TRANSPARENT);		//�`��͓��߃��[�h
		//pDC->TextOut(x,y,lpstr,nLength);				//�����`��
		pDC->DrawText(lpstr,nLength,&rc,nFormat);
		pDC->SetBkMode(nOldBkMode);						//�w�i�`�惂�[�h�̕��A
		pDC->SetTextColor(colOldText);					//TextColor�̕��A
	}else if(nMode&DRAW_OFF){
		colOldText	= pDC->SetTextColor(0);				//TextColor�̐ݒ�i���ŕ`��j
		nOldBkMode  = pDC->SetBkMode(TRANSPARENT);		//�`��͓��߃��[�h
		//pDC->TextOut(x,y,lpstr,nLength);				//�����`��
		pDC->DrawText(lpstr,nLength,&rc,nFormat);
		pDC->SetBkMode(nOldBkMode);						//�w�i�`�惂�[�h�̕��A
		pDC->SetTextColor(colOldText);					//TextColor�̕��A
	}else{
		TRACE("\n�s���ȕ`�惂�[�h(CBSchFont::PutWinFont())");
	}
	pFont=pDC->SelectObject(pFont);					//�t�H���g�̕��A
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
	
	p=(char*)lplf;	//LOGFONT�\���̂̂O�C�j�V�����C�Y
	for(n=0;n<sizeof(LOGFONT);n++) *p++=0;

	lplf->lfHeight=AfxGetApp()->GetProfileInt(sectionName,_T("Height"),defHeight); 	//�t�H���g�̍����𓾂�B
	lplf->lfWeight=AfxGetApp()->GetProfileInt(sectionName,_T("IsBold"),FW_REGULAR);	//�t�H���g�̃E�G�C�g�𓾂�B�f�t�H���g�l��0
	lplf->lfItalic=AfxGetApp()->GetProfileInt(sectionName,_T("IsItalic"),0);		//�t�H���g�̃C�^���b�N���𓾂�B�f�t�H���g�l��0
//	lplf->lfCharSet=AfxGetApp()->GetProfileInt(sectionName,"CharSet",SHIFTJIS_CHARSET);	//�t�H���g�̃L�����N�^�Z�b�g���𓾂�B�f�t�H���g�l��SHIFTJIS_CHARSET
	lplf->lfPitchAndFamily=0;//AfxGetApp()->GetProfileInt(sectionName,"PitchAndFamily",0);	//�t�H���g�̃s�b�`���t�@�~�����𓾂�B�f�t�H���g��0
	rFaceName=AfxGetApp()->GetProfileString(sectionName,_T("FaceName"),defFont);	//�t�H���g��FaceName�𓾂�B
	_tcsncpy(lplf->lfFaceName,rFaceName,LF_FACESIZE);
	lplf->lfFaceName[LF_FACESIZE-1]='\0';
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
CBSchFont* g_pFont;			//BSch�t�H���g�`��N���X
CBSchFont* g_pFontName;		//���i���A���i�ԍ��̕`��


//////////////////////////////////////////////////////////////////////
//�O���[�o���֐�

#define COMMENT_FONT_SECTION _T("CommentFont")
//���W�X�g������R�����g�̃f�t�H���g�̃t�H���g���𓾂�
BOOL g_IniReadFontInfo(LPLOGFONT lplf)
{
	return CBSchFont::IniReadFontInfo(COMMENT_FONT_SECTION,lplf,_T("Tahoma"),16);
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

#define NAME_FONT_SECTION _T("CompNameFont")
//���W�X�g�����畔�i���ԍ��̃t�H���g���𓾂�
BOOL g_IniReadNameFontInfo(LPLOGFONT lplf)
{
	return CBSchFont::IniReadFontInfo(NAME_FONT_SECTION,lplf,_T("Tahoma"),9);
}

//���W�X�g���ɕ��i���ԍ��̃t�H���g������������
BOOL g_IniWriteNameFontInfo(LPLOGFONT lplf)
{
	return CBSchFont::IniWriteFontInfo(NAME_FONT_SECTION,lplf);
}

#define QUICKPROP_FONT_SECTION _T("QuickPropFont")

//���W�X�g������A�g���r���[�g�v���r���[�̃t�H���g���𓾂�
BOOL g_IniReadQuickPropFontInfo(LPLOGFONT lplf)
{
	return CBSchFont::IniReadFontInfo(QUICKPROP_FONT_SECTION,lplf,_T("Tahoma"),10);
}
//���W�X�g���ɃA�g���r���[�g�v���r���[�̃t�H���g������������
BOOL g_IniWriteQuickPropFontInfo(LPLOGFONT lplf)
{
	return CBSchFont::IniWriteFontInfo(QUICKPROP_FONT_SECTION,lplf);
}
