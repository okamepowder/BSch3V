/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// SetColor2.cpp : 実装ファイル
//

#include "stdafx.h"
#include "BSch.h"
#include "SetColor2.h"
#include ".\setcolor2.h"


// CSetColor ダイアログ

IMPLEMENT_DYNAMIC(CSetColor2, CDialog)
CSetColor2::CSetColor2(CWnd* pParent /*=NULL*/)
	: CDialog(CSetColor2::IDD, pParent)
	, m_PRINTBG_WHITE(FALSE)
{
	m_pColor = NULL;
}

CSetColor2::~CSetColor2()
{
}

void CSetColor2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_PRINTBG_WHITE, m_PRINTBG_WHITE);
}


BEGIN_MESSAGE_MAP(CSetColor2, CDialog)
	ON_WM_CTLCOLOR()
	ON_LBN_SELCHANGE(IDC_LIST, OnLbnSelchangeList)
	ON_BN_CLICKED(IDC_CHANGE_COL, OnBnClickedChangeCol)
	ON_BN_CLICKED(IDC_DEFAULT_COLOR, OnBnClickedDefaultColor)
END_MESSAGE_MAP()


// CSetColor メッセージ ハンドラ

BOOL CSetColor2::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_pColor==NULL) return TRUE;

	m_ColorBack = *m_pColor;

	// TODO :  ここに初期化を追加してください
	CString str;
	int index;
	CListBox* pList=(CListBox*)GetDlgItem(IDC_LIST);

	str.LoadString(IDS_COL_BG);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&(m_pColor->colBG));	


	str.LoadString(IDS_COL_JUNC);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&(m_pColor->colJUNC));	

	str.LoadString(IDS_COL_WIRE);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&(m_pColor->colWIRE));

	str.LoadString(IDS_COL_BUS);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&(m_pColor->colBUS));

	str.LoadString(IDS_COL_DASH);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&(m_pColor->colDASH));

	str.LoadString(IDS_COL_TAG);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&(m_pColor->colTAG));

	str.LoadString(IDS_COL_TAG_NAME);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&(m_pColor->colTAG_NAME));

	str.LoadString(IDS_COL_COMMENT);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&(m_pColor->colCOMMENT));

	str.LoadString(IDS_COL_LABEL);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&(m_pColor->colLABEL));

	str.LoadString(IDS_COL_XOR);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&(m_pColor->colXOR));

	str.LoadString(IDS_COL_PART);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&(m_pColor->colPART));

	str.LoadString(IDS_COL_PART_PIN);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&(m_pColor->colPART_PIN));

	str.LoadString(IDS_COL_PART_PINNAME);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&(m_pColor->colPART_PINNAME));

	str.LoadString(IDS_COL_PART_PINNUM);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&(m_pColor->colPART_PINNUM));

	str.LoadString(IDS_COL_PART_NAME);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&(m_pColor->colPART_NAME));

	str.LoadString(IDS_COL_PART_NUM);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&(m_pColor->colPART_NUM));

	str.LoadString(IDS_COL_PART_FRAME);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&(m_pColor->colPART_FRAME));

	str.LoadString(IDS_COL_SHEET_FRAME);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&(m_pColor->colSHEET_FRAME));

	str.LoadString(IDS_COL_SHEET_GRID);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&(m_pColor->colSHEET_GRID));

	str.LoadString(IDS_COL_ZLPIN);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&(m_pColor->colZLPIN));

	str.LoadString(IDS_COL_DARK);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&(m_pColor->colDARK));

	str.LoadString(IDS_COL_NC_MARK);
	index = pList->AddString(str);	pList->SetItemDataPtr(index,&(m_pColor->colNC_MARK));

	str.LoadString(IDS_COL_QUICKPROPVIEW);
	index = pList->AddString(str); pList->SetItemDataPtr(index,&(m_pColor->colQUICKPROPVIEW));

	str.LoadString(IDS_COL_TEMPFRAME_FOCUSED);
	index = pList->AddString(str); pList->SetItemDataPtr(index,&(m_pColor->colTEMPFRAME_FOCUSED));

	str.LoadString(IDS_COL_TEMPFRAME_NOT_FOCUSED);
	index = pList->AddString(str); pList->SetItemDataPtr(index,&(m_pColor->colTEMPFRAME_NOT_FOCUSED));

	str.LoadString(IDS_COL_FIND_MARK);
	index = pList->AddString(str); pList->SetItemDataPtr(index,&(m_pColor->colFIND_MARK));


	pList->SetCurSel(0);
	OnLbnSelchangeList();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

