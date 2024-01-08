/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/


// MainFrm.cpp : CMainFrame クラスの実装
//

#include "stdafx.h"
#include "LCoV.h"

#include "MainFrm.h"

#include "coord.h"
#include "ce3io.h"

#include "bschfont.h"
#include ".\mainfrm.h"
#include "CompProperty.h"
#include "PropLib.h"
#include "AddAlias.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	ON_LBN_SELCHANGE(IDC_LIST_NAME, OnSelchangeNameList)
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_UP, OnUpdateButtonUp)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_DOWN, OnUpdateButtonDown)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_EDIT_COPY_COMP, OnEditCopyComp)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY_COMP, OnUpdateEditCopyComp)
	ON_COMMAND(ID_EDIT_CUT_COMP, OnEditCutComp)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT_COMP, OnUpdateEditCutComp)
	ON_COMMAND(ID_EDIT_PASTE_COMP, OnEditPasteComp)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_COMP, OnUpdateEditPasteComp)
//	ON_COMMAND(ID_VIEW_TOOLBAR, OnViewToolbar)
ON_COMMAND(ID_TOOL_SELECT, OnToolSelect)
ON_UPDATE_COMMAND_UI(ID_TOOL_SELECT, OnUpdateToolSelect)
ON_COMMAND(ID_TOOL_PIN, OnToolPin)
ON_UPDATE_COMMAND_UI(ID_TOOL_PIN, OnUpdateToolPin)
ON_COMMAND(ID_TOOL_LINE, OnToolLine)
ON_UPDATE_COMMAND_UI(ID_TOOL_LINE, OnUpdateToolLine)
ON_COMMAND(ID_TOOL_CIRCLE, OnToolCircle)
ON_UPDATE_COMMAND_UI(ID_TOOL_CIRCLE, OnUpdateToolCircle)
ON_COMMAND(ID_TOOL_POLYGON, OnToolPolygon)
ON_UPDATE_COMMAND_UI(ID_TOOL_POLYGON, OnUpdateToolPolygon)
ON_COMMAND(ID_TOOL_ARC, OnToolArc)
ON_UPDATE_COMMAND_UI(ID_TOOL_ARC, OnUpdateToolArc)
ON_COMMAND(ID_EDIT_PROPERTY, OnEditProperty)
ON_UPDATE_COMMAND_UI(ID_EDIT_PROPERTY, OnUpdateEditProperty)
ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
ON_COMMAND(ID_EDIT_CUT, OnEditCut)
ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
ON_COMMAND(ID_EDIT_NEW_COMP, OnEditNewComp)
ON_COMMAND(ID_EDIT_PROPERTY_COMP, OnEditPropertyComp)
ON_UPDATE_COMMAND_UI(ID_EDIT_PROPERTY_COMP, OnUpdateEditPropertyComp)
ON_UPDATE_COMMAND_UI(ID_EDIT_MIRROR, OnUpdateEditMirror)
ON_UPDATE_COMMAND_UI(ID_EDIT_ROTATE, OnUpdateEditRotate)
ON_COMMAND(ID_EDIT_ROTATE, OnEditRotate)
ON_COMMAND(ID_EDIT_MIRROR, OnEditMirror)
ON_COMMAND(ID_EDIT_NODE_MODE, OnEditNodeMode)
ON_UPDATE_COMMAND_UI(ID_EDIT_NODE_MODE, OnUpdateEditNodeMode)
ON_COMMAND(ID_TOOL_CREATE_BPTN, OnToolCreateBptn)
ON_UPDATE_COMMAND_UI(ID_TOOL_CREATE_BPTN, OnUpdateToolCreateBptn)
ON_WM_CLOSE()
ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditClear)
ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
ON_COMMAND(ID_EDIT_PROP_LIB, OnEditPropLib)
ON_COMMAND(ID_EDIT_ADD_ALIAS, OnEditAddAlias)
ON_UPDATE_COMMAND_UI(ID_EDIT_ADD_ALIAS, OnUpdateEditAddAlias)
ON_COMMAND(ID_EDIT_SORT_COMPONENT, OnEditSortComponent)
ON_UPDATE_COMMAND_UI(ID_EDIT_SORT_COMPONENT, OnUpdateEditSortComponent)
ON_COMMAND(ID_TOOL_TEXT, OnToolText)
ON_UPDATE_COMMAND_UI(ID_TOOL_TEXT, OnUpdateToolText)
ON_COMMAND(ID_EDIT_NEXTPINNAME, OnEditNextPinName)
ON_UPDATE_COMMAND_UI(ID_EDIT_NEXTPINNAME, OnUpdateEditNextPinName)
ON_COMMAND(ID_EDIT_NEXTPINNUM, OnEditNextPinNum)
ON_UPDATE_COMMAND_UI(ID_EDIT_NEXTPINNUM, OnUpdateNextPinNum)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ステータス ライン インジケータ
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame コンストラクション/デストラクション

CMainFrame::CMainFrame()
{
	// TODO: メンバ初期化コードをここに追加してください。
	m_OldSelectedCompInfo = NULL;
}

CMainFrame::~CMainFrame()
{
	delete g_pFont;
}


