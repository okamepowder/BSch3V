/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// PartAtrb.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <assert.h>
#include <string>
#include <list>
using namespace std;
#include "BSch.h"

//#include "PartLib.h"
#include "PartAtrb.h"
#include "complib.h"
#include "Preview.h"
#include "SelPart.h"

#include "xbschglobal.h"

#include <stdlib.h>  //_itoa()のためにインクルード
#include ".\partatrb.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPartAtrb ダイアログ


CPartAtrb::CPartAtrb(CWnd* pParent /*=NULL*/)
	: CDialog(CPartAtrb::IDD, pParent)
	, m_bHideNum(FALSE)
	, m_bHideVal(FALSE)
{
	//{{AFX_DATA_INIT(CPartAtrb)
	m_rPartName = _T("");
	m_rPartNum = _T("");
	m_rNote = _T("");
	//}}AFX_DATA_INIT
	m_nBlock   =1;
	m_nBlockMin=1;
	m_nBlockMax=1;
//	m_pPartIndexReplace = NULL;
	m_pPartIndex = NULL;
	m_enableEditComp = FALSE;
	m_modifiedCompInfo =FALSE;
	m_bNameVertical=FALSE;
	m_bNumVertical=FALSE;
	m_mfr = _T("");
	m_mfrpnum = _T("");
	m_package = _T("");
	m_nFocusedTo = FOCUSED_TO_BODY;
}


void CPartAtrb::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_EDIT_PARTBLOCK, m_nBlock);
	//DDV_MinMaxUInt(pDX, m_nBlock, 0, 100);
	//{{AFX_DATA_MAP(CPartAtrb)
	DDX_Text(pDX, IDC_EDIT_PARTNAME, m_rPartName);
	DDX_Text(pDX, IDC_EDIT_PARTNUM, m_rPartNum);
	DDX_Text(pDX, IDC_EDIT_NOTE, m_rNote);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK_NAMEVERT, m_bNameVertical);
	DDX_Check(pDX, IDC_CHECK_NUMVERT, m_bNumVertical);
	DDX_Text(pDX, IDC_EDIT_MFR, m_mfr);
	DDX_Text(pDX, IDC_EDIT_MFR_PNUM, m_mfrpnum);
	DDX_Text(pDX, IDC_EDIT_PKG, m_package);
	DDX_Check(pDX, IDC_CHECK_HIDENUM, m_bHideNum);
	DDX_Check(pDX, IDC_CHECK_HIDEVAL, m_bHideVal);
	DDV_MaxChars(pDX, m_rNote, 1023);
}


BEGIN_MESSAGE_MAP(CPartAtrb, CDialog)
	//{{AFX_MSG_MAP(CPartAtrb)
	ON_EN_CHANGE(IDC_EDIT_PARTBLOCK, OnChangeEditPartblock)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_REPLACE_COMPINDEX, OnBnClickedReplaceCompindex)
	ON_BN_CLICKED(IDC_EDIT_COMPINDEX, OnBnClickedEditCompindex)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPartAtrb メッセージ ハンドラ

