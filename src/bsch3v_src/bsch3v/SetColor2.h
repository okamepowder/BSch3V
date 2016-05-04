/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#pragma once

#include "BSchDrawColor.h"


// CSetColor2 �_�C�A���O

class CSetColor2 : public CDialog
{
	DECLARE_DYNAMIC(CSetColor2)

public:
	SBSchDrawColor* m_pColor;

protected:
	SBSchDrawColor m_ColorBack;


public:
	
	CSetColor2(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CSetColor2();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_SETCOLOR2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

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
protected:
	virtual void OnCancel();
};