void CMainFrame::setAppCaption(LPCTSTR filename)
{
	CString title = _T("LCoV");
	if(filename!=NULL && filename[0]!=0){
		title += _T(" - ");
		title += filename;
	}
	SetWindowText(title);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// フレームのクライアント領域全体を占めるビューを作成します。
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("ビュー ウィンドウを作成できませんでした。\n");
		return -1;
	}

	//SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	//SetIcon(IDR_MAINFRAME,TRUE);

	HICON hAppIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(hAppIcon, TRUE);		// 大きいアイコンを設定

	if (!m_wndToolBar.CreateEx(this, 0/*TBSTYLE_FLAT*/, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("ツール バーの作成に失敗しました。\n");
		return -1;      // 作成できませんでした。
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("ステータス バーの作成に失敗しました。\n");
		return -1;      // 作成できませんでした。
	}
	// TODO: ツール バーをドッキング可能にしない場合は、これらの 3 行を削除してください。
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);


	if (!m_wndDlgBar.Create(this, IDD_DIALOGBAR,CBRS_LEFT, IDD_DIALOGBAR)){
		TRACE0("Failed to create DlgBar\n");
		return -1;      // Fail to create.
	}
	CRect rcLB(0,0,10,10);

	m_LBCompName.CreateEx(WS_EX_CLIENTEDGE,_T("LISTBOX"),_T("Name&List"),
					  WS_CHILD|WS_TABSTOP|WS_VISIBLE|WS_VSCROLL|LBS_NOTIFY,rcLB,&m_wndDlgBar,IDC_LIST_NAME);

	CButton* pButton;
	pButton = (CButton*)m_wndDlgBar.GetDlgItem(IDC_BUTTON_UP);
	HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_UP);
	pButton->SetIcon(hIcon);
	pButton = (CButton*)m_wndDlgBar.GetDlgItem(IDC_BUTTON_DOWN);
	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_DOWN);
	pButton->SetIcon(hIcon);
	
	g_pFont = new CBSchFont;

	bool nocheckPntRect = AfxGetApp()->GetProfileInt(_T("Setting"),_T("NoCheckPtnRectOnSave"),50);
	m_wndView.m_doc.m_noChechAreaOnSave = nocheckPntRect;
	AfxGetApp()->WriteProfileInt(_T("Setting"),_T("NoCheckPtnRectOnSave"),nocheckPntRect);

	int pageMargin = AfxGetApp()->GetProfileInt(_T("Setting"),_T("EditWindowMargin"),0);
	if(pageMargin>0){	//実際に有効な範囲はsetPageMargin()内で決められる。
		m_wndView.setPageMargin(pageMargin,true);
	}
	AfxGetApp()->WriteProfileInt(_T("Setting"),_T("EditWindowMargin"),pageMargin);

	m_wndView.setCompIndex(NULL);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);

	cs.cx = m_sizeInitialWindowSize.cx;
	cs.cy = m_sizeInitialWindowSize.cy;
	return TRUE;
}


// CMainFrame 診断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame メッセージ ハンドラ

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// ビュー ウィンドウにフォーカスを与えます。
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// ビューに最初にコマンドを処理する機会を与えます。
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// それ以外の場合は、既定の処理を行います。
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);
	// TODO : ここにメッセージ ハンドラ コードを追加します。


	//>>>フレームウィンドウサイズの復元 BSch3Vから移植 2008/02/09
	WINDOWPLACEMENT wp;
	this->GetWindowPlacement(&wp);
	int wpcx = wp.rcNormalPosition.right - wp.rcNormalPosition.left;
	int wpcy = wp.rcNormalPosition.bottom- wp.rcNormalPosition.top;
	IniWriteWindowPosition(wp.showCmd,wpcx,wpcy);
	//<<<フレームウィンドウサイズの復元 BSch3Vから移植 2008/02/09

	CRect rcDlgBar;
	m_wndDlgBar.GetWindowRect(&rcDlgBar);
	CButton* pButton = (CButton*)m_wndDlgBar.GetDlgItem(IDC_BUTTON_UP);
	CRect rcButton;
	pButton->GetWindowRect(&rcButton);

	CEdit* pEdit = (CEdit*)m_wndDlgBar.GetDlgItem(IDC_EDIT_NOTE);
	CRect rcNote;
	pEdit->GetWindowRect(&rcNote);
	int height =rcNote.bottom - rcNote.top;
	rcNote=CRect(rcNote.left,rcDlgBar.bottom-5-height,rcDlgBar.right-5,rcDlgBar.bottom-5);

	CListBox* pLB =(CListBox*)m_wndDlgBar.GetDlgItem(IDC_LIST_NAME);
//	CRect rcLBName(rcButton.left,rcButton.bottom+5,rcDlgBar.right-5,rcDlgBar.bottom-5);
	CRect rcLBName(rcButton.left,rcButton.bottom+5,rcDlgBar.right-5,rcNote.top+5);
//	pLB->GetWindowRect(&rcLBName);
//	rcLBName.bottom = rcDlgBar.bottom-5;
	m_wndDlgBar.ScreenToClient(&rcLBName);
	pLB->MoveWindow(&rcLBName);
	m_wndDlgBar.ScreenToClient(&rcNote);
	pEdit->MoveWindow(&rcNote);
}



void CMainFrame::OnFileNew()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	if(DoSaveChanged()){
		m_LCoVDoc.m_CompLib.deleteContent();
		m_LCoVDoc.m_bDirty = false;
		setCompIndexToListBox();
		m_wndView.setCompIndex(NULL);
		setAppCaption(NULL);
		CEdit* pEdit = (CEdit*)m_wndDlgBar.GetDlgItem(IDC_EDIT_NOTE);
		pEdit->SetWindowText(_T(""));
	}
}

void CMainFrame::OnFileOpen()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	if(DoSaveChanged()){
		DoOpen();
	}
}

//ライブラリファイルのオープン
BOOL CMainFrame::DoOpen(LPCTSTR pszFileName)
{
	CString strOpenName;
	if(!pszFileName){
		CFileDialog dlg(TRUE,						//開くダイアログ
						_T("LB3"),						//拡張子が付けられなかった場合のデフォルト
						_T("*.LB3"),					//初期ファイル名
						OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
						_T("BSch3V library file(*.lb3)|*.LB3|BSch3V schematic(*.ce3)|*.CE3||"),	//フィルタ文字列
						this);						//親ウインドウ
		if(dlg.DoModal()!=IDOK) return FALSE;
		strOpenName=CString(dlg.m_ofn.lpstrFile);	//ファイルダイアログからファイル名を取得して
	}else{
		strOpenName=CString(pszFileName);
	}
	m_wndView.setCompIndex(NULL);
	m_LCoVDoc.m_CompLib.deleteContent();
	m_LCoVDoc.m_CompLib.readLibraryFile(strOpenName);
	m_LCoVDoc.m_bDirty = false;

	if(m_LCoVDoc.m_CompLib.m_dataVersion > SCompLib::m_nConstDataVersion){
		AfxMessageBox(IDS_LATERVERSION,MB_ICONEXCLAMATION,MB_OK);
	}


	setAppCaption(strOpenName);

	setCompIndexToListBox();
	CListBox* pLB =(CListBox*)m_wndDlgBar.GetDlgItem(IDC_LIST_NAME);
	CEdit* pEdit = (CEdit*)m_wndDlgBar.GetDlgItem(IDC_EDIT_NOTE);
	if(pLB->GetCount()>0){
		pLB->SetCurSel(0);
		SCompIndex* pCompIndex = (SCompIndex*)pLB->GetItemData(0);
		m_wndView.setCompIndex(pCompIndex);
		pEdit->SetWindowText(pCompIndex->compInfo()->m_note.c_str());
	}else{
		m_wndView.setCompIndex(NULL);
		pEdit->SetWindowText(_T(""));
	}

	return TRUE;
}