BOOL CPartAtrb::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	SetDlgItemText(IDC_ORGNAME,m_rOrgName);
	if(m_pPartIndex->m_pCompInfo){
		SetDlgItemText(IDC_EDIT_LIB_NOTE,m_pPartIndex->m_pCompInfo->m_note.c_str());
	}

	m_nBlockMax=m_pPartIndex->block();

	//-------2014/05/17--------------------------
	CString blockStr;
	switch(m_pPartIndex->m_pCompInfo->m_ciType){
		case SCompInfo::CITYPE::FIXED_BLOCK:
			m_nBlock = 1;
			blockStr.LoadString(IDS_FIXED_BLOCK);
			SetDlgItemInt(IDC_EDIT_PARTBLOCK,m_pPartIndex->m_pCompInfo->m_nFixedBlock+1);			
			break;
		case SCompInfo::CITYPE::SHARED_BLOCK:
			m_nBlock = 1;
			blockStr.LoadString(IDS_SHARED_BLOCK);
			SetDlgItemInt(IDC_EDIT_PARTBLOCK,1);			
		    break;
		default:
			blockStr.LoadString(IDS_BLOCK);
	}
	SetDlgItemText(IDC_STATIC_BLOCK_STATUS,blockStr);
	if(m_pPartIndex->m_pCompInfo->m_ciType == SCompInfo::CITYPE::GENERAL){
		GetDlgItem(IDC_EDIT_PARTBLOCK)->EnableWindow(TRUE);
		CSpinButtonCtrl* pSpin=(CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_PARTBLOCK);
		pSpin->ShowWindow(SW_SHOW);
		pSpin->SetRange(m_nBlockMin,m_nBlockMax);
		if(m_nBlock>m_nBlockMax){
			m_nBlock=m_nBlockMax;
		}
		pSpin->SetPos(m_nBlock);
	}else{
		GetDlgItem(IDC_SPIN_PARTBLOCK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_PARTBLOCK)->EnableWindow(FALSE);
	}


	//-------------------------------------------
	


	SetBlockPins(m_nBlock);
	CEdit* pEdit=(CEdit*)GetDlgItem(IDC_EDIT_PARTBLOCK);
	pEdit->LimitText(2);

	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];

	::GetModuleFileName(NULL,m_szLcoPath,_MAX_PATH);
	::_tsplitpath(m_szLcoPath,drive,dir,NULL,NULL);
	::_tmakepath(m_szLcoPath,drive,dir,_T("LCoV"),_T(".exe"));

	FILE* fp;

	fp = _tfopen(m_szLcoPath,_T("r"));
	m_enableEditComp = (fp != NULL);
	if(fp != NULL) fclose(fp);
	CButton* pButton = (CButton*)GetDlgItem(IDC_EDIT_COMPINDEX);
	pButton->EnableWindow(m_enableEditComp);

	CEdit* pEditFocused = NULL;
	if(m_nFocusedTo == FOCUSED_TO_VAL){
		pEditFocused = (CEdit*)GetDlgItem(IDC_EDIT_PARTNAME);
	}else{
		pEditFocused = (CEdit*)GetDlgItem(IDC_EDIT_PARTNUM);
	}
	if(pEditFocused){
		pEditFocused->SetFocus();
		pEditFocused->SetSel(0,-1);
		return FALSE;
	}else{
		return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
	}
}

void CPartAtrb::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	CString rStr;
	CEdit* pEdit=(CEdit*)GetDlgItem(IDC_EDIT_PARTBLOCK);
	pEdit->GetWindowText(rStr);
	m_nBlock=_tstoi(rStr);
	if(m_nBlock<m_nBlockMin) m_nBlock=m_nBlockMin;
	else if(m_nBlock>m_nBlockMax) m_nBlock=m_nBlockMax;

	SCompInfo* pInfo = m_pPartIndex->compInfo();
	CString str;
	GetDlgItemText(IDC_EDIT_MFR,str);
	pInfo->m_mfr = str;
	GetDlgItemText(IDC_EDIT_MFR_PNUM,str);
	pInfo->m_mfrpn = str;
	GetDlgItemText(IDC_EDIT_PKG,str);
	pInfo->m_pkg = str;

	CDialog::OnOK();
}

//使用ブロックを変更したとき、ピン番号を表示するように変更 1997/01/21
//この機能を OnVScroll応答からエディットコントロール変更応答に変更 1997/01/22
//void CPartAtrb::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
//{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	// このダイアログには WM_VSCROLL を発生するアイテムが一つしかないので
	// サボリのコードになっている。WM_VSCROLL を発生するアイテムを追加したら
	// ちゃんと書く
//	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);

//	if(nSBCode==SB_THUMBPOSITION){
//		SetBlockPins(nPos);
//	}
//}

