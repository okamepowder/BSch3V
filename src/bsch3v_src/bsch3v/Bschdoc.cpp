/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// BSchDoc.cpp : CBSchDoc クラスの動作の定義を行います。
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
// CBSchDoc クラスの構築/消滅

CBSchDoc::CBSchDoc()
{
	// TODO: この位置に１回だけ構築用のコードを追加してください。
//	m_nTopData		=0;
//	m_nDataDepth	=0;
//	m_fTmpData		=0;
	//m_bSelectForDrag=FALSE;
	SetSheetSize(1,0,0);	//1997/02/03
//	m_wShowLayer	=0xFF;  //1998/06/27
//	m_nEditLayer	=0;		//1998/06/27
//	m_fEditHighLight=FALSE;	//1998/06/29
	// INIファイルによるグローバル変数の初期化
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

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)
	// SetSheetSize(1);
	g_docPath = _T("");
	g_docTimeStamp  = _T("");
	g_docDate = _T(""); 
	g_printTime = _T("");

	CSize sz;
	IniReadSheetSize(&sz);
	SetSheetSize(-1,sz.cx,sz.cy);//図面サイズをレジストリから得る
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CBSchDoc シリアライゼーション

void CBSchDoc::Serialize(CArchive& ar)
{
//	CFile* pf;
	if (ar.IsStoring())
	{
//		// TODO: この位置に保存用のコードを追加してください。
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
		// TODO: この位置に読み込み用のコードを追加してください。
//		ar.Flush();
//		pf=ar.GetFile();
//		ReadCE2(pf);
	}
}



/////////////////////////////////////////////////////////////////////////////
// CBSchDoc クラスの診断

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
// CBSchDoc コマンド

//図面サイズの配列
SIZE CBSchDoc::m_tablesizeSheet[MAX_SHEETSIZE]={ 640, 400,
												1000, 750,
												1550,1050,
												2250,1550,
												3250,2250};



////No operation in Qt-BSch
//void CBSchDoc::IncrementDataDepth()
//{
//}

//レジストリから図面サイズを得る
//int  CBSchDoc::IniReadSheetSize()
//{
//	return AfxGetApp()->GetProfileInt("Option","SheetSize",1);
//}

//レジストリから図面サイズを得る
void  CBSchDoc::IniReadSheetSize(CSize* psize)
{
	psize->cx = AfxGetApp()->GetProfileInt(_T("Option"),_T("SheetSizeX"),0);
	psize->cy = AfxGetApp()->GetProfileInt(_T("Option"),_T("SheetSizeY"),0);

	if(psize->cx < SHEETSIZE_X_MIN) psize->cx=SHEETSIZE_X_MIN;
	else if(psize->cx > SHEETSIZE_X_MAX) psize->cx=SHEETSIZE_X_MAX;
	if(psize->cy < SHEETSIZE_Y_MIN) psize->cy=SHEETSIZE_Y_MIN;
	else if(psize->cy > SHEETSIZE_Y_MAX) psize->cy= SHEETSIZE_Y_MAX;
}


//レジストリに図面サイズを書き込む
//void CBSchDoc::IniWriteSheetSize(int nSize)
//{
//	AfxGetApp()->WriteProfileInt("Option","SheetSize",nSize);
//}

//レジストリに図面サイズを書き込む
void CBSchDoc::IniWriteSheetSize(CSize* pSize)
{
	AfxGetApp()->WriteProfileInt(_T("Option"),_T("SheetSizeX"),pSize->cx);
	AfxGetApp()->WriteProfileInt(_T("Option"),_T("SheetSizeY"),pSize->cy);
}

//一時データの原点を返す
CPoint CBSchDoc::GetTmpOrigin()
{
	SPoint pt = xbschdocsch.tempSnapPoint();
	return CPoint(pt.x(),pt.y());
}

//一時データの占有範囲を返す
CRect CBSchDoc::GetTmpRect()
{
	SRect rc= xbschdocsch.tempArea();
	return CRect(rc.l(),rc.t(),rc.r(),rc.b());
}

