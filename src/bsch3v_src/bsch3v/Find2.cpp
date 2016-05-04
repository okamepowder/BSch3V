// Find2.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Find2.h"
#include "FindOption.h"


// CFind2 ダイアログ

IMPLEMENT_DYNAMIC(CFind2, CDialog)

#define DLG_POS_INITVAL 100000

CFind2::CFind2(CWnd* pParent /*=NULL*/)
	: CDialog(CFind2::IDD, pParent)
{
	m_xpos = DLG_POS_INITVAL;
	m_ypos = DLG_POS_INITVAL;
	m_mincx = 100;	//適当な値
	m_mincy = 100;	//適当な値
	m_colWidth[0]=-1;

	m_pView = NULL;

	m_imageListSortMark.Create(IDB_BITMAP_SORTMARK,16,1,RGB(0xFF,0xFF,0xFF));
	
	m_str_refnum.LoadString(IDS_FINDTYPE_REFNUM);
	m_str_value.LoadString(IDS_FINDTYPE_VALUE);
	m_str_mfr.LoadString(IDS_FINDTYPE_MFR);
	m_str_mfrpn.LoadString(IDS_FINDTYPE_MFRPN);
	m_str_pkg.LoadString(IDS_FINDTYPE_PACKAGE);
	m_str_note.LoadString(IDS_FINDTYPE_NOTE);
	m_str_tag.LoadString(IDS_FINDTYPE_TAG);
	m_str_label.LoadString(IDS_FINDTYPE_Label);
	m_str_comment.LoadString(IDS_FINDTYPE_COMMENT);
	m_str_nameinlib.LoadString(IDS_FINDTYPE_NAMEINLIB);
	m_nSortBy = 0;
	m_bSortUp = true;
	//m_resultArray.clear();
	m_nSelected = -1;
}

CFind2::~CFind2()
{
}

void CFind2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFind2, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_OPTION, &CFind2::OnBnClickedButtonOption)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CFind2::OnBnClickedButtonSearch)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_FOUND, &CFind2::OnLvnColumnclickListFound)
//	ON_NOTIFY(LVN_ODSTATECHANGED, IDC_LIST_FOUND, &CFind2::OnLvnOdstatechangedListFound)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_FOUND, &CFind2::OnLvnItemchangedListFound)
ON_WM_SIZE()
ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


// CFind2 メッセージ ハンドラ

BOOL CFind2::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	HICON hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(hIcon, FALSE);		// 小さいアイコンを設定

	initListCtrlFound();

	setCtrlSize();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CFind2::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	//CDialog::OnOK();
}


void CFind2::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	DestroyWindow();
	//CDialog::OnCancel();
}

void CFind2::ShowDialog()
{
	m_pView->IniReadFindInfo(m_fi);

	this->Create(IDD);

	CRect rc;
	GetWindowRect(&rc);
	int width = rc.Width();
	int height = rc.Height();

	if(m_xpos == DLG_POS_INITVAL){
		m_xpos = rc.left;
		m_ypos = rc.top;
		m_mincx = width;
		m_mincy = height;
	}else{
		MoveWindow(m_xpos,m_ypos,m_cx,m_cy);
	}
	this->ShowWindow(SW_SHOW);
}

void CFind2::OnDestroy()
{
	CRect rc;
	GetWindowRect(&rc);
	deleteListItems();

	m_xpos = rc.left;
	m_ypos = rc.top;
	m_cx = rc.Width();
	m_cy = rc.Height();

	CListCtrl* pLc = (CListCtrl*)GetDlgItem(IDC_LIST_FOUND);
	LVCOLUMN columnInfo;
	columnInfo.mask = LVCF_WIDTH;
	for(int i=0;i<2;i++){
		pLc->GetColumn(i,&columnInfo);
		m_colWidth[i]=columnInfo.cx;
	}




	m_pView->OnCloseFind2Dialog();

	CDialog::OnDestroy();
}


void CFind2::setListColumnSortMark()
{
	CListCtrl* pLc = (CListCtrl*)GetDlgItem(IDC_LIST_FOUND);
	CHeaderCtrl* pHc = pLc->GetHeaderCtrl();
	int nColumnCount = pHc->GetItemCount();
	if(nColumnCount<2)return;
	HDITEM hdi;
	int nImage;

	hdi.mask = HDI_IMAGE;

	if(m_nSortBy != 0){
		nImage = 2;
	}else if(m_bSortUp){
		nImage = 1;
	}else{
		nImage = 0;
	}
	hdi.iImage = nImage;
	pHc->SetItem(0,&hdi);

	if(m_nSortBy != 1){
		nImage = 2;
	}else if(m_bSortUp){
		nImage = 1;
	}else{
		nImage = 0;
	}
	hdi.iImage = nImage;
	pHc->SetItem(1,&hdi);

}


