/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// SetColor.cpp : 実装ファイル
//

#include "stdafx.h"
#include "BSch.h"
#include "SetColor.h"
#include ".\setcolor.h"


// CSetColor ダイアログ

IMPLEMENT_DYNAMIC(CSetColor, CDialog)
CSetColor::CSetColor(CWnd* pParent /*=NULL*/)
	: CDialog(CSetColor::IDD, pParent)
	, m_PRINTBG_WHITE(FALSE)
{
}

CSetColor::~CSetColor()
{
}

void CSetColor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_PRINTBG_WHITE, m_PRINTBG_WHITE);
}


BEGIN_MESSAGE_MAP(CSetColor, CDialog)
	ON_WM_CTLCOLOR()
	ON_LBN_SELCHANGE(IDC_LIST, OnLbnSelchangeList)
	ON_BN_CLICKED(IDC_CHANGE_COL, OnBnClickedChangeCol)
	ON_BN_CLICKED(IDC_DEFAULT_COLOR, OnBnClickedDefaultColor)
END_MESSAGE_MAP()


// CSetColor メッセージ ハンドラ

BOOL CSetColor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO :  ここに初期化を追加してください
	CString str;
	int index;
	CListBox* pList=(CListBox*)GetDlgItem(IDC_LIST);

	str.LoadString(IDS_COL_BG);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&m_COL_BG);	


	str.LoadString(IDS_COL_JUNC);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&m_COL_JUNC);	

	str.LoadString(IDS_COL_WIRE);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&m_COL_WIRE);

	str.LoadString(IDS_COL_BUS);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&m_COL_BUS);

	str.LoadString(IDS_COL_DASH);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&m_COL_DASH);

	str.LoadString(IDS_COL_TAG);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&m_COL_TAG);

	str.LoadString(IDS_COL_TAG_NAME);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&m_COL_TAG_NAME);

	str.LoadString(IDS_COL_COMMENT);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&m_COL_COMMENT);

	str.LoadString(IDS_COL_LABEL);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&m_COL_LABEL);

	str.LoadString(IDS_COL_XOR);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&m_COL_XOR);

	str.LoadString(IDS_COL_PART);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&m_COL_PART);

	str.LoadString(IDS_COL_PART_PIN);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&m_COL_PART_PIN);

	str.LoadString(IDS_COL_PART_PINNAME);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&m_COL_PART_PINNAME);

	str.LoadString(IDS_COL_PART_PINNUM);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&m_COL_PART_PINNUM);

	str.LoadString(IDS_COL_PART_NAME);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&m_COL_PART_NAME);

	str.LoadString(IDS_COL_PART_NUM);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&m_COL_PART_NUM);

	str.LoadString(IDS_COL_PART_FRAME);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&m_COL_PART_FRAME);

	str.LoadString(IDS_COL_SHEET_FRAME);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&m_COL_SHEET_FRAME);

	str.LoadString(IDS_COL_SHEET_GRID);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&m_COL_SHEET_GRID);

	str.LoadString(IDS_COL_ZLPIN);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&m_COL_ZLPIN);

	str.LoadString(IDS_COL_DARK);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&m_COL_DARK);

	str.LoadString(IDS_COL_NC_MARK);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&m_COL_NC_MARK);

	str.LoadString(IDS_COL_QUICKPROPVIEW);
	index = pList->AddString(str); pList->SetItemDataPtr(index,&m_COL_QUICKPROPVIEW);

	str.LoadString(IDS_COL_TEMPFRAME_FOCUSED);
	index = pList->AddString(str); pList->SetItemDataPtr(index,&m_COL_TEMPFRAME_FOCUSED);

	str.LoadString(IDS_COL_TEMPFRAME_NOT_FOCUSED);
	index = pList->AddString(str); pList->SetItemDataPtr(index,&m_COL_TEMPFRAME_NOT_FOCUSED);

	pList->SetCurSel(0);
	OnLbnSelchangeList();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

HBRUSH CSetColor::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO :  ここで DC の属性を変更してください。
	int id=pWnd->GetDlgCtrlID();
	if(nCtlColor == CTLCOLOR_STATIC && id==IDC_STATIC_COL){
		return (HBRUSH) m_brush;
	}
	// TODO :  既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}

void CSetColor::OnLbnSelchangeList()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	CListBox* pList=(CListBox*)GetDlgItem(IDC_LIST);
	int index = pList->GetCurSel();
	if(index==-1) return;
	COLORREF col_STATIC = *((COLORREF*)pList->GetItemDataPtr(index));
	m_brush.DeleteObject();
	m_brush.CreateSolidBrush(col_STATIC);
	CStatic* pStatic=(CStatic*)GetDlgItem(IDC_STATIC_COL);
	pStatic->Invalidate();
}