//選択状態の解除
void CBSchDoc::FixSelection(int option)
{
	if(xbschdocsch.resetSelect(option)){
		SetModifiedFlag(TRUE);
	}
}	

//矩形範囲での選択
//指定した矩形範囲に引っかかったものをバックアップリストに移動して、さらに一時データに複写する
int CBSchDoc::SetSel(CRect* prc,SelectOption& refSelOption)
{
	SRect rect(prc->left,prc->top,prc->Width(),prc->Height());
	return xbschdocsch.select(rect,refSelOption);
}

//点指定での選択
//SetSel(CRect*)と違って、「重なっている度合」が最も大きいもの一つだけを選択する
int CBSchDoc::SetSel(CPoint* pp,SelectOption& refSelOption)
{
	SPoint point(pp->x,pp->y);
	return xbschdocsch.select(point,refSelOption);
}

//矩形範囲でのドラッグモード選択
int CBSchDoc::SetDrag(CRect* prc,bool bCutOffPinWire)
{
	SRect rect(prc->left,prc->top,prc->Width(),prc->Height());
	return xbschdocsch.selectForDrag(rect,bCutOffPinWire);
}


//点指定でのドラッグモード選択
int CBSchDoc::SetDrag(CPoint* pp,bool bCutOffPinWire)
{
	CRect rc;
	rc.left  =pp->x-2;
	rc.top   =pp->y-2;
	rc.right =pp->x+2;
	rc.bottom=pp->y+2;
	
	return SetDrag(&rc,bCutOffPinWire);
}
	

//一時データのリストを得る
XBSchObjList* CBSchDoc::GetTmpList()
{
	return &xbschdocsch.m_listTemp;
}

//一時データが単一図面オブジェクトのとき、それを返す
SXBSchObj* CBSchDoc::GetTmp()
{
	if(GetTmpDataCount()==1){
		XBSchObjListIterator pObjIte = xbschdocsch.m_listTemp.begin();
		return *pObjIte;
	}else{ 
		return NULL;
	}
}

//指定した点が一時データの上かどうか
//ドラッグモードでない単一データの場合は、図面オブジェクトのInAreaPoint()を使う
int CBSchDoc::IsOnTmpData(CPoint* pp,int nOption,SXBSchObj** ppObj,int* pRetOption)
{
	return xbschdocsch.isOnTempData(SPoint(pp->x,pp->y),nOption,ppObj,pRetOption);
}

//ペースト
//次の手順でクリップボードのデータを得る
// 1.クリップボードからメモリブロックを得る
// 2.そのメモリブロックで CMemFileを初期化する
// 3.ReadCE2を実行する。
void CBSchDoc::Paste(CPoint* pOrigin)
{
	SPoint pt(pOrigin->x,pOrigin->y);
	xbschdocsch.paste(pt);
	// >>> 2005/01/30 ペーストデータがフィックスされたときにSetModifiedFlag()するように変更
    //		SetModifiedFlag(TRUE);
	//		xbschdocsch.setDirty(true);
	// <<< 2005/01/30
}

//コピー
void CBSchDoc::Copy()
{
	xbschdocsch.copy();
	//SetModifiedFlag(TRUE);
}

//カット
void CBSchDoc::Cut()
{
	Copy();
	Clear();
}


//クリア
void CBSchDoc::Clear()
{
	if(xbschdocsch.del()){
		SetModifiedFlag(TRUE);
	}
	// >>> 2005/01/30
	//     ダーティフラグの設定は xbschdocsch.del()の中で行うように変更
	//	xbschdocsch.setDirty(true);
	// <<< 2005/01/30
}

//アンドゥ
void CBSchDoc::Undo()
{
	xbschdocsch.undo();
}

bool CBSchDoc::Redo()
{
	return xbschdocsch.redo();
}


//回転する
//一時データが単一データで、かつ、そのEditFlagのENABLE_ROTATEが有効であるとき
void CBSchDoc::Rotate()
{
	xbschdocsch.rotate();
	// >>> 2005/01/30 
	//	データがフィックスされたときにSetModifiedFlag()するように変更
	//  ダーティフラグ設定はxbschdocsch.rotate()で行う
	//SetModifiedFlag(TRUE);
	//xbschdocsch.setDirty(true);
	// <<< 2005/01/30
}


