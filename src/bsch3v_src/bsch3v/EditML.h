#pragma once


// CEditML

class CEditML : public CEdit
{
	DECLARE_DYNAMIC(CEditML)

public:
	CEditML();
	virtual ~CEditML();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};


