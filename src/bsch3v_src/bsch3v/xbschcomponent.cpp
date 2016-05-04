/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

/****************************************************************************
** XBSch�}�ʗv�f���i�N���X
****************************************************************************/
#include "stdafx.h"
#include <stdio.h>
#include <assert.h>
#include <list>
#include <string>
using namespace std;
#include "ce3io.h"
#include "xbschglobal.h"
#include "xbschobj.h"
#include "xbschcomponent.h"

//�R���X�g���N�^
SXBSchComponent::SXBSchComponent()
{
	m_name		= _T("");				//���O
	m_refnum	= _T("");				//�Q�Ɣԍ�
	m_name_horiz = true;			//2005/04/08
	m_refnum_horiz = true;			//2005/04/08
	m_block		= 0;				//�u���b�N�ԍ�
	m_dir		= COMPONENT_DIR_0;	//����
	m_compInfoIndex = NULL;			//���i�̏��ւ̃|�C���^
	m_useEmbeddedLib = true;
	m_pinltrb	=0;					//LTRB��10�s�N�Z�����s�������݂��Ă��邩�ǂ����̃t���O
	m_orgname	= _T("");				//�璷�����ǁA���C�u��������ǂݑ��˂����i���c�����߂̂���
	m_note		= _T("");
	m_packagename = _T("");
	m_mfrpnum	= _T("");
	m_mfr		= _T("");
	resetRefnumPos();
	m_refnum_drawSizeIsValid = false;
	resetNamePos();
	m_name_drawSizeIsValid = false;
	m_refnum_hide	= false;	//2009/06/16
	m_name_hide	= false;		//2009/06/16

}

//�f�X�g���N�^
SXBSchComponent::~SXBSchComponent()
{
	if(m_useEmbeddedLib && m_compInfoIndex != NULL){
		delete m_compInfoIndex->m_pCompInfo;
		delete m_compInfoIndex;
	}
}

//������Z�q
SXBSchComponent& SXBSchComponent:: operator=(const SXBSchComponent& comp)
{
	if(this != &comp){
		if(m_useEmbeddedLib && m_compInfoIndex != NULL){
			delete m_compInfoIndex->m_pCompInfo;
			delete m_compInfoIndex;
			m_compInfoIndex = NULL;
		}
		*((SXBSchObj*)this) = comp;
		m_name			= comp.m_name;			//���O
		m_name_pos		= comp.m_name_pos;		//���O�̌��_����̃I�t�Z�b�g
		m_name_hide		= comp.m_name_hide;
		m_name_horiz	= comp.m_name_horiz;	//2005/04/08
		m_refnum		= comp.m_refnum;		//�Q�Ɣԍ�
		m_refnum_pos	= comp.m_refnum_pos;	//�Q�Ɣԍ��̌��_����̃I�t�Z�b�g
		m_refnum_hide	= comp.m_refnum_hide;
		m_refnum_horiz	= comp.m_refnum_horiz;	//2005/04/08
		m_block			= comp.m_block;			//�u���b�N�ԍ�
		m_dir			= comp.m_dir;			//����
		m_pinltrb		= comp.m_pinltrb;		//LTRB��10�s�N�Z�����s�������݂��Ă��邩�ǂ����̃t���O
		m_note			= comp.m_note;
		m_packagename	= comp.m_packagename;	//2009/01/06
		m_mfrpnum		= comp.m_mfrpnum;		//2009/01/07
		m_mfr			= comp.m_mfr;			//2009/01/07


		m_useEmbeddedLib = comp.m_useEmbeddedLib;
		m_compInfoIndex = NULL;
		m_refnum_drawSizeIsValid = comp.m_refnum_drawSizeIsValid;
		m_name_drawSizeIsValid = comp.m_name_drawSizeIsValid;
		m_name_drawSize = comp.m_name_drawSize;
		m_refnum_drawSize = comp.m_refnum_drawSize;
		
		if(!m_useEmbeddedLib){
			m_compInfoIndex	= comp.m_compInfoIndex;	//���i�̏��ւ̃|�C���^
		}else if(comp.m_compInfoIndex != NULL && comp.m_compInfoIndex->m_pCompInfo != NULL){	//���ߍ��񂾕��i�̏��ւ̃|�C���^
			m_compInfoIndex = new SCompIndex;
			m_compInfoIndex->m_pCompInfo = new SCompInfo(*(comp.m_compInfoIndex->m_pCompInfo));
		}
		m_orgname		= comp.m_orgname;		//���C�u��������̌Ăяo����
	}
	return *this;
}


