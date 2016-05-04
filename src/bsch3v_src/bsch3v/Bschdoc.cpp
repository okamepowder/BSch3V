/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// BSchDoc.cpp : CBSchDoc �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"

#include <stdio.h>
#include <assert.h>
#include <string>
#include <list>
using namespace std;


#include <memory.h>
#include "BSch.h"


//#include "xbschdoc.h"
//#include "xbschdocsch.h"
#include "BSchDoc.h"

//#include "PartLib.h"
#include "BSchFont.h"
#include "Global.h"
#include "MainFrm.h"

//#include "ReadERC.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBSchDoc

IMPLEMENT_DYNCREATE(CBSchDoc, CDocument)

BEGIN_MESSAGE_MAP(CBSchDoc, CDocument)
	//{{AFX_MSG_MAP(CBSchDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBSchDoc �N���X�̍\�z/����

CBSchDoc::CBSchDoc()
{
	// TODO: ���̈ʒu�ɂP�񂾂��\�z�p�̃R�[�h��ǉ����Ă��������B
//	m_nTopData		=0;
//	m_nDataDepth	=0;
//	m_fTmpData		=0;
	//m_bSelectForDrag=FALSE;
	SetSheetSize(1,0,0);	//1997/02/03
//	m_wShowLayer	=0xFF;  //1998/06/27
//	m_nEditLayer	=0;		//1998/06/27
//	m_fEditHighLight=FALSE;	//1998/06/29
	// INI�t�@�C���ɂ��O���[�o���ϐ��̏�����
	bool bTidyUp = (AfxGetApp()->GetProfileInt(_T("Option"),_T("AutoCorrect"),1) ? true : false);
	AfxGetApp()->WriteProfileInt(_T("Option"),_T("AutoCorrect"),(bTidyUp?1:0));
	setTidyUpEnable(bTidyUp);
}

CBSchDoc::~CBSchDoc()
{
}

