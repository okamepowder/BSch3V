#pragma once


// CEditForGrid

#define WM_FIX_TEXT (WM_APP+1)
class CEditForGrid : public CEdit
{
	DECLARE_DYNAMIC(CEditForGrid)

public:
	CEditForGrid();
	virtual ~CEditForGrid();

protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};