//左右反転する
//一時データが単一データで、かつ、そのEditFlagのENABLE_MIRRORが有効であるとき
void CBSchDoc::Mirror()
{
	xbschdocsch.mirror();
	// >>> 2005/01/30 
	//	データがフィックスされたときにSetModifiedFlag()するように変更
	//  ダーティフラグ設定はxbschdocsch.mirror()で行う
	//SetModifiedFlag(TRUE);
	//xbschdocsch.setDirty(true);
	// <<< 2005/01/30
}

//移動する
//引数 CPoint*版
//一時データを移動する
//移動後、一時データのダーティフラグをセットする。
void CBSchDoc::Shift(CPoint* pOrigin)
{
	xbschdocsch.moveTempData(pOrigin->x,pOrigin->y);
	// >>> 2005/01/30 
	//	データがフィックスされたときにSetModifiedFlag()するように変更
	//  ダーティフラグ設定はxbschdocsch.moveTempData()で行う
	//SetModifiedFlag(TRUE);
	//xbschdocsch.setDirty(true);
	// <<< 2005/01/30
}

//移動する
//引数 Csize*版
void CBSchDoc::Shift(CSize*  psizeOffset)
{
	xbschdocsch.moveTempData(psizeOffset->cx,psizeOffset->cy);
	// >>> 2005/01/30 
	//	データがフィックスされたときにSetModifiedFlag()するように変更
	//  ダーティフラグ設定はxbschdocsch.moveTempData()で行う
	//SetModifiedFlag(TRUE);
	//xbschdocsch.setDirty(true);
	// <<< 2005/01/30
}



//ダーティフラグをセットする
void CBSchDoc::SetDirty()
{
	SetModifiedFlag(TRUE);
	xbschdocsch.setDirty(true);
}

//一時データのダーティフラグをセットする
void CBSchDoc::SetTempDirty()
{
	// >>> 2005/01/30
	//	データがフィックスされたときにSetModifiedFlag()するように変更
	//	SetModifiedFlag(TRUE);
	// <<< 2005/01/30
	xbschdocsch.setTempDirty(true);
}


//データが空かどうかを返す
BOOL CBSchDoc::IsEmpty()
{
	return xbschdocsch.isEmpty();
}

//選択されたものが存在するかどうか
//m_fTmpDataのうち、TMPDATA_NEWまたはTMPDATA_MOVEDのビットが立っていて、かつ
//                  TMPDATA_FOR_DRAGのビットがOFF
BOOL CBSchDoc::CanEdit()
{
	return xbschdocsch.canCopy();
}

//ペースト可能かどうか
//BSch独自フォーマットのデータがクリップボードに存在すれば可能とする。
BOOL CBSchDoc::CanPaste()
{
	return xbschdocsch.canPaste();
}

//アンドゥ可能かどうか
//m_nDataDepthが 1 以上なら可能。
//m_nDataDepthが 0 でも一時データが有効なら可能。
BOOL CBSchDoc::CanUndo()
{
	return xbschdocsch.canUndo();
}

BOOL CBSchDoc::CanRedo()
{
	return xbschdocsch.canRedo();
}


//アトリビュートの変更が可能かどうか
//一時データが単一データで、かつ、そのEditFlagのENABLE_ATTRIBUTEが有効であるとき
BOOL CBSchDoc::CanAttribute()
{
	return xbschdocsch.canAttribute();
}

//回転が可能かどうか
//一時データが単一データで、かつ、そのEditFlagのENABLE_ROTATEが有効であるとき
BOOL CBSchDoc::CanRotate()
{
	return xbschdocsch.canRotate();
}

//左右反転が可能かどうか
//一時データが単一データで、かつ、そのEditFlagのENABLE_MIRRORが有効であるとき
BOOL CBSchDoc::CanMirror()
{
	return xbschdocsch.canMirror();
}

//点の編集が可能か
BOOL CBSchDoc::CanEditPoints()
{
	return xbschdocsch.canEditPoints();
}