//部品インデックスをリストボックスに設定
void  CMainFrame::setCompIndexToListBox()
{
	SCompIndexListIterator iteIndex;
	iteIndex =m_LCoVDoc.m_CompLib.m_listCompIndex.begin();
	SLBCompName* pLB =(SLBCompName*)m_wndDlgBar.GetDlgItem(IDC_LIST_NAME);
	pLB->ResetContent();	//リストボックスの内容をすべて初期化
	int n=0;
	while(iteIndex != m_LCoVDoc.m_CompLib.m_listCompIndex.end()){
		int indexNum = pLB->AddString(_T(""));
		pLB->SetItemData(indexNum,(DWORD_PTR)(*iteIndex));
		//CString str;
		iteIndex++;
	}
}

void CMainFrame::OnSelchangeNameList() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	SLBCompName* pLB =(SLBCompName*)m_wndDlgBar.GetDlgItem(IDC_LIST_NAME);
	int n=pLB->GetCurSel();
	if(n==-1) return;
	if(m_wndView.m_pCompIndex!=NULL){
		m_wndView.releaseTemp();
		//m_wndView.m_doc.releaseTemp();
		if(m_wndView.m_doc.m_dirtyFlag){
			m_wndView.saveCompInfo();
			m_LCoVDoc.m_bDirty = true;
		}
	}
	SCompIndex* pCompIndex = (SCompIndex*)pLB->GetItemData(n);
	m_wndView.setCompIndex(pCompIndex);
	CEdit* pEdit = (CEdit*)m_wndDlgBar.GetDlgItem(IDC_EDIT_NOTE);
	pEdit->SetWindowText(pCompIndex->compInfo()->m_note.c_str());
}

void CMainFrame::OnButtonUp()
{
	TRACE("Button UP\n");
	SLBCompName* pLB =(SLBCompName*)m_wndDlgBar.GetDlgItem(IDC_LIST_NAME);
	int n=pLB->GetCurSel();
	if(n>0){
		SCompIndex* pCompIndex = (SCompIndex*)pLB->GetItemData(n);
		pLB->DeleteString(n);
		int newIndex = pLB->InsertString(n-1,_T(" "));
		pLB->SetItemData(newIndex,(DWORD_PTR)pCompIndex);
		pLB->SetCurSel(newIndex);
		m_LCoVDoc.m_bDirty = true;
	}
}

void CMainFrame::OnButtonDown()
{
	TRACE(_T("Button Down\n"));
	SLBCompName* pLB =(SLBCompName*)m_wndDlgBar.GetDlgItem(IDC_LIST_NAME);
	int n=pLB->GetCurSel();
	int nCount=pLB->GetCount();
	if(n>=0&&n<nCount-1){
		SCompIndex* pCompIndex = (SCompIndex*)pLB->GetItemData(n);
		pLB->DeleteString(n);
		int newIndex = pLB->InsertString(n+1,_T(" "));
		pLB->SetItemData(newIndex,(DWORD_PTR)pCompIndex);
		pLB->SetCurSel(newIndex);
		m_LCoVDoc.m_bDirty = true;
	}
}



void CMainFrame::OnUpdateButtonUp(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	SLBCompName* pLB =(SLBCompName*)m_wndDlgBar.GetDlgItem(IDC_LIST_NAME);
	int n=pLB->GetCurSel();
	pCmdUI->Enable(n>0);
}

void CMainFrame::OnUpdateButtonDown(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	SLBCompName* pLB =(SLBCompName*)m_wndDlgBar.GetDlgItem(IDC_LIST_NAME);
	int n=pLB->GetCurSel();
	int nCount=pLB->GetCount();
	pCmdUI->Enable(n>=0&&n<nCount-1);
}


void CMainFrame::OnEditSortComponent()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	if(AfxMessageBox(IDS_NOTICE_SORT,MB_OKCANCEL)==IDOK){
		sortComponent();
	}
}

void CMainFrame::OnUpdateEditSortComponent(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	SLBCompName* pLB =(SLBCompName*)m_wndDlgBar.GetDlgItem(IDC_LIST_NAME);
	pCmdUI->Enable(pLB->GetCount()>1);
}

void CMainFrame::sortComponent()
{
	SLBCompName* pLB =(SLBCompName*)m_wndDlgBar.GetDlgItem(IDC_LIST_NAME);
	int nCount=pLB->GetCount();
	PSCompIndex* pArray=new PSCompIndex[nCount];
	
	int n;
	for(n=0;n<nCount;n++){
		pArray[n] = (SCompIndex*)pLB->GetItemData(n);
	}
	qsort(pArray,nCount,sizeof(void*),
	(int(*)(const void *,const void *))sortComponentCompare);

	pLB->ResetContent();	//リストボックスの内容をすべて初期化

	for(n=0;n<nCount;n++){
		int indexNum = pLB->AddString(_T(""));
		pLB->SetItemData(indexNum,(DWORD_PTR)(pArray[n]));
	}
	pLB->SetCurSel(0);
	SCompIndex* pCompIndex = (SCompIndex*)pLB->GetItemData(0);
	m_wndView.setCompIndex(pCompIndex);
	CEdit* pEdit = (CEdit*)m_wndDlgBar.GetDlgItem(IDC_EDIT_NOTE);
	pEdit->SetWindowText(pCompIndex->compInfo()->m_note.c_str());


	delete[]pArray;
}

int CMainFrame::sortComponentCompare(SCompIndex** p1,SCompIndex ** p2)
{
	return _tcscmp( (*p1)->name(), (*p2)->name() );
}



BOOL CMainFrame::DoSaveChanged()
{
	if(m_wndView.m_pCompIndex!=NULL){
		m_wndView.releaseTemp();
		if(m_wndView.m_doc.m_dirtyFlag){
			m_wndView.saveCompInfo();
			m_wndView.m_doc.m_dirtyFlag=false;
			m_LCoVDoc.m_bDirty = true;
		}
	}
	if(m_LCoVDoc.m_bDirty){
		int n=AfxMessageBox(IDS_STRING_SAVE_CURRENT,MB_YESNOCANCEL);
		if(n==IDYES){
			if(DoOnFileSave()) return TRUE;
			return FALSE;
		}else if(n==IDCANCEL){
			return FALSE;
		}else{
			return TRUE;
		}
	}
	return TRUE;
}

