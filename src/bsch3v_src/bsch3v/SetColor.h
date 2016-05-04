/****************************************************************************
     BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#pragma once


// CSetColor ダイアログ

class CSetColor : public CDialog
{
	DECLARE_DYNAMIC(CSetColor)

public:
	COLORREF m_COL_BG;
	COLORREF m_COL_JUNC;
	COLORREF m_COL_WIRE;
	COLORREF m_COL_BUS;
	COLORREF m_COL_DASH;
	COLORREF m_COL_TAG;
	COLORREF m_COL_TAG_NAME;
	COLORREF m_COL_COMMENT;
	COLORREF m_COL_LABEL;
	COLORREF m_COL_XOR;
	COLORREF m_COL_PART;
	COLORREF m_COL_PART_PIN;
	COLORREF m_COL_PART_PINNAME;
	COLORREF m_COL_PART_PINNUM;
	COLORREF m_COL_PART_NAME;
	COLORREF m_COL_PART_NUM;
	COLORREF m_COL_PART_FRAME;
	COLORREF m_COL_SHEET_FRAME;
	COLORREF m_COL_SHEET_GRID;
	COLORREF m_COL_DARK;
	COLORREF m_COL_ZLPIN;	//ZeroLengthPin
	COLORREF m_COL_NC_MARK;
	COLORREF m_COL_QUICKPROPVIEW;
	COLORREF m_COL_TEMPFRAME_FOCUSED;
	COLORREF m_COL_TEMPFRAME_NOT_FOCUSED;


	
	CSetColor(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CSetColor();

// ダイアログ データ
	enum { IDD = IDD_SETCOLOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	//COLORREF m_col_STATIC;
	CBrush m_brush;
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLbnSelchangeList();
	afx_msg void OnBnClickedChangeCol();
	afx_msg void OnBnClickedDefaultColor();
protected:
	virtual void OnOK();
public:
	BOOL m_PRINTBG_WHITE;
};
