
// CE3SearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include <assert.h>
#include <string>
#include <list>
#include <vector>
using namespace std;

#include "atlpath.h"
#include "CE3Search.h"
#include "xbschdocsch.h"
#include "CE3SearchDlg.h"
#include "FindOption.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCE3SearchDlg dialog




CCE3SearchDlg::CCE3SearchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCE3SearchDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_imageListSortMark.Create(IDB_BITMAP_SORTMARK,16,1,RGB(0xFF,0xFF,0xFF));


	m_pathColumnWidth = 0;
	m_nSortBy = 0;
	m_bSortUp = true;
	m_nSelected = -1;

}

void CCE3SearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCE3SearchDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CCE3SearchDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCE3SearchDlg::OnBnClickedCancel)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_REF_FOLDER, &CCE3SearchDlg::OnBnClickedButtonRefFolder)
	ON_BN_CLICKED(IDC_BUTTON_START, &CCE3SearchDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CCE3SearchDlg::OnBnClickedButtonStop)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_OPTION, &CCE3SearchDlg::OnBnClickedButtonOption)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CCE3SearchDlg::OnBnClickedButtonExit)
	ON_WM_DESTROY()
	ON_WM_COPYDATA()
	ON_NOTIFY(HDN_ENDTRACK, 0, &CCE3SearchDlg::OnHdnEndtrackListFound)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CCE3SearchDlg::OnHdnItemclickListFound)
	ON_WM_SIZE()
	ON_MESSAGE(SEARCH_FIND_MSG,OnSearchFind)
	ON_MESSAGE(SEARCH_END_MSG,OnSearchEnd)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_FOUND, &CCE3SearchDlg::OnLvnItemchangedListFound)
	ON_NOTIFY(NM_CLICK, IDC_LIST_FOUND, &CCE3SearchDlg::OnNMClickListFound)
END_MESSAGE_MAP()


// CCE3SearchDlg message handlers

BOOL CCE3SearchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetProp(GetSafeHwnd(), _T("CE3Search-Suigyodo"), (HANDLE)1);

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	bool setPath = false;
	CString strInitialFolder;
	if(((CCE3SearchApp*)AfxGetApp())->m_cmdInfo.m_setPath){
		CPath strPath(((CCE3SearchApp*)AfxGetApp())->m_cmdInfo.m_strPath);
		if(!strPath.IsDirectory()){
			strPath.RemoveFileSpec();
		}
		strInitialFolder = strPath.m_strPath;
		setPath = true;

		CWnd* pWnd = CheckOtherCE3SearchPath(strPath.m_strPath);
		if(pWnd){
			if(pWnd->IsIconic()){
				pWnd->ShowWindow(SW_RESTORE);
			}
			pWnd->SetForegroundWindow();
			PostMessage(WM_CLOSE);
			return TRUE;
		}
	}
	if(!setPath){
		strInitialFolder = AfxGetApp()->GetProfileString(_T("FolderOption"),_T("InitialFolder"));
	}
	SetDlgItemText(IDC_EDIT_FOLDER,strInitialFolder);
	m_hSearchThread = 0;

	IniReadFindInfo(m_fi);
	initListCtrlFound();

	disableControls(false);

	return TRUE;  // return TRUE  unless you set the focus to a control
}




void CCE3SearchDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCE3SearchDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCE3SearchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCE3SearchDlg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	//OnOK();
}

void CCE3SearchDlg::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	//OnCancel();
}

void CCE3SearchDlg::OnClose()
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	OnCancel();
	//CDialog::OnClose();
}

//フォルダ選択ダイアログの初期値設定コールバック関数
int CALLBACK CCE3SearchDlg::BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData)
{
    if(uMsg==BFFM_INITIALIZED && lpData!=NULL){
		::SendMessage(hwnd,BFFM_SETSELECTION,(WPARAM)TRUE,lpData);
    }
    return 0;
}