SXBSchComponent::SXBSchComponent(const SXBSchComponent& comp)				//�R�s�[�R���X�g���N�^
{
	m_compInfoIndex = NULL;			//���i�̏��ւ̃|�C���^

	*this = comp;
}


//�������s��
SXBSchObj* SXBSchComponent::duplicate()
{
	SXBSchComponent* pComp = new SXBSchComponent(*this);
	SXBSchObj* newObj = pComp;
	return newObj;
}

//m_dir�ϐ������E���]����
void SXBSchComponent::mirrorDir()
{
	assert(m_dir>=0 && m_dir<=7);
	m_dir &=0x07;	//���ɈӖ��i�V
	m_dir ^=0x04;	//���]�t���O�̔��]
	int pinL = (m_pinltrb & COMPONENT_HASLPIN);
	int pinR = (m_pinltrb & COMPONENT_HASRPIN);
	m_pinltrb &= (COMPONENT_HASTPIN | COMPONENT_HASBPIN);
	if(pinL)  m_pinltrb |= COMPONENT_HASRPIN;
	if(pinR)  m_pinltrb |= COMPONENT_HASLPIN;
}

//m_dir�ϐ����E��]����
void SXBSchComponent::rotateDir()
{
	assert(m_dir>=0 && m_dir<=7);
	m_dir &=0x07;	//���ɈӖ��i�V
	if(m_dir & 0x04){
		m_dir -=1;
		m_dir |= 0x04;
	}else{
		m_dir +=1;
		m_dir &= 0x03;
	}
	m_pinltrb <<= 1;
	if(m_pinltrb & 0x10){
		m_pinltrb |= 1;
	}
	m_pinltrb &= 0xf;
}


//�Q�Ɣԍ��̈ʒu���f�t�H���g�̈ʒu�ɕύX
void SXBSchComponent::resetRefnumPos()
{
	//if(GetHideNum()) return;
	m_refnum_pos = SPoint(COMPONENT_DEFAULT_REFNUM_X,COMPONENT_DEFAULT_REFNUM_Y);
	m_refnum_horiz	= true;	//2005/04/08
}

//���i���̈ʒu���f�t�H���g�̈ʒu�ɕύX
void SXBSchComponent::resetNamePos()
{
	//if(GetHideVal()) return;
	m_name_pos = SPoint(COMPONENT_DEFAULT_NAME_X,COMPONENT_DEFAULT_NAME_Y);
	m_name_horiz	= true;	//2005/04/08
}

//�ʒu�𓮂���
void SXBSchComponent::move(int offsetx,int offsety)
{
	SPoint p(offsetx,offsety);
	m_p1 += p;
}	




//X���W���w�肵�č��E���]���s��
void SXBSchComponent::mirrorH(int x)
{
	int componentcenter;
	int componentwidth;


	if(m_compInfoIndex != NULL){
		componentwidth = size().w(); 
//		componentwidth = (m_compInfoIndex->size().w()*10); 
	}else{
		componentwidth = 20;
	}

	componentcenter = -componentwidth/2;


	int textcenter;	//�����񐅕������̈ʒu�̕��i��������̈ʒu�B
	int textX;

	int name_draw_width;
	int refnum_draw_width;

	if(this->m_name_horiz){
		if(m_name_drawSizeIsValid){
			name_draw_width = m_name_drawSize.w();
		}else{
			name_draw_width = m_name.length()*8;
		}
	}else{
		if(m_name_drawSizeIsValid){
			name_draw_width = -m_name_drawSize.h();
		}else{
			name_draw_width = -10;
		}
	}
	textcenter = m_name_pos.x() + name_draw_width/2 - componentcenter;	//���݈ʒu
	textcenter = -textcenter;	//���E���]�����ʒu
	textX = textcenter - name_draw_width/2 + componentcenter;
	m_name_pos.setX(textX);


	if(this->m_refnum_horiz){
		if(m_refnum_drawSizeIsValid){
			refnum_draw_width = m_refnum_drawSize.w();
		}else{
			refnum_draw_width = m_refnum.length()*8;
		}
	}else{
		if(m_refnum_drawSizeIsValid){
			refnum_draw_width = -m_refnum_drawSize.h();
		}else{
			refnum_draw_width = -10;
		}
	}

	textcenter = m_refnum_pos.x() + refnum_draw_width/2 - componentcenter;	//���݈ʒu
	textcenter = -textcenter;	//���E���]�����ʒu
	textX = textcenter - refnum_draw_width/2 + componentcenter;
	m_refnum_pos.setX(textX);

	componentcenter = m_p1.x() + componentcenter;
	componentcenter = x-(componentcenter-x);
	m_p1.setX(componentcenter + componentwidth/2);
	
	mirrorDir();
}

