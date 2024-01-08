/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#pragma once


// CSetFont ダイアログ

class CSetFont : public CDialog
{
	DECLARE_DYNAMIC(CSetFont)

public:
	CSetFont(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CSetFont();
	LOGFONT m_lfComment;
	LOGFONT m_lfLabel;
	LOGFONT m_lfName;
	LOGFONT m_lfQuickProp;
	bool	m_bChangedCommentFont;
	bool	m_bChangedLabelFont;
	bool	m_bChangedNameFont;
	bool	m_bChangedQuickPropFont;


// ダイアログ データ
	enum { IDD = IDD_SETFONT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	void SetCommentFontInfo(LOGFONT& lf);
	void SetLabelFontInfo(LOGFONT& lf);
	void SetNameFontInfo(LOGFONT& lf);
	void SetQuickPropFontInfo(LOGFONT& lf);
	static const TCHAR* StyleString(bool bold,bool italic);
	int PointToPixel(int point);
	int PixelToPoint(int pixel);



	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedSetComfont();
	afx_msg void OnBnClickedSetLblfont();
	afx_msg void OnBnClickedSetNamefont();
	afx_msg void OnBnClickedSetQuickpropfont();
};