void CCE3SearchDlg::OnBnClickedButtonRefFolder()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	BROWSEINFO bInfo;
    LPITEMIDLIST pIDList;
    TCHAR szNewFolder[MAX_PATH];

	CString strInitialFolder;
	GetDlgItemText(IDC_EDIT_FOLDER,strInitialFolder);

    
    // BROWSEINFO構造体に値を設定
    bInfo.hwndOwner			= AfxGetMainWnd()->m_hWnd;
	bInfo.pidlRoot			= NULL;
	bInfo.pszDisplayName	= szNewFolder;
	bInfo.lpszTitle			= _T("Select Folder");
	bInfo.ulFlags			= BIF_RETURNONLYFSDIRS;
	bInfo.lpfn				= &BrowseCallbackProc;
	bInfo.lParam			= (LPARAM)(const_cast<LPTSTR>((LPCTSTR)(strInitialFolder)));
	
    pIDList = ::SHBrowseForFolder(&bInfo);
    if(pIDList != NULL){
        if(!::SHGetPathFromIDList(pIDList, szNewFolder)){
            return;
        }
		AfxGetApp()->WriteProfileString(_T("FolderOption"),_T("InitialFolder"),szNewFolder);
		SetDlgItemText(IDC_EDIT_FOLDER,szNewFolder);
        ::CoTaskMemFree( pIDList );
    }

}





void typeString(unsigned int foundType, CString& rStrType)
{
	switch(foundType){
		case FINDTGT_TAG: rStrType = "Tag"; break;
		case FINDTGT_LABEL: rStrType = "Label"; break;
		case FINDTGT_COMMENT: rStrType = "Comment"; break;
		case FINDTGT_NAME: rStrType = "Value"; break;
		case FINDTGT_NAMELIB: rStrType = "NameInLib"; break;
		case FINDTGT_NUM: rStrType = "RefNum"; break;
		case FINDTGT_MANUFACTURE: rStrType = "Mfr"; break;
		case FINDTGT_MANUFACTUREPN: rStrType = "MfrPn"; break;
		case FINDTGT_PACKAGE: rStrType = "Pkg"; break;
		case FINDTGT_NOTE: rStrType = "Note"; break;
		default:rStrType = "";
	}
}


///////////////////////////////////////////////////////////////////////////////////
//  検索スレッド

//指定したファイルから検索を行い、検索結果をリストコントロールに追加する。
bool searchData(CString path, CCE3SearchDlg* pWnd)
{
	TRACE("Search test:%s\n",pWnd->m_fi.m_str);
	//SXBSchDoc bschdoc;
	SXBSchDocSch bschdoc;
	if(bschdoc.readFile(path)!=0) return false;

	const SXBSchObj* pObj= bschdoc.findStart(pWnd->m_fi.m_str,pWnd->m_fi.m_bMatchWhole,pWnd->m_fi.m_bCaseSensitive,pWnd->m_fi.m_targetFlag,false);
		
	while(pObj!=NULL){
		CString str;
		int index;

		SFindResult* pResult = new SFindResult();
		pResult->m_docpath = path;
		pResult->m_strText =bschdoc.m_foundString.c_str();
		unsigned int foundType = bschdoc.m_foundType;
		typeString(foundType,pResult->m_strType);
		pResult->m_pt = bschdoc.m_foundPoint;

		pWnd->SendMessage(SEARCH_FIND_MSG,0,(LPARAM)pResult);

		//::Sleep(0);
		if(pWnd->m_abort) break;

		pObj= bschdoc.find(false);
	}



	return true;
}


//指定したフォルダでCE3ファイルを検索する。
//CE3ファイルが見つかるたびに、searchData()を呼び出す。
bool searchFolder(const TCHAR* folder,CCE3SearchDlg* pWnd)
{
	CPath strSearchPath;

	WIN32_FIND_DATA findFileData;
	HANDLE hFindFile;
	bool retval = true;

	//最初にファイルを検索する
	strSearchPath.Combine(folder,_T("*.ce3"));
	//TRACE("Search PATH:%s\n",strSearchPath);
	hFindFile = FindFirstFile(strSearchPath,&findFileData);
	if(hFindFile!=INVALID_HANDLE_VALUE){
		do{
			::Sleep(10);
			if((findFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)==0){
				CPath strFoundPath;
				strFoundPath.Combine(folder,findFileData.cFileName);
				CString ext =strFoundPath.GetExtension();
				//FindFirstFile()では拡張子".ce3$"も引っ掛かるのでフィルタする。
				if(_tcsicmp(ext,_T(".ce3"))==0){
					TRACE("Found : %s\n",strFoundPath);
					searchData(strFoundPath, pWnd);
				}
			}else{
				TRACE("Found dir : %s\n", findFileData.cFileName);
			}
		}while(FindNextFile(hFindFile,&findFileData) && !pWnd->m_abort);
		FindClose(hFindFile);
	}


	if(pWnd->m_fi.m_bIncludeSubfolders){
		//次にフォルダを検索する
		strSearchPath.Combine(folder,_T("*"));
		//TRACE("Search PATH:%s\n",strSearchPath);
		hFindFile = FindFirstFile(strSearchPath,&findFileData);
		if(hFindFile!=INVALID_HANDLE_VALUE){
			::Sleep(10);
			do{
				if((findFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)!=0){
					if(findFileData.cFileName[0]!='.'){//自分や親を除外
						CPath strFoundFolder;
						strFoundFolder.Combine(folder,findFileData.cFileName);
						if(!searchFolder(strFoundFolder,pWnd)){
							retval =  false;
							break;
						}
					}
				}
			}while(FindNextFile(hFindFile,&findFileData) && !pWnd->m_abort);
			FindClose(hFindFile);
		}
	}
	TRACE("END****searchFolder()****\n");
	return retval;
}