//ファイル保存の実際
BOOL CMainFrame::DoSave(LPCTSTR lpszFileName)
{
//	return m_LCoVDoc.m_CompLib.writeLibraryFile(filename);

	m_wndView.save();

	SLBCompName* pLB =(SLBCompName*)m_wndDlgBar.GetDlgItem(IDC_LIST_NAME);
	int nCount=pLB->GetCount();
	int n,m;


	FILE* fp;
	if((fp = _tfopen(lpszFileName,_T("wt, ccs=UTF-8")))==NULL){
		AfxMessageBox(IDS_SAVE_ERROR);
		return false;
	}
	fseek( fp, 0, SEEK_SET ) ; //BAD KNOWHOW for removing BOM CODE.
	SWriteCE3 wce3;
	wce3.Attach(fp);
	wce3.WriteRecord(_T("+BSCH3_LIB_V.1.0"));
	wce3.WriteEOL();

	wce3.WriteRecordInt(_T("VER"),SCompLib::m_nConstDataVersion);
	wce3.WriteEOL();


	wce3.WriteRecordString(_T("PROP"),m_LCoVDoc.m_CompLib.m_prop_string);
	wce3.WriteEOL();

	for(n=0;n<nCount;n++){
		SCompIndex* pIndex = (SCompIndex*)pLB->GetItemData(n);
		if(!pIndex)continue;
		wstring name = pIndex->name();
		if(name.empty())continue;
		SCompInfo* pInfo = pIndex->compInfo();
		if(!pInfo)continue;
		bool bAlias = false;
		wstring aliasFor;
		for(m=0;m<n && m<nCount;m++){
			SCompIndex* pIndex2 = (SCompIndex*)pLB->GetItemData(m);
			if(!pIndex2)continue;
			if(pIndex2->compInfo()==pInfo){
				aliasFor = pIndex2->name();	//書き込み済みの名前
				if(aliasFor.empty())continue;
				bAlias = true;
				break;
			}
		}
		if(bAlias){
			SCompLib::WriteAlias(wce3,name.c_str(),aliasFor.c_str());
		}else{
			pInfo->writeCe3(wce3,name.c_str());
		}
	}
	wce3.WriteRecord(_T("-BSCH3_LIB_V.1.0"));
	wce3.WriteEOL();

	fclose(fp);
	m_LCoVDoc.m_bDirty = false;
	m_wndView.m_doc.m_dirtyFlag = false;
	setAppCaption(lpszFileName);
	return true;
}

//名前を付けてファイル保存の実際
BOOL CMainFrame::DoSaveAs(LPCTSTR defaultFilename)
{
	const TCHAR* initFilename;
	if(defaultFilename){
		initFilename = defaultFilename;
	}else{
		initFilename = _T("Untitled.LB3");
	}

	CFileDialog dlg(FALSE,			//保存ダイアログ
					_T("LB3"),			//拡張子が付けられなかった場合のデフォルト
					initFilename,	//初期ファイル名
					OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
					_T("BSch3V library file(*.lb3)|*.LB3||"),	//フィルタ文字列
					this);							//親ウインドウ

	if(dlg.DoModal()==IDOK){						//ファイルダイアログの実行
		CString strSaveName=dlg.GetPathName();		//ファイルダイアログからファイル名を取得して
		TRACE("%s\n",(const TCHAR*)strSaveName);
		if(DoSave(strSaveName)){
			m_LCoVDoc.m_CompLib.setLibraryName(strSaveName);
			return TRUE;
		}
	}
	return FALSE;
}

void CMainFrame::OnFileSaveAs()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	if(((CLCoVApp*)AfxGetApp())->m_bTempMode){
		return;
	}
	if(!m_LCoVDoc.m_CompLib.m_lpszLibraryName){
		DoSaveAs(NULL);
		return;
	}
	TCHAR szpath[_MAX_PATH];
	TCHAR szdrive[_MAX_DRIVE];
	TCHAR szdir[_MAX_DIR];
	TCHAR szname[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];
	_tsplitpath(m_LCoVDoc.m_CompLib.m_lpszLibraryName,szdrive,szdir,szname,ext);
	if(_tcsicmp(ext,_T(".LB3"))!=0){
		_tmakepath(szpath,szdrive,szdir,szname,_T(".LB3"));
		DoSaveAs(szpath);
	}else{
		DoSaveAs(m_LCoVDoc.m_CompLib.m_lpszLibraryName);
	}
}

BOOL CMainFrame::DoOnFileSave()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	if(!m_LCoVDoc.m_CompLib.m_lpszLibraryName){
		return DoSaveAs(NULL);
	}

	TCHAR szpath[_MAX_PATH];
	TCHAR szdrive[_MAX_DRIVE];
	TCHAR szdir[_MAX_DIR];
	TCHAR szname[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];
	_tsplitpath(m_LCoVDoc.m_CompLib.m_lpszLibraryName,szdrive,szdir,szname,ext);
	if(_tcsicmp(ext,_T(".CE3"))==0){
		_tmakepath(szpath,szdrive,szdir,szname,_T(".LB3"));
		return	DoSaveAs(szpath);
	}
	return DoSave(m_LCoVDoc.m_CompLib.m_lpszLibraryName);
}

void CMainFrame::OnFileSave()
{
	DoOnFileSave();
}

void CMainFrame::doCopyComp()
{
	if(m_wndView.m_pCompIndex!=NULL){
		m_wndView.releaseTemp();
		//m_wndView.m_doc.releaseTemp();
		if(m_wndView.m_doc.m_dirtyFlag){
			m_wndView.saveCompInfo();
		}
	}

	SLBCompName* pLB =(SLBCompName*)m_wndDlgBar.GetDlgItem(IDC_LIST_NAME);
	int nIndex = pLB->GetCurSel();
	SCompIndex* pIndex = (SCompIndex*)pLB->GetItemData(nIndex);
	if(!pIndex)return;
	wstring name = pIndex->name();
	if(name.empty())return;
	SCompInfo* pInfo = pIndex->compInfo();
	if(!pInfo)return;
	wstring str;
	SWriteCE3 wce3;
	wce3.Attach(&str);

	wce3.WriteRecord(_T("+BSCH3_DATA_V.1.0"));	wce3.WriteEOL();
	wce3.WriteRecord(_T("+COMPONENT"));			wce3.WriteEOL();
	wce3.WriteRecord(_T("+BSCH3_LIB_V.1.0"));	wce3.WriteEOL();
	wce3.WriteRecordInt(_T("VER"),SCompLib::m_nConstDataVersion);	wce3.WriteEOL();
	pInfo->writeCe3(wce3,name.c_str());
	wce3.WriteRecord(_T("-BSCH3_LIB_V.1.0"));	wce3.WriteEOL();
	wce3.WriteRecordInt(_T("X"),pInfo->m_size.w()*10);
	wce3.WriteRecordInt(_T("Y"),pInfo->m_size.h()*10);
	wce3.WriteRecordString(_T("LIB"),name);
	wce3.WriteRecordString(_T("N"),  name);
	wce3.WriteRecordString(_T("R"),  pInfo->m_pszRef);wce3.WriteEOL();
	wce3.WriteRecord(_T("-COMPONENT"));			wce3.WriteEOL();
	wce3.WriteRecord(_T("-BSCH3_DATA_V.1.0"));	wce3.WriteEOL();

	int nLength = str.size()+1;
	HGLOBAL hGMem=::GlobalAlloc(GMEM_MOVEABLE,nLength*sizeof(TCHAR));//クリップボードに渡すメモリの確保
	if(hGMem==NULL){
		return;
	}
	BYTE* pgBuff=(BYTE*)::GlobalLock(hGMem);	//割り当てたグローバルメモリをロックする
	memcpy(pgBuff,str.c_str(),nLength*sizeof(TCHAR));		//割り当てたグローバルメモリにメモリファイルの内容をコピー
	::GlobalUnlock(hGMem);				//グローバルメモリのアンロック
	::OpenClipboard(AfxGetMainWnd()->m_hWnd);//クリップボードのオープン
	::EmptyClipboard();					//クリップボードを空にする
	::SetClipboardData(CF_UNICODETEXT,hGMem);	//クリップボードにデータを渡す
	::CloseClipboard();					//クリップボードを閉じる
}