//���E���]���s��
void  SXBSchComponent::mirrorH()
{
	int componentHWidth;
	if(m_compInfoIndex != NULL){
		componentHWidth = size().w(); 
//		componentHWidth = m_compInfoIndex->size().w()*10; 
	}else{
		componentHWidth = 20;
	}
	int x = m_p1.x() - componentHWidth / 2;
	mirrorH(x);
}


SSize SXBSchComponent::size() const
{
	int w;
	int h;
	if(m_compInfoIndex == NULL){
		w=20; h=20;
	}else{
		w = m_compInfoIndex->size().w()*10;
		h = m_compInfoIndex->size().h()*10;
		if(m_dir & 1){	//���T�C�Y�Əc�T�C�Y������ւ��
			int n=w; w=h; h=n;
		}
	}
	return SSize(w,h);
}

//��L�͈͂�Ԃ�
SRect SXBSchComponent::area()
{
	SSize sizeComp = size();
	int r = m_p1.x();
	int b = m_p1.y();
	int l = r - sizeComp.w();
	int t = b - sizeComp.h();
	if(m_pinltrb & COMPONENT_HASLPIN) l -= PIN_LENGTH;
	if(m_pinltrb & COMPONENT_HASTPIN) t -= PIN_LENGTH;
	if(m_pinltrb & COMPONENT_HASRPIN) r += PIN_LENGTH;
	if(m_pinltrb & COMPONENT_HASBPIN) b += PIN_LENGTH;
	
	return SRect(SPoint(l,t),SPoint(r,b));
}

//��L�͈͂�Ԃ�
SRect SXBSchComponent::bodyArea()
{
	SSize sizeComp = size();
	int w = sizeComp.w();
	int h = sizeComp.h();
	return SRect(m_p1.x()-w,m_p1.y()-h,w,h);
}



//���i���̐�L�͈͂�Ԃ�
SRect SXBSchComponent::nameArea() const 
{
	int w,h;
	if(!m_name_drawSizeIsValid){
		w = m_name.length()*8;
		h = 10;
	}else{
		w = m_name_drawSize.w();
		h = m_name_drawSize.h();
	}
	if(this->m_name_horiz){
		return SRect(m_p1.x()+m_name_pos.x(),m_p1.y()+m_name_pos.y()-h,w,h);
	}else{
		return SRect(m_p1.x()+m_name_pos.x()-h,m_p1.y()+m_name_pos.y()-w,h,w);
	}
//	return SRect(m_p1.x()+m_name_pos.x(),m_p1.y()+m_name_pos.y()-h,w,h);
}

//�Q�Ɣԍ��̐�L�͈͂�Ԃ�
SRect SXBSchComponent::refnumArea() const
{
	int w,h;
	if(!m_refnum_drawSizeIsValid){
		w = m_refnum.length()*8;
		h = 10;
	}else{
		w = m_refnum_drawSize.w();
		h = m_refnum_drawSize.h();
	}
	if(this->m_refnum_horiz){
		return SRect(m_p1.x()+m_refnum_pos.x(),m_p1.y()+m_refnum_pos.y()-h,w,h);
	}else{
		return SRect(m_p1.x()+m_refnum_pos.x()-h,m_p1.y()+m_refnum_pos.y()-w,h,w);
	}
}

//���i���`��T�C�Y�̐ݒ�
void SXBSchComponent::setNameDrawSize(const SSize& size)
{
	m_name_drawSize=size;
	m_name_drawSizeIsValid=true;
}

//�Q�Ɣԍ��`��T�C�Y�̐ݒ�
void SXBSchComponent::setRefnumDrawSize(const SSize& size)
{
	m_refnum_drawSize=size;
	m_refnum_drawSizeIsValid=true;
}


//////////////////////////////////////////////////////////////////////////////////////
//���i�ԍ��A�l�̔�\�� 
void SXBSchComponent::SetHideVal(bool hide)
{
	m_name_hide = hide;
}

void SXBSchComponent::SetHideNum(bool hide)
{
	m_refnum_hide = hide;

}

bool SXBSchComponent::GetHideVal() const
{
	return m_name_hide;
}