UINT SearchThread(LPVOID lpParam)
{
	SearchThreadParam* psparam = (SearchThreadParam*)lpParam;
	searchFolder(psparam->pInitFolder,psparam->pWnd);
	psparam->pWnd->m_searchEnd = true;
	psparam->pWnd->PostMessage(SEARCH_END_MSG);

	TRACE("END****SearchThread(LPVOID lpParam****\n");

	return 0;
}
// ここまで検索スレッド
///////////////////////////////////////////////////////////////////////////////////

void CCE3SearchDlg::OnBnClickedButtonStart()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	if(m_hSearchThread!=0)return;

	disableControls(true);

	m_sparam.pWnd = this;
	GetDlgItemText(IDC_EDIT_FOLDER,m_strSearchFolder);
	m_sparam.pInitFolder = m_strSearchFolder;
	
	GetDlgItemText(IDC_EDIT_TEXT,m_fi.m_str);
	deleteListItems();

	m_abort = false;
	m_searchEnd = false;
	m_pSearchThread = AfxBeginThread(SearchThread,&m_sparam);
	m_hSearchThread = m_pSearchThread->m_hThread;
}



void CCE3SearchDlg::OnBnClickedButtonStop()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	m_abort = true;

}


//検索中は中止以外のコントロールをディセーブルする
void CCE3SearchDlg::disableControls(bool searching)
{
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(searching);

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(!searching);
	GetDlgItem(IDC_BUTTON_REF_FOLDER)->EnableWindow(!searching);
	GetDlgItem(IDC_BUTTON_OPTION)->EnableWindow(!searching);
	GetDlgItem(IDC_BUTTON_EXIT)->EnableWindow(!searching);
	GetDlgItem(IDC_LIST_FOUND)->EnableWindow(!searching);
	GetDlgItem(IDC_EDIT_FOLDER)->EnableWindow(!searching);
	GetDlgItem(IDC_EDIT_TEXT)->EnableWindow(!searching);
}

LRESULT CCE3SearchDlg::OnSearchEnd(UINT wParam,LONG lParam)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	TRACE("CCE3SearchDlg::OnSearchEnd()\n");
	m_searchEnd = false;
	m_hSearchThread = 0;
	disableControls(false);
	doSort();
	return 0;
}