void CMainFrame::OnEditCopyComp()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	SLBCompName* pLB =(SLBCompName*)m_wndDlgBar.GetDlgItem(IDC_LIST_NAME);
	int nIndex = pLB->GetCurSel();
	if(nIndex<0)return;
	doCopyComp();
}

void CMainFrame::OnUpdateEditCopyComp(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	SLBCompName* pLB =(SLBCompName*)m_wndDlgBar.GetDlgItem(IDC_LIST_NAME);
	pCmdUI->Enable(pLB->GetCurSel()>=0);
}

void CMainFrame::OnEditCutComp()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	doCopyComp();
	doDeleteComp();
}

void CMainFrame::OnUpdateEditCutComp(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	SLBCompName* pLB =(SLBCompName*)m_wndDlgBar.GetDlgItem(IDC_LIST_NAME);
	pCmdUI->Enable(pLB->GetCurSel()>=0);
}

void CMainFrame::doDeleteComp()
{
	SLBCompName* pLB =(SLBCompName*)m_wndDlgBar.GetDlgItem(IDC_LIST_NAME);
	int nIndex = pLB->GetCurSel();
	if(nIndex<0)return;
	m_wndView.setCompIndex(NULL);
	CEdit* pEdit = (CEdit*)m_wndDlgBar.GetDlgItem(IDC_EDIT_NOTE);
	pEdit->SetWindowText(_T(""));
	SCompIndex* pIndex = (SCompIndex*)pLB->GetItemData(nIndex);
	pLB->DeleteString(nIndex);


	bool shareConpInfo =false;	

	SCompIndexListIterator iteIndex =m_LCoVDoc.m_CompLib.m_listCompIndex.begin();
	while(iteIndex != m_LCoVDoc.m_CompLib.m_listCompIndex.end()){
		if((*iteIndex)==pIndex){
			SCompIndexListIterator iteIndex2 =m_LCoVDoc.m_CompLib.m_listCompIndex.begin();
			while(iteIndex2 != m_LCoVDoc.m_CompLib.m_listCompIndex.end()){
				if(iteIndex!=iteIndex2){
					if((*iteIndex)->m_pCompInfo ==(*iteIndex2)->m_pCompInfo){
						shareConpInfo = true;
						break;
					}
				}
				iteIndex2++;
			}
			if(!shareConpInfo){
				SCompInfoListIterator iteInfo = m_LCoVDoc.m_CompLib.m_listCompInfo.begin();
				while(iteInfo != m_LCoVDoc.m_CompLib.m_listCompInfo.end()){
					if((*iteIndex)->m_pCompInfo == (*iteInfo)){
						delete (*iteInfo);
						m_LCoVDoc.m_CompLib.m_listCompInfo.erase(iteInfo);
						break;
					}
					iteInfo++;
				}
			}
			delete (*iteIndex);
			m_LCoVDoc.m_CompLib.m_listCompIndex.erase(iteIndex);
			break;
		}
		iteIndex++;
	}
	int nCount = pLB->GetCount();
	if(nCount-1 < nIndex) nIndex = nCount-1;
	if(nIndex>=0){
		pLB->SetCurSel(nIndex);
		pIndex = (SCompIndex*)pLB->GetItemData(nIndex);
		m_wndView.setCompIndex(pIndex);
		CEdit* pEdit = (CEdit*)m_wndDlgBar.GetDlgItem(IDC_EDIT_NOTE);
		pEdit->SetWindowText(pIndex->m_pCompInfo->m_note.c_str());
	}
}


bool CMainFrame::canPaste()
{
	static const TCHAR* pcszID= _T("+BSCH3_DATA_V.1.0");
	int nIdLength = _tcslen(pcszID);
	bool retVal = false;
	if(::IsClipboardFormatAvailable(CF_UNICODETEXT)){
		::OpenClipboard(AfxGetMainWnd()->m_hWnd);		//Open clip board.
		HGLOBAL hGMem=::GetClipboardData(CF_UNICODETEXT);		//Get memory handle.
		if(hGMem!=NULL){
			int nLength=GlobalSize(hGMem);				//Size of clip board data.
			if(nLength>nIdLength*4){
				TCHAR* pgBuff=(TCHAR*)::GlobalLock(hGMem);	//Lock global memory.
				if(_tcsncmp(pcszID,pgBuff,nIdLength)==0){
					retVal = true;
				}
				::GlobalUnlock(hGMem);				//Unock global memory.
			}
		}
		::CloseClipboard();
	}
	return retVal;
}


