/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

// SetFont.cpp : 実装ファイル
//

#include "stdafx.h"
#include "BSch.h"
#include "SetFont.h"
#include ".\setfont.h"


// CSetFont ダイアログ

IMPLEMENT_DYNAMIC(CSetFont, CDialog)
CSetFont::CSetFont(CWnd* pParent /*=NULL*/)
	: CDialog(CSetFont::IDD, pParent)
{
	m_bChangedCommentFont = false;
	m_bChangedLabelFont = false;
	m_bChangedNameFont = false;
	m_bChangedQuickPropFont = false;
}

CSetFont::~CSetFont()
{
}

void CSetFont::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSetFont, CDialog)
	ON_BN_CLICKED(IDC_SET_COMFONT, OnBnClickedSetComfont)
	ON_BN_CLICKED(IDC_SET_LBLFONT, OnBnClickedSetLblfont)
	ON_BN_CLICKED(IDC_SET_NAMEFONT, OnBnClickedSetNamefont)
	ON_BN_CLICKED(IDC_SET_QUICKPROPFONT, &CSetFont::OnBnClickedSetQuickpropfont)
END_MESSAGE_MAP()


// CSetFont メッセージ ハンドラ

const TCHAR* CSetFont::StyleString(bool bold,bool italic)
{
	static TCHAR* styleString[4]=
	{
		_T("Regular"),
		_T("Bold"),
		_T("Italic"),
		_T("Bold-Italic")
	};
	if(!bold && !italic)return styleString[0];
	else if(bold && !italic)return styleString[1];
	else if(!bold && italic)return styleString[2];
	else return styleString[3];
}

int CSetFont::PointToPixel(int point)
{
	CWindowDC dc(this);
	return MulDiv(point, GetDeviceCaps(dc.m_hDC, LOGPIXELSY), 72);
}

int CSetFont::PixelToPoint(int pixel)
{
	if(pixel<0) pixel = -pixel;
	CWindowDC dc(this);
	return MulDiv(pixel, 72,GetDeviceCaps(dc.m_hDC, LOGPIXELSY));
}


void CSetFont::SetCommentFontInfo(LOGFONT& lf)
{
	CEdit* pEdit;
	const TCHAR* csz;
	TCHAR szSizeText[16];
	pEdit=(CEdit*)GetDlgItem(IDC_COM_FONTNAME);
	pEdit->SetWindowText(lf.lfFaceName);
	pEdit=(CEdit*)GetDlgItem(IDC_COM_FONTSTYLE);
	csz = StyleString(lf.lfWeight>=FW_BOLD,lf.lfItalic!=0);
	pEdit->SetWindowText(csz);
	pEdit=(CEdit*)GetDlgItem(IDC_COM_FONTSIZE);
	_stprintf(szSizeText,_T("%d"),PixelToPoint(lf.lfHeight));
	pEdit->SetWindowText(szSizeText);
}
void CSetFont::SetLabelFontInfo(LOGFONT& lf)
{
	CEdit* pEdit;
	const TCHAR* csz;
	TCHAR szSizeText[16];
	pEdit=(CEdit*)GetDlgItem(IDC_LBL_FONTNAME);
	pEdit->SetWindowText(lf.lfFaceName);
	pEdit=(CEdit*)GetDlgItem(IDC_LBL_FONTSTYLE);
	csz = StyleString(lf.lfWeight>=FW_BOLD,lf.lfItalic!=0);
	pEdit->SetWindowText(csz);
	pEdit=(CEdit*)GetDlgItem(IDC_LBL_FONTSIZE);
	_stprintf(szSizeText,_T("%d"),PixelToPoint(lf.lfHeight));
	pEdit->SetWindowText(szSizeText);
}

void CSetFont::SetNameFontInfo(LOGFONT& lf)
{
	CEdit* pEdit;
	const TCHAR* csz;
	TCHAR szSizeText[16];
	pEdit=(CEdit*)GetDlgItem(IDC_NAME_FONTNAME);
	pEdit->SetWindowText(lf.lfFaceName);
	pEdit=(CEdit*)GetDlgItem(IDC_NAME_FONTSTYLE);
	csz = StyleString(lf.lfWeight>=FW_BOLD,lf.lfItalic!=0);
	pEdit->SetWindowText(csz);
	pEdit=(CEdit*)GetDlgItem(IDC_NAME_FONTSIZE);
	_stprintf(szSizeText,_T("%d"),PixelToPoint(lf.lfHeight));
	pEdit->SetWindowText(szSizeText);
}


void CSetFont::SetQuickPropFontInfo(LOGFONT& lf)
{
	CEdit* pEdit;
	const TCHAR* csz;
	TCHAR szSizeText[16];
	pEdit=(CEdit*)GetDlgItem(IDC_QUICKPROP_FONTNAME);
	pEdit->SetWindowText(lf.lfFaceName);
	pEdit=(CEdit*)GetDlgItem(IDC_QUICKPROP_FONTSTYLE);
	csz = StyleString(lf.lfWeight>=FW_BOLD,lf.lfItalic!=0);
	pEdit->SetWindowText(csz);
	pEdit=(CEdit*)GetDlgItem(IDC_QUICKPROP_FONTSIZE);
	_stprintf(szSizeText,_T("%d"),PixelToPoint(lf.lfHeight));
	pEdit->SetWindowText(szSizeText);
}

BOOL CSetFont::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO :  ここに初期化を追加してください
	SetCommentFontInfo(m_lfComment);
	SetLabelFontInfo(m_lfLabel);
	SetNameFontInfo(m_lfName);
	SetQuickPropFontInfo(m_lfQuickProp);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}

void CSetFont::OnBnClickedSetComfont()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	LOGFONT lf = m_lfComment; 
	lf.lfOutPrecision = OUT_TT_ONLY_PRECIS;
	CFontDialog fontdlg(&lf,CF_TTONLY | CF_SCREENFONTS);
	if(fontdlg.DoModal()==IDOK){
		m_lfComment = lf;
		SetCommentFontInfo(m_lfComment);
		m_bChangedCommentFont = true;
	}
}

void CSetFont::OnBnClickedSetLblfont()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	LOGFONT lf = m_lfLabel; 
	lf.lfOutPrecision = OUT_TT_ONLY_PRECIS;
	CFontDialog fontdlg(&lf,CF_TTONLY | CF_SCREENFONTS);
	if(fontdlg.DoModal()==IDOK){
		m_lfLabel = lf;
		SetLabelFontInfo(m_lfLabel);
		m_bChangedLabelFont = true;
	}
}

void CSetFont::OnBnClickedSetNamefont()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	LOGFONT lf = m_lfName; 
	lf.lfOutPrecision = OUT_TT_ONLY_PRECIS;
	CFontDialog fontdlg(&lf,CF_TTONLY | CF_SCREENFONTS);
	if(fontdlg.DoModal()==IDOK){
		m_lfName = lf;
		SetNameFontInfo(m_lfName);
		m_bChangedNameFont = true;
	}
}

void CSetFont::OnBnClickedSetQuickpropfont()
{
	// TODO: ここにコントロール通知ハンドラ コードを追加します。
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	LOGFONT lf = m_lfQuickProp; 
	lf.lfOutPrecision = OUT_TT_ONLY_PRECIS;
	CFontDialog fontdlg(&lf,CF_TTONLY | CF_SCREENFONTS);
	if(fontdlg.DoModal()==IDOK){
		m_lfQuickProp = lf;
		SetQuickPropFontInfo(m_lfQuickProp);
		m_bChangedQuickPropFont = true;
	}
}