void CCE3SearchDlg::OnBnClickedButtonOption()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
		// TODO: ここにコントロール通知ハンドラ コードを追加します。
	CFindOption dlg;
	//FindInfo fi;
	//IniReadFindInfo(fi);
	//dlg.m_strFind=fi.m_str;
	dlg.m_bFindTag			= ((m_fi.m_targetFlag & FINDTGT_TAG)!=0);
	dlg.m_bFindLabel		= ((m_fi.m_targetFlag & FINDTGT_LABEL)!=0);
	dlg.m_bFindComment		= ((m_fi.m_targetFlag & FINDTGT_COMMENT)!=0);
	dlg.m_bFindName			= ((m_fi.m_targetFlag & FINDTGT_NAME)!=0);
	dlg.m_bFindNameLib		= ((m_fi.m_targetFlag & FINDTGT_NAMELIB)!=0);
	dlg.m_bFindNum			= ((m_fi.m_targetFlag & FINDTGT_NUM)!=0);
	dlg.m_bFindManufacture	= ((m_fi.m_targetFlag & FINDTGT_MANUFACTURE)!=0);
	dlg.m_bFindManufacturePartNumber	= ((m_fi.m_targetFlag & FINDTGT_MANUFACTUREPN)!=0);
	dlg.m_bFindPackage		= ((m_fi.m_targetFlag & FINDTGT_PACKAGE)!=0);
	dlg.m_bFindNote			= ((m_fi.m_targetFlag & FINDTGT_NOTE)!=0);
	dlg.m_bMatchWhole		= m_fi.m_bMatchWhole;
	dlg.m_bCaseSensitive	= m_fi.m_bCaseSensitive;
	dlg.m_bIncludeSubfolders = m_fi.m_bIncludeSubfolders;
	if(dlg.DoModal()==IDOK){
		m_fi.m_targetFlag = 0;
		if(dlg.m_bFindTag)		m_fi.m_targetFlag |=	FINDTGT_TAG;
        if(dlg.m_bFindLabel)	m_fi.m_targetFlag |=	FINDTGT_LABEL;
		if(dlg.m_bFindComment)	m_fi.m_targetFlag |=	FINDTGT_COMMENT;
		if(dlg.m_bFindName)		m_fi.m_targetFlag |=	FINDTGT_NAME;
        if(dlg.m_bFindNameLib)	m_fi.m_targetFlag |=	FINDTGT_NAMELIB;
		if(dlg.m_bFindNum)		m_fi.m_targetFlag |=	FINDTGT_NUM;
		if(dlg.m_bFindManufacture)			m_fi.m_targetFlag |=	FINDTGT_MANUFACTURE;
		if(dlg.m_bFindManufacturePartNumber)m_fi.m_targetFlag |=	FINDTGT_MANUFACTUREPN;
		if(dlg.m_bFindPackage)				m_fi.m_targetFlag |=	FINDTGT_PACKAGE;
		if(dlg.m_bFindNote)					m_fi.m_targetFlag |=  FINDTGT_NOTE;

		//m_fi.m_str = dlg.m_strFind;
		m_fi.m_bMatchWhole = dlg.m_bMatchWhole;
		m_fi.m_bCaseSensitive = dlg.m_bCaseSensitive;
		m_fi.m_bIncludeSubfolders = dlg.m_bIncludeSubfolders;
		IniWriteFindInfo(m_fi);
	}


}

void CCE3SearchDlg::OnBnClickedButtonExit()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	OnCancel();
}

void  CCE3SearchDlg::IniReadFindInfo(FindInfo& fi)
{
	fi.m_str			= AfxGetApp()->GetProfileString(_T("Find"),_T("String"));
	fi.m_targetFlag		= AfxGetApp()->GetProfileInt(_T("Find"),_T("TargetFlag"),0xffffffff);
	fi.m_bMatchWhole	= AfxGetApp()->GetProfileInt(_T("Find"),_T("MatchWhole"),1);
	fi.m_bCaseSensitive = AfxGetApp()->GetProfileInt(_T("Find"),_T("CaseSensitive"),1);
	fi.m_bIncludeSubfolders = AfxGetApp()->GetProfileInt(_T("Find"),_T("IncludeSubfolders"),1);

}

void  CCE3SearchDlg::IniWriteFindInfo(FindInfo& fi)
{
	AfxGetApp()->WriteProfileString(_T("Find"),_T("String"),fi.m_str);
	AfxGetApp()->WriteProfileInt(_T("Find"),_T("TargetFlag"),fi.m_targetFlag);
	AfxGetApp()->WriteProfileInt(_T("Find"),_T("MatchWhole"),fi.m_bMatchWhole);
	AfxGetApp()->WriteProfileInt(_T("Find"),_T("CaseSensitive"),fi.m_bCaseSensitive);
	AfxGetApp()->WriteProfileInt(_T("Find"),_T("IncludeSubfolders"),fi.m_bIncludeSubfolders);

}


