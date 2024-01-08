#pragma once


// CQuickProp

class CQuickProp : public CWnd
{
	DECLARE_DYNAMIC(CQuickProp)

public:
	CQuickProp();
	virtual ~CQuickProp();


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	CWnd *m_pView;
	COLORREF m_COL_BG;
	CString m_str;
	LOGFONT m_lf;
	CSize GetDrawSize();

protected:
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


