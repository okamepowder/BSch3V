// DlgObjFilter.cpp : 実装ファイル
//

#include "stdafx.h"
#include "DlgObjFilter.h"
#include "ObjFilter.h"




// CDlgObjFilter ダイアログ

IMPLEMENT_DYNAMIC(CDlgObjFilter, CDialog)

CDlgObjFilter::CDlgObjFilter(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgObjFilter::IDD, pParent)
{
	m_imageListButton.Create(IDB_OBJFILTER,16,1,RGB(0xC0,0xC0,0xC0));
	m_xpos = 100000;
	m_ypos = 100000;

	
}

CDlgObjFilter::~CDlgObjFilter()
{
}

void CDlgObjFilter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_F_PART, m_buttonComponent);
}


BEGIN_MESSAGE_MAP(CDlgObjFilter, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CDlgObjFilter::OnCancel)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_F_PART, &CDlgObjFilter::OnBnClickedComponent)
	ON_BN_CLICKED(IDC_F_BUS, &CDlgObjFilter::OnBnClickedBus)
	ON_BN_CLICKED(IDC_F_WIRE, &CDlgObjFilter::OnBnClickedWire)
	ON_BN_CLICKED(IDC_F_DASH, &CDlgObjFilter::OnBnClickedDash)
	ON_BN_CLICKED(IDC_F_MARKER, &CDlgObjFilter::OnBnClickedMarker)
	ON_BN_CLICKED(IDC_F_JUNCTION, &CDlgObjFilter::OnBnClickedJunction)
	ON_BN_CLICKED(IDC_F_BENTRY, &CDlgObjFilter::OnBnClickedBEntry)
	ON_BN_CLICKED(IDC_F_WENTRY, &CDlgObjFilter::OnBnClickedWEntry)
	ON_BN_CLICKED(IDC_F_TAG, &CDlgObjFilter::OnBnClickedTag)
	ON_BN_CLICKED(IDC_F_LABEL, &CDlgObjFilter::OnBnClickedLabel)
	ON_BN_CLICKED(IDC_F_COMMENT, &CDlgObjFilter::OnBnClickedComment)
	ON_BN_CLICKED(IDC_F_IMAGE, &CDlgObjFilter::OnBnClickedImage)
END_MESSAGE_MAP()


// CDlgObjFilter メッセージ ハンドラ

BOOL CDlgObjFilter::OnInitDialog()
{
	//HBITMAP hBmp;
	//hBmp=(HBITMAP)::LoadImage(AfxGetApp()->m_hInstance,MAKEINTRESOURCE (IDB_EDIT),IMAGE_BITMAP,0,0,LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS);

	//CWindowDC dc(this);
	//CDialog::OnInitDialog();
	//// TODO:  ここに初期化を追加してください

	//CDC dcMem;
	//dcMem.CreateCompatibleDC (&dc);

	//CBitmap bmp;
	//bmp.CreateCompatibleBitmap (&dc,16,16);
	//CBitmap* pBmpOld = dcMem.SelectObject(&bmp);

	//m_imageListButton.Draw(&dcMem,6,CPoint(0,0),ILD_NORMAL);
	//
	//dcMem.SelectObject(pBmpOld);
	

	m_buttonComponent.SubclassDlgItem(IDC_F_PART, this); 
	m_buttonBus.SubclassDlgItem(IDC_F_BUS, this); 
	m_buttonWire.SubclassDlgItem(IDC_F_WIRE, this); 
	m_buttonDash.SubclassDlgItem(IDC_F_DASH, this); 
	m_buttonMarker.SubclassDlgItem(IDC_F_MARKER, this); 
	m_buttonJunction.SubclassDlgItem(IDC_F_JUNCTION, this); 
	m_buttonBEntry.SubclassDlgItem(IDC_F_BENTRY, this); 
	m_buttonWEntry.SubclassDlgItem(IDC_F_WENTRY, this); 
	m_buttonTag.SubclassDlgItem(IDC_F_TAG, this); 
	m_buttonLabel.SubclassDlgItem(IDC_F_LABEL, this); 
	m_buttonComment.SubclassDlgItem(IDC_F_COMMENT, this); 
	m_buttonImage.SubclassDlgItem(IDC_F_IMAGE, this); 


	m_buttonComponent.SetImage(&m_imageListButton,0);
	m_buttonBus.SetImage(&m_imageListButton,1);
	m_buttonWire.SetImage(&m_imageListButton,2); 
	m_buttonDash.SetImage(&m_imageListButton,3); 
	m_buttonMarker.SetImage(&m_imageListButton,4); 
	m_buttonJunction.SetImage(&m_imageListButton,5); 
	m_buttonBEntry.SetImage(&m_imageListButton,6); 
	m_buttonWEntry.SetImage(&m_imageListButton,7);
	m_buttonTag.SetImage(&m_imageListButton,8);
	m_buttonLabel.SetImage(&m_imageListButton,9);
	m_buttonComment.SetImage(&m_imageListButton,10);
	m_buttonImage.SetImage(&m_imageListButton,11);
	

	m_buttonComponent.SetCheck(((*m_pObjectFilterValue) & OBJMASK_COMPONENT) ? 1 : 0);
	m_buttonBus.SetCheck(((*m_pObjectFilterValue) & OBJMASK_BUS) ? 1 : 0);
	m_buttonWire.SetCheck(((*m_pObjectFilterValue) & OBJMASK_WIRE) ? 1 : 0);
	m_buttonDash.SetCheck(((*m_pObjectFilterValue) & OBJMASK_DASH) ? 1 : 0);
	m_buttonMarker.SetCheck(((*m_pObjectFilterValue) & OBJMASK_MARKER) ? 1 : 0);
	m_buttonJunction.SetCheck(((*m_pObjectFilterValue) & OBJMASK_JUNCTION) ? 1 : 0);
	m_buttonBEntry.SetCheck(((*m_pObjectFilterValue) & OBJMASK_BENTRY) ? 1 : 0);
	m_buttonWEntry.SetCheck(((*m_pObjectFilterValue) & OBJMASK_WENTRY) ? 1 : 0);
	m_buttonTag.SetCheck(((*m_pObjectFilterValue) & OBJMASK_TAG) ? 1 : 0);
	m_buttonLabel.SetCheck(((*m_pObjectFilterValue) & OBJMASK_LABEL) ? 1 : 0);
	m_buttonComment.SetCheck(((*m_pObjectFilterValue) & OBJMASK_COMMENT) ? 1 : 0);
	m_buttonImage.SetCheck(((*m_pObjectFilterValue) & OBJMASK_IMAGE) ? 1 : 0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CDlgObjFilter::ShowDialog()
{
	this->Create(IDD);

	if(m_xpos<100000 && m_ypos<100000){
		CRect rc;
		GetWindowRect(&rc);
		int width = rc.Width();
		int height = rc.Height();
		MoveWindow(m_xpos,m_ypos,width,height);
	}
	this->ShowWindow(SW_SHOW);
}

void CDlgObjFilter::OnCancel()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	//OnCancel();
	if(m_pObjectFilterValue!=NULL){
		(*m_pObjectFilterValue) &= ~OBJMASK_USEFILTER;
	}
	DestroyWindow();
}

void CDlgObjFilter::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	//CDialog::OnOK();
}