void CCE3SearchDlg::setListColumnInfo()
{
	CListCtrl* pLc = (CListCtrl*)GetDlgItem(IDC_LIST_FOUND);

	int nColumnCount = (pLc->GetHeaderCtrl())->GetItemCount();
	if(nColumnCount>=2) return;
	

	LVCOLUMN columnInfo;
	int colWidth[3];
	CRect rcLV;
	pLc->GetClientRect(&rcLV);

	colWidth[0] = rcLV.Width()*40/100;
	colWidth[1] = rcLV.Width()*40/100;
	colWidth[2] = rcLV.Width()*20/100;


	CString columnTitle;


	//int nImage;
		
	::memset(&columnInfo, 0, sizeof(LVCOLUMN));

	columnInfo.mask = LVCF_TEXT|LVCF_WIDTH|LVCF_IMAGE;
	columnInfo.pszText = _T("Path");
	columnInfo.iImage =1;
	columnInfo.cx = colWidth[0];
	pLc->InsertColumn(0, &columnInfo);	


	columnInfo.mask = LVCF_TEXT|LVCF_WIDTH|LVCF_IMAGE;
	columnInfo.pszText = _T("Text");
	columnInfo.iImage =2;
	columnInfo.cx = colWidth[1];
	pLc->InsertColumn(1, &columnInfo);	

	columnInfo.mask = LVCF_TEXT|LVCF_WIDTH|LVCF_IMAGE;
	columnInfo.pszText = _T("Type");
	columnInfo.iImage =2;
	columnInfo.cx = colWidth[2];
	pLc->InsertColumn(2, &columnInfo);	

	setListColumnSortMark();
}


void CCE3SearchDlg::initListCtrlFound()
{

	CListCtrl* pLc = (CListCtrl*)GetDlgItem(IDC_LIST_FOUND);
	pLc->SetExtendedStyle(LVS_EX_FULLROWSELECT);
	(pLc->GetHeaderCtrl())->SetImageList(&m_imageListSortMark);

	setListColumnInfo();
	deleteListItems();
}

//リストコントロールの内容のクリア
void CCE3SearchDlg::deleteListItems()
{
	CListCtrl* pLc = (CListCtrl*)GetDlgItem(IDC_LIST_FOUND);
	int nCount = pLc->GetItemCount();
	for(int i=0;i<nCount;i++){
		SFindResult* pResult = (SFindResult*)pLc->GetItemData(i);
		delete pResult;
	}
	pLc->DeleteAllItems();
}

LRESULT CCE3SearchDlg::OnSearchFind(UINT wParam,LONG lParam)
{
	SFindResult* pResult = (SFindResult*)lParam;
	addFindResult(pResult);
	return 1;
}

//リストコントロールに検索結果を追加する。
void CCE3SearchDlg::addFindResult(SFindResult* pResult)
{
	CListCtrl* pLc = (CListCtrl*)GetDlgItem(IDC_LIST_FOUND);

	int pathColumnWidth = pLc->GetColumnWidth(0);
	m_pathColumnWidth = pathColumnWidth;


	pathColumnWidth = pathColumnWidth-10;

	CDC* pDC = pLc->GetDC();
	
	CFont* pfont = pLc->GetFont();
	//LOGFONT logfont;
	//pfont->GetLogFont(&logfont);
	CFont* pOldFont = pDC->SelectObject(pfont);
	//pfont = pDC->GetCurrentFont();
	//pfont->GetLogFont(&logfont);
	//CRect rcView;
	//pLc->GetClientRect(rcView);
	
	TCHAR pathbuff[MAX_PATH+1];
	_tcscpy_s(pathbuff,pResult->m_docpath);
	::PathCompactPath(pDC->m_hDC,pathbuff,pathColumnWidth);

	LVITEM lvitem;
	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = 0;
	lvitem.iSubItem = 0;
	lvitem.pszText = pathbuff;
	int index = pLc->InsertItem(&lvitem);

	pDC->SelectObject(pOldFont);
	pLc->ReleaseDC(pDC);


	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = index;
	lvitem.iSubItem = 1;
	lvitem.pszText =const_cast<LPTSTR>(static_cast<LPCTSTR>(pResult->m_strText));
	pLc->SetItem(&lvitem);

	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = index;
	lvitem.iSubItem = 2;
	lvitem.pszText =const_cast<LPTSTR>(static_cast<LPCTSTR>(pResult->m_strType));
	pLc->SetItem(&lvitem);

	pLc->SetItemData(index,(DWORD_PTR)pResult);
}

void CCE3SearchDlg::doSort()
{
	CListCtrl* pLc = (CListCtrl*)GetDlgItem(IDC_LIST_FOUND);

	LPARAM sortParam = m_nSortBy;
	if(m_bSortUp){
		sortParam |= 0x100;
	}
	pLc->SortItems(&(CCE3SearchDlg::CompareProc),sortParam);

}

