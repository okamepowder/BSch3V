/****************************************************************************
    NUT3W Numbering software for BSch3V
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#pragma once


// CDdLb

class CDdLb : public CListBox
{
	DECLARE_DYNAMIC(CDdLb)

public:
	CDdLb();
	virtual ~CDdLb();

protected:

protected:
	DECLARE_MESSAGE_MAP()
public:
	void SetListboxWidth();
	afx_msg void OnDropFiles(HDROP hDropInfo);
};