void CMainFrame::OnEditPasteComp()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	if(!canPaste())return;

	::OpenClipboard(AfxGetMainWnd()->m_hWnd);		//Open clip board.
	HGLOBAL hGMem=::GetClipboardData(CF_UNICODETEXT);		//Get memory handle.
	if(hGMem==NULL){
		::CloseClipboard();						//Close clip board.
		return;
	}
	TCHAR* pgBuff=(TCHAR*)::GlobalLock(hGMem);	//Lock global memory.
	wstring cbbuff=wstring(pgBuff);				//Initialize string with global memory.
	::GlobalUnlock(hGMem);						//Unock global memory.
	::CloseClipboard();							//Close clip board.


	SReadCE3 rce3;
	rce3.Attach(cbbuff.c_str());

	wstring str(_T(""));

	rce3.ReadRecord(str);
	if(str != _T("+BSCH3_DATA_V.1.0")) return;
	if(rce3.SkipTo(_T("+BSCH3_LIB_V.1.0"))==WEOF) return;

	if(!m_LCoVDoc.m_CompLib.ReadLb3(rce3,true))return;
	
	SCompIndex* pIndex = m_LCoVDoc.m_CompLib.m_listCompIndex.back();
	if(pIndex){
		if(m_wndView.m_pCompIndex!=NULL){
			m_wndView.releaseTemp();
			//m_wndView.m_doc.releaseTemp();
			if(m_wndView.m_doc.m_dirtyFlag){
				m_wndView.saveCompInfo();
			}
		}
		SLBCompName* pLB =(SLBCompName*)m_wndDlgBar.GetDlgItem(IDC_LIST_NAME);
		int n=pLB->GetCurSel();
		int newIndex = pLB->InsertString(n,_T(" "));
		pLB->SetItemData(newIndex,(DWORD_PTR)pIndex);
		pLB->SetCurSel(newIndex);
		m_wndView.setCompIndex(pIndex);
		CEdit* pEdit = (CEdit*)m_wndDlgBar.GetDlgItem(IDC_EDIT_NOTE);
		pEdit->SetWindowText(pIndex->compInfo()->m_note.c_str());
		m_wndView.setDirty();
		m_LCoVDoc.m_bDirty = true;
	}
}


void CMainFrame::OnUpdateEditPasteComp(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->Enable(canPaste());
}



void CMainFrame::OnToolSelect()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	m_wndView.setTool(CCompEditView::TOOL_SELECT);
}

void CMainFrame::OnUpdateToolSelect(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->Enable(m_wndView.m_pCompIndex!=NULL);
	pCmdUI->SetCheck(m_wndView.tool()==CCompEditView::TOOL_SELECT);
}

void CMainFrame::OnToolPin()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	m_wndView.setTool(CCompEditView::TOOL_PIN);
}

void CMainFrame::OnUpdateToolPin(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->Enable(m_wndView.m_pCompIndex!=NULL);
	pCmdUI->SetCheck(m_wndView.tool()==CCompEditView::TOOL_PIN);
}

void CMainFrame::OnToolLine()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	m_wndView.setTool(CCompEditView::TOOL_LINE);
}

void CMainFrame::OnUpdateToolLine(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->Enable(m_wndView.m_pCompIndex!=NULL && m_wndView.m_pCompIndex->pattern()!=NULL);
	pCmdUI->SetCheck(m_wndView.tool()==CCompEditView::TOOL_LINE);
}

void CMainFrame::OnToolCircle()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	m_wndView.setTool(CCompEditView::TOOL_CIRCLE);
}

void CMainFrame::OnUpdateToolCircle(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->Enable(m_wndView.m_pCompIndex!=NULL  && m_wndView.m_pCompIndex->pattern()!=NULL);
	pCmdUI->SetCheck(m_wndView.tool()==CCompEditView::TOOL_CIRCLE);
}

void CMainFrame::OnToolPolygon()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	m_wndView.setTool(CCompEditView::TOOL_POLYGON);
}

void CMainFrame::OnUpdateToolPolygon(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->Enable(m_wndView.m_pCompIndex!=NULL  && m_wndView.m_pCompIndex->pattern()!=NULL);
	pCmdUI->SetCheck(m_wndView.tool()==CCompEditView::TOOL_POLYGON);
}

void CMainFrame::OnToolText()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	m_wndView.setTool(CCompEditView::TOOL_TEXT);
}

void CMainFrame::OnUpdateToolText(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->Enable(m_wndView.m_pCompIndex!=NULL  && m_wndView.m_pCompIndex->pattern()!=NULL);
	pCmdUI->SetCheck(m_wndView.tool()==CCompEditView::TOOL_TEXT);
}

void CMainFrame::OnToolArc()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	m_wndView.setTool(CCompEditView::TOOL_ARC);
}

void CMainFrame::OnUpdateToolArc(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->Enable(m_wndView.m_pCompIndex!=NULL && m_wndView.m_pCompIndex->pattern()!=NULL);
	pCmdUI->SetCheck(m_wndView.tool()==CCompEditView::TOOL_ARC);
}

void CMainFrame::OnEditProperty()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	m_wndView.editProperty();
}

void CMainFrame::OnUpdateEditProperty(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->Enable(m_wndView.m_pCompIndex!=NULL && m_wndView.canProperty());
}

void CMainFrame::OnEditCopy()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	if(!((m_wndView.m_pCompIndex!=NULL)&&(m_wndView.canCopy()))) return;
	m_wndView.doCopy();
}

void CMainFrame::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->Enable((m_wndView.m_pCompIndex!=NULL)&&(m_wndView.canCopy()));
}

void CMainFrame::OnEditPaste()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	if(!((m_wndView.m_pCompIndex!=NULL)&&(m_wndView.canPaste()))) return;
	m_wndView.doPaste();
}

void CMainFrame::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->Enable((m_wndView.m_pCompIndex!=NULL)&&(m_wndView.canPaste()));
}

void CMainFrame::OnEditCut()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	if(!((m_wndView.m_pCompIndex!=NULL)&&(m_wndView.canCopy()))) return;
	m_wndView.doCopy();
	m_wndView.doDelete();
}

void CMainFrame::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->Enable((m_wndView.m_pCompIndex!=NULL)&&(m_wndView.canCopy()));
}

void CMainFrame::OnEditUndo()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	if(!((m_wndView.m_pCompIndex!=NULL)&&(m_wndView.canUndo()))) return;
	m_wndView.doUndo();
}

void CMainFrame::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->Enable((m_wndView.m_pCompIndex!=NULL)&&(m_wndView.canUndo()));
}