void CDlgObjFilter::OnDestroy()
{
	CRect rc;
	GetWindowRect(&rc);

	m_xpos = rc.left;
	m_ypos = rc.top;

	CDialog::OnDestroy();

	// TODO: ここにメッセージ ハンドラ コードを追加します。
}

void CDlgObjFilter::OnBnClickedComponent()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	(*m_pObjectFilterValue) ^= OBJMASK_COMPONENT;
	m_buttonComponent.SetCheck(((*m_pObjectFilterValue) & OBJMASK_COMPONENT) ? 1 : 0);
}

void CDlgObjFilter::OnBnClickedBus()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	(*m_pObjectFilterValue) ^= OBJMASK_BUS;
	m_buttonBus.SetCheck(((*m_pObjectFilterValue) & OBJMASK_BUS) ? 1 : 0);
}

void CDlgObjFilter::OnBnClickedWire()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	(*m_pObjectFilterValue) ^= OBJMASK_WIRE;
	m_buttonWire.SetCheck(((*m_pObjectFilterValue) & OBJMASK_WIRE) ? 1 : 0);
}

void CDlgObjFilter::OnBnClickedDash()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	(*m_pObjectFilterValue) ^= OBJMASK_DASH;
	m_buttonDash.SetCheck(((*m_pObjectFilterValue) & OBJMASK_DASH) ? 1 : 0);
}

void CDlgObjFilter::OnBnClickedMarker()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	(*m_pObjectFilterValue) ^= OBJMASK_MARKER;
	m_buttonMarker.SetCheck(((*m_pObjectFilterValue) & OBJMASK_MARKER) ? 1 : 0);
}

void CDlgObjFilter::OnBnClickedJunction()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	(*m_pObjectFilterValue) ^= OBJMASK_JUNCTION;
	m_buttonJunction.SetCheck(((*m_pObjectFilterValue) & OBJMASK_JUNCTION) ? 1 : 0);
}

void CDlgObjFilter::OnBnClickedBEntry()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	(*m_pObjectFilterValue) ^= OBJMASK_BENTRY;
	m_buttonBEntry.SetCheck(((*m_pObjectFilterValue) & OBJMASK_BENTRY) ? 1 : 0);
}

void CDlgObjFilter::OnBnClickedWEntry()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	(*m_pObjectFilterValue) ^= OBJMASK_WENTRY;
	m_buttonWEntry.SetCheck(((*m_pObjectFilterValue) & OBJMASK_WENTRY) ? 1 : 0);
}

void CDlgObjFilter::OnBnClickedTag()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	(*m_pObjectFilterValue) ^= OBJMASK_TAG;
	m_buttonTag.SetCheck(((*m_pObjectFilterValue) & OBJMASK_TAG) ? 1 : 0);
}

void CDlgObjFilter::OnBnClickedLabel()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	(*m_pObjectFilterValue) ^= OBJMASK_LABEL;
	m_buttonLabel.SetCheck(((*m_pObjectFilterValue) & OBJMASK_LABEL) ? 1 : 0);
}

void CDlgObjFilter::OnBnClickedComment()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	(*m_pObjectFilterValue) ^= OBJMASK_COMMENT;
	m_buttonComment.SetCheck(((*m_pObjectFilterValue) & OBJMASK_COMMENT) ? 1 : 0);
}

void CDlgObjFilter::OnBnClickedImage()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	(*m_pObjectFilterValue) ^= OBJMASK_IMAGE;
	m_buttonImage.SetCheck(((*m_pObjectFilterValue) & OBJMASK_IMAGE) ? 1 : 0);
}