//IDC_PIN_NUM スタティックコントロールにピン番号をセット
void CPartAtrb::SetBlockPins(int nBlock)
{
	CString rString;		//空の文字列
	//char szPinNum[32];			//ここに順次ピン番号を格納して rString に追加
//	char* pc;				//一時的に使う文字ポインタ
//	DWORD dwPinNumInfo;		//m_pPartIndexから得られる生のピン番号情報
	int n,nMaxPin;
	//WORD wNumPfix;
	const SPin* ppininfo;
	
	if(nBlock>=m_nBlockMin && nBlock<=m_nBlockMax){
		nMaxPin=m_pPartIndex->pinCount();
		if(nMaxPin>10) nMaxPin=10;	//たくさん表示しても仕方がない、と思う
		for(n=0;n<nMaxPin;n++){
			ppininfo = m_pPartIndex->pin(n);
			//dwPinNumInfo=m_pPartIndex->GetPinNum(n,nBlock-1);//「nBlock-1」が実際のブロック番号
			//wNumPfix=HIWORD(dwPinNumInfo);					 //ピン番号の上位ワード
			//pc=szPinNum;
			if(n>0) rString+=",";	//２つめ以降は先頭に','をつける
			rString+=ppininfo->pinNum(nBlock-1);
		}
	}
	CStatic* pST=(CStatic*)GetDlgItem(IDC_PIN_NUM);
	if(pST!=NULL)	pST->SetWindowText(rString);
}


//使用ブロックを変更したとき、ピン番号を表示する 1997/01/22
//    （この機能を OnVScroll応答からこれに変更した）
//・スピンコントロールを操作した場合
//   (1)スピンコントロールが自動でバディのIDC_EDIT_PARTBLOCKを変更
//   (2)EN_CHANGEを発生
//   (3)SetBlockPins()メンバー関数がIDC_PIN_NUMにピン番号をセット
//・直接 IDC_EDIT_PARTBLOCK を変更した場合
//   (1)ユーザーがIDC_EDIT_PARTBLOCKを変更
//   (2)EN_CHANGEを発生
//   (3)SetBlockPins()メンバー関数がIDC_PIN_NUMにピン番号をセット
void CPartAtrb::OnChangeEditPartblock() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CString rStr;
	CEdit* pEdit=(CEdit*)GetDlgItem(IDC_EDIT_PARTBLOCK);
	pEdit->GetWindowText(rStr);
	int nBlock=_tstoi(rStr);
	SetBlockPins(nBlock);
}

