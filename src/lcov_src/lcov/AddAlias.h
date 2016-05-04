/****************************************************************************
    LCoV library editor for BSch3V
    Copyright (C) 2004-2014 H.Okada (http://www.suigyodo.com/online)
*****************************************************************************/

#pragma once


// CAddAlias �_�C�A���O

class CAddAlias : public CDialog
{
	DECLARE_DYNAMIC(CAddAlias)

public:
	CAddAlias(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CAddAlias();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_ALIAS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	CString m_strName;
	CString m_strAlias;
};