int CALLBACK CCE3SearchDlg::CompareProc(LPARAM lParam1, LPARAM lParam2,LPARAM lParamSort)
{
	SFindResult* pfr1 = (SFindResult*)lParam1;
	SFindResult* pfr2 = (SFindResult*)lParam2;

	int retv;
	
	int sortby = lParamSort & 0xff;

	switch(sortby){
		case 2:
			retv =  _tcscmp(pfr1->m_strType,pfr2->m_strType);
			break;
		case 1:
			retv =  _tcscmp(pfr1->m_strText,pfr2->m_strText);
			break;
		default://case 0:
			retv =  _tcscmp(pfr1->m_docpath,pfr2->m_docpath);
	}

	if((lParamSort&0x100)==0){
		retv = -retv;
	}
	return retv;
}



void CCE3SearchDlg::OnDestroy()
{
	deleteListItems();
	CDialog::OnDestroy();

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	RemoveProp(GetSafeHwnd(), _T("CE3Search-Suigyodo"));
}


//カラムの区切りのドラッグが終了したとき
//
void CCE3SearchDlg::OnHdnEndtrackListFound(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	TRACE("CCE3SearchDlg::OnHdnEndtrackListFound\n");

	CListCtrl* pLc = (CListCtrl*)GetDlgItem(IDC_LIST_FOUND);

	int nCount = pLc->GetItemCount();
	if(nCount >0 && m_pathColumnWidth != pLc->GetColumnWidth(0)){
		int pathColumnWidth = pLc->GetColumnWidth(0);
		m_pathColumnWidth = pathColumnWidth;

		pathColumnWidth = pathColumnWidth-10;


		CDC* pDC = pLc->GetDC();
		CFont* pfont = pLc->GetFont();
		CFont* pOldFont = pDC->SelectObject(pfont);



		for(int i=0;i<nCount;i++){
			SFindResult* pFindResult = (SFindResult*)pLc->GetItemData(i);
			TCHAR pathbuff[MAX_PATH+1];
			_tcscpy_s(pathbuff,pFindResult->m_docpath);
			::PathCompactPath(pDC->m_hDC,pathbuff,pathColumnWidth);
	
			LVITEM lvitem;
			lvitem.mask = LVIF_TEXT;
			lvitem.iItem = i;
			lvitem.iSubItem = 0;
			lvitem.pszText = pathbuff;
			pLc->SetItem(&lvitem);
		}
		pDC->SelectObject(pOldFont);
		pLc->ReleaseDC(pDC);
	}
	*pResult = 0;
}

void CCE3SearchDlg::OnHdnItemclickListFound(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	TRACE("CCE3SearchDlg::OnHdnItemclickListFound(NMHDR *pNMHDR, LRESULT *pResult) %d\n",phdr->iItem);
	if(phdr->iItem == m_nSortBy){
		m_bSortUp = !m_bSortUp;
	}else{
		m_nSortBy = phdr->iItem;
		m_bSortUp = true;
	}

	setListColumnSortMark();

	doSort();

	*pResult = 0;
}


void CCE3SearchDlg::setListColumnSortMark()
{
	CListCtrl* pLc = (CListCtrl*)GetDlgItem(IDC_LIST_FOUND);
	CHeaderCtrl* pHc = pLc->GetHeaderCtrl();
	int nColumnCount = pHc->GetItemCount();
	if(nColumnCount<3)return;
	HDITEM hdi;
	int nImage;

	hdi.mask = HDI_IMAGE;

	for(int i=0;i<3;i++){
		if(m_nSortBy != i){
			nImage = 2;
		}else if(m_bSortUp){
			nImage = 1;
		}else{
			nImage = 0;
		}
		hdi.iImage = nImage;
		pHc->SetItem(i,&hdi);
	}

}

void CCE3SearchDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラ コードを追加します。
	CWnd* pWnd = GetDlgItem(IDC_LIST_FOUND);
	if(!pWnd || !pWnd->GetSafeHwnd())return;
	CRect rc;
	pWnd->GetWindowRect(&rc);
	this->ScreenToClient(&rc);
	int margin = rc.left;
	rc.right = cx-margin;
	rc.bottom = cy-margin;
	pWnd->MoveWindow(&rc);
}

