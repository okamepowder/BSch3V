/****************************************************************************
    BSch3V schematic capture
    Copyright (C) 1997-2014 H.Okada
*****************************************************************************/

#pragma once


// CFindDlg �_�C�A���O

class CFindDlg : public CDialog
{
	DECLARE_DYNAMIC(CFindDlg)

public:
	CFindDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CFindDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_FIND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:

	BOOL m_bFindComment;
	BOOL m_bFindLabel;
	BOOL m_bFindName;
	BOOL m_bFindNameLib;
	BOOL m_bFindManufacture;
	BOOL m_bFindManufacturePartNumber;
	BOOL m_bFindPackage;
	BOOL m_bFindNote;

	BOOL m_bFindNum;
	BOOL m_bFindTag;
	BOOL m_bMatchWhole;
	BOOL m_bCaseSensitive;
	CString m_strFind;
//	afx_msg void OnBnClickedFindManufacturepn();
};