BOOL CBSchDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)
	// SetSheetSize(1);
	g_docPath = _T("");
	g_docTimeStamp  = _T("");
	g_docDate = _T(""); 
	g_printTime = _T("");

	CSize sz;
	IniReadSheetSize(&sz);
	SetSheetSize(-1,sz.cx,sz.cy);//�}�ʃT�C�Y�����W�X�g�����瓾��
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CBSchDoc �V���A���C�[�[�V����

void CBSchDoc::Serialize(CArchive& ar)
{
//	CFile* pf;
	if (ar.IsStoring())
	{
//		// TODO: ���̈ʒu�ɕۑ��p�̃R�[�h��ǉ����Ă��������B
//		ar.Flush();
//		pf=ar.GetFile();
//		WriteCE2(pf);
//		pf->Flush();
//		DeleteContents();
//		pf->SeekToBegin();
//		ReadCE2(pf);
//		UpdateAllViews(NULL);
	}
	else
	{
		// TODO: ���̈ʒu�ɓǂݍ��ݗp�̃R�[�h��ǉ����Ă��������B
//		ar.Flush();
//		pf=ar.GetFile();
//		ReadCE2(pf);
	}
}



/////////////////////////////////////////////////////////////////////////////
// CBSchDoc �N���X�̐f�f

#ifdef _DEBUG
void CBSchDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBSchDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBSchDoc �R�}���h

//�}�ʃT�C�Y�̔z��
SIZE CBSchDoc::m_tablesizeSheet[MAX_SHEETSIZE]={ 640, 400,
												1000, 750,
												1550,1050,
												2250,1550,
												3250,2250};



////No operation in Qt-BSch
//void CBSchDoc::IncrementDataDepth()
//{
//}

//���W�X�g������}�ʃT�C�Y�𓾂�
//int  CBSchDoc::IniReadSheetSize()
//{
//	return AfxGetApp()->GetProfileInt("Option","SheetSize",1);
//}

//���W�X�g������}�ʃT�C�Y�𓾂�
void  CBSchDoc::IniReadSheetSize(CSize* psize)
{
	psize->cx = AfxGetApp()->GetProfileInt(_T("Option"),_T("SheetSizeX"),0);
	psize->cy = AfxGetApp()->GetProfileInt(_T("Option"),_T("SheetSizeY"),0);

	if(psize->cx < SHEETSIZE_X_MIN) psize->cx=SHEETSIZE_X_MIN;
	else if(psize->cx > SHEETSIZE_X_MAX) psize->cx=SHEETSIZE_X_MAX;
	if(psize->cy < SHEETSIZE_Y_MIN) psize->cy=SHEETSIZE_Y_MIN;
	else if(psize->cy > SHEETSIZE_Y_MAX) psize->cy= SHEETSIZE_Y_MAX;
}


//���W�X�g���ɐ}�ʃT�C�Y����������
//void CBSchDoc::IniWriteSheetSize(int nSize)
//{
//	AfxGetApp()->WriteProfileInt("Option","SheetSize",nSize);
//}

//���W�X�g���ɐ}�ʃT�C�Y����������
void CBSchDoc::IniWriteSheetSize(CSize* pSize)
{
	AfxGetApp()->WriteProfileInt(_T("Option"),_T("SheetSizeX"),pSize->cx);
	AfxGetApp()->WriteProfileInt(_T("Option"),_T("SheetSizeY"),pSize->cy);
}

//�ꎞ�f�[�^�̌��_��Ԃ�
CPoint CBSchDoc::GetTmpOrigin()
{
	SPoint pt = xbschdocsch.tempSnapPoint();
	return CPoint(pt.x(),pt.y());
}

//�ꎞ�f�[�^�̐�L�͈͂�Ԃ�
CRect CBSchDoc::GetTmpRect()
{
	SRect rc= xbschdocsch.tempArea();
	return CRect(rc.l(),rc.t(),rc.r(),rc.b());
}

//�I����Ԃ̉���
void CBSchDoc::FixSelection(int option)
{
	if(xbschdocsch.resetSelect(option)){
		SetModifiedFlag(TRUE);
	}
}	

//��`�͈͂ł̑I��
//�w�肵����`�͈͂Ɉ��������������̂��o�b�N�A�b�v���X�g�Ɉړ����āA����Ɉꎞ�f�[�^�ɕ��ʂ���
int CBSchDoc::SetSel(CRect* prc,SelectOption& refSelOption)
{
	SRect rect(prc->left,prc->top,prc->Width(),prc->Height());
	return xbschdocsch.select(rect,refSelOption);
}

//�_�w��ł̑I��
//SetSel(CRect*)�ƈ���āA�u�d�Ȃ��Ă���x���v���ł��傫�����̈������I������
int CBSchDoc::SetSel(CPoint* pp,SelectOption& refSelOption)
{
	SPoint point(pp->x,pp->y);
	return xbschdocsch.select(point,refSelOption);
}

//��`�͈͂ł̃h���b�O���[�h�I��
int CBSchDoc::SetDrag(CRect* prc,bool bCutOffPinWire)
{
	SRect rect(prc->left,prc->top,prc->Width(),prc->Height());
	return xbschdocsch.selectForDrag(rect,bCutOffPinWire);
}


//�_�w��ł̃h���b�O���[�h�I��
int CBSchDoc::SetDrag(CPoint* pp,bool bCutOffPinWire)
{
	CRect rc;
	rc.left  =pp->x-2;
	rc.top   =pp->y-2;
	rc.right =pp->x+2;
	rc.bottom=pp->y+2;
	
	return SetDrag(&rc,bCutOffPinWire);
}
	

//�ꎞ�f�[�^�̃��X�g�𓾂�
XBSchObjList* CBSchDoc::GetTmpList()
{
	return &xbschdocsch.m_listTemp;
}

//�ꎞ�f�[�^���P��}�ʃI�u�W�F�N�g�̂Ƃ��A�����Ԃ�
SXBSchObj* CBSchDoc::GetTmp()
{
	if(GetTmpDataCount()==1){
		XBSchObjListIterator pObjIte = xbschdocsch.m_listTemp.begin();
		return *pObjIte;
	}else{ 
		return NULL;
	}
}

//�w�肵���_���ꎞ�f�[�^�̏ォ�ǂ���
//�h���b�O���[�h�łȂ��P��f�[�^�̏ꍇ�́A�}�ʃI�u�W�F�N�g��InAreaPoint()���g��
int CBSchDoc::IsOnTmpData(CPoint* pp,int nOption,SXBSchObj** ppObj,int* pRetOption)
{
	return xbschdocsch.isOnTempData(SPoint(pp->x,pp->y),nOption,ppObj,pRetOption);
}

//�y�[�X�g
//���̎菇�ŃN���b�v�{�[�h�̃f�[�^�𓾂�
// 1.�N���b�v�{�[�h���烁�����u���b�N�𓾂�
// 2.���̃������u���b�N�� CMemFile������������
// 3.ReadCE2�����s����B
void CBSchDoc::Paste(CPoint* pOrigin)
{
	SPoint pt(pOrigin->x,pOrigin->y);
	xbschdocsch.paste(pt);
	// >>> 2005/01/30 �y�[�X�g�f�[�^���t�B�b�N�X���ꂽ�Ƃ���SetModifiedFlag()����悤�ɕύX
    //		SetModifiedFlag(TRUE);
	//		xbschdocsch.setDirty(true);
	// <<< 2005/01/30
}

//�R�s�[
void CBSchDoc::Copy()
{
	xbschdocsch.copy();
	//SetModifiedFlag(TRUE);
}

//�J�b�g
void CBSchDoc::Cut()
{
	Copy();
	Clear();
}


//�N���A
void CBSchDoc::Clear()
{
	if(xbschdocsch.del()){
		SetModifiedFlag(TRUE);
	}
	// >>> 2005/01/30
	//     �_�[�e�B�t���O�̐ݒ�� xbschdocsch.del()�̒��ōs���悤�ɕύX
	//	xbschdocsch.setDirty(true);
	// <<< 2005/01/30
}

//�A���h�D
void CBSchDoc::Undo()
{
	xbschdocsch.undo();
}

bool CBSchDoc::Redo()
{
	return xbschdocsch.redo();
}


//��]����
//�ꎞ�f�[�^���P��f�[�^�ŁA���A����EditFlag��ENABLE_ROTATE���L���ł���Ƃ�
void CBSchDoc::Rotate()
{
	xbschdocsch.rotate();
	// >>> 2005/01/30 
	//	�f�[�^���t�B�b�N�X���ꂽ�Ƃ���SetModifiedFlag()����悤�ɕύX
	//  �_�[�e�B�t���O�ݒ��xbschdocsch.rotate()�ōs��
	//SetModifiedFlag(TRUE);
	//xbschdocsch.setDirty(true);
	// <<< 2005/01/30
}


//���E���]����
//�ꎞ�f�[�^���P��f�[�^�ŁA���A����EditFlag��ENABLE_MIRROR���L���ł���Ƃ�
void CBSchDoc::Mirror()
{
	xbschdocsch.mirror();
	// >>> 2005/01/30 
	//	�f�[�^���t�B�b�N�X���ꂽ�Ƃ���SetModifiedFlag()����悤�ɕύX
	//  �_�[�e�B�t���O�ݒ��xbschdocsch.mirror()�ōs��
	//SetModifiedFlag(TRUE);
	//xbschdocsch.setDirty(true);
	// <<< 2005/01/30
}

//�ړ�����
//���� CPoint*��
//�ꎞ�f�[�^���ړ�����
//�ړ���A�ꎞ�f�[�^�̃_�[�e�B�t���O���Z�b�g����B
void CBSchDoc::Shift(CPoint* pOrigin)
{
	xbschdocsch.moveTempData(pOrigin->x,pOrigin->y);
	// >>> 2005/01/30 
	//	�f�[�^���t�B�b�N�X���ꂽ�Ƃ���SetModifiedFlag()����悤�ɕύX
	//  �_�[�e�B�t���O�ݒ��xbschdocsch.moveTempData()�ōs��
	//SetModifiedFlag(TRUE);
	//xbschdocsch.setDirty(true);
	// <<< 2005/01/30
}

//�ړ�����
//���� Csize*��
void CBSchDoc::Shift(CSize*  psizeOffset)
{
	xbschdocsch.moveTempData(psizeOffset->cx,psizeOffset->cy);
	// >>> 2005/01/30 
	//	�f�[�^���t�B�b�N�X���ꂽ�Ƃ���SetModifiedFlag()����悤�ɕύX
	//  �_�[�e�B�t���O�ݒ��xbschdocsch.moveTempData()�ōs��
	//SetModifiedFlag(TRUE);
	//xbschdocsch.setDirty(true);
	// <<< 2005/01/30
}



//�_�[�e�B�t���O���Z�b�g����
void CBSchDoc::SetDirty()
{
	SetModifiedFlag(TRUE);
	xbschdocsch.setDirty(true);
}

//�ꎞ�f�[�^�̃_�[�e�B�t���O���Z�b�g����
void CBSchDoc::SetTempDirty()
{
	// >>> 2005/01/30
	//	�f�[�^���t�B�b�N�X���ꂽ�Ƃ���SetModifiedFlag()����悤�ɕύX
	//	SetModifiedFlag(TRUE);
	// <<< 2005/01/30
	xbschdocsch.setTempDirty(true);
}


//�f�[�^���󂩂ǂ�����Ԃ�
BOOL CBSchDoc::IsEmpty()
{
	return xbschdocsch.isEmpty();
}

//�I�����ꂽ���̂����݂��邩�ǂ���
//m_fTmpData�̂����ATMPDATA_NEW�܂���TMPDATA_MOVED�̃r�b�g�������Ă��āA����
//                  TMPDATA_FOR_DRAG�̃r�b�g��OFF
BOOL CBSchDoc::CanEdit()
{
	return xbschdocsch.canCopy();
}

//�y�[�X�g�\���ǂ���
//BSch�Ǝ��t�H�[�}�b�g�̃f�[�^���N���b�v�{�[�h�ɑ��݂���Ή\�Ƃ���B
BOOL CBSchDoc::CanPaste()
{
	return xbschdocsch.canPaste();
}

//�A���h�D�\���ǂ���
//m_nDataDepth�� 1 �ȏ�Ȃ�\�B
//m_nDataDepth�� 0 �ł��ꎞ�f�[�^���L���Ȃ�\�B
BOOL CBSchDoc::CanUndo()
{
	return xbschdocsch.canUndo();
}

BOOL CBSchDoc::CanRedo()
{
	return xbschdocsch.canRedo();
}


//�A�g���r���[�g�̕ύX���\���ǂ���
//�ꎞ�f�[�^���P��f�[�^�ŁA���A����EditFlag��ENABLE_ATTRIBUTE���L���ł���Ƃ�
BOOL CBSchDoc::CanAttribute()
{
	return xbschdocsch.canAttribute();
}

//��]���\���ǂ���
//�ꎞ�f�[�^���P��f�[�^�ŁA���A����EditFlag��ENABLE_ROTATE���L���ł���Ƃ�
BOOL CBSchDoc::CanRotate()
{
	return xbschdocsch.canRotate();
}

//���E���]���\���ǂ���
//�ꎞ�f�[�^���P��f�[�^�ŁA���A����EditFlag��ENABLE_MIRROR���L���ł���Ƃ�
BOOL CBSchDoc::CanMirror()
{
	return xbschdocsch.canMirror();
}

//�_�̕ҏW���\��
BOOL CBSchDoc::CanEditPoints()
{
	return xbschdocsch.canEditPoints();
}

//�����̕ύX���\���H
BOOL CBSchDoc::CanChangeOrder()
{
	return xbschdocsch.canChangeOrder();
}


//�u���̌����v���\���ǂ���
BOOL CBSchDoc::CanFindNext()
{
	return xbschdocsch.canFindNext();
}

const SXBSchObj* CBSchDoc::FindStart(const TCHAR* sz,bool bMatchWhole,bool bCaseSensitive,unsigned int targetFlags,bool selectWhenFound)
{
	return xbschdocsch.findStart(sz,bMatchWhole,bCaseSensitive,targetFlags,selectWhenFound);
}

//���i��1�����P�ƂőI������Ă��邩
const SXBSchComponent* CBSchDoc::isComponentSelectedSingly()
{
	return xbschdocsch.isComponentSelectedSingly();
}

//�J�[�\�������i�̏�ɂ��邩�ǂ���
const SXBSchComponent* CBSchDoc::isOnComponent(CPoint *pp)
{
	SPoint point(pp->x,pp->y);
	return xbschdocsch.isOnComponent(point);
}





const SXBSchObj* CBSchDoc::Find()
{
	return xbschdocsch.find(false);
}

void CBSchDoc::GetFoundText(CString& str)
{
	str = xbschdocsch.m_foundString.c_str();
}


//�}�ʃI�u�W�F�N�g�̉��ǉ�
BOOL CBSchDoc::AddData(SXBSchObj* pObj)
{
	xbschdocsch.addSelect(pObj);
	// >>> 2005/01/30
	//	�y�[�X�g�f�[�^���t�B�b�N�X���ꂽ�Ƃ���SetModifiedFlag()����悤�ɕύX
	//	SetModifiedFlag(TRUE);
	// <<< 2005/01/30
	//xbschdocsch.setDirty(true);
	return TRUE;
}

//�擪����n�ڂ̃f�[�^�̃��X�g�ւ̃|�C���^�𓾂�
//n���L���ȃ��C���f�[�^�̐����z���Ă�����NULL��Ԃ�
XBSchObjList* CBSchDoc::GetMainData(int /* =0*/)
{
	return &xbschdocsch.m_listMain;
}

//�}�ʃT�C�Y��ԍ��ŃZ�b�g����
void CBSchDoc::SetSheetSize(int n, int w, int h)
{
	if(w < SHEETSIZE_X_MIN) w=SHEETSIZE_X_MIN;
	else if(w > SHEETSIZE_X_MAX) w=SHEETSIZE_X_MAX;
	if(h < SHEETSIZE_Y_MIN) h=SHEETSIZE_Y_MIN;
	else if(h > SHEETSIZE_Y_MAX) h=SHEETSIZE_Y_MAX;

	
	if(n<0 || n>=MAX_SHEETSIZE){//�͈͊O�Ȃ�0�ɂ���
		//TRACE("\nRange error (CBSchDoc::SetSheetSize)");//DEBUG���[�h�Ń��b�Z�[�W
		xbschdocsch.SetSheetSize(SSize(w,h));
	}else{
		xbschdocsch.SetSheetSize(SSize(m_tablesizeSheet[n].cx,m_tablesizeSheet[n].cy));
	}
	SetModifiedFlag(TRUE);
	//xbschdocsch.setDirty(true);
}


//�}�ʃT�C�Y��Ԃ�
CSize CBSchDoc::GetSheetSize()
{
	SSize sz=xbschdocsch.SheetSize();
	return CSize(sz.w(),sz.h());
}

//�}�ʃT�C�Y��Ԃ�
int	CBSchDoc::GetSheetSizeIndex()
{
	SSize sz=xbschdocsch.SheetSize();
	for(int i=0;i<MAX_SHEETSIZE;i++){
		SIZE ssz = m_tablesizeSheet[i];
		if(sz.w() == ssz.cx && sz.h() == ssz.cy) return i;
	}
	return -1;
}


//�f�[�^�̑S����
//�A�v���P�[�V�����t���[�����[�N���玟�̏ꍇ�ɌĂяo�����
//			�V�K�쐬
//			�t�@�C���ǂݍ���
//			�v���O�����̏I��
//���C���f�[�^�̃��X�g�Ɋ܂܂�邷�ׂẴf�[�^��Delete�B
//m_nTopData��m_nDataDepth�����Z�b�g�B
void CBSchDoc::DeleteContents() 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	xbschdocsch.init();
	SetModifiedFlag(FALSE);
	//CDocument::DeleteContents();	//��{�N���X�͉������Ȃ��A�������B
}


