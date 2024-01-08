/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

/****************************************************************************
** XBSch�}�ʗv�f���x���N���X�̎���
****************************************************************************/
#include "stdafx.h"
#include <stdio.h>
#include <assert.h>
#include <string>
using namespace std;
#include "ce3io.h"
#include "xbschobj.h"
#include "xbschcomment.h"


//�R���X�g���N�^
SXBSchComment::SXBSchComment()
{
	m_drawSizeIsValid	= false;
	m_fontName			= m_defaultFontName;
	m_fontSize			= m_defaultFontSize;
	m_fontBold			= m_defaultFontBold;
	m_fontItalic		= m_defaultFontItalic;
	m_fontUnderline		= m_defaultFontUnderline;
	m_fontStrikeOut		= m_defaultFontStrikeOut;
	m_enableTag			= false;
	m_strText			= _T("");
	m_width				= -1;
	m_bHoriz			= true;
	m_dir				= SXBSchComment::BSCHCOMMENT_DIR::COMMENT_DIR_0;
	m_pMarkupTextLineList = NULL;
}

SXBSchComment::~SXBSchComment()
{
	deleteMarkupLineList();
}

wstring	SXBSchComment::m_defaultFontName = _T("");
int		SXBSchComment::m_defaultFontSize = 12;
bool	SXBSchComment::m_defaultFontBold = false;
bool	SXBSchComment::m_defaultFontItalic= false;
bool	SXBSchComment::m_defaultFontUnderline= false;
bool	SXBSchComment::m_defaultFontStrikeOut= false;



bool SXBSchComment::readCe3(SReadCE3& rce3,const SPoint* pptOrigin,SXBSchDoc*)
{
	wstring str(_T(""));
	bool bInitX1 = false;
	bool bInitY1 = false;
	bool bInitText = false;
	//bool bIniDir = false;

	m_drawSizeIsValid	= false;
	m_fontName			= m_defaultFontName;
	m_fontSize			= m_defaultFontSize;
	m_fontBold			= m_defaultFontBold;
	m_fontItalic		= m_defaultFontItalic;
	m_fontUnderline		= m_defaultFontUnderline;
	m_fontStrikeOut		= m_defaultFontStrikeOut;
	m_width				= -1;
	m_enableTag			= false;
	m_bHoriz			= true;
	m_dir				= BSCHCOMMENT_DIR::COMMENT_DIR_0;

	while(1){
		if(rce3.ReadRecord(str)==WEOF) return false;
		if(str[0] == '-'){
			if( (str != _T("-COMMENT")) || !bInitX1 || !bInitY1 || !bInitText){
				return false;
			}else{
				break;	//break while(1){ }
			}
		}else if(str[0] == '+'){
			str[0]='-';
			if(rce3.SkipTo(str)==WEOF) return false;
		}else{
			int n;
			int l=str.length();
			for(n=0;n<l;n++){
				if(str[n]==':') break;
			}
			if(0<n && n<(l-1)){	// : �ŕ�����ꂽ���R�[�h�ł���
				wstring var = str.substr(0,n);				//�擪����:�̎�O�܂�
				wstring val = str.substr(n+1);
				int nParam;

//#ifdef _MFCBSCH	//Qt-BSch doesn't support layer.
				if(var==_T("L")){
					nParam = _tstoi(val.c_str());
					if(isValidLayer(nParam)){
						m_Layer	= nParam;			
					}
				}else
//#endif
				if(var==_T("X")){	//X���W
					nParam = _tstoi(val.c_str());
					if(pptOrigin) nParam += pptOrigin->x();		
					m_p1.setX(nParam);
					bInitX1 = true;
				}else
				if(var==_T("Y")){	//Y���W
					nParam = _tstoi(val.c_str());
					if(pptOrigin) nParam += pptOrigin->y();		
					m_p1.setY(nParam);
					bInitY1 = true;
				}else
				if(var==_T("D")){	//��������  20121112  �݊����̂��߂Ɏc��	D �� DIR�͌ォ��ǂ񂾂��̂ɃI�[�o�[���C�g�����B
					nParam = _tstoi(val.c_str());
					if(nParam==0){
						m_bHoriz = false;
						m_dir = BSCHCOMMENT_DIR::COMMENT_DIR_3;
					}else{
						m_bHoriz = true;
						m_dir = BSCHCOMMENT_DIR::COMMENT_DIR_0;
					}
					//bInitDir = true;
				}else
				if(var==_T("DIR")){ //���� 2014/12/14 
					nParam = _tstoi(val.c_str());
					if(nParam>=0 && nParam <4){
						m_dir = nParam;
						m_bHoriz =( m_dir== BSCHCOMMENT_DIR::COMMENT_DIR_0 || m_dir== BSCHCOMMENT_DIR::COMMENT_DIR_2);
					}
				}else
				if(var==_T("W")){	//��
					nParam = _tstoi(val.c_str());
					setWidth(nParam);
				}else

				if(var==_T("S")){	//������
					m_strText=val;
					//if(m_strText.length()>0){
						bInitText = true;
					//}
				}else
				if(var==_T("FN")){	//�t�H���g��
					m_fontName=val;
				}else
				if(var==_T("TAG")){	//�^�O�̃C�l�[�u��
					m_enableTag=(_tstoi(val.c_str())!=0);
				}else
				if(var==_T("FS")){	//�t�H���g�T�C�Y
					nParam = _tstoi(val.c_str());
					setFontSize(nParam);
				}else
				if(var==_T("FF")){	//�t�H���g�t���O
					int n=val.length();
					for(int i=0;i<n;i++){
						switch(val[i]){
						case 'B': m_fontBold		= true; break;
						case 'I': m_fontItalic		= true; break;
						case 'U': m_fontUnderline	= true; break;
						case 'S': m_fontStrikeOut	= true; break;
						}
					}
				}
			}
		}
	}
	return true;
}