void CSetColor::OnBnClickedChangeCol()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	COLORREF* pColRef;
	CListBox* pList=(CListBox*)GetDlgItem(IDC_LIST);
	int index = pList->GetCurSel();
	if(index==-1) return;
	pColRef = (COLORREF*)pList->GetItemDataPtr(index);
	CColorDialog dlg;
	dlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;
	dlg.m_cc.rgbResult = *pColRef;
	if(dlg.DoModal()==IDOK){
		COLORREF col = dlg.m_cc.rgbResult;
		//if(index == 0 && col != RGB(255,255,255)){
		//	AfxMessageBox(IDS_BG_NOT_PRINT,MB_ICONEXCLAMATION|MB_OK);
		//}
		*pColRef = col;
		m_brush.DeleteObject();
		m_brush.CreateSolidBrush(*pColRef);
		CStatic* pStatic=(CStatic*)GetDlgItem(IDC_STATIC_COL);
		pStatic->Invalidate();
	}
}


void CSetColor::OnBnClickedDefaultColor()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	if(AfxMessageBox(IDS_DEFAULT_COLOR,MB_ICONEXCLAMATION|MB_OKCANCEL|MB_DEFBUTTON2)==IDOK){
		m_COL_BG			=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("BG"),COL_BG));
		m_COL_JUNC			=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("Junction"),COL_JUNC));
		m_COL_WIRE			=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("Wire"),COL_WIRE));
		m_COL_BUS			=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("Bus"),COL_BUS));
		m_COL_DASH			=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("Dash"),COL_DASH));
		m_COL_TAG			=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("Tag"),COL_TAG));
		m_COL_TAG_NAME		=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("TagName"),COL_TAG_NAME));
		m_COL_COMMENT		=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("Comment"),COL_COMMENT));
		m_COL_LABEL			=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("Label"),COL_LABEL));
		m_COL_XOR			=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("Select"),COL_XOR));
		m_COL_PART			=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("Component"),COL_PART));
		m_COL_PART_PIN		=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("ComponentPin"),COL_PART_PIN));
		m_COL_PART_PINNAME	=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("PinName"),COL_PART_PINNAME));
		m_COL_PART_PINNUM	=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("PinNum"),COL_PART_PINNUM));
		m_COL_PART_NAME		=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("ComponentName"),COL_PART_NAME));
		m_COL_PART_NUM		=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("ComponentNum"),COL_PART_NUM));
		m_COL_PART_FRAME	=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("ComponentFrame"),COL_PART_FRAME));
		m_COL_SHEET_FRAME	=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("SheetFrame"),COL_SHEET_FRAME));
		m_COL_SHEET_GRID	=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("Grid"),COL_SHEET_GRID));
		m_COL_DARK			=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("InactiveLayer"),COL_DARK));
		m_COL_NC_MARK		=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("NcMark"),COL_NC_MARK));
		m_COL_QUICKPROPVIEW	=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("QuickPropView"),COL_QUICKPROPVIEW));
		m_COL_TEMPFRAME_FOCUSED		=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("TempFrameFocused"),COL_TEMPFRAME_FOCUSED));
		m_COL_TEMPFRAME_NOT_FOCUSED	=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("TempFrameNotFocused"),COL_TEMPFRAME_NOT_FOCUSED));

/*
		m_COL_BG			=	COL_BG;
		m_COL_JUNC			=	COL_JUNC;
		m_COL_WIRE			=	COL_WIRE;
		m_COL_BUS			=	COL_BUS;
		m_COL_DASH			=	COL_DASH;
		m_COL_TAG			=	COL_TAG;
		m_COL_TAG_NAME		=	COL_TAG_NAME;
		m_COL_COMMENT		=	COL_COMMENT;
		m_COL_LABEL			=	COL_LABEL;
		m_COL_XOR			=	COL_XOR;
		m_COL_PART			=	COL_PART;
		m_COL_PART_PIN		=	COL_PART_PIN;
		m_COL_PART_PINNAME	=	COL_PART_PINNAME;	
		m_COL_PART_PINNUM	=	COL_PART_PINNUM;
		m_COL_PART_NAME		=	COL_PART_NAME;	
		m_COL_PART_NUM		=	COL_PART_NUM;
		m_COL_PART_FRAME	=	COL_PART_FRAME;
		m_COL_SHEET_FRAME	=	COL_SHEET_FRAME;
		m_COL_SHEET_GRID	=	COL_SHEET_GRID;
		m_COL_DARK			=	COL_DARK;
*/

		COLORREF colRef;
		CListBox* pList=(CListBox*)GetDlgItem(IDC_LIST);
		int index = pList->GetCurSel();
		if(index==-1) return;
		colRef = *((COLORREF*)pList->GetItemDataPtr(index));
		m_brush.DeleteObject();
		m_brush.CreateSolidBrush(colRef);
		CStatic* pStatic=(CStatic*)GetDlgItem(IDC_STATIC_COL);
		pStatic->Invalidate();
	}
}

void CSetColor::OnOK()
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。


	CDialog::OnOK();
}