void CFind2::setListColumnInfo()
{
	CListCtrl* pLc = (CListCtrl*)GetDlgItem(IDC_LIST_FOUND);

	int nColumnCount = (pLc->GetHeaderCtrl())->GetItemCount();
	if(nColumnCount>=2) return;
	

	LVCOLUMN columnInfo;
	int colWidth[2];
	CRect rcLV;
	pLc->GetClientRect(&rcLV);

	if(m_colWidth[0]>0){
		colWidth[0] = m_colWidth[0];
		colWidth[1] = m_colWidth[1];
	}else{
		colWidth[0] = rcLV.Width()*55/100;
		colWidth[1] = rcLV.Width()*35/100;
	}


	CString columnTitle;


	//int nImage;
		
	::memset(&columnInfo, 0, sizeof(LVCOLUMN));

	columnInfo.mask = LVCF_TEXT|LVCF_WIDTH|LVCF_IMAGE;
	columnTitle.LoadString(IDS_FIND2_COLUMN_TEXT);
	columnInfo.pszText = const_cast<LPTSTR>(static_cast<LPCTSTR>(columnTitle));
	columnInfo.iImage =1;
	columnInfo.cx = colWidth[0];

	pLc->InsertColumn(0, &columnInfo);	

	columnTitle.LoadString(IDS_FIND2_COLUMN_TYPE);
	columnInfo.pszText = const_cast<LPTSTR>(static_cast<LPCTSTR>(columnTitle));
	columnInfo.cx = colWidth[1];
	columnInfo.iImage =2;

	pLc->InsertColumn(1, &columnInfo);

	setListColumnSortMark();
}


void CFind2::initListCtrlFound()
{

	CListCtrl* pLc = (CListCtrl*)GetDlgItem(IDC_LIST_FOUND);
	pLc->SetExtendedStyle(LVS_EX_FULLROWSELECT);
	(pLc->GetHeaderCtrl())->SetImageList(&m_imageListSortMark);

	setListColumnInfo();
}


void CFind2::OnBnClickedButtonOption()
{
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

		m_pView->IniWriteFindInfo(m_fi);
	}

}


void CFind2::deleteListItems()
{
	CListCtrl* pLc = (CListCtrl*)GetDlgItem(IDC_LIST_FOUND);
	int nCount = pLc->GetItemCount();
	for(int i=0;i<nCount;i++){
		SFindResult* pResult = (SFindResult*)pLc->GetItemData(i);
		delete pResult;
	}
	pLc->DeleteAllItems();
	
}

LPTSTR CFind2::typeString(unsigned int foundType)
{
	const TCHAR* pval = NULL;
	switch(foundType){
		case FINDTGT_TAG: pval = m_str_tag; break;
		case FINDTGT_LABEL: pval = m_str_label; break;
		case FINDTGT_COMMENT: pval = m_str_comment; break;
		case FINDTGT_NAME: pval = m_str_value; break;
		case FINDTGT_NAMELIB: pval = m_str_nameinlib; break;
		case FINDTGT_NUM: pval = m_str_refnum; break;
		case FINDTGT_MANUFACTURE: pval = m_str_mfr; break;
		case FINDTGT_MANUFACTUREPN: pval = m_str_mfrpn; break;
		case FINDTGT_PACKAGE: pval = m_str_pkg; break;
		case FINDTGT_NOTE: pval = m_str_note; break;
		default:pval = _T("");
	}
	return const_cast<LPTSTR>(static_cast<LPCTSTR>(pval));
}