bool SXBSchComment::writeCe3(SWriteCE3& wce3,const SPoint* pptOrigin){
//	char sz[32];

	int x = m_p1.x();
	int y = m_p1.y();
	int d = (m_bHoriz ? 1 : 0);
	//string str("");
	if(pptOrigin){
		x -= pptOrigin->x();
		y -= pptOrigin->y();
	}
	wce3.WriteRecord(_T("+COMMENT"));
	wce3.WriteRecordInt(_T("L"),m_Layer);
	wce3.WriteRecordInt(_T("X"),x);
	wce3.WriteRecordInt(_T("Y"),y);
	wce3.WriteRecordInt(_T("D"),d);		//�݊����̂��߂Ɏc��
	wce3.WriteRecordInt(_T("DIR"),m_dir);
	wce3.WriteRecordInt(_T("W"),m_width);
//	sprintf(sz,"X:%d",x);
//	wce3.WriteRecord(sz);
//	sprintf(sz,"Y:%d",y);
//	wce3.WriteRecord(sz);
	wce3.WriteRecordString(_T("S"),m_strText);
//	str = "S:"; str = str + m_strText; 
//	wce3.WriteRecord(str);
	wce3.WriteRecordString(_T("FN"),m_fontName);
	wce3.WriteRecordInt(_T("TAG"),(m_enableTag? 1:0));
//	str = "FN:"; str = str + m_fontName; 
//	wce3.WriteRecord(str);
	wce3.WriteRecordInt(_T("FS"),m_fontSize);
//	sprintf(sz,"FS:%d",m_fontSize);
//	wce3.WriteRecord(sz);
	wstring str = _T("FF:");
	if(m_fontBold)		str = str + _T("B");
	if(m_fontItalic)	str = str + _T("I");
	if(m_fontUnderline)	str = str + _T("U");
	if(m_fontStrikeOut) str = str + _T("S");
	wce3.WriteRecord(str);
	wce3.WriteRecord(_T("-COMMENT"));
	wce3.WriteEOL();
	return true;
}