//順序の変更が可能か？
BOOL CBSchDoc::CanChangeOrder()
{
	return xbschdocsch.canChangeOrder();
}


//「次の検索」が可能かどうか
BOOL CBSchDoc::CanFindNext()
{
	return xbschdocsch.canFindNext();
}

const SXBSchObj* CBSchDoc::FindStart(const TCHAR* sz,bool bMatchWhole,bool bCaseSensitive,unsigned int targetFlags,bool selectWhenFound)
{
	return xbschdocsch.findStart(sz,bMatchWhole,bCaseSensitive,targetFlags,selectWhenFound);
}

//部品が1つだけ単独で選択されているか
const SXBSchComponent* CBSchDoc::isComponentSelectedSingly()
{
	return xbschdocsch.isComponentSelectedSingly();
}

//カーソルが部品の上にあるかどうか
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


//図面オブジェクトの仮追加
BOOL CBSchDoc::AddData(SXBSchObj* pObj)
{
	xbschdocsch.addSelect(pObj);
	// >>> 2005/01/30
	//	ペーストデータがフィックスされたときにSetModifiedFlag()するように変更
	//	SetModifiedFlag(TRUE);
	// <<< 2005/01/30
	//xbschdocsch.setDirty(true);
	return TRUE;
}

//先頭からn個目のデータのリストへのポインタを得る
//nが有効なメインデータの数を越えていたらNULLを返す
XBSchObjList* CBSchDoc::GetMainData(int /* =0*/)
{
	return &xbschdocsch.m_listMain;
}

//図面サイズを番号でセットする
void CBSchDoc::SetSheetSize(int n, int w, int h)
{
	if(w < SHEETSIZE_X_MIN) w=SHEETSIZE_X_MIN;
	else if(w > SHEETSIZE_X_MAX) w=SHEETSIZE_X_MAX;
	if(h < SHEETSIZE_Y_MIN) h=SHEETSIZE_Y_MIN;
	else if(h > SHEETSIZE_Y_MAX) h=SHEETSIZE_Y_MAX;

	
	if(n<0 || n>=MAX_SHEETSIZE){//範囲外なら0にする
		//TRACE("\nRange error (CBSchDoc::SetSheetSize)");//DEBUGモードでメッセージ
		xbschdocsch.SetSheetSize(SSize(w,h));
	}else{
		xbschdocsch.SetSheetSize(SSize(m_tablesizeSheet[n].cx,m_tablesizeSheet[n].cy));
	}
	SetModifiedFlag(TRUE);
	//xbschdocsch.setDirty(true);
}


//図面サイズを返す
CSize CBSchDoc::GetSheetSize()
{
	SSize sz=xbschdocsch.SheetSize();
	return CSize(sz.w(),sz.h());
}

//図面サイズを返す
int	CBSchDoc::GetSheetSizeIndex()
{
	SSize sz=xbschdocsch.SheetSize();
	for(int i=0;i<MAX_SHEETSIZE;i++){
		SIZE ssz = m_tablesizeSheet[i];
		if(sz.w() == ssz.cx && sz.h() == ssz.cy) return i;
	}
	return -1;
}


//データの全消去
//アプリケーションフレームワークから次の場合に呼び出される
//			新規作成
//			ファイル読み込み
//			プログラムの終了
//メインデータのリストに含まれるすべてのデータをDelete。
//m_nTopDataとm_nDataDepthをリセット。
void CBSchDoc::DeleteContents() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	xbschdocsch.init();
	SetModifiedFlag(FALSE);
	//CDocument::DeleteContents();	//基本クラスは何もしない、そうだ。
}


//レイヤーの移動
void CBSchDoc::MoveLayer(int nLayer)
{
	xbschdocsch.moveToLayer(nLayer);
}



BOOL CBSchDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
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
	
	// TODO: この位置に固有の作成用コードを追加してください
	
	//return TRUE;
}

BOOL CBSchDoc::IsModified()
{
	return xbschdocsch.isDirty();
}


void CBSchDoc::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU)
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
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