bool SXBSchComponent::GetHideNum() const
{
	return m_refnum_hide;
}
//////////////////////////////////////////////////////////////////////////////////////


//XY���S���W���w�肵�āA��]���s��
void SXBSchComponent::rotate(const SPoint& p)
{
	resetRefnumPos();
	resetNamePos();
	SPoint center = bodyArea().center();
	int centerY = p.y() + (center.x() - p.x()) ;
	int centerX = p.x() - (center.y() - p.y()) ;
	rotateDir();
	SSize sizeComp = size();
	int x = centerX + sizeComp.w()/2;
	int y = centerY + sizeComp.h()/2;
	m_p1.setX(x);
	m_p1.setY(y);
}

//��]���s��
void  SXBSchComponent::rotate()
{
	resetRefnumPos();
	resetNamePos();
	rotateDir();
	//	rotate(bodyArea().center());
}

SRect SXBSchComponent::rotateRect()
{
	return bodyArea();
}


//�͈͂��w�肵�đI�����s��
unsigned SXBSchComponent::testSelection(const SRect& rc)
{
	if( rc.intersect(bodyArea()) ){
		return SELECT_ALL;
	}else{
		return 0;
	}
}


//�͈͂�I�����ăh���b�O�I�����s��
unsigned SXBSchComponent::testSelectionForDrag(const SRect& rc)
{
	SRect rcSel=bodyArea();
	rcSel.setL(rcSel.l()+3);
	rcSel.setT(rcSel.t()+3);
	rcSel.setR(rcSel.r()-3);
	rcSel.setB(rcSel.b()-3);

	if( rc.intersect(rcSel) ){
		return SELECT_ALL;
	}else{
		return 0;
	}
}

//�_���w�肵�đI���d�v�x��Ԃ�
int SXBSchComponent::qSelectWeight(const SPoint& p)
{
	int n;
	int nMax=0;
	int nOnFlag=0;
	n = PointAndRectMag(p,bodyArea());
	if(n){
		nOnFlag |= ON_OBJ;
		if(n>nMax) nMax = n;
	}

	if(!GetHideVal()){
		n = PointAndRectMag(p,nameArea());
		if(n){
			nOnFlag |= ON_NAME;
			if(n>nMax) nMax = n;
		}
	}

	if(!GetHideNum()){
		n = PointAndRectMag(p,refnumArea());
		if(n){
			nOnFlag |= ON_NUM;
			if(n>nMax) nMax = n;
		}
	}

	return nMax|nOnFlag;
}

//�w��_������\�Ȃ��̂̏�ɂ��邩�ǂ���
int SXBSchComponent::isEditablePoint(const SPoint& pt,int& info)
{
	info =0;
	return qSelectWeight(pt)&(ON_OBJ|ON_NAME|ON_NUM);
}



void SXBSchComponent::setName(const TCHAR* psz)
{
	m_name = psz;
	m_name_drawSizeIsValid = false;
}

void SXBSchComponent::setNameHorizontal(bool bHoriz)	//2005/04/09
{
	m_name_horiz = bHoriz;
	m_name_drawSizeIsValid = false;
}


void SXBSchComponent::setNote(const TCHAR* psz)
{
	m_note = psz;
}

void SXBSchComponent::setOrgName(const TCHAR* psz)
{
	m_orgname = psz;
}

void SXBSchComponent::setPackageName(const TCHAR* psz)
{
	m_packagename = psz;
}

void SXBSchComponent::setMfr(const TCHAR* psz)
{
	m_mfr = psz;
}

void SXBSchComponent::setMFrPNum(const TCHAR* psz)
{
	m_mfrpnum = psz;
}


bool SXBSchComponent::moveNamePos(int offsetx,int offsety)
{
	m_name_pos.setX(m_name_pos.x()+offsetx);
	m_name_pos.setY(m_name_pos.y()+offsety);
	return true;
}

void SXBSchComponent::setRefnum(const TCHAR* psz)
{
	m_refnum = psz;
	m_refnum_drawSizeIsValid = false;
}

void SXBSchComponent::setRefnumHorizontal(bool bHoriz)	//2005/04/09
{
	m_refnum_horiz = bHoriz;
	m_refnum_drawSizeIsValid = false;
}

bool SXBSchComponent::moveRefnumPos(int offsetx,int offsety)
{
	m_refnum_pos.setX(m_refnum_pos.x()+offsetx);
	m_refnum_pos.setY(m_refnum_pos.y()+offsety);
	return true;
}