void CPartAtrb::OnBnClickedReplaceCompindex()
{
	CSelPart dlg;
	dlg.m_bUsePreview=TRUE;		//プレビューを使うかどうかの設定
	if(g_LibraryInfo(m_nLastSelectLibrary)){//NULLでなければ...
		dlg.m_nLastSelectLibrary=m_nLastSelectLibrary;
		dlg.m_nLastSelectPart=m_nLastSelectPart;
	}else{
		dlg.m_nLastSelectLibrary=0;
		dlg.m_nLastSelectPart=0;
	}
	if(dlg.DoModal()==IDCANCEL){
		return;
	}

	const SCompIndex* pPartIndex = dlg.m_pCompIndex;
	if(pPartIndex==NULL){
		CString rString;
		//メッセージボックス用のメッセージの作成
		AfxFormatString1(rString,IDS_NOT_FOUND_PART,dlg.m_rStrPartName); 
		AfxMessageBox(rString,MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	m_nLastSelectPart=    dlg.m_nLastSelectPart;
	m_nLastSelectLibrary= dlg.m_nLastSelectLibrary;
	m_pPartIndex = pPartIndex;
	m_rOrgName = pPartIndex->name();
	m_nBlockMax=m_pPartIndex->block();

	//-------2014/05/17--------------------------
	CString blockStr;
	switch(m_pPartIndex->m_pCompInfo->m_ciType){
		case SCompInfo::CITYPE::FIXED_BLOCK:
			m_nBlock = 1;
			blockStr.LoadString(IDS_FIXED_BLOCK);
			SetDlgItemInt(IDC_EDIT_PARTBLOCK,m_pPartIndex->m_pCompInfo->m_nFixedBlock+1);			
			break;
		case SCompInfo::CITYPE::SHARED_BLOCK:
			m_nBlock = 1;
			blockStr.LoadString(IDS_SHARED_BLOCK);
			SetDlgItemInt(IDC_EDIT_PARTBLOCK,1);			
		    break;
		default:
			blockStr.LoadString(IDS_BLOCK);
	}
	SetDlgItemText(IDC_STATIC_BLOCK_STATUS,blockStr);
	if(m_pPartIndex->m_pCompInfo->m_ciType == SCompInfo::CITYPE::GENERAL){
		GetDlgItem(IDC_EDIT_PARTBLOCK)->EnableWindow(TRUE);
		CSpinButtonCtrl* pSpin=(CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_PARTBLOCK);
		pSpin->ShowWindow(SW_SHOW);
		pSpin->SetRange(m_nBlockMin,m_nBlockMax);
		if(m_nBlock>m_nBlockMax){
			m_nBlock=m_nBlockMax;
		}
		pSpin->SetPos(m_nBlock);
	}else{
		GetDlgItem(IDC_SPIN_PARTBLOCK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_PARTBLOCK)->EnableWindow(FALSE);
	}

//-------------------------------------------


	SetBlockPins(m_nBlock);

//	CStatic* pST=(CStatic*)GetDlgItem(IDC_ORGNAME);
//	pST->SetWindowText(m_rOrgName);
	SetDlgItemText(IDC_ORGNAME,m_rOrgName);
	if(m_pPartIndex->m_pCompInfo){
		SetDlgItemText(IDC_EDIT_LIB_NOTE,m_pPartIndex->m_pCompInfo->m_note.c_str());
		const TCHAR* pc;

		pc = m_pPartIndex->Mfr();
		if(pc!=NULL && *pc != 0){	SetDlgItemText(IDC_EDIT_MFR,pc);	}

		pc = m_pPartIndex->MfrPn();
		if(pc!=NULL && *pc != 0){	SetDlgItemText(IDC_EDIT_MFR_PNUM,pc);	}

		pc = m_pPartIndex->Pkg();
		if(pc!=NULL && *pc != 0){	SetDlgItemText(IDC_EDIT_PKG,pc);	}
	}
}





void CPartAtrb::OnBnClickedEditCompindex()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。

	if(!m_enableEditComp)return;

	SCompInfo* pInfo = m_pPartIndex->compInfo();

	CString str;
	GetDlgItemText(IDC_EDIT_MFR,str);
	pInfo->m_mfr = str;
	GetDlgItemText(IDC_EDIT_MFR_PNUM,str);
	pInfo->m_mfrpn = str;
	GetDlgItemText(IDC_EDIT_PKG,str);
	pInfo->m_pkg = str;

	CBSchApp::editWidthLCoV(m_rOrgName, pInfo, m_tempLib);

	//TCHAR dir[_MAX_PATH];
	//TCHAR path[_MAX_PATH];
	//TCHAR cmdline[_MAX_PATH*2+10];

	//::GetTempPath(_MAX_PATH,dir);
	//if(::GetTempFileName(dir,_T("b3v"),0,path)==0) return;

	//FILE* fp;
	//if((fp = _tfopen(path,_T("wt, ccs=UTF-8")))==NULL) return;
	//fseek( fp, 0, SEEK_SET ) ; //BAD KNOWHOW for removing BOM CODE.

	//SCompInfo* pInfo = m_pPartIndex->compInfo();

	//CString str;
	//GetDlgItemText(IDC_EDIT_MFR,str);
	//pInfo->m_mfr = str;
	//GetDlgItemText(IDC_EDIT_MFR_PNUM,str);
	//pInfo->m_mfrpn = str;
	//GetDlgItemText(IDC_EDIT_PKG,str);
	//pInfo->m_pkg = str;


	//SWriteCE3 wce3;
	//wce3.Attach(fp);
	//wce3.WriteRecord(_T("+BSCH3_LIB_V.1.0"));	wce3.WriteEOL();
	//wce3.WriteRecordInt(_T("VER"),SCompLib::m_nConstDataVersion);	wce3.WriteEOL();

	////SCompInfo* pInfo = m_pPartIndex->compInfo();
	//pInfo->writeCe3(wce3,m_rOrgName);
	//wce3.WriteRecord(_T("-BSCH3_LIB_V.1.0"));
	//wce3.WriteEOL();
	//fclose(fp);

	//_stprintf(cmdline,_T("%s \"%s\""),m_szLcoPath,path);

	//PROCESS_INFORMATION pi;
	//STARTUPINFO si;

 //   ZeroMemory(&pi, sizeof(pi));
	//ZeroMemory(&si, sizeof(si));
 //   si.cb = sizeof(si);

 //   BOOL resProc = CreateProcess(NULL, cmdline, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	//if(!resProc) return;
	//
	//while(WAIT_TIMEOUT==WaitForSingleObject(pi.hProcess,1)){
	//	MSG msg;
	//	if(GetMessage(&msg,NULL,0,0)){
	//		if(msg.message == WM_PAINT){				//	メッセージが WM_PAINT なら
	//			TranslateMessage(&msg);
	//			DispatchMessage(&msg);
	//		}
	//	}
	//}

	//TRACE("End LCoV\n");
	//CloseHandle(pi.hProcess);
 //   CloseHandle(pi.hThread);

	//tempLib.deleteContent();
	//tempLib.readLibraryFile(path);
	//DeleteFile(path);	
	
	if(m_tempLib.count()<1) return;

	SCompIndex* pPartIndex = m_tempLib.compIndex(0);
	if(pPartIndex==NULL) return;
	
	m_pPartIndex = pPartIndex;



	m_rOrgName = pPartIndex->name();

	m_nBlockMax=m_pPartIndex->block();


//-------2014/05/17--------------------------
	CString blockStr;
	switch(m_pPartIndex->m_pCompInfo->m_ciType){
		case SCompInfo::CITYPE::FIXED_BLOCK:
			m_nBlock = 1;
			blockStr.LoadString(IDS_FIXED_BLOCK);
			SetDlgItemInt(IDC_EDIT_PARTBLOCK,m_pPartIndex->m_pCompInfo->m_nFixedBlock+1);			
			break;
		case SCompInfo::CITYPE::SHARED_BLOCK:
			m_nBlock = 1;
			blockStr.LoadString(IDS_SHARED_BLOCK);
			SetDlgItemInt(IDC_EDIT_PARTBLOCK,1);			
		    break;
		default:
			blockStr.LoadString(IDS_BLOCK);
	}
	SetDlgItemText(IDC_STATIC_BLOCK_STATUS,blockStr);
	if(m_pPartIndex->m_pCompInfo->m_ciType == SCompInfo::CITYPE::GENERAL){
		GetDlgItem(IDC_EDIT_PARTBLOCK)->EnableWindow(TRUE);
		CSpinButtonCtrl* pSpin=(CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_PARTBLOCK);
		pSpin->ShowWindow(SW_SHOW);
		pSpin->SetRange(m_nBlockMin,m_nBlockMax);
		if(m_nBlock>m_nBlockMax){
			m_nBlock=m_nBlockMax;
		}
		pSpin->SetPos(m_nBlock);
	}else{
		GetDlgItem(IDC_SPIN_PARTBLOCK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_PARTBLOCK)->EnableWindow(FALSE);
	}

//-------------------------------------------

	SetBlockPins(m_nBlock);

//	CStatic* pST=(CStatic*)GetDlgItem(IDC_ORGNAME);
//	pST->SetWindowText(m_rOrgName);
	SetDlgItemText(IDC_ORGNAME,m_rOrgName);
	if(m_pPartIndex->m_pCompInfo){
		SetDlgItemText(IDC_EDIT_LIB_NOTE,m_pPartIndex->m_pCompInfo->m_note.c_str());

		SetDlgItemText(IDC_EDIT_MFR,m_pPartIndex->Mfr());
		SetDlgItemText(IDC_EDIT_MFR_PNUM,m_pPartIndex->MfrPn());
		SetDlgItemText(IDC_EDIT_PKG,m_pPartIndex->Pkg());
	}

	m_modifiedCompInfo = TRUE;
	
}