void CMainFrame::OnEditNewComp()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	CCompProperty dlg;

	if(dlg.DoModal()==IDOK){
		SCompInfo* pComp = new SCompInfo;
		SSize sizeComp=SSize(dlg.m_x,dlg.m_y);
		bool usePtn = (dlg.m_usePtn != FALSE);
		bool nobit =  (dlg.m_noBitPtn != FALSE);
		pComp->setSize(sizeComp,usePtn,nobit);
		pComp->setRef(dlg.m_num);
		pComp->setBlock(dlg.m_block);
		pComp->m_note = dlg.m_note;
		pComp->m_mfr  = dlg.m_mfr;
		pComp->m_mfrpn= dlg.m_mfrpn;
		pComp->m_pkg  = dlg.m_pkg;

		pComp->setFixedBlock(dlg.m_fixedBlockNum - 1);
		
		if(dlg.m_useFixedBlockNum){
			pComp->setCiType(SCompInfo::CITYPE::FIXED_BLOCK);
		}else if(dlg.m_sharedBlock){
			pComp->setCiType(SCompInfo::CITYPE::SHARED_BLOCK);
		}else{
			pComp->setCiType(SCompInfo::CITYPE::GENERAL);
		}



		wstring name = dlg.m_name;

		wstring uniqname = m_LCoVDoc.m_CompLib.uniqueName(name);
	
		SCompIndex* pIndex = new SCompIndex;
		pIndex->setName(uniqname.c_str());
		pIndex->setCompInfo(pComp);

		m_LCoVDoc.m_CompLib.m_listCompIndex.push_back(pIndex);
		m_LCoVDoc.m_CompLib.m_listCompInfo.push_back(pComp);
		if(pIndex){
			if(m_wndView.m_pCompIndex!=NULL){
				m_wndView.releaseTemp();
				//m_wndView.m_doc.releaseTemp();
				if(m_wndView.m_doc.m_dirtyFlag){
					m_wndView.saveCompInfo();
				}
			}
			SLBCompName* pLB =(SLBCompName*)m_wndDlgBar.GetDlgItem(IDC_LIST_NAME);
			int n=pLB->GetCurSel();
			int newIndex = pLB->InsertString(n,_T(" "));
			pLB->SetItemData(newIndex,(DWORD_PTR)pIndex);
			pLB->SetCurSel(newIndex);
			m_wndView.setCompIndex(pIndex);
			CEdit* pEdit = (CEdit*)m_wndDlgBar.GetDlgItem(IDC_EDIT_NOTE);
			pEdit->SetWindowText(pIndex->compInfo()->m_note.c_str());
		}
	}
	m_LCoVDoc.m_bDirty = true;
}

void CMainFrame::OnEditPropertyComp()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	SLBCompName* pLB =(SLBCompName*)m_wndDlgBar.GetDlgItem(IDC_LIST_NAME);
	int nIndex = pLB->GetCurSel();
	if(nIndex<0) return;

	if(m_wndView.m_pCompIndex!=NULL){
		m_wndView.releaseTemp();
		//m_wndView.m_doc.releaseTemp();
		if(m_wndView.m_doc.m_dirtyFlag){
			m_wndView.saveCompInfo();
		}
	}

	SCompIndex* pIndex = (SCompIndex*)pLB->GetItemData(nIndex);
	if(!pIndex)return;
	wstring oldName = pIndex->name();
	//if(oldName.empty())return;
	SCompInfo* pComp = pIndex->compInfo();
	if(!pComp)return;



	CCompProperty dlg;
	dlg.m_name = oldName.c_str();
	dlg.m_num = pComp->m_pszRef;
	dlg.m_x = pComp->m_size.w();
	dlg.m_y = pComp->m_size.h();
	dlg.m_block = pComp->m_nBlock;
	dlg.m_usePtn = (pComp->m_pPtn!=NULL);
	dlg.m_note = pComp->m_note.c_str();
	dlg.m_mfr = pComp->m_mfr.c_str();
	dlg.m_mfrpn = pComp->m_mfrpn.c_str();
	dlg.m_pkg = pComp->m_pkg.c_str();

	dlg.m_fixedBlockNum = pComp->m_nFixedBlock+1;

	switch(pComp->m_ciType){
		case SCompInfo::CITYPE::FIXED_BLOCK:
			dlg.m_block = 1;
			dlg.m_useFixedBlockNum = TRUE;
			dlg.m_sharedBlock = FALSE;
			break;
		case SCompInfo::CITYPE::SHARED_BLOCK:
			dlg.m_block = 1;
			dlg.m_fixedBlockNum = 1;
			dlg.m_useFixedBlockNum = FALSE;
			dlg.m_sharedBlock = TRUE;
			break;
		default:	//case SCompInfo::GENERAL:
			dlg.m_fixedBlockNum = 1;
			dlg.m_useFixedBlockNum = FALSE;
			dlg.m_sharedBlock = FALSE;
	}



	if(pComp->m_pPtn!=NULL){
		dlg.m_noBitPtn = (pComp->m_pPtn->m_pBuff==NULL);
	}else{
		dlg.m_noBitPtn = FALSE;
	}

	if(dlg.DoModal()==IDOK){
		SSize sizeComp=SSize(dlg.m_x,dlg.m_y);
		bool usePtn = (dlg.m_usePtn != FALSE);
		bool nobit =  (dlg.m_noBitPtn != FALSE);
		pComp->setSize(sizeComp,usePtn,nobit);
		pComp->setUsePattern(usePtn,nobit);
		pComp->setRef(dlg.m_num);
		pComp->setBlock(dlg.m_block);
		pComp->m_note = dlg.m_note;
		pComp->m_mfr  = dlg.m_mfr;
		pComp->m_mfrpn= dlg.m_mfrpn;
		pComp->m_pkg  = dlg.m_pkg;

		pComp->setFixedBlock(dlg.m_fixedBlockNum - 1);

		if(dlg.m_useFixedBlockNum){
			pComp->setCiType(SCompInfo::CITYPE::FIXED_BLOCK);
		}else if(dlg.m_sharedBlock){
			pComp->setCiType(SCompInfo::CITYPE::SHARED_BLOCK);
		}else{
			pComp->setCiType(SCompInfo::CITYPE::GENERAL);
		}

		wstring name = dlg.m_name;
		if(name != oldName){
			wstring uniqname = m_LCoVDoc.m_CompLib.uniqueName(name);
			pIndex->setName(uniqname.c_str());
			pLB->Invalidate();
		}
		m_wndView.setCompIndex(pIndex);
		CEdit* pEdit = (CEdit*)m_wndDlgBar.GetDlgItem(IDC_EDIT_NOTE);
		pEdit->SetWindowText(pIndex->compInfo()->m_note.c_str());

		m_wndView.setDirty();
		m_LCoVDoc.m_bDirty = true;
	}
}

void CMainFrame::OnUpdateEditPropertyComp(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	SLBCompName* pLB =(SLBCompName*)m_wndDlgBar.GetDlgItem(IDC_LIST_NAME);
	pCmdUI->Enable(pLB->GetCurSel()>=0);
}

void CMainFrame::OnUpdateEditMirror(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->Enable((m_wndView.m_pCompIndex!=NULL)&&(m_wndView.canMirrorRotate()));
}

void CMainFrame::OnUpdateEditRotate(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->Enable((m_wndView.m_pCompIndex!=NULL)&&(m_wndView.canMirrorRotate()));
}