HBRUSH CSetColor2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CSetColor2::OnLbnSelchangeList()
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


void CSetColor2::OnBnClickedChangeCol()
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
		m_pParentWnd->Invalidate();
	}
}


void CSetColor2::OnBnClickedDefaultColor()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	if(AfxMessageBox(IDS_DEFAULT_COLOR,MB_ICONEXCLAMATION|MB_OKCANCEL|MB_DEFBUTTON2)==IDOK){
		m_pColor->colBG				=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("BG"),COL_BG));
		m_pColor->colJUNC			=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("Junction"),COL_JUNC));
		m_pColor->colWIRE			=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("Wire"),COL_WIRE));
		m_pColor->colBUS			=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("Bus"),COL_BUS));
		m_pColor->colDASH			=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("Dash"),COL_DASH));
		m_pColor->colTAG			=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("Tag"),COL_TAG));
		m_pColor->colTAG_NAME		=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("TagName"),COL_TAG_NAME));
		m_pColor->colCOMMENT		=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("Comment"),COL_COMMENT));
		m_pColor->colLABEL			=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("Label"),COL_LABEL));
		m_pColor->colXOR			=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("Select"),COL_XOR));
		m_pColor->colPART			=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("Component"),COL_PART));
		m_pColor->colPART_PIN		=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("ComponentPin"),COL_PART_PIN));
		m_pColor->colPART_PINNAME	=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("PinName"),COL_PART_PINNAME));
		m_pColor->colPART_PINNUM	=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("PinNum"),COL_PART_PINNUM));
		m_pColor->colPART_NAME		=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("ComponentName"),COL_PART_NAME));
		m_pColor->colPART_NUM		=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("ComponentNum"),COL_PART_NUM));
		m_pColor->colPART_FRAME		=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("ComponentFrame"),COL_PART_FRAME));
		m_pColor->colSHEET_FRAME	=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("SheetFrame"),COL_SHEET_FRAME));
		m_pColor->colSHEET_GRID		=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("Grid"),COL_SHEET_GRID));
		m_pColor->colDARK			=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("InactiveLayer"),COL_DARK));
		m_pColor->colNC_MARK		=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("NcMark"),COL_NC_MARK));
		m_pColor->colQUICKPROPVIEW	=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("QuickPropView"),COL_QUICKPROPVIEW));
		m_pColor->colTEMPFRAME_FOCUSED		=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("TempFrameFocused"),COL_TEMPFRAME_FOCUSED));
		m_pColor->colTEMPFRAME_NOT_FOCUSED	=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("TempFrameNotFocused"),COL_TEMPFRAME_NOT_FOCUSED));
		m_pColor->colFIND_MARK		=(COLORREF)(AfxGetApp()->GetProfileInt(_T("DefaultColor"),_T("FindMark"),COL_FIND_MARK));

		COLORREF colRef;
		CListBox* pList=(CListBox*)GetDlgItem(IDC_LIST);
		int index = pList->GetCurSel();
		if(index==-1) return;
		colRef = *((COLORREF*)pList->GetItemDataPtr(index));
		m_brush.DeleteObject();
		m_brush.CreateSolidBrush(colRef);
		CStatic* pStatic=(CStatic*)GetDlgItem(IDC_STATIC_COL);
		pStatic->Invalidate();

		m_pParentWnd->Invalidate();
	}
}

void CSetColor2::OnOK()
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。


	CDialog::OnOK();
}

void CSetColor2::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	*m_pColor = m_ColorBack;
	CDialog::OnCancel();
}