void CFind2::OnBnClickedButtonSearch()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	deleteListItems();
	m_nSelected = -1;

	CListCtrl* pLc = (CListCtrl*)GetDlgItem(IDC_LIST_FOUND);
	CString strSearch;
	GetDlgItemText(IDC_EDIT_TEXT,strSearch);
	if(strSearch.IsEmpty() || m_fi.m_targetFlag==0) return;
	//m_fi.m_str = strSearch;

	CBSchDoc* pDoc = m_pView->GetDocument();

	int setIndex = 0;
	unsigned int foundType;
	const SXBSchObj* pObj= pDoc->FindStart(strSearch,m_fi.m_bMatchWhole,m_fi.m_bCaseSensitive,m_fi.m_targetFlag,false);
	while(pObj!=NULL){
		CString str;
		int index;

		SFindResult* pResult = new SFindResult();

		LVITEM lvitem;
		pDoc->GetFoundText(str);
		lvitem.mask = LVIF_TEXT;
		lvitem.iItem = setIndex;
		lvitem.iSubItem = 0;
		lvitem.pszText =const_cast<LPTSTR>(static_cast<LPCTSTR>(str));
		index = pLc->InsertItem(&lvitem);
		pResult->m_strText = lvitem.pszText;

		unsigned int foundType = pDoc->GetFountType();
		lvitem.mask = LVIF_TEXT;
		lvitem.iItem = index;
		lvitem.iSubItem = 1;
		lvitem.pszText =typeString(foundType);
		pLc->SetItem(&lvitem);
		pResult->m_strType = lvitem.pszText;

		pResult->m_pt = pDoc->GetFoundPoint();

		pLc->SetItemData(index,(DWORD_PTR)pResult);

		pObj= pDoc->Find();
	}
	doSort();
	m_pView->updateBuff();
	m_pView->Invalidate();

}

void CFind2::OnLvnColumnclickListFound(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	int sortby = pNMLV->iSubItem;
	if(m_nSortBy == sortby){
		m_bSortUp = !m_bSortUp;
	}else{
		m_bSortUp = true;
	}
	m_nSortBy = sortby;

	setListColumnSortMark();


	doSort();



	*pResult = 0;
}


void CFind2::doSort()
{
	CListCtrl* pLc = (CListCtrl*)GetDlgItem(IDC_LIST_FOUND);
	LPARAM sortParam = 0;
	if(m_nSortBy == 1){
		sortParam |= 2;
	}
	if(m_bSortUp){
		sortParam |= 1;
	}
	pLc->SortItems(&(CFind2::CompareProc),sortParam);

}


int CALLBACK CFind2::CompareProc(LPARAM lParam1, LPARAM lParam2,LPARAM lParamSort)
{
	SFindResult* pfr1 = (SFindResult*)lParam1;
	SFindResult* pfr2 = (SFindResult*)lParam2;

	int retv;

	if((lParamSort&2)==0){
		retv =  _tcscmp(pfr1->m_strText,pfr2->m_strText);
	}else{
		retv =  _tcscmp(pfr1->m_strType,pfr2->m_strType);
	}
	if((lParamSort&1)==0){
		retv = -retv;
	}
	return retv;
}


void CFind2::OnLvnItemchangedListFound(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラ コードを追加します。

	CListCtrl* pLc = (CListCtrl*)GetDlgItem(IDC_LIST_FOUND);
	int nSel = pLc ->GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if(nSel>=0 && nSel != m_nSelected){
		m_nSelected = nSel;
		TRACE("SelectChanged  %d\n",m_nSelected);
		SFindResult* pfr = (SFindResult*)pLc->GetItemData(nSel);
		CPoint pt = CPoint(pfr->m_pt.x(),pfr->m_pt.y());
		m_pView->ScrollToFoundPos(pt);
	}

	*pResult = 0;
}

void CFind2::setCtrlSize()
{
	CWnd* pCtrl = GetDlgItem(IDC_LIST_FOUND);
	if(!pCtrl->GetSafeHwnd())return;
	CRect rcClient;
	CRect rcCtrl;
	GetClientRect(&rcClient);

	pCtrl->GetWindowRect(&rcCtrl);
	this->ScreenToClient(&rcCtrl);
	rcCtrl.right = rcClient.right-rcCtrl.left;
	rcCtrl.bottom = rcClient.bottom-rcCtrl.left;
	pCtrl->MoveWindow(&rcCtrl);

	pCtrl = GetDlgItem(IDC_EDIT_TEXT);
	pCtrl->GetWindowRect(&rcCtrl);
	this->ScreenToClient(&rcCtrl);
	rcCtrl.right = rcClient.right-rcCtrl.left;
	pCtrl->MoveWindow(&rcCtrl);



}


void CFind2::OnSize(UINT nType, int cx, int cy)
{
	setCtrlSize();
	CDialog::OnSize(nType, cx, cy);
}

void CFind2::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	if(m_xpos != DLG_POS_INITVAL){	//m_mincxが設定済み
		lpMMI->ptMinTrackSize = CPoint(m_mincx,m_mincy);
	}

	CDialog::OnGetMinMaxInfo(lpMMI);
}