void CMainFrame::OnEditRotate()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	if((m_wndView.m_pCompIndex!=NULL)&&(m_wndView.canMirrorRotate())){
		m_wndView.doRotate();
	}
}

void CMainFrame::OnEditMirror()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	if((m_wndView.m_pCompIndex!=NULL)&&(m_wndView.canMirrorRotate())){
		m_wndView.doMirror();
	}
}

void CMainFrame::OnEditNodeMode()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	m_wndView.setEditNodeMode(!m_wndView.editNodeMode());
}

void CMainFrame::OnUpdateEditNodeMode(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->Enable(m_wndView.m_pCompIndex!=NULL  && m_wndView.m_pCompIndex->pattern()!=NULL);
	pCmdUI->SetCheck(m_wndView.editNodeMode());
}

void CMainFrame::OnToolCreateBptn()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	if(AfxMessageBox(IDS_STRING_CREATE_BPTN,MB_OKCANCEL) == IDOK){
		m_wndView.doCreateBitPtn();
	}
}

void CMainFrame::OnUpdateToolCreateBptn(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->Enable(m_wndView.canCreateBitPtn());
}

void CMainFrame::OnClose()
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	if(DoSaveChanged()){
		CFrameWnd::OnClose();
	}
}

void CMainFrame::OnEditClear()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	if(!((m_wndView.m_pCompIndex!=NULL)&&(m_wndView.canCopy()))) return;
	m_wndView.doDelete();
}

void CMainFrame::OnUpdateEditClear(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->Enable((m_wndView.m_pCompIndex!=NULL)&&(m_wndView.canCopy()));
}

void CMainFrame::OnUpdateFileSaveAs(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->Enable( ((CLCoVApp*)AfxGetApp())->m_bTempMode == false );
}

void CMainFrame::OnEditPropLib()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	CPropLib dlg;
	dlg.m_propString = m_LCoVDoc.m_CompLib.m_prop_string.c_str();
	if(dlg.DoModal()==IDOK){
		m_LCoVDoc.m_CompLib.m_prop_string = dlg.m_propString;
		m_LCoVDoc.m_bDirty = true;
	}
}

void CMainFrame::OnEditAddAlias()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	SLBCompName* pLB =(SLBCompName*)m_wndDlgBar.GetDlgItem(IDC_LIST_NAME);
	int nIndex = pLB->GetCurSel();
	if(nIndex<0) return;

	if(m_wndView.m_pCompIndex!=NULL){
		m_wndView.releaseTemp();
		//m_wndView.m_doc.releaseTemp();
		if(m_wndView.m_doc.m_dirtyFlag){
			m_wndView.saveCompInfo();
		}
	}

	SCompIndex* pIndex = (SCompIndex*)pLB->GetItemData(nIndex);
	if(!pIndex)return;
	wstring strName = pIndex->name();
	SCompInfo* pCompInfo = pIndex->compInfo();
	if(!pCompInfo)return;

	CAddAlias dlg;
	dlg.m_strName = strName.c_str();
	dlg.m_strAlias = "NAME";
// ここにダイアログ
	if(dlg.DoModal()==IDOK){

		wstring strAlias = dlg.m_strAlias;

		wstring uniqname = m_LCoVDoc.m_CompLib.uniqueName(strAlias);
		SCompIndex* pIndex = new SCompIndex;
		pIndex->setName(uniqname.c_str());
		pIndex->setCompInfo(pCompInfo);

		m_LCoVDoc.m_CompLib.m_listCompIndex.push_back(pIndex);

		int newIndex = pLB->InsertString(nIndex,_T(" "));
		pLB->SetItemData(newIndex,(DWORD_PTR)pIndex);
		pLB->SetCurSel(newIndex);
		m_wndView.setCompIndex(pIndex);
		CEdit* pEdit = (CEdit*)m_wndDlgBar.GetDlgItem(IDC_EDIT_NOTE);
		pEdit->SetWindowText(pIndex->compInfo()->m_note.c_str());
	}
}

void CMainFrame::OnUpdateEditAddAlias(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	SLBCompName* pLB =(SLBCompName*)m_wndDlgBar.GetDlgItem(IDC_LIST_NAME);
	pCmdUI->Enable(pLB->GetCurSel()>=0);
}



void CMainFrame::OnEditNextPinName()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	m_wndView.nextPinName();

}

void CMainFrame::OnUpdateEditNextPinName(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->Enable(m_wndView.m_pCompIndex!=NULL && m_wndView.canNextPinName());
}

void CMainFrame::OnEditNextPinNum()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	m_wndView.nextPinNumber();
}

void CMainFrame::OnUpdateNextPinNum(CCmdUI *pCmdUI)
{
	// TODO : ここにコマンド更新 UI ハンドラ コードを追加します。
	pCmdUI->Enable(m_wndView.m_pCompIndex!=NULL && m_wndView.canNextPinNumber());
}

//>>>フレームウィンドウサイズの復元 BSch3Vから移植 2008/02/09
CSize CMainFrame::m_sizeInitialWindowSize = CSize(CW_USEDEFAULT,CW_USEDEFAULT);

void CMainFrame::SetInitialWindowSize(int cx,int cy)
{
	if(cx!=CW_USEDEFAULT && cx<400) cx = 400;
	if(cy!=CW_USEDEFAULT && cy<300) cy = 300;
	m_sizeInitialWindowSize = CSize(cx,cy);
}

#define WINDOW_POS_SECTION _T("WindowPosition")
void CMainFrame::IniWriteWindowPosition(int cmdShow,int cx,int cy)
{
	AfxGetApp()->WriteProfileInt(WINDOW_POS_SECTION,_T("cmdShow"),cmdShow);
	AfxGetApp()->WriteProfileInt(WINDOW_POS_SECTION,_T("Width"),cx);
	AfxGetApp()->WriteProfileInt(WINDOW_POS_SECTION,_T("Height"),cy);
}
void CMainFrame::IniReadWindowPosition(int& cmdShow,int& cx,int& cy)
{
	cmdShow = AfxGetApp()->GetProfileInt(WINDOW_POS_SECTION,_T("cmdShow"),SW_NORMAL);
	cx = AfxGetApp()->GetProfileInt(WINDOW_POS_SECTION,_T("Width"),CW_USEDEFAULT);
	cy = AfxGetApp()->GetProfileInt(WINDOW_POS_SECTION,_T("Height"),CW_USEDEFAULT);
}
//<<<フレームウィンドウサイズの復元 BSch3Vから移植 2008/02/09