void SXBSchComponent::setBlock(int n)
{
	int blockCount;
	if(m_compInfoIndex != NULL){
		blockCount = m_compInfoIndex->block(); 
	}else{
		blockCount = 1;
	}
	if(n >= blockCount || n<0) return;
	else m_block = n;
}


void SXBSchComponent::setDir(int n)
{
	n &= 7;
	m_dir = n;
}

bool SXBSchComponent::readEmbLibCe3(SReadCE3& rce3, SCompInfo* compInfo, wstring& name)
{
	SPtn* pPtn = NULL;
	bool bComp = false;
	wstring str(_T(""));
	//string name;
	wstring ptnName(_T(""));
	if(!compInfo) return false;
	while(1){
		if(rce3.ReadRecord(str)==WEOF) return false;
		if(str[0] == '-'){
			if( (str != _T("-BSCH3_LIB_V.1.0") || !bComp)){
				return false;
			}else{
				break;	//break while(1){ }
			}
		}else if(str == _T("+PTN")){
			if(!pPtn){
				pPtn=new SPtn;
				if(pPtn->readCe3(rce3)){
					//�p�^�[���ǂݍ��ݐ���I��
				}else{
					delete pPtn;
					pPtn = NULL;
				}
			}else{
				if(rce3.SkipTo(_T("-PTN"))==WEOF) return false;
			}
		}else if(str == _T("+COMP")){
			if(!bComp){
				if(compInfo->readCe3(rce3,name,ptnName)){
					bComp = true;
					//���i��`�ǂݍ��ݐ���I��
				}else{
					return false;
				}
			}else{
				if(rce3.SkipTo(_T("-COMP"))==WEOF) return false;
			}
		}
	}
	if(pPtn){
		compInfo->m_pPtn = pPtn;
	}

	return true;
}

bool SXBSchComponent::writeEmbLibCe3(SWriteCE3& wce3)
{
	// >>> 2005/01/30 ���i��񂪌�����Ȃ��������i��ۑ�����ۂɗ�������̑΍�
	if(!m_compInfoIndex) return false;
	if(!m_compInfoIndex->m_pCompInfo) return false;
	// <<< 2005/01/30
	static const TCHAR* defName = _T("EMB");
	const TCHAR* name = m_orgname.c_str();
	if(name == NULL || name[0]=='\0'){
		name = m_name.c_str();
	}
	if(name == NULL || name[0]=='\0'){
		name = defName;
	}
	wce3.WriteEOL();
	wce3.WriteRecord(_T("+BSCH3_LIB_V.1.0"));	wce3.WriteEOL();
	wce3.WriteRecordInt(_T("VER"),SCompLib::m_nConstDataVersion);	wce3.WriteEOL();
	m_compInfoIndex->m_pCompInfo->writeCe3(wce3,name);
	wce3.WriteRecord(_T("-BSCH3_LIB_V.1.0"));	wce3.WriteEOL();
	return true;

}