struct EnumWindowsProcCheckOpenedParam{
	const TCHAR* pcszPathName;
	//HWND hMyWnd;
    HWND hFindWnd;
};


#define COPYDATA_CHECKOPENFILE 1

BOOL CALLBACK CCE3SearchDlg::EnumWindowsProcCheckOpened(HWND hwnd, LPARAM lParam)
{
	EnumWindowsProcCheckOpenedParam* pParam = (EnumWindowsProcCheckOpenedParam*)lParam;

	//見つかったウィンドウがBSch3Vでなければ何もしない。
	if(!::GetProp(hwnd, _T("BSch3V-Suigyodo"))) return TRUE;


	COPYDATASTRUCT cd;
	cd.dwData = COPYDATA_CHECKOPENFILE;
	cd.lpData = (PVOID)pParam->pcszPathName;
	cd.cbData = (_tcslen(pParam->pcszPathName)+1)*sizeof(wchar_t); 

	if (::SendMessage(hwnd, WM_COPYDATA, NULL, (LPARAM)&cd)){
		//該当ウィンドウが見つかったら、ウィンドウハンドルを保存して列挙を中断
		pParam->hFindWnd = hwnd;
		return FALSE;
	}
    return TRUE;
}

struct EnumWindowsProcIDParam{
	DWORD procID;
    HWND hFindWnd;
};


//プロセスIDからウィンドウのハンドルを取得するのに使う
BOOL CALLBACK CCE3SearchDlg::EnumWindowsProcID(HWND hwnd, LPARAM lParam)
{
	EnumWindowsProcIDParam* pParam = (EnumWindowsProcIDParam*)lParam;

	//見つかったウィンドウがBSch3Vでなければ何もしない。
	if(!::GetProp(hwnd, _T("BSch3V-Suigyodo"))) return TRUE;

	DWORD procID;
	::GetWindowThreadProcessId(hwnd,&procID);
	//HANDLE hProc = OpenProcess( PROCESS_QUERY_INFORMATION,FALSE,procID);

	if(procID == pParam->procID){
		pParam->hFindWnd = hwnd;
		return FALSE;
	}
    return TRUE;
}



//指定パスのファイルを開いているBSch3Vをチェックする。
CWnd* CCE3SearchDlg::CheckOpened(const TCHAR* pcszPathName)
{
	EnumWindowsProcCheckOpenedParam coParam;
	coParam.pcszPathName = pcszPathName;
	//coParam.hMyWnd = GetSafeHwnd();
	coParam.hFindWnd = NULL;
	EnumWindows(EnumWindowsProcCheckOpened,(LPARAM)&coParam);
    return CWnd::FromHandle(coParam.hFindWnd);
}


void CCE3SearchDlg::OpenAndScrollTo(const TCHAR* pcszPathName, CPoint pt)
{
	CWnd* pWndOpened = CheckOpened(pcszPathName);
	if(pWndOpened){
		TRACE("Already opened.\n");
		pWndOpened->SetForegroundWindow();
	}else{
		//指定パスのファイルを開いていない場合はBSch3Vを起動して開く
		TCHAR bschPath[_MAX_PATH];
		TCHAR drive[_MAX_DRIVE];
		TCHAR dir[_MAX_DIR];
		::GetModuleFileName(NULL,bschPath,_MAX_PATH);
		::_tsplitpath(bschPath,drive,dir,NULL,NULL);
		::_tmakepath(bschPath,drive,dir,_T("bsch3v"),_T(".exe"));

		CString strCmdline;
		strCmdline.Format(_T("\"%s\" \"%s\""),bschPath,pcszPathName);

		TCHAR* cmdline = new TCHAR[_tcslen(strCmdline)+1];
		_tcscpy(cmdline,strCmdline);

		PROCESS_INFORMATION pi;
		STARTUPINFO si;

		ZeroMemory(&pi, sizeof(pi));
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);

		BOOL resProc = CreateProcess(NULL, cmdline, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
		delete[]cmdline;

		if(!resProc){
			return;
		}
		DWORD waitResultErr = ::WaitForInputIdle(pi.hProcess,5000);
		if(waitResultErr) return;

		//プロセスIDからウィンドウのハンドルを取得する
		EnumWindowsProcIDParam param;
		param.procID = pi.dwProcessId;
		param.hFindWnd = 0;
		EnumWindows(EnumWindowsProcID,(LPARAM)&param);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		if(!param.hFindWnd){
			return;
		}
		pWndOpened = CWnd::FromHandle(param.hFindWnd);
	}
	if(pWndOpened){
		LONG lParam = MAKELONG(pt.x,pt.y);
		pWndOpened->SendMessage(SCROLL_TO_MSG,0,lParam);
	}
}