//���C���[�̈ړ�
void CBSchDoc::MoveLayer(int nLayer)
{
	xbschdocsch.moveToLayer(nLayer);
}



BOOL CBSchDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	int result = xbschdocsch.writeFile(lpszPathName);
	if(result==0){
		SetModifiedFlag(FALSE);
		if(g_bDisplayNcPinMark) setNcPinFlag();
		UpdateAllViews(NULL);
		TRACE(_T("m_strPathName : %s\n"),m_strPathName);
		return TRUE;
	}else{
		TRACE(_T("m_strPathName : %s\n"),m_strPathName);
		return FALSE;
	}
	//return CDocument::OnSaveDocument(lpszPathName);
}

BOOL CBSchDoc::ReloadFile()
{
	TRACE(_T("m_strPathName : %s\n"),m_strPathName);
	BOOL result = OnOpenDocument(m_strPathName);
	UpdateAllViews(NULL);
	return result;
}

BOOL CBSchDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	CWnd* pWndOpened=((CMainFrame*)AfxGetMainWnd())->CheckOpened(lpszPathName);
	if(pWndOpened){
		if(pWndOpened->IsIconic()){
			pWndOpened->ShowWindow(SW_RESTORE);
		}
		pWndOpened->SetForegroundWindow();
		AfxGetMainWnd()->PostMessage(WM_CLOSE);
		return TRUE;
	}

	//DeleteContents();
	int result = xbschdocsch.readFile(lpszPathName);
	if(result==0){
		SetModifiedFlag(FALSE);
		if(g_bDisplayNcPinMark) setNcPinFlag();
		if(xbschdocsch.createdNewerVersion()){
			AfxMessageBox(IDS_LATERVERSION,MB_ICONEXCLAMATION,MB_OK);
		}
		TRACE(_T("m_strPathName : %s\n"),m_strPathName);
		return TRUE;
	}else{
		TRACE(_T("m_strPathName : %s\n"),m_strPathName);
		return FALSE;
	}
		
//	if (!CDocument::OnOpenDocument(lpszPathName))
//		return FALSE;
	
	// TODO: ���̈ʒu�ɌŗL�̍쐬�p�R�[�h��ǉ����Ă�������
	
	//return TRUE;
}

BOOL CBSchDoc::IsModified()
{
	return xbschdocsch.isDirty();
}


void CBSchDoc::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU)
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	g_docPath = lpszPathName;

	HANDLE file= ::CreateFile(lpszPathName,GENERIC_READ,0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if(file == INVALID_HANDLE_VALUE){
		g_docTimeStamp = _T("");
	}else{
		FILETIME fileTime;
		::GetFileTime(file, NULL, NULL, &fileTime);
		::CloseHandle(file);
		CTime time = CTime(fileTime);
		CString s = time.Format(_T("%c"));
		g_docTimeStamp = s;
		s = time.Format(_T("%x"));
		g_docDate = s;
	}

	CDocument::SetPathName(lpszPathName, bAddToMRU);
}