bool SXBSchComponent::readCe3(SReadCE3& rce3,const SPoint* pptOrigin,SXBSchDoc*)
{
	wstring str(_T(""));
	wstring name(_T(""));
	bool bInitX1 = false;
	bool bInitY1 = false;
	bool bInitLib = false;
	bool bEmbLib = false;

	while(1){
		if(rce3.ReadRecord(str)==WEOF) return false;
		if(str[0] == '-'){
			if( (str != _T("-COMPONENT")) || !bInitX1 || !bInitY1 || !bInitLib){
				return false;
			}else{
				break;	//break while(1){ }
			}
		}else if(str == _T("+BSCH3_LIB_V.1.0")){
			if(!bEmbLib){
				SCompInfo* pCompInfo = new SCompInfo;
				if(!readEmbLibCe3(rce3,pCompInfo,name)){
					delete pCompInfo;
					return false;
				}
				if(!m_compInfoIndex) m_compInfoIndex = new SCompIndex;
				if(m_compInfoIndex->m_pCompInfo) delete m_compInfoIndex->m_pCompInfo;
				m_compInfoIndex->m_pCompInfo = pCompInfo;
				m_orgname = name;
				bEmbLib = true;
			}else{
				if(rce3.SkipTo(_T("-BSCH3_LIB_V.1.0"))==WEOF) return false;
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
			if(0<n && n<l){	// : �ŕ�����ꂽ���R�[�h�ł���
				wstring var = str.substr(0,n);				//�擪����:�̎�O�܂�
				wstring val = str.substr(n+1);
				int nVal;
//#ifdef _MFCBSCH	//Qt-BSch doesn't support layer.
				if(var==_T("L")){
					nVal = _tstoi(val.c_str());
					if(isValidLayer(nVal)){
						m_Layer	= nVal;			
					}
				}else
//#endif
				if(var==_T("X")){
					nVal = _tstoi(val.c_str());
					if(pptOrigin) nVal += pptOrigin->x();		
					m_p1.setX(nVal);
					bInitX1 = true;
				}else
				if(var==_T("Y")){
					nVal = _tstoi(val.c_str());
					if(pptOrigin) nVal += pptOrigin->y();		
					m_p1.setY(nVal);
					bInitY1 = true;
				}else
				if(var==_T("LIB")){
					m_orgname=val;
					bInitLib = true;
					//m_compInfoIndex = g_SearchComponentIndex(val.c_str(),NULL,NULL,NULL);
					if(!bEmbLib){
						const SCompIndex* compInfoIndex = g_SearchComponentIndex(val.c_str(),NULL,NULL,NULL);
						if(compInfoIndex){
							m_compInfoIndex = new SCompIndex;
							m_compInfoIndex->m_pCompInfo = new SCompInfo(*(compInfoIndex->m_pCompInfo));
						}else{
							m_compInfoIndex = NULL;
						}
					}
				}else
				if(var==_T("DIR")){
					nVal = _tstoi(val.c_str());
					if(nVal>=0 && nVal <8){
						m_dir = nVal;
					}
				}else
				if(var==_T("BLK")){
					if(m_compInfoIndex == NULL){
						m_block = 0;
					}else{
						nVal = _tstoi(val.c_str());
						if(nVal>=0 && nVal < m_compInfoIndex->block()){
							m_block = nVal;
						}else{
							m_block = 0;
						}
					}
				}else
				if(var==_T("N")){
					m_name=val;
				}else
				if(var==_T("ND")){
					nVal = _tstoi(val.c_str());
					m_name_horiz=(nVal!=0);
				}else
				if(var==_T("NX")){
					nVal = _tstoi(val.c_str());
					m_name_pos.setX(nVal);
				}else
				if(var==_T("NY")){
					nVal = _tstoi(val.c_str());
					m_name_pos.setY(nVal);
				}else
				if(var==_T("NH")){
					nVal = _tstoi(val.c_str());
					m_name_hide = (nVal!=0);
				}else
				if(var==_T("R")){
					m_refnum=val;
				}else
				if(var==_T("RD")){
					nVal = _tstoi(val.c_str());
					m_refnum_horiz=(nVal!=0);
				}else
				if(var==_T("RX")){
					nVal = _tstoi(val.c_str());
					m_refnum_pos.setX(nVal);
				}else
				if(var==_T("RY")){
					nVal = _tstoi(val.c_str());
					m_refnum_pos.setY(nVal);
				}else
				if(var==_T("RH")){
					nVal = _tstoi(val.c_str());
					m_refnum_hide = (nVal!=0);
				}else
				if(var==_T("NOTE")){
					m_note=val;
				}else
				if(var==_T("PKG")){			//2009/01/06
					m_packagename=val;
				}else
				if(var==_T("MFR")){			//2009/01/07
					m_mfr=val;
				}else
				if(var==_T("MFRPN")){		//2009/01/07
					m_mfrpnum=val;
				}
			}
		}
	}
	setPinLtrb();
	return true;
}


bool SXBSchComponent::writeCe3(SWriteCE3& wce3,const SPoint* pptOrigin)
{
//	char sz[32];

	int x = m_p1.x();
	int y = m_p1.y();

	wstring str(_T(""));

	if(pptOrigin){
		x -= pptOrigin->x();
		y -= pptOrigin->y();
	}

	wce3.WriteRecord(_T("+COMPONENT"));
	writeEmbLibCe3(wce3);
	wce3.WriteRecordInt(_T("L"),m_Layer);
	wce3.WriteRecordInt(_T("X"),x);
	wce3.WriteRecordInt(_T("Y"),y);


	wce3.WriteRecordString(_T("LIB"),m_orgname);

	wce3.WriteRecordInt(_T("DIR"),m_dir);

	wce3.WriteRecordInt(_T("BLK"),m_block);

	wce3.WriteRecordString(_T("N"),m_name);

	wce3.WriteRecordInt(_T("ND"),(m_name_horiz ? 1:0));

	wce3.WriteRecordInt(_T("NX"),m_name_pos.x());

	wce3.WriteRecordInt(_T("NY"),m_name_pos.y());

	wce3.WriteRecordInt(_T("NH"),(m_name_hide ? 1: 0));

	wce3.WriteRecordString(_T("R"),m_refnum);

	wce3.WriteRecordInt(_T("RD"),(m_refnum_horiz ? 1:0));

	wce3.WriteRecordInt(_T("RX"),m_refnum_pos.x());

	wce3.WriteRecordInt(_T("RY"),m_refnum_pos.y());

	wce3.WriteRecordInt(_T("RH"),(m_refnum_hide ? 1: 0));

	wce3.WriteRecordString(_T("NOTE"),m_note);

	wce3.WriteRecordString(_T("PKG"),m_packagename);

	wce3.WriteRecordString(_T("MFR"),m_mfr);

	wce3.WriteRecordString(_T("MFRPN"),m_mfrpnum);


	wce3.WriteRecord(_T("-COMPONENT"));
	wce3.WriteEOL();
	return true;
}


void SXBSchComponent::setCompInfoIndex(const SCompIndex* pIndex)
{
	if(pIndex == NULL)return;

	if(m_compInfoIndex != NULL){
		delete m_compInfoIndex->m_pCompInfo;
		delete m_compInfoIndex;
	}

//	m_compInfoIndex = pIndex;
//	if(m_block >= m_compInfoIndex->block()){
//		m_block = 0;
//	}
//	m_orgname = m_compInfoIndex->name();

	m_orgname = pIndex->name();
	m_compInfoIndex = new SCompIndex;
	m_compInfoIndex->m_pCompInfo = new SCompInfo(*(pIndex->m_pCompInfo));	
	if(m_block >= m_compInfoIndex->block()){
		m_block = 0;
	}
	
	setPinLtrb();
}
	
//LTRB��10�s�N�Z�����s�������݂��Ă��邩�ǂ����̃t���O�̐ݒ�
void SXBSchComponent::setPinLtrb()
{
	SPoint pt;	//�_�~�[
	int ltrb;

	m_pinltrb=0;

	if(m_compInfoIndex == NULL){
		return;
	}
	int nMax = m_compInfoIndex->pinCount();
	for(int nIndex = 0;nIndex < nMax;nIndex++){
		const SPin* pPin = pinEnd(nIndex,ltrb,pt);
		if(pPin){
			if((pPin->type() & PIN_TYPE_ZLENG) == 0){
				m_pinltrb |= ((1<<ltrb)&0xf);
			}
		}
	}
}

//n�Ԗڂ̃s���̈ʒu�𓾂�B
SPoint SXBSchComponent::pinPosition(int nIndex)const
{
	SPoint pt;
	int dummy;
	pinEnd(nIndex,dummy,pt);
	return pt;
}

int SXBSchComponent::pinCount() const
{
	if(m_compInfoIndex == NULL){
		return 0;
	}else{
		return m_compInfoIndex->pinCount();
	}
}


//n�Ԗڂ̃s����NC�t���O��bSet�ɂ���
void SXBSchComponent::setPinNC(int n,bool bSet)
{
	if(m_compInfoIndex == NULL) return;
	SPin* pininfo =  m_compInfoIndex->pin(n);
	pininfo->set_nc(bSet);
}

//n�Ԗڂ̃s����NC�t���O�𓾂�
bool SXBSchComponent::pinNC(int n) const
{
	if(m_compInfoIndex == NULL) return false;
	const SPin* pininfo =  m_compInfoIndex->pin(n);
	return pininfo->nc();
}

//NC�t���O���N���A����
void SXBSchComponent::presetPinNC()
{
	if(m_compInfoIndex == NULL) return;
	int n=m_compInfoIndex->pinCount();
	int i;
	for(i=0;i<n;i++){
		setPinNC(i,true);
	}
}

SPin* SXBSchComponent::pinInfo(int nIndex)
{
	return m_compInfoIndex->pin(nIndex);
}

//�s���̍������̈ʒu�����擾����
const SPin*  SXBSchComponent::pinLoc(int nIndex,int& nLtrb,SPoint& ptLoc) const
{
	if(m_compInfoIndex == NULL){
		return NULL;
	}
	const SPin* pininfo =  m_compInfoIndex->pin(nIndex);
	if(pininfo == NULL){
		return NULL;
	}
	int ltrb = pininfo->ltrb(); // L:0 T:1 R:2 B:3
	int offset = pininfo->offset()*10;

	nLtrb = (ltrb + (m_dir & 3)) &3;
	if(m_dir & 4){
		if(nLtrb==0){
			nLtrb = 2;
		}else if(nLtrb==2){
			nLtrb = 0;
		}
	}
	SSize wh = size();
	int w = wh.w();
	int h = wh.h();
	int x = m_p1.x();
	int y = m_p1.y();
	switch(nLtrb){
	case 0:		// L
		ptLoc.setX(x-w);
		if(m_dir == 2 || m_dir == 3 || m_dir == 6 || m_dir == 7){
			ptLoc.setY(y-offset);
		}else{
			ptLoc.setY(y-h+offset);
		}
		break;
	case 1:		// T
		ptLoc.setY(y-h);
		if(m_dir == 1 || m_dir == 2 || m_dir == 4 || m_dir == 7){
			ptLoc.setX(x-offset);
		}else{
			ptLoc.setX(x-w+offset);
		}
		break;
	case 2:		// R
		ptLoc.setX(x);
		if(m_dir == 2 || m_dir == 3 || m_dir == 6 || m_dir == 7){
			ptLoc.setY(y-offset);
		}else{
			ptLoc.setY(y-h+offset);
		}
		break;
	case 3:		// B
		ptLoc.setY(y);
		if(m_dir == 1 || m_dir == 2 || m_dir == 4 || m_dir == 7){
			ptLoc.setX(x-offset);
		}else{
			ptLoc.setX(x-w+offset);
		}
		break;
	}
	return pininfo;	
}

//�s���̐�̈ʒu�����擾����
const SPin* SXBSchComponent::pinEnd(int nIndex,int& nLtrb,SPoint& ptEnd) const
{
	const SPin*  pPin = pinLoc(nIndex,nLtrb,ptEnd);
	if(!pPin)return NULL;
	if(pPin->type() & PIN_TYPE_ZLENG){
		return pPin;
	}else{
		switch(nLtrb){
			case 0:	//L
				ptEnd.setX(ptEnd.x()-PIN_LENGTH);
				break;
			case 1:	//T
				ptEnd.setY(ptEnd.y()-PIN_LENGTH);
				break;
			case 2:	//R
				ptEnd.setX(ptEnd.x()+PIN_LENGTH);
				break;
			default://			case 3:	//B
				ptEnd.setY(ptEnd.y()+PIN_LENGTH);
		}
		return pPin;
	}


	//if(m_compInfoIndex == NULL){
	//	return NULL;
	//}
	//const SPin* pininfo =  m_compInfoIndex->pin(nIndex);
	//if(pininfo == NULL){
	//	return NULL;
	//}
	//int ltrb = pininfo->ltrb(); // L:0 T:1 R:2 B:3
	//int offset = pininfo->offset()*10;

	//nLtrb = (ltrb + (m_dir & 3)) &3;
	//if(m_dir & 4){
	//	if(nLtrb==0){
	//		nLtrb = 2;
	//	}else if(nLtrb==2){
	//		nLtrb = 0;
	//	}
	//}
	//SSize wh = size();
	//int w = wh.w();
	//int h = wh.h();
	//int x = m_p1.x();
	//int y = m_p1.y();
	//switch(nLtrb){
	//case 0:		// L
	//	ptEnd.setX(x-w-PIN_LENGTH);
	//	if(m_dir == 2 || m_dir == 3 || m_dir == 6 || m_dir == 7){
	//		ptEnd.setY(y-offset);
	//	}else{
	//		ptEnd.setY(y-h+offset);
	//	}
	//	break;
	//case 1:		// T
	//	ptEnd.setY(y-h-PIN_LENGTH);
	//	if(m_dir == 1 || m_dir == 2 || m_dir == 4 || m_dir == 7){
	//		ptEnd.setX(x-offset);
	//	}else{
	//		ptEnd.setX(x-w+offset);
	//	}
	//	break;
	//case 2:		// R
	//	ptEnd.setX(x+PIN_LENGTH);
	//	if(m_dir == 2 || m_dir == 3 || m_dir == 6 || m_dir == 7){
	//		ptEnd.setY(y-offset);
	//	}else{
	//		ptEnd.setY(y-h+offset);
	//	}
	//	break;
	//case 3:		// B
	//	ptEnd.setY(y+PIN_LENGTH);
	//	if(m_dir == 1 || m_dir == 2 || m_dir == 4 || m_dir == 7){
	//		ptEnd.setX(x-offset);
	//	}else{
	//		ptEnd.setX(x-w+offset);
	//	}
	//	break;
	//}
	//return pininfo;	
}