void CCE3SearchDlg::OnLvnItemchangedListFound(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	
	//CListCtrl* pLc = (CListCtrl*)GetDlgItem(IDC_LIST_FOUND);
	//int nSel = pLc ->GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	//if(nSel>=0 && nSel != m_nSelected){
	//	m_nSelected = nSel;
	//	TRACE("SelectChanged  %d\n",m_nSelected);
	//	SFindResult* pfr = (SFindResult*)pLc->GetItemData(nSel);
	//	CPoint pt = CPoint(pfr->m_pt.x(),pfr->m_pt.y());
	//	OpenAndScrollTo(pfr->m_docpath, pt);
	//}

	*pResult = 0;
}

void CCE3SearchDlg::OnNMClickListFound(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	TRACE("CCE3SearchDlg::OnNMClickListFound(NMHDR *pNMHDR, LRESULT *pResult)\n");

	CListCtrl* pLc = (CListCtrl*)GetDlgItem(IDC_LIST_FOUND);
	int nSel = pLc ->GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if(nSel>=0){// && nSel != m_nSelected){
		m_nSelected = nSel;
		SFindResult* pfr = (SFindResult*)pLc->GetItemData(nSel);
		CPoint pt = CPoint(pfr->m_pt.x(),pfr->m_pt.y());
		OpenAndScrollTo(pfr->m_docpath, pt);
	}

	*pResult = 0;
}


////////////////////////////////////////////////////////////////////////////////////
//起動中の他のCE3Searchの検索パスのチェックにかかわる関数群

#define COPYDATA_CHECK_SEARCH_PATH 1

struct EnumWindowsCheckSearchPathParam{
	const TCHAR* pcszPathName;
	HWND hMyWnd;
    HWND hFindWnd;
};


BOOL CALLBACK CCE3SearchDlg::EnumWindowsCheckSearchPath(HWND hwnd, LPARAM lParam)
{
	EnumWindowsCheckSearchPathParam* pParam = (EnumWindowsCheckSearchPathParam*)lParam;

	//見つかったウィンドウが自分自身なら何もしない
	if(pParam->hMyWnd == hwnd) return TRUE;

	//見つかったウィンドウが CE3Search でなければ何もしない。
	if(!::GetProp(hwnd, _T("CE3Search-Suigyodo"))) return TRUE;


	//見つかったウィンドウに検索パスを渡して、そのウィンドウの検索パスと一致するかどうか判断させる
	COPYDATASTRUCT cd;
	cd.dwData = COPYDATA_CHECK_SEARCH_PATH;
	cd.lpData = (PVOID)pParam->pcszPathName;
	cd.cbData = _tcslen(pParam->pcszPathName)+1; 
	if (::SendMessage(hwnd, WM_COPYDATA, NULL, (LPARAM)&cd)){
		//該当ウィンドウが見つかったら、ウィンドウハンドルを保存して列挙を中断
		pParam->hFindWnd = hwnd;
		return FALSE;
	}
    return TRUE;
}


CWnd* CCE3SearchDlg::CheckOtherCE3SearchPath(const TCHAR* pcszPathName)
{
	EnumWindowsCheckSearchPathParam cspParam;
	cspParam.pcszPathName = pcszPathName;
	cspParam.hMyWnd = GetSafeHwnd();
	cspParam.hFindWnd = NULL;
	EnumWindows(EnumWindowsCheckSearchPath,(LPARAM)&cspParam);
    return CWnd::FromHandle(cspParam.hFindWnd);
}

BOOL CCE3SearchDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	if(pCopyDataStruct->dwData == COPYDATA_CHECK_SEARCH_PATH)
	{
		CString strPathName; 
		GetDlgItemText(IDC_EDIT_FOLDER,strPathName);

		return (strPathName==(LPCTSTR)pCopyDataStruct->lpData);
	}
	return CWnd::OnCopyData(pWnd, pCopyDataStruct);
}