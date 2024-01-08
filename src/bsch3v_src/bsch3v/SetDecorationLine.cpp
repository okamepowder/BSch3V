// SetDecorationLine.cpp : 実装ファイル
//

#include "stdafx.h"
#include "BSch.h"
#include "SetDecorationLine.h"
#include "xbschdashline.h"


// CSetDecorationLine ダイアログ

IMPLEMENT_DYNAMIC(CSetDecorationLine, CDialog)

CSetDecorationLine::CSetDecorationLine(CWnd* pParent /*=NULL*/)
	: CDialog(CSetDecorationLine::IDD, pParent)
{
	m_bCurve = false;
	m_lineStyle = SXBSchDash::LINESTYLE_DASH;
	m_startPointStyle = SXBSchDash::ENDPOINTSTYLE_NORMAL;
	m_endPointStyle = SXBSchDash::ENDPOINTSTYLE_NORMAL;
	m_lineWidth = SXBSchDash::MIN_WIDTH;
	m_endMarkSize  = SXBSchDash::MIN_ENDMARKSIZE;

	m_imagelistLineStyle.Create(IDB_BITMAP_DECOLINESTYLE,32,1,RGB(0xC0,0xC0,0xC0));
	m_imagelistEndpointStyle.Create(IDB_BITMAP_LINEENDSTYLE,16,1,RGB(0xC0,0xC0,0xC0));
}

CSetDecorationLine::~CSetDecorationLine()
{
}

void CSetDecorationLine::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSetDecorationLine, CDialog)
END_MESSAGE_MAP()

int CSetDecorationLine::addItem(CComboBoxEx* pCbox,int index,int imageIndex)
{
	COMBOBOXEXITEM item;
	item.mask = CBEIF_IMAGE|CBEIF_SELECTEDIMAGE|CBEIF_TEXT;
    item.iItem = index;
    item.pszText = _T(" ");
    item.cchTextMax = 0;
    item.iImage = imageIndex;
    item.iSelectedImage = imageIndex;
    item.iOverlay = 0;
    item.iIndent =0;
    item.lParam=0;

	return pCbox->InsertItem(&item);
}


// CSetDecorationLine メッセージ ハンドラ

BOOL CSetDecorationLine::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ここに初期化を追加してください
	CSpinButtonCtrl* pSpin;
	pSpin=(CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_LINEWIDTH);
	pSpin->SetRange(SXBSchDash::MIN_WIDTH,SXBSchDash::MAX_WIDTH);
	pSpin=(CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_ENDMARKSIZE);
	pSpin->SetRange(SXBSchDash::MIN_ENDMARKSIZE,SXBSchDash::MAX_ENDMARKSIZE);

	CComboBoxEx* pCbox;
	pCbox = (CComboBoxEx*)GetDlgItem(IDC_COMBOBOXEX_LINESTYLE);
	pCbox->SetImageList(&m_imagelistLineStyle);
	for(int i=0;i<4;i++){
		addItem(pCbox,i,i);
	}
	pCbox->SetCurSel(m_lineStyle);


	pCbox = (CComboBoxEx*)GetDlgItem(IDC_COMBOBOXEX_STARTPOINT);
	pCbox->SetImageList(&m_imagelistEndpointStyle);
	addItem(pCbox,0,0);
	addItem(pCbox,1,1);
	addItem(pCbox,2,3);
	pCbox->SetCurSel(m_startPointStyle);


	pCbox = (CComboBoxEx*)GetDlgItem(IDC_COMBOBOXEX_ENDPOINT);
	pCbox->SetImageList(&m_imagelistEndpointStyle);
	addItem(pCbox,0,0);
	addItem(pCbox,1,2);
	addItem(pCbox,2,4);
	pCbox->SetCurSel(m_endPointStyle);

	((CButton*)GetDlgItem(IDC_CHECK_CURVE))->SetCheck(m_bCurve? 1:0);
	
	if(m_lineWidth<SXBSchDash::MIN_WIDTH) m_lineWidth=SXBSchDash::MIN_WIDTH;
	else if(m_lineWidth>SXBSchDash::MAX_WIDTH) m_lineWidth=SXBSchDash::MAX_WIDTH;

	SetDlgItemInt(IDC_EDIT_LINEWIDTH,m_lineWidth);

	if(m_endMarkSize<SXBSchDash::MIN_ENDMARKSIZE) m_endMarkSize=SXBSchDash::MIN_ENDMARKSIZE;
	else if(m_endMarkSize>SXBSchDash::MAX_ENDMARKSIZE) m_endMarkSize=SXBSchDash::MAX_ENDMARKSIZE;

	SetDlgItemInt(IDC_EDIT_ENDMARKSIZE,m_endMarkSize);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CSetDecorationLine::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	CString msg;
	CString str;
	int err=0;
	m_lineWidth=GetDlgItemInt(IDC_EDIT_LINEWIDTH);
	if(m_lineWidth<SXBSchDash::MIN_WIDTH || m_lineWidth>SXBSchDash::MAX_WIDTH){
		msg.LoadString(IDS_INVALID_LINEWIDTH);
		err++;
	}
	m_endMarkSize=GetDlgItemInt(IDC_EDIT_ENDMARKSIZE);
	if(m_endMarkSize<SXBSchDash::MIN_ENDMARKSIZE ||m_endMarkSize>SXBSchDash::MAX_ENDMARKSIZE){
		if(err) msg+="\n";
		str.LoadString(IDS_INVALID_MARKSIZE);
		msg+=str;
		err++;
	}
	if(err){
		AfxMessageBox(msg,MB_ICONEXCLAMATION|MB_OK);
		return;
	}

	m_bCurve = (((CButton*)GetDlgItem(IDC_CHECK_CURVE))->GetCheck())!=0;

	m_lineStyle = ((CComboBoxEx*)GetDlgItem(IDC_COMBOBOXEX_LINESTYLE))->GetCurSel();
	m_startPointStyle =((CComboBoxEx*)GetDlgItem(IDC_COMBOBOXEX_STARTPOINT))->GetCurSel(); 
	m_endPointStyle =((CComboBoxEx*)GetDlgItem(IDC_COMBOBOXEX_ENDPOINT))->GetCurSel(); 


	CDialog::OnOK();
}