//�������s��
SXBSchObj* SXBSchComment::duplicate()
{
	SXBSchObj* newObj =new SXBSchComment(*this);
	((SXBSchComment*)newObj)->m_pMarkupTextLineList = NULL;
	return newObj;
}

//�`�悪�K�v���ǂ�����Ԃ�
//�`��T�C�Y�������̂Ƃ��͏��true��Ԃ�
bool SXBSchComment::qRedraw(const SRect& rc)
{
	if(!m_drawSizeIsValid){
		return true;
	}else{
		return rc.intersect(area());
	}
}

//��L�͈͂�Ԃ�
//�`��T�C�Y�������̂Ƃ��͕`��T�C�Y��20�~10�s�N�Z���Ɖ��肵�Ĕ��肷��B
SRect SXBSchComment::area()
{
	if(!m_drawSizeIsValid){
		m_drawSize=SSize(20,10);
//		m_drawAreaVOffset = 10;
	}


//��������������������������������������������������������������������������������
//	if(m_bHoriz){
//		//return SRect(m_p1.x(),m_p1.y()-m_drawSize.h()-COMMENT_Y_OFFSET,m_drawSize.w(),m_drawSize.h());
//		return SRect(m_p1.x(),m_p1.y()-m_1lineDrawSize.h()-COMMENT_Y_OFFSET,m_drawSize.w(),m_drawSize.h());
//	}else{
//		return SRect(m_p1.x()-m_1lineDrawSize.h()-COMMENT_Y_OFFSET,m_p1.y()-m_drawSize.w(),m_drawSize.h(),m_drawSize.w());
//	}

	switch(m_dir){
	case BSCHCOMMENT_DIR::COMMENT_DIR_0:
		return SRect(m_p1.x(),m_p1.y()-m_1lineDrawSize.h()-COMMENT_Y_OFFSET,m_drawSize.w(),m_drawSize.h());
	case BSCHCOMMENT_DIR::COMMENT_DIR_1:
		return SRect(m_p1.x()+m_1lineDrawSize.h()+COMMENT_Y_OFFSET-m_drawSize.h(),m_p1.y(),m_drawSize.h(),m_drawSize.w());
	case BSCHCOMMENT_DIR::COMMENT_DIR_2:
		return SRect(m_p1.x()-m_drawSize.w(),m_p1.y()+m_1lineDrawSize.h()+COMMENT_Y_OFFSET-m_drawSize.h(),m_drawSize.w(),m_drawSize.h());
	//case BSCHCOMMENT_DIR::COMMENT_DIR_0:
	default:
		return SRect(m_p1.x()-m_1lineDrawSize.h()-COMMENT_Y_OFFSET,m_p1.y()-m_drawSize.w(),m_drawSize.h(),m_drawSize.w());
	}
}


//XY���S���W���w�肵�āA��]���s��
//2012/11/11
void SXBSchComment::rotate(const SPoint& p)
{
	m_bHoriz = !m_bHoriz;
	switch(m_dir){
	case BSCHCOMMENT_DIR::COMMENT_DIR_0: m_dir =  BSCHCOMMENT_DIR::COMMENT_DIR_1; break;
	case BSCHCOMMENT_DIR::COMMENT_DIR_1: m_dir =  BSCHCOMMENT_DIR::COMMENT_DIR_2; break;
	case BSCHCOMMENT_DIR::COMMENT_DIR_2: m_dir =  BSCHCOMMENT_DIR::COMMENT_DIR_3; break;
	default /*case BSCHCOMMENT_DIR::COMMENT_DIR_3*/ : m_dir =  BSCHCOMMENT_DIR::COMMENT_DIR_0; break;
	}

	SXBSchObj::rotate(p);
}

//��]���s��
//2012/11/11
void SXBSchComment::rotate()
{
	rotate(m_p1);
}

//�_���w�肵�đI���d�v�x��Ԃ�
int SXBSchComment::qSelectWeight(const SPoint& p)
{
	int nResult = PointAndRectMag(p,area());
	if( nResult ) return (nResult | ON_OBJ);
	else		return 0;
}

