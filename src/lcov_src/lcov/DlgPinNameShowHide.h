#pragma once
#include "afxwin.h"


// CDlgPinNameShowHide �_�C�A���O

class CDlgPinNameShowHide : public CDialog
{
	DECLARE_DYNAMIC(CDlgPinNameShowHide)

public:
	CDlgPinNameShowHide(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CDlgPinNameShowHide();


// �_�C�A���O �f�[�^
	enum { IDD = IDD_DISPNAMESETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	CCheckListBox m_listNames;

public:
	CString m_strName;
	unsigned long m_displayFlag;

protected:
	int parseName(CStringList &r_nameList);

public:
	virtual BOOL OnInitDialog();
//	afx_msg void OnBnClickedButtondispAll();
	afx_msg void OnBnClickedButtonshowAll();
	afx_msg void OnBnClickedButtonhideAll();
	virtual void OnOK();
};