//�͈͂��w�肵�đI�����s��
unsigned SXBSchComment::testSelection(const SRect& rc)
{
	if( rc.intersect(area()) ){
		return SELECT_ALL;
	}else{
		return 0;
	}
}

//�͈͂�I�����ăh���b�O�I�����s��
unsigned SXBSchComment::testSelectionForDrag(const SRect& rc)
{
	if( rc.intersect(area()) ){
		return SELECT_ALL;
	}else{
		return 0;
	}
}


SRect SXBSchComment::rotateRect()
{
	return SRect(m_p1,m_p1);
}


//�e�L�X�g�̐ݒ�
void SXBSchComment::setText(const TCHAR* psz)
{
	if(psz == NULL){
		m_strText = _T("");
	}else{
		m_strText = psz;
	}
	m_drawSizeIsValid = false;
}

//�`��T�C�Y�̐ݒ�
//
//void SXBSchComment::setDrawSize(int vOffset,const SSize& size)
//{
//	m_drawAreaVOffset = vOffset;
//	m_drawSize=size;
//	m_drawSizeIsValid=true;
//}

void SXBSchComment::setDrawSize(const SSize& size,const SSize& size1Line)
{
//	m_drawAreaVOffset = vOffset;
	m_drawSize=size;
	m_1lineDrawSize = size1Line;
	m_drawSizeIsValid=true;
}


//void setMaxWidth(int maxW);
//int maxWidth();
void SXBSchComment::setWidth(int w)
{
	if(w>=0 && w<50)w=50;
	m_width = w;
}

int SXBSchComment::width()
{
	return m_width;
}

//�t�H���g���̐ݒ�		
void SXBSchComment::setFontName(const TCHAR* psz)
{
	if(psz == NULL){
		m_fontName = _T("");
	}else{
		m_fontName = psz;
	}
	m_drawSizeIsValid = false;
}

//�t�H���g�T�C�Y�̐ݒ�
void SXBSchComment::setFontSize(int fontSize)
{
	if(fontSize<4) fontSize=4;
	else if(fontSize>80) fontSize=80;
	m_fontSize = fontSize;
	m_drawSizeIsValid = false;
}

//�{�[���h�ݒ�
void SXBSchComment::setFontBold(bool istrue)
{
	m_fontBold=istrue;
	m_drawSizeIsValid = false;
}

//�C�^���b�N�ݒ�
void SXBSchComment::setFontItalic(bool istrue)
{
	m_fontItalic=istrue;
	m_drawSizeIsValid = false;
}

//�A���_�[���C���ݒ�
void SXBSchComment::setFontUnderline(bool istrue)
{
	m_fontUnderline=istrue;
	m_drawSizeIsValid = false;
}

//���������ݒ�
void SXBSchComment::setFontStrikeOut(bool istrue)
{
	m_fontStrikeOut=istrue;
	m_drawSizeIsValid = false;
}

//�^�O�L���ݒ�
void SXBSchComment::setEnableTag(bool istrue)
{
	m_enableTag=istrue;
	m_drawSizeIsValid = false;
}

void SXBSchComment::setupMarkupLineList(bool printMode)
{
#ifdef _BSCHCAPTURE


	if(m_pMarkupTextLineList==NULL){
		m_pMarkupTextLineList = new MarkupTextLineList();
	}
	g_CreateMarkupTextLines(m_strText.c_str(),m_pMarkupTextLineList,printMode);
#endif
}

void SXBSchComment::deleteMarkupLineList()
{
    if(m_pMarkupTextLineList != NULL){
		MarkupTextLineIterator ite;
		ite = m_pMarkupTextLineList->begin();
		while(ite != m_pMarkupTextLineList->end()){
			delete(*ite);
			ite++;
		}
		m_pMarkupTextLineList->clear();
		delete m_pMarkupTextLineList;
		m_pMarkupTextLineList = NULL;
	}
}